#pragma once
#include "Scene.h"
#include <iostream>
#include <vector>
#include "Weapon.h"
#include "Inventory.h"
#include "MissionManager.h"
#include "ButtonManager.h"
#include "UIManager.h"
#include "InteractionManager.h"

enum SCENES
{
	S_2051 = 0,
	S_2021,
	S_GUNSHOP,
	S_GARAGE,
	S_COUNT
};

//Interface

class Game
{
public:
	Game();
	~Game();
	
	static float toggleTimer;

	static SCENES activeScene;
	static std::vector<Scene*> SceneList;

	static MissionManager mManager;
	static InteractionManager iManager;
	static ButtonManager bManager;
	static UIManager uiManager;
	static Inventory inv;

	static void Init();
	static void Update(double dt);
	static void InteractionUpdate(double dt);
	static void ButtonUpdate(double dt);
	static void RenderUI();
	static void Render();
	static void Exit();

	static void addScene(Scene* scene);
	static void switchScene(static SCENES);
	static Scene* getActiveScene();
	static Scene* getSceneByName(std::string);
	static Scene* getScene();
};
