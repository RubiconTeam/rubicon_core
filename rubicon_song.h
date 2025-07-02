#ifndef RUBICON_SONG_H
#define RUBICON_SONG_H

#include "scene/main/node.h"
#include "scene/gui/control.h"
#include "rubicon_song_meta.h"
#include "rubicon_play_field.h"

class RubiconSong : public Node {
    GDCLASS(RubiconSong, Node);

public:
    bool follow_conductor = false;

    Ref<RubiconSongMeta> song_meta = nullptr;

    // TODO: Change from Control to Playfield whenever we make the class.
    RubiconPlayField* play_field = nullptr;

    void set_song_meta(const Ref<RubiconSongMeta> p_song_meta);
    Ref<RubiconSongMeta> get_song_meta() const;

    void set_play_field(RubiconPlayField* p_play_field);
    RubiconPlayField* get_play_field() const;

    void set_follow_conductor(const bool p_value);
    bool get_follow_conductor() const;
    void set_song_position(const float p_value);
    float get_song_position() const;

protected:
    static void _bind_methods();
    void _validate_property(PropertyInfo &p_property) const;

private:
    float _song_position = 0.0;
};

#endif