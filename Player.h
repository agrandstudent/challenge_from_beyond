#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Item.h"
#include <list>
#include <vector>
#include "OverlayImage.h"

class Player
{
protected:
	int hp;
	int inventoryIndex;
	string helpText;
	Item *attackEquipped;
	Item *mscEquipped;
	list<OverlayImage*> *HPBar;
	vector<OverlayImage*> *HPImages;
	list<OverlayImage*> *EquipBar;
	vector<OverlayImage*> *EquipImages;
	list<OverlayImage*> *InventoryBar;
	vector<OverlayImage*> *InventoryImages;
	vector<Item*> *Inventory;
	Item *dummyItem;


public:
	Player();
	~Player();

	// INLINED METHODS
	int getHP()					{ return hp;					}
	Item* getAtkEquipped()		{ return attackEquipped;		}
	Item* getMscEquipped()		{ return mscEquipped;			}
	vector<OverlayImage*>* getHPImages()		{ return HPImages;}
	list<OverlayImage*>* getHPBar()				{ return HPBar;	}
	vector<OverlayImage*>* getEquipImages()		{ return EquipImages;}
	list<OverlayImage*>* getEquipBar()				{ return EquipBar;	}
	vector<Item*>* getInventoryImages()		{ return Inventory;}
	list<OverlayImage*>* getInventoryBar()				{ return InventoryBar;	}

	//MUTATOR METHODS
	void setHP(int newHP)
	{
		hp = newHP;
	}
	void increaseInventoryIndex()	{	inventoryIndex++;}
	int getInventoryIndex()			{return inventoryIndex;}
	void resetInventoryIndex()		{ inventoryIndex = 0;}
	Item* getDummyItem()			{ return dummyItem;}
	void setInventoryImage(int i, int ID)	{Inventory->at(i)->imageID = ID;}
	void decreaseHP();
	void	addHPIcon(OverlayImage *imageToAdd);
	void    resetHP(vector<OverlayImage*> *HPIcons);
	void	addEquipIcon(OverlayImage *imageToAdd);
	void setAtkEquipped(Item *item);
	void setMscEquipped(Item *item);
	void updateEquipBar(vector<OverlayImage*> *HPIcons);
	void updateInventoryBar(vector<Item*> *Inventory);
	bool hasItem(string s);

};