#ifndef TIME_CHANGE_H
#define TIME_CHANGE_H

#include "core/object/ref_counted.h"

#include "core/object/class_db.h"

#include "core/object/object.h"

class TimeChange : public RefCounted {
    GDCLASS(TimeChange, RefCounted);

public:
    float time;
    float bpm;
    float time_signature_numerator = 4;
    float time_signature_denominator = 4;

    float ms_time = 0.0;

    void set_time(const float p_time);
	float get_time() const;

    float get_measure_value();
    float get_beat_value();
    float get_step_value();

protected:
    static void _bind_methods();

};

#endif // TIME_CHANGE_H