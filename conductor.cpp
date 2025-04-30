#include "modules/rubicon_core/conductor.h"
#include "scene/main/scene_tree.h"
#include "core/variant/callable.h"
#include "core/os/time.h"

Conductor *Conductor::singleton = nullptr;

Conductor *Conductor::get_singleton() {
	return singleton;
}

void Conductor::set_time_change_index(const int p_time_change_index) {
    time_change_index = p_time_change_index;
}

int Conductor::get_time_change_index() const {
    return time_change_index;
}

void Conductor::set_offset(const float p_offset) {
    offset = p_offset;
}

float Conductor::get_offset() const {
    return offset;
}

void Conductor::set_speed(const float p_speed) {
    speed = p_speed;
}

float Conductor::get_speed() const {
    return speed;
}

void Conductor::set_playing(const bool p_playing) {
    playing = p_playing;
}

bool Conductor::get_playing() const {
    return playing;
}

void Conductor::set_time(const float p_time) {
    set_audio_time(p_time / speed);
}

float Conductor::get_time() const {
    return get_audio_time() * speed;
}

void Conductor::set_audio_time(const float p_time) {
    _time = p_time;
    _relative_start_time = Time::get_singleton()->get_unix_time_from_system();
    _relative_time_offset = p_time;

    _last_measure = int32_t(Math::floor(get_current_measure()));
    _last_beat = int32_t(Math::floor(get_current_beat()));
    _last_step = int32_t(Math::floor(get_current_step()));
}

float Conductor::get_audio_time() const {
    return offset + (playing ? float_t(Time::get_singleton()->get_unix_time_from_system() - _relative_start_time + _relative_time_offset) : _time != 0.0 ? float_t(_time) : 0.0f);
}

void Conductor::set_time_changes(const TypedArray<TimeChange> &p_time_changes) {
    _time_changes = p_time_changes;
}

TypedArray<TimeChange> Conductor::get_time_changes() const {
    return _time_changes;
}

void Conductor::run_callbacks() {
    if (!playing) 
        return;

    while (time_change_index < _time_changes.size() - 1) {
        Ref<TimeChange> next_time_change = _time_changes[time_change_index + 1];
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

void Conductor::play(const float p_time) {
    set_time(p_time);
    resume();
}

void Conductor::resume() {
    playing = true;
}

void Conductor::pause() {
    _time = get_audio_time();
    playing = false;
}

void Conductor::stop() {
    set_time(0.0);
    pause();
}

void Conductor::reset() {
    _time_changes.clear();
    time_change_index = 0;
    offset = 0;
    speed = 1.0f;
    stop();
}

float Conductor::get_current_step() {
    float time = get_time();
    if (_cached_step_time == time)
        return _cached_step;

    Ref<TimeChange> time_change = get_current_time_change();
    if (_time_changes.size() <= 1)
        return time / (60.0f / (time_change->bpm * time_change->time_signature_denominator));
    
    _cached_step_time = time;
    _cached_step = (time - time_change->ms_time / 1000.0f) / (60.0f / (time_change->bpm * time_change->time_signature_denominator)) + (time_change->time * time_change->time_signature_numerator * time_change->time_signature_denominator);
    return _cached_step;
}

float Conductor::get_current_beat() {
    float time = get_time();
    if (_cached_beat_time == time)
        return _cached_beat;
    
    Ref<TimeChange> time_change = get_current_time_change();
    if (_time_changes.size() <= 1)
        return time / (60.0f / time_change->bpm);
    
    _cached_beat_time = time;
    _cached_beat = (time - time_change->ms_time / 1000.0f) / (60.0f / time_change->bpm) + time_change->time * time_change->time_signature_numerator;
    return _cached_beat;
}

float Conductor::get_current_measure() {
    float time = get_time();
    if (_cached_measure_time == time)
        return _cached_measure;
    
    Ref<TimeChange> time_change = get_current_time_change();
    if (_time_changes.size() <= 1)
        return time / (60.0f / (time_change->bpm / time_change->time_signature_numerator));
    
    _cached_measure_time = time;
    _cached_measure = (time - time_change->ms_time / 1000.0f) / (60.0f / (time_change->bpm / time_change->time_signature_numerator)) + time_change->time;
    return _cached_measure;
}

Ref<TimeChange> Conductor::get_current_time_change() {
    return _time_changes[time_change_index];
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

float Conductor::measure_to_beats(float p_measure, float p_time_signature_numerator) const {
    return p_measure * p_time_signature_numerator;
}

float Conductor::measure_to_steps(float p_measure, float p_time_signature_numerator, float p_time_signature_denominator) const {
    return beats_to_steps(measure_to_beats(p_measure, p_time_signature_numerator), p_time_signature_denominator);
}

float Conductor::beats_to_steps(float p_beats, float p_time_signature_denominator) const {
    return p_beats * p_time_signature_denominator;
}

float Conductor::beats_to_measures(float p_beats, float p_time_signature_numerator) const {
    return p_beats / p_time_signature_numerator;
}

float Conductor::steps_to_measures(float p_steps,float p_time_signature_numerator, float p_time_signature_denominator) const {
    return p_steps / (p_time_signature_numerator * p_time_signature_denominator);
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
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_time_change_index", "time_change_index"), &Conductor::set_time_change_index);
    ClassDB::bind_method("get_time_change_index", &Conductor::get_time_change_index);
    ClassDB::bind_method(D_METHOD("set_offset", "offset"), &Conductor::set_offset);
    ClassDB::bind_method("get_offset", &Conductor::get_offset);
    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &Conductor::set_speed);
    ClassDB::bind_method("get_speed", &Conductor::get_speed);
    ClassDB::bind_method(D_METHOD("set_playing", "playing"), &Conductor::set_playing);
    ClassDB::bind_method("get_playing", &Conductor::get_playing);
    ClassDB::bind_method(D_METHOD("set_time", "time"), &Conductor::set_time);
    ClassDB::bind_method("get_time", &Conductor::get_time);
    ClassDB::bind_method(D_METHOD("set_audio_time", "time"), &Conductor::set_audio_time);
    ClassDB::bind_method("get_audio_time", &Conductor::get_audio_time);
    ClassDB::bind_method(D_METHOD("set_time_changes", "time_changes"), &Conductor::set_time_changes);
    ClassDB::bind_method("get_time_changes", &Conductor::get_time_changes);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::INT, "time_change_index"), "set_time_change_index", "get_time_change_index");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "offset"), "set_offset", "get_offset");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "set_playing", "get_playing");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "audio_time"), "set_audio_time", "get_audio_time");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "time_changes"), "set_time_changes", "get_time_changes");

    // Methods
    ClassDB::bind_method("run_callbacks", &Conductor::run_callbacks);
    
    ClassDB::bind_method(D_METHOD("play", "time"), &Conductor::play);
    ClassDB::bind_method("resume", &Conductor::resume);
    ClassDB::bind_method("pause", &Conductor::pause);
    ClassDB::bind_method("stop", &Conductor::resume);

    ClassDB::bind_method("get_current_time_change", &Conductor::get_current_time_change);

    ClassDB::bind_method("reset", &Conductor::reset);

    ClassDB::bind_method("get_current_step", &Conductor::get_current_step);
    ClassDB::bind_method("get_current_beat", &Conductor::get_current_beat);
    ClassDB::bind_method("get_current_measure", &Conductor::get_current_measure);

    // Utility functins
    ClassDB::bind_method(D_METHOD("measures_to_ms", "measure", "bpm", "ime_signature_numerator"), &Conductor::measure_to_ms);
    ClassDB::bind_method(D_METHOD("beats_to_ms", "beat", "bpm"), &Conductor::beats_to_ms);
    ClassDB::bind_method(D_METHOD("steps_to_ms", "step", "bpm", "time_signature_denominator"), &Conductor::steps_to_ms);
    ClassDB::bind_method(D_METHOD("measure_to_beats", "measure", "time_signature_numerator"), &Conductor::measure_to_beats);
    ClassDB::bind_method(D_METHOD("measure_to_steps", "measure", "time_signature_numerator", "time_signature_denominator"), &Conductor::measure_to_steps);
    ClassDB::bind_method(D_METHOD("beats_to_steps", "beats", "time_signature_denominator"), &Conductor::beats_to_steps);
    ClassDB::bind_method(D_METHOD("beats_to_measures", "beats", "time_signature_numerator"), &Conductor::beats_to_measures);
    ClassDB::bind_method(D_METHOD("steps_to_measures", "steps", "time_signature_numerator", "time_signature_denominator"), &Conductor::steps_to_measures);
    ClassDB::bind_method(D_METHOD("ms_to_measures", "ms_time", "time_changes"), &Conductor::ms_to_measures);

    // Signals
    ADD_SIGNAL(MethodInfo("beat_hit", PropertyInfo(Variant::INT, "beat")));
    ADD_SIGNAL(MethodInfo("step_hit", PropertyInfo(Variant::INT, "step")));
    ADD_SIGNAL(MethodInfo("measure_hit", PropertyInfo(Variant::INT, "measure")));
    ADD_SIGNAL(MethodInfo("time_change_reached", PropertyInfo(Variant::INT, "time_change")));
}

Conductor::Conductor() {
	singleton = this;
}

Conductor::~Conductor() {
	if (singleton == this) {
		singleton = nullptr;
	}
}