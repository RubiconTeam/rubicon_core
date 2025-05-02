#include "note_data.h"

void NoteData::convert_data(const TypedArray<NoteData> &p_time_changes, const TypedArray<SvChange> &p_sv_changes) {
    // NOTE: Port this from Rubicon.Fusion
}

bool NoteData::sort_notes_by_lane(const Variant &a, const Variant &b) {
    NoteData* note_a = Object::cast_to<NoteData>(a);
    NoteData* note_b = Object::cast_to<NoteData>(b);
    if (note_a != nullptr && note_b != nullptr)
        return note_a->lane < note_b->lane;

    return a < b;
}

bool NoteData::is_note_lane(const Variant &p_note, const u_int8_t p_lane) {
    NoteData* casted_note = Object::cast_to<NoteData>(p_note);
    if (casted_note != nullptr)
        return casted_note->lane == p_lane;

    return false;
}

bool NoteData::is_note_type(const Variant &p_note, const StringName &p_type) {
    NoteData* casted_note = Object::cast_to<NoteData>(p_note);
    if (casted_note != nullptr)
        return casted_note->type == p_type;

    return false;
}