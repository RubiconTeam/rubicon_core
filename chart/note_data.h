#ifndef NOTE_DATA_H
#define NOTE_DATA_H

#include "core/io/resource.h"

#include "core/object/class_db.h"
#include "core/variant/variant.h"
#include "core/variant/typed_array.h"
#include "core/variant/typed_dictionary.h"
#include "row_data.h"
#include "time_change.h"
#include "sv_change.h"

template <typename T>
class TypedArray;

class RowData; // Neccessary so NoteData knows that RowData is a class, at the very least.

class NoteData : public Resource {
    GDCLASS(NoteData, Resource);

public:
    uint8_t lane;
    StringName type;

    float measure_time;
    float measure_length;

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

    Ref<RowData> starting_row;
    Ref<RowData> ending_row;

    Ref<RowData> get_starting_row() const;
    Ref<RowData> get_ending_row() const;

    bool should_miss = false;
    bool hit = false;
    bool spawned = false;
    bool counts_towards_score = true;

    void set_should_miss(const bool p_value);
    bool get_should_miss() const;
    void set_hit(const bool p_value);
    bool get_hit() const;
    void set_spawned(const bool p_value);
    bool get_spawned() const;
    void set_counts_towards_score(const bool p_value);
    bool get_counts_towards_score() const;

    void convert_data(const TypedArray<TimeChange> &p_time_changes, const TypedArray<SvChange> &p_sv_changes);

    static bool compare_notes_by_lane(const Variant &p_a, const Variant &p_b);
    static bool is_note_lane(const Variant &p_note, const uint8_t p_lane);
    static bool is_note_type(const Variant &p_note, const StringName &p_type);

protected:
    static void _bind_methods();
};

#endif // NOTE_DATA_H