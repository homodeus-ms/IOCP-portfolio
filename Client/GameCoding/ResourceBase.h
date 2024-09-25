#pragma once
class ResourceBase
{
public:
	ResourceBase() {}
	~ResourceBase() {}

	virtual void LoadFile(const wstring& path);
	virtual void SaveFile(const wstring& path);

};

