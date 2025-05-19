#include "rubichart.h"

void RubiChart::set_charter(const String &p_value) {
    charter = p_value;
}

String RubiChart::get_charter() const {
    return charter;
}

void RubiChart::set_difficulty(const uint32_t p_value) {
    difficulty = p_value;
}

uint32_t RubiChart::get_difficulty() const {
    return difficulty;
}

void RubiChart::set_scroll_speed(const float p_value) {
    scroll_speed = p_value;
}

float RubiChart::get_scroll_speed() const {
    return scroll_speed;
}

void RubiChart::set_charts(const TypedArray<RubiconChartData> &p_value) {
    charts = p_value;
}

TypedArray<RubiconChartData> RubiChart::get_charts() const {
    return charts;
}

void RubiChart::convert_data(const TypedArray<RubiconTimeChange> &p_time_changes) {
    for (int c = 0; c < charts.size(); c++) {
        Ref<RubiconChartData> chart = charts[c];
        chart->convert_data(p_time_changes);
    }
}

PackedStringArray RubiChart::get_all_notetypes() const {
    PackedStringArray types;
    for (int c = 0; c < charts.size(); c++) {
        Ref<RubiconChartData> chart = charts[c];

        int s;
        for (s = 0; s < chart->sections.size(); s++) {
            Ref<RubiconSectionData> section = chart->sections[s];
            for (int r = 0; r < section->rows.size(); r++) {
                Ref<RubiconRowData> row = section->rows[r];
                PackedStringArray row_types = row->get_note_types();
                for (int t = 0; t < row_types.size(); t++) {
                    if (types.has(row_types[t]))
                        continue;
                    
                    types.append(row_types[t]);
                }
            }
        }

        for (s = 0; s < chart->strays.size(); s++) {
            Ref<RubiconNoteData> stray = chart->strays[s];
            String stray_type = stray->type;
            if (types.has(stray_type))
                continue;
                    
            types.append(stray_type);
        }
    }

    return types;
}

void RubiChart::_bind_methods() {
    BIND_CONSTANT(CHART_VERSION);
    BIND_CONSTANT(MAX_LANE_COUNT);

    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_charter", "value"), &RubiChart::set_charter);
    ClassDB::bind_method("get_charter", &RubiChart::get_charter);
    ClassDB::bind_method(D_METHOD("set_difficulty", "value"), &RubiChart::set_difficulty);
    ClassDB::bind_method("get_difficulty", &RubiChart::get_difficulty);
    ClassDB::bind_method(D_METHOD("set_scroll_speed", "value"), &RubiChart::set_scroll_speed);
    ClassDB::bind_method("get_scroll_speed", &RubiChart::get_scroll_speed);
    ClassDB::bind_method(D_METHOD("set_charts", "value"), &RubiChart::set_charts);
    ClassDB::bind_method("get_charts", &RubiChart::get_charts);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "charter"), "set_charter", "get_charter");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "difficulty"), "set_difficulty", "get_difficulty");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "scroll_speed"), "set_scroll_speed", "get_scroll_speed");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "charts", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconChartData")), "set_charts", "get_charts");

    // Methods
    ClassDB::bind_method(D_METHOD("convert_data", "time_changes"), &RubiChart::convert_data);
    ClassDB::bind_method("get_all_notetypes", &RubiChart::get_all_notetypes);
}

//Loader
void RubiconChartLoader::get_recognized_extensions(List<String> *p_extensions) const {
    p_extensions->push_back("rbc");
}

bool RubiconChartLoader::handles_type(const String &p_type) const {
    return (p_type == "RubiChart");
}

String RubiconChartLoader::get_resource_type(const String &p_path) const {
    String el = p_path.get_extension().to_lower();
    if (el == "rbc") {
        return "RubiChart";
    }
    return "";
}

//Saver
void RubiconChartSaver::get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const {
    Ref<RubiChart> chart = p_resource;
    if (chart.is_valid()) {
        p_extensions->push_back("rbc");
    }
}

bool RubiconChartSaver::recognize(const Ref<Resource> &p_resource) const {
    return (p_resource.is_valid() && p_resource->is_class("RubiChart"));
}
