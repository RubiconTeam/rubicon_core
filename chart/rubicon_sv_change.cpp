#include "rubicon_sv_change.h"
#include "../rubicon_conductor.h"

void RubiconSvChange::set_time(const float p_time) {
    time = p_time;
}

float RubiconSvChange::get_time() const {
    return time;
}

void RubiconSvChange::set_multiplier(const float p_multiplier) {
    multiplier = p_multiplier;
}

float RubiconSvChange::get_multiplier() const {
    return multiplier;
}

float RubiconSvChange::get_ms_time() const {
    return ms_time;
}

float RubiconSvChange::get_position() const {
    return position;
}

void RubiconSvChange::convert_data(const TypedArray<RubiconTimeChange> &p_time_changes, const Ref<RubiconSvChange> p_previous_change) {
    Ref<RubiconTimeChange> time_change = p_time_changes.back();
    for (int i = 0; i < p_time_changes.size(); i++) {
        Ref<RubiconTimeChange> current_time_change = p_time_changes[i];

        if (current_time_change->time > time) {
            Ref<RubiconTimeChange> previous_time_change = p_time_changes[i - 1];

            time_change = previous_time_change;
            break;
        }
    }

    ms_time = RubiconConductor::measure_to_ms(time - time_change->time, time_change->bpm, time_change->time_signature_numerator) + time_change->ms_time;
    if (p_previous_change.is_null())
        return;
    
    position = p_previous_change->position + (ms_time - p_previous_change->ms_time) * p_previous_change->multiplier;
}

void RubiconSvChange::_bind_methods() {
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_time", "time"), &RubiconSvChange::set_time);
    ClassDB::bind_method("get_time", &RubiconSvChange::get_time);
    
    ClassDB::bind_method(D_METHOD("set_multiplier", "multiplier"), &RubiconSvChange::set_multiplier);
    ClassDB::bind_method("get_multiplier", &RubiconSvChange::get_multiplier);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "multiplier"), "set_multiplier", "get_multiplier");

    // Methods
    ClassDB::bind_method("get_ms_time", &RubiconSvChange::get_ms_time);
    ClassDB::bind_method("get_position", &RubiconSvChange::get_position);

    ClassDB::bind_method("convert_data", &RubiconSvChange::convert_data);
}