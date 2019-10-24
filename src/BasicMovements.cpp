# include "BasicMovements.h"
# include <iostream>

using namespace std ;

BasicMovements::BasicMovements()
{
	int i,j;
	for (i=0 ; i<6 ; i++)
		for (j=0 ; j<2 ; j++)
		{
			flast[i][j] = EPSILON;
			hlast[i][j] = EPSILON;
			fAng[i][j] = EPSILON;
			hAng[i][j] = EPSILON;
		}
	beamTime = false;
}

void    BasicMovements::setWorldModel	( WorldModel* wm) 
{
	this->WM = wm;	
	stringstream ss;
	ss<<"../Logs/"<<WM->getPlayerNumber()<<".txt";
	log = new ofstream(ss.str().c_str()); 

}

void    BasicMovements::beam (VecPosition vec, double angle)
{
	beamPosition = vec;
	beamPosition.setZ(angle);
	beamTime = true;

	
}

void	BasicMovements::beam (double x,double y,double phi)
{
	beamPosition.setX(x);
	beamPosition.setY(y);
	beamPosition.setZ(phi);
	beamTime = true;
	
	
}

bool	BasicMovements::printLog( string s)
{
	(* log) << s;
	return true;
}
bool	BasicMovements::printLog( double d)
{
	(* log) << d;
	return true;
}
bool	BasicMovements::printLog( char *c)
{
	(* log) << c;
	return true;
}


string  BasicMovements::getCommand ()
{

	stringstream ss;
	int i;
	if ( beamTime )
	{
		ss << "(beam " <<beamPosition.getX()<< " " <<beamPosition.getY() << " "
		<<beamPosition.getZ()<<")";

	}
	
	for (i=0 ; i<6 ; i++)
		for (int j=0 ; j<2 ; j++)
		{
			if(fabs(fAng[i][j]) > 6.5)
				fAng[i][j] = 6.5*sign(fAng[i][j]);
			fAng[i][j]  = (fAng[i][j]+ flast[i][j])/2; 
		}
		
	for( i = 0;i<4;i++)
	{
		ss << "(rae"<<i+1<<" "<<hAng[i][0]-EPSILON<<" )";
   		ss << "(lae"<<i+1<<" "<<hAng[i][1]-EPSILON<<" )";
	}
	for( i = 0;i<6;i++)
	{
		ss << "(rle"<<i+1<<" "<<fAng[i][0]-EPSILON<<" )";
		ss << "(lle"<<i+1<<" "<<fAng[i][1]-EPSILON<<" )";
	}	
	if(ss.str().empty())
		 ss << "( )";
	return ss.str();
}

void    BasicMovements::resetComponents()
{
	int i,j;
	for (i=0 ; i<6 ; i++)
		for (j=0 ; j<2 ; j++)
		{
			flast[i][j] = fAng[i][j];
			hlast[i][j] = hAng[i][j];
			fAng[i][j] = EPSILON;
			hAng[i][j] =EPSILON;
		}
	beamTime = false;
}
		
void 	BasicMovements::beam(VecPosition vec)
{
	beamPosition = vec;
	beamTime = true;
}

void	BasicMovements::rotateFoot(double ang,int no,bool side)
{
	if(side)
		fAng[no-1][0] = ang;
	else
		fAng[no-1][1] = ang;
}


void	BasicMovements::rotateHand(double ang ,int no,bool side)
{	
	if(side)
		hAng[no-1][0] = ang;
	else
		hAng[no-1][1] = ang;
}

void    BasicMovements::rotateLeftFoot(double ang ,int no)
{
	rotateFoot(ang,no,false);
}

void    BasicMovements::rotateRightFoot(double ang ,int no) 
{
	rotateFoot(ang,no,true);
}

void    BasicMovements::rotateLeftHand(double ang ,int no)
{
	rotateHand(ang,no,false);
}

void    BasicMovements::rotateRightHand(double ang ,int no)
{
	rotateHand(ang,no,true);
}

void	BasicMovements::rotateBothFoot(double ang,int no)
{
	rotateRightFoot(ang,no);
	rotateLeftFoot(ang,no);
}
void	BasicMovements::rotateBothHand(double ang ,int no)
{
	rotateHand(ang,no,false);
	rotateHand(ang,no,true);
}

bool	BasicMovements::setHand(double ang,int no,bool side,double rate,double error )
{
	if(fabs(ang-WM->getHand(no,side))<error)
		return true;
	
	if(fabs(ang-WM->getHand(no,side))>0.5)
		rotateHand(rate *(ang-WM->getHand(no,side)) /100.0 ,no, side);
	return false;
}

bool	BasicMovements::setFoot(double ang ,int no,bool side,double rate ,double error)
{
	if(fabs(ang-WM->getLeg(no,side))<error)
		return true;
	if(fabs(ang-WM->getLeg(no,side))>0.5)
		rotateFoot(rate *(ang - WM->getLeg(no,side)) /100.0 ,no, side);
	return false;
}

bool    BasicMovements::setLeftFoot(double ang ,int no,double rate ,double error)
{
	return setFoot(ang,no,false,rate,error);
}

bool    BasicMovements::setRightFoot(double ang ,int no,double rate ,double error) 
{
	return setFoot(ang,no,true,rate,error);
}

bool    BasicMovements::setLeftHand(double ang ,int no,double rate ,double error)
{
	return setHand(ang,no,false,rate,error);
}

bool    BasicMovements::setRightHand(double ang ,int no,double rate ,double error)
{
	return setHand(ang,no,true,rate,error);
}

bool	BasicMovements::setBothFoot(double ang,int no,double rate ,double error)
{
	 bool tag = setRightFoot(ang,no,rate,error);
	 tag = tag & setLeftFoot(ang,no,rate,error);
	return tag;
}
bool	BasicMovements::setBothHand(double ang ,int no,double rate ,double error)
{		
	int k = 1;
	if(no == 2 || no == 3 || no == 4) k=-1;
	bool tag = setRightHand(ang,no,rate,error);
	tag = tag & setLeftHand(k*ang,no,rate,error);
	return tag;
}
bool	BasicMovements::setAllFoot(double ang ,double rate ,double error)
{
	int i;
	bool setall = true;
	for(i=1;i<7;i++)
	{
		if(!setBothFoot(ang ,i,rate ,error))
			setall = false;
	}
	return setall;
}
bool	BasicMovements::setAllLeftFoot(double ang ,double rate ,double error)
{
	int i;
	bool setall = true;
	for(i=1;i<7;i++)
	{
		if(!setLeftFoot(ang ,i,rate ,error))
			setall = false;
	}
	return setall;
}
bool	BasicMovements::setAllRightFoot(double ang ,double rate ,double error)
{
	int i;
	bool setall = true;
	for(i=1;i<7;i++)
	{
		if(!setRightFoot(ang ,i,rate ,error))
			setall = false;
	}
	return setall;
}

bool	BasicMovements::setAllHand (double ang ,double rate ,double error)
{
	int i;
	bool setall = true;
	for(i=1;i<7;i++)
	{
		if(!setBothHand(ang ,i,rate ,error))
			setall = false;
	}
	return setall;
}
bool BasicMovements::setAll (double ang ,double rate ,double error)
{
	return ( setAllFoot(ang, rate, error) & setAllHand (ang, rate, error) );
}

void	BasicMovements::goHand(double ang,int no,bool side)
{
	double t = ang-WM->getHand(no,side);
	rotateHand(t ,no, side);
}

void	BasicMovements::goAll(double ang )
{
	int i;
	
	for(i=1;i<7;i++)
		goBothFoot(ang ,i);
	for(i=1;i<5;i++)
		goBothHand(ang ,i);
}	

void	BasicMovements::goFoot(double ang ,int no,bool side)
{
	double t = ang-WM->getLeg(no,side);
		
	rotateFoot(t ,no, side);
}

void    BasicMovements::goLeftFoot(double ang ,int no)
{
	goFoot(ang,no,false);
}

void    BasicMovements::goRightFoot(double ang ,int no) 
{
	goFoot(ang,no,true);
}

void    BasicMovements::goLeftHand(double ang ,int no)
{
	goHand(ang,no,false);
}

void    BasicMovements::goRightHand(double ang ,int no)
{
	goHand(ang,no,true);
}

void	BasicMovements::goBothFoot(double ang,int no)
{
	goRightFoot(ang,no);
	goLeftFoot(ang,no);
}
void	BasicMovements::goBothHand(double ang ,int no)
{		
	goHand(ang,no,false);
	goHand(ang,no,true);
}
