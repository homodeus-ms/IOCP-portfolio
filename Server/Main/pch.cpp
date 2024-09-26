#include "pch.h"

ostream& operator<<(ostream& os, const VectorInt& v)
{
	os << "( " << v.x << ", " << v.y << " )";
	return os;
}