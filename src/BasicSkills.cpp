#include "BasicSkills.h"

bool BasicSkills::resetStates()
{
	shootState = 0;
	standState = 0;	
	diveState = 0;
	walkState = 0;
	StandUpState = 0;
	shootPower = 30;
	standUpTime = 1000;
	goBehindBallState =1;
	goState = 0;
	goBehindBallState =1;
	shootState =0;
	delay=0;
	standState = 0;
	walkState = 0;
	lastDelayTime = 0;
	onDelay = false;
	StandUpState = 0;
	
	return true;
}	

bool BasicSkills::Shoot()
{
	kicktag =false;
	if( shootState == 0 )
	{
		//cout<<fabs(WM->getRelativePosition(V_BALL).getPhiDirection())<<endl;
		if(fabs(WM->getRelativePosition(V_BALL).getPhiDirection())<5.0)
			shootType = true;
		else
			shootType = false;
		if(WM->getRelativePosition(V_BALL).getPhiDirection() < 0.0 )
			shootSide =JS_Right;
		else
	 		shootSide =JS_Left;
		if(WM->isStable())shootState = 1;
	}
	else
		kicktag = shoot( shootSide );
	
	return kicktag;

}


bool BasicSkills::delayAct(double t)
{
	return true;
}	


bool BasicSkills::walk(int i)
{
	bool tag = false;
	if(walkState == 0)
	{
		walkState = 1;
		walkSpeed = 0.5;
	}
	else
		tag = HumanoidMovements::walk(walkSpeed,i);
	//cout<<walkSpeed<<endl;
	if(tag == true)
		walkSpeed += 0.0;
	walkSpeed = min(1,walkSpeed);
	walkSpeed = max(-1,walkSpeed);
	return tag;
}

bool BasicSkills::GoalKeeperAction()
{	
	
	return false;

}


bool BasicSkills::goToPos(VecPosition target,double error
			,double maxSpeed,double minSpeed)
{
	minSpeed = 0.2;
	double d1 = normalizeAngle((target-WM->getMyPosition()).getPhiDirection()-WM->getPanAngle());
	VecPosition dd = target - WM->getMyPosition();
	double dist = sqrt(dd.getX()*dd.getX() + dd.getY()*dd.getY());
	bool walktag = false;
	if(dist<error)
	{
		goState = 0;
		bool b = HumanoidMovements::walk(walkSpeed , 0);
		if(b) walkSpeed -=0.1;
		return b;
	}
	if(goState == 0)
	{
		sp = 0;
		walkState = 1;
		if(fabs(d1)>10)
		{
			goState = 1;
			sp = 2*sign(d1);
		}
		else goState =2;
	}
	else if(goState == 1)
	{
		walktag = HumanoidMovements::walk(0 , sp);
		if(walktag && fabs(d1)<5)
			goState = 1;			
	}
	else if(goState == 2)
	{
		walktag = HumanoidMovements::walk(walkSpeed , sp);			
	
		if(walktag)
		{
			cout<<"WWW"<<walkSpeed<<"\n";
			if(fabs(d1)>5)
			{
				if(fabs(d1) > 30)
					goState = 0;
				if(walkSpeed >0.6)
					walkSpeed -= 0.08;
				else
				{
					cout<<walkSpeed<<endl;
					walkSpeed -= 0.05;			
					sp += 0.1*sign(d1);
				}
			}
			else
			{
				sp = sp/4;
				if(dist < 0.5 + error)
				{
					walkSpeed -= 0.03;
				}
				else
				{
					walkSpeed += 0.03;
				}
			}
			if(walkSpeed < minSpeed)
				walkSpeed = minSpeed;
			if(walkSpeed > maxSpeed)
				walkSpeed = maxSpeed;
			if(fabs(sp) > 1.5)
				sp = 1.5*sign(sp);
		
		}
	}	
	return false;
}


bool BasicSkills::goBehindBall(VecPosition target)
{
	VecPosition posBall = WM->getBallPosition();
	VecPosition posMe = WM->getMyPosition();	
	VecPosition posBehindBall;
	VecPosition pos1,pos2;
	posBehindBall = posBall - (target - posBall).setMagnitude(0.4);
	if(posBall.getX()<posMe.getX())
		{
		pos1 = posBall;
		pos2 = posBall;
		pos1.setY(posBall.getY()+0.5);
		pos2.setY(posBall.getY()-0.5);
		pos1.setX(posBall.getX()-0.5);
		pos2.setX(posBall.getX()-0.5);
		
		if(WM->getDistanceTo(pos2)<WM->getDistanceTo(pos1))
			posBehindBall = pos2;
		else
			posBehindBall = pos1;
		goBehindBallState = 11;
		}
	else
		if(WM->getDistanceTo(WM->getBallPosition()) > 1 )
			goBehindBallState = 1;
	
	//cout<<goBehindBallState<<endl;
	switch (goBehindBallState )
	{
		case 11:
			if(goToPos(posBehindBall,0.2,0.94,0.3))
			{
				goBehindBallState = 1;
				goState = 1;
}
		break;
		
		case 1:
			if(goToPos(posBehindBall,0.3,0.94,0.4))
			{
				goBehindBallState = 2;
				goState = 1;
			}
		break;
		
		case 2:
			posBehindBall = posBall - (target - posBall).setMagnitude(0.20);
			if(goToPos(posBehindBall,0.05,0.94,0.3))
			{
				goBehindBallState = 3;
			}
		break;
		
		case 3:
			if(goToPos(posBall,0.01,0.94,0.5))
			{
				if(fabs(WM->getRelativePosition(V_BALL).getPhiDirection())
					> 7.0)
					shootType = false;
				else
					shootType = true;
				goBehindBallState = 4;
				walkState = 0;
				goState = 0;
			}
		break;
			
		case 4:
			if(WM->isStable()) goBehindBallState = 5;
			
		break;
			
		case 5:
			if( Shoot())
			{
				goBehindBallState = 1;
			}
		break;
	}
	
	return false;
	
}
