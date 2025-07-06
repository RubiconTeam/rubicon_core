#include "rubicon_time_change_array.h"
#include "rubicon_conductor.h"

void RubiconTimeChangeArray::set_data(const TypedArray<RubiconTimeChange> p_value) {
    data = p_value;
}

TypedArray<RubiconTimeChange> RubiconTimeChangeArray::get_data() const {
    return data;
}

bool RubiconTimeChangeArray::is_valid() const {
    ERR_FAIL_COND_V_MSG(data.is_empty(), false, "Data needs to have at least one time change!");
    
    for (int i = 0; i < data.size(); i++) {
        Ref<RubiconTimeChange> current = data[i];
        ERR_FAIL_COND_V_MSG(current.is_null() || !current.is_valid(), false, "One of your time changes is either null or not valid.");
    }

    return true;
}

void RubiconTimeChangeArray::convert_data() {
    for (int i = 1; i < data.size(); i++) {
        Ref<RubiconTimeChange> previous_time_change = data[i-1];
        Ref<RubiconTimeChange> time_change = data[i];

        time_change->ms_time = previous_time_change->ms_time + RubiconConductor::measure_to_ms(time_change->time - previous_time_change->time, previous_time_change->bpm, time_change->time_signature_numerator);
    }
}

Ref<RubiconTimeChange> RubiconTimeChangeArray::get_time_change_at_ms(const float p_time) const {
    if (!_validate_data())
        return Ref<RubiconTimeChange>();

    return get_time_change_at_measure(get_measure_for_ms(p_time));
}

Ref<RubiconTimeChange> RubiconTimeChangeArray::get_time_change_at_measure(const float p_measure) const {
    if (!_validate_data())
        return Ref<RubiconTimeChange>();
    
    if (data.size() == 1)
        return data.front();
    
    for (int t = 0; t < data.size(); t++) {
        Ref<RubiconTimeChange> current = data[t];
        if (p_measure < current->time)
            continue;
        
        return current;
    }

    return data.back();
}

Ref<RubiconTimeChange> RubiconTimeChangeArray::get_time_change_at_beat(const float p_beat) const {
    if (!_validate_data())
        return Ref<RubiconTimeChange>();

    return get_time_change_at_measure(get_measure_for_beat(p_beat));
}

Ref<RubiconTimeChange> RubiconTimeChangeArray::get_time_change_at_step(const float p_step) const {
    if (!_validate_data())
        return Ref<RubiconTimeChange>();

    return get_time_change_at_measure(get_measure_for_step(p_step));
}

float RubiconTimeChangeArray::get_ms_for_measure(const float p_measure) const {
    if (!_validate_data())
        return 0.0;
    
    for (int i = 0; i < data.size(); i++) {
        Ref<RubiconTimeChange> current = data[i];
        
        if (p_measure >= current->time) 
            return current->ms_time + RubiconConductor::measure_to_ms(p_measure - current->time, current->bpm, current->time_signature_numerator);
    }

    return 0.0;
}

float RubiconTimeChangeArray::get_ms_for_beat(const float p_beat) const {
    return get_ms_for_measure(get_measure_for_beat(p_beat));
}

float RubiconTimeChangeArray::get_ms_for_step(const float p_step) const {
    return get_ms_for_measure(get_measure_for_step(p_step));
}

float RubiconTimeChangeArray::get_measure_for_ms(const float p_time) const {
    if (!_validate_data())
        return 0.0;
    
    for (int i = 0; i < data.size(); i++) {
        Ref<RubiconTimeChange> current = data[i];

        if (p_time >= current->ms_time) 
            return current->time + ((p_time - current->ms_time) / RubiconConductor::measure_to_ms(1.0, current->bpm, current->time_signature_numerator));
    }

    return 0.0;
}

float RubiconTimeChangeArray::get_measure_for_beat(const float p_beat) const {
    if (!_validate_data())
        return 0.0;
    
    if (data.size() == 1) {
        Ref<RubiconTimeChange> time_change = data[0];
        return RubiconConductor::beats_to_measures(p_beat, time_change->time_signature_numerator);
    }
    
    float beat_count = 0.0;
    for (int i = 1; i < data.size(); i++) {
        Ref<RubiconTimeChange> current = data[i];
        Ref<RubiconTimeChange> prev = data[i - 1];

        float beat_length = RubiconConductor::measure_to_beats(current->time - prev->time, prev->time_signature_numerator);
        if (beat_count + beat_length > p_beat) 
            return prev->time + RubiconConductor::beats_to_measures(p_beat - beat_count, prev->time_signature_numerator);

        beat_count += beat_length;
    }

    return 0.0;
}

float RubiconTimeChangeArray::get_measure_for_step(const float p_step) const {
    if (!_validate_data())
        return 0.0;
    
    if (data.size() == 1) {
        Ref<RubiconTimeChange> time_change = data[0];
        return RubiconConductor::steps_to_measures(p_step, time_change->time_signature_numerator, time_change->time_signature_denominator);
    }
    
    float step_count = 0.0;
    for (int i = 1; i < data.size(); i++) {
        Ref<RubiconTimeChange> current = data[i];
        Ref<RubiconTimeChange> prev = data[i - 1];

        float step_length = RubiconConductor::measure_to_steps(current->time - prev->time, prev->time_signature_numerator, prev->time_signature_denominator);
        if (step_count + step_length > p_step) 
            return prev->time + RubiconConductor::steps_to_measures(p_step - step_count, prev->time_signature_numerator, prev->time_signature_denominator);

        step_count += step_length;
    }

    return 0.0;
}

float RubiconTimeChangeArray::get_beat_for_ms(const float p_time) const {
    return get_beat_for_measure(get_measure_for_ms(p_time));
}

float RubiconTimeChangeArray::get_beat_for_measure(const float p_measure) const {
    if (!_validate_data())
        return 0.0;
    
    if (data.size() == 1) {
        Ref<RubiconTimeChange> time_change = data[0];
        return RubiconConductor::measure_to_beats(p_measure, time_change->time_signature_numerator);
    }
    
    float beat_count = 0.0;
    float measure_count = 0.0;
    for (int i = 1; i < data.size(); i++) {
        Ref<RubiconTimeChange> current = data[i];
        Ref<RubiconTimeChange> prev = data[i - 1];

        float measure_length = current->time - prev->time;
        float beat_length = RubiconConductor::measure_to_beats(current->time - prev->time, prev->time_signature_numerator);
        if (measure_count + measure_length > p_measure) 
            return beat_count + RubiconConductor::measure_to_beats(p_measure - prev->time, prev->time_signature_numerator);

        beat_count += beat_length;
        measure_count += measure_length;
    }

    return 0.0;
}

float RubiconTimeChangeArray::get_beat_for_step(const float p_step) const {
    return get_beat_for_measure(get_measure_for_step(p_step));
}

float RubiconTimeChangeArray::get_step_for_ms(const float p_time) const {
    return get_step_for_measure(get_measure_for_ms(p_time));
}

float RubiconTimeChangeArray::get_step_for_measure(const float p_measure) const {
    if (!_validate_data())
        return 0.0;
    
    if (data.size() == 1) {
        Ref<RubiconTimeChange> time_change = data[0];
        return RubiconConductor::measure_to_steps(p_measure, time_change->time_signature_numerator, time_change->time_signature_denominator);
    }
    
    float step_count = 0.0;
    float measure_count = 0.0;
    for (int i = 1; i < data.size(); i++) {
        Ref<RubiconTimeChange> current = data[i];
        Ref<RubiconTimeChange> prev = data[i - 1];

        float measure_length = current->time - prev->time;
        float step_length = RubiconConductor::measure_to_steps(current->time - prev->time, prev->time_signature_numerator, prev->time_signature_denominator);
        if (measure_count + measure_length > p_measure) 
            return step_count + RubiconConductor::measure_to_steps(p_measure - prev->time, prev->time_signature_numerator, prev->time_signature_denominator);

        step_count += step_length;
        measure_count += measure_length;
    }

    return 0.0;
}

float RubiconTimeChangeArray::get_step_for_beat(const float p_beat) const {
    return get_step_for_measure(get_measure_for_beat(p_beat));
}

void RubiconTimeChangeArray::get_ms_range_for_time_change(const int p_index, Array p_range) const {
    if (!_validate_data())
        return;
    
    ERR_FAIL_COND_MSG(p_index < 0, "Index must be >= 0.");
    ERR_FAIL_COND_MSG(p_index >= data.size(), "Index is out of bounds!");

    p_range.resize(2);
    p_range[0] = Ref<RubiconTimeChange>(data[p_index])->ms_time;
    if (p_index == data.size() - 1) {
        p_range[1] = -1;
        return;
    }

    p_range[1] = Ref<RubiconTimeChange>(data[p_index + 1])->ms_time;
}

void RubiconTimeChangeArray::get_measure_range_for_time_change(const int p_index, Array p_range) const {
    if (!_validate_data())
        return;
    
    ERR_FAIL_COND_MSG(p_index < 0, "Index must be >= 0.");
    ERR_FAIL_COND_MSG(p_index >= data.size(), "Index is out of bounds!");

    p_range.resize(2);
    p_range[0] = Ref<RubiconTimeChange>(data[p_index])->time;
    if (p_index == data.size() - 1) {
        p_range[1] = -1;
        return;
    }

    p_range[1] = Ref<RubiconTimeChange>(data[p_index + 1])->time;
}

void RubiconTimeChangeArray::get_beat_range_for_time_change(const int p_index, Array p_range) const {
    if (!_validate_data())
        return;
    
    ERR_FAIL_COND_MSG(p_index < 0, "Index must be >= 0.");
    ERR_FAIL_COND_MSG(p_index >= data.size(), "Index is out of bounds!");

    p_range.resize(2);

    int beat_count = 0;
    for (int i = 0; i < data.size(); i++) {
        Ref<RubiconTimeChange> current = data[i];
        
        if (i == p_index)
            p_range[0] = beat_count;
        
        if (i == p_index + 1) {
            p_range[1] = beat_count;
            break;
        }
        
        beat_count += current->time_signature_numerator * (Ref<RubiconTimeChange>(data[i + 1])->time - current->time); 
    }
}

void RubiconTimeChangeArray::get_step_range_for_time_change(const int p_index, Array p_range) const {
    if (!_validate_data())
        return;
    
    ERR_FAIL_COND_MSG(p_index < 0, "Index must be >= 0.");
    ERR_FAIL_COND_MSG(p_index >= data.size(), "Index is out of bounds!");

    p_range.resize(2);

    int step_count = 0;
    for (int i = 0; i < data.size(); i++) {
        Ref<RubiconTimeChange> current = data[i];
        
        if (i == p_index)
            p_range[0] = step_count;
        
        if (i == p_index + 1) {
            p_range[1] = step_count;
            break;
        }
        
        step_count += current->time_signature_numerator * current->time_signature_denominator * (Ref<RubiconTimeChange>(data[i + 1])->time - current->time);
    }
}

bool RubiconTimeChangeArray::_validate_data() const {
    ERR_FAIL_COND_V_MSG(data.size() == 0, false, "Data is completely empty!");

    for (int t = 0; t < data.size(); t++) {
        Ref<RubiconTimeChange> current = data[t];
        ERR_FAIL_COND_V_MSG(current.is_null() || !current.is_valid(), false, "At least one instance of time change data is null or invalid.");
    }

    return true;
}

void RubiconTimeChangeArray::_bind_methods() {
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_data", "value"), &RubiconTimeChangeArray::set_data);
    ClassDB::bind_method("get_data", &RubiconTimeChangeArray::get_data);
    
    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "data", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconTimeChange")), "set_data", "get_data");

    // Methods
    ClassDB::bind_method(D_METHOD("get_time_change_at_ms", "time"), &RubiconTimeChangeArray::get_time_change_at_ms);
    ClassDB::bind_method(D_METHOD("get_time_change_at_measure", "measure"), &RubiconTimeChangeArray::get_time_change_at_measure);
    ClassDB::bind_method(D_METHOD("get_time_change_at_beat", "beat"), &RubiconTimeChangeArray::get_time_change_at_beat);
    ClassDB::bind_method(D_METHOD("get_time_change_at_step", "step"), &RubiconTimeChangeArray::get_time_change_at_step);

    ClassDB::bind_method(D_METHOD("get_ms_for_measure", "measure"), &RubiconTimeChangeArray::get_ms_for_measure);
    ClassDB::bind_method(D_METHOD("get_ms_for_beat", "beat"), &RubiconTimeChangeArray::get_ms_for_beat);
    ClassDB::bind_method(D_METHOD("get_ms_for_step", "step"), &RubiconTimeChangeArray::get_ms_for_step);

    ClassDB::bind_method(D_METHOD("get_measure_for_ms", "time"), &RubiconTimeChangeArray::get_measure_for_ms);
    ClassDB::bind_method(D_METHOD("get_measure_for_beat", "beat"), &RubiconTimeChangeArray::get_measure_for_beat);
    ClassDB::bind_method(D_METHOD("get_measure_for_step", "step"), &RubiconTimeChangeArray::get_measure_for_step);

    ClassDB::bind_method(D_METHOD("get_beat_for_ms", "time"), &RubiconTimeChangeArray::get_beat_for_ms);
    ClassDB::bind_method(D_METHOD("get_beat_for_measure", "measure"), &RubiconTimeChangeArray::get_beat_for_measure);
    ClassDB::bind_method(D_METHOD("get_beat_for_step", "step"), &RubiconTimeChangeArray::get_beat_for_step);

    ClassDB::bind_method(D_METHOD("get_step_for_ms", "time"), &RubiconTimeChangeArray::get_step_for_ms);
    ClassDB::bind_method(D_METHOD("get_step_for_measure", "measure"), &RubiconTimeChangeArray::get_step_for_measure);
    ClassDB::bind_method(D_METHOD("get_step_for_beat", "beat"), &RubiconTimeChangeArray::get_step_for_beat);

    ClassDB::bind_method(D_METHOD("get_ms_range_for_time_change", "index", "range"), &RubiconTimeChangeArray::get_ms_range_for_time_change);
    ClassDB::bind_method(D_METHOD("get_measure_range_for_time_change", "index", "range"), &RubiconTimeChangeArray::get_measure_range_for_time_change);
    ClassDB::bind_method(D_METHOD("get_beat_range_for_time_change", "index", "range"), &RubiconTimeChangeArray::get_beat_range_for_time_change);
    ClassDB::bind_method(D_METHOD("get_step_range_for_time_change", "index", "range"), &RubiconTimeChangeArray::get_step_range_for_time_change);
}