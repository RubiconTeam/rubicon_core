#ifndef RUBICHART_LOADER_H
#define RUBICHART_LOADER_H

#include "rubichart.h"

class RubiChartLoader {
public:
    static Ref<RubiChart> convert(const Ref<FileAccess> p_reader);
};

#endif