#include "rubicon_core/conductor.h"
#include "scene/main/scene_tree.h"
#include "core/variant/callable.h"
#include "core/os/time.h"

Conductor *Conductor::singleton = nullptr;

Conductor *Conductor::get_singleton() {
	return singleton;
}

void Conductor::_process_internal() {
    if (!playing) {
        _relative_start_time = Time::get_singleton()->get_unix_time_from_system();
        _relative_time_offset = _time;
    }

    while (time_change_index < _time_changes.size() - 1) {
        Ref<TimeChange> nextTimeChange = _time_changes[time_change_index + 1];
        if (nextTimeChange->ms_time / 1000.0f > get_time())
            break;
            
        time_change_index++;
        
        //emit_signal("bpm_changed", )
    }
}

float Conductor::measure_to_ms(float p_measure, float p_bpm, float p_time_signature_numerator) const {
    return p_measure * (60000.0f / (p_bpm / p_time_signature_numerator));
}

float Conductor::beats_to_ms(float p_beat, float p_bpm) const {
    return p_beat * (60000.0f / p_bpm);
}

float Conductor::steps_to_ms(float p_step, float p_bpm, float p_time_signature_denominator) const {
    return p_step * (60000.0f / p_bpm / p_time_signature_denominator);
}

float Conductor::ms_to_measures(float p_ms_time, const TypedArray<TimeChange> &p_time_changes) const {
    Ref<TimeChange> time_change = p_time_changes.back();
    for (int i = 1; i < p_time_changes.size(); i++) {
        Ref<TimeChange> cur_time_change = p_time_changes[i];
        if (cur_time_change->ms_time > p_ms_time)
        {
            time_change = p_time_changes[i - 1];
            break;
        }
    }

    float measure_value = measure_to_ms(1, time_change->bpm, time_change->time_signature_numerator);
    float offset = p_ms_time - time_change->ms_time;
    return time_change->time + (offset / measure_value);
}

void Conductor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("measures_to_ms", "p_measure", "p_bpm", "p_time_signature_numerator"), &Conductor::measure_to_ms;
}

Conductor::Conductor() {
	singleton = this;
    SceneTree::get_singleton()->connect("process_frame", Callable(this, "_Process"));
}

Conductor::~Conductor() {
	if (singleton == this) {
		singleton = nullptr;
	}
}