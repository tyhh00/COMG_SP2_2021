#include "InteractionManager.h"
#include "Game.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "Debug.h"
#include "Application.h"


InteractionManager::InteractionManager() : latestInteractionSwitch(0), canInteractWithSomething(false), interactionElapsed(0) { 
	for (int i = 0; i < INTERACTION_COUNT; ++i) {
		this->completedInteractionsCount[i] = 0;
	}
}

InteractionManager::~InteractionManager()
{
	// do nothing
}

/******************************************************************************/
/*!
\brief
Returns the interaction queue.
*/
/******************************************************************************/
InteractionQueue& InteractionManager::getQueue()
{
	return this->interactionQueue;
}

/******************************************************************************/
/*!
\brief
Runs a command in the format of '/cmd' to carry out codes that require their own functions.
*/
/******************************************************************************/
bool InteractionManager::runCommand(Command cmd) {
	std::vector<std::string> splitVar;
	split(cmd.command, ' ', splitVar);

	if (splitVar.size() == 1) {

		if (splitVar.at(0) == "/endinteraction") {
			EndInteraction();
			return true;
		}
	}
	else if (splitVar.size() >= 2) {
		if (splitVar.at(0) == "/givecoin") {
			//this->addCoins(stoi(splitVar.at(1)));
			return true;
		}
	}

	return true;
}

/******************************************************************************/
/*!
\brief
Pushes interactions from the Interactions map into the queue to be shown on the UI.
*/
/******************************************************************************/
bool InteractionManager::loadInteraction(std::string key) {
	Application::setCursorEnabled(true);
	try {
		interactionQueue.pushInteraction(Interactions[key]);
		return true;
	} catch (...) {
		DEBUG_MSG("Interaction '" << key << "' not found");
	}
	return false;
}

/******************************************************************************/
/*!
\brief
Splits a string based on delimiters in between.
*/
/******************************************************************************/
void InteractionManager::split(std::string txt, char delim, std::vector<std::string>& out) {
	std::istringstream iss(txt);
	std::string item;
	while (std::getline(iss, item, delim)) {
		out.push_back(item);
	}
}

/******************************************************************************/
/*!
\brief
Initialises ALL interactions loaded from a file into the map of interactions.
*/
/******************************************************************************/
bool InteractionManager::initInteractions(const char* filePath)
{
	std::ifstream fileStream(filePath, std::ios::binary);
	if (!fileStream.is_open()) {
		DEBUG_MSG("Impossible to open " << filePath << ". Are you in the right directory ?\n");
		return false;
	}

	Command* command = nullptr;
	Interaction* interaction = nullptr;
	while (!fileStream.eof()) {
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("INTERACTION: ", buf, 13) == 0) {
			char interaction_ID[256];
			strcpy_s(interaction_ID, buf + 13);
			if (interaction_ID[strlen(interaction_ID) - 1] == '\r')
				interaction_ID[strlen(interaction_ID) - 1] = '\0';

			interaction = new Interaction();
			Interactions.insert(std::pair<std::string, Interaction*>(std::string(interaction_ID), interaction));
		}
		else if (strncmp("CHOICE # ", buf, 9) == 0) {
			char interaction_ID[256];
			strcpy_s(interaction_ID, buf + 9);
			if (interaction_ID[strlen(interaction_ID) - 1] == '\r')
				interaction_ID[strlen(interaction_ID) - 1] = '\0';

			this->Interactions[interaction_ID]->interactionChoices.push_back(new Interaction());
			interaction = Interactions[interaction_ID]->interactionChoices.back();
		}
		else if ((strncmp("- MSG: ", buf, 7) == 0)) {
			if (interaction != nullptr) {
				char msg[256];
				strcpy_s(msg, buf + 7);
				if (msg[strlen(msg) - 1] == '\r')
					msg[strlen(msg) - 1] = '\0';
				interaction->interactionText = msg;
			}
		}
		else if ((strncmp("- precmd: ", buf, 10) == 0)) {
			if (interaction != nullptr) {
				char cmd[256];
				strcpy_s(cmd, buf + 10);
				if (cmd[strlen(cmd) - 1] == '\r')
					cmd[strlen(cmd) - 1] = '\0';

				
				std::string cmdString = cmd;
				std::vector<std::string> splitVar;
				split(cmdString, ';', splitVar);
				if (splitVar.at(0)[0] == '/') {
					command = new Command(splitVar.at(0));
				}
				else { 
					for (auto& entry : Game::SceneList) {
						if (entry->getName() == splitVar.at(0)) {
							command = new Command(splitVar.at(1), entry);
							break;
						}
					}
				}

				interaction->preInteractionCMD.push_back(command);
			}
		}
		else if ((strncmp("- postcmd: ", buf, 10) == 0)) {
			if (interaction != nullptr) {
				char cmd[256];
				strcpy_s(cmd, buf + 10);
				if (cmd[strlen(cmd) - 1] == '\r')
					cmd[strlen(cmd) - 1] = '\0';


				std::string cmdString = cmd;
				std::vector<std::string> splitVar;
				split(cmdString, ';', splitVar);
				if (splitVar.at(0)[0] == '/') {
					command = new Command(splitVar.at(0));
				}
				else {
					for (auto& entry : Game::SceneList) {
						if (entry->getName() == splitVar.at(0)) {
							new Command(splitVar.at(1), entry);
							break;
						}
					}
				}

				interaction->postInteractionCMD.push_back(command);
			}
		}
	}
	fileStream.close(); // close file

	return true;
		
}

/******************************************************************************/
/*!
\brief
Ends the current interaction chain.
*/
/******************************************************************************/
void InteractionManager::EndInteraction()
{
	completedInteractionsCount[currentInteractionType]++;

	// isInteracting = false;
	//for (auto& entry : interactionQueue.Top()->postInteractionCMD) {
	//	runCommand(*entry);
	//}
	interactionElapsed = 0;
	currentInteractionType = INTERACTION_COUNT;
	Game::uiManager.setCurrentUI(UI_GENERAL);
	Application::setCursorEnabled(false);
}

/******************************************************************************/
/*!
\brief
Runs the pre and post commands of the current Interaction and pop it from the queue, loading in the next Interaction Message
*/
/******************************************************************************/
void InteractionManager::nextInteraction()
{

	for (auto& entry : interactionQueue.Top()->postInteractionCMD) {
		runCommand(*entry);
	}

	interactionQueue.popInteraction();


	if (isInteracting()) {
		for (auto& entry : interactionQueue.Top()->preInteractionCMD) {
			runCommand(*entry);
		}
	}
	else {
		EndInteraction();
	}
}

/******************************************************************************/
/*!
\brief
Returns true if the queue is not empty.
*/
/******************************************************************************/
bool InteractionManager::isInteracting() {
	return (!(interactionQueue.getQueue().size() == 0));
}

/******************************************************************************/
/*!
\brief
Returns true if the time elapsed is greater than the cooldown.
*/
/******************************************************************************/
bool InteractionManager::passedInteractionCooldown()
{
	const float INTERACTION_COOLDOWN = 0.5f;
	if (latestInteractionSwitch + INTERACTION_COOLDOWN < this->elapsed) {
		return true;
	}
	return false;
}

/******************************************************************************/
/*!
\brief
Updates the elapsed time
*/
/******************************************************************************/
void InteractionManager::Update(double dt) {
	if (isInteracting()) {
		this->interactionElapsed += dt;
	}
}
