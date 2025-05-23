#ifndef RUBICON_ROW_DATA_H
#define RUBICON_ROW_DATA_H

#include "core/io/resource.h"

#include "core/object/class_db.h"
#include "core/variant/typed_array.h"
#include "rubicon_note_data.h"
#include "rubicon_section_data.h"
#include "rubicon_sv_change.h"

template <typename T>
class TypedArray;

class RubiconNoteData; // Neccessary so RowData knows that NoteData is a class, at the very least.
class RubiconSectionData;

class RubiconRowData : public Resource {
    GDCLASS(RubiconRowData, Resource);

public:
    Ref<RubiconSectionData> section;
    uint8_t lane_priority = 0;
    uint8_t offset = 0;
    uint8_t quant = 4;

    TypedArray<RubiconNoteData> starting_notes;
    TypedArray<RubiconNoteData> ending_notes;

    void set_section(const Ref<RubiconSectionData> p_section);
    Ref<RubiconSectionData> get_section() const;

    void set_lane_priority(const uint8_t p_lane_priority);
    uint8_t get_lane_priority() const;

    void set_offset(const uint8_t p_offset);
    uint8_t get_offset() const;

    void set_quant(const uint8_t p_quant);
    uint8_t get_quant() const;

    TypedArray<RubiconNoteData> get_notes(const bool p_include_ends = false) const;
    Ref<RubiconNoteData> get_note_at_lane(const uint8_t p_lane, const bool p_include_ends = false) const;
    TypedArray<RubiconNoteData> get_notes_of_type(const StringName &p_note_type, const bool p_include_ends = false) const;
    PackedStringArray get_note_types() const;

    void set_starting_notes(const TypedArray<RubiconNoteData> p_starting_notes);
    TypedArray<RubiconNoteData> get_starting_notes() const;
    void add_starting_note(const Ref<RubiconNoteData> p_note);

    void set_ending_notes(const TypedArray<RubiconNoteData> p_ending_notes);
    TypedArray<RubiconNoteData> get_ending_notes() const;
    void add_ending_note(const Ref<RubiconNoteData> p_note);

    void remove_note(const Ref<RubiconNoteData> p_note);
    void remove_note_at_lane(const uint8_t p_lane);

    bool has_note_at_lane(uint8_t p_lane) const;
    bool is_note_starting(const Ref<RubiconNoteData> p_note) const;
    bool is_note_ending(const Ref<RubiconNoteData> p_note) const;
    
    void convert_data(const TypedArray<RubiconNoteData> &p_time_changes, const TypedArray<RubiconSvChange> &p_sv_changes);

    static bool is_of_value(const Variant &p_row, const uint8_t p_offset, const uint8_t p_quant);
    static bool compare_rows(const Variant &p_a, const Variant &p_b);

protected:
    static void _bind_methods();
};

#endif // RUBICON_ROW_DATA_H