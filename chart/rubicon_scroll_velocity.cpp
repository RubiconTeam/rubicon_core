#include "rubicon_scroll_velocity.h"
#include "../rubicon_conductor.h"

void RubiconScrollVelocity::set_time(const float p_time) {
    time = p_time;
}

float RubiconScrollVelocity::get_time() const {
    return time;
}

void RubiconScrollVelocity::set_multiplier(const float p_multiplier) {
    multiplier = p_multiplier;
}

float RubiconScrollVelocity::get_multiplier() const {
    return multiplier;
}

float RubiconScrollVelocity::get_ms_time() const {
    return ms_time;
}

float RubiconScrollVelocity::get_position() const {
    return position;
}

void RubiconScrollVelocity::convert_data(const Ref<RubiconTimeChangeArray> &p_time_changes, const Ref<RubiconScrollVelocity> p_previous) {
    ERR_FAIL_COND_MSG(p_time_changes.is_null() || !p_time_changes.is_valid(), "The provided time change array is null or invalid.");
    if (!p_time_changes->is_valid())
        return;
    
    Ref<RubiconTimeChange> time_change = p_time_changes->data.back();
    for (int i = 0; i < p_time_changes->data.size(); i++) {
        Ref<RubiconTimeChange> current_time_change = p_time_changes->data[i];

        if (current_time_change->time > time) {
            Ref<RubiconTimeChange> previous_time_change = p_time_changes->data[i - 1];

            time_change = previous_time_change;
            break;
        }
    }

    ms_time = RubiconConductor::measure_to_ms(time - time_change->time, time_change->bpm, time_change->time_signature_numerator) + time_change->ms_time;
    if (p_previous.is_null())
        return;
    
    position = p_previous->position + (ms_time - p_previous->ms_time) * p_previous->multiplier;
}

void RubiconScrollVelocity::_bind_methods() {
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_time", "time"), &RubiconScrollVelocity::set_time);
    ClassDB::bind_method("get_time", &RubiconScrollVelocity::get_time);
    
    ClassDB::bind_method(D_METHOD("set_multiplier", "multiplier"), &RubiconScrollVelocity::set_multiplier);
    ClassDB::bind_method("get_multiplier", &RubiconScrollVelocity::get_multiplier);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "multiplier"), "set_multiplier", "get_multiplier");

    // Methods
    ClassDB::bind_method("get_ms_time", &RubiconScrollVelocity::get_ms_time);
    ClassDB::bind_method("get_position", &RubiconScrollVelocity::get_position);

    ClassDB::bind_method("convert_data", &RubiconScrollVelocity::convert_data);
}