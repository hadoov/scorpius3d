#include "Data.h"
#include "Geometry.h"

Data::Data()
{
	period = 20;
}
//______________________________________
Data& Data::operator=(Data& d)
{
	return *this;
}

//______________________________________

VecPosition Data::getPos(int i)
{
	return vec[i];
}
//______________________________________
int Data::getPeriod()
{
	return period;
}

// -------------------------------------
bool Data::fin(int fileID)
{
	stringstream ss;
	ss <<"out" << fileID << ".txt";
	ifstream in(ss.str().c_str());
	if(!in)
		return false;
	char tbuffer[50];
	double x,y;
	in >> tbuffer >> period;
	for (int i=0 ;i<period ;i++)
	{
		in>>x>>y ;
		vec[i] = VecPosition(x,y);
	}
	return true;
		
}

//______________________________________
Data::~Data()
{
}

//______________________________________

double Data::getSigmaX()
{
	double sum = 0;
	for(int i = 0;i<getPeriod();i++)
		sum += getPos(i).getX();
	return sum;
}
//========================================

step::step(int fileID) : Data()
{
	stepIterator = 0;
	e = 0;
	if(!fin(fileID))
		cerr<<"Input Data File Error\n";
}

//______________________________________
VecPosition step::getNextPosition()
{
	if(!isActive())
		return VecPosition(0,0);
	VecPosition nvec = getPos(stepIterator);
	if(e == 1)
		nvec.setX(nvec.getX()/2);
	stepIterator++;
	return nvec;
}

//______________________________________
bool step::isFinished()
{
	return (stepIterator == getPeriod());
}
//______________________________________
bool step::reset(bool b)
{
	e = b;
	stepIterator = 0;
	return true;
}
//______________________________________
bool step::active()
{
	isAct = true;
	return true;
}
//______________________________________
bool step::sleep()
{
	isAct = false;
	return true;
}

double step::getSigmaX()
{
	Data::getSigmaX();	
}

bool step::isActive()
{
	return isAct;
}

bool step::isMiddel()
{
	return (stepIterator == getPeriod()/2)&& isActive();
}
