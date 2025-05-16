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

class NoteData; // Neccessary so RowData knows that NoteData is a class, at the very least.
class SectionData;

class RowData : public Resource {
    GDCLASS(RowData, Resource);

public:
    Ref<SectionData> section;
    uint8_t lane_priority = 0;
    uint8_t offset = 0;
    RubiChart::QuantValue quant = RubiChart::QuantValue::QUANT_VALUE_4TH;

    TypedArray<NoteData> starting_notes;
    TypedArray<NoteData> ending_notes;

    void set_section(const Ref<SectionData> p_section);
    Ref<SectionData> get_section() const;

    void set_lane_priority(const uint8_t p_lane_priority);
    uint8_t get_lane_priority() const;

    void set_offset(const uint8_t p_offset);
    uint8_t get_offset() const;

    void set_quant(const RubiChart::QuantValue p_quant);
    RubiChart::QuantValue get_quant() const;

    TypedArray<NoteData> get_notes(const bool p_include_ends = false) const;
    Ref<NoteData> get_note_at_lane(const uint8_t p_lane, const bool p_include_ends = false) const;
    TypedArray<NoteData> get_notes_of_type(const StringName &p_note_type, const bool p_include_ends = false) const;
    PackedStringArray get_note_types() const;

    void add_start_note(const Ref<NoteData> p_note);
    void add_end_note(const Ref<NoteData> p_note);

    void remove_note(const Ref<NoteData> p_note);
    void remove_note_at_lane(const uint8_t p_lane);

    bool has_note_at_lane(uint8_t p_lane) const;
    bool is_note_starting(const Ref<NoteData> p_note) const;
    bool is_note_ending(const Ref<NoteData> p_note) const;
    
    void convert_data(const TypedArray<NoteData> &p_time_changes, const TypedArray<SvChange> &p_sv_changes);

    static bool is_of_value(const Variant &p_row, const uint8_t p_offset, const RubiChart::QuantValue p_quant);
    static bool compare_rows(const Variant &p_a, const Variant &p_b);

protected:
    static void _bind_methods();
};

#endif // ROW_DATA_H