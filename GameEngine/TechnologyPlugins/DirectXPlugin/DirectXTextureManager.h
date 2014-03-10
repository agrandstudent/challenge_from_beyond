/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DirectXTextureManager.h

	This class is a TextureManager class that does all 
	texture management using DirectX.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "TextureManager.h"

class DirectXTextureManager:TextureManager
{
private:
	// HERE ARE OUR TEXTURES AND THEIR FILE PATHS/NAMES
	map<wchar_t*, LPDIRECT3DTEXTURE9> *textures;

public:
	// INLINED ACCESSOR METHODS
	LPDIRECT3DTEXTURE9 getTexture(wchar_t *fileName)
	{
		return textures->find(fileName)->second;
	}

	// METHODS DEFINED IN DirectXTextureManager.cpp
	DirectXTextureManager();
	~DirectXTextureManager();
	void				clear();
	LPDIRECT3DTEXTURE9	getTexture(int id);
	int					loadTexture(wchar_t *key);
	void				reloadAllTextures();
};