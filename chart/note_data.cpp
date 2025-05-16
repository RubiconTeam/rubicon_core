#include "note_data.h"
#include "../conductor.h"

void NoteData::set_lane(const uint8_t p_lane) {
    lane = p_lane;
}

uint8_t NoteData::get_lane() const {
    return lane;
}

void NoteData::set_type(const StringName &p_type) {
    type = p_type;
}

StringName NoteData::get_type() const {
    return type;
}

void NoteData::set_measure_time(const float p_value) {
    measure_time = p_value;
}

float NoteData::get_measure_time() const {
    return measure_time;
}

void NoteData::set_measure_length(const float p_value) {
    measure_length = p_value;
}

float NoteData::get_measure_length() const {
    return measure_length;
}

float NoteData::get_ms_time() const {
    return ms_time;
}

float NoteData::get_ms_length() const {
    return ms_length;
}

int NoteData::get_starting_scroll_velocity() const {
    return starting_scroll_velocity;
}

int NoteData::get_ending_scroll_velocity() const {
    return ending_scroll_velocity;
}

Ref<RowData> NoteData::get_starting_row() const {
    return starting_row;
}

Ref<RowData> NoteData::get_ending_row() const {
    return ending_row;
}

void NoteData::set_should_miss(const bool p_value) {
    should_miss = p_value;
}

bool NoteData::get_should_miss() const {
    return should_miss;
}

void NoteData::set_hit(const bool p_value) {
    hit = p_value;
}

bool NoteData::get_hit() const {
    return hit;
}

void NoteData::set_spawned(const bool p_value) {
    spawned = p_value;
}

bool NoteData::get_spawned() const {
    return spawned;
}

void NoteData::set_counts_towards_score(const bool p_value) {
    counts_towards_score = p_value;
}

bool NoteData::get_counts_towards_score() const {
    return counts_towards_score;
}

void NoteData::convert_data(const TypedArray<TimeChange> &p_time_changes, const TypedArray<SvChange> &p_sv_changes) {
    if (!starting_row.is_null()) {
        measure_time = starting_row->section->measure + (float(starting_row->offset) / float(starting_row->quant));
        measure_length = !ending_row.is_null() ? (ending_row->section->measure + (float(ending_row->offset) / float(ending_row->quant))) - measure_time : 0.0f;
    }
    
    int i = 0;

    Ref<TimeChange> starting_change = p_time_changes.back();
    for (i = 0; i < p_time_changes.size(); i++) {
        Ref<TimeChange> cur_change = p_time_changes[i];
        if (cur_change->time > measure_time) {
            starting_change = p_time_changes[i - 1];
            break;
        }
    }

    bool found_start = false;
    bool found_end = false;
    starting_scroll_velocity = ending_scroll_velocity = p_sv_changes.size() - 1;
    for (i = 0; i < p_sv_changes.size(); i++) {
        Ref<SvChange> cur_change = p_sv_changes[i];
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

    ms_time = Conductor::measure_to_ms(measure_time, starting_change->bpm, starting_change->time_signature_numerator) + starting_change->ms_time;
    ms_length = Conductor::measure_range_to_ms(measure_time, measure_time + measure_length, p_time_changes);
}

bool NoteData::compare_notes_by_lane(const Variant &p_a, const Variant &p_b) {
    NoteData* note_a = Object::cast_to<NoteData>(p_a);
    NoteData* note_b = Object::cast_to<NoteData>(p_b);
    if (note_a != nullptr && note_b != nullptr)
        return note_a->lane < note_b->lane;

    return p_a < p_b;
}

bool NoteData::is_note_lane(const Variant &p_note, const uint8_t p_lane) {
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

void NoteData::_bind_methods() {
    // Getters and Setters for Properties
    ClassDB::bind_method(D_METHOD("set_lane", "lane"), &NoteData::set_lane);
    ClassDB::bind_method("get_lane", &NoteData::get_lane);
    ClassDB::bind_method(D_METHOD("set_type", "type"), &NoteData::set_type);
    ClassDB::bind_method("get_type", &NoteData::get_type);
    ClassDB::bind_method(D_METHOD("set_measure_time", "measure_time"), &NoteData::set_measure_time);
    ClassDB::bind_method("get_measure_time", &NoteData::get_measure_time);
    ClassDB::bind_method(D_METHOD("set_measure_length", "measure_length"), &NoteData::set_measure_length);
    ClassDB::bind_method("get_measure_length", &NoteData::get_measure_length);
    ClassDB::bind_method(D_METHOD("set_should_miss", "should_miss"), &NoteData::set_should_miss);
    ClassDB::bind_method("get_should_miss", &NoteData::get_should_miss);
    ClassDB::bind_method(D_METHOD("set_hit", "hit"), &NoteData::set_hit);
    ClassDB::bind_method("get_hit", &NoteData::get_hit);
    ClassDB::bind_method(D_METHOD("set_spawned", "spawned"), &NoteData::set_spawned);
    ClassDB::bind_method("get_spawned", &NoteData::get_spawned);
    ClassDB::bind_method(D_METHOD("set_counts_towards_score", "counts_towards_score"), &NoteData::set_counts_towards_score);
    ClassDB::bind_method("get_counts_towards_score", &NoteData::get_counts_towards_score);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lane"), "set_lane", "get_lane");
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "type"), "set_type", "get_type");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "measure_time"), "set_measure_time", "get_measure_time");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "measure_length"), "set_measure_length", "get_measure_length");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "should_miss"), "set_should_miss", "get_should_miss");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hit"), "set_hit", "get_hit");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "spawned"), "set_spawned", "get_spawned");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "counts_towards_score"), "set_counts_towards_score", "get_counts_towards_score");

    // Pure Getters
    ClassDB::bind_method("get_ms_time", &NoteData::get_ms_time);
    ClassDB::bind_method("get_ms_length", &NoteData::get_ms_length);
    ClassDB::bind_method("get_starting_scroll_velocity", &NoteData::get_starting_scroll_velocity);
    ClassDB::bind_method("get_ending_scroll_velocity", &NoteData::get_ending_scroll_velocity);
    ClassDB::bind_method("get_starting_row", &NoteData::get_starting_row);
    ClassDB::bind_method("get_ending_row", &NoteData::get_ending_row);

    // Methods
    ClassDB::bind_method(D_METHOD("convert_data", "time_changes", "sv_changes"), &NoteData::convert_data);
    
    ClassDB::bind_static_method("NoteData", D_METHOD("compare_notes_by_lane", "a", "b"), &NoteData::compare_notes_by_lane);
    ClassDB::bind_static_method("NoteData", D_METHOD("is_note_lane", "note", "lane"), &NoteData::is_note_lane);
    ClassDB::bind_static_method("NoteData", D_METHOD("is_note_type", "a", "b"), &NoteData::is_note_type);
}