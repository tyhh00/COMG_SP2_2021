#include "Inventory.h"
#include "Application.h"

/******************************************************************************/
/*!
\brief Initialise the inventory here.
*/
/******************************************************************************/
Inventory::Inventory()
{
	currentCar = nullptr;

	//Create garage inventory
	currentCar = new GarageInventory(SEDAN);
	garageInv.push_back(currentCar);

	//For item inventory
	itemInventory = new ItemInventory();

	//for weapon inven
	weaponInv = new WeaponInventory();
}

Inventory::~Inventory()
{
}

/******************************************************************************/
/*!
\brief Adds a weapon to the weapon inventory.
*/
/******************************************************************************/
void Inventory::addWeap(WEAPON_TYPE weaponType)
{
	weaponInv->addWeapon(weaponType);
}

/******************************************************************************/
/*!
\brief Adds a car to the garage inventory.
*/
/******************************************************************************/
void Inventory::addCar(CAR_TYPE cartype)
{
	if (garageInv.empty()) //If player does not have any item yet
	{
		currentCar = new GarageInventory(cartype);
		garageInv.push_back(currentCar);
	}
	for (int i = 0; i < garageInv.size(); i++) //If player owns car already
	{
		if (garageInv[i]->getCarType() == cartype)
		{
			return;
		}
	}
	GarageInventory* addingCar; //If player does not own a specific car yet
	addingCar = new GarageInventory(cartype);
	garageInv.push_back(addingCar);
}

/******************************************************************************/
/*!
\brief Adds an item to the item inventory.
*/
/******************************************************************************/
void Inventory::addItem(ITEM_TYPE itemtype, int amt)
{
	if (itemInventory == nullptr) //If player does not have any item yet
	{
		itemInventory = new ItemInventory(itemtype, amt);
	}
	else
	{
		itemInventory->addItem(itemtype, amt);
	}
}

/******************************************************************************/
/*!
\brief Changes the item amount.
*/
/******************************************************************************/
void Inventory::changeItemAmt(ITEM_TYPE itemtype, int amt)
{
	if (itemInventory != nullptr)
		itemInventory->setAmtOfItems(itemtype, amt);
	return;
}

/******************************************************************************/
/*!
\brief Deletes a weapon from the weapon inventory.
*/
/******************************************************************************/
void Inventory::deleteWeapon(WEAPON_TYPE wType)
{
	if(this->weaponInv->getActiveWeapon() != nullptr)
		this->weaponInv->delWeapon(wType);
}

/******************************************************************************/
/*!
\brief switches the current car.
*/
/******************************************************************************/
void Inventory::switchCar(int cartype)
{
	for (int i = 0; i < garageInv.size(); i++)
	{
		if (garageInv[i]->getCarType() == cartype)
		{
			currentCar = garageInv[i];
			return;
		}
	}
}

/******************************************************************************/
/*!
\brief Switches the active weapon.
*/
/******************************************************************************/
void Inventory::switchWeapon(int index)
{
	if(weaponInv != nullptr)
		weaponInv->switchActiveWeapon(index);
}

/******************************************************************************/
/*!
\brief Toggles the current item to be displayed on the UI.
*/
/******************************************************************************/
void Inventory::toggleItem()
{
	if (itemInventory != nullptr)
		itemInventory->toggleItem();
	return;
}

/******************************************************************************/
/*!
\brief Returns the current car.
*/
/******************************************************************************/
GarageInventory* Inventory::getCurrentCar()
{
	if (currentCar != nullptr)
		return this->currentCar;
	else
		return nullptr;
}

/******************************************************************************/
/*!
\brief returns the Garage Inventory vector.
*/
/******************************************************************************/
std::vector<GarageInventory*> Inventory::getGarageVector()
{
	return this->garageInv;
}

/******************************************************************************/
/*!
\brief returns the Item Inventory.
*/
/******************************************************************************/
ItemInventory* Inventory::getItemInventory()
{
	return this->itemInventory;
}

/******************************************************************************/
/*!
\brief returns the Weapon Inventory.
*/
/******************************************************************************/
WeaponInventory* Inventory::getWeaponInventory() {
	return this->weaponInv;
}

/******************************************************************************/
/*!
\brief returns the WeaponList vector.
*/
/******************************************************************************/
std::vector<Weapon*> Inventory::getWeaponVector()
{
	return weaponInv->getWeaponList();
}

/******************************************************************************/
/*!
\brief returns the ItemInv vector
*/
/******************************************************************************/
std::vector<Item*> Inventory::getItemVector()
{
	return itemInventory->getItemVect();
}

/******************************************************************************/
/*!
\brief returns the current car's type.
*/
/******************************************************************************/
CAR_TYPE Inventory::getCurrentCarType()
{
	return currentCar->getCarType();
}

/******************************************************************************/
/*!
\brief returns the current item's type.
*/
/******************************************************************************/
ITEM_TYPE Inventory::getCurrentItemType()
{
	if (itemInventory != nullptr)
		if (itemInventory->getCurrentItem() != nullptr)
			return itemInventory->getCurrentItemType();
}

/******************************************************************************/
/*!
\brief Updates the Inventory's UI here.
*/
/******************************************************************************/
void Inventory::Update(double dt)
{
	//For General UI
	//Items
	if (itemInventory->getCurrentItem() != nullptr)
	{
		switch (getCurrentItemType())
		{
		case BURGER:
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIItem")->setQuadImage(UI_BURGER);
			break;
		case CORN:
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIItem")->setQuadImage(UI_CORN);
			break;
		case EGGPLANT:
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIItem")->setQuadImage(UI_EGGPLANT);
			break;
		default:
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIItem")->setQuadImage(UI_EMPTY);
			break;
		}
		Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIItem")->enable();
		Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIItemCurrent")->enable();
		Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIItemAmount")->setText(std::to_string(itemInventory->getCurrentItemAmt()));
		Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIItemAmount")->enable();
	}
	else
		Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIItemAmount")->disable();

	//Weapons
	if (weaponInv->getWeaponList().size() == 0)
	{
		Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon1")->disable();
		Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->disable();
		for (int i = 0; i < WEAPON_COUNT; i++)
		{
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIWeaponBorder" + std::to_string(i + 1))->disable();
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIWeaponCurrent" + std::to_string(i + 1))->disable();
		}
	}
	else if (weaponInv->getWeaponList().size() > 0)
	{
		if (weaponInv->getActiveWeapon() != nullptr)
		{
			//display first gun slot
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon1")->enable();
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->disable();

			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIWeaponCurrent1")->enable();
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIWeaponCurrent2")->disable();
			switch (weaponInv->getWeaponList()[0]->getWeaponType())
			{
			case PISTOL:
				Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon1")->setQuadImage(UI_PISTOL);
				break;
			case SILENCER:
				Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon1")->setQuadImage(UI_SILENCER);
				break;
			case FIRE_EXTINGUISHER:
				Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon1")->setQuadImage(UI_FIRE_EXTINGUISHER);
				break;
			}

			if (weaponInv->getWeaponList().size() == 2) //check if size is 2, then display second gun slot
			{
				Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->enable();
				switch (weaponInv->getWeaponList()[1]->getWeaponType())
				{
				case PISTOL:
					Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->setQuadImage(UI_PISTOL);
					break;
				case SILENCER:
					Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->setQuadImage(UI_SILENCER);
					break;
				case FIRE_EXTINGUISHER:
					Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->setQuadImage(UI_FIRE_EXTINGUISHER);
					break;
				}
				if (weaponInv->getWeaponList()[1] == weaponInv->getActiveWeapon())
				{
					Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIWeaponCurrent1")->disable();
					Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIWeaponCurrent2")->enable();
				}
			}
		}
		else
		{
			//display first gun slot
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon1")->enable();
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->disable();

			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIWeaponCurrent1")->disable();
			Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("UIWeaponCurrent2")->disable();

			switch (weaponInv->getWeaponList()[0]->getWeaponType())
			{
			case PISTOL:
				Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon1")->setQuadImage(UI_PISTOL);
				break;
			case SILENCER:
				Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon1")->setQuadImage(UI_SILENCER);
				break;
			case FIRE_EXTINGUISHER:
				Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon1")->setQuadImage(UI_FIRE_EXTINGUISHER);
				break;
			}

			if (weaponInv->getWeaponList().size() == 2) //check if size is 2, then display second gun slot
			{
				Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->enable();
				switch (weaponInv->getWeaponList()[1]->getWeaponType())
				{
				case PISTOL:
					Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->setQuadImage(UI_PISTOL);
					break;
				case SILENCER:
					Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->setQuadImage(UI_SILENCER);
					break;
				case FIRE_EXTINGUISHER:
					Game::uiManager.getbManagerArray(UI_GENERAL)->getButtonByName("Weapon2")->setQuadImage(UI_FIRE_EXTINGUISHER);
					break;
				}
			}
		}
	}

	//For Item Inventory UI
	if (itemInventory->getCurrentItem() == nullptr) //If there is no Item Inventory
	{
		Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIInventoryBackground")->enable();
		for (int i = 0; i < ITEM_AMOUNT; i++)
		{
			Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemInventorySlot" + std::to_string(i + 1))->disable();
			Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemInventorySlotCurrent" + std::to_string(i + 1))->disable();
		}
		Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryCurrent")->disable();
		Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryCurrentBorder")->disable();
		Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryName")->disable();
	}
	else if (itemInventory->getItemVect().size() > 0) //If item is owned at least once, then display
	{
		Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIInventoryBackground")->enable();
		for (int i = 0; i < itemInventory->getItemVect().size(); i++)
		{
			switch (itemInventory->getItemVect()[i]->getType())
			{
			case BURGER:
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_BURGER);
				break;
			case CORN:
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_CORN);
				break;
			case EGGPLANT:
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_EGGPLANT);
				break;
			}
			Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemInventorySlot" + std::to_string(i + 1))->enable();
		}
		for (int i = 0; i < itemInventory->getItemVect().size(); i++)
		{
			//Disable all non-current item UIs
			if (itemInventory->getCurrentItem()->getType() == itemInventory->getItemVect().at(i)->getType())
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemInventorySlotCurrent" + std::to_string(i + 1))->enable();
			else
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemInventorySlotCurrent" + std::to_string(i + 1))->disable();

			switch (itemInventory->getCurrentItemType())
			{
			case BURGER:
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryCurrent")->setQuadImage(UI_BURGER);
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryName")->setText("Burger\nAmount:\n" + std::to_string(itemInventory->getCurrentItemAmt()));
				break;
			case CORN:
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryCurrent")->setQuadImage(UI_CORN);
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryName")->setText("Corn\nAmount:\n" + std::to_string(itemInventory->getCurrentItemAmt()));
				break;
			case EGGPLANT:
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryCurrent")->setQuadImage(UI_EGGPLANT);
				Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryName")->setText("Eggplant\nAmount:\n" + std::to_string(itemInventory->getCurrentItemAmt()));
				break;
			}
			Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryCurrent")->enable();
			Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryCurrentBorder")->enable();
			Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryName")->enable();
		}
		
	}
	else //Do not display any item slots as no items owned
	{
		Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIInventoryBackground")->enable();
		for (int i = 0; i < ITEM_AMOUNT; i++)
		{
			Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemInventorySlot" + std::to_string(i + 1))->disable();
			Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemInventorySlotCurrent" + std::to_string(i + 1))->disable();
		}
		Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryCurrent")->disable();
		Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryCurrentBorder")->disable();
		Game::uiManager.getbManagerArray(UI_ITEM_INVENTORY)->getButtonByName("UIItemsInventoryName")->disable();
	}

	//For Weapon Inventory
	if (weaponInv == nullptr || weaponInv->getWeaponList().empty()) //If there is no Item Inventory
	{
		Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIInventoryBackground")->enable();
		for (int i = 0; i < WEAPON_COUNT; i++)
		{
			Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlot" + std::to_string(i + 1))->disable();
			Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlotCurrent" + std::to_string(i + 1))->disable();
		}
		Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrent")->disable();
		Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrentBorder")->disable();
		Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryName")->disable();
	}
	else if (weaponInv->getWeaponList().size() > 0) //If item is owned at least once, then display
	{
		Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIInventoryBackground")->enable();

		if (weaponInv->getActiveWeapon() == nullptr)
			Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrentBorder")->disable();
		else
			Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrentBorder")->enable();

		for (int i = 0; i < WEAPON_COUNT; i++)
		{
			Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlotCurrent" + std::to_string(i + 1))->disable();
			if (i < weaponInv->getWeaponList().size())
			{
				switch (weaponInv->getWeaponList()[i]->getWeaponType())
				{
				case PISTOL:
					Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_PISTOL);
					break;
				case SILENCER:
					Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_SILENCER);
					break;
				case FIRE_EXTINGUISHER:
					Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_FIRE_EXTINGUISHER);
					break;
				default:
					Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlot" + std::to_string(i + 1))->disable();
					break;
				}
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlot" + std::to_string(i + 1))->enable();
			}
			else
			{
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlot" + std::to_string(i + 1))->disable();
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrent")->disable();
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryName")->disable();
			}
			

		}

		for (int i = 0; i < weaponInv->getWeaponList().size(); i++)
		{
			if (weaponInv->getActiveWeapon() == nullptr) continue;
			//Disable all non-current item UIs
			if (weaponInv->getActiveWeapon()->getWeaponType() == weaponInv->getWeaponList()[i]->getWeaponType())
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlotCurrent" + std::to_string(i + 1))->enable();
			else
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlotCurrent" + std::to_string(i + 1))->disable();

			switch (weaponInv->getActiveWeapon()->getWeaponType())
			{
			case PISTOL:
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrent")->setQuadImage(UI_PISTOL);
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryName")->setText("Pistol\nAmmo:\n" + std::to_string(weaponInv->getActiveWeapon()->getWeaponAmmo()));
				break;
			case SILENCER:
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrent")->setQuadImage(UI_SILENCER);
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryName")->setText("Silencer\nAmmo:\n" + std::to_string(weaponInv->getActiveWeapon()->getWeaponAmmo()));
				break;
			case FIRE_EXTINGUISHER:
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrent")->setQuadImage(UI_FIRE_EXTINGUISHER);
				Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryName")->setText("Fire Extinguisher");
				break;
			}
			Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrent")->enable();
			Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryName")->enable();
		}

	}
	else //Do not display any item slots as no items owned
	{
		Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIInventoryBackground")->enable();
		for (int i = 0; i < WEAPON_COUNT; i++)
		{
			Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlot" + std::to_string(i + 1))->disable();
			Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponInventorySlotCurrent" + std::to_string(i + 1))->disable();
		}
		Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrent")->disable();
		Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryCurrentBorder")->disable();
		Game::uiManager.getbManagerArray(UI_WEAPON_INVENTORY)->getButtonByName("UIWeaponsInventoryName")->disable();
	}

	//For Garage Inventory UI
	if (currentCar == nullptr) //If there is no Garage Inventory
	{
		Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIInventoryBackground")->enable();
		for (int i = 0; i < CAR_COUNT; i++)
		{
			Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlot" + std::to_string(i + 1))->disable();
			Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlotCurrent" + std::to_string(i + 1))->disable();
		}
		Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrent")->disable();
		Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrentBorder")->disable();
		Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryName")->disable();
	}
	else if (garageInv.size() > 0) //If item is owned at least once, then display
	{
		Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIInventoryBackground")->enable();
		for (int i = 0; i < garageInv.size(); i++)
		{
			switch (garageInv[i]->getCarType())
			{
			case SEDAN:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_SEDAN);
				break;
			case SEDAN_SPORTS:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_SEDAN_SPORTS);
				break;
			case RACER:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_RACER);
				break;
			case HATCH_BACK_SPORTS:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_HATCH_BACK_SPORTS);
				break;
			case TRACTOR_SHOVEL:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_TRACTOR_SHOVEL);
				break;
			case TRUCK:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_TRUCK);
				break;
			case VAN:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlot" + std::to_string(i + 1))->setQuadImage(UI_VAN);
				break;
			}
			Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlot" + std::to_string(i + 1))->enable();
		}

		for (int i = 0; i < garageInv.size(); i++)
		{
			//Disable all non-current item UIs
			if (currentCar->getCarType() == garageInv[i]->getCarType())
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlotCurrent" + std::to_string(i + 1))->enable();
			else
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlotCurrent" + std::to_string(i + 1))->disable();

			switch (currentCar->getCarType())
			{
			case SEDAN:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrent")->setQuadImage(UI_SEDAN);
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryName")->setText("Sedan\nSpeed:\n" + currentCar->getMaxCarSpd(SEDAN) + "km/h");
				break;
			case SEDAN_SPORTS:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrent")->setQuadImage(UI_SEDAN_SPORTS);
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryName")->setText("Sedan Sports\nSpeed:\n" + currentCar->getMaxCarSpd(SEDAN_SPORTS) + "km/h");
				break;
			case RACER:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrent")->setQuadImage(UI_RACER);
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryName")->setText("Racer\nSpeed:\n" + currentCar->getMaxCarSpd(RACER) + "km/h");
				break;
			case HATCH_BACK_SPORTS:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrent")->setQuadImage(UI_HATCH_BACK_SPORTS);
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryName")->setText("Hatch Back\nSpeed:\n" + currentCar->getMaxCarSpd(HATCH_BACK_SPORTS) + "km/h");
				break;
			case TRACTOR_SHOVEL:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrent")->setQuadImage(UI_TRACTOR_SHOVEL);
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryName")->setText("Tractor Shovel\nSpeed:\n" + currentCar->getMaxCarSpd(TRACTOR_SHOVEL) + "km/h");
				break;
			case TRUCK:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrent")->setQuadImage(UI_TRUCK);
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryName")->setText("Truck\nSpeed:\n" + currentCar->getMaxCarSpd(TRUCK) + "km/h");
				break;
			case VAN:
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrent")->setQuadImage(UI_VAN);
				Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryName")->setText("Van\nSpeed:\n" + currentCar->getMaxCarSpd(VAN) + "km/h");
				break;
			}
			Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrent")->enable();
			Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrentBorder")->enable();
			Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryName")->enable();
		}

	}
	else //Do not display any item slots as no items owned
	{
		Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIInventoryBackground")->enable();
		for (int i = 0; i < CAR_COUNT; i++)
		{
			Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlot" + std::to_string(i + 1))->disable();
			Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventorySlotCurrent" + std::to_string(i + 1))->disable();
		}
		Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrent")->disable();
		Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryCurrentBorder")->disable();
		Game::uiManager.getbManagerArray(UI_GARAGE_INVENTORY)->getButtonByName("UIGarageInventoryName")->disable();
	}

	toggleTimer += dt;
	if (false && Application::IsKeyPressed('E')) //pick up weapon
		addWeap(PISTOL);
	if (false && Application::IsKeyPressed('F')) //pick up weapon
	{
		addItem(BURGER, 1);
		addItem(CORN, 2);
		addItem(EGGPLANT, 3);

		addWeap(SILENCER);

		addCar(SEDAN);
		addCar(HATCH_BACK_SPORTS);
		addCar(RACER);
		addCar(TRACTOR_SHOVEL);

	}
	if (GetAsyncKeyState('1') & 0x0001)
		switchWeapon(0); //weapon slot 1

	if (GetAsyncKeyState('2') & 0x0001)
		switchWeapon(1); //weapon slot 2

	if (toggleTimer > 1 && Application::IsKeyPressed('O')) //delete equipped weapon
	{
		toggleTimer = 0;
		deleteWeapon(getActiveWeapon()->getWeaponType());
	}
	if (toggleTimer > 1 && Application::IsKeyPressed('T')) //delete equipped weapon
	{
		toggleTimer = 0;
		toggleItem();
	}
}

/******************************************************************************/
/*!
\brief returns the current item's amount.
*/
/******************************************************************************/
int Inventory::getCurrentItemAmt()
{
	if (itemInventory != nullptr)
		return itemInventory->getCurrentItemAmt();
}

/******************************************************************************/
/*!
\brief returns the active weapon.
*/
/******************************************************************************/
Weapon* Inventory::getActiveWeapon()
{
	if (weaponInv->getActiveWeapon() == nullptr || weaponInv == nullptr || weaponInv->getWeaponList().empty())
		return nullptr;
	else
		return weaponInv->getActiveWeapon();
}

/******************************************************************************/
/*!
\brief returns the current weapon's type.
*/
/******************************************************************************/
WEAPON_TYPE Inventory::getCurrentWeaponType()
{
	if (weaponInv != nullptr)
		return weaponInv->getActiveWeapon()->getWeaponType();
}