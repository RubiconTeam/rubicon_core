#ifndef RUBICON_CONDUCTOR_H
#define RUBICON_CONDUCTOR_H

#include "scene/main/node.h"
#include "core/object/class_db.h"
#include "core/variant/typed_array.h"
#include "rubicon_time_change.h"

template <typename T>
class TypedArray;

class RubiconConductor : public Node {
    GDCLASS(RubiconConductor, Node);

public:
    // Fields
    int time_change_index = 0;
    bool playing = false;

    // Getters and Setters
    void set_time(const float p_time);
    float get_time() const;

    void set_playing(const bool p_playing);
    bool get_playing() const;

    void set_time_change_index(const int p_time_change_index);
    int get_time_change_index() const;

    void set_time_change_list(const TypedArray<RubiconTimeChange> &p_time_changes);
    TypedArray<RubiconTimeChange> get_time_change_list() const;

    // Methods
    void play(const float p_time = 0.0f);
    void stop();

    Ref<RubiconTimeChange> get_current_time_change();

    void reset();

    float get_current_step();
    float get_current_beat();
    float get_current_measure();

    // Utility functions
    static float measure_to_ms(float p_measure, float p_bpm, float p_time_signature_numerator);
    static float beats_to_ms(float p_beat, float p_bpm);
    static float steps_to_ms(float p_step, float p_bpm, float p_time_signature_denominator);
    static float measure_range_to_ms(float p_start, float p_end, const TypedArray<RubiconTimeChange> &p_time_changes);
    static float measure_to_beats(float p_measure, float p_time_signature_numerator);
    static float measure_to_steps(float p_measure, float p_time_signature_numerator, float p_time_signature_denominator);
    static float beats_to_steps(float p_beats, float p_time_signature_denominator);
    static float beats_to_measures(float p_beats, float p_time_signature_numerator);
    static float steps_to_measures(float p_steps, float p_time_signature_numerator, float p_time_signature_denominator);
    static float ms_to_measures(float p_ms_time, const TypedArray<RubiconTimeChange> &p_time_changes);

protected:
    void _notification(int p_what);
	static void _bind_methods();

private:
	double _relative_start_time = 0;
	double _relative_time_offset = 0;
	float _last_time = 0.0f;
	double _time = 0;
		
	float _cached_step;
	float _cached_step_time;
	
	float _cached_beat;
	float _cached_beat_time;
	
	float _cached_measure;
	float _cached_measure_time;

	TypedArray<RubiconTimeChange> _time_change_list;
	
	int _last_beat = 0;
	int _last_step = 0;
	int _last_measure = 0;

    void _time_changed(float delta);
    bool _validate_time_change_list() const;
};

#endif // RUBICON_CONDUCTOR_H