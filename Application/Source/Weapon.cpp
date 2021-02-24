#include "Weapon.h"
#include "Application.h"
#include "Debug.h"
#include "Vector3.h"

Weapon::Weapon() //set damage and magSize to 0
{
	damage = 0;
	magazineSize = 0;
	currentAmmo = 0;
}

Weapon::~Weapon()
{
}

Weapon::Weapon(WEAPON_TYPE wType)
{
	switch (wType) //based on new weaponType, init the dmg and magSize
	{
	case PISTOL:
		initPistol();
		break;

	case SILENCER:
		initSilencer();
		break;

	default:
		break;
	}
}

void Weapon::initPistol() //init fist with 20 dmg, 10 magSize
{
	initWeapon(PISTOL, 20, 10);
	associatedMeshType = GEO_PISTOL;
}

void Weapon::initSilencer() //init fist with 15 dmg, 15 magSize
{
	initWeapon(SILENCER, 15, 15);
	associatedMeshType = GEO_PISTOL_S;
}

void Weapon::Update(Scene* scene, EntityManager* eManager, Vector3 plrPos, Vector3 view, double dt) {
	if (this != nullptr) {
		if (Application::IsMousePressed(0) && !shoot && this->currentAmmo > 0 && Game::ammo > 0) {
			//Vector3 view = (cam->target - cam->position).Normalized();
			Entity* bullet = new Bullet(scene, view * 100.f, "bullet");
			bullet->getEntityData()->Translate.Set(plrPos.x, plrPos.y + 2, plrPos.z);
			bullet->getEntityData()->Scale.Set(0.1, 0.1, 0.1);
			// new function to spawn bullet at a scene's emanager, take in emanager as a refernce in this function to push back a bullet
			eManager->spawnMovingEntity(bullet);
			shoot = true;
			DEBUG_MSG("shot");
			DEBUG_MSG("x: " << view.x << " y: " << view.y << " z: " << view.z);
			this->currentAmmo--; //minus current gun ammo
			Game::ammo--; //minus total ammo
		}
		else if (!Application::IsMousePressed(0)) shoot = false;

		if (Application::IsKeyPressed('R') && !Reload) //start reload
			Reload = true;
		
		if (Reload)
			reloadTillTime += dt; //start timer
		else
			reloadTillTime = 0;

		if (reloadTillTime > 2) //after 2 seconds, reload complete
		{
			int reloadAmmo = this->magazineSize - this->currentAmmo; //get amt to reload
			this->currentAmmo += reloadAmmo; //add the reload amt to current gun ammo
			Game::ammo -= reloadAmmo; //minus total gun ammo to amt reloaded
			Reload = false;
		}
		Game::uiManager.getByTypeBM(UI_GENERAL)->getButtonByName("AmmoCount")->setText(std::to_string(this->currentAmmo) + "/" + std::to_string(this->magazineSize));
		Game::uiManager.getByTypeBM(UI_GENERAL)->getButtonByName("TotalAmmoCount")->setText(std::to_string(Game::ammo));
	}
}

void Weapon::initWeapon(WEAPON_TYPE wType, int damages, int magSize) //init weapon with new weaponType, damage and magSize
{
	this->weaponType = wType;
	this->damage = damages;
	this->magazineSize = magSize;
	this->currentAmmo = magSize;
}

GEOMETRY_TYPE Weapon::getMeshType()
{
	return associatedMeshType;
}

WEAPON_TYPE Weapon::getWeaponType()
{
	if (this != nullptr)
		return this->weaponType;
}
