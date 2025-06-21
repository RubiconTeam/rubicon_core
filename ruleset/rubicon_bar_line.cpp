#include "rubicon_bar_line.h"
#include "../rubicon_conductor.h"

void RubiconBarLine::set_chart(const Ref<RubiconChartData> p_chart) {
    _chart = p_chart;
    queue_redraw();
}

Ref<RubiconChartData> RubiconBarLine::get_chart() const {
    return _chart;
}

int RubiconBarLine::get_lane_count() const {
    return _note_list.size();
}

TypedArray<RubiconNoteData> RubiconBarLine::get_notes_for_lane(const int p_lane) const {
    ERR_FAIL_COND_V_MSG(p_lane < 0 || p_lane >= _note_list.size(), TypedArray<RubiconNoteData>(), "Requested lane was outside the bounds of the bar line's lane amount.");
    
    return _note_list[p_lane];
}

float RubiconBarLine::get_distance_offset() const {
    return _distance_offset;
}

bool RubiconBarLine::is_note_behind_lane(const int p_lane, const int p_note_index) {
    bool ret = false;
    GDVIRTUAL_CALL(_is_note_behind_lane, p_lane, p_note_index, ret);
    return ret;
}

void RubiconBarLine::draw_lane(const int p_lane) {
    GDVIRTUAL_CALL(_draw_lane, p_lane);
}

void RubiconBarLine::draw_note(const Ref<RubiconNoteData> p_data) {
    GDVIRTUAL_CALL(_draw_note, p_data);
}

void RubiconBarLine::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_DRAW: {
            int i; // Note index
            for (int l = 0; l < get_lane_count(); l++) {
                // Draw notes that are behind the lane first
                for (i = 0; i < get_notes_for_lane(l).size(); i++) 
                    if (is_note_behind_lane(l, i))
                        draw_note(get_notes_for_lane(l)[i]);
                
                draw_lane(l);

                // Draw notes that are in front of the lane last
                for (i = 0; i < get_notes_for_lane(l).size(); i++) 
                    if (!is_note_behind_lane(l, i))
                        draw_note(get_notes_for_lane(l)[i]);
            }
        } break;
        case NOTIFICATION_INTERNAL_PROCESS: {
            if (_chart == nullptr)
                break;

            if (_chart->scroll_velocities.is_empty()) {
                WARN_PRINT("There are no scroll velocities in this chart, so the distance offset will always be 0.");
                break;
            }

            float time = 0; // GET THE TIME SOMEHOW  //RubiconConductor::get_singleton()->get_time();
            while (_scroll_velocity_index + 1 < _chart->scroll_velocities.size() && static_cast<Ref<RubiconScrollVelocity>>(_chart->scroll_velocities[_scroll_velocity_index + 1])->ms_time - time <= 0)
                _scroll_velocity_index++;
            
            Ref<RubiconScrollVelocity> current = _chart->scroll_velocities[_scroll_velocity_index];
            _distance_offset = current->position + (time - current->ms_time) * current->multiplier * -1;
        } break;
    }
}