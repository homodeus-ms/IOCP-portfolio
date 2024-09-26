#include "pch.h"
#include "Component.h"



Component::Component()
{
}

Component::~Component()
{
	::DeleteObject(myBrush);
}


void Component::BeginPlay()
{
}

void Component::TickComponent()
{
}

void Component::Render(HDC hdc)
{
	

}
