#ifndef USP405_H
#define USP405_H

#include "ofMain.h"
#include "Extron.h"

enum Direction
{
    UP = 1,
    DOWN,
    LEFT,
    RIGHT,
};

class USP_405 : public ExtronDevice {
public:
    USP_405();
    void setup();
    void update();
    void adjustCentering(Direction dir);
    
protected:
    void updateGeometry(geometry_flag g);
    void sendSIScommand(int val, sisCommand_t type);
    
    void update_tint();
    
private:
    int target_pos_x;
    int target_pos_y;
    int target_size_x;
    int target_size_y;
    int x_drift;
    int y_drift;
    
    // tint and contrast are 8 bit values on USP 405
    // Midi is only 7 bits, so we will use use ofLerp to smooth
    // out the changes... especially tint
    int tint_follower;
    int contrast_follower;
    
    void seekGeometryTargets();
    void seekGeometryParam(int target, int& val, sisCommand_t type);
    
    bool isGeometryCommand(sisCommand_t type);
    
    set<sisCommand_t> geometry_commands
    {
        sisCommand_t::position_x,
        sisCommand_t::position_y,
        sisCommand_t::size_x,
        sisCommand_t::size_y,
    };
    
    map<sisCommand_t, string> SIS_map = {
        {sisCommand_t::position_x, "H"},
        {sisCommand_t::position_y, "/"},
        {sisCommand_t::size_x, ":"},
        {sisCommand_t::size_y, ";"},
        {sisCommand_t::brightness, "Y"},
        {sisCommand_t::contrast, "^"},
        {sisCommand_t::color, "C"},
        {sisCommand_t::tint, "T"},
        {sisCommand_t::detail, "D"}};
};

#endif
