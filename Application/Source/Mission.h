#pragma once
#include <vector>
#include <string>
#include "MeshHandler.h"

enum MISSIONTYPE {
	//For configuration detection purposes, all missions must start with MISSION_
	MISSION_EXTINGUISH_FIRE,
	MISSION_ENTER_TIMEPORTAL,

	MISSION_VISIT_FOUNTAIN,
	MISSION_VISIT_RESTAURANT,
	MISSION_TALK_TO_NPC,
	//MISSION_FIND_GUNSHOP,
	//MISSION_CALL_RICHARD,

	////How these will work: When player interacts with the different interactions in main city scene, depending on the Interaction done,
	///* One of these will be unlocked. When one of these are unlocked, the final "Date sim"
	//options will be dependent on which one is unlocked.

	//(Check list of Completable Missions at end of interaction,
	//if Completed Missions contain any of these finale endings, add to queue the last interaction)

	//Uses the "Queue System" for Interaction Manager to add Interactions based on the "CheckPoints" in interaction reached.
	//When a mission is complete, Add to Interaction Queue some commands that come with the mission being complete*/
	//MISSION_FINALE_PEACEFUL,
	//MISSION_FINALE_REWARDING,
	//MISSION_FINALE_ANGERY,

	MISSION_COUNT,
	INVALID
};

//Had to separate from actual Mission Object, so that you can render in MissionInfo from txt file only from Load() in MissionManager
struct MissionInfo {
	std::string missionObjective;
	std::string missionCompletionMessage;
	GEOMETRY_TYPE missionPreviewImage;
	std::vector<MISSIONTYPE> blackListedPreReqMissions;
	std::vector<MISSIONTYPE> requiredPreReqMissions;
};

class Mission {
	MISSIONTYPE type;
	float progress;
	bool complete;
	MissionInfo missionInfo;

public:
	Mission(MISSIONTYPE, MissionInfo);
	~Mission();
	void addProgress(float progress);
	void setProgress(float progress);
	bool isCompleted();
	float getProgress();
	GEOMETRY_TYPE getPreviewImage();
	MISSIONTYPE getType();
	MissionInfo getMissionInfo();


};