#ifndef RUBICHART_SAVER_H
#define RUBICHART_SAVER_H

#include "rubichart.h"

class RubiChartSaver {
public:
    static void save(const Ref<RubiChart> p_chart, const Ref<FileAccess> p_writer);
};

#endif