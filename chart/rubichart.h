#ifndef RUBICHART_H
#define RUBICHART_H

#include "core/io/resource.h"

class RubiChart : public Resource {
    GDCLASS(RubiChart, Resource);

public:
    enum QuantValue : u_int8_t {
        QUANT_VALUE_4TH = 4,
        QUANT_VALUE_8TH = 8,
        QUANT_VALUE_12TH = 12,
        QUANT_VALUE_16TH = 16,
        QUANT_VALUE_24ND = 24,
        QUANT_VALUE_32ND = 32,
        QUANT_VALUE_48TH = 48,
        QUANT_VALUE_64TH = 64,
        QUANT_VALUE_192ND = 192
    };

    static const u_int8_t MAX_LANE_COUNT = 32;

protected:
    static void _bind_methods();
};

VARIANT_ENUM_CAST(RubiChart::QuantValue);

#endif // RUBICHART_H