#include "Physics3D.h"



Physics3D::Physics3D()
{}

Physics3D::Physics3D(Transformer* trans)
{
	setTransformer(trans);
}

Physics3D::~Physics3D()
{}

void Physics3D::setTransformer(Transformer* trans)
{
	transform = trans;
}

void Physics3D::setGravityMagnitude(float mag)
{
	magnitude = mag;
}

void Physics3D::setGravityDirection(Coord3D dir)
{
	direction = dir;
}

void Physics3D::update()
{
	static Coord3D tmpDer;

	tmpDer *= direction;

	transform->translateBy(direction.x, direction.y, direction.z);
}