#include "rubicon_core.h"
#include "chart/rubichart.h"

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

    quants = {
        RubiChart::QuantValue::QUANT_VALUE_4TH,
        RubiChart::QuantValue::QUANT_VALUE_8TH,
        RubiChart::QuantValue::QUANT_VALUE_12TH,
        RubiChart::QuantValue::QUANT_VALUE_16TH,
        RubiChart::QuantValue::QUANT_VALUE_24ND,
        RubiChart::QuantValue::QUANT_VALUE_32ND,
        RubiChart::QuantValue::QUANT_VALUE_32ND,
        RubiChart::QuantValue::QUANT_VALUE_48TH,
        RubiChart::QuantValue::QUANT_VALUE_64TH,
        RubiChart::QuantValue::QUANT_VALUE_192ND
    };
}

RubiconCore::~RubiconCore() {
    if (singleton == this) {
        singleton = nullptr;
    }
}