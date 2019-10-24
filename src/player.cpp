#include "Player.h"

using namespace std;

bool f  = false;
bool t = true;
int ic = 0;
double q[2000][6];
double v = 90;
int pr;				
void Player::think()
{
	if (WM->isBeforeKickOff() /*|| (WM->getPlayMode() == PM_GOAL_LEFT)*/)
	{	
		resetStates();
		t = true;
		playState = 0;
		f = false;
		walkSpeed = 0.4;
		if(setAll(0,5,5))
			beam (WM->getBeamPosition());
	}
	else if(!f)
	{
		switch(WM->getPlayerNumber())
		{
			case 1:
			break;
			case 2:
 			break;
			case 3:
			break;
			case 4:
			break;
			case 10:
				BasicSkills::goBehindBall(VecPosition(5,3));
			break;
			case 9:
				//BasicSkills::walk(0.5);
				BasicSkills::Shoot();
			break;
		}
	}
	else
		if(0)
		{
			if(!WM->isPlayerFallDown())
				f = false;
			ic = 0;
			playState = 0;
			resetStates();
		}
	//if( (WM->isPlayerFallDown()) &&(WM->getPlayerNumber() != 1))
		;//f = true;
	
}


/*	if(WM->getBallPosition().getX()< -4.5)
		t =!goBehindBall(WM->getOppGoalPosition ( ));	
	else
		if(playState == 1)
		{
			if(goToPos(VecPosition(-5.7,0,0),0.4,0.6,0.2))
				playState = 2;
		}			
		else rotateTo(WM->getBallPosition(),20);
*/
/*	
bool	Player::player_2()
{
	cout<<"Player 2\n";
	if(WM->getBallPosition().getX() < -3.5)
		t =!goBehindBall(WM->getOppGoalPosition ( ));	
	else if(WM->getBallPosition().getX() < .6)
		goToPos(WM->getBallPosition(),0.01,0.95,0.6);
	else if(WM->getBallPosition().getX() < 1.5)
		t =!goBehindBall(WM->getOppGoalPosition ( ));	
	else
		if(playState == 1)
		{
			if(goToPos(VecPosition(-4.0,0,0),0.4,0.6,0.2))
				playState = 2;
		}			
		else rotateTo(WM->getBallPosition(),20);
	
	return false;
}
bool	Player::player_3()
{
	cout<<"Player_3\n";
	if(WM->getBallPosition().getX() > -3.0)
		t =!goBehindBall(WM->getOppGoalPosition ( ));	
	else
		if(playState == 1)
		{
			if(goToPos(VecPosition(1.5,0,0),0.4,0.6,0.1))
				playState = 2;
		}
		else rotateTo(WM->getBallPosition(),20);
	return true;
}
*/
string Player::getCommand()
{
	return BasicMovements::getCommand();
}
