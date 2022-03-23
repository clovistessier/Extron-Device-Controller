#include "USP405.h"


USP_405::USP_405()
{
    start_position_x = 0;
    start_position_y = 0;
    size_x = 0;
    size_y = 0;
    midi_channel = 2;
    maxSisMessages = 10;
    
    osc_addr_map =
    {
        {"2/pos_x", parameter_list::position_x_mod},
        {"2/pos_y", parameter_list::position_y_mod},
        {"2/scale", parameter_list::scale_mod},
        {"2/brightness", parameter_list::brightness_mod},
        {"2/contrast", parameter_list::contrast_mod},
        {"2/color", parameter_list::color_mod},
        {"2/tint", parameter_list::tint_mod},
        {"2/detail", parameter_list::detail_mod}
    };
}

void USP_405::setup()
{
    // position range -> needs tuning
    int pr = 100;
    int npr = -1*pr;
    // assign groups
    position_x_group.setName("POSITION X");
    position_x_group.add(position_x.set("position_x", 0, npr, pr));
    position_x_group.add(position_x_mod.set("mod", 0, npr, pr));
    position_x_group.add(position_x_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    
    position_y_group.setName("POSITION Y");
    position_y_group.add(position_y.set("position_y", 0, npr, pr));
    position_y_group.add(position_y_mod.set("mod", 0, npr, pr));
    position_y_group.add(position_y_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    
    
    scale_group.setName("SCALE");
    scale_follower = 1.0f;
    scale_group.add(scale.set("scale", 1.0f, 0.5f, 1.5f));
    scale_group.add(scale_mod.set("mod", 0.0f, -1.0f, 1.0f));
    scale_group.add(scale_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    scale_group.add(smoothing.set("smoothing", 0.9f, 0.0f, 1.0f));
    
    // size range
    int sr = 100;
    int nsr = -1*sr;
    size_x_group.setName("SIZE X");
    size_x_group.add(size_x.set("size x", 0, nsr, sr));
    size_x_group.add(size_x_mod.set("mod", 0, nsr, sr));
    size_x_group.add(size_x_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    
    
    size_y_group.setName("SIZE Y");
    size_y_group.add(size_y.set("size y", 0, nsr, sr));
    size_y_group.add(size_y_mod.set("mod", 0, -1000, 1000));
    size_y_group.add(size_y_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    
    
    brightness_group.setName("BRIGHTNESS");
    brightness_group.add(brightness.set("brightness", 32, 0, 63));
    brightness_group.add(brightness_mod.set("mod", 0, -63, 63));
    brightness_group.add(brightness_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    
    
    contrast_group.setName("CONTRAST");
    contrast_group.add(contrast.set("contrast", 128, 0, 255));
    contrast_group.add(contrast_mod.set("mod", 0, -255, 255));
    contrast_group.add(contrast_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    
    
    color_group.setName("COLOR");
    color_group.add(color.set("color", 64, 0, 127));
    color_group.add(color_mod.set("mod", 0, -127, 127));
    color_group.add(color_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    
    
    tint_group.setName("TINT");
    tint_group.add(tint.set("tint", 128, 0, 255));
    tint_group.add(tint_mod.set("mod", 0, -255, 255));
    tint_group.add(tint_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    tint_follower = tint.get();
    
    
    detail_group.setName("DETAIL");
    detail_group.add(detail.set("detail", 0, 0, 7));
    detail_group.add(detail_mod.set("mod", 0, -7, 7));
    detail_group.add(detail_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    
    geometry.setName("Geometry");
    geometry.add(position_x_group);
    geometry.add(position_y_group);
    geometry.add(scale_group);
    geometry.add(size_x_group);
    geometry.add(size_y_group);
    
    picture_controls.setName("Picture Controls");
    picture_controls.add(brightness_group);
    picture_controls.add(contrast_group);
    picture_controls.add(color_group);
    picture_controls.add(tint_group);
    picture_controls.add(detail_group);
    
    parameters.setName("USP 405");
    parameters.add(geometry);
    parameters.add(picture_controls);
    
    // add listeners
    addListeners();
    
    
    // init Serial connection
    serial.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    int baud = 9600;
    serial.setup("/dev/tty.usbserial-214410", baud);
    
    ofBuffer buffer;
    buffer.append("4!");
    sisMessages.push_back(buffer);
}

void USP_405::update()
{
    ofBuffer rbuffer;
    
    for (int i = 0; i < 1; i++)
    {
        seekGeometryTargets();
    }
    // empty the queue of sisMessages
    while (!sisMessages.empty())
    {
        cout << sisMessages.front().getText() << "\t";
        long bytesWritten = serial.writeBytes(sisMessages.front());
        cout << bytesWritten << endl;
        sisMessages.pop_front();
        
        //        ofBuffer wbuffer = sisMessages.front();
        //        auto it = wbuffer.begin();
        //        while (it != wbuffer.end())
        //        {
        //            serial.writeByte(*it);
        //            ofSleepMillis(5);
        //            it++;
        //        }
        //        sisMessages.pop_front();
        
        // Listen for the response before sending another message
        // All responses from the device end with carriage return character
        rbuffer.clear();
        while (serial.available() > 0)
        {
            serial.readBytes(rbuffer, 1);
            cout << rbuffer.getText();
            if (*rbuffer.getData() == cr)
            {
                cout << endl;
            }
            rbuffer.clear();
        }
    }
    
    // check for incoming messages from the device
    // i.e. from turning the knobs
    rbuffer.clear();
    while (serial.available() > 0)
    {
        serial.readBytes(rbuffer, 1);
        cout << rbuffer.getText();
        if (*rbuffer.getData() == cr)
        {
            cout << endl;
        }
        rbuffer.clear();
    }
    
    
    // scale is particularaly jerky
    // scale_follower holds a smoothed out version using ofLerp
    if (abs(scale_follower - scale.get()) > 0.01)
    {
        updateGeometry(geometry_flag::scale);
    }
    
    if (tint_follower != tint.get())
    {
        update_tint();
    }
}

void USP_405::adjustCentering(Direction dir)
{
    switch (dir)
    {
        case UP: sendSIScommand(1, sisCommand_t::position_y); break;
        case DOWN: sendSIScommand(-1, sisCommand_t::position_y); break;
        case LEFT: sendSIScommand(-1, sisCommand_t::position_x); break;
        case RIGHT: sendSIScommand(1, sisCommand_t::position_x); break;
    }
}

void USP_405::updateGeometry(geometry_flag g)
{
    // USP 405 has no ability to set the horizontal / vertial
    // centering to a specific value, nor the h/v size.
    int delta;
    int inc;
    
    switch (g)
    {
        case geometry_flag::pos_x:
            target_pos_x = position_x + (position_x_mod * position_x_mod_depth);
            target_pos_x = ofClamp(target_pos_x, position_x.getMin(), position_x.getMax());
            //            delta = target_pos_x - pos_x_val;
            //            inc = (delta > 0 ? 1 : -1);
            //            for (int i = 0; i < abs(delta); i++)
            //            {
            //                sendSIScommand(inc, sisCommand_t::position_x);
            //            }
            //            pos_x_val = target_pos_x;
            break;
            
        case geometry_flag::pos_y:
            target_pos_y = position_y + (position_y_mod * position_y_mod_depth);
            target_pos_y = ofClamp(target_pos_y, position_y.getMin(), position_y.getMax());
            break;
    }
}

void USP_405::sendSIScommand(int val, sisCommand_t type)
{
    ofBuffer buffer;
    // No special characters are required to begin or end a command
    // sequence, but I will start each with an escape key.
    //    buffer.append(&esc, 1);
    if (isGeometryCommand(type))
    {
        switch (val)
        {
            case 1: buffer.append("+");
                break;
            case -1: buffer.append("-");
                break;
        }
    }
    else
    {
        buffer.append(int2str(val));
    }
    buffer.append(SIS_map[type]);
    
    // shouldn't require any special characters to end
    //    buffer.append(&cr, 1);
    if(sisMessages.size() < maxSisMessages)
    {
        sisMessages.push_back(buffer);
    }
}

void USP_405::update_tint()
{
    // tint value is stored with 8 bits on the USP 405
    // but MIDI CCs are only 7 bits.
    // so we will set a target using the midi and then ofLerp to it
    // for smoother motion
    int tint_follower_pre = tint_follower;
    
    int diff_pre = tint_follower - tint_val;
    
    applyModulation(parameter_list::tint, tint_val);
    tint_follower = static_cast<int>(ofLerp(tint_follower, tint_val, 0.5));
    
    int diff_post = tint_follower - tint_val;
    if (diff_pre == diff_post)
    {
        tint_follower = tint_val;
    }
    
    // only put an SiS message in the queue if the
    // value is actually changed
    if (tint_follower != tint_follower_pre)
    {
        sendSIScommand(tint_follower, sisCommand_t::tint);
    }
}

bool USP_405::isGeometryCommand(sisCommand_t type)
{
    return (geometry_commands.find(type) != geometry_commands.end());
}

void USP_405::seekGeometryTargets()
{
    seekGeometryParam(target_pos_x, pos_x_val, sisCommand_t::position_x);
    seekGeometryParam(target_pos_y, pos_y_val, sisCommand_t::position_y);
}

void USP_405::seekGeometryParam(int target, int &val, sisCommand_t type)
{
    if (sisMessages.size() > maxSisMessages) {return;}
    int delta;
    delta = target - val;
    if (delta > 0)
    {
        sendSIScommand(1, type);
        val++;
    } else if (delta < 0)
    {
        sendSIScommand(-1, type);
        val--;
    }
}
