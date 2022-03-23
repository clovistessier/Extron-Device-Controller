#include "Extron.h"


ExtronDevice::ExtronDevice()
{
    start_position_x = 240;
    start_position_y = 0;
    start_size_x = 1440;
    start_size_y = 1080;
    midi_channel = 1;
    maxSisMessages = 10;
    
    osc_addr_map =
    {
        {"1/pos_x", parameter_list::position_x_mod},
        {"1/pos_y", parameter_list::position_y_mod},
        {"1/scale", parameter_list::scale_mod},
        {"1/brightness", parameter_list::brightness_mod},
        {"1/contrast", parameter_list::contrast_mod},
        {"1/color", parameter_list::color_mod},
        {"1/tint", parameter_list::tint_mod},
        {"1/detail", parameter_list::detail_mod}
    };
}

void ExtronDevice::setup()
{
        // assign groups
    position_x_group.setName("POSITION X");
    position_x_group.add(position_x.set("position_x", 0, -500, 500));
    position_x_group.add(position_x_mod.set("mod", 0, -500, 500));
    position_x_group.add(position_x_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));

    position_y_group.setName("POSITION Y");
    position_y_group.add(position_y.set("position_y", 0, -500, 500));
    position_y_group.add(position_y_mod.set("mod", 0, -500, 500));
    position_y_group.add(position_y_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));


    scale_group.setName("SCALE");
    scale_follower = 1.0f;
    scale_group.add(scale.set("scale", 1.0f, 0.5f, 1.5f));
    scale_group.add(scale_mod.set("mod", 0.0f, -1.0f, 1.0f));
    scale_group.add(scale_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));
    scale_group.add(smoothing.set("smoothing", 0.9f, 0.0f, 1.0f));
    

    size_x_group.setName("SIZE X");
    size_x_group.add(size_x.set("size x", 0, -500, 500));
    size_x_group.add(size_x_mod.set("mod", 0, -1000, 1000));
    size_x_group.add(size_x_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));


    size_y_group.setName("SIZE Y");
    size_y_group.add(size_y.set("size y", 0, -500, 500));
    size_y_group.add(size_y_mod.set("mod", 0, -1000, 1000));
    size_y_group.add(size_y_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));


    brightness_group.setName("BRIGHTNESS");
    brightness_group.add(brightness.set("brightness", 64, 0, 127));
    brightness_group.add(brightness_mod.set("mod", 0, -127, 127));
    brightness_group.add(brightness_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));


    contrast_group.setName("CONTRAST");
    contrast_group.add(contrast.set("contrast", 64, 0, 127));
    contrast_group.add(contrast_mod.set("mod", 0, -127, 127));
    contrast_group.add(contrast_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));


    color_group.setName("COLOR");
    color_group.add(color.set("color", 64, 0, 127));
    color_group.add(color_mod.set("mod", 0, -127, 127));
    color_group.add(color_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));


    tint_group.setName("TINT");
    tint_group.add(tint.set("tint", 64, 0, 127));
    tint_group.add(tint_mod.set("mod", 0, -127, 127));
    tint_group.add(tint_mod_depth.set("mod depth", 0.0f, -1.0f, 1.0f));


    detail_group.setName("DETAIL");
    detail_group.add(detail.set("detail", 64, 0, 127));
    detail_group.add(detail_mod.set("mod", 0, -127, 127));
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

    parameters.setName("Extron Device");
    parameters.add(geometry);
    parameters.add(picture_controls);

    // add listeners
    addListeners();


    // init Serial connection
    serial.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    int baud = 9600;
    serial.setup("/dev/tty.usbmodem2144201", baud);
}

void ExtronDevice::addListeners()
{
    position_x.addListener(this, &ExtronDevice::position_xChanged<int>);
    position_x_mod.addListener(this, &ExtronDevice::position_xChanged<int>);
    position_x_mod_depth.addListener(this, &ExtronDevice::position_xChanged<float>);

    position_y.addListener(this, &ExtronDevice::position_yChanged<int>);
    position_y_mod.addListener(this, &ExtronDevice::position_yChanged<int>);
    position_y_mod_depth.addListener(this, &ExtronDevice::position_yChanged<float>);

    scale.addListener(this, &ExtronDevice::scaleChanged<float>);
    scale_mod.addListener(this, &ExtronDevice::scaleChanged<float>);
    scale_mod_depth.addListener(this, &ExtronDevice::scaleChanged<float>);

    size_x.addListener(this, &ExtronDevice::size_xChanged<int>);
    size_x_mod.addListener(this, &ExtronDevice::size_xChanged<int>);
    size_x_mod_depth.addListener(this, &ExtronDevice::size_xChanged<float>);

    size_y.addListener(this, &ExtronDevice::size_yChanged<int>);
    size_y_mod.addListener(this, &ExtronDevice::size_yChanged<int>);
    size_y_mod_depth.addListener(this, &ExtronDevice::size_yChanged<float>);

    brightness.addListener(this, &ExtronDevice::brightnessChanged<int>);
    brightness_mod.addListener(this, &ExtronDevice::brightnessChanged<int>);
    brightness_mod_depth.addListener(this, &ExtronDevice::brightnessChanged<float>);

    contrast.addListener(this, &ExtronDevice::contrastChanged<int>);
    contrast_mod.addListener(this, &ExtronDevice::contrastChanged<int>);
    contrast_mod_depth.addListener(this, &ExtronDevice::contrastChanged<float>);

    color.addListener(this, &ExtronDevice::colorChanged<int>);
    color_mod.addListener(this, &ExtronDevice::colorChanged<int>);
    color_mod_depth.addListener(this, &ExtronDevice::colorChanged<float>);

    tint.addListener(this, &ExtronDevice::tintChanged<int>);
    tint_mod.addListener(this, &ExtronDevice::tintChanged<int>);
    tint_mod_depth.addListener(this, &ExtronDevice::tintChanged<float>);

    detail.addListener(this, &ExtronDevice::detailChanged<int>);
    detail_mod.addListener(this, &ExtronDevice::detailChanged<int>);
    detail_mod_depth.addListener(this, &ExtronDevice::detailChanged<float>);
}

void ExtronDevice::update()
{
    // empty the queue of sisMessages
    while (!sisMessages.empty())
    {
        serial.writeBytes(sisMessages.front());
        sisMessages.pop_front();
        
        // check for response
        ofBuffer buffer;
        while (serial.available() > 0)
        {
            serial.readBytes(buffer, 1);
            cout << buffer.getText();
            if (*buffer.getData() == cr)
            {
                cout << endl;
            }
            buffer.clear();
        }
    }

    // scale is particularaly jerky
    // scale_follower holds a smoothed out version using ofLerp
    // margin of 0.01 since ofLerp will never make the two values
    // actually equal
    if (abs(scale_follower - scale.get()) > 0.01)
    {
        updateGeometry(geometry_flag::scale);
    }
}

bool ExtronDevice::isMidiCC(int cc)
{
    return (midi_cc_map.find(cc) != midi_cc_map.end());
}

void ExtronDevice::updateFromMidi(int cc, int value)
{
    if (!isMidiCC(cc)) {return;}
    parameter_list p = midi_cc_map.at(cc);

//    int group_num = getGroupNum(p);
//    int param_num = getParamNum(p);
//    ofParameterGroup param = parameters.getGroup(group_num).getGroup(param_num);
    
    string group_name = getGroupName(p);
    string param_name = getParamName(p);
    ofParameterGroup param = parameters.getGroup(group_name).getGroup(param_name);

    bool isFloat = (p == parameter_list::scale);

//    int param_index = static_cast<int>(p);

//    if (param_index % 3 == 0) // parameter value
    parameter_type_t t = param_type_map[p];
    if (t == parameter_type_t::value)
    {
        if (isFloat == true)
        {
            float min = param.getFloat(0).getMin();
            float max = param.getFloat(0).getMax();
            float new_val = ofMap(value, 0, 127, min, max, true);
            param.getFloat(0).set(new_val);
        }
        else
        {
            int min = param.getInt(0).getMin();
            int max = param.getInt(0).getMax();
            int new_val = ofMap(value, 0, 127, min, max, true);
            param.getInt(0).set(new_val);
        }
    }
//    if (param_index % 3 == 2) // mod depth
    else if (t == parameter_type_t::mod_depth)
    {
        float new_value = ofMap(value, 0, 127, -1.0f, 1.0f);
        // floating point math ends up mapping 64 -> 0.00787401
        // so we are rounding to zero to actually turn off modulation
        if (abs(new_value) < 0.01) {new_value = 0.0;}
        param.getFloat("mod depth").set(new_value);
    }
}

bool ExtronDevice::isOscAddress(const string &addr)
{
    return (osc_addr_map.find(addr) != osc_addr_map.end());
}

void ExtronDevice::updateFromOsc(const string &addr, float val)
{
    if (!isOscAddress(addr)) {return;}
    parameter_list p = osc_addr_map.at(addr);
    
//    int group_num = getGroupNum(p);
//    int param_num = getParamNum(p);
//    ofParameterGroup param = parameters.getGroup(group_num).getGroup(param_num);
    
    string group_name = getGroupName(p);
    string param_name = getParamName(p);
    ofParameterGroup param = parameters.getGroup(group_name).getGroup(param_name);

    bool isFloat = (param_name == "SCALE");

//    int param_index = static_cast<int>(p);

//    if (param_index % 3 == 1) // mod
//    {
        if (isFloat == true)
        {
            float min = param.getFloat("mod").getMin();
            float max = param.getFloat("mod").getMax();
            float new_val = ofMap(val, 0.0f, 1.0f, min, max, true);
            param.getFloat("mod").set(new_val);
        }
        else
        {
            int min = param.getInt("mod").getMin();
            int max = param.getInt("mod").getMax();
            int new_val = ofMap(val, 0.0f, 1.0f, min, max, true);
            param.getInt("mod").set(new_val);
        }
//    }
}

int ExtronDevice::getMidiChannel()
{
    return midi_channel;
}

void ExtronDevice::updateGeometry(geometry_flag g)
{
    // There's no actual scale setting on the DSC301 HD
    // Instead, we can change size_x and size_y to change the size
    // of the image.
    // This scales the image from the top left corner, so to preserve
    // the centering of the image, we also have to change position_x and
    // position_y

    switch (g)
    {
    case geometry_flag::scale:
        scale_val = scale + (scale_mod * scale_mod_depth);
        scale_val = ofClamp(scale_val, scale.getMin(), scale.getMax());

        scale_follower = ofLerp(scale_follower, scale_val, 1.0f - smoothing.get());

        anchor_size_x = static_cast<int>(scale_follower * start_size_x);
        anchor_size_y = static_cast<int>(scale_follower * start_size_y);
        anchor_position_x = start_position_x - ((anchor_size_x - start_size_x) / 2);
        anchor_position_y = start_position_y - ((anchor_size_y - start_size_y) / 2);
        break;

    case geometry_flag::pos_x:
        pos_x_val = position_x + (position_x_mod * position_x_mod_depth);
        pos_x_val = ofClamp(pos_x_val, position_x.getMin(), position_x.getMax());
        break;

    case geometry_flag::pos_y:
        pos_y_val = position_y + (position_y_mod * position_y_mod_depth);
        pos_y_val = ofClamp(pos_y_val, position_y.getMin(), position_y.getMax());
        break;

    case geometry_flag::size_x:
        size_x_val = size_x + (size_x_mod * size_x_mod_depth);
        size_x_val = ofClamp(size_x_val, size_x.getMin(), size_x.getMax());
        break;

    case geometry_flag::size_y:
        size_y_val = size_y + (size_y_mod * size_y_mod_depth);
        size_y_val = ofClamp(size_y_val, size_y.getMin(), size_y.getMax());
        break;
    }

    // The anchor values are used to calculate the actual size and position

    int new_size_x = anchor_size_x + size_x_val;
    int new_size_y = anchor_size_y + size_y_val;
    int new_pos_x = anchor_position_x + pos_x_val;
    int new_pos_y = anchor_position_y + pos_y_val;

    sendSIScommand(new_pos_x, new_pos_y, new_size_x, new_size_y);
}

//void ExtronDevice::position_xChanged_i(int &new_val)
//{
//    updateGeometry(geometry_flag::pos_x);
//}
//
//void ExtronDevice::position_xChanged_f(float &new_val)
//{
//    updateGeometry(geometry_flag::pos_x);
//}
//
//void ExtronDevice::position_yChanged_i(int &new_val)
//{
//    updateGeometry(geometry_flag::pos_y);
//}
//void ExtronDevice::position_yChanged_f(float &new_val)
//{
//    updateGeometry(geometry_flag::pos_y);
//}
//
//void ExtronDevice::scaleChanged_f(float &new_val)
//{
//    updateGeometry(geometry_flag::scale);
//}
//
//void ExtronDevice::size_xChanged_i(int &new_val)
//{
//    updateGeometry(geometry_flag::size_x);
//}
//void ExtronDevice::size_xChanged_f(float &new_val)
//{
//    updateGeometry(geometry_flag::size_x);
//}
//
//void ExtronDevice::size_yChanged_i(int &new_val)
//{
//    updateGeometry(geometry_flag::size_y);
//}
//void ExtronDevice::size_yChanged_f(float &new_val)
//{
//    updateGeometry(geometry_flag::size_y);
//}
//
//void ExtronDevice::brightnessChanged_i(int &new_val)
//{
//    update_brightness();
//}
//void ExtronDevice::brightnessChanged_f(float &new_val)
//{
//    update_brightness();
//}
//
//void ExtronDevice::contrastChanged_i(int &new_val)
//{
//    update_contrast();
//}
//void ExtronDevice::contrastChanged_f(float &new_val)
//{
//    update_contrast();
//}
//
//void ExtronDevice::colorChanged_i(int &new_val)
//{
//    update_color();
//}
//void ExtronDevice::colorChanged_f(float &new_val)
//{
//    update_color();
//}
//
//void ExtronDevice::tintChanged_i(int &new_val)
//{
//    update_tint();
//}
//void ExtronDevice::tintChanged_f(float &new_val)
//{
//    update_tint();
//}
//
//void ExtronDevice::detailChanged_i(int &new_val)
//{
//    update_detail();
//}
//void ExtronDevice::detailChanged_f(float &new_val)
//{
//    update_detail();
//}

void ExtronDevice::update_brightness()
{
    int pre = brightness_val;
    applyModulation(parameter_list::brightness, brightness_val);
    if (brightness_val != pre)
    {
        sendSIScommand(brightness_val, sisCommand_t::brightness);
    }
}
void ExtronDevice::update_contrast()
{
    int pre = contrast_val;
    applyModulation(parameter_list::contrast, contrast_val);
    if (contrast_val != pre)
    {
        sendSIScommand(contrast_val, sisCommand_t::contrast);
    }
}
void ExtronDevice::update_color()
{
    int pre = color_val;
    applyModulation(parameter_list::color, color_val);
    if (color_val != pre)
    {
        sendSIScommand(color_val, sisCommand_t::color);
    }
}
void ExtronDevice::update_tint()
{
    int pre = tint_val;
    applyModulation(parameter_list::tint, tint_val);
    if (tint_val != pre)
    {
        sendSIScommand(tint_val, sisCommand_t::tint);
    }
}
void ExtronDevice::update_detail()
{
    int pre = detail_val;
    applyModulation(parameter_list::detail, detail_val);
    if (detail_val != pre)
    {
        sendSIScommand(detail_val, sisCommand_t::detail);
    }
}

void ExtronDevice::applyModulation(parameter_list p, int &result)
{
//    int group_num = getGroupNum(p);
//    int param_num = getParamNum(p);
//
//    ofParameterGroup param = parameters.getGroup(group_num).getGroup(param_num);
    
    string group_name = getGroupName(p);
    string param_name = getParamName(p);
    
    ofParameterGroup param = parameters.getGroup(group_name).getGroup(param_name);

    int value = param.getInt(0).get();
    int mod = param.getInt("mod").get();
    float mod_depth = param.getFloat("mod depth").get();

    int min = param.getInt(0).getMin();
    int max = param.getInt(0).getMax();

    result = static_cast<int>(ofClamp(value + (mod * mod_depth), min, max));
}

void ExtronDevice::applyModulation(parameter_list p, float &result)
{
//    int group_num = getGroupNum(p);
//    int param_num = getParamNum(p);
//
//    ofParameterGroup param = parameters.getGroup(group_num).getGroup(param_num);
    string group_name = getGroupName(p);
    string param_name = getParamName(p);
    
    ofParameterGroup param = parameters.getGroup(group_name).getGroup(param_name);

    float value = param.getFloat(0).get();
    float mod = param.getFloat("mod").get();
    float mod_depth = param.getFloat("mod depth").get();

    float min = param.getFloat(0).getMin();
    float max = param.getFloat(0).getMax();

    result = ofClamp(value + (mod * mod_depth), min, max);
}

void ExtronDevice::sendSIScommand(int val, sisCommand_t type)
{
    ofBuffer buffer;
    // all of the Picture Control SIS commands begin with the escape key
    buffer.append(&esc, 1);

    // if (type == sisCommand_t::position_x || type == sisCommand_t::position_y)
    // {
    //     val = ofClamp(val, POS_MIN, POS_MAX);
    // }
    // else if (type == sisCommand_t::size_x || type == sisCommand_t::size_y)
    // {
    //     val = ofClamp(val, IMG_SIZE_MIN, IMG_SIZE_MAX);
    // }
    // else
    // {
    //     val = ofClamp(val, PC_MIN, PC_MAX);
    // }

    buffer.append(int2str(val));
    buffer.append(SIS_map[type]);

    // Carriage Return character signifies end of command
    buffer.append(&cr, 1);
    if (sisMessages.size() < maxSisMessages)
    {
        sisMessages.push_back(buffer);
    }
}

void ExtronDevice::sendSIScommand(int x, int y, int w, int h)
{
    ofBuffer buffer;
    buffer.append(&esc, 1);

    vector<int> dims = {x, y, w, h};

    for (int i = 0; i < 4; i++)
    {
        // if (i < 2)
        // {
        //     dims[i] = ofClamp(dims[i], POS_MIN, POS_MAX);
        // }
        // else
        // {
        //     dims[i] = ofClamp(dims[i], IMG_SIZE_MIN, IMG_SIZE_MAX);
        // }

        buffer.append(int2str(dims[i]));

        if (i != 3)
        {
            buffer.append("*");
        }
    }

    buffer.append("XIMG");
    buffer.append(&cr, 1);

    if (sisMessages.size() < maxSisMessages)
    {
        sisMessages.push_back(buffer);
    }
}

int ExtronDevice::getGroupNum(parameter_list p)
{
    int param_int = static_cast<int>(p);
    return param_int > 14 ? 1 : 0;
}

int ExtronDevice::getParamNum(parameter_list p)
{
    int group_num = getGroupNum(p);
    int param_int = static_cast<int>(p);

    return floor((param_int + (group_num * -15)) / 3);
}

string ExtronDevice::getGroupName(parameter_list p)
{
    return group_map[p];
}

string ExtronDevice::getParamName(parameter_list p)
{
    return param_map[p];
}

string int2str(int val)
{
    string res = "";

    if (val < 0)
    {
        res.append("-");
    }

    if (abs(val) >= 1000)
    {
        char thousands = '0' + abs((val / 1000) % 10);
        res.append(&thousands, 1);
    }

    if (abs(val) >= 100)
    {
        char hundreds = '0' + abs((val / 100) % 10);
        res.append(&hundreds, 1);
    }

    if (abs(val) >= 10)
    {
        char tens = '0' + abs((val / 10) % 10);
        res.append(&tens, 1);
    }

    char ones = '0' + abs(val % 10);
    res.append(&ones, 1);

    return res;
}
