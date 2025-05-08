#include "plugin.h"
#include <game_api.h>
#include <chrono>
#include <thread>
#include <fstream>

std::unique_ptr<SAMPFUNCS> SF;



class DrugTimer {
public:
	bool isReadyToUse = true, useAfterDeath = false, stop = false;
	int coolDown = 60;
	std::chrono::milliseconds noFloodMS = std::chrono::milliseconds(1000);
	
	void _stdcall countDown() {
		int len = coolDown;
		for (int i = 0; i < len; i++) {
			std::this_thread::sleep_for(noFloodMS);
			coolDown--;
			if (isReadyToUse) {
				coolDown = 60;
				return;
			}
		}
		coolDown = 60;
		isReadyToUse = true;
		SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(213, 240, 218), "{d5f0da}[drugTimer]: {25f54b}Drugs Are Ready{a6a298}.");
	}

	void NoFloodCheck(std::string& cmd) {
		bool nfldBool = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::ifstream file(SF->getSAMP()->getChat()->logFilePathChatLog);
		if (!file.is_open()) {
			SF->Log("[gTool]: File wasn't open");
			return;
		}
		std::deque <std::string> lines;
		std::string line;
		while (std::getline(file, line)) {
			lines.push_back(line);
			if (lines.size() > static_cast<size_t>(5)) {
				lines.pop_front();
			}
		}
		std::string toFindString = SF->getSAMP()->getPlayers()->localPlayerNickName + " употребил";
		for (std::string& x : lines) {
			SF->Log("[gToolDebug]: " + x);
			if (x.find(toFindString) != std::string::npos) {
				nfldBool = true;
			}
		}
		file.close();
		if (!nfldBool) {
			NoFloodByPass(cmd);
		}

	}
	void _stdcall NoFloodByPass(std::string cmdline) {
		std::this_thread::sleep_for(noFloodMS);
		SF->getSAMP()->getPlayers()->localPlayerData->Say((char*)cmdline.c_str());
	}

};


std::unique_ptr<DrugTimer> drugTimer = std::make_unique<DrugTimer>();

static void CALLBACK mainloop() {
	static bool initialized = false;
	if (!initialized)
	{
		if (GAME && GAME->GetSystemState() == eSystemState::GS_PLAYING_GAME && SF->getSAMP()->IsInitialized())
		{
			initialized = true;
			SF->Log("{d5f0da}[gTool]: {25f54b}loaded{d5f0da}. Press 'Z', 'L', '2', '3', '4' as cheatcode.");
			
		}
	}
	else {
		if (SF->getSAMP()->getInfo()->gameState == GAMESTATE_CONNECTED) {
			// драгтаймер
			if (SF->getGame()->isKeyPressed(88)) {
				if (drugTimer->isReadyToUse) {
					int hp = SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->hitpoints;
					int needToUse = (160 - hp) / 10 + 1;
					std::string cmdline = "/usedrugs " + std::to_string(needToUse);
					SF->getSAMP()->getPlayers()->localPlayerData->Say((char*)cmdline.data());
					std::thread([&] { drugTimer->NoFloodCheck(cmdline);}).detach();
					drugTimer->isReadyToUse = false;
					std::thread drugTimerThread([&] {drugTimer->countDown();});
					drugTimerThread.detach();
				}
				else {
					SF->getSAMP()->getChat()->AddChatMessage(-1, "{d5f0da}[drugTimer]: {a6a298}Are Not Ready, Wait For %d Seconds.", drugTimer->coolDown);
				}
			}
			int actorState = (SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->hitpoints);
			uint carState = (SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->state);
			if (actorState == 0) {
				drugTimer->isReadyToUse = true;
				
				if (drugTimer->useAfterDeath && !drugTimer->stop) {
					std::thread([&] { 
					drugTimer->NoFloodByPass("/usedrugs 16");
						}).detach();
					drugTimer->stop = true;
					// отсчёт 4 секунды на смерть
					std::thread([&] { std::this_thread::sleep_for(std::chrono::seconds(4)); drugTimer->stop = false;}).detach();
				}
			}
			if (SF->getGame()->isKeyPressed(52)) {
				if (drugTimer->useAfterDeath) {
					drugTimer->useAfterDeath = false;
					SF->getSAMP()->getChat()->AddChatMessage(-1, "{d5f0da}[drugTimer]: {a6a298}Auto Use After Death is {f70a0a}OFF{d5f0da}.");
				}
				else {
					drugTimer->useAfterDeath = true;
					SF->getSAMP()->getChat()->AddChatMessage(-1, "{d5f0da}[drugTimer]: {a6a298}Auto Use After Death is {25f54b}ON{d5f0da}.");
				}
			}
			// тройка
			else if (SF->getGame()->isKeyPressed(51)) {
				PEDSELF->Teleport(PEDSELF->GetPosition()->fX, PEDSELF->GetPosition()->fY, PEDSELF->GetPosition()->fZ);
			}
			// крафт дигла
			else if (SF->getGame()->isKeyPressed(90) && carState != 50) {
				SF->getSAMP()->getPlayers()->localPlayerData->Say("/de 10");

			}
			// /lock
			else if (SF->getGame()->isKeyPressed(76)) {
				SF->getSAMP()->getPlayers()->localPlayerData->Say("/lock");

			}
			// крафт рифлы
			else if (SF->getGame()->isKeyPressed(50) && carState != 50) {
				SF->getSAMP()->getPlayers()->localPlayerData->Say("/ri 10");
			}
			else if (SF->getGame()->isKeyPressed(66) && carState != 50) {
				SF->getSAMP()->getPlayers()->localPlayerData->Say("/sd 10");

			}
		}
		else {
			drugTimer->isReadyToUse = true;
		}

	}
}




bool PluginInit(HMODULE hModule) {
	SF = std::make_unique<SAMPFUNCS>();
	SF->initPlugin(&mainloop, hModule);
	return true;
}
