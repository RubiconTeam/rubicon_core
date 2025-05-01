#ifndef ROW_DATA_H
#define ROW_DATA_H

#include "core/io/resource.h"

#include "core/object/class_db.h"
#include "core/variant/typed_array.h"
#include "note_data.h"
#include "rubichart.h"
#include "section_data.h"
#include "sv_change.h"

template <typename T>
class TypedArray;

class RowData : public Resource {
    GDCLASS(RowData, Resource);

public:
    Ref<SectionData> section;
    u_int8_t lane_priority = 0;
    u_int8_t offset = 0;
    u_int8_t quant = RubiChart::QUANTS[0];

    TypedArray<NoteData> starting_notes;
    TypedArray<NoteData> ending_notes;

    void set_section(const Ref<SectionData> section);
    Ref<SectionData> get_section() const;

    void set_lane_priority(const u_int8_t p_lane_priority);
    u_int8_t get_lane_priority() const;

    void set_offset(const u_int8_t p_offset);
    u_int8_t get_offset() const;

    void set_quant(const u_int8_t p_quant);
    u_int8_t get_quant() const;

    TypedArray<NoteData> get_notes(const bool p_include_ends = false) const;
    Ref<NoteData> get_note_at_lane(const u_int8_t lane, const bool p_include_ends = false) const;
    TypedArray<NoteData> get_notes_of_type(const StringName p_note_type, const bool p_include_ends = false) const;
    PackedStringArray get_note_types() const;

    void add_start_note(const Ref<NoteData> note);
    void add_end_note(const Ref<NoteData> note);

    void remove_note(const Ref<NoteData> note);
    void remove_note_at_lane(int lane);

    bool has_note_at_lane(int lane);
    bool is_note_starting(const Ref<NoteData> note);
    bool is_note_ending(const Ref<NoteData> note);

    static bool sort_notes_by_lane(const Variant &a, const Variant &b);
    
    void convert_data(const TypedArray<NoteData> &p_time_changes, const TypedArray<SvChange> p_sv_changes);
};

#endif // ROW_DATA_H