#ifndef RUBICON_PLAY_FIELD_H
#define RUBICON_PLAY_FIELD_H

#include "core/object/class_db.h"
#include "scene/gui/control.h"
#include "chart/rubichart.h"

class RubiconPlayField : public Control {
    GDCLASS(RubiconPlayField, Control);

public:
    void set_chart(const Ref<RubiChart> chart);
    Ref<RubiChart> get_chart() const;

    

private:
    Ref<RubiChart> chart;
};

#endif // RUBICON_PLAY_FIELD_H