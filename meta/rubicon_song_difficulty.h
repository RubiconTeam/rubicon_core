#ifndef RUBICON_SONG_DIFFICULTY_H
#define RUBICON_SONG_DIFFICULTY_H

#include "core/io/resource.h"
#include "rubicon_core/chart/rubichart.h"

class RubiconSongDifficulty : public Resource {
    GDCLASS(RubiconSongDifficulty, Resource);

public:
    String name;
    Ref<RubiChart> chart;

    void set_name(const String p_name);
    String get_name() const;

    void set_chart(const Ref<RubiChart> p_chart);
    RubiChart get_chart() const;
};

#endif