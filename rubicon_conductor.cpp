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
    if (!_validate_time_change_list()) {
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
    _time_changed(_time - old_time);
}

float RubiconConductor::get_time() const {
    return playing ? float_t(Time::get_singleton()->get_unix_time_from_system() - _relative_start_time + _relative_time_offset) : _time;
}

void RubiconConductor::set_time_change_list(const TypedArray<RubiconTimeChange> &p_time_changes) {
    _time_change_list = p_time_changes;
    time_change_index = 0;
    emit_signal(SNAME("time_change_reached"), get_current_time_change());
}

TypedArray<RubiconTimeChange> RubiconConductor::get_time_change_list() const {
    return _time_change_list;
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
    _time_change_list.clear();
    time_change_index = 0;
    stop();
}

float RubiconConductor::get_current_step() {
    if (_time_change_list.is_empty())
        return 0.0f;

    float time = get_time();
    if (_cached_step_time == time)
        return _cached_step;

    Ref<RubiconTimeChange> time_change = get_current_time_change();
    if (_time_change_list.size() <= 1)
        return time / (60.0f / (time_change->bpm * time_change->time_signature_denominator));
    
    _cached_step_time = time;
    _cached_step = (time - time_change->ms_time / 1000.0f) / (60.0f / (time_change->bpm * time_change->time_signature_denominator)) + (time_change->time * time_change->time_signature_numerator * time_change->time_signature_denominator);
    return _cached_step;
}

float RubiconConductor::get_current_beat() {
    if (_time_change_list.is_empty())
        return 0.0f;

    float time = get_time();
    if (_cached_beat_time == time)
        return _cached_beat;
    
    Ref<RubiconTimeChange> time_change = get_current_time_change();
    if (_time_change_list.size() <= 1)
        return time / (60.0f / time_change->bpm);
    
    _cached_beat_time = time;
    _cached_beat = (time - time_change->ms_time / 1000.0f) / (60.0f / time_change->bpm) + time_change->time * time_change->time_signature_numerator;
    return _cached_beat;
}

float RubiconConductor::get_current_measure() {
    if (_time_change_list.is_empty())
        return 0.0f;

    float time = get_time();
    if (_cached_measure_time == time)
        return _cached_measure;
    
    Ref<RubiconTimeChange> time_change = get_current_time_change();
    if (_time_change_list.size() <= 1)
        return time / (60.0f / (time_change->bpm / time_change->time_signature_numerator));
    
    _cached_measure_time = time;
    _cached_measure = (time - time_change->ms_time / 1000.0f) / (60.0f / (time_change->bpm / time_change->time_signature_numerator)) + time_change->time;
    return _cached_measure;
}

Ref<RubiconTimeChange> RubiconConductor::get_current_time_change() {
    if (_time_change_list.is_empty())
        return Ref<RubiconTimeChange>();

    return _time_change_list[time_change_index];
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

float RubiconConductor::measure_range_to_ms(float p_start, float p_end, const TypedArray<RubiconTimeChange> &p_time_changes) {
    ERR_FAIL_COND_V_MSG(p_start > p_end, 0.0f, "The starting point can not go above the end point.");
    ERR_FAIL_COND_V_MSG(p_end < p_start, 0.0f, "The ending point can not go below the starting point.");
    
    int array_size = p_time_changes.size();
    ERR_FAIL_COND_V_MSG(array_size == 0, 0.0f, "Time change array's size is less than 0.");

    Ref<RubiconTimeChange> prev = p_time_changes[0];
    if (array_size == 1)
        return measure_to_ms(p_end - p_start, prev->bpm, prev->time_signature_numerator);

    bool started = false;
    bool ended = false;
    float ms_result = 0.0f;
    for (int i = 1; i < array_size; i++) {
        Ref<RubiconTimeChange> cur = p_time_changes[i];
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

float RubiconConductor::ms_to_measures(float p_ms_time, const TypedArray<RubiconTimeChange> &p_time_changes)  {
    Ref<RubiconTimeChange> time_change = p_time_changes.back();
    for (int i = 1; i < p_time_changes.size(); i++) {
        Ref<RubiconTimeChange> cur_time_change = p_time_changes[i];
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

void RubiconConductor::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_READY: {
            set_process_internal(true);
        } break;
        case NOTIFICATION_INTERNAL_PROCESS: {
            if (!playing) 
                return;
            
            if (!_validate_time_change_list()) {
                set_playing(false);
                return;
            }

            float cur_time = get_time();
            _time_changed(cur_time - _time);
            _time = cur_time;
        } break;
    }
}

void RubiconConductor::_time_changed(float delta) {
    int sign = SIGN(delta);
    if (sign == 0)
        return;
    
    int i;
    int cur_measure = int32_t(Math::floor(get_current_measure()));
    int cur_beat = int32_t(Math::floor(get_current_beat()));
    int cur_step = int32_t(Math::floor(get_current_step()));
    if (sign == 1) { // Going forward
        while (time_change_index < _time_change_list.size() - 1) {
            Ref<RubiconTimeChange> next_time_change = _time_change_list[time_change_index + 1];
            if (next_time_change->ms_time / 1000.0f > get_time())
                break;
                    
            time_change_index++; 
                
            emit_signal(SNAME("time_change_reached"), next_time_change);
        }

        if (cur_measure != _last_measure)
            for (i = _last_measure + 1; i <= cur_measure; i++)
                emit_signal(SNAME("measure_hit"), i);
    
        if (cur_beat != _last_beat)
            for (i = _last_beat + 1; i <= cur_beat; i++)
                emit_signal(SNAME("beat_hit"), i);

        if (cur_step != _last_step)
            for (i = _last_step + 1; i <= cur_step; i++)
                emit_signal(SNAME("step_hit"), i);
    }

    if (sign == -1) { // Going backward
        while (time_change_index > 0) {
            Ref<RubiconTimeChange> prev_time_change = _time_change_list[time_change_index - 1];
            if (prev_time_change->ms_time / 1000.0f < get_time())
                break;
            
            time_change_index--;
                
            emit_signal(SNAME("time_change_reached"), prev_time_change);
        }

        if (cur_measure != _last_measure)
            for (i = _last_measure - 1; i >= cur_measure; i--)
                emit_signal(SNAME("measure_hit"), i);
    
        if (cur_beat != _last_beat)
            for (i = _last_beat - 1; i >= cur_beat; i--)
                emit_signal(SNAME("beat_hit"), i);

        if (cur_step != _last_step)
            for (i = _last_step - 1; i >= cur_step; i--)
                emit_signal(SNAME("step_hit"), i);
    }

    _last_measure = cur_measure;
    _last_beat = cur_beat;
    _last_step = cur_step;
}

bool RubiconConductor::_validate_time_change_list() const {
    ERR_FAIL_COND_V_MSG(_time_change_list.size() == 0, false, "Time change list needs to have at least one time change!");
    
    for (int i = 0; i < _time_change_list.size(); i++) {
        Ref<RubiconTimeChange> current = _time_change_list[i];
        ERR_FAIL_COND_V_MSG(current.is_null() || !current.is_valid(), false, "One of your time changes is either null or not valid.");
    }

    return true;
}

void RubiconConductor::_bind_methods() {
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_time", "time"), &RubiconConductor::set_time);
    ClassDB::bind_method("get_time", &RubiconConductor::get_time);
    ClassDB::bind_method(D_METHOD("set_playing", "playing"), &RubiconConductor::set_playing);
    ClassDB::bind_method("get_playing", &RubiconConductor::get_playing);
    ClassDB::bind_method(D_METHOD("set_time_change_index", "time_change_index"), &RubiconConductor::set_time_change_index);
    ClassDB::bind_method("get_time_change_index", &RubiconConductor::get_time_change_index);
    ClassDB::bind_method(D_METHOD("set_time_change_list", "time_changes"), &RubiconConductor::set_time_change_list);
    ClassDB::bind_method("get_time_change_list", &RubiconConductor::get_time_change_list);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "set_playing", "get_playing");

    ADD_GROUP("Time Changes", "time_change_");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "time_change_index"), "set_time_change_index", "get_time_change_index");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "time_change_list", PROPERTY_HINT_RESOURCE_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconTimeChange")), "set_time_change_list", "get_time_change_list");

    // Methods
    ClassDB::bind_method(D_METHOD("play", "time"), &RubiconConductor::play);
    ClassDB::bind_method("stop", &RubiconConductor::stop);

    ClassDB::bind_method("get_current_time_change", &RubiconConductor::get_current_time_change);

    ClassDB::bind_method("reset", &RubiconConductor::reset);

    ClassDB::bind_method("get_current_step", &RubiconConductor::get_current_step);
    ClassDB::bind_method("get_current_beat", &RubiconConductor::get_current_beat);
    ClassDB::bind_method("get_current_measure", &RubiconConductor::get_current_measure);

    // Utility functins
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("measures_to_ms", "measure", "bpm", "ime_signature_numerator"), &RubiconConductor::measure_to_ms);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("beats_to_ms", "beat", "bpm"), &RubiconConductor::beats_to_ms);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("steps_to_ms", "step", "bpm", "time_signature_denominator"), &RubiconConductor::steps_to_ms);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("measure_to_beats", "measure", "time_signature_numerator"), &RubiconConductor::measure_to_beats);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("measure_to_steps", "measure", "time_signature_numerator", "time_signature_denominator"), &RubiconConductor::measure_to_steps);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("beats_to_steps", "beats", "time_signature_denominator"), &RubiconConductor::beats_to_steps);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("beats_to_measures", "beats", "time_signature_numerator"), &RubiconConductor::beats_to_measures);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("steps_to_measures", "steps", "time_signature_numerator", "time_signature_denominator"), &RubiconConductor::steps_to_measures);
    ClassDB::bind_static_method("RubiconConductor", D_METHOD("ms_to_measures", "ms_time", "time_changes"), &RubiconConductor::ms_to_measures);

    // Signals
    ADD_SIGNAL(MethodInfo("beat_hit", PropertyInfo(Variant::INT, "beat")));
    ADD_SIGNAL(MethodInfo("step_hit", PropertyInfo(Variant::INT, "step")));
    ADD_SIGNAL(MethodInfo("measure_hit", PropertyInfo(Variant::INT, "measure")));
    ADD_SIGNAL(MethodInfo("time_change_reached", PropertyInfo(Variant::INT, "time_change")));
}