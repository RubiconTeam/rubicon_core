#ifndef RUBICON_SONG_META_H
#define RUBICON_SONG_META_H

#include "core/io/resource.h"
#include "servers/audio_server.h"
#include "rubicon_song_difficulty.h"
#include "rubicon_event_meta.h"

class AudioStream;

class RubiconSongMeta : public Resource {
    GDCLASS(RubiconSongMeta, Resource);

public:
    String name;
    String artist;

    Ref<AudioStream> instrumental;
    Ref<AudioStream> vocals;

    TypedArray<RubiconSongDifficulty> difficulties;
    Ref<RubiconEventMeta> event_meta;

    TypedArray<RubiconTimeChange> time_changes;
    PackedStringArray modules;

    String ui_style;

    PackedStringArray stages;
    PackedStringArray characters;

    float audio_start_offset = 0.0;

    PackedStringArray playable_charts;

    Ref<RubiconSongMeta> convert_data() const;
    
    Ref<RubiconSongDifficulty> get_difficulty_by_index(const int index) const;
    Ref<RubiconSongDifficulty> get_difficulty_by_name(const String name, const String rule_set) const;
    Ref<RubiconSongDifficulty> get_first_difficulty_of_rule_set(const String rule_set) const;
    
    int find_difficulty(const String name, const String rule_set) const;
    bool has_difficulty(const String name, const String rule_set) const;

    // Setters / Getters
    void set_name(const String p_name);
    String get_name() const;

    void set_artist(const String p_artist);
    String get_artist() const;

    void set_instrumental(const Ref<AudioStream> p_instrumental);
    Ref<AudioStream> get_instrumental() const;

    void set_vocals(const Ref<AudioStream> p_vocals);
    Ref<AudioStream> get_vocals() const;

    void set_difficulties(const TypedArray<RubiconSongDifficulty> p_difficulties);
    TypedArray<RubiconSongDifficulty> get_difficulties() const;

    void set_event_meta(const RubiconEventMeta p_event_meta);
    RubiconEventMeta get_event_meta() const;

    void set_time_changes(const TypedArray<RubiconTimeChange> p_time_changes);
    TypedArray<RubiconTimeChange> get_time_changes() const;

    void set_modules(const PackedStringArray p_modules);
    PackedStringArray get_modules() const;

    void set_ui_style(const String p_ui_style);
    String get_ui_style() const;

    void set_stages(const PackedStringArray p_stage);
    PackedStringArray get_stages() const;

    void set_characters(const PackedStringArray p_characters);
    PackedStringArray get_characters() const;

    void set_audio_start_offset(const float p_audio_start_offset);
    float get_audio_start_offset() const;

    void set_playable_charts(const PackedStringArray p_playable_charts);
    PackedStringArray get_playable_charts() const;
};

#endif