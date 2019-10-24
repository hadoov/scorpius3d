#ifndef __Humanoid_Movements_h__
#define __Humanoid_Movements_h__
#include "BasicMovements.h"
#include "Data.h"
#include "Body.h"

class HumanoidMovements : public BasicMovements
{
	
	private:
	protected :
		int shootState;
		int walkState;
		float delay;
		int standState;
		int diveState;
		int StandUpState;
		double lastDelayTime;
		double diveTime;
		bool onDelay;
		int shootPower ;
		bool divetag;
		double standUpTime ;
///////////////////
		int fileID;
		step *left,	*right;
		body *Body;
		VecPosition lvr,lvl;
	

		
	
	public:
		HumanoidMovements ( );		
		~HumanoidMovements(){ delete Body; delete right ; delete left;};
		bool 		shoot3 (JointSide side , double kickDistan = 5);
		bool 		shoot (JointSide side);
		bool		goToRest();
		bool		newWalk(double , double);
		bool		backWalk();
		bool		standup(bool);
		bool 		driveWalk();
		bool 		driveShoot();
		bool		delay_act(double t);
		
		bool		shootBefore(JointSide side, double kickDistan);
		bool		shootFinal(JointSide side, double kickDistan);
		bool 		backJamping();
		bool		standUpFront();
		bool		standUpBack();
		bool		dive(JointSide side, double t);
////////////////////////////////////////////////////////////////////////////
		bool shoot9_1(double ,double = 0);
		bool walk(double ,double = 0);
};
#endif
