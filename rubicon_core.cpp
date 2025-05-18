#include "rubicon_core.h"

RubiconCore *RubiconCore::singleton = nullptr;

RubiconCore *RubiconCore::get_singleton() {
    return singleton;
}

void RubiconCore::set_quants(const PackedByteArray &p_quants) {
    quants = p_quants;
}

PackedByteArray RubiconCore::get_quants() const {
    return quants;
}

void RubiconCore::_bind_methods() {

}

RubiconCore::RubiconCore() {
    singleton = this;

    quants = {4, 8, 12, 16, 24, 32, 48, 64, 192};
}

RubiconCore::~RubiconCore() {
    if (singleton == this) {
        singleton = nullptr;
    }
}