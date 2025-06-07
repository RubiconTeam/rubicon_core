#include "rubicon_event_meta.h"

void RubiconEventMeta::set_events_data(const TypedArray<RubiconEventData> p_events_data) {
    events_data = p_events_data;
}

TypedArray<RubiconEventData> RubiconEventMeta::get_events_data() const {
    return events_data;
}

Ref<RubiconEventData> RubiconEventMeta::get_current_event() {
    return events_data[index];
}

bool RubiconEventMeta::has_next_event() {
    return index < events_data.size();
}

Ref<RubiconEventData> RubiconEventMeta::get_next_event() {
    if (!has_next_event())
        return nullptr;

    index++;
    return events_data[index];
}

void RubiconEventMeta::set_index(const int p_index) {
    index = p_index;
}

int RubiconEventMeta::get_index() const {
    return index;
}

bool RubiconEventMeta::sort_events(const Variant &p_a, const Variant &p_b) {
    RubiconEventData* event_a = Object::cast_to<RubiconEventData>(p_a);
    RubiconEventData* event_b = Object::cast_to<RubiconEventData>(p_b);
    if (event_a != nullptr && event_b != nullptr)
        return event_a->time < event_b->time;

    return p_a < p_b;
}

void RubiconEventMeta::format() {
    events_data.sort_custom(callable_mp_static(RubiconEventMeta::sort_events));
}

void RubiconEventMeta::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_events_data", "events_data"), &RubiconEventMeta::set_events_data);
    ClassDB::bind_method("get_events_data", &RubiconEventMeta::get_events_data);

    ClassDB::bind_method(D_METHOD("set_index", "index"), &RubiconEventMeta::set_index);
    ClassDB::bind_method("get_index", &RubiconEventMeta::get_index);

    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "events_data", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconEventData")), "set_events_data", "get_events_data");
}
