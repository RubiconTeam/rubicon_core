#ifndef RUBICHART_H
#define RUBICHART_H

#include "core/io/resource.h"

class RubiChart : public Resource {
    GDCLASS(RubiChart, Resource);

public:
    static const u_int8_t MAX_LANE_COUNT = 32;
    static const PackedByteArray QUANTS;
};

#endif // RUBICHART_H