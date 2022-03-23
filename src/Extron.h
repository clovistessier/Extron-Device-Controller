#ifndef EXTRON_H
#define EXTRON_H


#include "ofMain.h"

enum struct sisCommand_t
{
    position_x = 0,
    position_y,
    size_x,
    size_y,
    brightness,
    contrast,
    color,
    tint,
    detail,
};

enum struct geometry_flag
{
    scale = 0,
    pos_x,
    pos_y,
    size_x,
    size_y
};

enum struct parameter_list : int
{
    position_x = 0,
    position_x_mod,
    position_x_mod_depth,
    position_y,
    position_y_mod,
    position_y_mod_depth,
    scale,
    scale_mod,
    scale_mod_depth,
    size_x,
    size_x_mod,
    size_x_mod_depth,
    size_y,
    size_y_mod,
    size_y_mod_depth,
    brightness,
    brightness_mod,
    brightness_mod_depth,
    contrast,
    contrast_mod,
    contrast_mod_depth,
    color,
    color_mod,
    color_mod_depth,
    tint,
    tint_mod,
    tint_mod_depth,
    detail,
    detail_mod,
    detail_mod_depth
};

enum struct parameter_type_t
{
    value = 0,
    mod,
    mod_depth,
};

enum midi_cc_lut
{
    midi_cc_position_x_mod_depth = 49,
    midi_cc_position_y_mod_depth,
    midi_cc_scale_mod_depth,
    midi_cc_brightness_mod_depth,
    midi_cc_contrast_mod_depth,
    midi_cc_color_mod_depth,
    midi_cc_tint_mod_depth,
    midi_cc_detail_mod_depth,
    midi_cc_position_x = 77,
    midi_cc_position_y,
    midi_cc_scale,
    midi_cc_brightness,
    midi_cc_contrast,
    midi_cc_color,
    midi_cc_tint,
    midi_cc_detail,
};

string int2str(int val);

// ascii code for esc character
const char esc = 27;
// ascii code for carriage return (no line feed)
const char cr = 13;

class ExtronDevice
{
public:
    ExtronDevice();
    virtual void setup();
    virtual void update();
    
    int getMidiChannel();

    bool isMidiCC(int cc);
    void updateFromMidi(int cc, int value);

    bool isOscAddress(const string &addr);
    void updateFromOsc(const string &addr, float val);
    
    template <typename ParameterType>
    void position_xChanged(ParameterType &new_val)
    {
        updateGeometry(geometry_flag::pos_x);
    }
    
    template <typename ParameterType>
    void position_yChanged(ParameterType &new_val)
    {
        updateGeometry(geometry_flag::pos_y);
    }

    template <typename ParameterType>
    void scaleChanged(ParameterType &new_val)
    {
        updateGeometry(geometry_flag::scale);
    }
    
    template <typename ParameterType>
    void size_xChanged(ParameterType &new_val)
    {
        updateGeometry(geometry_flag::size_x);
    }
    
    template <typename ParameterType>
    void size_yChanged(ParameterType &new_val)
    {
        updateGeometry(geometry_flag::size_y);
    }
    
    template <typename ParameterType>
    void brightnessChanged(ParameterType &new_val)
    {
        update_brightness();
    }
    
    template <typename ParameterType>
    void contrastChanged(ParameterType &new_val)
    {
        update_contrast();
    }
    
    template <typename ParameterType>
    void colorChanged(ParameterType &new_val)
    {
        update_color();
    }
    
    template <typename ParameterType>
    void tintChanged(ParameterType &new_val)
    {
        update_tint();
    }
    
    template <typename ParameterType>
    void detailChanged(ParameterType &new_val)
    {
        update_detail();
    }
    
    
//    void position_xChanged_i(int &new_val);
//    void position_xChanged_f(float &new_val);
//
//    void position_yChanged_i(int &new_val);
//    void position_yChanged_f(float &new_val);
//
//    void scaleChanged_f(float &new_val);
//
//    void size_xChanged_i(int &new_val);
//    void size_xChanged_f(float &new_val);
//
//    void size_yChanged_i(int &new_val);
//    void size_yChanged_f(float &new_val);
//
//    void brightnessChanged_i(int &new_val);
//    void brightnessChanged_f(float &new_val);
//
//    void contrastChanged_i(int &new_val);
//    void contrastChanged_f(float &new_val);
//
//    void colorChanged_i(int &new_val);
//    void colorChanged_f(float &new_val);
//
//    void tintChanged_i(int &new_val);
//    void tintChanged_f(float &new_val);
//
//    void detailChanged_i(int &new_val);
//    void detailChanged_f(float &new_val);

    ofParameterGroup parameters;

    // Geometry Parameters
    ofParameterGroup geometry;

    ofParameterGroup position_x_group;
    ofParameter<int> position_x;
    ofParameter<int> position_x_mod;
    ofParameter<float> position_x_mod_depth;

    ofParameterGroup position_y_group;
    ofParameter<int> position_y;
    ofParameter<int> position_y_mod;
    ofParameter<float> position_y_mod_depth;

    ofParameterGroup scale_group;
    ofParameter<float> scale;
    ofParameter<float> scale_mod;
    ofParameter<float> scale_mod_depth;
    ofParameter<float> smoothing;

    ofParameterGroup size_x_group;
    ofParameter<int> size_x;
    ofParameter<int> size_x_mod;
    ofParameter<float> size_x_mod_depth;

    ofParameterGroup size_y_group;
    ofParameter<int> size_y;
    ofParameter<int> size_y_mod;
    ofParameter<float> size_y_mod_depth;

    // Picture Control Parameters
    ofParameterGroup picture_controls;

    ofParameterGroup brightness_group;
    ofParameter<int> brightness;
    ofParameter<int> brightness_mod;
    ofParameter<float> brightness_mod_depth;

    ofParameterGroup contrast_group;
    ofParameter<int> contrast;
    ofParameter<int> contrast_mod;
    ofParameter<float> contrast_mod_depth;

    ofParameterGroup color_group;
    ofParameter<int> color;
    ofParameter<int> color_mod;
    ofParameter<float> color_mod_depth;

    ofParameterGroup tint_group;
    ofParameter<int> tint;
    ofParameter<int> tint_mod;
    ofParameter<float> tint_mod_depth;

    ofParameterGroup detail_group;
    ofParameter<int> detail;
    ofParameter<int> detail_mod;
    ofParameter<float> detail_mod_depth;

protected:
    void addListeners();
    
    void applyModulation(parameter_list p, int &result);
    void applyModulation(parameter_list p, float &result);

    virtual void updateGeometry(geometry_flag g);

    void update_brightness();
    void update_contrast();
    void update_color();
    virtual void update_tint();
    void update_detail();

    virtual void sendSIScommand(int val, sisCommand_t type);
    

    int getGroupNum(parameter_list p);
    int getParamNum(parameter_list p);
    
    string getGroupName(parameter_list p);
    string getParamName(parameter_list p);

    ofSerial serial;
    deque<ofBuffer> sisMessages;
    size_t maxSisMessages;

    map<int, parameter_list> midi_cc_map = {
        {midi_cc_position_x, parameter_list::position_x},
        {midi_cc_position_y, parameter_list::position_y},
        {midi_cc_scale, parameter_list::scale},
        {midi_cc_brightness, parameter_list::brightness},
        {midi_cc_contrast, parameter_list::contrast},
        {midi_cc_color, parameter_list::color},
        {midi_cc_tint, parameter_list::tint},
        {midi_cc_detail, parameter_list::detail},

        {midi_cc_position_x_mod_depth, parameter_list::position_x_mod_depth},
        {midi_cc_position_y_mod_depth, parameter_list::position_y_mod_depth},
        {midi_cc_scale_mod_depth, parameter_list::scale_mod_depth},
        {midi_cc_brightness_mod_depth, parameter_list::brightness_mod_depth},
        {midi_cc_contrast_mod_depth, parameter_list::contrast_mod_depth},
        {midi_cc_color_mod_depth, parameter_list::color_mod_depth},
        {midi_cc_tint_mod_depth, parameter_list::tint_mod_depth},
        {midi_cc_detail_mod_depth, parameter_list::detail_mod_depth}};

    map<string, parameter_list> osc_addr_map;
    
    map<parameter_list, string> group_map = {
        {parameter_list::position_x, "Geometry"},
        {parameter_list::position_x_mod, "Geometry"},
        {parameter_list::position_x_mod_depth, "Geometry"},
        {parameter_list::position_y,"Geometry"},
        {parameter_list::position_y_mod, "Geometry"},
        {parameter_list::position_y_mod_depth, "Geometry"},
        {parameter_list::scale, "Geometry"},
        {parameter_list::scale_mod, "Geometry"},
        {parameter_list::scale_mod_depth, "Geometry"},
        {parameter_list::size_x, "Geometry"},
        {parameter_list::size_x_mod, "Geometry"},
        {parameter_list::size_x_mod_depth, "Geometry"},
        {parameter_list::size_y, "Geometry"},
        {parameter_list::size_y_mod, "Geometry"},
        {parameter_list::size_y_mod_depth, "Geometry"},
        {parameter_list::brightness, "Picture Controls"},
        {parameter_list::brightness_mod, "Picture Controls"},
        {parameter_list::brightness_mod_depth, "Picture Controls"},
        {parameter_list::contrast, "Picture Controls"},
        {parameter_list::contrast_mod, "Picture Controls"},
        {parameter_list::contrast_mod_depth, "Picture Controls"},
        {parameter_list::color, "Picture Controls"},
        {parameter_list::color_mod, "Picture Controls"},
        {parameter_list::color_mod_depth, "Picture Controls"},
        {parameter_list::tint, "Picture Controls"},
        {parameter_list::tint_mod, "Picture Controls"},
        {parameter_list::tint_mod_depth, "Picture Controls"},
        {parameter_list::detail, "Picture Controls"},
        {parameter_list::detail_mod, "Picture Controls"},
        {parameter_list::detail_mod_depth, "Picture Controls"}};
    
    map<parameter_list, string> param_map = {
        {parameter_list::position_x, "POSITION X"},
        {parameter_list::position_x_mod, "POSITION X"},
        {parameter_list::position_x_mod_depth, "POSITION X"},
        {parameter_list::position_y,"POSITION Y"},
        {parameter_list::position_y_mod, "POSITION Y"},
        {parameter_list::position_y_mod_depth, "POSITION Y"},
        {parameter_list::scale, "SCALE"},
        {parameter_list::scale_mod, "SCALE"},
        {parameter_list::scale_mod_depth, "SCALE"},
        {parameter_list::size_x, "SIZE X"},
        {parameter_list::size_x_mod, "SIZE X"},
        {parameter_list::size_x_mod_depth, "SIZE X"},
        {parameter_list::size_y, "SIZE Y"},
        {parameter_list::size_y_mod, "SIZE Y"},
        {parameter_list::size_y_mod_depth, "SIZE Y"},
        {parameter_list::brightness, "BRIGHTNESS"},
        {parameter_list::brightness_mod, "BRIGHTNESS"},
        {parameter_list::brightness_mod_depth, "BRIGHTNESS"},
        {parameter_list::contrast, "CONTRAST"},
        {parameter_list::contrast_mod, "CONTRAST"},
        {parameter_list::contrast_mod_depth, "CONTRAST"},
        {parameter_list::color, "COLOR"},
        {parameter_list::color_mod, "COLOR"},
        {parameter_list::color_mod_depth, "COLOR"},
        {parameter_list::tint, "TINT"},
        {parameter_list::tint_mod, "TINT"},
        {parameter_list::tint_mod_depth, "TINT"},
        {parameter_list::detail, "DETAIL"},
        {parameter_list::detail_mod, "DETAIL"},
        {parameter_list::detail_mod_depth, "DETAIL"}
    };
    
    map<parameter_list, parameter_type_t> param_type_map = {
        {parameter_list::position_x, parameter_type_t::value},
        {parameter_list::position_x_mod, parameter_type_t::mod},
        {parameter_list::position_x_mod_depth, parameter_type_t::mod_depth},
        {parameter_list::position_y,parameter_type_t::value},
        {parameter_list::position_y_mod, parameter_type_t::mod},
        {parameter_list::position_y_mod_depth, parameter_type_t::mod_depth},
        {parameter_list::scale, parameter_type_t::value},
        {parameter_list::scale_mod, parameter_type_t::mod},
        {parameter_list::scale_mod_depth, parameter_type_t::mod_depth},
        {parameter_list::size_x, parameter_type_t::value},
        {parameter_list::size_x_mod, parameter_type_t::mod},
        {parameter_list::size_x_mod_depth, parameter_type_t::mod_depth},
        {parameter_list::size_y, parameter_type_t::value},
        {parameter_list::size_y_mod, parameter_type_t::mod},
        {parameter_list::size_y_mod_depth, parameter_type_t::mod_depth},
        {parameter_list::brightness, parameter_type_t::value},
        {parameter_list::brightness_mod, parameter_type_t::mod},
        {parameter_list::brightness_mod_depth, parameter_type_t::mod_depth},
        {parameter_list::contrast, parameter_type_t::value},
        {parameter_list::contrast_mod, parameter_type_t::mod},
        {parameter_list::contrast_mod_depth, parameter_type_t::mod_depth},
        {parameter_list::color, parameter_type_t::value},
        {parameter_list::color_mod, parameter_type_t::mod},
        {parameter_list::color_mod_depth, parameter_type_t::mod_depth},
        {parameter_list::tint, parameter_type_t::value},
        {parameter_list::tint_mod, parameter_type_t::mod},
        {parameter_list::tint_mod_depth, parameter_type_t::mod_depth},
        {parameter_list::detail, parameter_type_t::value},
        {parameter_list::detail_mod, parameter_type_t::mod},
        {parameter_list::detail_mod_depth, parameter_type_t::mod_depth}
    };



    int pos_x_val;
    int pos_y_val;
    float scale_val;
    float scale_follower;
    int size_x_val;
    int size_y_val;

    int brightness_val;
    int contrast_val;
    int color_val;
    int tint_val;
    int detail_val;

    int start_position_x;
    int start_position_y;
    int start_size_x;
    int start_size_y;

    int anchor_position_x;
    int anchor_position_y;
    int anchor_size_x;
    int anchor_size_y;
    
    int midi_channel;
    
private:
    virtual void sendSIScommand(int x, int y, int w, int h);
    
    map<sisCommand_t, string> SIS_map = {
        {sisCommand_t::position_x, "HCTR"},
        {sisCommand_t::position_y, "VCTR"},
        {sisCommand_t::size_x, "HSIZ"},
        {sisCommand_t::size_y, "VSIZ"},
        {sisCommand_t::brightness, "BRIT"},
        {sisCommand_t::contrast, "CONT"},
        {sisCommand_t::color, "COLR"},
        {sisCommand_t::tint, "TINT"},
        {sisCommand_t::detail, "HDET"}};
    
};
#endif
