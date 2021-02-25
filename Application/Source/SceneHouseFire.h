#ifndef SCENE_HOUSEFIRE_H
#define SCENE_HOUSEFIRE_H

#include <map>

#include "Scene.h"
#include "Camera.h"
#include "Game.h"
#include "Inventory.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Interactions.h"

#include "EntityManager.h"
#include "MeshHandler.h"
#include "ButtonManager.h"
#include "AudioHandler.h"
#include "InteractionManager.h"

//Entities
#include "NPC.h"
#include "CustomEntity.h"
#include "WorldObject.h"

/*
* Scene Setup
* In Init,
Change Scene Name Accordingly
Init the Player Entity
*/

class SceneHouseFire : public Scene
{
	enum INTERACTION_TYPE {
		TEST,
		INTERACTION_COUNT,
	};

private:
	void RenderUI(); //Handles the next interaction (May end interaction if there is no more to go through0
	void SpawnNPCs(Vector3 v3Tmin, Vector3 v3Tmax, NPCTYPE geoType);

	Light light[3];
	ButtonManager bManager;

	bool hitboxEnable;
	float fps;

	float toggleTimer = 0; //Used for toggling between items

	void RenderSkybox();
	void RenderSceneMeshes();

	//topdown cam map
	bool camMap;

	float CameraBobber = 0;
	double bobTime = 0;

public:
	SceneHouseFire();
	~SceneHouseFire();

	bool eHeld = false;
	Camera camera;
	Camera camera2;

	EntityManager eManager;

	//Game Variables
	Player* player;

	virtual void Init();
	virtual void Update(double dt);
	virtual void InitLights();
	virtual void CollisionHandler(double dt);
	virtual void MissionCompleteListener(double dt);
	virtual void TopDownMapUpdate(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif