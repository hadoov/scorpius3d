#include "WorldModel.h"
#include <fstream>
#define PI 3.1415

WorldModel::WorldModel ( )  ///////////***********************//////////////////
{
	oppTeamNameIsSet = false;
	time = 0.0;
	gameTime = 0.0;
	playerNumber = 0;
	side = S_UNKNOWN;
	PLAYMODE = PM_UNKNOWN;
	gameHalf = GH_UNKNOWN;
	begin = false;
	initialed = false;
	fieldLength = FIELD_LENGTH;
	fieldWidth = FIELD_WIGHTH;
	goalWidth = GOAL_WIGHTH;
	teamName = TEAMNAME ;
}


bool	WorldModel::initialComponents()
{
	setFlag();
	initialed = true;
	//        INITIAL POSITION
	initialPosition[1] = VecPosition(-0.8,-0,0);
	initialPosition[2] = VecPosition(-0.175,0.11,-25);
	initialPosition[3] = VecPosition(-0.50,-3,0);
	initialPosition[4] = VecPosition(-0.8,1.0,0.0);
	initialPosition[5] = VecPosition(-0.8,2,0);
	initialPosition[6] = VecPosition(-3,-2,0);
	initialPosition[7] = VecPosition(-2,2,0);
	initialPosition[8] = VecPosition(-2,0,0);
	initialPosition[9] = VecPosition(-0.17,-0.05,-0);
	initialPosition[10] = VecPosition(-0.8,-0,0);
	initialPosition[11] = VecPosition(-0.2,0,0);
	return true;
}


void WorldModel::findGlobalPositions()
{
	setMyPosition();
	setBallPosition();
	setPlayersGlobalPosition ( );
}

void WorldModel::setData ()
{
	int i ;
	lastTime = time;
	lastGameTime = gameTime ;
	for(i = 0;i<MAX_PLAYERS;i++)
	{
		if(teammateSeen[i])
			{
			lastTeammatePos[i] = teammatePos [i];
			lastRelTeammatePos[i] = relTeammatePos[i]; 
			}
		if(opponentSeen[i])
			{
			lastOpponentPos[i]  = opponentPos[i];
			lastRelOpponentPos[i] = relOpponentPos[i];
			}
		teammateSeen[i] = false;
		opponentSeen[i] = false;
		
	}

	for(i = 0;i<MAX_FLAGS;i++)
	{
		if(flagSeen[i])
			lastRelFlag[i]  = relFlag[i] ;
		flagSeen[i] = false;
	}

	if(ballSeen)
	{
		lastBallPos = Ball;
		lastRelBall = relBall ;
	}
	lastMyPosition = Me;
	ballSeen = false;
	falled = false;
}




void WorldModel::setGoalRelPosition  ( string Goal, VecPosition Vec )
{

	if ( Goal == "1_r" )
	{
		relFlag[g_r_1] = Vec;
		flagSeen[g_r_1] = true;
	}
	else if ( Goal == "2_r" )
	{
		relFlag[g_r_2] = Vec;
		flagSeen[g_r_2] = true;
	}
	else if ( Goal == "1_l" )
	{
		relFlag[g_l_1] = Vec;
		flagSeen[g_l_1] = true;
	}
	else if ( Goal == "2_l" )
	{
		relFlag[g_l_2] = Vec;
		flagSeen[g_l_2] = true;
	}
	return;
}

void WorldModel::setFlagRelPosition  ( string flag, VecPosition Vec )
{

	if ( flag == "1_r" )
	{
		relFlag[r_1] = Vec;
		flagSeen[r_1] = true;
	}
	else if ( flag == "2_r" )
	{
		relFlag[r_2] = Vec;		
		flagSeen[r_2] = true;
	}
	else if ( flag == "1_l" )
	{
		relFlag[l_1] = Vec;
		flagSeen[l_1] = true;
	}
	else if ( flag == "2_l" )
	{
		relFlag[l_2] = Vec;
		flagSeen[l_2] = true;
	}
	return;
}

VecPosition WorldModel::getGoalRelPosition (TGoal goalLocation)//be nzar ghalat
{
	if (side==S_LEFT)
	{
		switch (goalLocation)
		{
			case G_OUR_UP   : return flag [l_2];
			case G_OUR_DOWN : return flag [l_1];
			case G_OPP_UP   : return flag [r_2];
			case G_OPP_DOWN : return flag [r_1];
			default :return VecPosition(1000,1000);
		}
	}
	else if (side==S_RIGHT)
	{
		switch (goalLocation)
		{
			case G_OPP_UP   : return flag [l_2];
			case G_OPP_DOWN : return flag [l_1];
			case G_OUR_UP   : return flag [r_2];
			case G_OUR_DOWN : return flag [r_1];
			default :return VecPosition(1000,1000);
		
		}
	}
	return flag[G_UNKNOWN] ;
}

void WorldModel::setPlayersGlobalPosition ( )
{
	int i;
	for ( i =0;i < MAX_PLAYERS ; i++ )
	{
		if( i != (getPlayerNumber()-1) && teammateSeen[i])
			teammatePos [i] = m*relTeammatePos[i];
		else
			teammatePos [i] =  Me;
	}
	for ( i =0;i < MAX_PLAYERS ; i++ )
	{
		if(opponentSeen[i] )
			opponentPos[i] = m*relOpponentPos[i];
	}
	return;
}

void WorldModel::setBallPosition ( )
{
	Ball = m*relBall;
}

Matrix WorldModel::LocalizeWithThreeFlagsTriangle(int vs1,
                                                  int vs2,
                                                  int vs3)
{
    VecPosition pos;
    Matrix Mat;
	Mat.Identity();
	    	
    float a1 = pow (relFlag[vs1].getMagnitude(),2 ) - 
	pow(relFlag[vs2].getMagnitude(),2 );
    float b1 = 2.0f * (-flag[vs1].getY()) - 2.0f * (-flag[vs2].getY());
    float a2 = pow (relFlag[vs1].getMagnitude() ,2 ) - 
	pow ( relFlag[vs3].getMagnitude() ,2 );
	float b2 =2.0f * (-flag[vs1].getX()) - 
	2.0f * (-flag[vs3].getX() );
	
	VecPosition vv1 = relFlag[vs3] - relFlag[vs1];
	VecPosition vv2 = relFlag[vs2] - relFlag[vs1];
	VecPosition vv3(vv1.getX(),vv1.getY(),vv1.getZ());
	VecPosition vv4(vv2.getX(),vv2.getY(),vv2.getZ());
	VecPosition vv5 = vv3.Cross(vv4);
	if(vv5.getY() < 0)
		fallSide = true;
	else
		fallSide = false;
	
    if (fabs(b1) < 1e-3 ||
        fabs(b2) < 1e-3)
    {
        return Mat;
    }

    float under;
    under = b2 * -0.5;
    Mat(0, 0) = (relFlag[vs1].getX() - relFlag[vs3].getX()) / under;
    Mat(0, 1) = (relFlag[vs1].getY() - relFlag[vs3].getY()) / under;
    Mat(0, 2) = (relFlag[vs1].getZ() - relFlag[vs3].getZ()) / under;

    under = b1 * -0.5;
    Mat(1, 0) = (relFlag[vs1].getX() - relFlag[vs2].getX()) / under;
    Mat(1, 1) = (relFlag[vs1].getY() - relFlag[vs2].getY()) / under;
    Mat(1, 2) = (relFlag[vs1].getZ() - relFlag[vs2].getZ()) / under;

    VecPosition vx(Mat(0, 0), Mat(0, 1), Mat(0, 2));
    VecPosition vy(Mat(1, 0), Mat(1, 1), Mat(1, 2));
    VecPosition vz = vx.Cross(vy);
    Mat(2, 0) = vz.getX(); Mat(2, 1) = vz.getY(); Mat(2, 2) = vz.getZ();
	
	pos.setX( a2 / b2);
    pos.setY( a1 / b1);
    pos.setZ( flag[vs1].getZ()
             - m(2, 0) * relFlag[vs1].getX()
             - m(2, 1) * relFlag[vs1].getY()
             - m(2, 2) * relFlag[vs1].getZ());
	Mat.setPos(pos);
	return Mat;
}

void WorldModel::setMyPosition ( )
{ 
    m = LocalizeWithThreeFlagsTriangle(0, 1, 2);
    Me = m.getPos();
	
}

bool WorldModel::isPlayerFallDown()
{
	if (Me.getZ()<0.20) return true;
	else
	return false;	
}

bool WorldModel::isPlayerFallingDown()
{
	VecPosition we = m*VecPosition(1,0,0);
	if(fabs(Me.getZ() - we.getZ())>0.6)
		return true;
	else
		return false;

}

bool WorldModel::getFallSide()
{
	VecPosition we = m*VecPosition(1,0,0);
	if(we.getZ()>0.8)
		return true;
	else
		return false;
}

bool 		WorldModel::isStable()
{
	return ((Me - lastMyPosition).getMagnitude()<0.00000000001);
}

VecPosition   WorldModel::getMyPosition ( )
{
	return Me;
}

VecPosition WorldModel::getTeammatePosition ( int Number )
{	
	return teammatePos [Number-1];
}

VecPosition WorldModel::getOpponentPosition(int number)
{
	return opponentPos[number-1];
}

VecPosition WorldModel::getLastBallPosition ( )
{
	return	lastBallPos;
}

VecPosition WorldModel::getLastMyPosition ( )
{		
	return getLastPlayerPosition (playerNumber);
}

VecPosition WorldModel::getLastPlayerPosition ( int Number )
{
	return lastTeammatePos[Number];
}

double	WorldModel::getDistanceTo(VecPosition vec)
{
	return Me.getDistanceTo(vec);
}

double	WorldModel::getPanAngle()
{
	VecPosition we = m*VecPosition(1,0,0);
	return (we - Me).getPhiDirection();

}
double WorldModel::getTiltAngle()
{
	return tiltAngle;
}

bool WorldModel::isFootOnGround(JointSide side)
{
	if(side == JS_Right)
		return 	RightOnGround;
	else
		return LeftOnGround;
}
bool WorldModel::isRightFootOnGround()
{
	return isFootOnGround(JS_Right);
}
bool WorldModel::isLeftFootOnGround()
{
	return isFootOnGround(JS_Left);
}

 
VecPosition WorldModel::getBeamPosition()
{
	return initialPosition[getPlayerNumber()];
	if(getPlayMode() ==  PM_BEFORE_KICKOFF)
	{
		if( (getSide() == S_LEFT) && (getGameTime() < 1))
		{
		}	
		if( (getSide() == S_RIGHT) && (getGameTime() > 290))
		{
		}
	}

}
//===========================================
TVisionObject WorldModel::getFlagId ( int number )
{
	switch ( number )
	{
		case 1:
			return V_FLAG_ID_R_1;
		case 2:
			return V_FLAG_ID_R_2;
		case 3:
			return V_FLAG_ID_L_1;
		case 4:
			return V_FLAG_ID_L_2;
		case 5:
			return V_GOAL_ID_R_1;
		case 6:
			return V_GOAL_ID_R_2;
		case 7:
			return V_GOAL_ID_L_1;
		case 8:
			return V_GOAL_ID_L_2;
		default :
			return V_UNKNOWN;
	} 
}
VecPosition WorldModel::getGlobalPosition(TVisionObject obj)
{
	switch(obj)
	{
		case V_TEAMMATE_ID_1 	: return getTeammatePosition (1);
		case V_TEAMMATE_ID_2 	: return getTeammatePosition (2); 
		case V_TEAMMATE_ID_3 	: return getTeammatePosition (3);
		case V_TEAMMATE_ID_4 	: return getTeammatePosition (4);
		case V_TEAMMATE_ID_5 	: return getTeammatePosition (5);
		case V_OPPONENT_ID_1 	: return getOpponentPosition (1);
		case V_OPPONENT_ID_2 	: return getOpponentPosition (2);
		case V_OPPONENT_ID_3 	: return getOpponentPosition (3);
		case V_OPPONENT_ID_4 	: return getOpponentPosition (4);
		case V_OPPONENT_ID_5 	: return getOpponentPosition (5);
		case V_FLAG_ID_L_1 	: return flag[l_1];
		case V_FLAG_ID_L_2 	: return flag[l_2];
		case V_FLAG_ID_R_1 	: return flag[r_1];
		case V_FLAG_ID_R_2 	: return flag[r_2];
		case V_GOAL_ID_L_1 	: return flag[g_l_1];
		case V_GOAL_ID_R_1 	: return flag[g_r_1];
		case V_GOAL_ID_L_2 	: return flag[g_l_2];
		case V_GOAL_ID_R_2 	: return flag[g_r_2];
		case V_BALL 	   	: return getBallPosition();
			
		default
			: return getMyPosition();
	}
}


VecPosition WorldModel::getRelativePosition(TVisionObject obj)
{
	switch(obj)
	{
		case V_TEAMMATE_ID_1 	: return relTeammatePos[obj] ;
		case V_TEAMMATE_ID_2 	: return relTeammatePos[obj] ; 
		case V_TEAMMATE_ID_3 	: return relTeammatePos[obj] ;
		case V_TEAMMATE_ID_4 	: return relTeammatePos[obj] ;
		case V_TEAMMATE_ID_5 	: return relTeammatePos[obj] ;
		case V_OPPONENT_ID_1 	: return relOpponentPos[obj-5] ;
		case V_OPPONENT_ID_2 	: return relOpponentPos[obj-5] ;
		case V_OPPONENT_ID_3 	: return relOpponentPos[obj-5] ;
		case V_OPPONENT_ID_4 	: return relOpponentPos[obj-5] ;
		case V_OPPONENT_ID_5 	: return relOpponentPos[obj-5] ;
		case V_FLAG_ID_L_1 		: return relFlag[l_1];
		case V_FLAG_ID_L_2 		: return relFlag[l_2];
		case V_FLAG_ID_R_1 		: return relFlag[r_1];
		case V_FLAG_ID_R_2 		: return relFlag[r_2];
		case V_GOAL_ID_L_1 		: return relFlag[g_l_1];
		case V_GOAL_ID_R_1 		: return relFlag[g_r_1];
		case V_GOAL_ID_L_2 		: return relFlag[g_l_2];
		case V_GOAL_ID_R_2 		: return relFlag[g_r_2];
		case V_BALL 	   		: return relBall;
			
		default
			: return VecPosition ( 0.0 , 0.0 , 0.0 );
	}
}

void WorldModel::setFlag ( )
{
	if (side==S_RIGHT)
	{
	flag[r_1].setVecPosition ( -fieldLength / 2.0 , -fieldWidth / 2.0 ,0.0);
	flag[r_2].setVecPosition ( -fieldLength / 2.0 , fieldWidth / 2.0 ,0.0);
	flag[l_1].setVecPosition ( fieldLength / 2.0 , -fieldWidth / 2.0 ,0.0);
	flag[l_2].setVecPosition ( fieldLength / 2.0 , fieldWidth / 2.0 ,0.0);
	
	flag[g_r_1].setVecPosition ( -fieldLength / 2.0 , -goalWidth / 2.0 ,0.4);
	flag[g_r_2].setVecPosition ( -fieldLength / 2.0 , goalWidth / 2.0 ,0.4);
	flag[g_l_1].setVecPosition ( fieldLength / 2.0 , -goalWidth / 2.0 ,0.4);
	flag[g_l_2].setVecPosition ( fieldLength / 2.0 , goalWidth / 2.0 ,0.4);
	}
	else
	{
	flag[r_1].setVecPosition ( fieldLength / 2.0 ,fieldWidth / 2.0 ,0.0);
	flag[r_2].setVecPosition ( fieldLength / 2.0 ,-fieldWidth / 2.0 ,0.0);
	flag[l_1].setVecPosition ( -fieldLength / 2.0 ,fieldWidth / 2.0 ,0.0);
	flag[l_2].setVecPosition ( -fieldLength / 2.0 ,-fieldWidth / 2.0 ,0.0);
	
	flag[g_r_1].setVecPosition ( fieldLength / 2.0 , goalWidth / 2.0 ,0.4);
	flag[g_r_2].setVecPosition ( fieldLength / 2.0 , -goalWidth / 2.0 ,0.4);
	flag[g_l_1].setVecPosition ( -fieldLength / 2.0 , goalWidth / 2.0 ,0.4);
	flag[g_l_2].setVecPosition ( -fieldLength / 2.0 , -goalWidth / 2.0 ,0.4);
	}
}

TVisionObject WorldModel::getPlayerType ( int Number )
{
	switch ( Number )
	{
		case 1 :
			return V_TEAMMATE_ID_1;
		case 2 :
			return V_TEAMMATE_ID_2; 
		case 3 :
			return V_TEAMMATE_ID_3;
		case 4 :
			return V_TEAMMATE_ID_4;
		case 5 :
			return V_TEAMMATE_ID_5;
		case 6 :
			return V_OPPONENT_ID_1;
		case 7 :
			return V_OPPONENT_ID_2; 
		case 8 :
			return V_OPPONENT_ID_3;
		case 9 :
			return V_OPPONENT_ID_4;
		case 10 :
			return V_OPPONENT_ID_5;
		default :
			return V_UNKNOWN;
	}
}


//======================= High Level =================


bool	WorldModel::isBallKickAble()
{
	VecPosition vec = getRelativePosition(V_BALL);
	vec.setZ(0);
	return(vec.getMagnitude()< 0.5);
}

VecPosition WorldModel::behindBall(VecPosition target,double distance)
{
	VecPosition v1,v2;
	Circle c1 (Ball,distance);
	Line l1 (target,Ball);
	l1.getCircleIntersectionPoints(c1,v1,v2);
	if (v1.getDistanceTo(target) > v2.getDistanceTo(target)) return v1;
	return v2; 
}


bool WorldModel::canShootToGoal()
{
		return false;
}
bool WorldModel::isBehindBall(VecPosition target)
{
	
	return false;
}	 

bool WorldModel::getNearestPlayerTo(VecPosition vec,TSet set)
{

  int     closestObject = 0,o;
  double  dMinMag   = 1000.0;
  double v; 
	
  if (set == SET_TEAMMATES)
  {
  for(  o =1 ;o <= MAX_PLAYERS;o ++ )
  { 
       v = getTeammatePosition(o).getDistanceTo(vec);
		 
	  
      if( v < dMinMag )
      {
        dMinMag       = v;
        closestObject = o;
      }
  }
  return closestObject;
  }
  
  else
  {
  for(  o =1 ;o <= MAX_PLAYERS;o ++ )
  { 
       v = getOpponentPosition(o).getDistanceTo(vec);
		 
	  
      if( v < dMinMag )
      {
        dMinMag       = v;
        closestObject = o;
      }
  }
  return closestObject;
  }
  
}
bool WorldModel::isNearestToBall()
{

  int     closestObject = 0,o;
  double  dMinMag   = 1000.0;
  double v; 
	
  
  for(  o =1 ; o <=MAX_PLAYERS ; o ++ )
  { 
       v = getTeammatePosition(o).getDistanceTo(getBallPosition());
		 
	  
      if( v < dMinMag )
      {
        dMinMag       = v;
        closestObject = o;
      }
  }
  if (o==playerNumber) return true;
  return false;
}

VecPosition WorldModel::getStrategicPosition(int num)
{
	return strategicPosition[num];
	
}

//================== game state =========================

void WorldModel::setSide ( string Side )
{
	begin = true;
	if (Side == "left")
		side = S_LEFT;
	else if (Side == "right")
		side = S_RIGHT;
	else 
		side = S_UNKNOWN;
	return;
}

void WorldModel::setGameHalf ( TGameHalf g )
{
	switch ( g )
	{
		case GH_FIRST:
			gameHalf = GH_FIRST;
			break;
		case GH_SECOND:
			gameHalf = GH_SECOND;
			break;
		default :
			gameHalf =GH_UNKNOWN;
			
	}
	return;
}

TPlayMode WorldModel::getPlayMode ( )
{
	return PLAYMODE;	
}

TSide WorldModel::getSide ( )
{	
	return side;
}

TGameHalf WorldModel::getGameHalf ( )
{
	return gameHalf;
}


bool WorldModel::isFreeKickUs ( )
{
	TPlayMode pm = getPlayMode();

	bool bLeftKick = ( pm==PM_FREE_KICK_LEFT );
	bool bRightKick= ( pm==PM_FREE_KICK_RIGHT );

	return ( bLeftKick   && getSide() == S_LEFT  ) ||
			( bRightKick  && getSide() == S_RIGHT ) ;
}

bool WorldModel::isFreeKickThem ( )
{
	TPlayMode pm = getPlayMode();

	bool bLeftKick = ( pm==PM_FREE_KICK_LEFT  );
	bool bRightKick= ( pm==PM_FREE_KICK_RIGHT );

	return ( bRightKick  && getSide() == S_LEFT ) ||
			( bLeftKick   && getSide() == S_RIGHT ) ;
}

bool WorldModel::isBeforeKickOff ( )
{

	TPlayMode pm = getPlayMode();
	return ( pm == PM_BEFORE_KICKOFF  || pm == PM_GOAL_LEFT  || pm == PM_GOAL_RIGHT ) ;
}

bool WorldModel::isDeadBallUs ( )
{
//	TPlayMode pm = getPlayMode();

	return (isKickInUs  ( ) || isFreeKickUs  ( ) || isCornerKickUs     ( )
			|| isKickOffUs ( ) || isOffsideThem ( ) || isGoalKickUs( )) ;
}

bool WorldModel::isDeadBallThem ( )
{
	//TPlayMode pm = getPlayMode();

	return isFreeKickThem( ) || isKickInThem  ( ) || isCornerKickThem ( )
			||  isKickOffThem ( ) || isGoalKickThem( ) || isOffsideUs    ( ) ;
}

bool WorldModel::isCornerKickUs ( )
{
	TPlayMode pm = getPlayMode();

	return ( pm == PM_CORNER_KICK_LEFT  && getSide() == S_LEFT  ) ||
			( pm == PM_CORNER_KICK_RIGHT && getSide() == S_RIGHT ) ;
}

bool WorldModel::isCornerKickThem ( )
{
	TPlayMode pm = getPlayMode();

	return ( pm == PM_CORNER_KICK_RIGHT  && getSide() == S_LEFT  ) ||
			( pm == PM_CORNER_KICK_LEFT   && getSide() == S_RIGHT ) ;
}

bool WorldModel::isOffsideUs ( )
{
	TPlayMode pm = getPlayMode();

	return ( pm == PM_OFFSIDE_RIGHT  && getSide() == S_RIGHT ) ||
			( pm == PM_OFFSIDE_LEFT   && getSide() == S_LEFT );
}

bool WorldModel::isOffsideThem ( )
{
	TPlayMode pm = getPlayMode();

	return ( pm == PM_OFFSIDE_LEFT  && getSide() == S_RIGHT ) ||
			( pm == PM_OFFSIDE_RIGHT && getSide() == S_LEFT );
}

bool WorldModel::isKickInUs ( )
{
	TPlayMode pm = getPlayMode();

	return ( pm == PM_KICKIN_LEFT  && getSide() == S_LEFT  ) ||
			( pm == PM_KICKIN_RIGHT && getSide() == S_RIGHT ) ;
}
bool WorldModel::isBallUs ( )
{
	TPlayMode pm = getPlayMode();

	if ( pm == PM_GOAL_LEFT  && getSide() == S_RIGHT  ) return true;
	if ( pm == PM_GOAL_RIGHT && getSide() == S_LEFT ) return true;
	return false;
}


bool WorldModel::isKickInThem ( )
{
	TPlayMode pm = getPlayMode();

	return ( pm == PM_KICKIN_RIGHT  && getSide() == S_LEFT  ) ||
			( pm == PM_KICKIN_LEFT   && getSide() == S_RIGHT ) ;
}

bool WorldModel::isKickOffUs ( )
{
	TPlayMode pm = getPlayMode();

	return ( pm == PM_KICKOFF_LEFT  && getSide() == S_LEFT  ) ||
			( pm == PM_KICKOFF_RIGHT && getSide() == S_RIGHT ) ;
}

bool WorldModel::isKickOffThem ( )
{
	TPlayMode pm = getPlayMode();

	return ( pm == PM_KICKOFF_RIGHT  && getSide() == S_LEFT  ) ||
			( pm == PM_KICKOFF_LEFT   && getSide() == S_RIGHT ) ;
}

bool WorldModel::isGoalKickUs ( )
{
	TPlayMode pm = getPlayMode();

	return ( pm == PM_GOAL_KICK_LEFT  && getSide() == S_LEFT  ) ||
			( pm == PM_GOAL_KICK_RIGHT && getSide() == S_RIGHT ) ;
}

bool WorldModel::isGoalKickThem ( )
{
	TPlayMode pm = getPlayMode();

	return ( pm == PM_GOAL_KICK_RIGHT  && getSide() == S_LEFT  ) ||
			( pm == PM_GOAL_KICK_LEFT   && getSide() == S_RIGHT ) ;
}

bool WorldModel::isPlayOn ()
{
	return (getPlayMode() == PM_PLAYON);
}


void WorldModel::setPlayMode ( string PlayMode )
{
	if ( PlayMode == "BeforeKickOff" )
		PLAYMODE = PM_BEFORE_KICKOFF;
	else if ( PlayMode == "KickOff_Left" )
		PLAYMODE =PM_KICKOFF_LEFT;
	else if ( PlayMode == "KickOff_Right" )
		PLAYMODE =PM_KICKOFF_RIGHT;
	else if ( PlayMode == "PlayOn" )
		PLAYMODE =PM_PLAYON;
	else if ( PlayMode == "KickIn_Left" )
		PLAYMODE =PM_KICKIN_LEFT;
	else if ( PlayMode == "KickIn_Right" )
		PLAYMODE =PM_KICKIN_RIGHT;
	else if ( PlayMode == "corner_kick_left" )
		PLAYMODE =PM_CORNER_KICK_LEFT;
	else if ( PlayMode == "corner_kick_right" )
		PLAYMODE =PM_CORNER_KICK_RIGHT;
	else if ( PlayMode == "goal_kick_left" )
		PLAYMODE =PM_GOAL_KICK_LEFT;
	else if ( PlayMode == "goal_kick_right" )
		PLAYMODE =PM_GOAL_KICK_RIGHT;
	else if ( PlayMode == "offside_left" )
		PLAYMODE =PM_OFFSIDE_LEFT;
	else if ( PlayMode == "offside_right" )
		PLAYMODE =PM_OFFSIDE_RIGHT;
	else if ( PlayMode == "GameOver" )
		PLAYMODE =PM_GAME_OVER;
	else if ( PlayMode == "Goal_Left" )
		PLAYMODE =PM_GOAL_LEFT;
	else if ( PlayMode == "Goal_Right" )
		PLAYMODE =PM_GOAL_RIGHT;
	else if ( PlayMode == "free_kick_left" )
		PLAYMODE =PM_FREE_KICK_LEFT;
	else if ( PlayMode == "free_kick_right" )
		PLAYMODE =PM_FREE_KICK_RIGHT;
	else
		PLAYMODE = PM_UNKNOWN;
		
 
	return;
}

bool	WorldModel::setNextLegAxis  ( int i, JointSide j, double x)
{
	nextLeg[i][j].set(x);
	return true;
}
bool	WorldModel::setNextHandAxis  ( int i, JointSide j, double x)
{
	nextHand[i][j].set(x);
	return true;
}



//--------------Tested ------------
Torso WorldModel::getTorso()
{
	return torso;
}			

Torso WorldModel::getLastTorso()
{

	return lastTorso;
}			

Torso WorldModel::getDTorso()
{
	return torso-lastTorso;
}			

int  WorldModel::getFlagNumber(bool forg,bool sside ,int no)
{
	int fno;
	if(forg) fno =no;
	else fno = no + 4;
	if(sside) fno +=2;
 		
	return fno;
}
void WorldModel::setRelativeFlag(VecPosition vec,bool forg,bool side ,int no)
{
	relFlag[getFlagNumber(forg,side,no)] = vec;
}



double WorldModel::getTime ()
{
	return time;
}

double WorldModel::getGameTime ( )
{
	return gameTime;
}

VecPosition WorldModel::getBallPosition ()
{
	return Ball;
}

void WorldModel::updateHeadJoint(int i,double a,double r)
{

	head[i].set(a,r);

}

void WorldModel::updateJoint (bool side,bool ish,double a,double r,int i)
{
	if(side)
		if(ish)
			rightHand[i].set(a,r);
		else
			rightLeg[i].set(a,r);
	else
		if(ish)
			leftHand[i].set(a,r);
		else
			leftLeg[i].set(a,r);
}

void WorldModel::setLastTime ()
{
	lastTime = time;
}

double WorldModel::getLastTime ()
{
	return lastTime;
}

void WorldModel::setTeamName(const string tname)
{
	teamName = tname;
	
}

string WorldModel::getTeamName ( )
{
	return teamName;
}

void WorldModel::setOppTeamName ( const string s )
{
	oppTeamName = s;
	oppTeamNameIsSet = true;
}

string WorldModel::getOppTeamName ( )
{
	return oppTeamName;
}

void WorldModel::setPlayerNumber ( int nr )
{
	playerNumber=nr;
}

int WorldModel::getPlayerNumber ( )
{
	return playerNumber;
}

VecPosition  WorldModel::getOppGoalPosition ( )
{
	return VecPosition ( fieldLength / 2.0 , 0.0 , 0.0 );
}

VecPosition  WorldModel::getOurGoalPosition ( )
{
	return VecPosition ( -fieldLength / 2.0 , 0.0 , 0.0 );
}


void WorldModel::setPlayerRelPosition ( int Number, VecPosition Vec ,bool b)
{
	if(Number < 1 || Number>MAX_PLAYERS)
	{
		//cout<<"See illegal player with number " <<Number<<endl;
		return ;
	}
	if(b == true)
	{
		relTeammatePos[Number-1] = Vec;
		teammateSeen[Number-1]= true;
	}
	else
	{
		relOpponentPos[Number-1] = Vec;
		opponentSeen[Number-1]= true;
	}
	return;
}

VecPosition WorldModel::getOpponentGoaliPosition()
{
	VecPosition oppGoli = VecPosition(-100,-100,-100,CARTESIAN);
	int j = 0;
	for(int i =0;i<MAX_PLAYERS;i++)
		if(opponentSeen[i])
			if(opponentPos[i].getX()>oppGoli.getX())
				{
				oppGoli = opponentPos[i];
				j = i+1;
				}
	return oppGoli;
}
void WorldModel::setBallRelPosition ( VecPosition Vec )
{
	relBall = Vec;
	ballSeen = true;
}


//================ Joint funcions ======================
Joint WorldModel::getLegJoint (int i , JointSide side)
{
	return getJoint (i,side,0);
}

Joint WorldModel::getHeadJoint (int i )
{
	return head[i];
}

Joint WorldModel::getHandJoint (int i,JointSide side)
{
	return getJoint (i,side,1);
}

Joint WorldModel::getJoint (int i,JointSide side,bool ish)
{
	if(side == JS_Right)
		if(ish)
			return rightHand[i];	
		else
			return rightLeg[i];
	else
		if(ish)
			return leftHand[i];
		else
			return leftLeg[i];
}

Joint WorldModel::getLegJoint (int i,bool side)
{
	return getJoint (i,side,0);
}

Joint WorldModel::getHandJoint (int i,bool side)
{
	return getJoint (i,side,1);
}

Joint WorldModel::getJoint (int i,bool side,bool ish)
{
	if(side)
		if(ish)
			return rightHand[i];
		else
			return rightLeg[i];
	else
		if(ish)
			return leftHand[i];
		else
			return leftLeg[i];
}

double WorldModel::getLeg (int i , JointSide side)
{
	return getLegJoint (i,side).getAxis();
}

double WorldModel::getHead (int i )
{
	return getHeadJoint (i).getAxis();
}

double WorldModel::getHand (int i,JointSide side)
{
	return getHandJoint (i,side).getAxis();
}

double WorldModel::getLeg (int i,bool side)
{
	return getLegJoint (i,side).getAxis();
}

double WorldModel::getHand (int i,bool side)
{
	return getHandJoint (i,side).getAxis();
}
//================================================================
