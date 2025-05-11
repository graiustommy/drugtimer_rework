#include "plugin.h"
#include <game_api.h>
#include <chrono>
#include <thread>
#include <fstream>

std::unique_ptr<SAMPFUNCS> SF;


class DrugTimer {
	bool boolAfterDeath = false, stop = false;
	int coolDown = 60;
	std::chrono::milliseconds noFloodMS = std::chrono::milliseconds(1000);
public:
	bool isReadyToUse = true;

	// проверка на использование
	bool NoFloodCheck() {
		bool nfldBool = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		std::ifstream file(SF->getSAMP()->getChat()->logFilePathChatLog);
		if (!file.is_open()) {
			SF->Log("[gTool]: File wasn't open");
			return true;
		}
		std::deque <std::string> lines;
		std::string line;
		while (std::getline(file, line)) {
			lines.push_back(line);
			if (lines.size() > static_cast<size_t>(5)) {
				lines.pop_front();
			}
		}
		std::string toFindString = SF->getSAMP()->getPlayers()->localPlayerNickName;
		for (std::string x : lines) {
			if (x.find(toFindString) != std::string::npos) {
				nfldBool = true;
			}
		}
		file.close();
		return nfldBool;
	}
	// использование после смерти
	void useAfterDeath() {
		int actorState = (SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->hitpoints);
		if (actorState == 0) {
			isReadyToUse = true;
			if (boolAfterDeath && !stop) {
				// использование нарко во время смерти спустя секкунду
				std::thread([=] {
					char* cmd = "/usedrugs 16";
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					SF->getSAMP()->getPlayers()->localPlayerData->Say(cmd);
					if (!NoFloodCheck()) {
						SF->getSAMP()->getPlayers()->localPlayerData->Say(cmd);
					}
					}).detach();
				stop = true;
				// отсчёт 4 секунды на смерть
				std::thread([&] {
					std::this_thread::sleep_for(std::chrono::seconds(4));
					stop = false;
					}).detach();
			}
		}
	}
	// отсчёт до 60 секунд
	void countDown() {
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
	// использование 
	void useDrugs() {
		if (isReadyToUse) {
			int hp = SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->hitpoints;
			int needToUse = (160 - hp) / 10 + 1;
			std::string commandLine = "/usedrugs ";
			std::string needToUseString = std::to_string(needToUse);
			commandLine += needToUseString;
			SF->getSAMP()->getPlayers()->localPlayerData->Say((char*)commandLine.data());
			std::ifstream file(SF->getSAMP()->getChat()->logFilePathChatLog);
			if (!file.is_open()) {
				SF->Log("[gTool]: File wasn't open");
				//return true;
			}
			std::deque <std::string> lines;
			std::string line;
			while (std::getline(file, line)) {
				lines.push_back(line);
				if (lines.size() > static_cast<size_t>(5)) {
					lines.pop_front();
				}
			}
			std::thread([=] {
				SF->Log("Nofloodcheck 1");
				if (!NoFloodCheck()) {
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					SF->Log("Nofloodcheck 2");
					SF->getSAMP()->getPlayers()->localPlayerData->Say((char*)commandLine.data());
				}
				}).detach();
			isReadyToUse = false;
			std::thread drugTimerThread([&] {countDown();});
			drugTimerThread.detach();
		}
		else {
			SF->getSAMP()->getChat()->AddChatMessage(-1, "{d5f0da}[drugTimer]: {a6a298}Are Not Ready, Wait For %d Seconds.", coolDown);
		}
	}

	// включить отключить после смерти
	void __stdcall turnOnOffAfterDeath() {
		if (boolAfterDeath) {
			boolAfterDeath = false;
			SF->getSAMP()->getChat()->AddChatMessage(-1, "{d5f0da}[drugTimer]: {a6a298}Auto Use After Death is {f70a0a}OFF{d5f0da}.");
		}
		else {
			boolAfterDeath = true;
			SF->getSAMP()->getChat()->AddChatMessage(-1, "{d5f0da}[drugTimer]: {a6a298}Auto Use After Death is {25f54b}ON{d5f0da}.");
		}
	}

};

std::unique_ptr<DrugTimer> drugTimer;

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
			uint carState = (SF->getSAMP()->getPlayers()->localPlayerData->sampActor->gtaPed->state);
			
			if (SF->getGame()->isKeyPressed(88)) {
				drugTimer->useDrugs();
			}
			if (SF->getGame()->isKeyPressed(52)) {
				drugTimer->turnOnOffAfterDeath();
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
			drugTimer->useAfterDeath();
		}
		else {
			drugTimer->isReadyToUse = true;
		}
	}
	
}




bool PluginInit(HMODULE hModule) {
	drugTimer = std::make_unique<DrugTimer>();
	SF = std::make_unique<SAMPFUNCS>();
	SF->initPlugin(&mainloop, hModule);
	return true;
}
