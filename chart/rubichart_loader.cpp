#include "rubichart_loader.h"

Ref<RubiChart> RubiChartLoader::convert(const Ref<FileAccess> p_reader) {
    Ref<RubiChart> chart = memnew(RubiChart);
    chart->difficulty = p_reader->get_32();
    chart->scroll_speed = p_reader->get_float();

    int charter_length = p_reader->get_32();
    chart->charter = String::utf8((const char *)(p_reader->get_buffer(charter_length).ptr()), charter_length);

    int note_types_length = p_reader->get_32();
    PackedStringArray note_types;
}