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

void NoteData::convert_data(const TypedArray<NoteData> &p_time_changes, const TypedArray<SvChange> &p_sv_changes) {
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

bool NoteData::sort_notes_by_lane(const Variant &a, const Variant &b) {
    NoteData* note_a = Object::cast_to<NoteData>(a);
    NoteData* note_b = Object::cast_to<NoteData>(b);
    if (note_a != nullptr && note_b != nullptr)
        return note_a->lane < note_b->lane;

    return a < b;
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