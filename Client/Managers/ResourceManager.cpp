#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "Flipbook.h"
#include "Sound.h"
#include "Tilemap.h"

void ResourceManager::Init(HWND hwnd, fs::path resourcePath)
{
	_resourcePath = resourcePath;
	_hwnd = hwnd;
}

Texture* ResourceManager::LoadTexture(const wstring& nameKey, const wstring& path, uint32 transparent)
{
	if (_textures.find(nameKey) != _textures.end())
		return _textures[nameKey];

	fs::path fullPath = _resourcePath / path;

	Texture* texture = Xnew<Texture>();
	texture->LoadBmp(_hwnd, fullPath);
	texture->SetTransparent(transparent);
	_textures[nameKey] = texture;

	return texture;
}

Texture* ResourceManager::GetTexture(const wstring& nameKey)
{
	if (_textures.find(nameKey) == _textures.end())
		CRASH("No Texture");

	return _textures[nameKey];
}

Sprite* ResourceManager::CreateSprite(const wstring& nameKey, Texture* texture, uint32 x, uint32 y, uint32 endX, uint32 endY)
{
	if (_sprites.find(nameKey) != _sprites.end())
		return _sprites[nameKey];

	
	if (endX == 0)
		endX = texture->GetSize().x;
	if (endY == 0)
		endY = texture->GetSize().y;

	Sprite* sprite = Xnew<Sprite>(texture, x, y, endX, endY);
	_sprites[nameKey] = sprite;
	
	return sprite;
}

Sprite* ResourceManager::GetSprite(const wstring& nameKey)
{
	if (_sprites.find(nameKey) == _sprites.end())
		CRASH("No Sprite");

	return _sprites[nameKey];
}

Flipbook* ResourceManager::CreateFlipbook(const wstring& nameKey)
{
	if (_flipbooks.find(nameKey) != _flipbooks.end())
		return _flipbooks[nameKey];

	Flipbook* flipbook = Xnew<Flipbook>();
	_flipbooks[nameKey] = flipbook;

	return flipbook;
}

Flipbook* ResourceManager::GetFlipbook(const wstring& nameKey)
{
	if (_flipbooks.find(nameKey) == _flipbooks.end())
		CRASH("No Flipbook")

	return _flipbooks[nameKey];
}

Sound* ResourceManager::GetSound(const wstring& key)
{
	if (!_sounds.contains(key))
		return nullptr;
	return _sounds[key];
}

Sound* ResourceManager::LoadSound(const wstring& key, const wstring& path)
{
	if (_sounds.find(key) != _sounds.end())
		return _sounds[key];

	fs::path fullPath = _resourcePath / path;
	
	Sound* sound = Xnew<Sound>();
	sound->LoadWave(fullPath);
	_sounds[key] = sound;

	return sound;
}

Tilemap* ResourceManager::CreateTilemap(const wstring& key, const VectorInt mapSize, const uint32 tileSize)
{
	if (_tilemaps.contains(key))
		return _tilemaps[key];

	Tilemap* tilemap = Xnew<Tilemap>(mapSize, tileSize);
	_tilemaps[key] = tilemap;

	return tilemap;
}

void ResourceManager::SaveTilemap(const wstring& key, const wstring& path)
{
	Tilemap* tilemap = GetTilemap(key);
	fs::path fullpath = _resourcePath / path;
	tilemap->SaveFile(fullpath);
}

Tilemap* ResourceManager::LoadTilemap(const wstring& key, const wstring& path)
{
	Tilemap* tilemap = nullptr;

	if (_tilemaps.find(key) == _tilemaps.end())
		_tilemaps[key] = Xnew<Tilemap>(VectorInt{0, 0}, 0);

	tilemap = _tilemaps[key];

	fs::path fullPath = _resourcePath / path;
	tilemap->LoadFile(fullPath);

	return tilemap;
	
}

Tilemap* ResourceManager::GetTilemap(const wstring& key)
{
	if (!_tilemaps.contains(key))
		return nullptr;

	return _tilemaps[key];
}

void ResourceManager::Clear()
{
	for (auto& s : _sprites)
		SAFE_XRELEASE(s.second);
	for (auto& t : _textures)
		SAFE_XRELEASE(t.second);
	for (auto& f : _flipbooks)
		SAFE_XRELEASE(f.second);
	for (auto& ptr : _sounds)
		SAFE_XRELEASE(ptr.second);
	for (auto& ptr : _tilemaps)
		SAFE_XRELEASE(ptr.second);

	_textures.clear();
	_sprites.clear();
	_flipbooks.clear();
	_sounds.clear();
	_tilemaps.clear();
}
