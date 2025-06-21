#ifndef RUBICON_BAR_LINE_H
#define RUBICON_BAR_LINE_H

#include "core/object/class_db.h"
#include "scene/gui/control.h"
#include "core/object/gdvirtual.gen.inc"
#include "core/variant/typed_array.h"
#include "../chart/rubicon_chart_data.h"
#include "../chart/rubicon_note_data.h"

template <typename T>
class TypedArray;

class RubiconBarLine : public Control {
    GDCLASS(RubiconBarLine, Control);

public:
    void set_chart(const Ref<RubiconChartData> p_chart);
    Ref<RubiconChartData> get_chart() const;

    int get_lane_count() const;
    TypedArray<RubiconNoteData> get_notes_for_lane(const int p_lane) const;

    float get_distance_offset() const;

    virtual bool is_note_behind_lane(const int p_lane, const int p_note_index);

    virtual void draw_lane(const int p_lane);
    virtual void draw_note(const Ref<RubiconNoteData> p_data);

private:
    Ref<RubiconChartData> _chart;
    Vector<TypedArray<RubiconNoteData>> _note_list;

    float _distance_offset = 0;

    int _scroll_velocity_index = 0;

protected:
    void _notification(int p_what);
    static void _bind_methods();

    GDVIRTUAL2RC(bool, _is_note_behind_lane, int, int);

    GDVIRTUAL1(_draw_lane, int);
    GDVIRTUAL1(_draw_note, Ref<RubiconNoteData>);
};

#endif // RUBICON_BAR_LINE_H