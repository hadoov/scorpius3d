#ifndef __Player_h__
#define __Player_h__
#include "SoccerSkills.h"

class Player : public SoccerSkills
{
	private:
		TEAM_STATE teamState;
		int	 playState;
		bool tag;
		bool tag2;
		bool fside;
	public:
	
		Player ( ) : SoccerSkills ( ) 
		{
			playState = 0;
			tag=false;
			tag2 = false;
			fside = true;
		};
		void  think ( );
		bool	player_1();
		bool	player_2();
		bool	player_3();
		bool	newGenWalk(char *c);
		string getCommand();
		
};
#endif
