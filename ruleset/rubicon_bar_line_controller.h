#ifndef RUBICON_BAR_LINE_CONTROLLER_H
#define RUBICON_BAR_LINE_CONTROLLER_H

#include "core/object/class_db.h"
#include "scene/gui/control.h"
#include "../rubicon_play_field.h"

class RubiconBarLineController : public Control {
    GDCLASS(RubiconBarLineController, Control);

public:
    RubiconPlayField* play_field;

    void set_play_field(RubiconPlayField* p_value);
    RubiconPlayField* get_play_field();
};

#endif // RUBICON_BAR_LINE_CONTROLLER_H