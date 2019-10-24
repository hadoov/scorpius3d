#include "HumanoidMovements.h"

HumanoidMovements::HumanoidMovements ( ) : BasicMovements ( )
{
	shootState =0;
	delay=0;
	standState = 0;
	walkState = 0;
	lastDelayTime = 0;
	onDelay = false;
	StandUpState = 0;
	diveState = 1;
////////////////////////////
	fileID = 1;
	left = new step(fileID);
	right = new step (fileID);
	Body = new body (bodyRoot);

}
//==========================do not work well===================
bool HumanoidMovements::shoot3(JointSide side, double kickDistan)
{
	//cout<<shootState<<endl;
	double error = 5 ;
	//bool s;
	if(1)//side == JS_Left)
	{
		if(shootState == 1)
		if(  	setBothHand(-90,1,5,error) & 
				setBothHand(90,4,5,error)	& 
				setBothHand(90,3,5,error) & 				
				setLeftFoot(-10,4,10,error)  	& setRightFoot(-10,4,10,error) 	&
		      	setLeftFoot(10,2,10,error)		& setRightFoot(10,2,10,error)	&
		     	setLeftFoot(5,5,10,error)		& setRightFoot(5,5,10,error)	&
				setLeftFoot(5,3,10,error)		& setRightFoot(5,3,10,error)	&	
				setLeftFoot(-10,6,10,error) 	& setRightFoot(-10,6,10,error))
		shootState = 2;
	
		if(shootState == 2)
			if(	setLeftFoot(35,3,10,error) & setLeftFoot(-70,4,10,error)
				& setLeftFoot(35,5,10,error)&
				
				setRightFoot(5,3,10,error) & setRightFoot(-10,4,10,error)
				& setRightFoot(5,5,10,error)
			) shootState = 3;
	if(shootState == 3)
	{
		//goFoot(50,3,false);
		//goFoot(0,4,false);
		//goFoot(-50,5,false);
		if(		setRightFoot(5,3,10,error) &
				
				setLeftFoot(10,3,10,error) &
				setLeftFoot(-70,4,10,error) & 
				setLeftFoot(35,5,10,error)) shootState = 4;
			//goFoot(40,3,false);
	}
	if(shootState == 4)
	{
		//goFoot(50,3,false);
		//goFoot(0,4,false);
		//goFoot(-50,5,false);
		if(	setRightFoot(-10,3,kickDistan*10,error) & setRightFoot(-10,4,10,error)
				& setRightFoot(5,5,10,error)&
		
				setLeftFoot(60,3,kickDistan*10,error) &
				setLeftFoot(-30,4,kickDistan*10,error) & 
				setLeftFoot(-40,5,kickDistan*10,error)) shootState = 5;
			//goFoot(40,3,false);
	}
	if(shootState ==5)
			if(	setRightFoot(5,3,10,error) & setRightFoot(-10,4,10,error)
				& setRightFoot(5,5,10,error)&
			
				setLeftFoot(50,3,20,error) & 
				setLeftFoot(-100,4,20,error)& 
				setLeftFoot(50,5,20,error)
				) shootState = 6;
	if(shootState ==6) if( setAllFoot(0,10,error))	shootState =7;
	if(shootState ==7)	if( WM->isStable())	{shootState =0;return true;}
	}
	else
	{
	if(shootState == 1)
		if(   	setLeftFoot(-20,4,2)  	& setRightFoot(-20,4,2) 	&
		      	setLeftFoot(10,2,2) 	& setRightFoot(10,2,2)  	&
		     	setLeftFoot(10,5,2) 	& setRightFoot(10,5,2)	&
				setLeftFoot(-10,3,2) 	& setRightFoot(-10,3,2)	&	
				setLeftFoot(10,6,2) 	& setRightFoot(10,6,2)	) shootState = 2;
	
	if(shootState == 2)
			if(	setRightFoot(20,2,10) & setRightFoot(-40,4,10)
				& setRightFoot(20,5,10)
			) shootState = 3;
	if(shootState == 3)
			if(	setRightFoot(0,4,kickDistan*(35.0/15.0)) 
				& setRightFoot(-40,5,kickDistan*(35.0/15.0))) shootState = 5;
	if(shootState ==4)
		if (setLeftFoot(0,4,20,5) & setLeftFoot(0,5,20,5)&
			setRightFoot(-50,4,20,5) & setRightFoot(45,2,20,5)&
			setRightFoot(0,5,20,5)) shootState = 5;
	if(shootState ==5) if( setAllFoot(0,10))	shootState =6;
	if(shootState ==6)	if( WM->isStable())	{shootState =0;return true;}
	}
	return false;
}

bool HumanoidMovements::standUpFront()
{
	double error = 2;	
	switch (StandUpState)
		{
			case 0:
				if( setBothHand(-90,1,15,error) & setBothHand(0,4,5,error)
					& setBothHand(0,3,5,error) & setBothFoot(20,5,5,error)
					& setBothFoot(0,4,5,error) & setBothFoot(0,3,5,error)
					& setBothFoot(0,2,5,error) & setBothFoot(0,1,5,error)
					& setBothFoot(0,6,5,error) )
					StandUpState = 1;
				break;
			case 1:
				if( setBothFoot(90,3,40,error) & setBothHand(115,1,40,error)
					& setRightFoot(-60,1,40,error) & setLeftFoot(-60,1,40,error) )
					StandUpState = 2;
				break;
			// standUpBack
			
			case 2:
				if( setBothHand(-90,1,15,error) & setBothHand(0,4,5,error)
					& setBothHand(0,3,5,error) & setBothFoot(-20,5,15,error)
					& setBothFoot(0,4,5,error) & setBothFoot(0,3,5,error)
					& setBothFoot(0,2,5,error) & setBothFoot(0,1,5,error)
					& setBothFoot(0,6,5,error) )
					StandUpState = 3;
				break;
			case 3:
				if( setBothFoot(90,3,40,error) & setBothHand(0,1,40,error) )
					StandUpState = 4;
				break;
			case 4:
				if( setBothFoot(98,3,40,error) & setBothFoot(-80,1,40,error) 
					& setBothFoot(-80,4,40,error)
					& setBothHand(-40,1,4,error) & setLeftFoot(-10,6,20,error)
					& setRightFoot(10,6,20,error))
					if (WM->isStable()) StandUpState = 6;
				break;
			case 6:
				if( setBothHand(-70,1,10,error) & setBothHand(0,4,10,error) &
					setBothHand(0,2,10,error)
					& setBothHand(0,3,10,error) & setBothFoot(0,5,10,error)
					& setBothFoot(0,4,10,error) & setBothFoot(0,3,10,error)
					& setBothFoot(0,2,10,error) & setBothFoot(0,1,10,error)
					& setBothFoot(0,6,10,error)/*setAll(0,10)*/ )
					{
							StandUpState = 0;
							return true;
					}
				break;
			case 7:
//				isStandingFront = false;
				break;
			default:
				break;		
		}
	return false;
}

bool HumanoidMovements::standUpBack()
{
	
	switch (StandUpState)
		{
			case 0:
				if( setBothHand(-90,1,15) & setBothHand(0,4,5)
					& setBothHand(0,3,5) & setBothFoot(-20,5,15)
					& setBothFoot(0,4,5) & setBothFoot(0,3,5)
					& setBothFoot(0,2,5) & setBothFoot(0,1,5)
					& setBothFoot(0,6,5) )
					StandUpState = 1;
				break;
			case 1:
				if( setBothFoot(90,3,100)& setBothHand(0,1,100) )
					StandUpState = 2;
				break;
			case 2:
				if( setBothFoot(90,3,100) & setBothHand(0,1,100)
					& setRightFoot(-60,1,100) & setLeftFoot(-60,1,100))
					StandUpState = 3;
				break;
			case 3:
				if( setBothFoot(-90,4,100)& setBothHand(0,1,50) & setBothFoot(100,3,100)
					& setRightFoot(-60,1,100) & setLeftFoot(-60,1,100) )
					StandUpState = 4;
				break;
			case 4:
				if( setBothHand(0,1,10) & setBothHand(0,4,10) & setBothHand(0,2,10)
					& setBothHand(0,3,10) & setBothFoot(0,5,10)
					& setBothFoot(0,4,10) & setBothFoot(0,3,10)
					& setBothFoot(0,2,10) & setBothFoot(0,1,10)
					& setBothFoot(0,6,10) )
						StandUpState = 5;
				break;
			case 5:
				StandUpState = 0;
				return true;
			break;
			default:
				break;		
		}
	return false;
}

bool HumanoidMovements::delay_act(double t)
{
	 
	if (!onDelay)
	{
		lastDelayTime=WM->getTime();
		onDelay = true;
	}
	
	if ( t>WM->getTime()-lastDelayTime ) 
	{
		return false;
	}
	else
	{
	onDelay = false;
	return true;
	}
}

bool HumanoidMovements::dive(JointSide side, double t)
{
	//cout<<shootState<<endl;
	double error = 5 ;
	//bool s;
	if(side == JS_Left)
	{
		if(diveState == 1)
		if( 	setBothHand(90,1,60,error) & 
				setBothHand(-0,2,60,error) &
				setBothHand(0,4,60,error)	& 
				setBothHand(0,3,60,error) & 				
				setLeftFoot(-10,4,10,error)  	& setRightFoot(-0,4,60,error) &
		      	setLeftFoot(-30,2,60,error)	& setRightFoot(-30,2,60,error)	&
		     	setLeftFoot(5,5,60,error)	& setRightFoot(0,5,50,error)	&
				setLeftFoot(5,3,60,error)	& setRightFoot(0,3,50,error)	&	
				setLeftFoot(30,6,60,error) 	& setRightFoot(30,6,60,error))
		diveState = 2;
	
	if(diveState == 2)
			if(	setBothHand(90,1,50,error) & 
				setBothHand(-0,2,50,error) &
				setBothHand(0,4,50,error)	& 
				setBothHand(0,3,50,error) & 
				setAllFoot(0,1.7/t,error)
			) {diveState =0;return true;}
	}
	else
	{
	if(diveState == 1)
		if( 	setBothHand(90,1,60,error) & 
				setBothHand(-0,2,60,error) &
				setBothHand(0,4,60,error)	& 
				setBothHand(0,3,60,error) & 				
				setLeftFoot(-0,4,10,error)  	& setRightFoot(-10,4,60,error) &
		      	setLeftFoot(30,2,60,error)	& setRightFoot(30,2,60,error)	&
		     	setLeftFoot(0,5,10,error)	& setRightFoot(5,5,50,error)	&
				setLeftFoot(0,3,10,error)	& setRightFoot(5,3,50,error)	&	
				setLeftFoot(-30,6,60,error) 	& setRightFoot(-30,6,60,error))
		diveState = 2;
	
	if(diveState == 2)
			if(	setBothHand(90,1,50,error) & 
				setBothHand(-0,2,50,error) &
				setBothHand(0,4,50,error)	& 
				setBothHand(0,3,50,error) & 
				setAllFoot(0,1.7/t,error)
			) {diveState =0;return true;}
	}
	
	return false;
}
	
bool HumanoidMovements::walk(double speed,double div)
{
	VecPosition vecl,vecr;
	double rg =0,lg =0;
	if(walkState == 1)
	{
		left->reset(1);
		right->reset(0);
		left->active();
		right->sleep();
		Body->setFoots(VecPosition(),VecPosition());
	}
	else
	{
		vecr = right->getNextPosition();
		vecr.setX(vecr.getX()*speed);
		vecl = left->getNextPosition();
		vecl.setX(vecl.getX()*speed);
		Body->setFoots(
				Body->getRightFoot()->getEnd()+vecr,
				Body->getLeftFoot()->getEnd()+vecl);
		if(fabs(div) > 0.1)
		{
			if(div < 0) lg = fabs(div);
			else rg = fabs(div);
		}
	
		if( right->isMiddel())
		{
			Body->getLeftFoot()->setLeg(-lg*3.5);
			Body->getRightFoot()->setLeg(-lg*3.5);
		}
		if( left->isMiddel())
		{
			Body->getLeftFoot()->setLeg(-rg*3.5);
			Body->getRightFoot()->setLeg(-rg*3.5);
		}
	
		if(left->isFinished())
		{
			right->active();
			left->sleep();
			left->reset(0);
		}
		if(right->isFinished())
		{
			left->active();
			right->sleep();
			right->reset(0);
		}
	}
	driveWalk();
	
	return (left->isMiddel());
}

bool HumanoidMovements::driveWalk()
{
	double error  = 3,
		speed = 10;
	int j;
	bool b = true;
	switch (walkState)
	{
		case 1:
			for(int i = 3;i<6;i++)
				b = b &setBothFoot(Body->getLeftFoot()->getLeg(i),i,speed,error);
			if(b)
				walkState = 2;				
		break;
		
		case 2:
			for(int i = 1;i<7;i++)
			{
				if(i == 5)
					j = 9;
				else j= 0;
				goLeftFoot(j+Body->getLeftFoot()->getLeg(i),i);
				goRightFoot(j+Body->getRightFoot()->getLeg(i),i);
			}
		break;
	}
	return true;
}

//====================
bool HumanoidMovements::shoot9_1(double speed,double div)
{
	VecPosition vecl,vecr;
	double rg =0,lg =0;
	if(walkState == 1)
	{
		left->reset(1);
		right->reset(0);
		left->active();
		right->sleep();
		Body->setFoots(VecPosition(),VecPosition());
	}
	else
	{
		vecr = right->getNextPosition();
		vecr.setX(vecr.getX()*speed);
		vecl = left->getNextPosition();
		vecl.setX(vecl.getX()*speed);
		Body->setFoots(
				Body->getRightFoot()->getEnd()+vecr,
				Body->getLeftFoot()->getEnd()+vecl);
		if(fabs(div) > 0.1)
		{
			if(div < 0) lg = fabs(div);
			else rg = fabs(div);
		}
	
		if( right->isMiddel())
		{
			Body->getLeftFoot()->setLeg(-lg*3.5);
			Body->getRightFoot()->setLeg(-lg*3.5);
		}
		if( left->isMiddel())
		{
			Body->getLeftFoot()->setLeg(-rg*3.5);
			Body->getRightFoot()->setLeg(-rg*3.5);
		}
	
		if(left->isFinished())
		{
			right->active();
			left->sleep();
			left->reset(0);
		}
		if(right->isFinished())
		{
			left->active();
			right->sleep();
			right->reset(0);
		}
	}
	driveWalk();
	
	return (left->isMiddel());
}

bool HumanoidMovements::driveShoot()
{
	double error  = 3,
		speed = 10;
	int j;
	bool b = true;
	switch (walkState)
	{
		case 1:
			for(int i = 3;i<6;i++)
				b = b &setBothFoot(Body->getLeftFoot()->getLeg(i),i,speed,error);
			if(b)
				walkState = 2;				
		break;
		
		case 2:
			for(int i = 1;i<7;i++)
			{
				if(i == 5)
					j = 9;
				else j= 0;
				goLeftFoot(j+Body->getLeftFoot()->getLeg(i),i);
				goRightFoot(j+Body->getRightFoot()->getLeg(i),i);
			}
		break;
	}
	return true;
}

bool HumanoidMovements::shoot(JointSide side)
{
	if(side == JS_Left)
	switch (shootState)
	{
		case 1:
			if(setBothFoot(0,1,10,2)&
			setBothFoot(10,2,10,4)&
			setBothFoot(-15,6,10,4)&
			setBothFoot(-60,4,10,10)&	
			setBothFoot(30,3,10,10)&	
			setBothFoot(30,5,10,10))
			shootState = 2;			
		
		break;
		case 2:
			if(//setRightFoot(25,3,99,10)&
			setLeftFoot(-40,4,40,10)&
			setLeftFoot(60,3,40,10)&
			setLeftFoot(-20,5,40,10)&
			setRightFoot(20,3,40,10))
			shootState = 3;		
		break;
		case 3:
			if(setBothFoot(-60,4,10,5)&	
			setBothFoot(30,3,10,5)&	
			setBothFoot(30,5,10,5)&
			setBothFoot(0,2,10,2)&
			setBothFoot(0,6,10,2))
			{
				shootState = 0;
				return true;
			}				
		break;
	}
	else 
	switch(shootState)
		{
		case 1:
			if(setBothFoot(0,1,10,2)&
				setBothFoot(-10,2,10,4)&
			setBothFoot(15,6,10,4)&
			setBothFoot(-60,4,10,10)&	
			setBothFoot(30,3,10,10)&	
			setBothFoot(30,5,10,10))
			shootState = 2;			
		
		break;
		
		case 2:
			if(//setRightFoot(25,3,99,10)&
			setRightFoot(-40,4,40,10)&
			setRightFoot(60,3,40,10)&
			setRightFoot(-20,5,40,10)&
			setLeftFoot(20,3,40,10))
			shootState = 3;		
		break;
		case 3:
			if(setBothFoot(-60,4,10,5)&	
			setBothFoot(30,3,10,5)&	
			setBothFoot(30,5,10,5)&
			setBothFoot(0,2,10,2)&
			setBothFoot(0,6,10,2))
			{
				shootState = 0;
				return true;
			}				
		break;
	}
		
	return false;	
}
//==========================do not work well===================
bool HumanoidMovements::shootBefore(JointSide side,double kickDistan)
{
	//cout<<shootState<<endl;
	double error = 5 ;
	//bool s;
	if(1)//(side == JS_Left)//side == JS_Left)
	{
		if(shootState == 1)
		if( 	setBothHand(-90,1,5,error) & 
				setBothHand(50,4,5,error)	& 
				setBothHand(90,3,5,error) & 				
				setLeftFoot(-20,4,10,error)  	& setRightFoot(-20,4,10,error) &
		      	setLeftFoot(10,2,10,error)	& setRightFoot(10,2,10,error)	&
		     	setLeftFoot(10,5,10,error)	& setRightFoot(10,5,10,error)	&
				setLeftFoot(10,3,10,error)	& setRightFoot(10,3,10,error)	&	
				setLeftFoot(-10,6,10,error) 	& setRightFoot(-10,6,10,error))
		shootState = 2;
	
	if(shootState == 2)
			if(	setLeftFoot(40,3,40,error*2) & setLeftFoot(-50,4,40,error*2)
				& setLeftFoot(10,5,4,error*2)
			) shootState = 3;
	if(shootState == 3)
	{
		//setLeftFoot(80,3,kickDistan,error) ;
		//setLeftFoot(-40,4,kickDistan,error);
		//setLeftFoot(0,5,kickDistan,error);
		//goFoot(0,5,false);
		//goFoot(0,4,false);
		//goFoot(80,3,false);
		if(	setLeftFoot(90,3,kickDistan*10,error) &
				setLeftFoot(-10,4,kickDistan*10,error) & 
				setLeftFoot(-20,5,kickDistan*10,error)) shootState = 4;
			//goFoot(40,3,false);
	}
	if(shootState ==4)
			if(	setLeftFoot(40,3,10,error) & 
				setLeftFoot(-80,4,10,error)& 
				setLeftFoot(40,5,10,error)
				) shootState = 5;
	if(shootState ==5) if( setAllFoot(0,5,error))	shootState =6;
	if(shootState ==6)	if( WM->isStable())	{shootState =0;return true;}
	}
	else
	{
	if(shootState == 1)
		if( 	setBothHand(-90,1,5,error) & 
				setBothHand(50,4,5,error)	& 
				setBothHand(90,3,5,error) & 				
				setLeftFoot(-20,4,10,error)  	& setRightFoot(-20,4,10,error) &
		      	setLeftFoot(-10,2,10,error)	& setRightFoot(-10,2,10,error)	&
		     	setLeftFoot(10,5,10,error)	& setRightFoot(10,5,10,error)	&
				setLeftFoot(10,3,10,error)	& setRightFoot(10,3,10,error)	&	
				setLeftFoot(10,6,10,error) 	& setRightFoot(10,6,10,error))
		shootState = 2;
	
	if(shootState == 2)
			if(	setRightFoot(60,3,10,error) & setRightFoot(-40,4,10,error)
				& setRightFoot(-50,5,5,error)
			) shootState = 3;
	if(shootState == 3)
	{
		//setLeftFoot(80,3,kickDistan,error) ;
		//setLeftFoot(-40,4,kickDistan,error);
		//setLeftFoot(0,5,kickDistan,error);
		//goFoot(0,5,false);
		//goFoot(0,4,false);
		//goFoot(80,3,false);
		if(	setRightFoot(80,3,kickDistan*10,error) &
				setRightFoot(-20,4,kickDistan*10,error) & 
				setRightFoot(-20,5,kickDistan*10,error)) shootState = 4;
			//goFoot(40,3,false);
	}
	if(shootState ==4)
			if(	setRightFoot(50,3,10,error) & 
				setRightFoot(-100,4,10,error)& 
				setRightFoot(50,5,10,error)
				) shootState = 5;
	if(shootState ==5) if( setAllFoot(0,5,error))	shootState =6;
	if(shootState ==6)	if( WM->isStable())	{shootState =0;return true;}
	}
	return false;
}
