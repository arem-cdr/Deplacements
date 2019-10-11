#include "stdafx.h"
#include "RenderObject.h"

RenderObject::RenderObject() : m_pos(0, 0, 0)
{
}

RenderObject::RenderObject(Point pos) : m_pos(pos)
{
}

RenderObject::~RenderObject()
{
}

Point RenderObject::getPos() const
{
	return m_pos;
}
