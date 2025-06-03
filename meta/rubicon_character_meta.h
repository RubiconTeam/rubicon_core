#ifndef RUBICON_CHARACTER_META_H
#define RUBICON_CHARACTER_META_H

#include "core/io/resource.h"

class RubiconCharacterMeta : public Resource {
    GDCLASS(RubiconCharacterMeta, Resource);

public:
    String character;
    StringName nickname;
    StringName bar_line;

    void set_character(const String p_character);
    String get_character() const;

    void set_nickname(const StringName p_nickname);
    StringName get_nickname() const;

    void set_bar_line(const StringName p_bar_line);
    StringName get_bar_line() const;

protected:
    static void _bind_methods();
};

#endif