#include "rubicon_conductor.h"
#include "scene/main/scene_tree.h"
#include "core/variant/callable.h"
#include "core/os/time.h"

void RubiconConductor::set_time_change_index(const int p_time_change_index) {
    time_change_index = p_time_change_index;
}

int RubiconConductor::get_time_change_index() const {
    return time_change_index;
}

void RubiconConductor::set_playing(const bool p_playing) {
    if (!_validate_time_change_array(true)) {
        playing = false;
        return;
    }

    _time = get_time();
    playing = p_playing;
    set_time(_time);
}

bool RubiconConductor::get_playing() const {
    return playing;
}

void RubiconConductor::set_time(const float p_time) {
    float old_time = _time;

    _time = p_time;

    _relative_start_time = Time::get_singleton()->get_unix_time_from_system();
    _relative_time_offset = p_time;
    time_change_index = _time_change_array->data.find(_time_change_array->get_time_change_at_ms(_time * 1000.0f));
}

float RubiconConductor::get_time() const {
    return playing ? float_t(Time::get_singleton()->get_unix_time_from_system() - _relative_start_time + _relative_time_offset) : _time;
}

void RubiconConductor::set_time_change_array(const Ref<RubiconTimeChangeArray> &p_time_changes) {
    _time_change_array = p_time_changes;
    time_change_index = 0;
    emit_signal(SNAME("time_change_reached"), get_current_time_change());
}

Ref<RubiconTimeChangeArray> RubiconConductor::get_time_change_array() const {
    return _time_change_array;
}

void RubiconConductor::play(const float p_time) {
    set_time(p_time);
    playing = true;
}

void RubiconConductor::stop() {
    _time = 0.0;
    playing = false;
}

void RubiconConductor::reset() {
    _time_change_array = Ref<RubiconTimeChangeArray>();
    time_change_index = 0;
    stop();
}

void RubiconConductor::set_current_step(const float p_value) {
    if (!_validate_time_change_array(true))
        return;
    
    time_change_index = _time_change_array->data.find(_time_change_array->get_time_change_at_step(p_value));
    set_time(_time_change_array->get_ms_for_step(p_value) / 1000.0f);
}

float RubiconConductor::get_current_step() {
    if (!_validate_time_change_array(false))
        return 0.0f;

    float time = get_time();
    if (_cached_step_time == time)
        return _cached_step;

    Ref<RubiconTimeChange> time_change = get_current_time_change();
    if (_time_change_array->data.size() <= 1)
        return time / (60.0f / (time_change->bpm * time_change->time_signature_denominator));
    
    _cached_step_time = time;
    _cached_step = (time - time_change->ms_time / 1000.0f) / (60.0f / (time_change->bpm * time_change->time_signature_denominator)) + (time_change->time * time_change->time_signature_numerator * time_change->time_signature_denominator);
    return _cached_step;
}

void RubiconConductor::set_current_beat(const float p_value) {
    if (!_validate_time_change_array(true))
        return;
    
    time_change_index = _time_change_array->data.find(_time_change_array->get_time_change_at_beat(p_value));
    set_time(_time_change_array->get_ms_for_beat(p_value) / 1000.0f);
}

float RubiconConductor::get_current_beat() {
    if (!_validate_time_change_array(false))
        return 0.0f;

    float time = get_time();
    if (_cached_beat_time == time)
        return _cached_beat;
    
    Ref<RubiconTimeChange> time_change = get_current_time_change();
    if (_time_change_array->data.size() <= 1)
        return time / (60.0f / time_change->bpm);
    
    _cached_beat_time = time;
    _cached_beat = (time - time_change->ms_time / 1000.0f) / (60.0f / time_change->bpm) + time_change->time * time_change->time_signature_numerator;
    return _cached_beat;
}

void RubiconConductor::set_current_measure(const float p_value) {
    if (!_validate_time_change_array(true))
        return;
    
    time_change_index = _time_change_array->data.find(_time_change_array->get_time_change_at_measure(p_value));
    set_time(_time_change_array->get_ms_for_measure(p_value) / 1000.0f);
}

float RubiconConductor::get_current_measure() {
    if (!_validate_time_change_array(false))
        return 0.0f;

    float time = get_time();
    if (_cached_measure_time == time)
        return _cached_measure;
    
    Ref<RubiconTimeChange> time_change = get_current_time_change();
    if (_time_change_array->data.size() <= 1)
        return time / (60.0f / (time_change->bpm / time_change->time_signature_numerator));
    
    _cached_measure_time = time;
    _cached_measure = (time - time_change->ms_time / 1000.0f) / (60.0f / (time_change->bpm / time_change->time_signature_numerator)) + time_change->time;
    return _cached_measure;
}

Ref<RubiconTimeChange> RubiconConductor::get_current_time_change() {
    if (!_validate_time_change_array(true))
        return Ref<RubiconTimeChange>();

    return _time_change_array->data[time_change_index];
}

float RubiconConductor::measure_to_ms(float p_measure, float p_bpm, float p_time_signature_numerator)  {
    return p_measure * (60000.0f / (p_bpm / p_time_signature_numerator));
}

float RubiconConductor::beats_to_ms(float p_beat, float p_bpm)  {
    return p_beat * (60000.0f / p_bpm);
}

float RubiconConductor::steps_to_ms(float p_step, float p_bpm, float p_time_signature_denominator)  {
    return p_step * (60000.0f / p_bpm / p_time_signature_denominator);
}

float RubiconConductor::measure_range_to_ms(float p_start, float p_end, const Ref<RubiconTimeChangeArray> &p_time_changes) {
    ERR_FAIL_COND_V_MSG(p_time_changes.is_null() || !p_time_changes.is_valid(), 0.0f, "The provided time change array is null or invalid.");
    if (!p_time_changes->is_valid())
        return 0.0;

    ERR_FAIL_COND_V_MSG(p_start > p_end, 0.0f, "The starting point can not go above the end point.");
    ERR_FAIL_COND_V_MSG(p_end < p_start, 0.0f, "The ending point can not go below the starting point.");
    
    int array_size = p_time_changes->data.size();
    ERR_FAIL_COND_V_MSG(array_size == 0, 0.0f, "Time change array's size is less than 0.");

    Ref<RubiconTimeChange> prev = p_time_changes->data[0];
    if (array_size == 1)
        return measure_to_ms(p_end - p_start, prev->bpm, prev->time_signature_numerator);

    bool started = false;
    bool ended = false;
    float ms_result = 0.0f;
    for (int i = 1; i < array_size; i++) {
        Ref<RubiconTimeChange> cur = p_time_changes->data[i];
        if (cur->time < p_start) {
            continue;
        }

        if (!started && cur->time < p_start) {
            ms_result += measure_to_ms(cur->time - p_start, prev->bpm, prev->time_signature_numerator);
            started = true;

            if (i == array_size - 1) {
                ms_result += measure_to_ms(p_end - prev->time, prev->bpm, prev->time_signature_numerator);
                break;
            }

            continue;
        }

        ended = cur->time > p_end || i == array_size - 1;
        if (ended) {
            ms_result += measure_to_ms(p_end - prev->time, prev->bpm, prev->time_signature_numerator);
            break;
        }
        
        ms_result += measure_to_ms(cur->time - prev->time, prev->bpm, prev->time_signature_numerator);
        prev = cur;
    }

    return ms_result;
}

float RubiconConductor::measure_to_beats(float p_measure, float p_time_signature_numerator)  {
    return p_measure * p_time_signature_numerator;
}

float RubiconConductor::measure_to_steps(float p_measure, float p_time_signature_numerator, float p_time_signature_denominator)  {
    return beats_to_steps(measure_to_beats(p_measure, p_time_signature_numerator), p_time_signature_denominator);
}

float RubiconConductor::beats_to_steps(float p_beats, float p_time_signature_denominator)  {
    return p_beats * p_time_signature_denominator;
}

float RubiconConductor::beats_to_measures(float p_beats, float p_time_signature_numerator)  {
    return p_beats / p_time_signature_numerator;
}

float RubiconConductor::steps_to_measures(float p_steps,float p_time_signature_numerator, float p_time_signature_denominator)  {
    return p_steps / (p_time_signature_numerator * p_time_signature_denominator);
}

void RubiconConductor::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_READY: {
            set_process_internal(true);
        } break;
        case NOTIFICATION_INTERNAL_PROCESS: {
            if (!playing) 
                return;
            
            if (!_validate_time_change_array(true)) {
                set_playing(false);
                return;
            }

            time_change_index = _time_change_array->data.find(_time_change_array->get_time_change_at_measure(get_current_measure()));
            _time = get_time();
        } break;
    }
}

bool RubiconConductor::_validate_time_change_array(const bool p_error) const {
    if (_time_change_array.is_null() || !_time_change_array.is_valid()) {
        if (p_error)
            ERR_FAIL_V_MSG(false, "Time change array object is null or not valid");
        
        return false;
    }
    
    return _time_change_array->is_valid(p_error);
}

void RubiconConductor::_bind_methods() {
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_time", "time"), &RubiconConductor::set_time);
    ClassDB::bind_method("get_time", &RubiconConductor::get_time);
    ClassDB::bind_method(D_METHOD("set_playing", "playing"), &RubiconConductor::set_playing);
    ClassDB::bind_method("get_playing", &RubiconConductor::get_playing);
    ClassDB::bind_method(D_METHOD("set_time_change_index", "time_change_index"), &RubiconConductor::set_time_change_index);
    ClassDB::bind_method("get_time_change_index", &RubiconConductor::get_time_change_index);
    ClassDB::bind_method(D_METHOD("set_time_change_array", "time_changes"), &RubiconConductor::set_time_change_array);
    ClassDB::bind_method("get_time_change_array", &RubiconConductor::get_time_change_array);

    ClassDB::bind_method(D_METHOD("set_current_step", "value"), &RubiconConductor::set_current_step);
    ClassDB::bind_method("get_current_step", &RubiconConductor::get_current_step);

    ClassDB::bind_method(D_METHOD("set_current_beat", "value"), &RubiconConductor::set_current_beat);
    ClassDB::bind_method("get_current_beat", &RubiconConductor::get_current_beat);
    
    ClassDB::bind_method(D_METHOD("set_current_measure", "value"), &RubiconConductor::set_current_measure);
    ClassDB::bind_method("get_current_measure", &RubiconConductor::get_current_measure);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "set_playing", "get_playing");

    ADD_GROUP("Time Changes", "time_change_");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "time_change_index"), "set_time_change_index", "get_time_change_index");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "time_change_array", PROPERTY_HINT_RESOURCE_TYPE, "RubiconTimeChangeArray"), "set_time_change_array", "get_time_change_array");

    ADD_GROUP("Current", "current_");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "current_measure", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "set_current_measure", "get_current_measure");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "current_beat", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "set_current_beat", "get_current_beat");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "current_step", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "set_current_step", "get_current_step");

    // Methods
    ClassDB::bind_method(D_METHOD("play", "time"), &RubiconConductor::play);
    ClassDB::bind_method("stop", &RubiconConductor::stop);

    ClassDB::bind_method("get_current_time_change", &RubiconConductor::get_current_time_change);

    ClassDB::bind_method("reset", &RubiconConductor::reset);

    // Utility functins
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("measures_to_ms", "measure", "bpm", "ime_signature_numerator"), &RubiconConductor::measure_to_ms);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("beats_to_ms", "beat", "bpm"), &RubiconConductor::beats_to_ms);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("steps_to_ms", "step", "bpm", "time_signature_denominator"), &RubiconConductor::steps_to_ms);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("measure_to_beats", "measure", "time_signature_numerator"), &RubiconConductor::measure_to_beats);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("measure_to_steps", "measure", "time_signature_numerator", "time_signature_denominator"), &RubiconConductor::measure_to_steps);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("beats_to_steps", "beats", "time_signature_denominator"), &RubiconConductor::beats_to_steps);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("beats_to_measures", "beats", "time_signature_numerator"), &RubiconConductor::beats_to_measures);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("steps_to_measures", "steps", "time_signature_numerator", "time_signature_denominator"), &RubiconConductor::steps_to_measures);

    // Signals
    ADD_SIGNAL(MethodInfo("beat_hit", PropertyInfo(Variant::INT, "beat")));
    ADD_SIGNAL(MethodInfo("step_hit", PropertyInfo(Variant::INT, "step")));
    ADD_SIGNAL(MethodInfo("measure_hit", PropertyInfo(Variant::INT, "measure")));
    ADD_SIGNAL(MethodInfo("time_change_reached", PropertyInfo(Variant::INT, "time_change")));
}