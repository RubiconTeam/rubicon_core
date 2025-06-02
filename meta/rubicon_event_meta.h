#ifndef RUBICON_EVENT_META_H
#define RUBICON_EVENT_META_H

#include "core/io/resource.h"
#include "rubicon_event_data.h"
#include "core/variant/typed_array.h"

class RubiconEventMeta : public Resource {
    GDCLASS(RubiconEventMeta, Resource);

public:
    TypedArray<RubiconEventData> events_data;
    int index;

    void set_events_data(const TypedArray<RubiconEventData> p_events_data);
    TypedArray<RubiconEventData> get_events_data() const;

    Ref<RubiconEventData> get_current_event();
    bool has_next_event();
    Ref<RubiconEventData> get_next_event();

    void set_index(const int p_index);
    int get_index() const;

    void format();

protected:
    static void _bind_methods();

private:
    static bool sort_events(const Variant &p_a, const Variant &p_b);
};

#endif