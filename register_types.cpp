#include "register_types.h"

#include "core/config/engine.h"
#include "core/object/class_db.h"
#include "rubicon_conductor.h"
#include "rubicon_core.h"
#include "chart/rubicon_note_data.h"
#include "chart/rubicon_row_data.h"
#include "chart/rubichart.h"
#include "chart/rubicon_section_data.h"
#include "chart/rubicon_scroll_velocity.h"
#include "chart/rubicon_time_change.h"
#include "chart/rubicon_chart_data.h"
#include "modules/rubicon_engine/events/rubicon_event_data.h"
#include "modules/rubicon_engine/events/rubicon_event_meta.h"
//#include "meta/rubicon_song_difficulty.h"
#include "rubicon_song_meta.h"
#include "rubicon_song.h"

static RubiconCore *CorePtr;
static RubiconConductor *ConductorPtr;

static Ref<RubiconChartLoader> rubicon_chart_loader;
static Ref<RubiconChartSaver> rubicon_chart_saver;

void initialize_rubicon_core_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

	// Rubicon.Core
    GDREGISTER_CLASS(RubiconConductor);
    ConductorPtr = memnew(RubiconConductor);
    Engine::get_singleton()->add_singleton(Engine::Singleton("RubiconConductor", RubiconConductor::get_singleton()));  

	GDREGISTER_CLASS(RubiconCore);
	CorePtr = memnew(RubiconCore);
	Engine::get_singleton()->add_singleton(Engine::Singleton("RubiconCore", RubiconCore::get_singleton()));  

	GDREGISTER_CLASS(RubiconSong);

	// Rubicon.Core.Chart
	GDREGISTER_CLASS(RubiconNoteData);
	GDREGISTER_CLASS(RubiconRowData);
	GDREGISTER_CLASS(RubiconSectionData);
	GDREGISTER_CLASS(RubiconScrollVelocity);
	GDREGISTER_CLASS(RubiconTimeChange); 
	GDREGISTER_CLASS(RubiconChartData);
	GDREGISTER_CLASS(RubiChart);

	// Rubicon.Core.Meta
	GDREGISTER_CLASS(RubiconEventData);
	GDREGISTER_CLASS(RubiconEventMeta);
	//GDREGISTER_CLASS(RubiconSongDifficulty);
	GDREGISTER_CLASS(RubiconSongMeta);

	// Rubicon Chart Loader/Saver
	rubicon_chart_loader.instantiate();
	ResourceLoader::add_resource_format_loader(rubicon_chart_loader);
	
	rubicon_chart_saver.instantiate();
	ResourceSaver::add_resource_format_saver(rubicon_chart_saver);
}

void uninitialize_rubicon_core_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

    Engine::get_singleton()->remove_singleton("RubiconConductor");
	memdelete(ConductorPtr);

	Engine::get_singleton()->remove_singleton("RubiconCore");
	memdelete(CorePtr);

	ResourceLoader::remove_resource_format_loader(rubicon_chart_loader);
	rubicon_chart_loader = nullptr;
	ResourceSaver::remove_resource_format_saver(rubicon_chart_saver);
	rubicon_chart_saver = nullptr;
}