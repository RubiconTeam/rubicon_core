#include "rubicon_song.h"
#include "rubicon_conductor.h"

void RubiconSong::set_song_meta(const Ref<RubiconSongMeta> p_song_meta) {
    song_meta = p_song_meta;
}

Ref<RubiconSongMeta> RubiconSong::get_song_meta() const {
    return song_meta;
}

void RubiconSong::set_play_field(RubiconPlayField* p_play_field) {
    play_field = p_play_field;
}

RubiconPlayField* RubiconSong::get_play_field() const {
    if (play_field)
        return play_field;
    
    return Object::cast_to<RubiconPlayField>(Variant());
}

void RubiconSong::set_follow_conductor(const bool p_value) {
    follow_conductor = p_value;
}

bool RubiconSong::get_follow_conductor() const {
    return follow_conductor;
}

void RubiconSong::set_song_position(const float p_value) {
    if (follow_conductor)
        return;
    
    _song_position = p_value;
}

float RubiconSong::get_song_position() const {
    return _song_position;
}

void RubiconSong::_validate_property(PropertyInfo &p_property) const {
    if (follow_conductor && p_property.name == "song_position") {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}
}

void RubiconSong::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_song_meta", "song_meta"), &RubiconSong::set_song_meta);
    ClassDB::bind_method("get_song_meta", &RubiconSong::get_song_meta);

    ClassDB::bind_method(D_METHOD("set_play_field", "play_field"), &RubiconSong::set_play_field);
    ClassDB::bind_method("get_play_field", &RubiconSong::get_play_field);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "song_meta", PROPERTY_HINT_RESOURCE_TYPE, "RubiconSongMeta"), "set_song_meta", "get_song_meta");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "play_field", PROPERTY_HINT_NODE_TYPE, "Control"), "set_play_field", "get_play_field");
}