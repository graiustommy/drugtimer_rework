#include "plugin.h"
#include <game_api.h>
#include <chrono>
#include <thread>

std::unique_ptr<SAMPFUNCS> SF;

auto noFloodMS = std::chrono::milliseconds(666);
bool isReadyToUse = true, useAfterDeath = false;
int coolDown = 60;



void NoFloodByPass(std::string cmdline) {
	std::this_thread::sleep_for(noFloodMS);
	SF->getSAMP()->getPlayers()->localPlayerData->Say((char*)cmdline.data());
}

void countDown() {
	int len = coolDown;
	for (int i = 0; i < len; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		coolDown--;
	}
	coolDown = 60;
	isReadyToUse = true;
	SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(213, 240, 218), "{d5f0da}[drug_time]: Ready.");
}

bool  checkIfWasNotUsed(int hpAfter) {
	if (hpAfter < 150) {
		return true;
	}
	return false;
}

static void CALLBACK mainloop() {
	static bool initialized = false;	
	if (!initialized && GAME && GAME->GetSystemState() == eSystemState::GS_PLAYING_GAME && SF->getSAMP()->IsInitialized()) {
		initialized = true;
		SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(166, 162, 152), "{d5f0da}[drug_time]: {25f54b}loaded. {d5f0da}-> /dtc.");

	}
	else {
		if (SF->getGame()->isKeyPressed(88)) {
			if (isReadyToUse) {
				int hp = SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->hitpoints;
				int needToUse = (160 - hp) / 10 + 1;
				std::string cmdline = "/usedrugs " + std::to_string(needToUse);
				SF->getSAMP()->getPlayers()->localPlayerData->Say((char*)cmdline.data());
				if (checkIfWasNotUsed(SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->hitpoints)) {
					std::thread DoAgain(NoFloodByPass, cmdline);
					DoAgain.detach();
				}
				isReadyToUse = false;
				std::thread DrugTimer(countDown);
				DrugTimer.detach();
			}
			else {
				SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(166, 162, 152), "{d5f0da}[drug_time]: {a6a298}Are Not Ready, Wait For %d Seconds.", coolDown);
			}
		}
		// тройка
		if (SF->getGame()->isKeyPressed(51)) {
			PEDSELF->Teleport(PEDSELF->GetPosition()->fX, PEDSELF->GetPosition()->fY, PEDSELF->GetPosition()->fZ);
		}
		// крафт дигла
		if (SF->getGame()->isKeyPressed(90)) {
			SF->getSAMP()->getPlayers()->localPlayerData->Say("/de 10");
		}
		if (SF->getGame()->isKeyPressed(50)) {
			SF->getSAMP()->getPlayers()->localPlayerData->Say("/ri 10");
		}
		

	}
}


bool PluginInit(HMODULE hModule) {
	SF = std::make_unique<SAMPFUNCS>();
	SF->initPlugin(&mainloop, hModule);
	return true;
}
