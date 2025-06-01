#ifndef RUBICON_EVENT_META_H
#define RUBICON_EVENT_META_H

#include "core/io/resource.h"
#include "rubicon_event_data.h"
#include "core/variant/typed_array.h"

class RubiconEventMeta : public Resource {
    GDCLASS(RubiconEventMeta, Resource);

public:
    TypedArray<RubiconEventData> event_data_array;
    int index;

    RubiconEventData get_current_event() const;
    RubiconEventData get_next_event() const;
    bool has_next_event() const;

    void format();
    
    void set_index(const int idx);
    int get_index() const;
};

#endif