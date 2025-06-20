#include "rubicon_chart_data.h"
#include "../rubicon_core.h"
#include "core/math/math_funcs.h"

void RubiconChartData::set_chart_name(const StringName &p_name) {
    chart_name = p_name;
}

StringName RubiconChartData::get_chart_name() const {
    return chart_name;
}

void RubiconChartData::set_lanes(const uint8_t p_lanes) {
    lanes = p_lanes;
}

uint8_t RubiconChartData::get_lanes() const {
    return lanes;
}

void RubiconChartData::set_scroll_velocities(const TypedArray<RubiconScrollVelocity> &p_value) {
    scroll_velocities = p_value;
}

TypedArray<RubiconScrollVelocity> RubiconChartData::get_scroll_velocities() const {
    return scroll_velocities;
}

void RubiconChartData::set_sections(const TypedArray<RubiconSectionData> &p_value) {
    sections = p_value;
}

TypedArray<RubiconSectionData> RubiconChartData::get_sections() const {
    return sections;
}

void RubiconChartData::set_strays(const TypedArray<RubiconNoteData> &p_value) {
    strays = p_value;
}

TypedArray<RubiconNoteData> RubiconChartData::get_strays() const {
    return strays;
}

TypedArray<RubiconNoteData> RubiconChartData::get_notes(const bool p_include_ends) const {
    TypedArray<RubiconNoteData> notes;
    int s, r;
    for (s = 0; s < sections.size(); s++) {
        Ref<RubiconSectionData> cur_section = sections[s];
        for (r = 0; r < cur_section->rows.size(); r++) {
            Ref<RubiconRowData> cur_row = cur_section->rows[r];
            notes.append_array(cur_row->get_notes(p_include_ends));
        }
    }

    notes.append_array(strays);
    notes.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_time));

    return notes;
}

TypedArray<RubiconNoteData> RubiconChartData::get_notes_at_lane(const uint8_t p_lane, const bool p_include_ends) const {
    TypedArray<RubiconNoteData> notes;
    int s, r;
    for (s = 0; s < sections.size(); s++) {
        Ref<RubiconSectionData> cur_section = sections[s];
        for (r = 0; r < cur_section->rows.size(); r++) {
            Ref<RubiconRowData> cur_row = cur_section->rows[r];
            Ref<RubiconNoteData> row_note = cur_row->get_note_at_lane(p_lane, p_include_ends);
            if (row_note.is_null())
                continue;

            notes.append(row_note);
        }
    }

    notes.append_array(strays.filter(callable_mp_static(&RubiconNoteData::is_note_lane).bind(p_lane)));
    notes.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_time));

    return notes;
}

TypedArray<RubiconNoteData> RubiconChartData::get_notes_of_type(const StringName &p_type, const bool p_include_ends) const {
    TypedArray<RubiconNoteData> notes;
    int s, r;
    for (s = 0; s < sections.size(); s++) {
        Ref<RubiconSectionData> cur_section = sections[s];
        for (r = 0; r < cur_section->rows.size(); r++) {
            Ref<RubiconRowData> cur_row = cur_section->rows[r];
            notes.append_array(cur_row->get_notes_of_type(p_type, p_include_ends));
        }
    }

    notes.append_array(strays.filter(callable_mp_static(&RubiconNoteData::is_note_type).bind(p_type)));
    notes.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_time));

    return notes;
}

void RubiconChartData::add_note_start(Ref<RubiconNoteData> p_note, const int p_measure, const uint8_t p_offset, const uint8_t p_quant) {
    Ref<RubiconRowData> row = add_section(p_measure)->add_row(p_offset, p_quant);
    row->add_starting_note(p_note);
    p_note->starting_row = row;
}

void RubiconChartData::add_note_end(Ref<RubiconNoteData> p_note, const int p_measure, const uint8_t p_offset, const uint8_t p_quant) {
    Ref<RubiconRowData> row = add_section(p_measure)->add_row(p_offset, p_quant);
    row->add_ending_note(p_note);
    p_note->starting_row = row;
}

void RubiconChartData::add_note_at_measure_time(Ref<RubiconNoteData> p_note, const float p_measure_time, const float p_length) {
    uint8_t start_offset; uint8_t start_quant;
    int base_measure = int(Math::floor(p_measure_time));
    measure_offset_to_offset_and_quant(p_measure_time - base_measure, start_offset, start_quant);
    add_note_start(p_note, base_measure, start_offset, start_quant);

    if (p_length <= 0.0f)
        return;

    float measure_end = p_measure_time + p_length;
    uint8_t end_offset; uint8_t end_quant;
    int base_end_measure = int(Math::floor(measure_end));
    measure_offset_to_offset_and_quant(measure_end - base_end_measure, end_offset, end_quant);
    add_note_end(p_note, base_end_measure, end_offset, end_quant);
}

void RubiconChartData::add_stray_note(Ref<RubiconNoteData> p_note) {
    strays.append(p_note);
    strays.sort_custom(callable_mp_static(&RubiconNoteData::compare_notes_by_time));
}

void RubiconChartData::move_note_start(Ref<RubiconNoteData> p_note, const int p_measure, const uint8_t p_offset, const uint8_t p_quant) {
    if (p_note->ending_row.is_null()) {
        remove_note_start(p_note);
        add_note_start(p_note, p_measure, p_offset, p_quant);
        return;
    }

    int measure_distance = p_note->ending_row->section->measure - p_note->starting_row->section->measure;
    uint8_t distance_quant = MAX(p_note->starting_row->quant, p_note->ending_row->quant);

    uint8_t starting_offset = p_note->starting_row->offset;
    if (p_note->ending_row->quant > p_note->starting_row->quant)
        starting_offset *= p_note->ending_row->quant / p_note->starting_row->quant;

    uint8_t ending_offset = p_note->ending_row->offset;
    if (p_note->starting_row->quant > p_note->ending_row->quant)
        ending_offset *= p_note->starting_row->quant / p_note->ending_row->quant;

    int distance_offset = ((measure_distance * distance_quant) + ending_offset) - starting_offset;
    uint8_t ending_quant = MAX(p_quant, distance_quant);

    // I'm sorry it's kinda tough figuring out what to name these
    int offset_int = int32_t(p_offset);
    if (distance_quant > p_quant)
        offset_int *= distance_quant / p_quant;

    int distance_int = int32_t(distance_offset);
    if (p_quant > distance_quant)
        distance_int *= p_quant / distance_quant;

    int total_distance = offset_int + distance_int;
    add_note_end(p_note, total_distance / ending_quant, uint8_t(total_distance % ending_quant), ending_quant);
}

void RubiconChartData::move_note_end(Ref<RubiconNoteData> p_note, const int p_measure, const uint8_t p_offset, const uint8_t p_quant) {
    remove_note_end(p_note);
    add_note_end(p_note, p_measure, p_offset, p_quant);
}

void RubiconChartData::remove_note_start(Ref<RubiconNoteData> p_note) {
    p_note->starting_row->remove_note(p_note);
    cleanup_sections();

    if (p_note->ending_row.is_null())
        return;

    remove_note_end(p_note);
}

void RubiconChartData::remove_note_end(Ref<RubiconNoteData> p_note) {
    p_note->ending_row->remove_note(p_note);
    cleanup_sections();
}

void RubiconChartData::remove_stray_note(Ref<RubiconNoteData> p_stray) {
    int index = strays.find(p_stray);
    if (index == -1)
        return;

    strays.remove_at(index);
}

Ref<RubiconSectionData> RubiconChartData::add_section(const int p_measure) {
    Ref<RubiconSectionData> section = get_section_at_measure(p_measure);
    if (!section.is_null())
        return section;

    section = memnew(RubiconSectionData);
    section->measure = p_measure;

    sections.append(section);
    sections.sort_custom(callable_mp_static(&RubiconSectionData::compare_sections_by_measure));

    return section;
}

void RubiconChartData::remove_section(const int p_measure) {
    Ref<RubiconSectionData> section = get_section_at_measure(p_measure);
    if (section.is_null())
        return;

    int index = sections.find(section);
    sections.remove_at(index);
    sections.sort_custom(callable_mp_static(&RubiconSectionData::compare_sections_by_measure));
}

Ref<RubiconSectionData> RubiconChartData::get_section_at_measure(const int p_measure) const {
    int index = sections.find_custom(callable_mp_static(&RubiconSectionData::compare_sections_by_measure));
    if (index < 0)
        return Ref<RubiconSectionData>();

    return sections[index];
}

void RubiconChartData::cleanup_sections() {
    TypedArray<RubiconSectionData> new_list;
    for (int s = 0; s < sections.size(); s++) {
        Ref<RubiconSectionData> cur_section = sections[s];
        cur_section->cleanup_rows();
        if (cur_section->rows.size() <= 0)
            continue;

        new_list.append(cur_section);
    }

    sections = new_list;
    sections.sort_custom(callable_mp_static(&RubiconSectionData::compare_sections_by_measure));
}

void RubiconChartData::convert_data(const TypedArray<RubiconTimeChange> &p_time_changes) {
    int s;
    for (s = 0; s < scroll_velocities.size(); s++) {
        Ref<RubiconScrollVelocity> sv = scroll_velocities[s];
        sv->convert_data(p_time_changes, scroll_velocities[s - 1]);
    }

    for (s = 0; s < sections.size(); s++) {
        Ref<RubiconSectionData> section = sections[s];
        section->convert_data(p_time_changes, scroll_velocities);
    }

    for (s = 0; s < strays.size(); s++) {
        Ref<RubiconNoteData> stray = strays[s];
        stray->convert_data(p_time_changes, scroll_velocities);
    }
}

void RubiconChartData::measure_offset_to_offset_and_quant(const float p_measure_offset, uint8_t &p_offset, uint8_t &p_quant) {
    PackedByteArray quants = RubiconCore::get_singleton()->quants;

    p_quant = static_cast<uint8_t>(quants[quants.size() - 1]);
    p_offset = uint8_t(CLAMP(Math::round(p_measure_offset * p_quant), 0, p_quant - 1));
    for (int q = 0; q < quants.size(); q++) {
        float result = p_measure_offset * quants[q];
        if (fmodf(result, 1.0f) != 0) { // If it's not already snapped
            int rounded_result = uint8_t(result);
            if (!Math::is_equal_approx(result, rounded_result, 0.1f))
                continue;

            p_offset = uint8_t(rounded_result);
            p_quant = static_cast<uint8_t>(quants[q]);
            return;
        }

        // If it's already snapped
        p_offset = uint8_t(result);
        p_quant = static_cast<uint8_t>(quants[q]);
        return;
    }
}

void RubiconChartData::_bind_methods() {
    // Getters and Setters
    ClassDB::bind_method(D_METHOD("set_chart_name", "value"), &RubiconChartData::set_chart_name);
    ClassDB::bind_method("get_chart_name", &RubiconChartData::get_chart_name);
    ClassDB::bind_method(D_METHOD("set_lanes", "value"), &RubiconChartData::set_lanes);
    ClassDB::bind_method("get_lanes", &RubiconChartData::get_lanes);
    ClassDB::bind_method(D_METHOD("set_scroll_velocities", "value"), &RubiconChartData::set_scroll_velocities);
    ClassDB::bind_method("get_scroll_velocities", &RubiconChartData::get_scroll_velocities);
    ClassDB::bind_method(D_METHOD("set_sections", "value"), &RubiconChartData::set_sections);
    ClassDB::bind_method("get_sections", &RubiconChartData::get_sections);
    ClassDB::bind_method(D_METHOD("set_strays", "value"), &RubiconChartData::set_strays);
    ClassDB::bind_method("get_strays", &RubiconChartData::get_strays);

    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "chart_name"), "set_chart_name", "get_chart_name");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lanes"), "set_lanes", "get_lanes");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "scroll_velocities", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconScrollVelocity")), "set_scroll_velocity", "get_scroll_velocity");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "sections", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconSectionData")), "set_sections", "get_sections");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "strays", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconNoteData")), "set_strays", "get_strays");

    // Methods
    ClassDB::bind_method(D_METHOD("get_notes", "include_ends"), &RubiconChartData::get_notes);
    ClassDB::bind_method(D_METHOD("get_notes_at_lane", "lane", "include_ends"), &RubiconChartData::get_notes_at_lane);
    ClassDB::bind_method(D_METHOD("get_notes_of_type", "type", "include_ends"), &RubiconChartData::get_notes_of_type);

    ClassDB::bind_method(D_METHOD("add_note_start", "note", "measure", "offset", "quant"), &RubiconChartData::add_note_start);
    ClassDB::bind_method(D_METHOD("add_note_end", "note", "measure", "offset", "quant"), &RubiconChartData::add_note_end);
    ClassDB::bind_method(D_METHOD("add_note_at_measure_time", "note", "measure_time", "length"), &RubiconChartData::add_note_at_measure_time);
    ClassDB::bind_method(D_METHOD("add_stray_note", "note"), &RubiconChartData::add_stray_note);

    ClassDB::bind_method(D_METHOD("move_note_start", "note", "measure", "offset", "quant"), &RubiconChartData::move_note_start);
    ClassDB::bind_method(D_METHOD("move_note_end", "note", "measure", "offset", "quant"), &RubiconChartData::move_note_end);

    ClassDB::bind_method(D_METHOD("remove_note_start", "note"), &RubiconChartData::remove_note_start);
    ClassDB::bind_method(D_METHOD("remove_note_end", "note"), &RubiconChartData::remove_note_end);
    ClassDB::bind_method(D_METHOD("remove_stray_note", "note"), &RubiconChartData::remove_stray_note);

    ClassDB::bind_method(D_METHOD("add_section", "measure"), &RubiconChartData::add_section);
    ClassDB::bind_method(D_METHOD("remove_section", "measure"), &RubiconChartData::remove_section);
    ClassDB::bind_method(D_METHOD("get_section_at_measure", "measure"), &RubiconChartData::get_section_at_measure);

    ClassDB::bind_method("cleanup_sections", &RubiconChartData::cleanup_sections);
    ClassDB::bind_method(D_METHOD("convert_data", "time_changes"), &RubiconChartData::convert_data);
}
