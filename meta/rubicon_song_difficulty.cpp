#include "rubicon_song_difficulty.h"

void RubiconSongDifficulty::set_difficulty_name(const String p_name) {
    name = p_name;
}

String RubiconSongDifficulty::get_difficulty_name() const {
    return name;
}

void RubiconSongDifficulty::set_chart(const Ref<RubiChart> p_chart) {
    chart = p_chart;
}

Ref<RubiChart> RubiconSongDifficulty::get_chart() const {
    return chart;
}

void RubiconSongDifficulty::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_difficulty_name", "name"), &RubiconSongDifficulty::set_difficulty_name);
    ClassDB::bind_method("get_difficulty_name", &RubiconSongDifficulty::get_difficulty_name);

    ClassDB::bind_method(D_METHOD("set_chart", "chart"), &RubiconSongDifficulty::set_chart);
    ClassDB::bind_method("get_chart", &RubiconSongDifficulty::get_chart);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "chart", PROPERTY_HINT_RESOURCE_TYPE, "RubiChart"), "set_chart", "get_chart");
}