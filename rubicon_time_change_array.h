#ifndef RUBICON_TIME_CHANGE_ARRAY_H
#define RUBICON_TIME_CHANGE_ARRAY_H

#include "core/io/resource.h"
#include "core/object/class_db.h"
#include "core/variant/typed_array.h"
#include "rubicon_time_change.h"

template <typename T>
class TypedArray;

class RubiconTimeChangeArray : public Resource {
    GDCLASS(RubiconTimeChangeArray, Resource);

public:
    TypedArray<RubiconTimeChange> data;

    void set_data(const TypedArray<RubiconTimeChange> p_value);
    TypedArray<RubiconTimeChange> get_data() const;

    bool is_valid() const;
    void convert_data();

    Ref<RubiconTimeChange> get_time_change_at_ms(const float p_time) const;
    Ref<RubiconTimeChange> get_time_change_at_measure(const float p_measure) const;
    Ref<RubiconTimeChange> get_time_change_at_beat(const float p_beat) const;
    Ref<RubiconTimeChange> get_time_change_at_step(const float p_step) const;

    float get_ms_for_measure(const float p_measure) const;
    float get_ms_for_beat(const float p_beat) const;
    float get_ms_for_step(const float p_step) const;
    
    float get_measure_for_ms(const float p_time) const;
    float get_measure_for_beat(const float p_beat) const;
    float get_measure_for_step(const float p_step) const;

    float get_beat_for_ms(const float p_time) const;
    float get_beat_for_measure(const float p_measure) const;
    float get_beat_for_step(const float p_step) const;
    
    float get_step_for_ms(const float p_time) const;
    float get_step_for_measure(const float p_measure) const;
    float get_step_for_beat(const float p_beat) const;
    
    void get_ms_range_for_time_change(const int p_index, Array p_range) const;
    void get_measure_range_for_time_change(const int p_index, Array p_range) const;
    void get_beat_range_for_time_change(const int p_index, Array p_range) const;
    void get_step_range_for_time_change(const int p_index, Array p_range) const;

protected:
    static void _bind_methods();

private:
    bool _validate_data() const;
};

#endif // RUBICON_TIME_CHANGE_ARRAY_H