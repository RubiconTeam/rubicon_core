#ifndef SECTION_DATA_H
#define SECTION_DATA_H

#include "core/io/resource.h"
#include "time_change.h"
#include "sv_change.h"
#include "row_data.h"
#include "rubichart.h"

template <typename T>
class TypedArray;

class RowData;

class SectionData : public Resource {
    GDCLASS(SectionData, Resource);

public:
    int measure = 0;
    TypedArray<RowData> rows;

    void set_measure(const int p_value);
    int get_measure() const;

    void set_rows(const TypedArray<RowData> &p_rows);
    TypedArray<RowData> get_rows() const;

    void convert_data(const TypedArray<TimeChange> &p_time_changes, const TypedArray<SvChange> &p_sv_changes);
    
    Ref<RowData> add_row(uint8_t p_offset, RubiChart::QuantValue p_quant);
    void remove_row(const Ref<RowData> p_row);
    void remove_row_at(uint8_t p_offset, RubiChart::QuantValue p_quant);

    void cleanup_rows();
    Ref<RowData> get_row(uint8_t p_offset, RubiChart::QuantValue p_quant) const;
    bool has_row(uint8_t p_offset, RubiChart::QuantValue p_quant) const;

    static void gcd_offset_and_quant(uint8_t &p_offset, RubiChart::QuantValue &p_quant);

protected:
    static void _bind_methods();
};

#endif // SECTION_DATA_H