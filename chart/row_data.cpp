#include "row_data.h"
#include "../conductor.h"

void RowData::set_section(const Ref<SectionData> p_section) {
    section = p_section;
}

Ref<SectionData> RowData::get_section() const {
    return section;
}

void RowData::set_lane_priority(const uint8_t p_lane_priority) {
    lane_priority = p_lane_priority;
}

uint8_t RowData::get_lane_priority() const {
    return lane_priority;
}

void RowData::set_offset(const uint8_t p_offset) {
    offset = p_offset;
}

uint8_t RowData::get_offset() const {
    return offset;
}

void RowData::set_quant(const RubiChart::QuantValue p_quant) {
    quant = p_quant;
}

RubiChart::QuantValue RowData::get_quant() const {
    return quant;
}

TypedArray<NoteData> RowData::get_notes(const bool p_include_ends) const {
    TypedArray<NoteData> notes = starting_notes.duplicate();
    if (!p_include_ends)
        return notes;
    
    notes.append_array(ending_notes);
    notes.sort_custom(callable_mp_static(&NoteData::sort_notes_by_lane));
    return notes;
}

Ref<NoteData> RowData::get_note_at_lane(const uint8_t p_lane, const bool p_include_ends) const {
    int note_index = starting_notes.find_custom(callable_mp_static(NoteData::is_note_lane).bind(p_lane));
    if (note_index != -1) {
        return starting_notes[note_index];
    } else if (!p_include_ends) {
        return nullptr;
    }
    
    note_index = ending_notes.find_custom(callable_mp_static(NoteData::is_note_lane).bind(p_lane));
    if (note_index != -1)
        return ending_notes[note_index];

    return nullptr;
}

TypedArray<NoteData> RowData::get_notes_of_type(const StringName &p_note_type, const bool p_include_ends) const {
    TypedArray<NoteData> note_list = starting_notes.filter(callable_mp_static(NoteData::is_note_type).bind(p_note_type));
    if (p_include_ends) {
        note_list.append_array(ending_notes.filter(callable_mp_static(NoteData::is_note_type).bind(p_note_type)));
        note_list.sort_custom(callable_mp_static(&NoteData::sort_notes_by_lane));
    }

    return note_list;
}

PackedStringArray RowData::get_note_types() const {
    PackedStringArray note_types;
    int i;
    for (i = 0; i < starting_notes.size(); i++) {
        Ref<NoteData> cur_note = starting_notes[i];
        bool has_type = !cur_note->type.is_empty();
        if (!has_type || note_types.has(cur_note->type))
            continue;
        
        note_types.push_back(cur_note->type);
    }

    for (i = 0; i < ending_notes.size(); i++) {
        Ref<NoteData> cur_note = ending_notes[i];
        bool has_type = !cur_note->type.is_empty();
        if (!has_type || note_types.has(cur_note->type))
            continue;
        
        note_types.push_back(cur_note->type);
    }

    return note_types;
}

void RowData::add_start_note(const Ref<NoteData> p_note) {
    ERR_FAIL_COND_MSG(has_note_at_lane(p_note->lane), vformat("Already has note at lane %d.", p_note->lane));

    starting_notes.push_back(p_note);
    starting_notes.sort_custom(callable_mp_static(&NoteData::sort_notes_by_lane));
}

void RowData::add_end_note(const Ref<NoteData> p_note) {
    ERR_FAIL_COND_MSG(has_note_at_lane(p_note->lane), vformat("Already has note at lane %d.", p_note->lane));

    ending_notes.push_back(p_note);
    ending_notes.sort_custom(callable_mp_static(&NoteData::sort_notes_by_lane));
}

void RowData::remove_note(const Ref<NoteData> p_note) {
    int index = starting_notes.find(p_note);
    if (index != -1)
        starting_notes.remove_at(index);
    
    index = ending_notes.find(p_note);
    if (index != 1)
        ending_notes.remove_at(index);
    
    starting_notes.sort_custom(callable_mp_static(&NoteData::sort_notes_by_lane));
    ending_notes.sort_custom(callable_mp_static(&NoteData::sort_notes_by_lane));
}

void RowData::remove_note_at_lane(const uint8_t p_lane) {
    int index = starting_notes.find_custom(callable_mp_static(NoteData::is_note_lane).bind(p_lane));
    if (index != -1)
        starting_notes.remove_at(index);
    
    index = ending_notes.find_custom(callable_mp_static(NoteData::is_note_lane).bind(p_lane));
    if (index != 1)
        ending_notes.remove_at(index);
    
    starting_notes.sort_custom(callable_mp_static(&NoteData::sort_notes_by_lane));
    ending_notes.sort_custom(callable_mp_static(&NoteData::sort_notes_by_lane));
}

bool RowData::has_note_at_lane(const uint8_t p_lane) const {
    bool has_note = starting_notes.any(callable_mp_static(NoteData::is_note_lane).bind(p_lane));
    if (!has_note)
        has_note = ending_notes.any(callable_mp_static(NoteData::is_note_lane).bind(p_lane));
    
    return has_note;
}

bool RowData::is_note_starting(const Ref<NoteData> p_note) const {
    return starting_notes.has(p_note);
}

bool RowData::is_note_ending(const Ref<NoteData> p_note) const {
    return ending_notes.has(p_note);
}

void RowData::convert_data(const TypedArray<NoteData> &p_time_changes, const TypedArray<SvChange> &p_sv_changes) {
    int i;
    for (i = 0; i < starting_notes.size(); i++) {
        Ref<NoteData> cur_note = starting_notes[i];
        cur_note->starting_row = this;
        cur_note->convert_data(p_time_changes, p_sv_changes);
    }
    
    for (i = 0; i < ending_notes.size(); i++) {
        Ref<NoteData> cur_note = ending_notes[i];
        cur_note->ending_row = this;
        cur_note->convert_data(p_time_changes, p_sv_changes);
    }
}

void RowData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_section", "section"), &RowData::set_section);
    ClassDB::bind_method("get_section", &RowData::get_section);
    ClassDB::bind_method(D_METHOD("set_lane_priority", "lane_priority"), &RowData::set_lane_priority);
    ClassDB::bind_method("get_lane_priority", &RowData::get_lane_priority);
    ClassDB::bind_method(D_METHOD("set_offset", "offset"), &RowData::set_offset);
    ClassDB::bind_method("get_offset", &RowData::get_offset);
    ClassDB::bind_method(D_METHOD("set_quant", "quant"), &RowData::set_quant);
    ClassDB::bind_method("get_quant", &RowData::get_quant);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "section", PROPERTY_HINT_RESOURCE_TYPE, "SectionData"), "set_section", "get_section");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lane_priority"), "set_lane_priority", "get_lane_priority");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "offset"), "set_offset", "get_offset");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "quant", PROPERTY_HINT_ENUM, "4th:4,8th:8,12th:12,16th:16,24nd:24,32nd:32,48th:48,64th:64,192nd:192"), "set_quant", "get_quant");

    ClassDB::bind_method(D_METHOD("get_notes", "include_ends"), &RowData::get_notes);
    ClassDB::bind_method(D_METHOD("get_note_at_lane", "lane", "include_ends"), &RowData::get_note_at_lane);
    ClassDB::bind_method(D_METHOD("get_notes_of_type", "note_type", "include_ends"), &RowData::get_notes_of_type);
    ClassDB::bind_method("get_note_types", &RowData::get_note_types);

    ClassDB::bind_method(D_METHOD("add_start_note", "note"), &RowData::add_start_note);
    ClassDB::bind_method(D_METHOD("add_end_note", "note"), &RowData::add_end_note);

    ClassDB::bind_method(D_METHOD("remove_note", "note"), &RowData::remove_note);
    ClassDB::bind_method(D_METHOD("remove_note_at_lane", "lane"), &RowData::remove_note_at_lane);

    ClassDB::bind_method(D_METHOD("has_note_at_lane", "lane"), &RowData::has_note_at_lane);
    ClassDB::bind_method(D_METHOD("is_note_starting", "note"), &RowData::is_note_starting);
    ClassDB::bind_method(D_METHOD("is_note_ending", "note"), &RowData::is_note_ending);

    ClassDB::bind_method(D_METHOD("convert_data", "time_changes", "sv_changes"), &RowData::convert_data);
}