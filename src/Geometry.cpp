#include "Geometry.h"
#include <stdio.h> // needed for sprintf

int sign( double d1 )
{
  return (d1>0)?1:-1;
}

double max( double d1, double d2 )
{
  return (d1>d2)?d1:d2;
}

double min( double d1, double d2 )
{
  return (d1<d2)?d1:d2;
}

bool Equal(double d1, double d2)
{
	return (fabs(d1-d2)<EPSILON);
}
double sqr ( double d)
{
	return (d*d);
}
AngDeg normalizeAngle( AngDeg angle )
{
  while( angle > 180.0  ) angle -= 360.0;
  while( angle < -180.0 ) angle += 360.0;

  return ( angle );
}


AngDeg Rad2Deg( AngRad x )
{
  return ( x * 180 / M_PI );
}

AngRad Deg2Rad( AngDeg x )
{
  return ( x * M_PI / 180 );
}

double cosDeg( AngDeg x )
{
  return ( cos( Deg2Rad( x ) ) );
}

double sinDeg( AngDeg x )
{
  return ( sin( Deg2Rad( x ) ) );
}

double tanDeg( AngDeg x )
{
  return ( tan( Deg2Rad( x ) ) );
}

AngDeg atanDeg( double x )
{
  return ( Rad2Deg( atan( x ) ) );
}

double atan2Deg( double x, double y )
{
  if( fabs( x ) < EPSILON && fabs( y ) < EPSILON )
    return ( 0.0 );

  return ( Rad2Deg( atan2( x, y ) ) );
}

AngDeg acosDeg( double x )
{
  if( x >= 1 )
    return ( 0.0 );
  else if( x <= -1 )
    return ( 180.0 );

  return ( Rad2Deg( acos( x ) ) );
}

AngDeg asinDeg( double x )
{
  if( x >= 1 )
    return ( 90.0 );
  else if ( x <= -1 )
    return ( -90.0 );

  return ( Rad2Deg( asin( x ) ) );
}

/*! This function returns a boolean value which indicates whether the value
   'ang' (from interval [-180..180] lies in the interval [angMin..angMax].
    Examples: isAngInInterval( -100, 4, -150) returns false
             isAngInInterval(   45, 4, -150) returns true
    \param ang angle that should be checked
    \param angMin minimum angle in interval
    \param angMax maximum angle in interval
    \return boolean indicating whether ang lies in [angMin..angMax] */
bool isAngInInterval( AngDeg ang, AngDeg angMin, AngDeg angMax )
{
  // convert all angles to interval 0..360
  if( ( ang    + 360 ) < 360 ) ang    += 360;
  if( ( angMin + 360 ) < 360 ) angMin += 360;
  if( ( angMax + 360 ) < 360 ) angMax += 360;

  if( angMin < angMax ) // 0 ---false-- angMin ---true-----angMax---false--360
    return angMin < ang && ang < angMax ;
  else                  // 0 ---true--- angMax ---false----angMin---true---360
    return !( angMax < ang && ang < angMin );
}

/*! This method returns the bisector (average) of two angles. It deals
    with the boundary problem, thus when 'angMin' equals 170 and 'angMax'
    equals -100, -145 is returned.
    \param angMin minimum angle [-180,180]
    \param angMax maximum angle [-180,180]
    \return average of angMin and angMax. */
AngDeg getBisectorTwoAngles( AngDeg angMin, AngDeg angMax )
{
  // separate sine and cosine part to circumvent boundary problem
  return normalizeAngle(
            atan2Deg( (sinDeg( angMin) + sinDeg( angMax ) )/2.0,
                      (cosDeg( angMin) + cosDeg( angMax ) )/2.0 ) );
}

int abcFormula(double a, double b, double c, double *s1, double *s2)
{
  double dDiscr = b*b - 4*a*c;       // discriminant is b^2 - 4*a*c
  if (fabs(dDiscr) < EPSILON )       // if discriminant = 0
  {
    *s1 = -b / (2 * a);              //  only one solution
    return 1;
  }
  else if (dDiscr < 0)               // if discriminant < 0
    return 0;                        //  no solutions
  else                               // if discriminant > 0
  {
    dDiscr = sqrt(dDiscr);           //  two solutions
    *s1 = (-b + dDiscr ) / (2 * a);
    *s2 = (-b - dDiscr ) / (2 * a);
    return 2;
  }
}
/*****************************************************************************/
/*******************   CLASS VECPOSITION   ***********************************/
/*****************************************************************************/


VecPosition operator * ( double d, const VecPosition &v)
{
  return ( VecPosition( v.m_x * d, v.m_y * d, v.m_z * d  ) );
}


VecPosition::VecPosition( double x, double y,double z, CoordSystemT cs )
{
  setVecPosition( x, y,z, cs );
}

VecPosition VecPosition::operator - (  )
{
  return ( VecPosition( -m_x, -m_y, -m_z ) );
}

VecPosition VecPosition::operator + ( const double &d )
{
  return ( VecPosition( m_x + d, m_y + d, m_z + d ) );
}

VecPosition VecPosition::operator + ( const VecPosition &p )
{
  return ( VecPosition( m_x + p.m_x, m_y + p.m_y, m_z+ p.m_z ) );
}

VecPosition VecPosition::operator - ( const double &d )
{
  return ( VecPosition( m_x - d, m_y - d, m_z - d ) );
}

VecPosition VecPosition::operator - ( const VecPosition &p )
{
  return ( VecPosition( m_x - p.m_x, m_y - p.m_y, m_z - p.m_z ) );
}

VecPosition VecPosition::operator * ( const double &d  )
{
  return ( VecPosition( m_x * d, m_y * d, m_z * d  ) );
}

VecPosition VecPosition::operator * ( const VecPosition &p )
{
  return ( VecPosition( m_x * p.m_x, m_y * p.m_y, m_z * p.m_z ) );
}

double VecPosition::operator &(const VecPosition &p)
{
	return ( m_x * p.m_x+ m_y * p.m_y+ m_z * p.m_z );
}

VecPosition VecPosition::operator / ( const double &d )
{
  return ( VecPosition( m_x / d, m_y / d, m_z / d  ) );
}

VecPosition VecPosition::operator / ( const VecPosition &p )
{
  return ( VecPosition( m_x / p.m_x, m_y / p.m_y, m_z / p.m_z ) );
}

void VecPosition::operator = ( const double &d )
{
  m_x = d;
  m_y = d;
  m_z = d;
}

void VecPosition::operator +=( const VecPosition &p )
{
  m_x += p.m_x;
  m_y += p.m_y;
  m_z += p.m_z;
}

void VecPosition::operator += ( const double &d )
{
  m_x += d;
  m_y += d;
  m_z += d;
}

void VecPosition::operator -=( const VecPosition &p )
{
  m_x -= p.m_x;
  m_y -= p.m_y;
  m_z -= p.m_z;
}

void VecPosition::operator -=( const double &d )
{
  m_x -= d;
  m_y -= d;
  m_z -= d;
}

void VecPosition::operator *=( const VecPosition &p )
{
  m_x *= p.m_x;
  m_y *= p.m_y;
  m_z *= p.m_z;
}

void VecPosition::operator *=( const double &d )
{
  m_x *= d;
  m_y *= d;
  m_z *= d;
}

void VecPosition::operator /=( const VecPosition &p )
{
  m_x /= p.m_x;
  m_y /= p.m_y;
  m_z /= p.m_z;
}

void VecPosition::operator /=( const double &d )
{
  m_x /= d;
  m_y /= d;
  m_z /= d;
}

bool VecPosition::operator !=( const VecPosition &p )
{
  return ( ( m_x != p.m_x ) || ( m_y != p.m_y ) || ( m_z != p.m_z ) );
}

bool VecPosition::operator !=( const double &d )
{
  return ( ( m_x != d ) || ( m_y != d ) || ( m_z != d ) );
}

bool VecPosition::operator ==( const VecPosition &p )
{
  return ( ( m_x == p.m_x ) && ( m_y == p.m_y ) && ( m_z == p.m_z ) );
}

bool VecPosition::operator ==( const double &d )
{
  return ( ( m_x == d ) && ( m_y == d ) && ( m_z == d )  );
}
ostream& operator << (ostream & output ,VecPosition vec)
{
	output <<"( "<<vec.getX()<<" , "<<vec.getY()<<" , "<<vec.getZ()<<" )" ;
	return output ;
}
string VecPosition::str( CoordSystemT cs )
{
  char buf[ 1024 ];

  if( cs == CARTESIAN )
    sprintf( buf, "( %f, %f, %f )", getX( ), getY( ), getZ( ) );
  else
	if(cs==POLAR)
    		sprintf( buf, "( r: %f, phi: %f ,Z: %f)", getMagnitude( ), getPhiDirection( ), getZ( ) );
	else
		sprintf( buf, "( r: %f, phi: %f ,teta: %f)", getMagnitude( ), getPhiDirection( ), getTetaDirection( ) );		
  string str( buf );
  return ( str );
}

void VecPosition::setX( double dX )
{
  m_x = dX;
}

double VecPosition::getX( ) const
{
  return ( m_x );
}

void VecPosition::setY( double dY )
{
  m_y = dY;
}

double VecPosition::getY( ) const
{
  return ( m_y );
}

void VecPosition::setZ( double dZ )
{
  m_z = dZ;
}

double VecPosition::getZ( ) const
{
  return ( m_z );
}

void VecPosition::setVecPosition( double dX, double dY,double dZ, CoordSystemT cs)
{
  if( cs == CARTESIAN )
  {
    m_x = dX;
    m_y = dY;
    m_z = dZ;
  }
  else
    if(cs==POLAR)
	*this = getVecPositionFromPolar( dX, dY, dZ );
    else
	*this = getVecPositionFromKORAVI( dX, dY, dZ );
}

double VecPosition::getDistanceTo( const VecPosition p )
{
  return ( ( *this - p ).getMagnitude( ) );
}

VecPosition VecPosition::setMagnitude( double d )
{
  if( getMagnitude( ) > EPSILON )
     ( *this ) *= ( d / getMagnitude( ) );

  return ( *this );
}

double VecPosition::getMagnitude( ) const
{
  return ( sqrt( m_x * m_x + m_y * m_y + m_z * m_z ) );
}

AngDeg VecPosition::getPhiDirection( ) const
{
  return (  atan2Deg( m_y, m_x )  );
}

AngDeg VecPosition::getTetaDirection()const
{
    double g = getMagnitude();
    if ( g > EPSILON)
	return ( asinDeg( m_z / g ) );
    else
	return 1;
}

bool VecPosition::isInFrontOf( const VecPosition &p )
{
  return ( ( m_x > p.getX( ) ) ? true : false );
}

bool VecPosition::isInFrontOf( const double &d )
{
  return ( ( m_x > d ) ? true : false );
}

bool VecPosition::isBehindOf( const VecPosition &p )
{
  return ( ( m_x < p.getX( ) ) ? true : false );
}

bool VecPosition::isBehindOf( const double &d )
{
  return ( ( m_x < d ) ? true : false );
}

bool VecPosition::isLeftOf( const VecPosition &p )
{
  return ( ( m_y < p.getY( ) ) ? true : false );
}

bool VecPosition::isLeftOf( const double &d )
{
  return ( ( m_y < d ) ? true : false );
}

bool VecPosition::isRightOf( const VecPosition &p )
{
  return ( ( m_y > p.getY( ) ) ? true : false );
}

bool VecPosition::isRightOf( const double &d )
{
  return ( ( m_y > d ) ? true : false );
}

bool VecPosition::isBetweenX( const VecPosition &p1, const VecPosition &p2 )
{
  return ( ( isInFrontOf( p1 ) && isBehindOf( p2 ) ) ? true : false );
}

bool VecPosition::isBetweenX( const double &d1, const double &d2 )
{
  return ( ( isInFrontOf( d1 ) && isBehindOf( d2 ) ) ? true : false );
}

bool VecPosition::isBetweenY( const VecPosition &p1, const VecPosition &p2 )
{
  return ( ( isRightOf( p1 ) && isLeftOf( p2 ) ) ? true : false );
}

bool VecPosition::isBetweenY( const double &d1, const double &d2 )
{
  return ( ( isRightOf( d1 ) && isLeftOf( d2 ) ) ? true : false );
}

VecPosition VecPosition::normalize( )
{
  return ( setMagnitude( 1.0 ) );
}

VecPosition VecPosition::rotate( AngDeg angle )
{
  double dMag    = getMagnitude( );
  double dNewDir = getTetaDirection( ) + angle; 
  setVecPosition( dMag, dNewDir, POLAR );     
  return ( *this );
}

VecPosition VecPosition::getVecPositionOnLineFraction( VecPosition &p,
                                                       double      dFrac )
{
  // determine point on line that lies at fraction dFrac of whole line
  // example: this --- 0.25 ---------  p
  // formula: this + dFrac * ( p - this ) = this - dFrac * this + dFrac * p =
  //          ( 1 - dFrac ) * this + dFrac * p
  return ( ( *this ) * ( 1.0 - dFrac ) + ( p * dFrac ) );
}

VecPosition VecPosition::getVecPositionFromKORAVI( double dMag, AngDeg phi, AngDeg teta)
{
  // cos(phi) = x/r <=> x = r*cos(phi); sin(phi) = y/r <=> y = r*sin(phi)
  return ( VecPosition( dMag * cosDeg( teta ) * cosDeg (phi) , dMag * cosDeg( teta ) * sinDeg (phi), dMag * sinDeg( teta ) ) );
}

VecPosition VecPosition::getVecPositionFromPolar(double dMag,AngDeg phi,double z)
{
	return ( VecPosition( dMag *  cosDeg (phi) , dMag * sinDeg (phi), z ) );
}

VecPosition VecPosition::Cross(const VecPosition &v)
{
	return (VecPosition(getY()*v.getZ() - getZ()*v.getY(),
						getZ()*v.getX() - getX()*v.getZ(),
						getX()*v.getY() - getY()*v.getX()));
}

/*****************************************************************************/
/********************* CLASS CIRCLE ******************************************/
/*****************************************************************************/

Circle::Circle( VecPosition pos, double dR )
{
  setCircle( pos, dR );
}

Circle::Circle( )
{
  setCircle( VecPosition(-1000.0,-1000.0), 0);
}

bool Circle::setCircle( VecPosition pos, double dR )
{
  setCenter( pos );
  return setRadius( dR  );
}

bool Circle::setRadius( double dR )
{
  if( dR > 0 )
  {
    m_dRadius = dR;
    return true;
  }
  return false;
}

double Circle::getRadius()
{
  return m_dRadius;
}

void Circle::setCenter( VecPosition pos )
{
  m_posCenter = pos;
}

VecPosition Circle::getCenter()
{
  return m_posCenter;
}

double Circle::getCircumference()
{
  return 2.0*M_PI*getRadius();
}

double Circle::getArea()
{
  return M_PI*getRadius()*getRadius();
}

bool Circle::isInside( VecPosition pos )
{
  return m_posCenter.getDistanceTo( pos ) < getRadius() ;
}

int Circle::getIntersectionPoints( Circle c, VecPosition &p1, VecPosition &p2)
{
    double x0, y0, r0;
    double x1, y1, r1;

    x0 = getCenter( ).getX();
    y0 = getCenter( ).getY();
    r0 = getRadius( );
    x1 = c.getCenter( ).getX();
    y1 = c.getCenter( ).getY();
    r1 = c.getRadius( );

    double      d, dx, dy, h, a, x, y, p2_x, p2_y;

    // first calculate distance between two centers circles P0 and P1.
    dx = x1 - x0;
    dy = y1 - y0;
    d = sqrt(dx*dx + dy*dy);

    // normalize differences
    dx /= d; dy /= d;

    // a is distance between p0 and point that is the intersection point P2
    // that intersects P0-P1 and the line that crosses the two intersection
    // points P3 and P4.
    // Define two triangles: P0,P2,P3 and P1,P2,P3.
    // with distances a, h, r0 and b, h, r1 with d = a + b
    // We know a^2 + h^2 = r0^2 and b^2 + h^2 = r1^2 which then gives
    // a^2 + r1^2 - b^2 = r0^2 with d = a + b ==> a^2 + r1^2 - (d-a)^2 = r0^2
    // ==> r0^2 + d^2 - r1^2 / 2*d
    a = (r0*r0 + d*d - r1*r1) / (2.0 * d);

    // h is then a^2 + h^2 = r0^2 ==> h = sqrt( r0^2 - a^2 )
    double      arg = r0*r0 - a*a;
    h = (arg > 0.0) ? sqrt(arg) : 0.0;

    // First calculate P2
    p2_x = x0 + a * dx;
    p2_y = y0 + a * dy;

    // and finally the two intersection points
    x =  p2_x - h * dy;
    y =  p2_y + h * dx;
    p1.setVecPosition( x, y );
    x =  p2_x + h * dy;
    y =  p2_y - h * dx;
    p2.setVecPosition( x, y );

    return (arg < 0.0) ? 0 : ((arg == 0.0 ) ? 1 :  2);
}

double Circle::getIntersectionArea( Circle c )
{
  VecPosition pos1, pos2, pos3;
  double d, h, dArea;
  AngDeg ang;

  d = getCenter().getDistanceTo( c.getCenter() ); // dist between two centers
  if( d > c.getRadius() + getRadius() )           // larger than sum radii
    return 0.0;                                   // circles do not intersect
  if( d <= fabs(c.getRadius() - getRadius() ) )   // one totally in the other
  {
    double dR = min( c.getRadius(), getRadius() );// return area smallest circ
    return M_PI*dR*dR;
  }

  int iNrSol = getIntersectionPoints( c, pos1, pos2 );
  if( iNrSol != 2 )
    return 0.0;

  // the intersection area of two circles can be divided into two segments:
  // left and right of the line between the two intersection points p1 and p2.
  // The outside area of each segment can be calculated by taking the part
  // of the circle pie excluding the triangle from the center to the
  // two intersection points.
  // The pie equals pi*r^2 * rad(2*ang) / 2*pi = 0.5*rad(2*ang)*r^2 with ang
  // the angle between the center c of the circle and one of the two
  // intersection points. Thus the angle between c and p1 and c and p3 where
  // p3 is the point that lies halfway between p1 and p2.
  // This can be calculated using ang = asin( d / r ) with d the distance
  // between p1 and p3 and r the radius of the circle.
  // The area of the triangle is 2*0.5*h*d.

  pos3 = pos1.getVecPositionOnLineFraction( pos2, 0.5 );
  d = pos1.getDistanceTo( pos3 );
  h = pos3.getDistanceTo( getCenter() );
  ang = asin( d / getRadius() );

  dArea = ang*getRadius()*getRadius();
  dArea = dArea - d*h;

  // and now for the other segment the same story
  h = pos3.getDistanceTo( c.getCenter() );
  ang = asin( d / c.getRadius() );
  dArea = dArea + ang*c.getRadius()*c.getRadius();
  dArea = dArea - d*h;

  return dArea;
}

/*****************************************************************************/
/***********************  CLASS LINE *****************************************/
/*****************************************************************************/

//A line is specified by the formula ay + bx + c = 0.
Line::Line( double dA, double dB, double dC )
{
  if(dA==0 && dB==0)
	  dA=1;
  m_a = dA;
  m_b = dB;
  m_c = dC;
}

bool Line::isParallel(Line line)
{
	return  Equal( m_a * line.getB() , m_b * line.getA()  );
}

int Line::getIntersection( Line line , VecPosition &v)
{
  VecPosition pos;
  double x, y;
  if( isParallel(line))
    return 0; // lines are parallel, no intersection
  if( m_a == 0 )            // bx + c = 0 and a2*y + b2*x + c2 = 0 ==> x = -c/b
  {                          // calculate x using the current line
    x = -m_c/m_b;                // and calculate the y using the second line
    y = line.getYGivenX(x);
  }
  else if( line.getA() == 0 )
  {                         // ay + bx + c = 0 and b2*x + c2 = 0 ==> x = -c2/b2
   x = -line.getC()/line.getB(); // calculate x using
   y = getYGivenX(x);       // 2nd line and calculate y using current line
  }
  // ay + bx + c = 0 and a2y + b2*x + c2 = 0
  // y = (-b2/a2)x - c2/a2
  // bx = -a*y - c =>  bx = -a*(-b2/a2)x -a*(-c2/a2) - c ==>
  // ==> a2*bx = a*b2*x + a*c2 - a2*c ==> x = (a*c2 - a2*c)/(a2*b - a*b2)
  // calculate x using the above formula and the y using the current line
  else
  {
    x = (m_a*line.getC() - line.getA()*m_c)/
                    (line.getA()*m_b - m_a*line.getB());
    y = getYGivenX(x);
  }
  v = VecPosition( x, y );
  return 1; 
}


int Line::getCircleIntersectionPoints( Circle circle,
              VecPosition &posSolution1, VecPosition &posSolution2 )
{
  int    iSol;
  double dSol1=0, dSol2=0;
  double h = circle.getCenter().getX();
  double k = circle.getCenter().getY();

  // line:   x = -c/b (if a = 0)
  // circle: (x-h)^2 + (y-k)^2 = r^2, with h = center.x and k = center.y
  // fill in:(-c/b-h)^2 + y^2 -2ky + k^2 - r^2 = 0
  //         y^2 -2ky + (-c/b-h)^2 + k^2 - r^2 = 0
  // and determine solutions for y using abc-formula
  if( fabs(m_a) < EPSILON )
  {
    iSol = abcFormula( 1, -2*k, ((-m_c/m_b) - h)*((-m_c/m_b) - h)
              + k*k - circle.getRadius()*circle.getRadius(), &dSol1, &dSol2);
    posSolution1.setVecPosition( (-m_c/m_b), dSol1 );
    posSolution2.setVecPosition( (-m_c/m_b), dSol2 );
    return iSol;
  }

  // ay + bx + c = 0 => y = -b/a x - c/a, with da = -b/a and db = -c/a
  // circle: (x-h)^2 + (y-k)^2 = r^2, with h = center.x and k = center.y
  // fill in:x^2 -2hx + h^2 + (da*x-db)^2 -2k(da*x-db) + k^2 - r^2 = 0
  //         x^2 -2hx + h^2 + da^2*x^2 + 2da*db*x + db^2 -2k*da*x -2k*db
  //                                                         + k^2 - r^2 = 0
  //       (1+da^2)*x^2 + 2(da*db-h-k*da)*x + h2 + db^2  -2k*db + k^2 - r^2 = 0
  // and determine solutions for x using abc-formula
  // fill in x in original line equation to get y coordinate
  double da = -m_b/m_a;
  double db = -m_c/m_a;

  double dA = 1 + da*da;
  double dB = 2*( da*db - h - k*da );
  double dC = h*h + db*db-2*k*db + k*k - circle.getRadius()*circle.getRadius();

  iSol = abcFormula( dA, dB, dC, &dSol1, &dSol2 );

  posSolution1.setVecPosition( dSol1, da*dSol1 + db );
  posSolution2.setVecPosition( dSol2, da*dSol2 + db );
  return iSol;

}

Line Line::getTangentLine      ( VecPosition pos )
{
	return Line (m_a, m_b, -m_a*pos.getX()-m_b*pos.getY());
}

Line Line::getPerpendicularLine( VecPosition pos )
{
  // ay + bx + c = 0 -> y = (-b/a)x + (-c/a)
  // tangent: y = (a/b)*x + C1 -> by - ax + C2 = 0 => C2 = ax - by
  // with pos.y = y, pos.x = x
  return Line( m_b, -m_a, m_a*pos.getX() - m_b*pos.getY() );
}

VecPosition Line::getPointOnLineClosestTo( VecPosition pos )
{
  Line l2 = getTangentLine( pos );  // get tangent line
  VecPosition v;
  getIntersection(l2, v);
  return v;     // and intersection between the two lines
}

double Line::getDistanceWithPoint( VecPosition pos )
{
  return pos.getDistanceTo( getPointOnLineClosestTo( pos ) );
}

/*! This method determines whether the projection of a point on the
    current line lies between two other points ('point1' and 'point2')
    that lie on the same line.

    \param pos point of which projection is checked.
    \param point1 first point on line
    \param point2 second point on line
    \return true when projection of 'pos' lies between 'point1' and 'point2'.*/
bool Line::isInBetween( VecPosition pos, VecPosition point1,VecPosition point2 )
{
	pos          = getPointOnLineClosestTo( pos ); // get closest point
	double dDist = point1.getDistanceTo( point2 ); // get distance between 2 pos

  // if the distance from both points to the projection is smaller than this
  // dist, the pos lies in between.
	return pos.getDistanceTo( point1 ) <= dDist &&
			pos.getDistanceTo( point2 ) <= dDist;
}

double Line::getYGivenX( double x )
{
 if( m_a == 0 )
   return 0;
  // ay + bx + c = 0 ==> ay = -(b*x + c)/a
  return -(m_b*x+m_c)/m_a;
}

double Line::getXGivenY( double y )
{
 if( m_b == 0 )
   return 0;
  // ay + bx + c = 0 ==> bx = -(a*y + c)/a
  return -(m_a*y+m_c)/m_b;
}

Line::Line( VecPosition pos1, VecPosition pos2 )
{
  // 1*y + bx + c = 0 => y = -bx - c
  // with -b the direction coefficient (or slope)
  // and c = - y - bx
	if(pos1.getDistanceTo(pos2)<.01)
		pos2=pos1+VecPosition(10,10,0);
  double dA, dB, dC;
  double dTemp = pos2.getX() - pos1.getX(); // determine the slope
  if( fabs(dTemp) < EPSILON )
  {
    // ay + bx + c = 0 with vertical slope=> a = 0, b = 1
    dA = 0.0;
    dB = 1.0;
  }
  else
  {
    // y = (-b)x -c with -b the slope of the line
    dA = 1.0;
    dB = -(pos2.getY() - pos1.getY())/dTemp;
  }
  // ay + bx + c = 0 ==> c = -a*y - b*x
  dC =  - dA*pos2.getY()  - dB * pos2.getX();
  m_a = dA;
  m_b = dB;
  m_c = dC;
}

Line::Line( VecPosition vec, AngDeg angle )
{
  *this = Line( vec, vec+VecPosition(1,angle,0,POLAR));
}

double Line::getA() const
{
  return m_a;
}

double Line::getB() const
{
 return m_b;
}

double Line::getC() const
{
 return m_c;
}

/*! This method creates a line given two points.
    \param pos1 first point
    \param pos2 second point
    \return line that passes through the two specified points. */
Line Line::makeLineFromTwoPoints( VecPosition pos1, VecPosition pos2 )
{
  // 1*y + bx + c = 0 => y = -bx - c
  // with -b the direction coefficient (or slope)
  // and c = - y - bx
	double dA, dB, dC;
	double dTemp = pos2.getX() - pos1.getX(); // determine the slope
	if( fabs(dTemp) < EPSILON )
	{
    // ay + bx + c = 0 with vertical slope=> a = 0, b = 1
		dA = 0.0;
		dB = 1.0;
	}
	else
	{
    // y = (-b)x -c with -b the slope of the line
		dA = 1.0;
		dB = -(pos2.getY() - pos1.getY())/dTemp;
	}
  // ay + bx + c = 0 ==> c = -a*y - b*x
	dC =  - dA*pos2.getY()  - dB * pos2.getX();
	return Line( dA, dB, dC );
}

/*****************************************************************************/
/********************* CLASS SEGMENT *****************************************/
/*****************************************************************************/
Segment::Segment(VecPosition _p1, VecPosition _p2)
	:p1(_p1),p2(_p2)
{
	if(p1.getDistanceTo(p2)<.01)
		p2=p1+VecPosition(10,10,0);
}

int Segment::getIntersection(Line l, VecPosition &p)
{
	if(p1.getDistanceTo(p2)<.01)
		return 0;
	Line l2(p1,p2);
	if(l2.getIntersection(l,p)==0)
		return 0;
	return ! isOn(p);
}

int Segment::getIntersection(Segment s, VecPosition &p)
{
	if(p1.getDistanceTo(p2)<.01)
		return 0;
	if(s.p1.getDistanceTo(s.p2)<.01)
		return 0;
	Line l1(p1,p2),l2(s.p1,s.p2);
	if(!l1.getIntersection(l2,p))
		return 0;
	if(isOn(p) && s.isOn(p))
		return 1;
	return 0;
}

bool Segment::isOn(VecPosition p)
{
	if(p1.getDistanceTo(p2)<.01)
		return false;
	if(Line(p1,p2).getDistanceWithPoint(p)>0.1)
		return false;
	if(p.getX()<max(p1.getX(),p2.getX())&& p.getX()>min(p1.getX(),p2.getX())
	&& p.getX()>max(p1.getX(),p2.getX())&& p.getX()<min(p1.getX(),p2.getX()))
				return true;
	if(p == p1)
		return true;
	return false;
}

/*****************************************************************************/
/********************* CLASS POLYGAN *****************************************/
/*****************************************************************************/
/*
Poly::Poly( int n, ... )
{
	vector <VecPosition> v;
	v.clear();
	va_list list;
	va_start(list, n);
	for(int i=0;i<n;i++)
		v.push_back(va_arg(list, VecPosition));
	va_end(list);
	setCorners(v);
}
*/
Poly::Poly()
{
}

Poly::Poly(vector <VecPosition> v)
{
	setCorners(v);
}

void Poly::setCorners( vector <VecPosition> v)
{
	corners=v;
}

vector <VecPosition> Poly::getCorners()
{
	return corners;
}

vector <VecPosition> Poly::getIntersection(Line l)
{
	vector <VecPosition> result;
	result.clear();
	int n=corners.size();
	for(int i=0;i<n;i++)
	{
		VecPosition p;
		if(Segment(corners[i],corners[(i+1)%n]).getIntersection(l,p))
			result.push_back(p);
	}
	return result;
}

vector <VecPosition> Poly::getIntersection(Segment s)
{
	vector <VecPosition> result;
	result.clear();
	int n=corners.size();
	for(int i=0;i<n;i++)
	{
		VecPosition p;
		if(Segment(corners[i],corners[(i+1)%n]).getIntersection(s,p))
			result.push_back(p);
	}
	return result;
}

bool Poly::isInside( VecPosition &pos )
{
	VecPosition v(0,0,0);
	int n=corners.size();
	for(int i=0;i<n;i++)
	{
		v.setX(max(v.getX(),corners[i].getX()+10));
		v.setY(max(v.getY(),corners[i].getY()+10));
	}
	vector <VecPosition> juncs = getIntersection(Segment(pos,v));
	return juncs.size()%2;
}

double Poly::getArea()
{
	double result=0;
	int n=corners.size();
	for(int i=0;i<n;i++)
		result += corners[i].getX()*corners[(i+1)%n].getY() - 
				  corners[i].getY()*corners[(i+1)%n].getX();
	return fabs(result/2);
}

double midAngle ( VecPosition center, VecPosition leftLeg, VecPosition rightLeg )
{
    return ( normalizeAngle ( normalizeAngle ( ( leftLeg - center ).getPhiDirection () ) + normalizeAngle ( ( rightLeg - center ).getPhiDirection () ) ) / 2.0  ) ;
}

double pointsAngle ( VecPosition center, VecPosition leftLeg, VecPosition rightLeg )
{
    return fabs ( normalizeAngle (  normalizeAngle ( ( leftLeg - center ).getPhiDirection () ) - normalizeAngle ( ( rightLeg - center ).getPhiDirection () ) ) );
}
/*****************************************************************************************************************************************************************************************************************************************Cone Class****************************************************/
Cone::Cone ( VecPosition Start,VecPosition PointLeft, VecPosition PointRight )
{	
	setCone (Start,PointLeft,PointRight);
}
void Cone::setStartPoint ( VecPosition st )
{
	start = st;
}
void Cone::setStartPoint ( double x,double y,double z)
{
	z= 0.000;
	start = VecPosition (x,y,z);
}
void Cone::setPointLeft  ( VecPosition st )
{
	left = st;
}
void Cone::setPointLeft  ( double x,double y,double z)
{
	left = VecPosition (x,y,z);
}
void Cone::setPointRight ( VecPosition st )
{
	right = st;
}
void Cone::setPointRight ( double x,double y,double z)
{
	right = VecPosition (x,y,z);
}
void Cone::setCone (VecPosition a, VecPosition b,VecPosition c)	
{
	start = a;
	left = b;
	right = c;
}
VecPosition Cone::getStartPoint ( )
{
	return start;
}
double Cone::getXPositionStart  ( )
{
	return start.getX ( );
}
double Cone::getYPositionStart  ( )	
{
	return start.getY ( );
}
double Cone::getZPositionStart  ( )
{
	return start.getZ ( );
}
VecPosition Cone::getPointLeft  ( )
{
	return left;
}
double Cone::getXPositionLeft  ( )
{
	return left.getX ( );
}
double Cone::getYPositionLeft  ( )	
{
	return left.getY ( );
}
double Cone::getZPositionLeft  ( )
{
	return left.getZ ( );
}
VecPosition Cone::getPointRight ( )
{
	return right;
}
double Cone::getXPositionRight  ( )
{
	return right.getX ( );
}
double Cone::getYPositionRight  ( )	
{
	return right.getY ( );
}
double Cone::getZPositionRight  ( )
{
	return right.getZ ( );
}

/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////

bool Cone::chekPoint ( VecPosition position )
{
	VecPosition center = ( getPointLeft() + getPointRight() ) / 2.0;
	
	double l , r , z;
	
	double length1 = ( position - getStartPoint() ).getMagnitude ();
	
	double length2 = ( getStartPoint() - center ).getMagnitude ();
	
	if ( position.getX ( ) <= length2 )
		for ( z = 0.000 ; z < length1 ; z += BigZ() )
			for ( r = 0.000 ; r < length1 ; r += BigLR() )
				for ( l = 0.000 ; l < length1 ; l -= BigLR() )
					if ( position.getZ() <= z )
						if ( position.getY() >= l && position.getY() <= r )
							return true;
						else 
							return false;
					else
						return false;
	
	return false;

}

double Cone::BigZ  ( )
{
	VecPosition center = ( getPointLeft() + getPointRight() ) / 2.0;
	
	double length = ( getStartPoint() - center ).getMagnitude ();
				
	return ( length / center.getZ() );
}

double Cone::BigLR ( )
{
	VecPosition center = ( getPointLeft() + getPointRight() ) / 2.0;
	
	double Redu = ( getPointLeft() - center ).getMagnitude ();
	
	double length = ( getStartPoint() - center ).getMagnitude ();
	
	return ( Redu / length ); 
}
