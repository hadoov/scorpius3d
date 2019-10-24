#ifndef __WORLD_MODEL_H__
#define __WORLD_MODEL_H__

#include "types.h"

using namespace std;

class WorldModel
{
	//private:
	public:
		VecPosition	teammatePos [MAX_PLAYERS],
				opponentPos[MAX_PLAYERS],
				flag[MAX_FLAGS],Ball,Me;

		VecPosition	lastMyPosition,lastBallPos,
				lastTeammatePos[MAX_PLAYERS],
				lastOpponentPos[MAX_PLAYERS] ;
		
		VecPosition 	relTeammatePos[MAX_PLAYERS],
				relOpponentPos[MAX_PLAYERS],
				relFlag[MAX_FLAGS] ,relBall;
		Matrix m;
    
		VecPosition 	lastRelTeammatePos[MAX_PLAYERS],
				lastRelOpponentPos[MAX_PLAYERS],
				lastRelFlag[MAX_FLAGS] ,lastRelBall;

		
		bool		teammateSeen[MAX_PLAYERS],
				opponentSeen[MAX_PLAYERS],
				flagSeen[MAX_FLAGS] , ballSeen ;

		double		lastTimePlayerSeen [MAX_PLAYERS];
		double		lastTimeBallSeen;
		double		lastTimeFlagSeen[MAX_FLAGS];
				

		VecPosition 	strategicPosition [MAX_PLAYERS+1];
		VecPosition     initialPosition[MAX_PLAYERS+1];
		
		double 		fieldLength, fieldWidth, fieldHeight,
				goalWidth, goalDepth, goalHeight,
				ballRadius, ballMass, MaxKickableDistance;
		
		TSide		side;
		TPlayMode	PLAYMODE;
		TGameHalf	gameHalf;
		
		string		teamName, oppTeamName;
		
		bool		oppTeamNameIsSet,begin ,isnew,initialed
				,isFallingDown,fallSide;
		
		int 		playerNumber;
		
		bool  		RightOnGround,LeftOnGround;
		Torso		torso,lastTorso;
		
		bool		falled;
		Joint 		leftHand[6] ,leftLeg[6] ,rightHand[6] ,	rightLeg[6]
				,head[3];
		Joint 		nextHand[6][2] ,nextLeg[6][2] , nextHead[3];
		
		Joint		lastHand[6][2] ,lastLeg[6][2] , lastHead[3];
		double 		time ,lastTime , gameTime ,lastGameTime,
				panAngle,tiltAngle;

	public :
	//===========================
	//.........functions ........
	bool initialComponents	();


	bool getFallSide();
	bool isPlayerFallingDown ();
	bool updateTime 	( char *str );
	void updateVision	( char *str );
	void setVision	    ( char *str );
	void updateJoint	( char *str );
	void updateTorso    ( char *str );
	void updateTCH      ( char *str );
	void updateFRP      ( char *str );
	void updateJoint	( bool side , bool ish 	, double a , double r , int i );
	void updateHeadJoint(int i,double a,double r =0);
	void updateGameState( char *str );
	void findFalgs		(			);
	int  getFlagNumber  (bool forg  ,bool side 	,int no		);
	void setRelativeFlag(VecPosition,bool forg	,bool side ,int no		);
	void findGlobalPositions();
	void findMyPosition();
	void findBallPosition();
	bool hasValidPosition();
	VecPosition	findMyPositionFromFlag(int i);
	bool isFootOnGround (JointSide side);
	bool isLeftFootOnGround();
	bool isRightFootOnGround();
	VecPosition getStrategicPosition(int);
	bool isPlayerFallDown();
	bool isBallMine();
	bool isBallOurs();
	bool isBallOpponents();
	bool canShootToGoal();
	Matrix LocalizeWithThreeFlagsTriangle(int ,int ,int );
	VecPosition SFindPosition(int ,int ,int );
	public:
	// ------------------ member parameters -----------------
	double	thighHeight , shankHeight , footHeight , hipHeight , KneeJointHeight;
	//--------------------------------------------
	
	WorldModel			( );
	bool	update		( string );
	void	resetPeriod	( );
	bool	startSim	( int i );
	bool	findPeriod	( int i );
	void	setData		( );
	
	/////////////////////////////////////////////
	// methods that return truth values about current play mode
	bool		isFreeKickUs		( );
	bool		isFreeKickThem		( );
	bool		isCornerKickUs		( );
	bool		isCornerKickThem	( );
	bool		isOffsideUs			( );
	bool		isOffsideThem		( );
	bool		isKickInUs			( );
	bool		isKickInThem		( );
	bool		isKickOffUs			( );
	bool		isKickOffThem		( );
	bool		isGoalKickUs		( );
	bool		isGoalKickThem		( );
	bool		isBeforeKickOff		( );
	bool		isDeadBallUs		( );
	bool		isDeadBallThem		( );
	bool		isBallCatchable		( );
	bool		isPlayOn		( );
	//--------------------------------------------
	void setPlayerNumber			( int );
	void setTeamName				( const string );
	void setOppTeamName				( const string );
	void setLastTime				(	 );
	void setPlayMode	  			( string );
	void setFlag					( );
	void setSide	 	  			( string );
	void setFlageId	  				( string );
	void setGameHalf	  			( TGameHalf );
	void setAgentStateObject 		( string , double , double );
	void setPlayersGlobalPosition	( );
	void setBallPosition			( );
	void setMyPosition				( );
	void setGoalRelPosition  		( string , VecPosition );
	void setFlagRelPosition			( string , VecPosition );
	void setPlayerRelPosition		( int , VecPosition ,bool);
	void setBallRelPosition			( VecPosition );
	void setFieldLength				( double );
	void setFieldWidth 				( double );
	void setFieldHeight 			( double );
	void setBallRadius 				( double );
	void setBallMass 				( double );
	bool isBallUs ( );
	void setLastPosition			( );
	
	//--------------------------------------------
	TVisionObject	getPlayerType	( int );
	TVisionObject	getFlagId		( int );
	TSide 			getSide			( );
	TPlayMode 		getPlayMode		( );
	int			getPlayerNumber ( );
	string			getTeamName		( );
	double			getLastTime		( );
	int			getPeriod		( );
	int			getStart		( );
	Joint			getJoint		( int , JointSide , bool );
	
	Joint			getHandJoint	( int , JointSide );
	Joint			getHeadJoint	(int i );
	Joint			getLegJoint		( int , JointSide );
	Joint			getJoint		( int , bool ,	bool );
	Joint			getHandJoint	( int , bool );
	Joint			getLegJoint		( int , bool );
	double			getHand			( int , JointSide );
	double			getLeg			( int , JointSide );
	double			getHead 		(int i );
	double			getHand			( int , bool );
	double			getLeg			( int , bool );
	bool			setNextLegAxis  ( int , JointSide , double);
	bool			setNextHandAxis  ( int , JointSide , double);
	
	
	Torso 			getTorso();
	Torso 			getLastTorso();
	Torso 			getDTorso();


	string			getOppTeamName			( );
	double			getTime					( );
	double			getGameTime		( );
	TGameHalf 		getGameHalf				( );
	VecPosition 		getTeammatePosition		( int );
	VecPosition		getOpponentPosition     ( int );
	VecPosition		getGlobalPosition		( TVisionObject );
	VecPosition		getMyPosition           ( );
	VecPosition 		getBallPosition	  		( );
	VecPosition		getGoalRelPosition 		( TGoal );
	VecPosition		getOppGoalPosition      ( );
	VecPosition		getOurGoalPosition		( );
	double 			getFieldLength			( );
	double 			getFieldWidth 			( );
	double 			getFieldHeight			( );
	double 			getBallRadius 			( );
	double 			getBallMass 			( );
	VecPosition		getRelativePosition		( TVisionObject );
	VecPosition		getBeamPosition			( );
	VecPosition		getOpponentGoaliPosition	( );
	int				vision2Int				( TVisionObject obj );
	
	
	bool			isBallFront				();
	VecPosition		getLastPlayerPosition	( int );
	VecPosition		getLastBallPosition		( );
	VecPosition		getLastMyPosition		( );
	bool 			isStable				(	);
	double			getDistanceTo			(VecPosition vec);
	bool			isBallKickAble			(	);
	double 			findMyPan				(	);
	bool 			findMyPanTilt			(	);
	VecPosition 		Vec2D				(VecPosition );
	bool			isInField			(VecPosition );
	double			getPanAngle			(		);
	double			getTiltAngle			(		);
	VecPosition 	behindBall				(VecPosition target,double distance);
	bool 			isPlayerOnLine			(VecPosition v1, VecPosition v2, double distance);
	bool 			isNearestToBall			();
	bool 			getNearestPlayerTo		(VecPosition vec,TSet set);
	int 			findValidNumber			();
	void			setValidNumber			(int no);
	bool			isBehindBall			(VecPosition target);
};
#endif
