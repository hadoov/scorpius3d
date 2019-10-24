#ifndef __BasicSkills_h__
#define __BasicSkills_h__
#include <iostream>
#include "HumanoidMovements.h"

using namespace std;
class BasicSkills : public HumanoidMovements
{
	public:
		JointSide shootSide;
		bool	rotateSide;
		int 	goBehindBallState;
		int 	goState;
		bool    kicktag;
		double  walkSpeed ;
		double  sp;
		bool	shootType;
	
	public:
		BasicSkills ( ) : HumanoidMovements ( ) {

			goBehindBallState = 1;
			goState = 1;
			walkSpeed = 0.6;
			};
		bool	resetStates	();
		bool 	Shoot 		();
		bool    goBehindBall(VecPosition target);
		bool    goToPos		(VecPosition target,double error ,double ,double);
		bool    newGoToPos		(VecPosition target,double error ,
							double maxSpeed ,double minSpeed );
		bool 	goBehindBallGerman(VecPosition target);
		bool	diveGoali 	(double t );
		bool 	delayAct	(double t);
		bool	StandUp		();
		bool	GoalKeeperAction();
		bool	goToBallPos	();
		bool	fastStandUp	(bool );
		bool	goBehindPoint(VecPosition target,VecPosition );
		bool	rotateTo	(VecPosition target,double error);
		bool	walk		(int i);
	
};
#endif
