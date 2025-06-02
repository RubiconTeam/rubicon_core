#ifndef RUBICON_EVENT_DATA_H
#define RUBICON_EVENT_DATA_H

#include "core/io/resource.h"
#include "core/variant/typed_array.h"
#include "core/variant/typed_dictionary.h"
#include "modules/rubicon_core/chart/rubicon_time_change.h"

class RubiconEventData : public Resource {
    GDCLASS(RubiconEventData, Resource);

public:
    float time;
    StringName name;
    TypedDictionary<StringName, Variant> arguments;
    float ms_time;

    void convert_data(const TypedArray<RubiconTimeChange> p_bpm_info);

    void set_time(const float p_time);
    float get_time() const;
    
    void set_event_name(const StringName p_name);
    StringName get_event_name() const;

    void set_arguments(const TypedDictionary<StringName, Variant> p_arguments);
    TypedDictionary<StringName, Variant> get_arguments() const;

    void set_ms_time(const float p_ms_time);
    float get_ms_time() const;

protected:
    static void _bind_methods();
};

#endif