#ifndef _TYPES_H_
#define _TYPES_H_

#include "Geometry.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>



# define l_1 	0
# define l_2 	1
# define r_1 	2
# define r_2 	3
# define g_l_1 	4
# define g_l_2 	5
# define g_r_1 	6 
# define g_r_2 	7

# define MAX_PLAYERS 11
# define MAX_FLAGS    8
# define FIELD_LENGTH 12
# define FIELD_WIGHTH 8
# define GOAL_WIGHTH 1.4

// body Defines
# define MAX_3	100
# define MAX_4	1
# define MAX_5	75
# define MIN_3	-25
# define MIN_4	-130
# define MIN_5	-45
# define MAX_SPEED	6.5
# define bodyRoot	1.7

# define TEAMNAME "Scorpius"

enum TPlayMode
{
    // the order of the first 3 play modes should not be changed.
	PM_BEFORE_KICKOFF = 0,      /*!< before_kick_off:   before the match  */
	PM_KICKOFF_LEFT ,       /*!< kick_off_left:     kick off for the left team  */
	PM_KICKOFF_RIGHT ,      /*!< kick_off_right:    kick off for the right team */
	PM_PLAYON,                 /*!< play_on:           regular game play */
	PM_KICKIN_LEFT,
	PM_KICKIN_RIGHT,
	PM_CORNER_KICK_LEFT,       /*!< corner_kick_l:     corner kick left team   */
	PM_CORNER_KICK_RIGHT,      /*!< corner_kick_r:     corner kick right team  */
	PM_GOAL_KICK_LEFT,         /*!< goal_kick_l:       goal kick for left team */
	PM_GOAL_KICK_RIGHT,        /*!< goal_kick_r:       goal kick for right team*/
	PM_OFFSIDE_LEFT,           /*!< offside_l:         offside for left team   */
	PM_OFFSIDE_RIGHT,          /*!< offside_r:         offside for right team  */
	PM_GAME_OVER,
	PM_GOAL_LEFT,
	PM_GOAL_RIGHT,
	PM_FREE_KICK_LEFT,         /*!< free_kick_l:       free kick for left team */
	PM_FREE_KICK_RIGHT,        /*!< free_kick_r:       free kick for right team*/
	PM_UNKNOWN                    /*!< no play mode, this must be the last entry */
};


enum TSide
{
	S_LEFT,
	S_RIGHT,
	S_UNKNOWN
};
enum TEAM_STATE
{
	TS_ATTACK,
	TS_DEFENSE
};
enum TGameHalf
{
	GH_FIRST,
	GH_SECOND,
	GH_UNKNOWN
};

enum TVisionObject
{
	V_TEAMMATE_ID_1 = 0,
	V_TEAMMATE_ID_2,
	V_TEAMMATE_ID_3,
	V_TEAMMATE_ID_4,
	V_TEAMMATE_ID_5,
	V_OPPONENT_ID_1,
	V_OPPONENT_ID_2,
	V_OPPONENT_ID_3,
	V_OPPONENT_ID_4,
	V_OPPONENT_ID_5,
	V_FLAG_ID_L_1,
	V_FLAG_ID_L_2,
	V_FLAG_ID_R_1,
	V_FLAG_ID_R_2,
	V_GOAL_ID_L_1,
	V_GOAL_ID_L_2,
	V_GOAL_ID_R_1,
	V_GOAL_ID_R_2,
	V_BALL,
	V_UNKNOWN
};

enum JointSide
{
	JS_Right = 0,
	JS_Left
};

enum TGoal
{
	G_OUR_UP = 1, 
	G_OUR_DOWN,
	G_OPP_UP,
	G_OPP_DOWN,
	G_UNKNOWN
};
enum TAction
{
	A_Walk,
	A_Rotate,
	A_Init,
	A_Kick,
	A_Stop,
	A_Unkown
};
enum TSet
{
	SET_TEAMMATES,
	SET_OPPONENTS
};

class Torso
{
 	friend ostream& operator << ( ostream & , Torso );
	private :
		double angX,angY,angZ;
	public  :
		Torso(double =0,double =0,double = 0);
		Torso	operator + ( Torso &p );
		Torso	operator - ( Torso &p );
		double getXDirection();
		double getYDirection();
		double getZDirection();
		bool setTorso(Torso);
		bool setTorso(double =0,double =0,double = 0);
		bool setXDirection(double =0);
		bool setYDirection(double =0);
		bool setZDirection(double =0);
		double getMagnitude();
	
};



class Joint
{
	public:
		double axis , rate;
	
		Joint	( double x=0 , double y=0 ) { set (x,y); };
		void	set	( double , double  = 0);
		double	getAxis	( );
		double	getRate	( );
};

#endif
