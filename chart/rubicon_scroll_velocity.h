#ifndef RUBICON_SCROLL_VELOCITY_H
#define RUBICON_SCROLL_VELOCITY_H

#include "core/io/resource.h"

#include "core/object/class_db.h"
#include "core/variant/typed_array.h"
#include "rubicon_time_change.h"

template <typename T>
class TypedArray;

class RubiconScrollVelocity : public Resource {
    GDCLASS(RubiconScrollVelocity, Resource);

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

    void convert_data(const TypedArray<RubiconTimeChange> &p_time_changes, const Ref<RubiconScrollVelocity> p_previous = Ref<RubiconScrollVelocity>());

protected:
    static void _bind_methods();
};

#endif // RUBICON_SCROLL_VELOCITY_H