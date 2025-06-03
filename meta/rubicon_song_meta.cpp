#include "rubicon_song_meta.h"
#include "../rubicon_conductor.h"

void RubiconSongMeta::set_song_name(const String p_name) {
    name = p_name;
}

String RubiconSongMeta::get_song_name() const {
    return name;
}

void RubiconSongMeta::set_artist(const String p_artist) {
    artist = p_artist;
}

String RubiconSongMeta::get_artist() const {
    return artist;
}

void RubiconSongMeta::set_instrumental(const Ref<AudioStream> p_instrumental) {
    instrumental = p_instrumental;
}

Ref<AudioStream> RubiconSongMeta::get_instrumental() const {
    return instrumental;
}

void RubiconSongMeta::set_vocals(const Ref<AudioStream> p_vocals) {
    vocals = p_vocals;
}

Ref<AudioStream> RubiconSongMeta::get_vocals() const {
    return vocals;
}

void RubiconSongMeta::set_difficulties(const TypedArray<RubiconSongDifficulty> p_difficulties) {
    difficulties = p_difficulties;
}

TypedArray<RubiconSongDifficulty> RubiconSongMeta::get_difficulties() const {
    return difficulties;
}

void RubiconSongMeta::set_event_meta(const Ref<RubiconEventMeta> p_event_meta) {
    event_meta = p_event_meta;
}

Ref<RubiconEventMeta> RubiconSongMeta::get_event_meta() const {
    return event_meta;
}

void RubiconSongMeta::set_time_changes(const TypedArray<RubiconTimeChange> p_time_changes) {
    time_changes = p_time_changes;
}

TypedArray<RubiconTimeChange> RubiconSongMeta::get_time_changes() const {
    return time_changes;
}

void RubiconSongMeta::set_modules(const PackedStringArray p_modules) {
    modules = p_modules;
}

PackedStringArray RubiconSongMeta::get_modules() const {
    return modules;
}

void RubiconSongMeta::set_ui_style(const String p_ui_style) {
    ui_style = p_ui_style;
}

String RubiconSongMeta::get_ui_style() const {
    return ui_style;
}

void RubiconSongMeta::set_stages(const PackedStringArray p_stages) {
    stages = p_stages;
}

PackedStringArray RubiconSongMeta::get_stages() const {
    return stages;
}

PackedStringArray RubiconSongMeta::get_default_stages() {
    // this is gonna be changed once we figure out how stages should be displayed
    return ProjectSettings::get_singleton()->get_setting("rubicon/stage_placeholder", "stage");
}

void RubiconSongMeta::set_characters(const TypedArray<RubiconCharacterMeta> p_characters) {
    characters = p_characters;
}

TypedArray<RubiconCharacterMeta> RubiconSongMeta::get_characters() const {
    return characters;
}

TypedArray<RubiconCharacterMeta> RubiconSongMeta::get_default_characters() {
    TypedArray<RubiconCharacterMeta> default_characters;

    Ref<RubiconCharacterMeta> default_player = memnew(RubiconCharacterMeta);
    default_player->character = ProjectSettings::get_singleton()->get_setting("rubicon/character_placeholder", "res://resources/game/characters/placeholder.tscn");
    default_player->nickname = "Player";
    default_player->bar_line = "Player";
    default_characters.push_back(default_player);

    Ref<RubiconCharacterMeta> default_opponent = memnew(RubiconCharacterMeta);
    default_opponent->character = ProjectSettings::get_singleton()->get_setting("rubicon/character_placeholder", "res://resources/game/characters/placeholder.tscn");
    default_opponent->nickname = "Opponent";
    default_opponent->bar_line = "Opponent";
    default_characters.push_back(default_opponent);

    return default_characters;
}

void RubiconSongMeta::set_audio_start_offset(const float p_audio_start_offset) {
    audio_start_offset = p_audio_start_offset;
}

float RubiconSongMeta::get_audio_start_offset() const {
    return audio_start_offset;
}

void RubiconSongMeta::set_playable_charts(const PackedStringArray p_playable_charts) {
    playable_charts = p_playable_charts;
}

PackedStringArray RubiconSongMeta::get_playable_charts() const {
    return playable_charts;
}

Ref<RubiconSongMeta> RubiconSongMeta::convert_data() {
    for (int i = 1; i < time_changes.size(); i++) {
        Ref<RubiconTimeChange> previous_time_change = time_changes[i-1];
        Ref<RubiconTimeChange> time_change = time_changes[i];

        time_change->ms_time = previous_time_change->ms_time + RubiconConductor::measure_to_ms(time_change->time - previous_time_change->time, previous_time_change->bpm, time_change->time_signature_numerator);
    }

    return this;
}

bool RubiconSongMeta::find_index(const Variant &p_a, const Variant &p_b) {
    // figure this out later ig
    return true;
}

int RubiconSongMeta::find_difficulty(const String p_name) const {
    return difficulties.find_custom(callable_mp_static(RubiconSongMeta::find_index));
}

bool RubiconSongMeta::has_difficulty(const String p_name) const {
    for (int i = 0; i < difficulties.size(); i++) {
        Ref<RubiconSongDifficulty> difficulty = difficulties[i];
        if (p_name == difficulty->name)
            return true;
    }

    return false;
}

Ref<RubiconSongDifficulty> RubiconSongMeta::get_difficulty_by_index(const int p_index) const {
    if (p_index >= difficulties.size())
        return nullptr;
    
    return difficulties[p_index];
}

Ref<RubiconSongDifficulty> RubiconSongMeta::get_difficulty_by_name(const String p_name) const {
    int idx = find_difficulty(p_name);
    if (idx == -1)
        return nullptr;
    
    return get_difficulty_by_index(idx);
}

void RubiconSongMeta::_bind_methods() {
    // Binding methods
    ClassDB::bind_method(D_METHOD("set_song_name", "name"), &RubiconSongMeta::set_song_name);
    ClassDB::bind_method("get_song_name", &RubiconSongMeta::get_song_name);

    ClassDB::bind_method(D_METHOD("set_artist", "artist"), &RubiconSongMeta::set_artist);
    ClassDB::bind_method("get_artist", &RubiconSongMeta::get_artist);

    ClassDB::bind_method(D_METHOD("set_instrumental", "instrumental"), &RubiconSongMeta::set_instrumental);
    ClassDB::bind_method("get_instrumental", &RubiconSongMeta::get_instrumental);

    ClassDB::bind_method(D_METHOD("set_vocals", "vocals"), &RubiconSongMeta::set_vocals);
    ClassDB::bind_method("get_vocals", &RubiconSongMeta::get_vocals);
    
    ClassDB::bind_method(D_METHOD("set_difficulties", "difficulties"), &RubiconSongMeta::set_difficulties);
    ClassDB::bind_method("get_difficulties", &RubiconSongMeta::get_difficulties);

    ClassDB::bind_method(D_METHOD("set_event_meta", "event_meta"), &RubiconSongMeta::set_event_meta);
    ClassDB::bind_method("get_event_meta", &RubiconSongMeta::get_event_meta);

    ClassDB::bind_method(D_METHOD("set_time_changes", "time_changes"), &RubiconSongMeta::set_time_changes);
    ClassDB::bind_method("get_time_changes", &RubiconSongMeta::get_time_changes);

    ClassDB::bind_method(D_METHOD("set_modules", "modules"), &RubiconSongMeta::set_modules);
    ClassDB::bind_method("get_modules", &RubiconSongMeta::get_modules);

    ClassDB::bind_method(D_METHOD("set_ui_style", "ui_style"), &RubiconSongMeta::set_ui_style);
    ClassDB::bind_method("get_ui_style", &RubiconSongMeta::get_ui_style);

    ClassDB::bind_method(D_METHOD("set_stages", "stages"), &RubiconSongMeta::set_stages);
    ClassDB::bind_method("get_stages", &RubiconSongMeta::get_stages);
    
    ClassDB::bind_method(D_METHOD("set_characters", "characters"), &RubiconSongMeta::set_characters);
    ClassDB::bind_method("get_characters", &RubiconSongMeta::get_characters);

    ClassDB::bind_method(D_METHOD("set_audio_start_offset", "audio_start_offset"), &RubiconSongMeta::set_audio_start_offset);
    ClassDB::bind_method("get_audio_start_offset", &RubiconSongMeta::get_audio_start_offset);

    ClassDB::bind_method(D_METHOD("set_playable_charts", "playable_charts"), &RubiconSongMeta::set_playable_charts);
    ClassDB::bind_method("get_playable_charts", &RubiconSongMeta::get_playable_charts);

    // Adding properties
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_song_name", "get_song_name");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "artist"), "set_artist", "get_artist");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "instrumental", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_instrumental", "get_instrumental");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "vocals", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_vocals", "get_vocals");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "difficulties", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconSongDifficulty")), "set_difficulties", "get_difficulties");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "event_meta", PROPERTY_HINT_RESOURCE_TYPE, "RubiconEventMeta"), "set_event_meta", "get_event_meta");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "time_changes", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconTimeChange")), "set_time_changes", "get_time_changes");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "modules"), "set_modules", "get_modules");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "ui_style"), "set_ui_style", "get_ui_style");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "stages"), "set_stages", "get_stages");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "characters", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconCharacterMeta")), "set_characters", "get_characters");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "audio_start_offset"), "set_audio_start_offset", "get_audio_start_offset");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "playable_charts"), "set_playable_charts", "get_playable_charts");
}