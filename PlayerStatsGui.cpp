#pragma once
#include "stdafx.h"
#include "Button.h"
#include "Game.h"
#include "GameGraphics.h"
#include "OverlayImage.h"
#include "RenderList.h"
#include "ScreenGUI.h"
#include "PlayerStatsGui.h"
#include <list>
#include <vector>

PlayerStatsGui::PlayerStatsGui()
{
	HPBar = new list<OverlayImage*>();
	EquipBar = new list<OverlayImage*>();
	InventoryBar = new list<OverlayImage*>();
}


PlayerStatsGui::~PlayerStatsGui()
{
	delete HPBar;
	delete EquipBar;
	delete InventoryBar;
}



void PlayerStatsGui::addRenderItemsToRenderList(RenderList *renderList)
{

RenderItem renderItemToAdd;

	// FIRST ADD THE OVERLAY IMAGES TO THE RENDER LIST
	// FOR THIS WE'LL USE AN ITERATOR
	list<OverlayImage*>::iterator imageIterator;
	imageIterator = overlayImages->begin();
	while (imageIterator != overlayImages->end())
	{
		// GET THE OverlayImage WE WANT TO ADD
		// FROM THE ITERATOR
		OverlayImage *image = (*imageIterator);

		// ADD IT TO THE LIST
		renderList->addRenderItem(	image->imageID,
									image->x,
									image->y,
									image->z,
									image->alpha,
									image->width,
									image->height	);

		// ADVANCE THE ITERATOR
		imageIterator++;
	}
	//Draw HP icons
	list<OverlayImage*>::iterator imageIterator2;
	imageIterator2 = HPBar->begin();
	while (imageIterator2 != HPBar->end())
	{
		// GET THE OverlayImage WE WANT TO ADD
		// FROM THE ITERATOR
		OverlayImage *image = (*imageIterator2);

		// ADD IT TO THE LIST
		renderList->addRenderItem(	image->imageID,
									image->x,
									image->y,
									image->z,
									image->alpha,
									image->width,
									image->height	);

		// ADVANCE THE ITERATOR
		imageIterator2++;
	}
	//Draw Equipment
	list<OverlayImage*>::iterator imageIterator3;
	imageIterator3 = EquipBar->begin();
	while (imageIterator3 != EquipBar->end())
	{
		// GET THE OverlayImage WE WANT TO ADD
		// FROM THE ITERATOR
		OverlayImage *image = (*imageIterator3);

		// ADD IT TO THE LIST
		renderList->addRenderItem(	image->imageID,
									image->x,
									image->y,
									image->z,
									image->alpha,
									image->width,
									image->height	);

		// ADVANCE THE ITERATOR
		imageIterator3++;
	}
	list<OverlayImage*>::iterator imageIterator4;
	imageIterator4 = InventoryBar->begin();
	while (imageIterator4 != InventoryBar->end())
	{
		// GET THE OverlayImage WE WANT TO ADD
		// FROM THE ITERATOR
		OverlayImage *image = (*imageIterator4);

		// ADD IT TO THE LIST
		renderList->addRenderItem(	image->imageID,
									image->x,
									image->y,
									image->z,
									image->alpha,
									image->width,
									image->height	);

		// ADVANCE THE ITERATOR
		imageIterator4++;
	}


	// NOW ADD THE BUTTONS TO THE RENDER LIST
	int imageId;

	// WE'LL NEED A NEW ITERATOR
	list<Button*>::iterator buttonIterator;
	buttonIterator = buttons->begin();
	list<Button*>::iterator end = buttons->end();
	while(buttonIterator != end)
	{
		// GET THE Button WE WANT TO ADD
		// FROM THE ITERATOR
		Button *buttonToRender = (*buttonIterator);

		// DETERMINE WHICH Button IMAGE TO USE
		if (buttonToRender->isMouseOver())
			imageId = buttonToRender->getMouseOverTextureID();
		else
			imageId = buttonToRender->getNormalTextureID();

		// ADD IT TO THE LIST
		renderList->addRenderItem(	imageId,
									buttonToRender->getX(),
									buttonToRender->getY(),
									buttonToRender->getZ(),
									buttonToRender->getAlpha(),
									buttonToRender->getWidth(),
									buttonToRender->getHeight());

		// ADVANCE THE ITERATOR
		buttonIterator++;
	}
}