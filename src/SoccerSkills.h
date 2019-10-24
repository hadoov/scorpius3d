#ifndef __Soccer_Skills_h__
#define __Soccer_Skills_h__

#include "BasicSkills.h"

class SoccerSkills : public BasicSkills
{
	private:
	
	public:
		
		SoccerSkills ( ) : BasicSkills ( ) {};
		void helpGoalKeeper();
		void helpAttacker();
		void shootToGoal();
		void goOnLine();	
		
};
#endif
