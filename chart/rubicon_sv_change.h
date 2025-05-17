#ifndef RUBICON_SV_CHANGE_H
#define RUBICON_SV_CHANGE_H

#include "core/io/resource.h"

#include "core/object/class_db.h"
#include "core/variant/typed_array.h"
#include "rubicon_time_change.h"

template <typename T>
class TypedArray;

class RubiconSvChange : public Resource {
    GDCLASS(RubiconSvChange, Resource);

public:
    float time = 0.0f;
    float multiplier = 1.0f;

    float ms_time = 0.0f;
    float position = 0.0f;

    void set_time(const float p_time);
    float get_time() const;

    void set_multiplier(const float p_multiplier);
    float get_multiplier() const;

    float get_ms_time() const;
    float get_position() const;

    void convert_data(const TypedArray<RubiconTimeChange> &p_time_changes, const Ref<RubiconSvChange> p_previous_change = Ref<RubiconSvChange>());

protected:
    static void _bind_methods();
};

#endif // RUBICON_SV_CHANGE_H