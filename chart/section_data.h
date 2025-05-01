#ifndef SECTION_DATA_H
#define SECTION_DATA_H

#include "core/io/resource.h"

class SectionData : public Resource {
    GDCLASS(SectionData, Resource);

public:
    int measure = 0;
};

#endif // SECTION_DATA_H