#include "modules/rubicon_core/chart/time_change.h"
#include "modules/rubicon_core/conductor.h"

void TimeChange::set_time(const float p_time) {
    time = p_time;
}

float TimeChange::get_time() const {
    return time;
}

void TimeChange::set_bpm(const float p_bpm) {
    bpm = p_bpm;
}

float TimeChange::get_bpm() const {
    return bpm;
}

void TimeChange::set_time_signature_numerator(const float p_time_signature_numerator) {
    time_signature_numerator = p_time_signature_numerator;
}

float TimeChange::get_time_signature_numerator() const {
    return time_signature_numerator;
}

void TimeChange::set_time_signature_denominator(const float p_time_signature_denominator) {
    time_signature_denominator = p_time_signature_denominator;
}

float TimeChange::get_time_signature_denominator() const {
    return time_signature_denominator;
}

void TimeChange::set_ms_time(const float p_ms_time) {
    ms_time = p_ms_time;
}

float TimeChange::get_ms_time() const {
    return ms_time;
}

float TimeChange::get_measure_value() const {
    return Conductor::get_singleton()->measure_to_ms(1.0f, bpm, time_signature_numerator);
}

float TimeChange::get_beat_value() const {
    return Conductor::get_singleton()->beats_to_ms(1.0f, bpm);
}

float TimeChange::get_step_value() const {
    return Conductor::get_singleton()->steps_to_ms(1.0f, bpm, time_signature_denominator);
}

void TimeChange::_bind_methods() {
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_time", "time"), &TimeChange::set_time);
	ClassDB::bind_method(D_METHOD("get_time"), &TimeChange::get_time);
    ClassDB::bind_method(D_METHOD("set_bpm", "bpm"), &TimeChange::set_bpm);
	ClassDB::bind_method(D_METHOD("get_bpm"), &TimeChange::get_bpm);
    ClassDB::bind_method(D_METHOD("set_time_signature_numerator", "time_signature_numerator"), &TimeChange::set_time_signature_numerator);
	ClassDB::bind_method(D_METHOD("get_time_signature_numerator"), &TimeChange::get_time_signature_numerator);
    ClassDB::bind_method(D_METHOD("set_time_signature_denominator", "time_signature_denominator"), &TimeChange::set_time_signature_denominator);
	ClassDB::bind_method(D_METHOD("get_time_signature_denominator"), &TimeChange::get_time_signature_denominator);

    ClassDB::bind_method(D_METHOD("set_ms_time", "ms_time"), &TimeChange::set_ms_time);
	ClassDB::bind_method(D_METHOD("get_ms_time"), &TimeChange::get_ms_time);
    
    // Fields
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "bpm"), "set_bpm", "get_bpm");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_signature_numerator"), "set_time_signature_numerator", "get_time_signature_numerator");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_signature_denominator"), "set_time_signature_denominator", "get_time_signature_denominator");

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ms_time"), "set_ms_time", "get_ms_time");
    
    // Methods
    ClassDB::bind_method("get_measure_value", &TimeChange::get_measure_value);
    ClassDB::bind_method("get_beat_value", &TimeChange::get_beat_value);
    ClassDB::bind_method("get_step_value", &TimeChange::get_step_value);
}