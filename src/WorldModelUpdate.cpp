#include "WorldModel.h"
#include <fstream>
#include <iostream>

bool WorldModel::update (string s)
{
	//(time (now 9.48))
	
	//(GS (t 2.44) (pm KickOff_Left))
	//(GS (unum 1) (team left) (t 0.00) (pm BeforeKickOff))
	
	//(GYR (n torso) (rt -0.00 -0.00 0.03))
	
	//(See (F1L (pol 21.95 133.70 -8.33)) (F2L (pol 22.38 -132.62 -8.16)) 
	//(F1R (pol 38.49 24.16 -4.73)) (F2R (pol 38.74 -24.97 -4.70)) 
	//(G1L (pol 15.98 164.36 -12.84)) (G2L (pol 16.14 -162.25 -12.70)) 
	//(G1R (pol 35.43 6.85 -5.75)) (G2R (pol 35.51 -7.81 -5.74)) 
	//(B (pol 10.49 -1.72 -17.49)))
	
	//(UJ (n laj1_2) (ax1 0.00) (ax2 -0.00))
	//(UJ (n raj1_2) (ax1 -0.00) (ax2 0.00))(HJ (n laj3) (ax 0.00))
	//(HJ (n raj3) (ax -0.00))(HJ (n laj4) (ax 0.00))(HJ (n raj4) (ax -0.00))
	//(HJ (n llj1) (ax -0.00))(HJ (n rlj1) (ax 0.00))
	//(UJ (n llj2_3) (ax1 0.00) (ax2 -0.01))
	//(UJ (n rlj2_3) (ax1 0.00) (ax2 -0.00))(HJ (n llj4) (ax 0.00))
	//(HJ (n rlj4) (ax -0.00)) (UJ (n llj5_6) (ax1 -0.00) (ax2 0.02))
	//(UJ (n rlj5_6) (ax1 -0.00) (ax2 0.00))
	
	//(FRP (n lf) (c 0.10 0.09 -0.05) (f -0.39 -0.41 12.29))
	//(FRP (n rf) (c -0.10 -0.09 -0.05) (f 0.40 0.40 12.22))
	
	char m[10000];
	strcpy (m,s.c_str());
	char str[2000];
	unsigned int i , j , k  = 0;
	bool isnew = true;
	while (k < strlen(m) - 1)
	{
		j = 0;
		for (i= 0 ;; i++)
		{
			str[i] = m[k];
			if(str[i] == ')') j--;
			if(str[i] == '(') j++;
			str[i+1] =0;
			k++;
			if(j==0)
				break;
		}
		if(!isnew) break;
		switch(str[2])
		{
			case  'i' : //(time
				isnew = updateTime(str);
				break;
			case  'e' : //(See
				updateVision(str);
				break;
			case  'J' : //(UJ
				updateJoint(str);
				break;
			case  'Y' : //(GYR
				updateTorso(str);
				break;
			case  'S' : //(GS
				updateGameState(str);
				break;
			case  'R' : //(FRP
				updateFRP(str);
				break;
			default :
				cout<<"Unkown Command : "<<str<<endl;	
		}
		
	}
	if(begin && !initialed)
		initialComponents();
	//findMyPanTilt();
	findGlobalPositions();
	return isnew;
}
//================================================

bool WorldModel::updateTime (char *str)
{
	stringstream ss (str);
	//(time (now 521.85))
	char c[20];
	double t;
	ss >> c >> c >> t;

	if(t != time)
	{
		setData();
		time = t;
		return true;
	}
	return false;
}

void WorldModel::updateFRP(char *s)
{
	//cout<<"FRP"<<s<<endl;
	//(FRP (n rf) (c -0.08 0.09 -0.05) (f 0.30 -0.30 12.30))
	stringstream ss (s);
	char c[30];
	ss>>c>>c>>c;
	if(c[0] == 'r')
		RightOnGround = true;
	else
		LeftOnGround = true;
}

void WorldModel::updateGameState(char *s)
{
	
	//(GS (unum 1) (team left) (t 0.00) (pm BeforeKickOff))
	//cout<<"GS"<<s<<endl;
	stringstream ss (s);
	char c[30];
	double Gtime;
	int no;
	ss>>c;
	while(ss>>c)
	{
		if(c[1] == 'u')//(unum 1)
		{
			ss>>no>>c[0];
			playerNumber = no;
		}
		else if(c[2] == 'e')//(team left)
		{
			ss>>c;
			begin = true;
			if(!strcmp(c,"left)"))
				side = S_LEFT;
			else	
				side = S_RIGHT;
			
			
		}
		else if(c[1] == 'p')// (pm BeforeKickOff))
		{
			ss>>c;
			for(int i= 0;i<int(strlen(c));i++)
				if(c[i] == ')')
					c[i] ='\0';
			setPlayMode ( string ( c ) );
		}
		else if(c[1] == 't')// (t 0.00)
		{
			ss>>Gtime>>c[0];
			gameTime = Gtime;
		}
	}
}
void WorldModel::updateTorso (char *str)
{
	lastTorso = torso;
	stringstream ss (str);
	//cout<<"Torso"<<str<<endl;
	//(GYR (n torso) (rt -0.00 -0.00 -0.00))
	char c[20];
	double x,y,z;
	ss >> c >> c >>c>>c>> x>>y>>z;
	torso = Torso(x,y,z);
}


void WorldModel::updateVision (char *s)
{
	char m[10000];
	strcpy (m,s+5);
	char str[2000];
	unsigned int i , j , k  = 0;
	while (k < strlen(m) - 1)
	{
		j = 0;
		for (i= 0 ;; i++)
		{
			str[i] = m[k];
			if(str[i] == ')') j--;
			if(str[i] == '(') j++;
			str[i+1] =0;
			k++;
			if(j==0)
			{
				k++;
				break;
			}
		}
		setVision(str);
	}		
}

void WorldModel::setVision (char *str)
{
//	(F1L (pol 31.71 126.69 -5.23))
//	(F2L (pol 14.84 -154.91 -11.60))
//	(F1R (pol 45.88 48.47 -4.55))
//	(F2R (pol 36.33 4.36 -5.89))
//	(G1L (pol 22.14 140.22 -8.57))
//	(G2L (pol 16.41 161.30 -11.69))
//	(G1R (pol 39.87 35.30 -5.82))
//	(G2R (pol 37.00 22.49 -6.32))
//	(B (pol 18.02 54.43 -10.59))

	stringstream ss(str);
	char c[20],d[20];
	int no = 1;
	bool forg = false,sside = false;
	double a1,a2,a3;
	ss>>c;
//	cout<<str<<endl;
	switch(c[1])
		{
		case 'P'://(P (team Scorpius) (id 7) (pol 8.32 0.29 -0.75))
			ss>>d>>c>>d>>no>>d>>d>>a1>>a2>>a3;	
			c[strlen(c)-1] = 0;
			if(!strcmp(c,getTeamName().c_str()))
				{
				setPlayerRelPosition(no,VecPosition(a1,a2,a3,KORAVI)
				,true);
				}
			else
			{
				setOppTeamName(c);
				setPlayerRelPosition(no,VecPosition(a1,a2,a3,KORAVI)
				,false);
			}
			break;
		case 'B':
			ss>>d>>a1>>a2>>a3;
			relBall = VecPosition(a1,a2,a3,KORAVI);
			break;
		case 'F':
			ss>>d>>a1>>a2>>a3;
			forg = true;
			if(c[3] == 'R')
				sside = true;
			if(c[2] == '2')
				no = 2 ;
			setRelativeFlag(VecPosition(a1,a2,a3,KORAVI),forg,sside,no-1);
			break;
		case 'G':
			ss>>d>>a1>>a2>>a3;
			forg = false;
			if(c[3] == 'R')
				sside = true;
			if(c[2] == '2')
				no = 2 ;
			setRelativeFlag(VecPosition(a1,a2,a3,KORAVI),forg,sside,no-1);
			break;
		default :
			;
			//cout<<"Unknown Object : "<<str<<endl;	
		}
}

void WorldModel::updateJoint (char *str)
{
	//(UJ (n laj1_2) (ax1 0.01) (ax2 0.04))
	//(HJ (n laj3) (ax 0.00))
	//(HJ (n hj1) (ax -0.00))
	stringstream ss(str);
	char c[200];
	int i,j;
	bool side = true,ish = true;
	double a,r=0;
	ss>>c>>c>>c[0]>>c[1];
	if(c[0] == 'l') 
		side = false;
	if(c[0] == 'h')
	{
		ss >> i >> c[0] >> c >> a;
		updateHeadJoint(i,a);
		return;
	}
	if(c[1] == 'l') 
		ish = false;
	ss>>c[3];
	if(str[1] == 'U')
		
	{
		ss >> i >> c[0] >> j >> c[0] >> c >> a >>c;
		updateJoint (side,ish,a,r,i);
		ss >> c >> a ;
		updateJoint(side,ish,a,r,j);
	}
	else
	{
     		ss >> i >> c[0] >> c >> a;
		updateJoint (side,ish,a,r,i);
	}
}



bool	WorldModel::isInField(VecPosition vec)
{
	return ( vec.isBetweenX(-(FIELD_LENGTH/2)*1.1 ,(FIELD_LENGTH/2)*1.1) 	&& 
	vec.isBetweenY((-FIELD_WIGHTH/2)*1.1,(FIELD_WIGHTH/2 )*1.1) );
}

VecPosition	WorldModel::findMyPositionFromFlag(int i)
{
	if((i < 0 )||(i>7))
	{
		cout<<"Invalid Flag "<<i<<"\n";
		return VecPosition(-1000,-1000);
	}
	
	VecPosition pos(relFlag[i].getMagnitude(),
	relFlag[i].getPhiDirection()+panAngle,
	relFlag[i].getTetaDirection()+tiltAngle,KORAVI);
	return ( flag[i]  - pos ) ;
}
