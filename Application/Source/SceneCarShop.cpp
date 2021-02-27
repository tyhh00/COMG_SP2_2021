#include "SceneCarShop.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "LoadTGA.h"
#include <Windows.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "Player.h"
#include "shader.hpp"
#include "Utility.h"
#include "Car.h"
#include "AudioHandler.h"
#include "InteractionManager.h"
#include "Scene2021.h"

SceneCarShop::SceneCarShop() : 
	eManager(this)
{
	//Scene
	sceneName = "OfficeScene";

	//Game
	fps = 0;
	lightEnable = true;
	hitboxEnable = false;
	showNotifUntil = 0.0;

	//Interaction
	interactionElapsed = 0.0;
	for (int i = 0; i < INTERACTION_COUNT; i++) {
		this->completedInteractionsCount[i] = 0;
	}
	canInteractWithSomething = false;
	isInteracting = false;
}

SceneCarShop::~SceneCarShop()
{

}

void SceneCarShop::Init() 
{
	// Init VBO here
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");

	//week 7
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	//week 9
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	//Week 14
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	glUseProgram(m_programID);

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 128.0f / 72.0f, 0.1f, 867.f);
	projectionStack.LoadMatrix(projection);

	MeshHandler::loadMeshes();

	//Mesh* coinMesh;
	//Entity* newCoin;

	Entity* Hitbox = new CustomEntity(this, new Box(Vector3(-1, 0, 5), Vector3(1, 3.75, -5)), "hitbox");
	Hitbox->getEntityData()->Scale = Vector3(2.5, 2, 2.5);
	Hitbox->getEntityData()->Translate = Vector3(-22.5, -3, -10);
	eManager.spawnWorldEntity(Hitbox);

	Hitbox = new CustomEntity(this, new Box(Vector3(-3.25, 0, 1.5), Vector3(3.25, 3.75, -1.5)), "hitbox");
	Hitbox->getEntityData()->Translate = Vector3(-13.15, -3, -21.25);
	Hitbox->getEntityData()->Scale = Vector3(2.5, 2, 2.5);
	eManager.spawnWorldEntity(Hitbox);

	Hitbox = new CustomEntity(this, new Box(Vector3(-0.3, 0, 0.3), Vector3(0.3, 3.75, -0.3)), "hitbox");
	Hitbox->getEntityData()->Translate = Vector3(-5.375, -3, 2.125);
	Hitbox->getEntityData()->Scale = Vector3(2.5, 2, 2.5);
	eManager.spawnWorldEntity(Hitbox);

	Entity* car = new WorldObject(this, GEO_SEDAN_SPORTS, "sedansports");
	car->getEntityData()->Translate = Vector3(3, 0, -17);
	car->getEntityData()->SetScale(2.5, 2.5, 2.5);
	eManager.spawnWorldEntity(car);

	car = new WorldObject(this, GEO_HATCH_BACK_SPORTS, "hatchback");
	car->getEntityData()->Translate = Vector3(17, 0, -15);
	car->getEntityData()->Rotation = Vector3(0, -45, 0);
	car->getEntityData()->SetScale(2.5, 2.5, 2.5);
	eManager.spawnWorldEntity(car);

	car = new WorldObject(this, GEO_TRACTOR_SHOVEL, "tractor");
	car->getEntityData()->Translate = Vector3(17, 0, 0);
	car->getEntityData()->Rotation = Vector3(0, -90, 0);
	car->getEntityData()->SetScale(2.5, 2.5, 2.5);
	eManager.spawnWorldEntity(car);

	car = new WorldObject(this, GEO_TRUCK, "truck");
	car->getEntityData()->Translate = Vector3(17, 0, 15);
	car->getEntityData()->Rotation = Vector3(0, -135, 0);
	car->getEntityData()->SetScale(2.5, 2.5, 2.5);
	eManager.spawnWorldEntity(car);

	car = new WorldObject(this, GEO_VAN, "van");
	car->getEntityData()->Translate = Vector3(3, 0, 17);
	car->getEntityData()->Rotation = Vector3(0, -180, 0);
	car->getEntityData()->SetScale(2.5, 2.5, 2.5);
	eManager.spawnWorldEntity(car);

	car = new WorldObject(this, GEO_RACER, "racer");
	car->getEntityData()->Translate = Vector3(-12, 0, 17);
	car->getEntityData()->Rotation = Vector3(0, -225, 0);
	car->getEntityData()->SetScale(2.5, 2.5, 2.5);
	eManager.spawnWorldEntity(car);

	Entity* door = new WorldObject(this, GEO_DOOR, "door");
	door->getEntityData()->Translate = Vector3(-14, 2.25, -17.5);
	// door->getEntityData()->Rotation = Vector3(0, 90, 0);
	door->getEntityData()->Scale = Vector3(2, 2, 2);
	eManager.spawnWorldEntity(door);

	/*Entity* car = new Car(SEDAN, this, "car");
	car->getEntityData()->SetTransform(0, 0.25, 20);
	car->getEntityData()->SetRotate(0, 0, 0);
	car->getEntityData()->SetScale(2.5, 2.5, 2.5);
	eManager.spawnMovingEntity(car);

	Entity* car2 = new Car(SEDAN_SPORTS, this, "car");
	car2->getEntityData()->SetTransform(5, 0.25, 20);
	car2->getEntityData()->SetRotate(0, 0, 0);
	car2->getEntityData()->SetScale(2.5, 2.5, 2.5);
	eManager.spawnMovingEntity(car2);

	Entity* car3 = new Car(RACER, this, "car");
	car3->getEntityData()->SetTransform(-5, 0.25, 20);
	car3->getEntityData()->SetRotate(0, 0, 0);
	car3->getEntityData()->SetScale(2.5, 2.5, 2.5);
	eManager.spawnMovingEntity(car3);*/

	SpawnWalls();

	//initCollidables(Vector3(24.0f, 0.0f, -49.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARREL);

	//for (int i = 0; i < 10; i++)
	//{
	//	initCollidables(Vector3(24.0f, 0.0f, (1.8f - (i * 1.8f))), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARREL);
	//}
	//for (int i = 0; i < 9; i++)
	//{
	//	if (i < 8)
	//		initCollidables(Vector3(24.0f, 2.5f, (0.0f - (i * 1.8f))), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARREL);
	//	initCollidables(Vector3(22.2f, 0.0f, (0.9f - (i * 1.8f))), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARREL);
	//}
	//
	//for (int i = 0; i < 2; i++)
	//{
	//	initCollidables(Vector3(-22.5f, 0.0f, (47.0f - (i * 5.5f))), Vector3(0.0f, 90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARRELS);
	//}
	//
	//initCollidables(Vector3(22.5f, 0.0f, 47.0f), Vector3(0.0f, 90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARRELS_RAIL);

	//initCollidables(Vector3(19.0f, 0.0f, -44.0f), Vector3(0.0f, -45.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), SNOWMAN_FANCY);

	//for (int i = 0; i < 3; i++)
	//{
	//	//1st floor
	//	if (i > 0)
	//		initCollidables(Vector3(-22.0f + (i * 5.0f), 0.0f, -47.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);
	//	initCollidables(Vector3(-22.0f, 0.0f, -47.0f + (i * 5.0f)), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);
	//	if (i == 1)
	//		initCollidables(Vector3(-22.0f + (i * 5.0f), 0.0f, -47.0f + (i * 5.0f)), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);
	//}

	//for (int i = 0; i < 2; i++)
	//{
	//	//2nd floor
	//	if (i > 0)
	//		initCollidables(Vector3(-22.0f + (i * 5.0f), 5.0f, -47.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);
	//	initCollidables(Vector3(-22.0f, 5.0f, -47.0f + (i * 5.0f)), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);
	//}

	//initCollidables(Vector3(-22.0f, 0.0f, -47.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);
	//initCollidables(Vector3(-17.0f, 0.0f, -47.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);
	//initCollidables(Vector3(-22.0f, 5.0f, -47.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);

	//Barrel increment of 1.8f between each one
	//initCollidables(Vector3(24.0f, 0.0f, 1.8f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARREL);
	//initCollidables(Vector3(24.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARREL);
	//initCollidables(Vector3(24.0f, 2.5f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARREL);

	//Barrels increment of 5.0f between each one
	//5.5f when rotated
	//initCollidables(Vector3(-22.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARRELS);
	//initCollidables(Vector3(-22.0f, 0.0f, 5.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARRELS);
	//initCollidables(Vector3(-22.5f, 0.0f, (47.0f - (i * 5.5f))), Vector3(0.0f, 90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), BARRELS);

	//Crate increment of 5.0f between each one
	//initCollidables(Vector3(-22.0f, 0.0f, -47.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);
	//initCollidables(Vector3(-17.0f, 0.0f, -47.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);
	//initCollidables(Vector3(-22.0f, 5.0f, -47.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CRATE);

	//Vector3 uniformWallScale = Vector3(100.0f, 100.0f, 100.0f);
	//Vector3 longWallScale = Vector3(200.0f, 200.0f, 200.0f);
	//initCollidables(Vector3(0.0f, 50.0f, 0.0f), Vector3(90.0f, 0.0f, 0.0f), longWallScale, GARAGE_WALL); //top
	//initCollidables(Vector3(50.0f, 0.0f, 0.0f), Vector3(0.0f, -90.0f, 0.0f), longWallScale, GARAGE_WALL); //x-axis
	//initCollidables(Vector3(-50.0f, 0.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f), longWallScale, GARAGE_WALL);
	//initCollidables(Vector3(0.0f, 0.0f, 100.0f), Vector3(0.0f, 180.0f, 0.0f), uniformWallScale, GARAGE_WALL); // z-axis
	//initCollidables(Vector3(0.0f, 0.0f, -100.0f), Vector3(0.0f, 0.0f, 0.0f), uniformWallScale, GARAGE_WALL);
	//initCollidables(Vector3(0.0f, 22.0f, 99.0f), Vector3(180.0f, 0.0f, 0.0f), Vector3(80.0f, 44.0f, 100.0f), GARAGE_DOOR); //garage door

	//Camera init(starting pos, where it looks at, up
	player = new Player(this, Vector3(0, 0, 0), "player");
	camera.playerPtr = player;
	eManager.spawnMovingEntity(player);

	camera.Init(Vector3(player->getEntityData()->Translate.x, player->getEntityData()->Translate.y + 2, player->getEntityData()->Translate.z),
				Vector3(player->getEntityData()->Translate.x, player->getEntityData()->Translate.y + 2, player->getEntityData()->Translate.z - 1),
				Vector3(0, 1, 0));

	camera2.Init(Vector3(player->getEntityData()->Translate.x, 150, player->getEntityData()->Translate.z),
		Vector3(0, -50, -1),
		Vector3(0, 1, 0));

	//Light init
	InitLights();

	//Practical 10a
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SceneCarShop::Update(double dt)
{
	light[0].position.set(player->getEntityData()->Translate.x, 450, player->getEntityData()->Translate.z);
	light[1].position.set(player->getEntityData()->Translate.x, player->getEntityData()->Translate.y + 2, player->getEntityData()->Translate.z);
	light[2].position.set(player->getEntityData()->Translate.x, player->getEntityData()->Translate.y + 2, player->getEntityData()->Translate.z);
	//light[0].position.set(0, 5, 0);
	//light[1].position.set(0, 5, 0);
	//light[2].position.set(0, 5, 0);

	bool ePressed = Application::IsKeyPressed('E');
	bool pPressed = Application::IsKeyPressed('P');
	bool tPressed = Application::IsKeyPressed('T');

	if (GetAsyncKeyState('1') & 0x8001) {
		glEnable(GL_CULL_FACE);
	}
	else if (GetAsyncKeyState('2') & 0x8001) {
		glDisable(GL_CULL_FACE);
	}

	if (Application::IsKeyPressed('9')) {
		hitboxEnable = !hitboxEnable;
	}
	if (Application::IsKeyPressed('0')) {
		lightEnable = !lightEnable;
	}

	if (!Game::iManager.isInteracting()) {
		//Keys that are used inside checks (Not reliant detection if checking for pressed inside conditions etc)
		CollisionHandler(dt);

		Vector3 pLoc = player->getEntityData()->Translate;
		Vector3 oldLoc = Vector3(pLoc);

		//Requires Implementation of Velocity by Joash
		float playerSpeed = 15.0;
		if (!((Player*)player)->isDriving()) {
			Vector3 view = (camera.target - camera.position).Normalized();
			if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D')) {
				camera.position.y += CameraBobber;
			}

			if (Application::IsKeyPressed('W')) {

				if (Application::IsKeyPressed(VK_LSHIFT) && Game::inv.getActiveWeapon() == nullptr) {
					playerSpeed = 25.f;
				}

				pLoc += view * (float)dt * playerSpeed;

			}
			if (Application::IsKeyPressed('A')) {
				Vector3 right = view.Cross(camera.up);
				right.y = 0;
				right.Normalize();
				Vector3 up = right.Cross(view).Normalized();
				pLoc -= right * (float)dt * playerSpeed;
			}

			if (Application::IsKeyPressed('S')) {
				pLoc -= view * (float)dt * playerSpeed;
			}

			if (Application::IsKeyPressed('D')) {
				Vector3 right = view.Cross(camera.up);
				right.y = 0;
				right.Normalize();
				Vector3 up = right.Cross(view).Normalized();
				pLoc += right * (float)dt * playerSpeed;
			}
			// SCENE WORLD BOUNDARIES
			pLoc.x = Math::Clamp(pLoc.x, -24.f, 24.f);
			pLoc.z = Math::Clamp(pLoc.z, -24.f, 24.f);

			// START MOVEMENT, TRIGGERED NEXT FRAME IF MOVEMENT NOT CANCELLED
			player->getEntityData()->Translate.x = pLoc.x;
			// Skip y since we want level ground
			player->getEntityData()->Translate.z = pLoc.z;

			bobTime += dt;
			CameraBobber = 0.002 * sin(bobTime * playerSpeed);
		}

		if (player->isDriving()) {
			player->getCar()->Drive(dt);
			camera.position.x = Math::Clamp(camera.position.x, -24.f, 24.f);
			camera.position.z = Math::Clamp(camera.position.z, -49.f, 48.f);
		}

		Vector3 view = (camera.target - camera.position).Normalized();
		Game::inv.getActiveWeapon()->Update(this, &this->eManager, player->getEntityData()->Translate, view, dt);
	}
}

void SceneCarShop::InitLights()
{
	light[0].type = Light::LIGHT_POINT;
	light[0].position.set(0, 450, 0);
	light[0].color.set(1, 1, 0.85f);
	light[0].power = 200;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 1.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_SPOT;
	light[1].position.set(0, 0, 0);
	light[1].color.set(1, 0, 1);
	light[1].power = 0.f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	light[2].type = Light::LIGHT_POINT;
	light[2].position.set(0, 0, 0);
	light[2].color.set(0.8, 1, 1);
	light[2].power = 0.5f;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(45));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 2.f;
	light[2].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	//week6
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	//week7
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	//Week 7 - Code to change number of lights
	glUniform1i(m_parameters[U_NUMLIGHTS], 3);
}

void SceneCarShop::MissionCompleteListener(double dt) {
	//MISSION HANDLING EXAMPLES
	//for (auto& entry : Game::mManager.getCompletableMissions()) {
	//	DEBUG_MSG("Completable Mission EnumID: " << entry);
	//}
	//if (Application::IsKeyPressed('V')) {
	//	Game::mManager.addProgress(MISSIONTYPE::MISSION_EXTINGUISH_FIRE, 30.0);
	//}
	std::vector<Mission*> justCompletedMissions = Game::mManager.getJustCompletedMissions();
	for (auto& entry : justCompletedMissions) {
		//If check for type of mission, e.g. if mission is extinguish fire, add balance.
	}
}

void SceneCarShop::CollisionHandler(double dt) {
	if (Application::IsKeyReleased('E')) eHeld = false;
	bool ePressed = Application::IsKeyPressed('E');
	bool pPressed = Application::IsKeyPressed('P');
	bool tPressed = Application::IsKeyPressed('T');

	bool foundInteractionZone = false;

	std::vector<CollidedWith*> collided = eManager.preCollisionUpdate();

	//Entity Collision Handling
	for (auto& entry : eManager.getEntities()) {
		if (entry->getType() == ENTITYTYPE::BULLET) {
			((Bullet*)entry)->Move(dt);
			if (((Bullet*)entry)->getTimer() > 5) {
				entry->setDead(true);
			}
		}

		if (entry->getType() == ENTITYTYPE::WORLDOBJ) {
			if (entry->getName().find("hitbox") != std::string::npos) {}
			else {
				if ((entry->getEntityData()->Translate - player->getEntityData()->Translate).Magnitude() < 5) {
					Game::uiManager.setUIactive(UI_E_TO_INTERACT);
					if (Application::IsKeyPressed('E')) {
						if (entry->getName().find("door") != std::string::npos) {
							ISound* door = AudioHandler::getEngine()->play3D(
								AudioHandler::getSoundSource(DOOR),
								AudioHandler::to_vec3df(Vector3(0, 0, 0)),
								LOOPED::NOLOOP);
							Game::switchScene(S_2021);
						}
						if (entry->getName().find("sedansports") != std::string::npos) {
							Game::iManager.loadInteraction("buy sedan sports");
						}
						if (entry->getName().find("hatchback") != std::string::npos) {
							Game::iManager.loadInteraction("buy hatchback");
						}
						if (entry->getName().find("tractor") != std::string::npos) {
							Game::iManager.loadInteraction("buy tractor");
						}
						if (entry->getName().find("truck") != std::string::npos) {
							Game::iManager.loadInteraction("buy truck");
						}
						if (entry->getName().find("van") != std::string::npos) {
							Game::iManager.loadInteraction("buy van");
						}
						if (entry->getName().find("racer") != std::string::npos) {
							Game::iManager.loadInteraction("buy racer");
						}
					}
				}
			}

			// entry->getEntityData()->Rotation.x += 2 * dt;
			// if (entry->getEntityData()->Rotation.x > 360) entry->getEntityData()->Rotation.x -= 360;
		}
		if (entry->getType() == ENTITYTYPE::CAR) {
			if (Math::FAbs((entry->getEntityData()->Translate - player->getEntityData()->Translate).Magnitude()) < 6 && !camMap) {
				if (!player->isDriving())
					Game::uiManager.setUIactive(UI_E_TO_INTERACT);
				// Show interaction UI
				if (ePressed && !eHeld) {
					eHeld = true;
					if (((Car*)entry)->getPlayer() == nullptr && !player->isDriving()) {
						player->setDriving((Car*)entry, true);
						((Car*)entry)->setPlayer(player);
						camera.camType = THIRDPERSON;
						std::cout << "Player Set" << std::endl;
					}
					else if (((Car*)entry)->getPlayer() != nullptr && player->isDriving()) {
						player->setDriving(nullptr, false);
						camera.position = camera.playerPtr->getEntityData()->Translate - camera.TPSPositionVector;
						((Car*)entry)->setPlayer(nullptr);
						camera.camType = FIRSTPERSON;
						player->getEntityData()->Translate.Set(entry->getEntityData()->Translate.x + 6, 0, entry->getEntityData()->Translate.z);
						player->PostUpdate(); // set old data to new data, lazy fix for now
						camera.position = player->getEntityData()->Translate;
						camera.position.y += 2;
						camera.total_pitch = 0;
						camera.total_yaw = 0;
						camera.up = camera.defaultUp;
						camera.target = camera.position - Vector3(0, 0, 1);
					}
				}
			}
		}
	}

	for (auto& entry : collided) {
		if (entry->attacker->getType() == ENTITYTYPE::PLAYER && !player->isDriving()) {
			if (entry->victim->getType() == ENTITYTYPE::LIVE_NPC || entry->victim->getType() == ENTITYTYPE::WORLDOBJ || entry->victim->getType() == ENTITYTYPE::CAR) {
				// player->getEntityData()->Translate += entry->plane * 2;
				// player->cancelNextMovement();
				entry->attacker->getEntityData()->Translate -= entry->translationVector;
				std::cout << "Collided " << entry->translationVector.x << " " << entry->translationVector.y << " " << entry->translationVector.z << std::endl;
			}

			if (entry->victim->getType() == ENTITYTYPE::CUSTOM) {
				if (entry->victim->getName().find("hitbox") != std::string::npos) {
					entry->attacker->getEntityData()->Translate -= entry->translationVector;
				}

				if (entry->victim->getName().find("interaction") != std::string::npos) {
					foundInteractionZone = true;
					if (!canInteractWithSomething)
						canInteractWithSomething = true;
				}
			}

		}

		if (entry->attacker->getType() == ENTITYTYPE::CAR) {
			if (entry->victim->getType() == ENTITYTYPE::WORLDOBJ) {
				// entry->attacker->cancelNextMovement();
				float backwardsMomentum = -((Car*)entry->attacker)->getSpeed() * 0.5f;
				((Car*)entry->attacker)->setSpeed(backwardsMomentum);
				entry->attacker->getEntityData()->Translate -= entry->translationVector + ((Car*)entry->attacker)->getVelocity();
				std::cout << backwardsMomentum << std::endl;
				std::cout << "Car Collided" << std::endl;
			}

			if (entry->victim->getType() == ENTITYTYPE::LIVE_NPC) {
				float backwardsMomentum = 0.f;
				float resultantForce = ((Car*)entry->attacker)->getSpeed() * 5.f;
				Vector3 resultantVec = resultantForce * ((Car*)entry->attacker)->getVelocity();
				resultantVec.y = resultantForce * 0.2f;
				Math::Clamp(resultantVec.y, 0.f, 1.0f);
				((Car*)entry->attacker)->setSpeed(backwardsMomentum);
				entry->attacker->getEntityData()->Translate -= entry->translationVector + ((Car*)entry->attacker)->getVelocity();
				((NPC*)entry->victim)->getRigidBody().velocity = resultantVec;
				std::cout << "Car Collided" << std::endl;
			}
		}

		if (entry->attacker->getType() == ENTITYTYPE::LIVE_NPC) {
			if (entry->victim->getType() == ENTITYTYPE::WORLDOBJ) {
				Vector3 resultantVec;
				Vector3 d = ((NPC*)entry->attacker)->getRigidBody().velocity;
				Vector3 n = entry->normal;
				resultantVec = d - 2 * d.Dot(n) * n;
				((NPC*)entry->attacker)->getRigidBody().velocity = resultantVec;
				entry->attacker->getEntityData()->Translate -= entry->translationVector;
			}

		}

		if (entry->attacker->getType() == ENTITYTYPE::PLAYER) {
			if (entry->victim->getType() == ENTITYTYPE::CUSTOM) {
				if (entry->victim->getName().find("garageDoorHitBox") != std::string::npos)
				{
					Game::uiManager.setUIactive(UI_E_TO_INTERACT);
					if (ePressed && !eHeld && Game::sceneCooldown > 3)
					{
						eHeld = true;
						Scene* var = Game::getSceneByName("MainScene");
						Game::switchScene(S_2021);
					}
				}
			}
		}
		if (entry->attacker->getType() == ENTITYTYPE::CAR)
		{
			if (entry->victim->getType() == ENTITYTYPE::CUSTOM)
			{
				if (entry->victim->getName().find("garageDoorHitBox") != std::string::npos)
				{
					Scene* var = Game::getSceneByName("MainScene");
					static_cast <Scene2021*>(var)->spawnGarageCar(player->getCar()->getCartype());


					entry->attacker->getEntityData()->Translate.Set(0.f, -10.f,0.0f);
					
					Game::switchScene(S_2021);
				}
			}
		}
	}

	if (foundInteractionZone == false) {
		canInteractWithSomething = false;
	}
	eManager.collisionUpdate(dt);

	if (player->usingNewData()) { //Aka movement not cancelled
		camera.Move(player->getEntityData()->Translate.x - player->getOldEntityData()->Translate.x,
			player->getEntityData()->Translate.y - player->getOldEntityData()->Translate.y,
			player->getEntityData()->Translate.z - player->getOldEntityData()->Translate.z);
	}

	camera.Update(dt);
	eManager.postCollisionUpdate();

	fps = (float)1 / dt;

	/*if (isInteracting && passedInteractCooldown()) {
		if (ePressed) {
			nextInteraction();

		}
		latestInteractionSwitch = this->elapsed;
	}*/
}

void SceneCarShop::Render()
{
	glEnableVertexAttribArray(0); // 1st attribute buffer: vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer: color

	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (camMap || Game::uiManager.getCurrentMenu() == UI_MAIN_MENU || Game::uiManager.getCurrentMenu() == UI_CREDITS)
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(camera2.position.x, camera2.position.y, camera2.position.z,
			camera2.target.x, camera2.target.y, camera2.target.z,
			camera2.up.x, camera2.up.y, camera2.up.z);
	}
	else
	{
		viewStack.LoadIdentity();
		viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z);

	}

	modelStack.LoadIdentity();


	if (light[0].type == Light::LIGHT_DIRECTIONAL) {
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDir_cameraSpace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDir_cameraSpace.x);

	}
	else if (light[0].type == Light::LIGHT_SPOT) {
		Position lightPos_cameraSpace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPos_cameraSpace.x);
		Vector3 spotDir_cameraSpace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDir_cameraSpace.x);

	}
	else { //Point light
		Position lightPos_cameraSpace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPos_cameraSpace.x);
	}

	switch (camera.camType)
	{
	case TOPDOWN_FIRSTPERSON:
		if (light[1].type == Light::LIGHT_DIRECTIONAL) {
			Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
			Vector3 lightDir_cameraSpace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDir_cameraSpace.x);
		}
		else if (light[1].type == Light::LIGHT_SPOT) {
			Position lightPos_cameraSpace = viewStack.Top() * light[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPos_cameraSpace.x);
			Vector3 spotDir_cameraSpace = viewStack.Top() * light[1].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDir_cameraSpace.x);
		}
		else { //Point light
			Position lightPos_cameraSpace = viewStack.Top() * light[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPos_cameraSpace.x);
		}
		modelStack.PushMatrix();
		modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
		RenderMesh(MeshHandler::getMesh(GEO_LIGHTBALL), false);
		modelStack.PopMatrix();
		break;
	case TOPDOWN_THIRDPERSON:
		if (light[1].type == Light::LIGHT_DIRECTIONAL) {
			Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
			Vector3 lightDir_cameraSpace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDir_cameraSpace.x);

		}
		else if (light[1].type == Light::LIGHT_SPOT) {
			Position lightPos_cameraSpace = viewStack.Top() * light[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPos_cameraSpace.x);
			Vector3 spotDir_cameraSpace = viewStack.Top() * light[1].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDir_cameraSpace.x);

		}
		else { //Point light
			Position lightPos_cameraSpace = viewStack.Top() * light[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPos_cameraSpace.x);
		}
		break;
	case THIRDPERSON:
		if (light[2].type == Light::LIGHT_DIRECTIONAL) {
			Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
			Vector3 lightDir_cameraSpace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDir_cameraSpace.x);

		}
		else if (light[2].type == Light::LIGHT_SPOT) {
			Position lightPos_cameraSpace = viewStack.Top() * light[2].position;
			glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPos_cameraSpace.x);
			Vector3 spotDir_cameraSpace = viewStack.Top() * light[2].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDir_cameraSpace.x);

		}
		else { //Point light
			Position lightPos_cameraSpace = viewStack.Top() * light[2].position;
			glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPos_cameraSpace.x);
		}
		break;
	default:
		break;
	}


	this->RenderSkybox();

	modelStack.PushMatrix();
	modelStack.Translate(0, -0.1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(MeshHandler::getMesh(GEO_GRASS), lightEnable, GL_REPEAT);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(2.5, 2, 2.5);
	RenderMesh(MeshHandler::getMesh(GEO_SHOWROOM), lightEnable);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3, 0, -17);
	modelStack.Scale(5, 0.4, 5);
	RenderMesh(MeshHandler::getMesh(GEO_CYLINDER), lightEnable);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(17, 0, -15);
	modelStack.Scale(5, 0.4, 5);
	RenderMesh(MeshHandler::getMesh(GEO_CYLINDER), lightEnable);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(17, 0, 0);
	modelStack.Scale(5, 0.4, 5);
	RenderMesh(MeshHandler::getMesh(GEO_CYLINDER), lightEnable);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(17, 0, 15);
	modelStack.Scale(5, 0.4, 5);
	RenderMesh(MeshHandler::getMesh(GEO_CYLINDER), lightEnable);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3, 0, 17);
	modelStack.Scale(5, 0.4, 5);
	RenderMesh(MeshHandler::getMesh(GEO_CYLINDER), lightEnable);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(-12, 0, 17);
	modelStack.Scale(5, 0.4, 5);
	RenderMesh(MeshHandler::getMesh(GEO_CYLINDER), lightEnable);
	modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	modelStack.Translate(19.0f, 0.0f, -44.0f);
	modelStack.Scale(10.0f, 10.0f, 10.0f);
	RenderMesh(MeshHandler::getMesh(SNOW_PATCH), true);
	modelStack.PopMatrix();*/

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 22, 99); 
	//modelStack.Rotate(180, 0, 1, 0);
	//modelStack.Scale(80, 44, 100); //scaling of y-axis is 2x of translate y
	//RenderMesh(MeshHandler::getMesh(GARAGE_DOOR), true);
	//modelStack.PopMatrix();

	for (auto& entity : eManager.getEntities()) {
		entity->Render();
		if (hitboxEnable) { //Downside: Can't view hitbox accurately of Objects that are rotated
			modelStack.PushMatrix();
			Mesh* mesh = MeshBuilder::GenerateHitBox("hitbox", *entity->getHitBox()->getThisTickBox());
			modelStack.Translate(entity->getEntityData()->Translate.x, entity->getEntityData()->Translate.y, entity->getEntityData()->Translate.z);
		    modelStack.Rotate(entity->getEntityData()->Rotation.x, 1, 0, 0);
			modelStack.Rotate(entity->getEntityData()->Rotation.y, 0, 1, 0);
			modelStack.Rotate(entity->getEntityData()->Rotation.z, 0, 0, 1);
			modelStack.Translate(-entity->getEntityData()->Translate.x, -entity->getEntityData()->Translate.y, -entity->getEntityData()->Translate.z);
			this->RenderMesh(mesh, false);
			modelStack.PopMatrix();
			delete mesh;
		}
	}

	if (Game::inv.getActiveWeapon() != nullptr && !player->isDriving()) {
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 right = view.Cross(camera.up);
		right.Normalize();

		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + view.x, camera.position.y + view.y, camera.position.z + view.z);
		//modelStack.Translate(0.175, -0.1, -0.35);
		modelStack.Scale(0.8, 0.8, 0.8);
		modelStack.Rotate(camera.total_pitch, right.x, right.y, right.z);
		modelStack.Rotate(camera.total_yaw, 0, 1, 0);
		modelStack.Translate(0.25, -0.1, 0.75);
		modelStack.Rotate(185, 0, 1, 0);
		RenderMesh(MeshHandler::getMesh(Game::inv.getActiveWeapon()->getMeshType()), lightEnable);
		modelStack.PopMatrix();

		RenderMeshOnScreen(MeshHandler::getMesh(UI_CROSSHAIR), 64, 36, 2, 2);
	}
	RenderUI();
	bManager.Render(this);

	std::ostringstream ss;
	
	//Interaction MSG UI
	if (canInteractWithSomething && !isInteracting) {
		ss.str("");
		ss.clear();
		ss << "Press 'E' to Interact";
		RenderTextOnScreen(MeshHandler::getMesh(GEO_TEXT), ss.str(), Color(1, 1, 1), 4, 20, 10);
	}
	
	//FPS UI
	ss.str("");
	ss.clear();
	ss << "FPS: " << fps;
	RenderTextOnScreen(MeshHandler::getMesh(GEO_TEXT), ss.str(), Color(0, 1, 0), 4, 0, 5);
}

void SceneCarShop::RenderSkybox() {
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
		modelStack.PushMatrix();
		modelStack.Translate(-250.0f, 0.0f, 0.0f);
		modelStack.Rotate(90, 0.0f, 1.0f, 0.0f);
		modelStack.Scale(500.0f, 500.0f, 500.0f);
		this->RenderMesh(MeshHandler::getMesh(GEO_SKY_LEFT), false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(250.0f, 0.0f, 0.0f);
		modelStack.Rotate(-90, 0.0f, 1.0f, 0.0f);
		modelStack.Scale(500.0f, 500.0f, 500.0f);
		this->RenderMesh(MeshHandler::getMesh(GEO_SKY_RIGHT), false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.0f, 250.0f, 0.0f);
		modelStack.Rotate(-90, 0.0f, 1.0f, 0.0f);
		modelStack.Rotate(90, 1.0f, 0.0f, 0.0f);
		modelStack.Scale(500.0f, 500.0f, 500.0f);
		this->RenderMesh(MeshHandler::getMesh(GEO_SKY_TOP), false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.0f, -250.0f, 0.0f);
		modelStack.Rotate(-90, 0.0f, 1.0f, 0.0f);
		modelStack.Rotate(90, -1.0f, 0.0f, 0.0f);
		modelStack.Scale(500.0f, 500.0f, 500.0f);
		this->RenderMesh(MeshHandler::getMesh(GEO_SKY_BOTTOM), false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.0f, 0.0f, -250.0f);
		modelStack.Rotate(0, 1.0f, 0.0f, 0.0f);
		modelStack.Scale(500.0f, 500.0f, 500.0f);
		this->RenderMesh(MeshHandler::getMesh(GEO_SKY_BACK), false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0.0f, 0.0f, 250.0f);
		modelStack.Rotate(180, 0.0f, 1.0f, 0.0f);
		modelStack.Scale(500.0f, 500.0f, 500.0f);
		this->RenderMesh(MeshHandler::getMesh(GEO_SKY_FRONT), false);
		modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneCarShop::initCollidables(Vector3 v3T, Vector3 v3R, Vector3 v3S, GEOMETRY_TYPE geoType)
{
	Entity* collidables = new WorldObject(this, geoType, "building");
	collidables->getEntityData()->SetTransform(v3T.x, v3T.y, v3T.z);
	collidables->getEntityData()->SetRotate(v3R.x, v3R.y, v3R.z);
	collidables->getEntityData()->SetScale(v3S.x, v3S.y, v3S.z);
	eManager.spawnWorldEntity(collidables);
}

void SceneCarShop::SpawnWalls() {
//{
//	Vector3 uniformWallScale = Vector3(50.0f, 25.0f, 50.0f);
//	Vector3 longWallScale = Vector3(100.0f, 25.0f, 100.0f);
//	initCollidables(Vector3(0.0f, 9.9f, 0.0f), Vector3(90.0f, 0.0f, 0.0f), Vector3(50.0f, 100.0f, 100.0f), CONCRETE_WALL); //top
//	initCollidables(Vector3(25.0f, 12.4f, 0.0f), Vector3(0.0f, -90.0f, 0.0f), longWallScale, CONCRETE_WALL); //x-axis
//	initCollidables(Vector3(-25.0f, 12.4f, 0.0f), Vector3(0.0f, 90.0f, 0.0f), longWallScale, CONCRETE_WALL);
//	initCollidables(Vector3(0.0f, 12.4f, 50.0f), Vector3(0.0f, 180.0f, 0.0f), uniformWallScale, CONCRETE_WALL); // z-axis
//	initCollidables(Vector3(0.0f, 12.4f, -50.0f), Vector3(0.0f, 0.0f, 0.0f), uniformWallScale, CONCRETE_WALL);
//	initCollidables(Vector3(0.0f, 2.0f, -50.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(2.0f, 2.0f, 1.0f), GEO_DOOR); //door
//
//	//Objects
//	initCollidables(Vector3(20.0f, 2.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(2.0f, 2.0f, 1.0f), GEO_DOOR); //north door
//	initCollidables(Vector3(-15.0f, 2.0f, 15.0f), Vector3(0.0f, -90.0f, 0.0f), Vector3(2.0f, 2.0f, 1.0f), GEO_DOOR); //north door
//
//	initCollidables(Vector3(10.0f, 2.0f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(2.0f, 2.0f, 1.0f), GEO_DOOR); //west door
//	
//	initCollidables(Vector3(-20.0f, 2.0f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(2.0f, 2.0f, 1.0f), GEO_DOOR); //east door
//
//	//North Walls
//	initCollidables(Vector3(-10.0f, 4.9f, 0.0f), Vector3(0.0f, 180.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve z-axis
//	initCollidables(Vector3(0.0f, 4.9f, 0.0f), Vector3(0.0f, 180.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve z-axis
//	initCollidables(Vector3(10.0f, 4.9f, 0.0f), Vector3(0.0f, 180.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve z-axis
//	initCollidables(Vector3(20.0f, 4.9f, 0.0f), Vector3(0.0f, 180.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve z-axis
//
//	initCollidables(Vector3(-15.0f, 4.9f, 5.0f), Vector3(0.0f, -90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve x-axis
//	initCollidables(Vector3(-15.0f, 4.9f, 15.0f), Vector3(0.0f, -90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve x-axis
//	initCollidables(Vector3(-15.0f, 4.9f, 25.0f), Vector3(0.0f, -90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve x-axis
//	initCollidables(Vector3(-15.0f, 4.9f, 35.0f), Vector3(0.0f, -90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve x-axis
//
//	initCollidables(Vector3(-10.0f, 4.9f, 40.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve z-axis
//	initCollidables(Vector3(0.0f, 4.9f, 40.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve z-axis
//	initCollidables(Vector3(10.0f, 4.9f, 40.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve z-axis
//
//	initCollidables(Vector3(15.0f, 4.9f, 45.0f), Vector3(0.0f, -90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve x-axis
//
//	//West Walls
//	initCollidables(Vector3(5.0f, 4.9f, -45.0f), Vector3(0.0f, -90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve x-axis
//	initCollidables(Vector3(5.0f, 4.9f, -35.0f), Vector3(0.0f, -90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve x-axis
//	initCollidables(Vector3(5.0f, 4.9f, -25.0f), Vector3(0.0f, -90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve x-axis
//	initCollidables(Vector3(5.0f, 4.9f, -15.0f), Vector3(0.0f, -90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards -ve x-axis
//
//	initCollidables(Vector3(10.0f, 4.9f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve z-axis
//	initCollidables(Vector3(20.0f, 4.9f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve z-axis
//
//	//East Walls
//	initCollidables(Vector3(-5.0f, 4.9f, -45.0f), Vector3(0.0f, 90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve x-axis
//	initCollidables(Vector3(-5.0f, 4.9f, -35.0f), Vector3(0.0f, 90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve x-axis
//	initCollidables(Vector3(-5.0f, 4.9f, -25.0f), Vector3(0.0f, 90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve x-axis
//	initCollidables(Vector3(-5.0f, 4.9f, -15.0f), Vector3(0.0f, 90.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve x-axis
//
//	initCollidables(Vector3(-10.0f, 4.9f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve z-axis
//	initCollidables(Vector3(-20.0f, 4.9f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), CONCRETE_WALL); //face towards +ve z-axis
//
//	//initCollidables(Vector3(25.0f, 0.0, 25.0f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), CONCRETE_WALL);
//	//initCollidables(Vector3(0.0f, 11.0f, 49.0f), Vector3(180.0f, 0.0f, 0.0f), Vector3(40.0f, 22.0f, 50.0f), GARAGE_DOOR); //garage door
}

void SceneCarShop::RenderUI()
{
	Game::RenderUI();
}

void SceneCarShop::Exit()
{
	// Cleanup VBO here
	//this->EndInteraction(); //To clear up queuedMessages pointers

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}