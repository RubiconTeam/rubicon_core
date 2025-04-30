#include "rubicon_core/chart/time_change.h"
#include "rubicon_core/conductor.h"

void TimeChange::set_time(const float p_time) {
    time = p_time;
}

float TimeChange::get_time() const {
    return time;
}

float TimeChange::get_measure_value() {
    return Conductor::get_singleton()->measure_to_ms(1.0f, bpm, time_signature_numerator);
}

float TimeChange::get_beat_value() {
    return Conductor::get_singleton()->beats_to_ms(1.0f, bpm);
}

float TimeChange::get_step_value() {
    return Conductor::get_singleton()->steps_to_ms(1.0f, bpm, time_signature_denominator);
}

void TimeChange::_bind_methods() {
    // Methods
    ClassDB::bind_method("get_measure_value", &TimeChange::get_measure_value);
    ClassDB::bind_method("get_beat_value", &TimeChange::get_beat_value);
    ClassDB::bind_method("get_step_value", &TimeChange::get_step_value);

    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_time", "time"), &TimeChange::set_time);
	ClassDB::bind_method(D_METHOD("get_time"), &TimeChange::get_time);

    // Fields
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_ime");
}