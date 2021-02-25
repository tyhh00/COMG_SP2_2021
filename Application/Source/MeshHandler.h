#pragma once
#include "Mesh.h"
#include "MeshBuilder.h"
#include "EntityDataHandler.h"
#include <unordered_map>

enum GEOMETRY_TYPE {
	//General
	EMPTY = 0,

	GEO_AXES,
	GEO_LIGHTBALL,
	GEO_TEXT,
	GEO_TEXT_CALIBRI,
	GEO_TEXT_SUPERMARIO,

	//For OBJs
	GEO_BUILDING_1,
	GEO_BUILDING_2,
	GEO_BUILDING_3,
	GEO_BUILDING_4,
	GEO_BUILDING_5,
	GEO_BUILDING_6,
	GEO_BUILDING_7,
	
	GEO_SUBURBAN_1,
	GEO_SUBURBAN_2,
	GEO_SUBURBAN_3,
	GEO_SUBURBAN_4,
	GEO_SUBURBAN_5,

	GEO_BOSS_BUILDING,
	GEO_FOUNTAIN,
	GEO_NPC,
	GEO_ROAD_TILE,
	GEO_ROAD_CROSSING,
	GEO_ROAD_INTERSECTION_PATH,
	GEO_ROAD_ROUNDABOUT,
	GEO_ROAD_BEND,
	GEO_ROAD,
	GEO_ROAD_END,
	GEO_ROAD_STREET_LAMP,
	GEO_ROAD_BLOCK,
	GEO_TREE,






	GEO_SEDAN,
	GEO_SEDAN_SPORTS,
	GEO_RACER,
	GEO_HATCHBACKSPORTS,
	GEO_TRACTOR,
	GEO_TRUCK,
	GEO_VAN,



	//Platform
	GEO_QUAD,
	GEO_BOOSTMETER,
	GEO_SPHERE,


	//Misc

	//Skybox
	GEO_SKY_LEFT,
	GEO_SKY_RIGHT,
	GEO_SKY_TOP,
	GEO_SKY_BOTTOM,
	GEO_SKY_FRONT,
	GEO_SKY_BACK,

	// Gun Shop
	GEO_GUNSHOP_LEFT,
	GEO_GUNSHOP_RIGHT,
	GEO_GUNSHOP_TOP,
	GEO_GUNSHOP_BOTTOM,
	GEO_GUNSHOP_FRONT,
	GEO_GUNSHOP_BACK,
	GEO_WIREMESH,
	GEO_COUNTER,

	GEO_DRUGMAN,

	GEO_MACHINE_GUN,
	GEO_MACHINE_GUN_LAUNCHER,
	GEO_PISTOL,
	GEO_PISTOL_S,
	GEO_RLM,
	GEO_SHOTGUN,
	GEO_SHOTGUN_S,
	GEO_SNIPER,
	GEO_UZI,
	GEO_UZI_L,
	GEO_UZI_L_S,
	GEO_UZI_S,

	// Time Portal
	GEO_TIMEPORTAL_WALL,
	GEO_TIMEPORTAL_DOOR,

	//Title Screen
	TITLE_BG,
	PLAY_BUTTON,

	//General UI
	UI_BLACK,
	UI_BLUE,
	UI_WINDOW,
	UI_EMPTY,
	UI_BLANK,

	//UI Items
	UI_EGGPLANT,
	UI_CORN,
	UI_BURGER,

	//UI Garage Cars
	UI_SEDAN,
	UI_SUV,

	//UI Weapons
	UI_PISTOL,
	UI_SILENCER,

	//Garage
	GARAGE_WALL,
	GARAGE_DOOR,
	CONCRETE_WALL,
	GARAGE_FLOOR,

	//Textured Quads
	GEO_MISSION_COMPLETE,
	GEO_MISSION_TASK,

	TAILSS,
	//For non-OBJs
	UI_TEXTBOX,
	UI_CHOICE,
	UI_CROSSHAIR,


	//END
	NUM_GEOMETRY
};

class MeshHandler
{
	static std::unordered_map<std::string, GEOMETRY_TYPE> const geoTypeTable;
	static Mesh* meshList[NUM_GEOMETRY];
	static bool isLoaded;

public:
	static bool loadMeshes();
	static bool unloadMeshes();
	static GEOMETRY_TYPE getGeoTypeByName(std::string geoType);
	static Mesh* getMesh(GEOMETRY_TYPE);
};
