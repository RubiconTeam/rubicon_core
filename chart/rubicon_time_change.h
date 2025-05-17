#ifndef RUBICON_TIME_CHANGE_H
#define RUBICON_TIME_CHANGE_H

#include "core/object/ref_counted.h"
#include "core/io/resource.h"

#include "core/object/class_db.h"

class RubiconTimeChange : public RefCounted {
    GDCLASS(RubiconTimeChange, RefCounted);

public:
    float time;
    float bpm;
    float time_signature_numerator = 4;
    float time_signature_denominator = 4;

    float ms_time = 0.0;

    void set_time(const float p_time);
	float get_time() const;

    void set_bpm(const float p_bpm);
	float get_bpm() const;

    void set_time_signature_numerator(const float p_time_signature_numerator);
	float get_time_signature_numerator() const;

    void set_time_signature_denominator(const float p_time_signature_denominator);
	float get_time_signature_denominator() const;

    float get_ms_time() const;

    float get_measure_value() const;
    float get_beat_value() const;
    float get_step_value() const;

protected:
    static void _bind_methods();
};

#endif // RUBICON_TIME_CHANGE_H