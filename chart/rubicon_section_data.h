#ifndef RUBICON_SECTION_DATA_H
#define RUBICON_SECTION_DATA_H

#include "core/io/resource.h"
#include "rubicon_time_change.h"
#include "rubicon_sv_change.h"
#include "rubicon_row_data.h"
#include "rubichart.h"

template <typename T>
class TypedArray;

class RubiconRowData;

class RubiconSectionData : public Resource {
    GDCLASS(RubiconSectionData, Resource);

public:
    int measure = 0;
    TypedArray<RubiconRowData> rows;

    void set_measure(const int p_value);
    int get_measure() const;

    void set_rows(const TypedArray<RubiconRowData> &p_rows);
    TypedArray<RubiconRowData> get_rows() const;

    void convert_data(const TypedArray<RubiconTimeChange> &p_time_changes, const TypedArray<RubiconSvChange> &p_sv_changes);
    
    Ref<RubiconRowData> add_row(uint8_t p_offset, RubiChart::QuantValue p_quant);
    void remove_row(const Ref<RubiconRowData> p_row);
    void remove_row_at(uint8_t p_offset, RubiChart::QuantValue p_quant);

    void cleanup_rows();
    Ref<RubiconRowData> get_row(uint8_t p_offset, RubiChart::QuantValue p_quant) const;
    bool has_row(uint8_t p_offset, RubiChart::QuantValue p_quant) const;

    static bool is_of_measure(const Variant &p_section, const uint8_t p_measure);
    static bool compare_sections_by_measure(const Variant &p_a, const Variant &p_b);
    static void gcd_offset_and_quant(uint8_t &p_offset, RubiChart::QuantValue &p_quant);

protected:
    static void _bind_methods();
};

#endif // RUBICON_SECTION_DATA_H