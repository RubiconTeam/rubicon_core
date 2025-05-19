#ifndef RUBICHART_H
#define RUBICHART_H

#include "core/io/resource.h"
#include "core/variant/typed_array.h"
#include "rubicon_chart_data.h"
#include "rubicon_time_change.h"
#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"

template <typename T>
class TypedArray;

class RubiconChartData;

class RubiChart : public Resource {
    GDCLASS(RubiChart, Resource);

public:
    static const uint32_t CHART_VERSION = 33619968; // 2.1.0.0
    static const uint8_t MAX_LANE_COUNT = 32;

    String charter = "";
    uint32_t difficulty = 0;
    float scroll_speed = 0.0f;
    TypedArray<RubiconChartData> charts;

    void set_charter(const String &p_value);
    String get_charter() const;
    void set_difficulty(const uint32_t p_value);
    uint32_t get_difficulty() const;
    void set_scroll_speed(const float p_value);
    float get_scroll_speed() const;
    void set_charts(const TypedArray<RubiconChartData> &p_value);
    TypedArray<RubiconChartData> get_charts() const;

    void convert_data(const TypedArray<RubiconTimeChange> &p_time_changes);
    PackedStringArray get_all_notetypes() const;

protected:
    static void _bind_methods();
};

class RubiconChartLoader : public ResourceFormatLoader {
    public:
        virtual Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE) override;
        virtual void get_recognized_extensions(List<String> *p_extensions) const override;
        virtual bool handles_type(const String &p_type) const override;
        virtual String get_resource_type(const String &p_path) const override;
    };

class RubiconChartSaver : public ResourceFormatSaver {
    public:
        virtual Error save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags = 0) override;
        virtual void get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const override;
        virtual bool recognize(const Ref<Resource> &p_resource) const override;
    };

#endif // RUBICHART_H