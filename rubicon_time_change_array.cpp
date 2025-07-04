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
    
    for (int t = 0; t < data.size(); t++) {
        Ref<RubiconTimeChange> current = data[t];
        if (current->ms_time < p_time)
            continue;

        return current;
    }

    return Ref<RubiconTimeChange>(); // ?????????????
}

Ref<RubiconTimeChange> RubiconTimeChangeArray::get_time_change_at_measure(const float p_measure) const {
    if (!_validate_data())
        return Ref<RubiconTimeChange>();
    
    for (int t = 0; t < data.size(); t++) {
        Ref<RubiconTimeChange> current = data[t];
        if (current->time < p_measure)
            continue;

        return current;
    }

    return Ref<RubiconTimeChange>(); // ?????????????
}

Ref<RubiconTimeChange> RubiconTimeChangeArray::get_time_change_at_beat(const float p_beat) const {
    if (!_validate_data())
        return Ref<RubiconTimeChange>();
    
    int beat_count = 0;
    for (int t = 0; t < data.size(); t++) {
        Ref<RubiconTimeChange> current = data[t];
        if (beat_count < p_beat) {
            if (t < data.size() - 1) {
                Ref<RubiconTimeChange> next = data[t + 1];
                beat_count += current->time_signature_numerator * (next->time - current->time);
            }
                
            continue;
        }

        return current;
    }

    return Ref<RubiconTimeChange>(); // ?????????????
}

Ref<RubiconTimeChange> RubiconTimeChangeArray::get_time_change_at_step(const float p_step) const {
    if (!_validate_data())
        return Ref<RubiconTimeChange>();
    
    int step_count = 0;
    for (int t = 0; t < data.size(); t++) {
        Ref<RubiconTimeChange> current = data[t];
        if (step_count < p_step) {
            if (t < data.size() - 1) {
                Ref<RubiconTimeChange> next = data[t + 1];
                step_count += current->time_signature_numerator * current->time_signature_denominator * (next->time - current->time);
            }
                
            continue;
        }

        return current;
    }

    return Ref<RubiconTimeChange>(); // ?????????????
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

    ClassDB::bind_method(D_METHOD("get_ms_range_for_time_change", "index", "range"), &RubiconTimeChangeArray::get_ms_range_for_time_change);
    ClassDB::bind_method(D_METHOD("get_measure_range_for_time_change", "index", "range"), &RubiconTimeChangeArray::get_measure_range_for_time_change);
    ClassDB::bind_method(D_METHOD("get_beat_range_for_time_change", "index", "range"), &RubiconTimeChangeArray::get_beat_range_for_time_change);
    ClassDB::bind_method(D_METHOD("get_step_range_for_time_change", "index", "range"), &RubiconTimeChangeArray::get_step_range_for_time_change);
}