#include "row_data.h"
#include "../conductor.h"

void RowData::set_section(const Ref<SectionData> p_section) {
    section = p_section;
}

Ref<SectionData> RowData::get_section() const {
    return section;
}

void RowData::set_lane_priority(const u_int8_t p_lane_priority) {
    lane_priority = p_lane_priority;
}

u_int8_t RowData::get_lane_priority() const {
    return lane_priority;
}

void RowData::set_offset(const u_int8_t p_offset) {
    offset = p_offset;
}

u_int8_t RowData::get_offset() const {
    return offset;
}

void RowData::set_quant(const u_int8_t p_quant) {
    quant = p_quant;
}

u_int8_t RowData::get_quant() const {
    return quant;
}

TypedArray<NoteData> RowData::get_notes(const bool p_include_ends) const {
    TypedArray<NoteData> notes = starting_notes.duplicate();
    if (!p_include_ends)
        return notes;
    
    notes.append_array(ending_notes);
    notes.sort_custom(Callable(this, "sort_notes_by_lane"));
}

bool RowData::sort_notes_by_lane(const Variant &a, const Variant &b) {
    NoteData* note_a = Object::cast_to<NoteData>(a);
    NoteData* note_b = Object::cast_to<NoteData>(b);
    if (note_a != nullptr && note_b != nullptr)
        return note_a->lane < note_b->lane;

    return a < b;
}