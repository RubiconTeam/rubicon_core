#include "modules/rubicon_core/conductor.h"
#include "scene/main/scene_tree.h"
#include "core/variant/callable.h"
#include "core/os/time.h"

Conductor *Conductor::singleton = nullptr;

Conductor *Conductor::get_singleton() {
	return singleton;
}

void Conductor::set_time_change_index(int p_time_change_index) {
    time_change_index = p_time_change_index;
}

int Conductor::get_time_change_index() {
    return time_change_index;
}

void Conductor::set_offset(float p_offset) {
    offset = p_offset;
}

float Conductor::get_offset() {
    return offset;
}

void Conductor::set_speed(float p_speed) {
    speed = p_speed;
}

float Conductor::get_speed() {
    return speed;
}

void Conductor::set_playing(bool p_playing) {
    playing = p_playing;
}

bool Conductor::get_playing() {
    return playing;
}

float Conductor::get_audio_time() {
    return offset + (playing ? float_t(Time::get_singleton()->get_unix_time_from_system() - _relative_start_time + _relative_time_offset) : _time != 0.0 ? float_t(_time) : 0.0f);
}

void Conductor::run_callbacks() {
    if (!playing) {
        _relative_start_time = Time::get_singleton()->get_unix_time_from_system();
        _relative_time_offset = _time;
    }

    while (time_change_index < _time_changes.size() - 1) {
        Ref<TimeChange> next_time_change = _time_changes[time_change_index + 1];
        if (next_time_change->ms_time / 1000.0f > get_time())
            break;
            
        time_change_index++;
        
        emit_signal(SNAME("bpm_changed"), next_time_change);
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

void Conductor::play(float p_time = 0.0f) {
    set_time(p_time);
    resume();
}

void Conductor::resume() {
    set_audio_time(float_t(_time));
    playing = true;

    _last_measure = int32_t(Math::floor(get_current_measure()));
    _last_beat = int32_t(Math::floor(get_current_beat()));
    _last_step = int32_t(Math::floor(get_current_step()));
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
    ClassDB::bind_method(D_METHOD("measures_to_ms", "p_measure", "p_bpm", "p_time_signature_numerator"), &Conductor::measure_to_ms);
}

Conductor::Conductor() {
	singleton = this;
}

Conductor::~Conductor() {
	if (singleton == this) {
		singleton = nullptr;
	}
}