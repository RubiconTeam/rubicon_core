#include "register_types.h"

#include "core/config/engine.h"
#include "core/object/class_db.h"
#include "conductor.h"
#include "chart/note_data.h"
#include "chart/row_data.h"
#include "chart/rubichart.h"
#include "chart/section_data.h"
#include "chart/sv_change.h"
#include "chart/time_change.h"

static Conductor *ConductorPtr;

void initialize_rubicon_core_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

	// Register classes

	// Rubicon.Core
    GDREGISTER_CLASS(Conductor);

	// Rubicon.Core.Chart
	GDREGISTER_CLASS(NoteData);
	GDREGISTER_CLASS(RowData);
	GDREGISTER_CLASS(RubiChart);
	GDREGISTER_CLASS(SectionData);
	GDREGISTER_CLASS(SvChange);
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