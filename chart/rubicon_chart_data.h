#ifndef RUBICON_CHART_DATA_H
#define RUBICON_CHART_DATA_H

#include "core/io/resource.h"
#include "core/object/class_db.h"
#include "core/variant/typed_array.h"
#include "core/string/string_name.h"
#include "rubicon_scroll_velocity.h"
#include "rubicon_section_data.h"
#include "rubicon_note_data.h"

template <typename T>
class TypedArray;

class RubiconSectionData;

class RubiconChartData : public Resource {
    GDCLASS(RubiconChartData, Resource);

public:
    StringName chart_name = "";
    uint8_t lanes = 0;
    TypedArray<RubiconScrollVelocity> scroll_velocities;

    TypedArray<RubiconSectionData> sections;
    TypedArray<RubiconNoteData> strays;

    void set_chart_name(const StringName &p_name);
    StringName get_chart_name() const;
    void set_lanes(const uint8_t p_lanes);
    uint8_t get_lanes() const;
    void set_scroll_velocities(const TypedArray<RubiconScrollVelocity> &p_value);
    TypedArray<RubiconScrollVelocity> get_scroll_velocities() const;

    void set_sections(const TypedArray<RubiconSectionData> &p_value);
    TypedArray<RubiconSectionData> get_sections() const;
    void set_strays(const TypedArray<RubiconNoteData> &p_value);
    TypedArray<RubiconNoteData> get_strays() const;

    TypedArray<RubiconNoteData> get_notes(const bool p_include_ends = false) const;
    TypedArray<RubiconNoteData> get_notes_at_lane(const uint8_t p_lane, const bool p_include_ends = false) const;
    TypedArray<RubiconNoteData> get_notes_of_type(const StringName &p_type, const bool p_include_ends = false) const;

    void add_note_start(Ref<RubiconNoteData> p_note, const int p_measure, const uint8_t p_offset, const uint8_t p_quant);
    void add_note_end(Ref<RubiconNoteData> p_note, const int p_measure, const uint8_t p_offset, const uint8_t p_quant);
    void add_note_at_measure_time(Ref<RubiconNoteData> p_note, const float p_measure_time, const float p_length);
    void add_stray_note(Ref<RubiconNoteData> note);

    void move_note_start(Ref<RubiconNoteData> p_note, const int p_measure, const uint8_t p_offset, const uint8_t p_quant);
    void move_note_end(Ref<RubiconNoteData> p_note, const int p_measure, const uint8_t p_offset, const uint8_t p_quant);

    void remove_note_start(Ref<RubiconNoteData> p_note);
    void remove_note_end(Ref<RubiconNoteData> p_note);
    void remove_stray_note(Ref<RubiconNoteData> p_note);

    Ref<RubiconSectionData> add_section(const int p_measure);
    void remove_section(const int p_measure);
    Ref<RubiconSectionData> get_section_at_measure(const int p_measure) const;

    void cleanup_sections();
    void convert_data(const TypedArray<RubiconTimeChange> &p_time_changes);

    static void measure_offset_to_offset_and_quant(const float p_measure_time, uint8_t &p_offset, uint8_t &p_quant);

protected:
    static void _bind_methods();
};

#endif // RUBICON_CHART_DATA_H