#ifndef _BODY_H_
#define _BODY_H_

#include "Geometry.h"
#include "types.h"


class foot
{
	private:
		VecPosition root, end, midel;
		double h,
			ang1;
	public :
		foot (double d = 1.99 );
		~foot () {};
		bool setRoot(const VecPosition& vec);
		bool setMidel();
		VecPosition getMidel();
		VecPosition getRoot();
		VecPosition getEnd();
		double getLeg(int i);
		bool	setLeg(double );
		bool isValid();
		bool setEnd(const VecPosition&	vec);
};

//______________________________________
class body
{
	private:
		foot* leftFoot;
		foot* rightFoot;

	public :
		body(double h = 1.99);
		~body();
		bool setRightFoot(const VecPosition&);
		bool setLeftFoot(const VecPosition&);
		foot* getLeftFoot();
		foot* getRightFoot();
		bool setFoots(const VecPosition&,const VecPosition&);
		bool isValid();
};

#endif
