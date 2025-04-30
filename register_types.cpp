#include "register_types.h"

#include "core/config/engine.h"
#include "core/object/class_db.h"
#include "modules/rubicon_core/conductor.h"
#include "modules/rubicon_core/chart/time_change.h"

static Conductor *ConductorPtr;

void initialize_rubicon_core_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

	// Register classes

	// Rubicon.Core
    GDREGISTER_CLASS(Conductor);

	// Rubicon.Core.Chart
	GDREGISTER_CLASS(TimeChange);

	// Initialize singletons
    ConductorPtr = memnew(Conductor);
    Engine::get_singleton()->add_singleton(Engine::Singleton("Conductor", Conductor::get_singleton()));   
}

void uninitialize_rubicon_core_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

    Engine::get_singleton()->remove_singleton("Conductor");
	memdelete(ConductorPtr);
}