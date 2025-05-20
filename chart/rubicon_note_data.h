#ifndef RUBICON_NOTE_DATA_H
#define RUBICON_NOTE_DATA_H

#include "core/io/resource.h"

#include "core/object/class_db.h"
#include "core/variant/variant.h"
#include "core/variant/typed_array.h"
#include "core/variant/typed_dictionary.h"
#include "rubicon_row_data.h"
#include "rubicon_time_change.h"
#include "rubicon_sv_change.h"

template <typename T>
class TypedArray;

class RubiconRowData;

class RubiconNoteData : public Resource {
    GDCLASS(RubiconNoteData, Resource);

public:
    uint8_t lane = 0;
    StringName type = "";

    TypedDictionary<StringName, Variant> parameters;

    void set_parameters(const TypedDictionary<StringName, Variant> &p_parameters);
    TypedDictionary<StringName, Variant> get_parameters() const;

    float measure_time = 0.0f;
    float measure_length = 0.0f;

    void set_lane(const uint8_t p_lane);
    uint8_t get_lane() const;

    void set_type(const StringName &p_type);
    StringName get_type() const;

    void set_measure_time(const float p_value);
    float get_measure_time() const;
    void set_measure_length(const float p_value);
    float get_measure_length() const;
    
    float ms_time;
    float ms_length;

    float get_ms_time() const;
    float get_ms_length() const;

    int starting_scroll_velocity = 0;
    int ending_scroll_velocity = 0;

    int get_starting_scroll_velocity() const;
    int get_ending_scroll_velocity() const;

    Ref<RubiconRowData> starting_row;
    Ref<RubiconRowData> ending_row;

    Ref<RubiconRowData> get_starting_row() const;
    Ref<RubiconRowData> get_ending_row() const;

    bool internal_should_miss = false;
    bool internal_hit = false;
    bool internal_spawned = false;
    bool internal_counts_towards_score = true;

    void set_internal_should_miss(const bool p_value);
    bool get_internal_should_miss() const;
    void set_internal_hit(const bool p_value);
    bool get_internal_hit() const;
    void set_internal_spawned(const bool p_value);
    bool get_internal_spawned() const;
    void set_internal_counts_towards_score(const bool p_value);
    bool get_internal_counts_towards_score() const;

    void convert_data(const TypedArray<RubiconTimeChange> &p_time_changes, const TypedArray<RubiconSvChange> &p_sv_changes);

    static bool compare_notes_by_time(const Variant &p_a, const Variant &p_b);
    static bool compare_notes_by_lane(const Variant &p_a, const Variant &p_b);
    static bool is_note_lane(const Variant &p_note, const uint8_t p_lane);
    static bool is_note_type(const Variant &p_note, const StringName &p_type);

protected:
    static void _bind_methods();
};

#endif // RUBICON_NOTE_DATA_H