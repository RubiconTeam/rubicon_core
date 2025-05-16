#include "section_data.h"
#include "../rubicon_core.h"

void SectionData::set_measure(const int p_value) {
    measure = p_value;
}

int SectionData::get_measure() const {
    return measure;
}

void SectionData::set_rows(const TypedArray<RowData> &p_rows) {
    rows = p_rows;
}

TypedArray<RowData> SectionData::get_rows() const {
    return rows;
}

void SectionData::convert_data(const TypedArray<TimeChange> &p_time_changes, const TypedArray<SvChange> &p_sv_changes) {
    for (int r = 0; r < rows.size(); r++) {
        Ref<RowData> cur = rows[r];
        cur->section = this;
        cur->convert_data(p_time_changes, p_sv_changes);
    }
}

Ref<RowData> SectionData::add_row(uint8_t p_offset, RubiChart::QuantValue p_quant) {
    gcd_offset_and_quant(p_offset, p_quant);

    Ref<RowData> row = get_row(p_offset, p_quant);
    if (!row.is_null())
        return row;
    
    row = memnew(RowData);
    row->offset = p_offset;
    row->quant = p_quant;

    rows.push_back(row);
    rows.sort_custom(callable_mp_static(RowData::compare_rows));

    return row;
}

void SectionData::remove_row(const Ref<RowData> p_row) {
    int row_index = rows.find(p_row);
    if (row_index != -1)
        rows.remove_at(row_index);;
    
    rows.sort_custom(callable_mp_static(RowData::compare_rows));
}

void SectionData::remove_row_at(uint8_t p_offset, RubiChart::QuantValue p_quant) {
    gcd_offset_and_quant(p_offset, p_quant);
    Ref<RowData> row = get_row(p_offset, p_quant);
    if (row.is_null())
        return;
    
    remove_row(row);
}

void SectionData::cleanup_rows() {
    TypedArray<RowData> new_list;
    for (int r = 0; r < rows.size(); r++) {
        Ref<RowData> cur = rows[r];
        if (cur->starting_notes.size() == 0 && cur->ending_notes.size() == 0)
            continue;
        
        new_list.push_back(cur);
    }

    rows = new_list;
}

Ref<RowData> SectionData::get_row(uint8_t p_offset, RubiChart::QuantValue p_quant) const {
    gcd_offset_and_quant(p_offset, p_quant);
    
    int index = rows.find_custom(callable_mp_static(RowData::is_of_value).bind(p_offset, p_quant));
    if (index != -1)
        return rows[index];
    
    return Ref<RowData>();
}

bool SectionData::has_row(uint8_t p_offset, RubiChart::QuantValue p_quant) const {
    gcd_offset_and_quant(p_offset, p_quant);
    return rows.find_custom(callable_mp_static(RowData::is_of_value).bind(p_offset, p_quant)) != -1;
}

void SectionData::gcd_offset_and_quant(uint8_t &p_offset, RubiChart::QuantValue &p_quant) {
    for (int q = 0; q < RubiconCore::get_singleton()->quants.size(); q++) {
        uint8_t cur = RubiconCore::get_singleton()->quants[q];
        if (cur >= p_quant) 
            break;
        
        if (p_offset % cur != 0 || p_quant % cur != 0)
            continue;
        
        p_offset /= cur;
        p_quant = static_cast<RubiChart::QuantValue>(cur);
    }
}

void SectionData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_measure", "value"), &SectionData::set_measure);
    ClassDB::bind_method("get_measure", &SectionData::get_measure);
    ClassDB::bind_method(D_METHOD("set_rows", "value"), &SectionData::set_rows);
    ClassDB::bind_method("get_rows", &SectionData::get_rows);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "measure"), "set_measure", "get_measure");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "rows", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RowData")), "set_rows", "get_rows");

    ClassDB::bind_method(D_METHOD("convert_data", "time_changes", "sv_changes"), &SectionData::convert_data);
    
    ClassDB::bind_method(D_METHOD("add_row", "offset", "quant"), &SectionData::add_row);
    ClassDB::bind_method(D_METHOD("remove_row", "row"), &SectionData::remove_row);
    ClassDB::bind_method(D_METHOD("remove_row_at", "offset", "quant"), &SectionData::remove_row_at);

    ClassDB::bind_method("cleanup_rows", &SectionData::cleanup_rows);
    ClassDB::bind_method(D_METHOD("get_row", "offset", "quant"), &SectionData::get_row);
    ClassDB::bind_method(D_METHOD("has_row", "offset", "quant"), &SectionData::has_row);

    //ClassDB::bind_static_method("SectionData", D_METHOD("gcd_offset_and_quant", "offset", "quant"), &SectionData::gcd_offset_and_quant);
}