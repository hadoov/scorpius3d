#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <cmath>  
#include <stdarg.h>
#include <string>  
#include <vector>
#include <iostream>

using namespace std;

typedef double AngRad;
typedef double AngDeg;

#define EPSILON 0.01
#define sq(a) ((a)*(a))
double max     ( double d1, double d2 );
double min     ( double d1, double d2 );
int    sign    ( double d1            );
bool	Equal  ( double d1, double d2 );

AngDeg normalizeAngle ( AngDeg angle  );
AngDeg Rad2Deg ( AngRad x             );
AngRad Deg2Rad ( AngDeg x             );

double cosDeg  ( AngDeg x             );
double sinDeg  ( AngDeg x             );
double tanDeg  ( AngDeg x             );
AngDeg atanDeg ( double x             );
double atan2Deg( double x,  double y  );
AngDeg acosDeg ( double x             );
AngDeg asinDeg ( double x             );

// various goniometric functions
bool   isAngInInterval     ( AngDeg ang,    AngDeg angMin,    AngDeg angMax );
AngDeg getBisectorTwoAngles( AngDeg angMin, AngDeg angMax );
int    abcFormula(double a,double b, double c, double *s1,double *s2);

enum CoordSystemT {
  CARTESIAN,
  POLAR,
  KORAVI
  
};

/*****************************************************************************/
/********************   CLASS VECPOSITION   **********************************/
/*****************************************************************************/

class VecPosition
{
	friend VecPosition operator * ( double, const VecPosition &);
 	friend ostream& operator << ( ostream & , VecPosition );

private:

  double m_x; 
  double m_y;
  double m_z;

public:
  VecPosition  ( double            vx = 0, double            vy = 0,double vz =0,
                                              CoordSystemT      cs =CARTESIAN);

  VecPosition        operator -             (                                );
  VecPosition        operator +             ( const double      &d           );
  VecPosition        operator +             ( const VecPosition &p           );
  VecPosition        operator -             ( const double      &d           );
  VecPosition        operator -             ( const VecPosition &p           );
  VecPosition        operator *             ( const double      &d           );
  VecPosition        operator *             ( const VecPosition &p           );
  double             operator &             ( const VecPosition &p           );
  VecPosition        operator /             ( const double      &d           );
  VecPosition        operator /             ( const VecPosition &p           );
  void               operator =             ( const double      &d           );
  void               operator +=            ( const VecPosition &p           );
  void               operator +=            ( const double      &d           );
  void               operator -=            ( const VecPosition &p           );
  void               operator -=            ( const double      &d           );
  void               operator *=            ( const VecPosition &p           );
  void               operator *=            ( const double      &d           );
  void               operator /=            ( const VecPosition &p           );
  void               operator /=            ( const double      &d           );
  bool               operator !=            ( const VecPosition &p           );
  bool               operator !=            ( const double      &d           );
  bool               operator ==            ( const VecPosition &p           );
  bool               operator ==            ( const double      &d           );

  // methods for producing output
  string             str                    ( CoordSystemT      cs =CARTESIAN);

  // set- and get methods for private member variables
  void               setX                   ( double            dX           );
  double             getX                   (                          ) const;
  void               setY                   ( double            dY           );
  double             getY                   (                          ) const;
  void               setZ                   ( double            dZ           );
  double             getZ                   (                          ) const;

  // set- and get methods for derived position information
  void               setVecPosition         ( double            dX = 0,
                                              double            dY = 0,
                                              double            dZ = 0,
                                              CoordSystemT      cs =CARTESIAN);
  double             getDistanceTo          ( const VecPosition p            );
  VecPosition        setMagnitude           ( double            d            );
  double             getMagnitude           (                          ) const;
  AngDeg             getPhiDirection        (                          ) const;
  AngDeg             getTetaDirection       (                          ) const;

  // comparison methods for positions
  bool               isInFrontOf            ( const VecPosition &p           );
  bool               isInFrontOf            ( const double      &d           );
  bool               isBehindOf             ( const VecPosition &p           );
  bool               isBehindOf             ( const double      &d           );
  bool               isLeftOf               ( const VecPosition &p           );
  bool               isLeftOf               ( const double      &d           );
  bool               isRightOf              ( const VecPosition &p           );
  bool               isRightOf              ( const double      &d           );
  bool               isBetweenX             ( const VecPosition &p1,
                                              const VecPosition &p2          );
  bool               isBetweenX             ( const double      &d1,
                                              const double      &d2          );
  bool               isBetweenY             ( const VecPosition &p1,
                                              const VecPosition &p2          );
  bool               isBetweenY             ( const double      &d1,
                                              const double      &d2          );

  // conversion methods for positions
  VecPosition        normalize              (                                );
  VecPosition        rotate                 ( AngDeg            angle        );

  VecPosition        getVecPositionOnLineFraction( VecPosition  &p,
                                              double            dFrac        );

  // static class methods
  static VecPosition getVecPositionFromPolar( double        ,
                                              AngDeg ,     double     );
  
  static VecPosition getVecPositionFromKORAVI( double        ,
                                              AngDeg ,    AngDeg   );
  VecPosition 		 Cross					(const 	VecPosition 	&v);

};

/*****************************************************************************/
/********************** CLASS CIRCLE *****************************************/
/*****************************************************************************/

class Circle
{
    VecPosition m_posCenter;
    double      m_dRadius; 

public:
    Circle( );
    Circle( VecPosition pos, double dR );

    // get and set methods
    bool        setCircle             ( VecPosition pos,
                                        double      dR  );
    bool        setRadius             ( double dR       );
    double      getRadius             (                 );
    void        setCenter             ( VecPosition pos );
    VecPosition getCenter             (                 );
    double      getCircumference      (                 );
    double      getArea               (                 );

    // calculate intersection points and area with other circle
    bool        isInside              ( VecPosition pos );
    int         getIntersectionPoints ( Circle      c,
                                        VecPosition &p1,
                                        VecPosition &p2 );
    double      getIntersectionArea   ( Circle c        );


}  ;

/*****************************************************************************/
/*********************** CLASS LINE ******************************************/
/*****************************************************************************/

class Line
{
	/* ay + bx + c = 0 */
  double m_a; 
  double m_b;
  double m_c;

public:
  Line(){}
  Line( double a, double b, double c );

  Line ( VecPosition pos1, VecPosition pos2 );

  Line ( VecPosition vec, AngDeg angle );

  // print methods

  // get intersection points with this line
  bool		  isParallel				 ( Line		   line				   	 );
  int         getIntersection            ( Line        line, 
		  								   VecPosition &p);
  int         getCircleIntersectionPoints( Circle      circle,
                                           VecPosition &posSolution1,
                                           VecPosition &posSolution2         );
  Line        getTangentLine             ( VecPosition pos           );
  Line        getPerpendicularLine       ( VecPosition pos           );
  VecPosition getPointOnLineClosestTo    ( VecPosition pos                   );
  double      getDistanceWithPoint       ( VecPosition pos                   );
  bool        isInBetween                ( VecPosition pos,
					   VecPosition point1,
					   VecPosition point2                );

  static Line makeLineFromTwoPoints      ( VecPosition pos1,
					   VecPosition pos2                  );

  // calculate associated variables in the line
  double      getYGivenX                ( double      x );
  double      getXGivenY                ( double      y );
  double      getA         				(               ) const;
  double      getB						(               ) const;
  double      getC						(               ) const;
};

/*****************************************************************************/
/********************** CLASS SEGMENT ****************************************/
/*****************************************************************************/

class Segment
{
		VecPosition p1,p2;
	public:
		Segment(VecPosition, VecPosition);
  		int getIntersection ( Segment, VecPosition &);
		int getIntersection ( Line, VecPosition &);
		bool isOn(VecPosition p);
};

/*****************************************************************************/
/********************** CLASS POLYGAN ****************************************/
/*****************************************************************************/
class Poly
{
  vector <VecPosition> corners;
public:
//  Poly (int, ...);
  Poly ();
  Poly ( vector <VecPosition>);


  void setCorners ( vector <VecPosition>);
  vector <VecPosition> getCorners();

  vector <VecPosition>  getIntersection(Line l);
  vector<VecPosition> getIntersection(Segment s);
  bool isInside ( VecPosition &pos );
  double getArea();
};
/*****************************************************************************************************************************************************************************************************************************************Cone Class****************************************************/
class Cone : public VecPosition
{
    public :			
	
	Cone ( VecPosition Start,VecPosition PointLeft, VecPosition PointRight );
	void setStartPoint ( VecPosition st );
	void setStartPoint ( double x,double y,double z);
	void setPointLeft  ( VecPosition st );
	void setPointLeft  ( double x,double y,double z);
	void setPointRight ( VecPosition st );
	void setPointRight ( double x,double y,double z);
	void setCone (VecPosition a, VecPosition b,VecPosition c);	
	VecPosition getStartPoint ( );
	double getXPositionStart  ( );
	double getYPositionStart  ( );	
	double getZPositionStart  ( );

	VecPosition getPointLeft  ( );
	double getXPositionLeft  ( );
	double getYPositionLeft  ( );	
	double getZPositionLeft  ( );
	

	VecPosition getPointRight ( );
	double getXPositionRight  ( );
	double getYPositionRight  ( );	
	double getZPositionRight  ( );

	bool chekPoint ( VecPosition );
	double BigZ  ( );
	double BigLR ( );
	
   private :
	VecPosition start,left,right;
};

double midAngle ( VecPosition center, VecPosition leftLeg, VecPosition rightLeg ); // nimsaz
//                   " midAngle : "
//      leftLeg          /            _ rightLeg
//           |         /         __ / 
//           |       /      __ / 
//           |     /   __ /  
//           |   /   / 
//           center


double pointsAngle ( VecPosition center, VecPosition leftLeg, VecPosition rightLeg );
//                   " pointsAngle : "
//      leftLeg                       _ rightLeg
//           |                  __ / 
//           |             __ / 
//           |         __ /  
//           |       / 
//           center


class Matrix
{
public:
    float m[16];

    inline Matrix(){}

    /** copy constructs the matrix from newMatrix, a pointer to a float array*/
    inline Matrix(float *newMatrix)   {       memcpy(m, newMatrix,    sizeof(float)*16);      }

    /** copy constructs the matrix from newMatrix, a reference to another Matrix*/
    inline Matrix(const Matrix &newMatrix)    {       memcpy(m, newMatrix.m,  sizeof(float)*16);      }

    /** copy constructs the matrix from newMatrix, a pointer to another Matrix*/
    inline Matrix(Matrix *newMatrix)                  {       memcpy(m, newMatrix->m, sizeof(float)*16);      }

    /** constructs the matrix from 16 float values */
    inline Matrix(float m00, float m01, float m02, float m03,
                    float m10, float m11, float m12, float m13,
                    float m20, float m21, float m22, float m23,
                    float m30, float m31, float m32, float m33);

	void setPos(VecPosition a){	m[12]=a.getX();
								m[13]=a.getY();
								m[14]=a.getZ();};
	VecPosition getPos (){return VecPosition (m[12],m[13],m[14]);};




    inline void Identity() {       
						for(int i = 0;i<4;i++)
							for(int j = 0;j<4;j++)
							{
								if(i == j)
									m[i*4+j] = 1;
								else
									m[i*4+j] = 0;
							}
						}
    
	inline  const VecPosition operator*(const VecPosition &inRHS) const; 
						/** returns a reference to a single element of the matrix */
   inline float& operator() (int inRow, int inColumn) {      return El(inRow, inColumn);     }


public:
    /** returns a reference to a single element of the matrix */
    inline float& El(int inRow, int inColumn) {       return m[inColumn*4 + inRow];   }

    /** returns a const reference to a single element of the matrix */
    inline const float& El(int inRow, int inColumn) const {   return m[inColumn*4 + inRow];   }

};

inline Matrix::Matrix(float m00, float m01, float m02, float m03,
                        float m10, float m11, float m12, float m13,
                        float m20, float m21, float m22, float m23,
                        float m30, float m31, float m32, float m33)
{
    m[0] = m00; m[4] = m01;     m[8] = m02;     m[12] = m03;
    m[1] = m10; m[5] = m11;     m[9] = m12;     m[13] = m13;
    m[2] = m20; m[6] = m21;     m[10]= m22;     m[14] = m23;
    m[3] = m30; m[7] = m31;     m[11]= m32;     m[15] = m33;
}

inline const VecPosition Matrix::operator*(const VecPosition &inRHS) const
{
    float x = inRHS.getX(), y = inRHS.getY(), z = inRHS.getZ();

    return VecPosition(x*m[0] + y*m[4] + z*m[8] + m[12],
                    x*m[1] + y*m[5] + z*m[9] + m[13],
                    x*m[2] + y*m[6] + z*m[10]+ m[14]);
}

#endif
