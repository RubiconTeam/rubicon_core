#ifndef RUBICON_SONG_DIFFICULTY_H
#define RUBICON_SONG_DIFFICULTY_H

#include "core/io/resource.h"
#include "modules/rubicon_core/chart/rubichart.h"

class RubiconSongDifficulty : public Resource {
    GDCLASS(RubiconSongDifficulty, Resource);

public:
    String name;
    Ref<RubiChart> chart = nullptr;

    void set_difficulty_name(const String p_name);
    String get_difficulty_name() const;

    void set_chart(const Ref<RubiChart> p_chart);
    Ref<RubiChart> get_chart() const;

protected:
    static void _bind_methods();
};

#endif