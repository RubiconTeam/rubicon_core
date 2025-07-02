#ifndef RUBICON_PLAY_FIELD_H
#define RUBICON_PLAY_FIELD_H

#include "core/object/class_db.h"
#include "scene/gui/control.h"
#include "chart/rubichart.h"
#include "rubicon_song.h"

class RubiconPlayField : public Control {
    GDCLASS(RubiconPlayField, Control);

    /*
public:
    void set_chart(const Ref<RubiChart> chart);
    Ref<RubiChart> get_chart() const;

    void set_song(const RubiconSong* p_value);
    RubiconSong* get_song() const;    

private:
    Ref<RubiChart> _chart;
    RubiconSong* _song;*/
};

#endif // RUBICON_PLAY_FIELD_H