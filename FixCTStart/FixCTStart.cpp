#include "pch.h"
#include "FixCTStart.h"


BAKKESMOD_PLUGIN(FixCTStart, "Fix Custom Training Start", plugin_version, PLUGINTYPE_FREEPLAY);

void FixCTStart::onLoad()
{
	ignoreSteer = std::make_shared<bool>();
	ignoreThrottle = std::make_shared<bool>();

	cvarManager->registerCvar("fix_ct_start_steer", "1", "Prevent steer input from starting training", true, true, 0, true, 1)
		.bindTo(ignoreSteer);
	cvarManager->registerCvar("fix_ct_start_throttle", "0", "Prevent throttle input from starting training", true, true, 0, true, 1)
		.bindTo(ignoreThrottle);

	// set steer and throttle on countdown tick to 0 and then set it back after
	gameWrapper->HookEvent("Function GameEvent_TrainingEditor_TA.Countdown.Tick", [this](std::string eventName) {
		auto pc = gameWrapper->GetPlayerController();
		if (!pc) return;
		auto input = pc.GetVehicleInput();
		steer = input.Steer;
		throttle = input.Throttle;
		if (*ignoreSteer) input.Steer = 0.f;
		if (*ignoreThrottle) input.Throttle = 0.f;
		pc.SetVehicleInput(input);
	});
	gameWrapper->HookEventPost("Function GameEvent_TrainingEditor_TA.Countdown.Tick", [this](std::string eventName) {
		auto pc = gameWrapper->GetPlayerController();
		if (!pc) return;
		auto input = pc.GetVehicleInput();
		input.Steer = steer;
		input.Throttle = throttle;
		pc.SetVehicleInput(input);
	});
}

void FixCTStart::onUnload()
{
}
