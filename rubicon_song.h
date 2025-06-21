#ifndef RUBICON_SONG_H
#define RUBICON_SONG_H

#include "scene/main/node.h"
#include "scene/gui/control.h"
#include "rubicon_song_meta.h"

class RubiconSong : public Node {
    GDCLASS(RubiconSong, Node);

public:
    Ref<RubiconSongMeta> song_meta = nullptr;

    // TODO: Change from Control to Playfield whenever we make the class.
    Control* playfield = nullptr;

    void set_song_meta(const Ref<RubiconSongMeta> p_song_meta);
    Ref<RubiconSongMeta> get_song_meta() const;

    void set_playfield(const Control* p_playfield);
    Control* get_playfield() const;

protected:
    static void _bind_methods();
};

#endif