#include "rubicon_event_data.h"
#include "../rubicon_conductor.h"

void RubiconEventData::set_time(const float p_time) {
    time = p_time;
}

float RubiconEventData::get_time() const {
    return time;
}

void RubiconEventData::set_event_name(const StringName p_name) {
    name = p_name;
}

StringName RubiconEventData::get_event_name() const {
    return name;
}

void RubiconEventData::set_arguments(const TypedDictionary<StringName, Variant> p_arguments) {
    arguments = p_arguments;
}

TypedDictionary<StringName, Variant> RubiconEventData::get_arguments() const {
    return arguments;
}

void RubiconEventData::set_ms_time(const float p_ms_time) {
    ms_time = p_ms_time;
}

float RubiconEventData::get_ms_time() const {
    return ms_time;
}

void RubiconEventData::convert_data(const TypedArray<RubiconTimeChange> p_bpm_info) {
    Ref<RubiconTimeChange> bpm = p_bpm_info[-1];
    for (int i = 0; i < p_bpm_info.size(); i++) {
        Ref<RubiconTimeChange> bpm_info = p_bpm_info[i];
        if (bpm_info->time > time) {
            bpm = p_bpm_info[i - 1];
            break;
        }
    }

    ms_time = RubiconConductor::measure_to_ms(time - bpm->time, bpm->bpm, bpm->time_signature_numerator) + bpm->ms_time;
}

void RubiconEventData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_time", "time"), &RubiconEventData::set_time);
    ClassDB::bind_method("get_time", &RubiconEventData::get_time);

    ClassDB::bind_method(D_METHOD("set_event_name", "name"), &RubiconEventData::set_event_name);
    ClassDB::bind_method("get_event_name", &RubiconEventData::get_event_name);

    ClassDB::bind_method(D_METHOD("set_arguments", "arguments"), &RubiconEventData::set_arguments);
    ClassDB::bind_method("get_arguments", &RubiconEventData::get_arguments);

    ClassDB::bind_method(D_METHOD("set_ms_time", "ms_time"), &RubiconEventData::set_ms_time);
    ClassDB::bind_method("get_ms_time", &RubiconEventData::get_ms_time);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "arguments", PROPERTY_HINT_DICTIONARY_TYPE, "StringName;Variant"), "set_arguments", "get_arguments");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ms_time"), "set_ms_time", "get_ms_time");
}