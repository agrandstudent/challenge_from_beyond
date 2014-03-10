#pragma once
#include "stdafx.h"
#include "Button.h"
#include "Game.h"
#include "GameGraphics.h"
#include "OverlayImage.h"
#include "RenderList.h"
#include "ScreenGUI.h"
#include <list>
#include <vector>

class PlayerStatsGui: public ScreenGUI
{
protected:
	list<OverlayImage*> *HPBar;
	list<OverlayImage*> *EquipBar;
	list<OverlayImage*> *InventoryBar;
public:
	PlayerStatsGui();
	~PlayerStatsGui();
	void setHPBar(list<OverlayImage*> *list)		{ HPBar = list;}
	void setEquipBar(list<OverlayImage*> *list)		{ EquipBar = list;}
	void setInventoryBar(list<OverlayImage*> *list)	{ InventoryBar = list;}
	void addRenderItemsToRenderList(RenderList *renderList);
};