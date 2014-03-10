#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Item.h"
#include "player.h"
#include <list>
#include <vector>
#include "OverlayImage.h"
#include "GameWorld.h"
#include "Game.h"
#include "GameGraphics.h"
#include "PhysicalProperties.h"
#include "Game.h"

TextureManager *guiTextureManager;
Item *empty = new Item();


Player::Player()
{
	hp = 6;			//6 characters, each time you get hit, a character dies
	inventoryIndex=0;
	HPBar = new list<OverlayImage*>();
	HPImages = new vector<OverlayImage*>();
	InventoryBar = new list<OverlayImage*>();
	InventoryImages = new vector<OverlayImage*>();
	Inventory = new vector<Item*>();
	dummyItem = new Item();
	dummyItem->damage=0;
	dummyItem->name="empty";

	//Initialize InventoryImage properties
	for (int i = 0; i <15;i++)
	{
		OverlayImage *inventoryItem = new OverlayImage();
		inventoryItem->alpha = 255;
		inventoryItem->height = 50;
		inventoryItem->width = 50;
		inventoryItem->x=(290 + 8 + (i%5)*95);
		inventoryItem->y=(450 + 8 + (i/5)*90);
		inventoryItem->z=0;
		InventoryImages->push_back(inventoryItem);
		Inventory->push_back(dummyItem);
	}


	EquipBar = new list<OverlayImage*>();
	//make the atkItem and mscItem icon to render
	OverlayImage *atkItem = new OverlayImage();
	atkItem->alpha=255;
	atkItem->height=50;
	atkItem->width=50;
	atkItem->x=710;
	atkItem->y=10;
	
	
	OverlayImage *mscItem = new OverlayImage();
	mscItem->alpha=255;
	mscItem->height=50;
	mscItem->width=50;
	mscItem->x=920;
	mscItem->y=10;
	EquipBar->push_back(atkItem);
	EquipBar->push_back(mscItem);

	EquipImages = new vector<OverlayImage*>();
	EquipImages->push_back(atkItem);
	EquipImages->push_back(mscItem);
}

void Player::addHPIcon(OverlayImage *imageToAdd)
{
	HPImages->push_back(imageToAdd);
}

void Player::addEquipIcon(OverlayImage *imageToAdd)
{
	EquipImages->push_back(imageToAdd);
}


void Player::resetHP(vector<OverlayImage*> *HPIcons)
{
	HPBar->clear();
	for (int i = 0; i <HPIcons->size(); i++)
	{
		OverlayImage *HPIcon = HPIcons->at(i);
		HPBar->push_back(HPIcon);
	}
	attackEquipped = empty;
	mscEquipped = empty;
	Inventory->clear();
	for (int i = 0; i<15; i++)
	{
		Inventory->push_back(dummyItem);
	}
}

void Player::updateEquipBar(vector<OverlayImage*> *EquipIcons)
{
	EquipBar->clear();
	for (int i = 0; i <EquipIcons->size(); i++)
	{
		OverlayImage *EquipIcon = EquipIcons->at(i);
		EquipBar->push_back(EquipIcon);
	}
}

void Player::updateInventoryBar(vector<Item*> *Inventorylist)
{
	InventoryBar->clear();
	for (int i = 0; i <Inventory->size(); i++)
	{
		Item *itemToRender = Inventorylist->at(i);
		OverlayImage *InventoryIcon = InventoryImages->at(i);
		InventoryIcon->imageID = itemToRender->imageID;
		InventoryBar->push_back(InventoryIcon);
	}
}

void Player::decreaseHP()
{
	HPBar->pop_back();
	hp--;
}
void Player::setMscEquipped(Item *item)
{ 
	mscEquipped = item;
	EquipImages->at(1)->imageID = item->imageID;
	updateInventoryBar(Inventory);
	updateEquipBar(EquipImages);
}

void Player::setAtkEquipped(Item *item)
{
	attackEquipped = item;
	EquipImages->at(0)->imageID = item->imageID;
	updateInventoryBar(Inventory);
	updateEquipBar(EquipImages);
}

bool Player::hasItem(string s)
{
	for (int i = 0; i<Inventory->size();i++)
	{
		if (Inventory->at(i)->name.compare(s)==0)
		{
			return true;
		}
	}
	return false;
}