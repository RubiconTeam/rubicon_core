#ifndef RUBICON_CORE_H
#define RUBICON_CORE_H

#include "core/object/object.h"
#include "core/object/class_db.h"
#include "core/variant/typed_array.h"

template <typename T>
class TypedArray;

class RubiconCore : public Object {
    GDCLASS(RubiconCore, Object);

public:
    RubiconCore();
    ~RubiconCore();

    static RubiconCore *get_singleton();

    PackedByteArray quants;

    void set_quants(const PackedByteArray &p_quants);
    PackedByteArray get_quants() const;

protected:
    static RubiconCore *singleton;

    static void _bind_methods();
};

#endif