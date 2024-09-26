#pragma once

class ResourceBase;
class Texture;
class Sprite;
class Flipbook;
class Sound;
class UI;
class Tilemap;

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);
	
public:
	~ResourceManager() { Clear(); }

	void Init(HWND hwnd, fs::path resourcePath);

	Texture* LoadTexture(const wstring& nameKey, const wstring& path, uint32 transparent = RGB(255, 0, 255));
	Texture* GetTexture(const wstring& nameKey);

	Sprite* CreateSprite(const wstring& nameKey, Texture* texture, uint32 x = 0, uint32 y = 0, uint32 endX = 0, uint32 endY = 0);
	Sprite* GetSprite(const wstring& nameKey);

	Flipbook* CreateFlipbook(const wstring& nameKey);
	Flipbook* GetFlipbook(const wstring& nameKey);

	Sound* GetSound(const wstring& key);
	Sound* LoadSound(const wstring& key, const wstring& path);

	Tilemap* CreateTilemap(const wstring& key, const VectorInt mapSize, const uint32 tileSize);
	void SaveTilemap(const wstring& key, const wstring& path);
	Tilemap* LoadTilemap(const wstring& key, const wstring& path);
	Tilemap* GetTilemap(const wstring& key);

	void Clear();

private:
	HWND _hwnd = nullptr;
	fs::path _resourcePath = {};

	HashMap<wstring, Texture*> _textures;
	HashMap<wstring, Sprite*> _sprites;
	HashMap<wstring, Flipbook*> _flipbooks;
	HashMap<wstring, Sound*> _sounds;
	HashMap<wstring, Tilemap*> _tilemaps;
	
};

