#include "rubicon_character_meta.h"

void RubiconCharacterMeta::set_character(const String p_character) {
    character = p_character;
}

String RubiconCharacterMeta::get_character() const {
    return character;
}

void RubiconCharacterMeta::set_nickname(const StringName p_nickname) {
    nickname = p_nickname;
}

StringName RubiconCharacterMeta::get_nickname() const {
    return nickname;
}

void RubiconCharacterMeta::set_bar_line(const StringName p_bar_line) {
    bar_line = p_bar_line;
}

StringName RubiconCharacterMeta::get_bar_line() const {
    return bar_line;
}

void RubiconCharacterMeta::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_character", "character"), &RubiconCharacterMeta::set_character);
    ClassDB::bind_method("get_character", &RubiconCharacterMeta::get_character);

    ClassDB::bind_method(D_METHOD("set_nickname", "nickname"), &RubiconCharacterMeta::set_nickname);
    ClassDB::bind_method("get_nickname", &RubiconCharacterMeta::get_nickname);

    ClassDB::bind_method(D_METHOD("set_bar_line", "bar_line"), &RubiconCharacterMeta::set_bar_line);
    ClassDB::bind_method("get_bar_line", &RubiconCharacterMeta::get_bar_line);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "character", PROPERTY_HINT_FILE, "*.tscn,*.scn"), "set_character", "get_character");
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "nickname"), "set_nickname", "get_nickname");
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "bar_line"), "set_bar_line", "get_bar_line");
}