#include "register_types.h"

#include "core/config/engine.h"
#include "core/object/class_db.h"
#include "rubicon_conductor.h"
#include "rubicon_core.h"
#include "chart/rubicon_note_data.h"
#include "chart/rubicon_row_data.h"
#include "chart/rubichart.h"
#include "chart/rubicon_section_data.h"
#include "chart/rubicon_sv_change.h"
#include "chart/rubicon_time_change.h"

static RubiconCore *CorePtr;
static RubiconConductor *ConductorPtr;

void initialize_rubicon_core_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

	// Register classes

	// Rubicon.Core
    GDREGISTER_CLASS(RubiconConductor);
    ConductorPtr = memnew(RubiconConductor);
    Engine::get_singleton()->add_singleton(Engine::Singleton("RubiconConductor", RubiconConductor::get_singleton()));  

	GDREGISTER_CLASS(RubiconCore);
	CorePtr = memnew(RubiconCore);
	Engine::get_singleton()->add_singleton(Engine::Singleton("RubiconCore", RubiconCore::get_singleton()));  

	// Rubicon.Core.Chart
	GDREGISTER_CLASS(RubiconNoteData);
	GDREGISTER_CLASS(RubiconRowData);
	GDREGISTER_CLASS(RubiChart);
	GDREGISTER_CLASS(RubiconSectionData);
	GDREGISTER_CLASS(RubiconSvChange);
	GDREGISTER_CLASS(RubiconTimeChange); 
}

void uninitialize_rubicon_core_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

    Engine::get_singleton()->remove_singleton("RubiconConductor");
	memdelete(ConductorPtr);

	Engine::get_singleton()->remove_singleton("RubiconCore");
	memdelete(CorePtr);
}