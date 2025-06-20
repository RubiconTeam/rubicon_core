#include "rubichart_saver.h"
#include "core/variant/variant_utility.h"

void write_note_parameters(Ref<FileAccess> p_writer, Ref<RubiconNoteData> p_note) {
    p_writer->store_32(p_note->parameters.size());

    TypedArray<StringName> param_keys = p_note->parameters.keys();
    for (int k = 0; k < param_keys.size(); k++) {
        PackedByteArray param_name_buffer = String(param_keys[k]).to_utf8_buffer();
        p_writer->store_32(param_name_buffer.size());
        p_writer->store_buffer(param_name_buffer);

        PackedByteArray param_value_buffer = VariantUtilityFunctions::var_to_bytes(p_note->parameters[k]);
        p_writer->store_32(param_value_buffer.size());
        p_writer->store_buffer(param_value_buffer);
    }
}

int get_serialized_tap_note_type(Ref<RubiconNoteData> p_note) {
    if (String(p_note->type).to_lower() != "normal") {
        if (p_note->parameters.size() > 0)
            return 3; // Typed Tap Note with parameters
        
        return 1; // Typed Tap Note
    }

    if (p_note->parameters.size() > 0)
        return 2; // Tap Note with parameters
    
    return 0; // Normal Tap Note
}

int get_serialized_type(Ref<RubiconNoteData> p_note) {
    int offset = 0;
    if (p_note->measure_length > 0.0) {
        offset += 4;
    }

    return get_serialized_tap_note_type(p_note) + offset;
}

void RubiChartSaver::save(const Ref<RubiChart> chart, Ref<FileAccess> writer) {
    TypedArray<StringName> note_types;
    TypedDictionary<StringName, int> type_index_map;

    // Note type caching
    int i;
    for (i = 0; i < chart->charts.size(); i++) {
        Ref<RubiconChartData> chart_data = chart->charts[i];

        int s;
        for (s = 0; s < chart_data->sections.size(); s++) {
            Ref<RubiconSectionData> section = chart_data->sections[s];
            
            for (int r = 0; r < section->rows.size(); r++) {
                Ref<RubiconRowData> row = section->rows[r];

                for (int n = 0; n < row->starting_notes.size(); n++) {
                    Ref<RubiconNoteData> note = row->starting_notes[n];

                    bool has_type = String(note->type).to_lower() != "normal";
                    if (!has_type || type_index_map.has(note->type))
                        continue;
                    print_line("found note type: "+String(note->type));
                    note_types.push_back(note->type);
                    type_index_map.set(note->type, note_types.size() - 1);
                }
            }
        }

        // Stray note type caching
        for (s = 0; s < chart_data->strays.size(); s++) {
            Ref<RubiconNoteData> stray_note = chart_data->strays[s];

            bool has_type = String(stray_note->type).to_lower() != "normal";
            if (!has_type || type_index_map.has(stray_note->type))
                continue;
                    
            note_types.push_back(stray_note->type);
            type_index_map.set(stray_note->type, note_types.size() - 1);
        }
    }

    String signature = "RBCN";
    writer->store_buffer(signature.to_utf8_buffer());
    writer->store_32(chart->CHART_VERSION);
    writer->store_32(chart->difficulty);
    writer->store_float(chart->scroll_speed);

    PackedByteArray charter_buffer = chart->charter.to_utf8_buffer();
    writer->store_32(charter_buffer.size());
    writer->store_buffer(charter_buffer);

    writer->store_32(note_types.size());
    for (i = 0; i < note_types.size(); i++) {
        PackedByteArray note_type_buffer = String(note_types[i]).to_utf8_buffer();
        writer->store_32(note_type_buffer.size());
        writer->store_buffer(note_type_buffer);
    }

    writer->store_32(chart->charts.size());
    for (i = 0; i < chart->charts.size(); i++) {
        Ref<RubiconChartData> chart_data = chart->charts[i];

        PackedByteArray name_buffer = String(chart_data->chart_name).to_utf8_buffer();
        writer->store_32(name_buffer.size());
        writer->store_buffer(name_buffer);

        writer->store_32(chart_data->lanes);

        writer->store_32(chart_data->scroll_velocities.size());
        int s;
        for (s = 0; s < chart_data->scroll_velocities.size(); s++) {
            Ref<RubiconScrollVelocity> sv = chart_data->scroll_velocities[s];
            writer->store_float(sv->time);
            writer->store_float(sv->multiplier);
        }

        writer->store_32(chart_data->sections.size());
        for (s = 0; s < chart_data->sections.size(); s++) {
            Ref<RubiconSectionData> section = chart_data->sections[s];
            writer->store_32(section->measure);

            writer->store_32(section->rows.size());
            for (int r = 0; r < section->rows.size(); r++) {
                Ref<RubiconRowData> row = section->rows[r];
                row->section = section;
                writer->store_8(row->quant);
                writer->store_8(row->offset);
                writer->store_8(row->lane_priority);

                TypedArray<RubiconNoteData> row_notes = row->get_notes(true);
                writer->store_8(row_notes.size());
                for (int n = 0; n < row_notes.size(); n++) {
                    Ref<RubiconNoteData> note = row_notes[n];
                    int note_data = note->lane;

                    bool is_ending_row = row->ending_notes.has(note);
                    if (is_ending_row) {
                        writer->store_8(note_data);
                        continue;
                    }

                    bool is_hold = note->measure_length > 0;
                    if (is_hold)
                        note_data = (1 << 7) | note_data;
                    
                    bool has_type = String(note->type).to_lower() != "normal";
                    if (has_type)
                        note_data = (1 << 6) | note_data;
                    
                    bool has_params = note->parameters.size() > 0;
                    if (has_params)
                        note_data = (1 << 5) | note_data;
                    
                    writer->store_8(note_data);
                    if (has_type)
                        writer->store_32(type_index_map[note->type]);
                    
                    if (has_params)
                        write_note_parameters(writer, note);
                }
            }
        }

        // Strays
        writer->store_32(chart_data->strays.size());
        for (s = 0; s < chart_data->strays.size(); s++) {
            Ref<RubiconNoteData> stray_note = chart_data->strays[s];
            int serialized_type = get_serialized_type(stray_note);
            writer->store_8(serialized_type);

            writer->store_float(stray_note->measure_time);
            writer->store_32(stray_note->lane);

            if (serialized_type >= 4) {
                writer->store_float(stray_note->measure_length);
                serialized_type -= 4;
            }

            switch (serialized_type)
            {
                case 1: // Typed Note
                    writer->store_32(type_index_map[stray_note->type]);
                    break;
                case 2: // Note with parameters
                    write_note_parameters(writer, stray_note);
                    break;
                case 3: // Typed Note with parameters
                    writer->store_32(type_index_map[stray_note->type]);
                    write_note_parameters(writer, stray_note);
                    break;
            }
        }
    }
}