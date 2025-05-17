#include "rubicon_time_change.h"
#include "../rubicon_conductor.h"

void RubiconTimeChange::set_time(const float p_time) {
    time = p_time;
}

float RubiconTimeChange::get_time() const {
    return time;
}

void RubiconTimeChange::set_bpm(const float p_bpm) {
    bpm = p_bpm;
}

float RubiconTimeChange::get_bpm() const {
    return bpm;
}

void RubiconTimeChange::set_time_signature_numerator(const float p_time_signature_numerator) {
    time_signature_numerator = p_time_signature_numerator;
}

float RubiconTimeChange::get_time_signature_numerator() const {
    return time_signature_numerator;
}

void RubiconTimeChange::set_time_signature_denominator(const float p_time_signature_denominator) {
    time_signature_denominator = p_time_signature_denominator;
}

float RubiconTimeChange::get_time_signature_denominator() const {
    return time_signature_denominator;
}

float RubiconTimeChange::get_ms_time() const {
    return ms_time;
}

float RubiconTimeChange::get_measure_value() const {
    return RubiconConductor::measure_to_ms(1.0f, bpm, time_signature_numerator);
}

float RubiconTimeChange::get_beat_value() const {
    return RubiconConductor::beats_to_ms(1.0f, bpm);
}

float RubiconTimeChange::get_step_value() const {
    return RubiconConductor::steps_to_ms(1.0f, bpm, time_signature_denominator);
}

void RubiconTimeChange::_bind_methods() {
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_time", "time"), &RubiconTimeChange::set_time);
	ClassDB::bind_method(D_METHOD("get_time"), &RubiconTimeChange::get_time);
    ClassDB::bind_method(D_METHOD("set_bpm", "bpm"), &RubiconTimeChange::set_bpm);
	ClassDB::bind_method(D_METHOD("get_bpm"), &RubiconTimeChange::get_bpm);
    ClassDB::bind_method(D_METHOD("set_time_signature_numerator", "time_signature_numerator"), &RubiconTimeChange::set_time_signature_numerator);
	ClassDB::bind_method(D_METHOD("get_time_signature_numerator"), &RubiconTimeChange::get_time_signature_numerator);
    ClassDB::bind_method(D_METHOD("set_time_signature_denominator", "time_signature_denominator"), &RubiconTimeChange::set_time_signature_denominator);
	ClassDB::bind_method(D_METHOD("get_time_signature_denominator"), &RubiconTimeChange::get_time_signature_denominator);

	ClassDB::bind_method(D_METHOD("get_ms_time"), &RubiconTimeChange::get_ms_time);
    
    // Fields
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "bpm"), "set_bpm", "get_bpm");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_signature_numerator"), "set_time_signature_numerator", "get_time_signature_numerator");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_signature_denominator"), "set_time_signature_denominator", "get_time_signature_denominator");
    
    // Methods
    ClassDB::bind_method("get_measure_value", &RubiconTimeChange::get_measure_value);
    ClassDB::bind_method("get_beat_value", &RubiconTimeChange::get_beat_value);
    ClassDB::bind_method("get_step_value", &RubiconTimeChange::get_step_value);
}