#include "types.h"

void Joint::set (double a , double r)
{
	axis = a;
	rate = r;
}

double Joint::getAxis ( )
{
	return axis;
}

double Joint::getRate ( )
{
	return rate;
}


//=======================Torso ====================

Torso::Torso(double x,double y,double z)
{
	angX = x;
	angY = y;
	angZ = z;
}

double	Torso::getXDirection()
{
	return angX;
}

double	Torso::getYDirection()
{
	return angY;
}

double 	Torso::getZDirection()
{
	return angZ;
}

bool 	Torso::setTorso(Torso t)
{
	return setTorso(t.getXDirection(),t.getYDirection(),t.getZDirection());
}
bool 	Torso::setTorso(double x,double y,double z)
{
	setXDirection(x);
	setYDirection(y);
	setZDirection(z);
	return true;
}
bool	Torso::setXDirection(double x)
{
	angX = x;
	return true;
}
bool	Torso::setYDirection(double y)
{
	angY = y;
	return true;
}

bool	Torso::setZDirection(double z)
{
	angZ = z;
	return true;
}


Torso	Torso::operator + ( Torso &p )
{
  return ( Torso( angX + p.getXDirection(), angY + p.getYDirection(),
	 angZ+ p.getZDirection() ) );
}

Torso	Torso::operator - ( Torso &p )
{
  return ( Torso( angX - p.getXDirection(), angY - p.getYDirection(),
	 angZ - p.getZDirection() ) );
}

ostream& operator << (ostream & output ,Torso t)
{
	output <<"( "<<t.getXDirection()<<" , "<<t.getYDirection()
	<<" , "<<t.getZDirection()<<" )" ;
	return output ;
}

double	Torso::getMagnitude()
{

	return ( sqrt( angX * angX + angY * angY + angZ * angZ )); 

}
