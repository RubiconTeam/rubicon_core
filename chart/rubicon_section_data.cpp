#include "rubicon_section_data.h"
#include "../rubicon_core.h"

void RubiconSectionData::set_measure(const int p_value) {
    measure = p_value;
}

int RubiconSectionData::get_measure() const {
    return measure;
}

void RubiconSectionData::set_rows(const TypedArray<RubiconRowData> &p_rows) {
    rows = p_rows;
}

TypedArray<RubiconRowData> RubiconSectionData::get_rows() const {
    return rows;
}

void RubiconSectionData::convert_data(const Ref<RubiconTimeChangeArray> &p_time_changes, const TypedArray<RubiconScrollVelocity> &p_velocities) {
    for (int r = 0; r < rows.size(); r++) {
        Ref<RubiconRowData> cur = rows[r];
        cur->section = this;
        cur->convert_data(p_time_changes, p_velocities);
    }
}

Ref<RubiconRowData> RubiconSectionData::add_row(uint8_t p_offset, uint8_t p_quant) {
    gcd_offset_and_quant(p_offset, p_quant);

    Ref<RubiconRowData> row = get_row(p_offset, p_quant);
    if (!row.is_null())
        return row;
    
    row = memnew(RubiconRowData);
    row->offset = p_offset;
    row->quant = p_quant;

    rows.push_back(row);
    rows.sort_custom(callable_mp_static(RubiconRowData::compare_rows));

    return row;
}

void RubiconSectionData::remove_row(const Ref<RubiconRowData> p_row) {
    int row_index = rows.find(p_row);
    if (row_index != -1)
        rows.remove_at(row_index);;
    
    rows.sort_custom(callable_mp_static(RubiconRowData::compare_rows));
}

void RubiconSectionData::remove_row_at(uint8_t p_offset, uint8_t p_quant) {
    gcd_offset_and_quant(p_offset, p_quant);
    Ref<RubiconRowData> row = get_row(p_offset, p_quant);
    if (row.is_null())
        return;
    
    remove_row(row);
}

void RubiconSectionData::cleanup_rows() {
    TypedArray<RubiconRowData> new_list;
    for (int r = 0; r < rows.size(); r++) {
        Ref<RubiconRowData> cur = rows[r];
        if (cur->starting_notes.size() == 0 && cur->ending_notes.size() == 0)
            continue;
        
        new_list.push_back(cur);
    }

    rows = new_list;
}

Ref<RubiconRowData> RubiconSectionData::get_row(uint8_t p_offset, uint8_t p_quant) const {
    gcd_offset_and_quant(p_offset, p_quant);
    
    int index = rows.find_custom(callable_mp_static(RubiconRowData::is_of_value).bind(p_offset, p_quant));
    if (index != -1)
        return rows[index];
    
    return Ref<RubiconRowData>();
}

bool RubiconSectionData::has_row(uint8_t p_offset, uint8_t p_quant) const {
    gcd_offset_and_quant(p_offset, p_quant);
    return rows.find_custom(callable_mp_static(RubiconRowData::is_of_value).bind(p_offset, p_quant)) != -1;
}

bool RubiconSectionData::is_of_measure(const Variant &p_section, const uint8_t p_measure) {
    RubiconSectionData* casted_section = Object::cast_to<RubiconSectionData>(p_section);
    if (casted_section != nullptr)
        return casted_section->measure == p_measure;
    
    return false;
}

bool RubiconSectionData::compare_sections_by_measure(const Variant &p_a, const Variant &p_b) {
    RubiconSectionData* section_a = Object::cast_to<RubiconSectionData>(p_a);
    RubiconSectionData* section_b = Object::cast_to<RubiconSectionData>(p_b);
    if (section_a != nullptr && section_b != nullptr)
        return section_a->measure < section_b->measure;

    return p_a < p_b;
}

void RubiconSectionData::gcd_offset_and_quant(uint8_t &p_offset, uint8_t &p_quant) {
    for (int q = 0; q < RubiconCore::get_singleton()->quants.size(); q++) {
        uint8_t cur = RubiconCore::get_singleton()->quants[q];
        if (cur >= p_quant) 
            break;
        
        if (p_offset % cur != 0 || p_quant % cur != 0)
            continue;
        
        p_offset /= cur;
        p_quant = cur;
    }
}

void RubiconSectionData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_measure", "value"), &RubiconSectionData::set_measure);
    ClassDB::bind_method("get_measure", &RubiconSectionData::get_measure);
    ClassDB::bind_method(D_METHOD("set_rows", "value"), &RubiconSectionData::set_rows);
    ClassDB::bind_method("get_rows", &RubiconSectionData::get_rows);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "measure"), "set_measure", "get_measure");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "rows", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("RubiconRowData")), "set_rows", "get_rows");

    ClassDB::bind_method(D_METHOD("convert_data", "time_changes", "velocities"), &RubiconSectionData::convert_data);
    
    ClassDB::bind_method(D_METHOD("add_row", "offset", "quant"), &RubiconSectionData::add_row);
    ClassDB::bind_method(D_METHOD("remove_row", "row"), &RubiconSectionData::remove_row);
    ClassDB::bind_method(D_METHOD("remove_row_at", "offset", "quant"), &RubiconSectionData::remove_row_at);

    ClassDB::bind_method("cleanup_rows", &RubiconSectionData::cleanup_rows);
    ClassDB::bind_method(D_METHOD("get_row", "offset", "quant"), &RubiconSectionData::get_row);
    ClassDB::bind_method(D_METHOD("has_row", "offset", "quant"), &RubiconSectionData::has_row);

    ClassDB::bind_static_method("RubiconSectionData", D_METHOD("compare_sections_by_measure", "a", "b"), &RubiconSectionData::compare_sections_by_measure);
    //ClassDB::bind_static_method("RubiconSectionData", D_METHOD("gcd_offset_and_quant", "offset", "quant"), &RubiconSectionData::gcd_offset_and_quant);
}