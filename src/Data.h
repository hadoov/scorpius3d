#ifndef _DATA_H_
#define _DATA_H_

#include "Geometry.h"
#include "types.h"

class Data
{
private:
	
	VecPosition vec[20];
	int period;

public:
	Data();
	~Data();
	Data& operator=(Data& d);
	int	getPeriod();
	VecPosition getPos(int i);
	bool fin(int );
	double getSigmaX();
};
//______________________________________
class step:Data
{
	private:
		int stepIterator;
		bool e,isAct;
	
	public:
		step ( int fileID );
		~step() {};
		VecPosition getNextPosition();
		bool isFinished();
		bool reset(bool b);
		bool active();
		bool sleep();
		double getSigmaX();
		bool isActive();
		bool isMiddel();
};

#endif
