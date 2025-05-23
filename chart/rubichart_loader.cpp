#include "rubichart_loader.h"
#include "rubicon_chart_data.h"
#include "rubicon_sv_change.h"
#include "core/variant/typed_dictionary.h"
#include "core/variant/variant_utility.h"

PackedByteArray get_next_buffer(const Ref<FileAccess> p_reader) {
    return p_reader->get_buffer(int32_t(p_reader->get_32()));
}

String RubiChartLoader::get_string_from_utf8(PackedByteArray *p_instance) {
    String s;
    if (p_instance->size() > 0) {
        const uint8_t *r = p_instance->ptr();
        s.parse_utf8((const char *)r, p_instance->size());
    }
    return s;
}

void read_note_parameters(const Ref<FileAccess> p_reader, Ref<RubiconNoteData> note) {
    int param_count = int32_t(p_reader->get_32());
    for (int i = 0; i < param_count; i++) {
        PackedByteArray param_name_buffer = get_next_buffer(p_reader);
        StringName param_name = RubiChartLoader::get_string_from_utf8(&param_name_buffer);
        Variant param_value = VariantUtilityFunctions::bytes_to_var(get_next_buffer(p_reader));
        note->parameters.set(param_name, param_value);
    }
}

Ref<RubiChart> RubiChartLoader::convert(const Ref<FileAccess> p_reader, const int version) {
    print_line("RubiChartLoader::convert");
    Ref<RubiChart> chart = memnew(RubiChart);
    chart->difficulty = p_reader->get_32();
    chart->scroll_speed = p_reader->get_float();
    PackedByteArray charter_buffer = get_next_buffer(p_reader);
    chart->charter = get_string_from_utf8(&charter_buffer);
    
    int note_types_amount = int32_t(p_reader->get_32());
    print_line("note type amount: "+String::num_int64(note_types_amount));
    PackedStringArray note_types;
    int i = 0;
    for (i = 0; i < note_types_amount; i++) {
        PackedByteArray note_type_buffer = get_next_buffer(p_reader);
        note_types.push_back(get_string_from_utf8(&note_type_buffer));
    }

    int chart_amount = int32_t(p_reader->get_32());
    print_line("chart amount: "+String::num_int64(chart_amount));
    for (i = 0; i < chart_amount; i++) {
        Ref<RubiconChartData> chart_data = memnew(RubiconChartData);
        PackedByteArray chart_name_buffer = get_next_buffer(p_reader);
        chart_data->chart_name = get_string_from_utf8(&chart_name_buffer);
        print_line(get_string_from_utf8(&chart_name_buffer));
        chart_data->lanes = int32_t(p_reader->get_32());
        print_line(chart_data->lanes);

        int s = 0;
        
        if (version == 33554432) { // target switch skipping for RBC 2.0.0
            // todo: proper skipping
            int target_switch_amount = int32_t(p_reader->get_32());
            for (s = 0; s < target_switch_amount; s++) {
                float time = p_reader->get_float();
                PackedByteArray switch_name_length = get_next_buffer(p_reader);
                String name = get_string_from_utf8(&switch_name_length);
            }

            //p_reader->seek(p_reader->get_position() + int32_t(p_reader->get_32()));
        }

        int sv_change_count = int32_t(p_reader->get_32());
        print_line("sv changes count: "+String::num_int64(sv_change_count));
        for (s = 0; s < sv_change_count; s++) {
            Ref<RubiconSvChange> sv_change = memnew(RubiconSvChange);
            sv_change->time = p_reader->get_float();
            sv_change->multiplier = p_reader->get_float();
            chart_data->sv_changes.push_back(sv_change);
        }

        TypedDictionary<uint8_t, RubiconNoteData> hold_note_cache;
        int section_count = int32_t(p_reader->get_32());
        TypedArray<RubiconSectionData> sections;
        print_line("section count: "+String::num_int64(section_count));
        while (section_count > 0) {
            section_count--;

            Ref<RubiconSectionData> cur_section = memnew(RubiconSectionData);
            cur_section->measure = int32_t(p_reader->get_32());
            sections.push_back(cur_section);

            int row_count = int32_t(p_reader->get_32());
            TypedArray<RubiconRowData> rows;
            print_line("row count: "+String::num_int64(row_count));
            while (row_count > 0) {
                row_count--;

                Ref<RubiconRowData> cur_row = memnew(RubiconRowData);
                cur_row->quant = p_reader->get_8();
                cur_row->offset = p_reader->get_8();
                cur_row->lane_priority = p_reader->get_8();

                rows.push_back(cur_row);

                uint8_t note_count = p_reader->get_8();
                //print_line("note count: "+String::num_int64(note_count));
                TypedArray<RubiconNoteData> starting_notes;
                TypedArray<RubiconNoteData> ending_notes;
                while (note_count > 0) {
                    note_count--;

                    uint8_t note_data = p_reader->get_8();
                    uint8_t lane = note_data & 0b00011111;
                    float measure_time = cur_section->measure + float(cur_row->offset) / cur_row->quant;
                    if (hold_note_cache.has(lane) && !hold_note_cache[lane].is_null()) {
                        Ref<RubiconNoteData> hold_note = hold_note_cache[lane];
                        hold_note->measure_length = measure_time - hold_note->measure_time;

                        hold_note_cache.erase(lane);
                        ending_notes.push_back(hold_note);
                    }

                    bool is_hold = ((note_data & 0b10000000) >> 7) == 1;
                    bool has_type = ((note_data & 0b01000000) >> 6) == 1;
                    bool has_params = ((note_data & 0b00100000) >> 5) == 1;

                    Ref<RubiconNoteData> cur_note = memnew(RubiconNoteData);
                    cur_note->measure_time = measure_time;
                    cur_note->lane = lane;

                    if (is_hold)
                        hold_note_cache[lane] = cur_note;
                    
                    if (has_type)
                        cur_note->type = note_types[int32_t(p_reader->get_32())];
                    
                    if (has_params)
                        read_note_parameters(p_reader, cur_note);
                    
                    starting_notes.push_back(cur_note);
                }

                cur_row->starting_notes = starting_notes;
                cur_row->ending_notes = ending_notes;
            }
            
            cur_section->rows = rows;
        }

        // here
        chart_data->sections = sections;

        int note_count = int32_t(p_reader->get_32());
        TypedArray<RubiconNoteData> stray_notes;
        for (int n = 0; n < note_count; n++) {
            Ref<RubiconNoteData> note = memnew(RubiconNoteData);
            stray_notes.push_back(note);

            uint8_t serialized_type = p_reader->get_8();
            note->measure_time = p_reader->get_float();
            note->lane = int32_t(p_reader->get_32());

            if (serialized_type >= 4) {
                note->measure_length = p_reader->get_float();
                serialized_type -= 4;
            }

            switch (serialized_type) {
                case 1: // Typed Note
                    note->type = note_types[int8_t(p_reader->get_32())];
                    break;
                case 2: // Note with parameters
                    read_note_parameters(p_reader, note);
                    break;
                case 3: // Typed Note with parameters
                    note->type = note_types[int8_t(p_reader->get_32())];
                    read_note_parameters(p_reader, note);
                    break;
            }
        }
        
        chart_data->strays = stray_notes;
        chart->charts.push_back(chart_data);
    }

    return chart;
}