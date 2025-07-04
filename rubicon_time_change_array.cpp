#include "rubicon_time_change_array.h"

void RubiconTimeChangeArray::set_data(const TypedArray<RubiconTimeChange> p_value) {
    data = p_value;
}

TypedArray<RubiconTimeChange> RubiconTimeChangeArray::get_data() const {
    return data;
}

Ref<RubiconTimeChange> RubiconTimeChangeArray::get_time_change_at_ms(const float p_time) const {

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

void RubiconTimeChangeArray::get_beat_range_for_time_change(Array p_range) const {
    
}

bool RubiconTimeChangeArray::_validate_data() const {
    ERR_FAIL_COND_V_MSG(data.size() == 0, false, "Data is completely empty!");

    for (int t = 0; t < data.size(); t++) {
        Ref<RubiconTimeChange> current = data[t];
        ERR_FAIL_COND_V_MSG(current.is_null() || !current.is_valid(), false, "At least one instance of time change data is null or invalid.");
    }

    return true;
}