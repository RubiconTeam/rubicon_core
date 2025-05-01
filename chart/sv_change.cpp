#include "sv_change.h"
#include "../conductor.h"

void SvChange::set_time(const float p_time) {
    time = p_time;
}

float SvChange::get_time() const {
    return time;
}

void SvChange::set_multiplier(const float p_multiplier) {
    multiplier = p_multiplier;
}

float SvChange::get_multiplier() const {
    return multiplier;
}

float SvChange::get_ms_time() const {
    return ms_time;
}

float SvChange::get_position() const {
    return position;
}

void SvChange::convert_data(const TypedArray<TimeChange> &p_time_changes, const Ref<SvChange> p_previous_change) {
    Ref<TimeChange> time_change = p_time_changes.back();
    for (int i = 0; i < p_time_changes.size(); i++) {
        Ref<TimeChange> current_time_change = p_time_changes[i];

        if (current_time_change->time > time) {
            Ref<TimeChange> previous_time_change = p_time_changes[i - 1];

            time_change = previous_time_change;
            break;
        }
    }

    ms_time = Conductor::measure_to_ms(time - time_change->time, time_change->bpm, time_change->time_signature_numerator) + time_change->ms_time;
    if (p_previous_change.is_null())
        return;
    
    position = p_previous_change->position + (ms_time - p_previous_change->ms_time) * p_previous_change->multiplier;
}

void SvChange::_bind_methods() {
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_time", "time"), &SvChange::set_time);
    ClassDB::bind_method("get_time", &SvChange::get_time);
    
    ClassDB::bind_method(D_METHOD("set_multiplier", "multiplier"), &SvChange::set_multiplier);
    ClassDB::bind_method("get_multiplier", &SvChange::get_multiplier);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "multiplier"), "set_multiplier", "get_multiplier");

    // Methods
    ClassDB::bind_method("get_ms_time", &SvChange::get_ms_time);
    ClassDB::bind_method("get_position", &SvChange::get_position);

    ClassDB::bind_method("convert_data", &SvChange::convert_data);
}