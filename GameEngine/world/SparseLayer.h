/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SparseLayer.h

	A SparseLayer manages a sparse background, which is
	number of images that are placed in a world with
	space in between them. They are not packed into a grid
	and can be in any random arrangement. 
*/
#pragma once
#include "resource.h"
#include "stdafx.h"
#include "OverlayImage.h"
#include "Viewport.h"
#include "WorldLayer.h"
#include <vector>

// THIS CLASS IS-A WorldLayer

class SparseLayer : public WorldLayer
{
private:
	// SPARSE TILES NEED NOT BE IN ANY ORDER OR PATTERNED ARRANGEMENT
	vector<OverlayImage*> *sparseTiles;

public:
	// INLINED ACCESSOR METHOD
	OverlayImage* getTile(int index)	{	return sparseTiles->at(index);	}
	int getNumberOfSparseTiles()		{   return sparseTiles->size();		}
	vector<OverlayImage*>* getSparseTiles()		{   return sparseTiles;		}

	// INLINED MUTATOR METHOD
	void addTile(OverlayImage *initTile)
	{	sparseTiles->push_back(initTile);	}
	void SparseLayer::setTile(OverlayImage *initTile, int index)
	{	sparseTiles->at(index) = initTile;	}

	// METHODS DEFINED IN SparseLayer.cpp
	SparseLayer();
	~SparseLayer();
	void			addRenderItemsToRenderList(RenderList *renderList,	Viewport *viewport);
};