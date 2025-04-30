#ifndef CONDUCTOR_H
#define CONDUCTOR_H

#include "core/object/object.h"
#include "core/object/class_db.h"
#include "core/variant/typed_array.h"
#include "rubicon_core/chart/time_change.h"

template <typename T>
class TypedArray;

class Conductor : public Object {
    GDCLASS(Conductor, Object);

public:
    Conductor();
    ~Conductor();

    static Conductor *get_singleton();

    // Fields
    int time_change_index = 0;
    float offset = 0.0;
    float speed = 1.0;
    bool playing = false;

    // Getters and Setters
    void set_time_change_index(int p_time_change_index);
    int get_time_change_index();

    void set_offset(float p_offset);
    float get_offset();

    void set_speed(float p_speed);
    float get_speed();

    void set_playing(float p_playing);
    bool get_playing();

    void set_time(float p_time);
    float get_time();

    void set_audio_time(float p_time);
    float get_audio_time();

    float get_current_step();
    float get_current_beat();
    float get_current_measure();

    void set_time_changes(const TypedArray<TimeChange> &p_time_changes);
    TypedArray<TimeChange> get_time_changes();

    // Utility functions
    float measure_to_ms(float p_measure, float p_bpm, float p_timeSignatureNumerator) const;
    float beats_to_ms(float p_beat, float p_bpm) const;
    float steps_to_ms(float p_step, float p_bpm, float timeSignatureDenominator) const;
    float ms_to_measures(float p_msTime, const TypedArray<TimeChange> &p_time_changes) const;
    float measureToBeats(float p_measure, float p_timeSignatureNumerator) const;
    float measure_to_steps(float p_measure, float p_timeSignatureNumerator, float p_timeSignatureDenominator) const;
    float beats_to_steps(float p_beats, float p_timeSignatureDenominator) const; 
    float beats_to_measures(float p_beats, float p_timeSignatureNumerator) const;
    float steps_to_measures(float p_steps, float p_timeSignatureNumerator, float p_timeSignatureDenominator) const;

protected:
    static Conductor *singleton;

	static void _bind_methods();
    void _process_internal();

private:
	double _relative_start_time = 0;
	double _relative_time_offset = 0;
	float _last_time = -1.79769e308;
	double _time = 0;
		
	float _cached_step;
	float _cached_step_time;
	
	float _cached_beat;
	float _cached_beat_time;
	
	float _cached_measure;
	float _cached_measure_time;

	TypedArray<TimeChange> _time_changes;
	
	int _last_beat = -2147483648;
	int _last_step = -2147483648;
	int _last_measure = -2147483648;
};

#endif // CONDUCTOR_H