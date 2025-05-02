#ifndef NOTE_DATA_H
#define NOTE_DATA_H

#include "core/io/resource.h"

#include "core/object/class_db.h"
#include "core/variant/variant.h"
#include "core/variant/typed_array.h"
#include "core/variant/typed_dictionary.h"
#include "row_data.h"
#include "sv_change.h"

template <typename T>
class TypedArray;

template <typename K, typename V>
class TypedDictionary;

class RowData; // Neccessary so NoteData knows that RowData is a class, at the very least.

class NoteData : public Resource {
    GDCLASS(NoteData, Resource);

public:
    u_int8_t lane;
    StringName type;
    TypedDictionary<StringName, Variant> parameters;
    
    float ms_time;
    float ms_length;

    float measure_time;
    float measure_length;

    int starting_scroll_velocity = 0;
    int ending_scroll_velocity = 0;

    Ref<RowData> starting_row;
    Ref<RowData> ending_row;

    bool should_miss = false;
    bool hit = false;
    bool spawned = false;
    bool counts_towards_score = true;

    void convert_data(const TypedArray<NoteData> &p_time_changes, const TypedArray<SvChange> &p_sv_changes);

    static bool sort_notes_by_lane(const Variant &a, const Variant &b);
    static bool is_note_lane(const Variant &p_note, const u_int8_t p_lane);
    static bool is_note_type(const Variant &p_note, const StringName &p_type);
};

#endif // NOTE_DATA_H