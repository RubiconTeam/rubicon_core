#ifndef RUBICHART_LOADER_H
#define RUBICHART_LOADER_H

#include "rubichart.h"

class RubiChartLoader {
public:
    static String get_string_from_utf8(PackedByteArray *p_instance);
    static Ref<RubiChart> convert(const Ref<FileAccess> p_reader, const int version = 0);
};

#endif