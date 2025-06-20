#include "rubicon_note_data.h"
#include "../rubicon_conductor.h"

void RubiconNoteData::set_parameters(const TypedDictionary<StringName, Variant> &p_parameters) {
    parameters = p_parameters;
}

TypedDictionary<StringName, Variant> RubiconNoteData::get_parameters() const {
    return parameters;
}

void RubiconNoteData::set_lane(const uint8_t p_lane) {
    lane = p_lane;
}

uint8_t RubiconNoteData::get_lane() const {
    return lane;
}

void RubiconNoteData::set_type(const StringName &p_type) {
    type = p_type;
}

StringName RubiconNoteData::get_type() const {
    return type;
}

void RubiconNoteData::set_measure_time(const float p_value) {
    measure_time = p_value;
}

float RubiconNoteData::get_measure_time() const {
    return measure_time;
}

void RubiconNoteData::set_measure_length(const float p_value) {
    measure_length = p_value;
}

float RubiconNoteData::get_measure_length() const {
    return measure_length;
}

float RubiconNoteData::get_ms_time() const {
    return ms_time;
}

float RubiconNoteData::get_ms_length() const {
    return ms_length;
}

int RubiconNoteData::get_starting_scroll_velocity() const {
    return starting_scroll_velocity;
}

int RubiconNoteData::get_ending_scroll_velocity() const {
    return ending_scroll_velocity;
}

Ref<RubiconRowData> RubiconNoteData::get_starting_row() const {
    return starting_row;
}

Ref<RubiconRowData> RubiconNoteData::get_ending_row() const {
    return ending_row;
}

void RubiconNoteData::set_internal_should_miss(const bool p_value) {
    internal_should_miss = p_value;
}

bool RubiconNoteData::get_internal_should_miss() const {
    return internal_should_miss;
}

void RubiconNoteData::set_internal_hit(const bool p_value) {
    internal_hit = p_value;
}

bool RubiconNoteData::get_internal_hit() const {
    return internal_hit;
}

void RubiconNoteData::set_internal_spawned(const bool p_value) {
    internal_spawned = p_value;
}

bool RubiconNoteData::get_internal_spawned() const {
    return internal_spawned;
}

void RubiconNoteData::set_internal_counts_towards_score(const bool p_value) {
    internal_counts_towards_score = p_value;
}

bool RubiconNoteData::get_internal_counts_towards_score() const {
    return internal_counts_towards_score;
}

void RubiconNoteData::convert_data(const TypedArray<RubiconTimeChange> &p_time_changes, const TypedArray<RubiconScrollVelocity> &p_velocities) {
    if (!starting_row.is_null()) {
        measure_time = starting_row->section->measure + (float(starting_row->offset) / float(starting_row->quant));
        measure_length = !ending_row.is_null() ? (ending_row->section->measure + (float(ending_row->offset) / float(ending_row->quant))) - measure_time : 0.0f;
    }
    
    int i = 0;

    Ref<RubiconTimeChange> starting_change = p_time_changes.back();
    for (i = 0; i < p_time_changes.size(); i++) {
        Ref<RubiconTimeChange> cur_change = p_time_changes[i];
        if (cur_change->time > measure_time) {
            starting_change = p_time_changes[i - 1];
            break;
        }
    }

    bool found_start = false;
    bool found_end = false;
    starting_scroll_velocity = ending_scroll_velocity = p_velocities.size() - 1;
    for (i = 0; i < p_velocities.size(); i++) {
        Ref<RubiconScrollVelocity> cur_change = p_velocities[i];
        if (cur_change->time > measure_time && !found_start) {
            starting_scroll_velocity = i - 1;
            found_start = true;
        }

        if (cur_change->time > measure_time + measure_length && !found_end) {
            ending_scroll_velocity = i - 1;
            found_end = true;
        }

        if (found_start && found_end)
            break;
    }

    ms_time = RubiconConductor::measure_to_ms(measure_time, starting_change->bpm, starting_change->time_signature_numerator) + starting_change->ms_time;
    ms_length = RubiconConductor::measure_range_to_ms(measure_time, measure_time + measure_length, p_time_changes);
}

bool RubiconNoteData::compare_notes_by_time(const Variant &p_a, const Variant &p_b) {
    RubiconNoteData* note_a = Object::cast_to<RubiconNoteData>(p_a);
    RubiconNoteData* note_b = Object::cast_to<RubiconNoteData>(p_b);
    if (note_a != nullptr && note_b != nullptr)
        return note_a->measure_time < note_b->measure_time;
    
    return p_a < p_b;
}

bool RubiconNoteData::compare_notes_by_lane(const Variant &p_a, const Variant &p_b) {
    RubiconNoteData* note_a = Object::cast_to<RubiconNoteData>(p_a);
    RubiconNoteData* note_b = Object::cast_to<RubiconNoteData>(p_b);
    if (note_a != nullptr && note_b != nullptr)
        return note_a->lane < note_b->lane;

    return p_a < p_b;
}

bool RubiconNoteData::is_note_lane(const Variant &p_note, const uint8_t p_lane) {
    RubiconNoteData* casted_note = Object::cast_to<RubiconNoteData>(p_note);
    if (casted_note != nullptr)
        return casted_note->lane == p_lane;

    return false;
}

bool RubiconNoteData::is_note_type(const Variant &p_note, const StringName &p_type) {
    RubiconNoteData* casted_note = Object::cast_to<RubiconNoteData>(p_note);
    if (casted_note != nullptr)
        return casted_note->type == p_type;

    return false;
}

void RubiconNoteData::_bind_methods() {
    // Getters and Setters for Properties
    ClassDB::bind_method(D_METHOD("set_parameters", "parameters"), &RubiconNoteData::set_parameters);
    ClassDB::bind_method("get_parameters", &RubiconNoteData::get_parameters);
    ClassDB::bind_method(D_METHOD("set_lane", "lane"), &RubiconNoteData::set_lane);
    ClassDB::bind_method("get_lane", &RubiconNoteData::get_lane);
    ClassDB::bind_method(D_METHOD("set_type", "type"), &RubiconNoteData::set_type);
    ClassDB::bind_method("get_type", &RubiconNoteData::get_type);
    ClassDB::bind_method(D_METHOD("set_measure_time", "measure_time"), &RubiconNoteData::set_measure_time);
    ClassDB::bind_method("get_measure_time", &RubiconNoteData::get_measure_time);
    ClassDB::bind_method(D_METHOD("set_measure_length", "measure_length"), &RubiconNoteData::set_measure_length);
    ClassDB::bind_method("get_measure_length", &RubiconNoteData::get_measure_length);
    ClassDB::bind_method(D_METHOD("set_internal_should_miss", "should_miss"), &RubiconNoteData::set_internal_should_miss);
    ClassDB::bind_method("get_internal_should_miss", &RubiconNoteData::get_internal_should_miss);
    ClassDB::bind_method(D_METHOD("set_internal_hit", "hit"), &RubiconNoteData::set_internal_hit);
    ClassDB::bind_method("get_internal_hit", &RubiconNoteData::get_internal_hit);
    ClassDB::bind_method(D_METHOD("set_internal_spawned", "spawned"), &RubiconNoteData::set_internal_spawned);
    ClassDB::bind_method("get_internal_spawned", &RubiconNoteData::get_internal_spawned);
    ClassDB::bind_method(D_METHOD("set_internal_counts_towards_score", "counts_towards_score"), &RubiconNoteData::set_internal_counts_towards_score);
    ClassDB::bind_method("get_internal_counts_towards_score", &RubiconNoteData::get_internal_counts_towards_score);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "parameters", PROPERTY_HINT_DICTIONARY_TYPE, "StringName;Variant"), "set_parameters", "get_parameters");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lane"), "set_lane", "get_lane");
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "type"), "set_type", "get_type");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "measure_time"), "set_measure_time", "get_measure_time");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "measure_length"), "set_measure_length", "get_measure_length");
    
    ADD_GROUP("Internal", "internal_");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "internal_should_miss"), "set_internal_should_miss", "get_internal_should_miss");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "internal_hit"), "set_internal_hit", "get_internal_hit");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "internal_spawned"), "set_internal_spawned", "get_internal_spawned");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "internal_counts_towards_score"), "set_internal_counts_towards_score", "get_internal_counts_towards_score");

    // Pure Getters
    ClassDB::bind_method("get_ms_time", &RubiconNoteData::get_ms_time);
    ClassDB::bind_method("get_ms_length", &RubiconNoteData::get_ms_length);
    ClassDB::bind_method("get_starting_scroll_velocity", &RubiconNoteData::get_starting_scroll_velocity);
    ClassDB::bind_method("get_ending_scroll_velocity", &RubiconNoteData::get_ending_scroll_velocity);
    ClassDB::bind_method("get_starting_row", &RubiconNoteData::get_starting_row);
    ClassDB::bind_method("get_ending_row", &RubiconNoteData::get_ending_row);

    // Methods
    ClassDB::bind_method(D_METHOD("convert_data", "time_changes", "velocities"), &RubiconNoteData::convert_data);
    
    ClassDB::bind_static_method("RubiconNoteData", D_METHOD("comapre_notes_by_time", "a", "b"), &RubiconNoteData::compare_notes_by_time);
    ClassDB::bind_static_method("RubiconNoteData", D_METHOD("compare_notes_by_lane", "a", "b"), &RubiconNoteData::compare_notes_by_lane);
    ClassDB::bind_static_method("RubiconNoteData", D_METHOD("is_note_lane", "note", "lane"), &RubiconNoteData::is_note_lane);
    ClassDB::bind_static_method("RubiconNoteData", D_METHOD("is_note_type", "a", "b"), &RubiconNoteData::is_note_type);
}