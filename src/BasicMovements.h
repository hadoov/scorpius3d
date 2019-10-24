#ifndef __BASIC_MOVEMENTS_H__
#define __BASIC_MOVEMENTS_H__

#include "WorldModel.h"
#include <string>
#include <sstream>

class BasicMovements
{
	protected:
		
		WorldModel *WM;
		bool  beamTime;
		VecPosition beamPosition;
		ofstream *log;	
	
	private :
		
		double 	fAng[6][2];
		double 	hAng[6][2];
		double 	flast[6][2];
		double 	hlast[6][2];
		string commandQueue;
		
	
	public:
		
		BasicMovements			( 				);
		bool	printLog		( string		);
		bool	printLog		( double		);
		bool	printLog		( char *		);
	//	bool	printLog		( void			);
		void    setWorldModel	( WorldModel* wm);
		void    resetComponents (				);
		void 	beam			(VecPosition 	vec );
		void    beam			(VecPosition , double angle);
		void	beam 			(double x,double y,double phi);

		void	rotateFoot		( double ang , int no , bool side );
		void	rotateHand		( double ang , int no , bool side );
		void	rotateBothFoot	( double ang , int no );
		void	rotateBothHand	( double ang , int no );
		void    rotateLeftFoot	( double ang , int no );
		void    rotateRightFoot	( double ang , int no );
		void    rotateLeftHand	( double ang , int no );
		void    rotateRightHand	( double ang , int no );
		bool	setFoot		( double ang , int no 
								, bool side , double rate = 1  ,double = 1	);
		bool	setHand		( double ang , int no 
							, bool side , double rate = 1  ,double = 1	);
		bool	setAllFoot  ( double ang , 			double rate = 1 ,double = 1 );
		bool	setAllLeftFoot  ( double ang , 			double rate = 1 ,double = 1 );
		bool	setAllRightFoot  ( double ang , 			double rate = 1 ,double = 1 );
		bool	setAllHand  (double ang ,double rate = 1 , double error = 1 );
		bool	setAll  (double ang ,double rate = 1 , double error = 1 );
		bool	setBothFoot	( double ang , int no , double rate = 1 ,double = 1	);
		bool	setBothHand	( double ang , int no , double rate = 1 ,double = 1	);
		bool    setLeftFoot	( double ang , int no , double rate = 1 ,double = 1	);
		bool    setRightFoot( double ang , int no , double rate = 1 ,double = 1	);
		bool    setLeftHand	( double ang , int no , double rate = 1 ,double = 1	);
		bool    setRightHand( double ang , int no , double rate = 1 ,double = 1	);
	
		bool	setRightAnkle_1 ( double angle , double rate = 1 ,double = 1	);
		bool	setRightAnkle_2 ( double angle , double rate = 1 ,double = 1	);
		bool	setRightKnee    ( double angle , double rate = 1 ,double = 1	);
		bool	setRightHip_1   ( double angle , double rate = 1 ,double = 1	);
		bool	setRightHip_2   ( double angle , double rate = 1 ,double = 1	);
	
		bool	setLeftAnkle_1	( double angle , double rate = 1 ,double = 1	);
		bool	setLeftAnkle_2	( double angle , double rate = 1 ,double = 1	);
		bool	setLeftKnee		( double angle , double rate = 1 ,double = 1	);
		bool	setLeftHip_1	( double angle , double rate = 1 ,double = 1	);
		bool	setLeftHip_2	( double angle , double rate = 1 ,double = 1	);

		bool	setBothAnkle_1	( double angle , double rate = 1 ,double = 1	);
		bool	setBothAnkle_2	( double angle , double rate = 1 ,double = 1	);
		bool	setBothKnee		( double angle , double rate = 1 ,double = 1	);
		bool	setBothHip_1	( double angle , double rate = 1 ,double = 1	);
		bool	setBothHip_2	( double angle , double rate = 1 ,double = 1	);
	
		bool	setRightSoulder	( double angle , double rate = 1 ,double = 1	);
		bool	setRightElbow	( double angle , double rate = 1 ,double = 1	);
		bool 	setLeftSoulder	( double angle , double rate = 1 ,double = 1	);
		bool 	setLeftElbow	( double angle , double rate = 1 ,double = 1	);
		string  getCommand		();
		
		
		//.....................Go Functions..........................
		void	goFoot         (double ang ,int no ,bool side);
		void	goHand         (double ang ,int no ,bool side);
		void	goBothFoot     (double ang ,int no  );
		void	goBothHand     (double ang ,int no  );
		void    goLeftFoot     (double ang ,int no  );
		void    goRightFoot    (double ang ,int no  ); 
		void    goLeftHand     (double ang ,int no  );
		void    goRightHand    (double ang ,int no  );
		void    goAll          (double ang  );
};
#endif
