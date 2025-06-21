#include "rubicon_song.h"

void RubiconSong::set_song_meta(const Ref<RubiconSongMeta> p_song_meta) {
    song_meta = p_song_meta;
}

Ref<RubiconSongMeta> RubiconSong::get_song_meta() const {
    return song_meta;
}

void RubiconSong::set_playfield(Control* p_playfield) {
    playfield = p_playfield;
}

Control* RubiconSong::get_playfield() const {
    if (playfield)
        return playfield;
    
    return Object::cast_to<Control>(Variant());
}

void RubiconSong::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_song_meta", "song_meta"), &RubiconSong::set_song_meta);
    ClassDB::bind_method("get_song_meta", &RubiconSong::get_song_meta);

    ClassDB::bind_method(D_METHOD("set_playfield", "playfield"), &RubiconSong::set_playfield);
    ClassDB::bind_method("get_playfield", &RubiconSong::get_playfield);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "song_meta", PROPERTY_HINT_RESOURCE_TYPE, "RubiconSongMeta"), "set_song_meta", "get_song_meta");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "playfield", PROPERTY_HINT_NODE_TYPE, "Control"), "set_playfield", "get_playfield");
}