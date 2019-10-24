#include "Body.h"

foot::foot(double d )
{
	h = d;
}

//______________________________________
bool foot::isValid()
{
	bool t = true;
	t = t &(getLeg(3) > MIN_3);
	t = t &(getLeg(4) > MIN_4);
	t = t &(getLeg(5) > MIN_5);
	t = t &(getLeg(3) < MAX_3);
	t = t &(getLeg(4) < MAX_4);
	t = t &(getLeg(5) < MAX_5);
	return t;
}

//______________________________________
VecPosition foot::getMidel()
{
	return midel;
}

//______________________________________
VecPosition foot::getRoot()
{
	return root;
}

//______________________________________
double foot::getLeg(int i)
{
	VecPosition vec1,vec2;
	double d = 0;
	switch(i)
	{
		case 1:
			d = ang1;
		break;
		case 3:
			vec1 = getRoot();
			vec2 = getMidel();
			d = (vec2 - vec1).getPhiDirection();
			d += 90;
			break;
		case 4:
			vec1 = getEnd();
			vec2 = getMidel();
			d = (vec2 - vec1).getPhiDirection();
			d -= 90;
			d = -d;
			vec1 = getRoot();
			vec2 = getMidel();
			d += (vec2 - vec1).getPhiDirection();
			d += 90;
			d = -d;
			break;
		case 5:
			vec1 = getEnd();
			vec2 = getMidel();
			d = (vec2 - vec1).getPhiDirection();
			d -= 90;
			d = -d;
			break;
		default:
			d =0;
			break;
	}
	return d;
}

//______________________________________
VecPosition foot::getEnd()
{
	return end;
}

//______________________________________
bool foot::setEnd(const VecPosition& vec)
{
	end = vec;
	return true;
}

bool foot::setLeg(double d)
{
	ang1 = d;
	return true;
}

//______________________________________
bool foot::setMidel()
{
	Circle cEnd(end,1);
	Circle cRoot(root,1);
	VecPosition p1,p2;
	//p1 = new VecPosition();
	//p2 = new VecPosition();
	int i = cEnd.getIntersectionPoints(cRoot,p1,p2);
	if(i == 2)
	{
		if(p1.getX()<p2.getX())
		{
			midel.setX(p2.getX());
			midel.setY(p2.getY());
		}
		else
		{
			midel.setX(p1.getX());
			midel.setY(p1.getY());
		}
	}
	else
	{
		cout << "Can not find foot error\n";
		return false;
	}
	return true;
}

//______________________________________
bool foot::setRoot(const VecPosition& vec)
{
	VecPosition vec1,vec2 ;
	if(vec.getX()> end.getX())
	{
		vec2 = vec;
		vec1 = end;
	}
	else
	{
		vec1 = vec;
		vec2 = end;
	}
	double a = 0.52;
	root = VecPosition((a*vec2.getX() + (1-a)*vec1.getX()),h);
	return true;
}

//______________________________________
//______________________________________
//______________________________________

//______________________________________
body::body(double h  )
{
	leftFoot = new foot(h);
	rightFoot = new foot(h);


}

//______________________________________
bool body::setLeftFoot(const VecPosition& vec)
{
	leftFoot->setEnd(vec);
	return true;
}

//______________________________________
bool body::setRightFoot(const VecPosition& vec)
{
	rightFoot->setEnd(vec);
	return true;
}

//______________________________________
foot *body::getLeftFoot()
{
	return leftFoot;
}

//______________________________________
foot *body::getRightFoot()
{
	return rightFoot;
}

//______________________________________
bool body::setFoots(const VecPosition& vec1,const VecPosition& vec2)
{
	setRightFoot(vec1);
	setLeftFoot(vec2);
	leftFoot->setRoot(vec1);
	rightFoot->setRoot(vec2);
	return (leftFoot->setMidel()& rightFoot->setMidel ());
	
}

//______________________________________
bool body::isValid()
{
	return (rightFoot->isValid() && leftFoot->isValid());
}

//______________________________________


body::~body()
{
	delete leftFoot;
	delete rightFoot;
}


//______________________________________
//______________________________________
//______________________________________
