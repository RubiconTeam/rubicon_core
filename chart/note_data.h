#ifndef NOTE_DATA_H
#define NOTE_DATA_H

#include "core/io/resource.h"

#include "core/object/class_db.h"
#include "core/variant/variant.h"
#include "core/variant/typed_dictionary.h"

template <typename K, typename V>
class TypedDictionary;

class NoteData : public Resource {
    GDCLASS(NoteData, Resource);

public:
    u_int8_t lane;
    StringName type = SNAME("Normal");
    TypedDictionary<StringName, Variant> parameters;
    
    float ms_time;
    float ms_length;

    float measure_time;
    float measure_length;

    int starting_scroll_velocity = 0;
    int ending_scroll_velocity = 0;

    bool should_miss = false;
    bool hit = false;
    bool spawned = false;
    bool counts_towards_score = true;
};

#endif // NOTE_DATA_H