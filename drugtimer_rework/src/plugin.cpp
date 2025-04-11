#include "plugin.h"
#include <game_api.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>

std::unique_ptr<SAMPFUNCS> SF;

bool NoFloodCheck() {
	std::ifstream file(SF->getSAMP()->getChat()->logFilePathChatLog);
	if (!file.is_open()) {
		return false;
	}
	std::deque <std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(line.substr(11, line.size() - 12));
		if (lines.size() > static_cast<size_t>(3)) {
			lines.pop_front();
		}
	}
	for (auto& x : lines) {
		if (x == " Не флуди!") {
			file.close();
			return false;
		}
	}
	file.close();
	return true;

}



std::chrono::milliseconds noFloodMS = std::chrono::milliseconds(888);
static bool isReadyToUse = true, useAfterDeath = false;
static int coolDown = 60;
static bool UAD = false;
static bool stop = false;

void _stdcall NoFloodByPass(std::string cmdline) {
	std::this_thread::sleep_for(noFloodMS);
	SF->getSAMP()->getPlayers()->localPlayerData->Say((char*)cmdline.data());
}


void _stdcall countDown() {
	int len = coolDown;
	for (int i = 0; i < len; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		coolDown--;
		if (isReadyToUse) {
			coolDown = 60;
			return;
		}
	}
	coolDown = 60;
	isReadyToUse = true;
	SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(213, 240, 218), "{d5f0da}[drugTimer]: {25f54b}Drugs Are Ready.");
}

bool  checkIfWasNotUsed(int hpAfter) {
	if (hpAfter < 150) {
		return true;
	}
	return false;
}

static void CALLBACK mainloop() {
	static bool initialized = false;
	if (!initialized)
	{
		if (GAME && GAME->GetSystemState() == eSystemState::GS_PLAYING_GAME && SF->getSAMP()->IsInitialized())
		{
			initialized = true;
			SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(166, 162, 152), "{d5f0da}[drugTimer]: {25f54b}loaded{d5f0da}. Press 'Z', 'L', '2', '3', '4' as cheatcode.");
		}
	}
	else {
		// драгтаймер
		if (SF->getGame()->isKeyPressed(88)) {
			if (isReadyToUse) {
				int hp = SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->hitpoints;
				int needToUse = (160 - hp) / 10 + 1;
				std::string cmdline = "/usedrugs " + std::to_string(needToUse);
				SF->getSAMP()->getPlayers()->localPlayerData->Say((char*)cmdline.data());
				if (!NoFloodCheck()) {
					std::thread DoAgain(NoFloodByPass, cmdline);
					DoAgain.detach();
				}
				isReadyToUse = false;
				std::thread DrugTimer(countDown);
				DrugTimer.detach();
			}
			else {
				SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(166, 162, 152), "{d5f0da}[drugTimer]: {a6a298}Are Not Ready, Wait For %d Seconds.", coolDown);
			}
		}

		if (SF->getSAMP()->getInfo()->gameState == GAMESTATE_CONNECTED) {
			int actorState = SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->hitpoints;
			if (actorState == 0) {
				isReadyToUse = true;
				if (UAD && !stop) {
					SF->getSAMP()->getPlayers()->localPlayerData->Say("/usedrugs 16");
					if (!NoFloodCheck()) {

						std::thread DoAgain(NoFloodByPass, "/usedrugs 16");
						DoAgain.detach();
					}
					stop = true;
					// отсчёт 4 секунды на смерть
					std::thread([] { std::this_thread::sleep_for(std::chrono::seconds(4)); stop = false;}).detach();
				}
			}
			if (SF->getGame()->isKeyPressed(52)) {
				if (UAD) {
					UAD = false;
					SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(166, 162, 152), "{d5f0da}[drugTimer]: Auto Use After Death is {f70a0a}OFF{d5f0da}.");
				}
				else {
					UAD = true;
					SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(166, 162, 152), "{d5f0da}[drugTimer]: Auto Use After Death is {25f54b}ON{d5f0da}.");
				}
			}
			// тройка
			if (SF->getGame()->isKeyPressed(51)) {
				PEDSELF->Teleport(PEDSELF->GetPosition()->fX, PEDSELF->GetPosition()->fY, PEDSELF->GetPosition()->fZ);
			}
			// крафт дигла
			int carState = SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->state;
			if (SF->getGame()->isKeyPressed(90) && carState != 50) {
				SF->getSAMP()->getPlayers()->localPlayerData->Say("/de 10");
			}
			// /lock
			if (SF->getGame()->isKeyPressed(76)) {
				SF->getSAMP()->getPlayers()->localPlayerData->Say("/lock");
			}
			// крафт рифлы
			if (SF->getGame()->isKeyPressed(50) && carState != 50) {
				SF->getSAMP()->getPlayers()->localPlayerData->Say("/ri 10");
			}
			if (SF->getGame()->isKeyPressed(66) && carState != 50) {
				SF->getSAMP()->getPlayers()->localPlayerData->Say("/sd 10");
			}

		}
		else {
			isReadyToUse = true;
		}

	}
}





bool PluginInit(HMODULE hModule) {
	SF = std::make_unique<SAMPFUNCS>();
	SF->initPlugin(&mainloop, hModule);
	return true;
}
