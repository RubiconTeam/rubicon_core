#ifndef RUBICON_SONG_META_H
#define RUBICON_SONG_META_H

#include "core/io/resource.h"
#include "servers/audio/audio_stream.h"
//#include "rubicon_song_difficulty.h"
#include "modules/rubicon_engine/events/rubicon_event_meta.h"
#include "core/config/project_settings.h"
//#include "rubicon_character_meta.h"

class AudioStream;

class RubiconSongMeta : public Resource {
    GDCLASS(RubiconSongMeta, Resource);

public:
    String name;
    String artist;

    Ref<AudioStream> instrumental = nullptr;
    Ref<AudioStream> vocals = nullptr;

    //TypedArray<RubiconSongDifficulty> difficulties;
    Ref<RubiconEventMeta> event_meta = nullptr;

    TypedArray<RubiconTimeChange> time_changes;
    PackedStringArray modules;

    String ui_style = ProjectSettings::get_singleton()->get_setting("rubicon/default_ruleset", "default");

    // this is gonna be changed once we figure out how stages should be displayed
    // ^ we might've figured it out
    /*PackedStringArray stages = get_default_stages();
    TypedArray<RubiconCharacterMeta> characters = get_default_characters();*/

    String song_scene_path;

    float audio_start_offset = 0.0;

    PackedStringArray playable_charts = {"Player"};

    Ref<RubiconSongMeta> convert_data();
    
    /*Ref<RubiconSongDifficulty> get_difficulty_by_index(const int p_index) const;
    Ref<RubiconSongDifficulty> get_difficulty_by_name(const String p_name) const;
    
    int find_difficulty(const String p_name) const;
    bool has_difficulty(const String p_name) const;*/

    // Setters / Getters
    void set_song_name(const String p_name);
    String get_song_name() const;

    void set_artist(const String p_artist);
    String get_artist() const;

    void set_instrumental(const Ref<AudioStream> p_instrumental);
    Ref<AudioStream> get_instrumental() const;

    void set_vocals(const Ref<AudioStream> p_vocals);
    Ref<AudioStream> get_vocals() const;

    /*void set_difficulties(const TypedArray<RubiconSongDifficulty> p_difficulties);
    TypedArray<RubiconSongDifficulty> get_difficulties() const;*/

    void set_event_meta(const Ref<RubiconEventMeta> p_event_meta);
    Ref<RubiconEventMeta> get_event_meta() const;

    void set_time_changes(const TypedArray<RubiconTimeChange> p_time_changes);
    TypedArray<RubiconTimeChange> get_time_changes() const;

    void set_modules(const PackedStringArray p_modules);
    PackedStringArray get_modules() const;

    void set_ui_style(const String p_ui_style);
    String get_ui_style() const;

    /*void set_stages(const PackedStringArray p_stages);
    PackedStringArray get_stages() const;

    void set_characters(const TypedArray<RubiconCharacterMeta> p_characters);
    TypedArray<RubiconCharacterMeta> get_characters() const;*/

    void set_song_scene_path(const String p_song_scene_path);
    String get_song_scene_path() const;

    void set_audio_start_offset(const float p_audio_start_offset);
    float get_audio_start_offset() const;

    void set_playable_charts(const PackedStringArray p_playable_charts);
    PackedStringArray get_playable_charts() const;

protected:
    static void _bind_methods();

private:
    //PackedStringArray get_default_stages();
    //TypedArray<RubiconCharacterMeta> get_default_characters();
    //static bool find_index(const Variant &p_a, const Variant &p_b);
};

#endif