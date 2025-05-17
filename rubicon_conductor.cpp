#include "rubicon_conductor.h"
#include "scene/main/scene_tree.h"
#include "core/variant/callable.h"
#include "core/os/time.h"

RubiconConductor *RubiconConductor::singleton = nullptr;

RubiconConductor *RubiconConductor::get_singleton() {
	return singleton;
}

void RubiconConductor::set_time_change_index(const int p_time_change_index) {
    time_change_index = p_time_change_index;
}

int RubiconConductor::get_time_change_index() const {
    return time_change_index;
}

void RubiconConductor::set_offset(const float p_offset) {
    offset = p_offset;
}

float RubiconConductor::get_offset() const {
    return offset;
}

void RubiconConductor::set_speed(const float p_speed) {
    speed = p_speed;
}

float RubiconConductor::get_speed() const {
    return speed;
}

void RubiconConductor::set_playing(const bool p_playing) {
    playing = p_playing;
}

bool RubiconConductor::get_playing() const {
    return playing;
}

void RubiconConductor::set_time(const float p_time) {
    set_audio_time(p_time / speed);
}

float RubiconConductor::get_time() const {
    return get_audio_time() * speed;
}

void RubiconConductor::set_audio_time(const float p_time) {
    _time = p_time;
    _relative_start_time = Time::get_singleton()->get_unix_time_from_system();
    _relative_time_offset = p_time;

    _last_measure = int32_t(Math::floor(get_current_measure()));
    _last_beat = int32_t(Math::floor(get_current_beat()));
    _last_step = int32_t(Math::floor(get_current_step()));
}

float RubiconConductor::get_audio_time() const {
    return offset + (playing ? float_t(Time::get_singleton()->get_unix_time_from_system() - _relative_start_time + _relative_time_offset) : _time != 0.0 ? float_t(_time) : 0.0f);
}

void RubiconConductor::set_time_changes(const TypedArray<RubiconTimeChange> &p_time_changes) {
    _time_changes = p_time_changes;
    time_change_index = 0;
    emit_signal(SNAME("time_change_reached"), get_current_time_change());
}

TypedArray<RubiconTimeChange> RubiconConductor::get_time_changes() const {
    return _time_changes;
}

void RubiconConductor::run_callbacks() {
    if (!playing) 
        return;

    while (time_change_index < _time_changes.size() - 1) {
        Ref<RubiconTimeChange> next_time_change = _time_changes[time_change_index + 1];
        if (next_time_change->ms_time / 1000.0f > get_time())
            break;
            
        time_change_index++;
        
        emit_signal(SNAME("time_change_reached"), next_time_change);
    }

    int cur_measure = int32_t(Math::floor(get_current_measure()));
    int cur_beat = int32_t(Math::floor(get_current_beat()));
    int cur_step = int32_t(Math::floor(get_current_step()));

    int i = 0;
    if (cur_measure != _last_measure)
        for (i = _last_measure + 1; i <= cur_measure; i++)
            emit_signal(SNAME("measure_hit"), i);
    
    if (cur_beat != _last_beat)
        for (i = _last_beat + 1; i <= cur_beat; i++)
            emit_signal(SNAME("beat_hit"), i);

    if (cur_step != _last_step)
        for (i = _last_step + 1; i <= cur_step; i++)
            emit_signal(SNAME("step_hit"), i);
    
    _last_measure = cur_measure;
    _last_beat = cur_beat;
    _last_step = cur_step;
}

void RubiconConductor::play(const float p_time) {
    set_time(p_time);
    resume();
}

void RubiconConductor::resume() {
    playing = true;
}

void RubiconConductor::pause() {
    _time = get_audio_time();
    playing = false;
}

void RubiconConductor::stop() {
    set_time(0.0);
    pause();
}

void RubiconConductor::reset() {
    _time_changes.clear();
    time_change_index = 0;
    offset = 0;
    speed = 1.0f;
    stop();
}

float RubiconConductor::get_current_step() {
    if (_time_changes.is_empty())
        return 0.0f;

    float time = get_time();
    if (_cached_step_time == time)
        return _cached_step;

    Ref<RubiconTimeChange> time_change = get_current_time_change();
    if (_time_changes.size() <= 1)
        return time / (60.0f / (time_change->bpm * time_change->time_signature_denominator));
    
    _cached_step_time = time;
    _cached_step = (time - time_change->ms_time / 1000.0f) / (60.0f / (time_change->bpm * time_change->time_signature_denominator)) + (time_change->time * time_change->time_signature_numerator * time_change->time_signature_denominator);
    return _cached_step;
}

float RubiconConductor::get_current_beat() {
    if (_time_changes.is_empty())
        return 0.0f;

    float time = get_time();
    if (_cached_beat_time == time)
        return _cached_beat;
    
    Ref<RubiconTimeChange> time_change = get_current_time_change();
    if (_time_changes.size() <= 1)
        return time / (60.0f / time_change->bpm);
    
    _cached_beat_time = time;
    _cached_beat = (time - time_change->ms_time / 1000.0f) / (60.0f / time_change->bpm) + time_change->time * time_change->time_signature_numerator;
    return _cached_beat;
}

float RubiconConductor::get_current_measure() {
    if (_time_changes.is_empty())
        return 0.0f;

    float time = get_time();
    if (_cached_measure_time == time)
        return _cached_measure;
    
    Ref<RubiconTimeChange> time_change = get_current_time_change();
    if (_time_changes.size() <= 1)
        return time / (60.0f / (time_change->bpm / time_change->time_signature_numerator));
    
    _cached_measure_time = time;
    _cached_measure = (time - time_change->ms_time / 1000.0f) / (60.0f / (time_change->bpm / time_change->time_signature_numerator)) + time_change->time;
    return _cached_measure;
}

Ref<RubiconTimeChange> RubiconConductor::get_current_time_change() {
    if (_time_changes.is_empty())
        return Ref<RubiconTimeChange>();

    return _time_changes[time_change_index];
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

void RubiconConductor::_bind_methods() {
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_time_change_index", "time_change_index"), &RubiconConductor::set_time_change_index);
    ClassDB::bind_method("get_time_change_index", &RubiconConductor::get_time_change_index);
    ClassDB::bind_method(D_METHOD("set_offset", "offset"), &RubiconConductor::set_offset);
    ClassDB::bind_method("get_offset", &RubiconConductor::get_offset);
    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &RubiconConductor::set_speed);
    ClassDB::bind_method("get_speed", &RubiconConductor::get_speed);
    ClassDB::bind_method(D_METHOD("set_playing", "playing"), &RubiconConductor::set_playing);
    ClassDB::bind_method("get_playing", &RubiconConductor::get_playing);
    ClassDB::bind_method(D_METHOD("set_time", "time"), &RubiconConductor::set_time);
    ClassDB::bind_method("get_time", &RubiconConductor::get_time);
    ClassDB::bind_method(D_METHOD("set_audio_time", "time"), &RubiconConductor::set_audio_time);
    ClassDB::bind_method("get_audio_time", &RubiconConductor::get_audio_time);
    ClassDB::bind_method(D_METHOD("set_time_changes", "time_changes"), &RubiconConductor::set_time_changes);
    ClassDB::bind_method("get_time_changes", &RubiconConductor::get_time_changes);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::INT, "time_change_index"), "set_time_change_index", "get_time_change_index");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "offset"), "set_offset", "get_offset");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "set_playing", "get_playing");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "audio_time"), "set_audio_time", "get_audio_time");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "time_changes"), "set_time_changes", "get_time_changes");

    // Methods
    ClassDB::bind_method("run_callbacks", &RubiconConductor::run_callbacks);
    
    ClassDB::bind_method(D_METHOD("play", "time"), &RubiconConductor::play);
    ClassDB::bind_method("resume", &RubiconConductor::resume);
    ClassDB::bind_method("pause", &RubiconConductor::pause);
    ClassDB::bind_method("stop", &RubiconConductor::resume);

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

RubiconConductor::RubiconConductor() {
	singleton = this;
}

RubiconConductor::~RubiconConductor() {
	if (singleton == this) {
		singleton = nullptr;
	}
}