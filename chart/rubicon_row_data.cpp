#include "rubicon_row_data.h"
#include "../rubicon_conductor.h"

void RubiconRowData::set_section(const Ref<RubiconSectionData> p_section) {
    section = p_section;
}

Ref<RubiconSectionData> RubiconRowData::get_section() const {
    return section;
}

void RubiconRowData::set_lane_priority(const uint8_t p_lane_priority) {
    lane_priority = p_lane_priority;
}

uint8_t RubiconRowData::get_lane_priority() const {
    return lane_priority;
}

void RubiconRowData::set_offset(const uint8_t p_offset) {
    offset = p_offset;
}

uint8_t RubiconRowData::get_offset() const {
    return offset;
}

void RubiconRowData::set_quant(const uint8_t p_quant) {
    quant = p_quant;
}

uint8_t RubiconRowData::get_quant() const {
    return quant;
}

TypedArray<RubiconNoteData> RubiconRowData::get_notes(const bool p_include_ends) const {
    TypedArray<RubiconNoteData> notes = starting_notes.duplicate();
    if (!p_include_ends)
        return notes;
    
    notes.append_array(ending_notes);
    notes.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_lane));
    return notes;
}

Ref<RubiconNoteData> RubiconRowData::get_note_at_lane(const uint8_t p_lane, const bool p_include_ends) const {
    int note_index = starting_notes.find_custom(callable_mp_static(RubiconNoteData::is_note_lane).bind(p_lane));
    if (note_index != -1) {
        return starting_notes[note_index];
    } else if (!p_include_ends) {
        return nullptr;
    }
    
    note_index = ending_notes.find_custom(callable_mp_static(RubiconNoteData::is_note_lane).bind(p_lane));
    if (note_index != -1)
        return ending_notes[note_index];

    return nullptr;
}

TypedArray<RubiconNoteData> RubiconRowData::get_notes_of_type(const StringName &p_note_type, const bool p_include_ends) const {
    TypedArray<RubiconNoteData> note_list = starting_notes.filter(callable_mp_static(RubiconNoteData::is_note_type).bind(p_note_type));
    if (p_include_ends) {
        note_list.append_array(ending_notes.filter(callable_mp_static(RubiconNoteData::is_note_type).bind(p_note_type)));
        note_list.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_lane));
    }

    return note_list;
}

PackedStringArray RubiconRowData::get_note_types() const {
    PackedStringArray note_types;
    int i;
    for (i = 0; i < starting_notes.size(); i++) {
        Ref<RubiconNoteData> cur_note = starting_notes[i];
        bool has_type = !cur_note->type.is_empty();
        if (!has_type || note_types.has(cur_note->type))
            continue;
        
        note_types.push_back(cur_note->type);
    }

    for (i = 0; i < ending_notes.size(); i++) {
        Ref<RubiconNoteData> cur_note = ending_notes[i];
        bool has_type = !cur_note->type.is_empty();
        if (!has_type || note_types.has(cur_note->type))
            continue;
        
        note_types.push_back(cur_note->type);
    }

    return note_types;
}

void RubiconRowData::set_starting_notes(const TypedArray<RubiconNoteData> p_starting_notes) {
    starting_notes = p_starting_notes;
}

TypedArray<RubiconNoteData> RubiconRowData::get_starting_notes() const {
    return starting_notes;
}

void RubiconRowData::add_starting_note(const Ref<RubiconNoteData> p_note) {
    ERR_FAIL_COND_MSG(has_note_at_lane(p_note->lane), vformat("Already has note at lane %d.", p_note->lane));

    starting_notes.push_back(p_note);
    starting_notes.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_lane));
}

void RubiconRowData::set_ending_notes(const TypedArray<RubiconNoteData> p_ending_notes) {
    ending_notes = p_ending_notes;
}

TypedArray<RubiconNoteData> RubiconRowData::get_ending_notes() const {
    return ending_notes;
}

void RubiconRowData::add_ending_note(const Ref<RubiconNoteData> p_note) {
    ERR_FAIL_COND_MSG(has_note_at_lane(p_note->lane), vformat("Already has note at lane %d.", p_note->lane));

    ending_notes.push_back(p_note);
    ending_notes.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_lane));
}

void RubiconRowData::remove_note(const Ref<RubiconNoteData> p_note) {
    int index = starting_notes.find(p_note);
    if (index != -1)
        starting_notes.remove_at(index);
    
    index = ending_notes.find(p_note);
    if (index != 1)
        ending_notes.remove_at(index);
    
    starting_notes.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_lane));
    ending_notes.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_lane));
}

void RubiconRowData::remove_note_at_lane(const uint8_t p_lane) {
    int index = starting_notes.find_custom(callable_mp_static(RubiconNoteData::is_note_lane).bind(p_lane));
    if (index != -1)
        starting_notes.remove_at(index);
    
    index = ending_notes.find_custom(callable_mp_static(RubiconNoteData::is_note_lane).bind(p_lane));
    if (index != 1)
        ending_notes.remove_at(index);
    
    starting_notes.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_lane));
    ending_notes.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_lane));
}

bool RubiconRowData::has_note_at_lane(const uint8_t p_lane) const {
    bool has_note = starting_notes.any(callable_mp_static(RubiconNoteData::is_note_lane).bind(p_lane));
    if (!has_note)
        has_note = ending_notes.any(callable_mp_static(RubiconNoteData::is_note_lane).bind(p_lane));
    
    return has_note;
}

bool RubiconRowData::is_note_starting(const Ref<RubiconNoteData> p_note) const {
    return starting_notes.has(p_note);
}

bool RubiconRowData::is_note_ending(const Ref<RubiconNoteData> p_note) const {
    return ending_notes.has(p_note);
}

void RubiconRowData::convert_data(const TypedArray<RubiconNoteData> &p_time_changes, const TypedArray<RubiconScrollVelocity> &p_velocities) {
    int i;
    for (i = 0; i < starting_notes.size(); i++) {
        Ref<RubiconNoteData> cur_note = starting_notes[i];
        cur_note->starting_row = this;
        cur_note->convert_data(p_time_changes, p_velocities);
    }
    
    for (i = 0; i < ending_notes.size(); i++) {
        Ref<RubiconNoteData> cur_note = ending_notes[i];
        cur_note->ending_row = this;
        cur_note->convert_data(p_time_changes, p_velocities);
    }
}

bool RubiconRowData::is_of_value(const Variant &p_row, const uint8_t p_offset, const uint8_t p_quant) {
    RubiconRowData* casted_row = Object::cast_to<RubiconRowData>(p_row);
    if (casted_row != nullptr)
        return casted_row->offset == p_offset && casted_row->quant == p_quant;
    
    return false;
}

bool RubiconRowData::compare_rows(const Variant &p_a, const Variant &p_b) {
    RubiconRowData* row_a = Object::cast_to<RubiconRowData>(p_a);
    RubiconRowData* row_b = Object::cast_to<RubiconRowData>(p_b);
    if (row_a != nullptr && row_b != nullptr)
        return ((float)row_a->offset / (float)row_a->quant) < ((float)row_b->offset / (float)row_b->quant);

    return p_a < p_b;
}

void RubiconRowData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_section", "section"), &RubiconRowData::set_section);
    ClassDB::bind_method("get_section", &RubiconRowData::get_section);
    ClassDB::bind_method(D_METHOD("set_lane_priority", "lane_priority"), &RubiconRowData::set_lane_priority);
    ClassDB::bind_method("get_lane_priority", &RubiconRowData::get_lane_priority);
    ClassDB::bind_method(D_METHOD("set_offset", "offset"), &RubiconRowData::set_offset);
    ClassDB::bind_method("get_offset", &RubiconRowData::get_offset);
    ClassDB::bind_method(D_METHOD("set_quant", "quant"), &RubiconRowData::set_quant);
    ClassDB::bind_method("get_quant", &RubiconRowData::get_quant);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "section", PROPERTY_HINT_RESOURCE_TYPE, "RubiconSectionData"), "set_section", "get_section");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lane_priority"), "set_lane_priority", "get_lane_priority");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "offset"), "set_offset", "get_offset");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "quant", PROPERTY_HINT_ENUM, "4th:4,8th:8,12th:12,16th:16,24nd:24,32nd:32,48th:48,64th:64,192nd:192"), "set_quant", "get_quant");

    ClassDB::bind_method(D_METHOD("get_notes", "include_ends"), &RubiconRowData::get_notes);
    ClassDB::bind_method(D_METHOD("get_note_at_lane", "lane", "include_ends"), &RubiconRowData::get_note_at_lane);
    ClassDB::bind_method(D_METHOD("get_notes_of_type", "note_type", "include_ends"), &RubiconRowData::get_notes_of_type);
    ClassDB::bind_method("get_note_types", &RubiconRowData::get_note_types);

    ClassDB::bind_method(D_METHOD("set_starting_notes", "starting_notes"), &RubiconRowData::set_starting_notes);
    ClassDB::bind_method("get_starting_notes", &RubiconRowData::get_starting_notes);
    ClassDB::bind_method(D_METHOD("add_starting_note", "note"), &RubiconRowData::add_starting_note);
    
    ClassDB::bind_method(D_METHOD("set_ending_notes", "ending_notes"), &RubiconRowData::set_ending_notes);
    ClassDB::bind_method("get_ending_notes", &RubiconRowData::get_ending_notes);
    ClassDB::bind_method(D_METHOD("add_ending_note", "note"), &RubiconRowData::add_ending_note);
    
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "starting_notes", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconNoteData")), "set_starting_notes", "get_starting_notes");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "ending_notes", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconNoteData")), "set_ending_notes", "get_ending_notes");

    ClassDB::bind_method(D_METHOD("remove_note", "note"), &RubiconRowData::remove_note);
    ClassDB::bind_method(D_METHOD("remove_note_at_lane", "lane"), &RubiconRowData::remove_note_at_lane);

    ClassDB::bind_method(D_METHOD("has_note_at_lane", "lane"), &RubiconRowData::has_note_at_lane);
    ClassDB::bind_method(D_METHOD("is_note_starting", "note"), &RubiconRowData::is_note_starting);
    ClassDB::bind_method(D_METHOD("is_note_ending", "note"), &RubiconRowData::is_note_ending);

    ClassDB::bind_method(D_METHOD("convert_data", "time_changes", "velocities"), &RubiconRowData::convert_data);

    ClassDB::bind_static_method("RubiconRowData", D_METHOD("is_of_value", "row", "offset", "quant"), &RubiconRowData::is_of_value);
    ClassDB::bind_static_method("RubiconRowData", D_METHOD("compare_rows", "a", "b"), &RubiconRowData::compare_rows);
}