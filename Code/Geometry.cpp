/*
 * Geometry.cpp
 *
 *  Created on: 17 Mar 2015
 *      Author: Spencer Newton
 */

#include "Geometry.h"
#include <math.h>
#include <cmath>
using std::abs;
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#define PI 3.14159265
#define min 0.0000001


void runGeometryCalculations()
{
	testReflection();
	testRotation();
}

/*
 *	Requests user to input vector data into the console
 */
void inputVector( Vector3D *v )
{
	cin >> v->p.x;
	cin >> v->p.y;
	cin >> v->p.z;
}

/*
 *	Function to test vector reflection 
 */
void testReflection()
{
	Vector3D light, normal, reflection;

	// Get vector from user
	cout << "Input Light Vector: \n";
	inputVector( &light );
	cout << "L  = \t"; light.print(); cout << endl;
	cout << "Ln = \t"; light.getNormalized().print(); cout << endl;

	// Get mirror normal from user
	cout << "Input Mirror Normal: \n";
	inputVector( &normal );
	cout << "N  = \t"; normal.print(); cout << endl;
	cout << "Nn = \t"; normal.getNormalized().print(); cout << endl;
	
	// Calculate light vector reflected about mirror normal vector
	reflection = light.reflect( normal );

	// Print out result
	cout << "Reflected Vector: \n";
	cout << "R  = \t"; reflection.print(); cout << endl;
	cout << "Rn = \t"; reflection.getNormalized().print(); cout << endl;
}

/*
 *	Function to test vector rotation 
 */
void testRotation()
{
	Vector3D normal, angles, rotated;

	// Get vector from user
	cout << "Input Start Vector: \n";
	inputVector( &normal );
	cout << "V  = \t"; normal.print(); cout << endl;
	cout << "Vn = \t"; normal.getNormalized().print(); cout << endl;

	// Get rotation euler angles from user
	cout << "Input Rotation Angles (degrees): \n";
	inputVector( &angles );
	cout << "Angles = \t"; angles.print(); cout << endl;

	// Convert to radians
	angles.p.x = angles.p.x * PI / 180;
	angles.p.y = angles.p.y * PI / 180;
	angles.p.z = angles.p.z * PI / 180;

	// Rotate vector about each axis by specified angle
	rotated = normal;
	if ( abs(angles.p.x) > min ) rotated = rotated.rotateRoll( angles.p.x );
	if ( abs(angles.p.y) > min ) rotated = rotated.rotatePitch( angles.p.y );
	if ( abs(angles.p.z) > min ) rotated = rotated.rotateYaw( angles.p.z );

	// Print result
	cout << "Rotated Vector: \n";
	cout << "R  = \t"; rotated.print(); cout << endl;
	cout << "Rn = \t"; rotated.getNormalized().print(); cout << endl;
}


// ===================================================
// 				VECTOR CLASS
// ===================================================

/*
 *	Default Constructor whereby vector parameters are 0
 */
Vector3D::Vector3D()
{
	p.x = 0; p.y = 0; p.z = 0;
}

/*
 *	Constructor for Vector3D class that allows input of x, y, z values
 */
Vector3D::Vector3D( float x, float y, float z )
{
	p.x = x; p.y = y; p.z = z;
}

Vector3D::~Vector3D() { }

// ============= Functions
/*
 *	Returns dot product of this vector and argument vector 'v'
 */
float Vector3D::dotProduct( const Vector3D v ) const
{
	return (p.x * v.p.x) + (p.y * v.p.y) + (p.z * v.p.z);
}

/*
 *	Returns cross product of this vector and argument vector 'v'
 */
Vector3D Vector3D::crossProduct( const Vector3D v ) const
{
	float x, y, z;

	// y*q.z-z*q.y, z*q.x-x*q.z, x*q.y-y*q.x
	x = (p.y * v.p.z) - (p.z * v.p.y);
	y = (p.z * v.p.x) - (p.x * v.p.z);
	z = (p.x * v.p.y) - (p.y * v.p.x);

	return Vector3D( x, y, z );
}

/*
 *	Returns length of this vector
 */
float Vector3D::length()
{
	return sqrtf( (p.x * p.x) + (p.y * p.y) + (p.z * p.z) );
}

/*
 *	Normalizes this vector (length one)
 */
void Vector3D::normalize()
{
	float l = length();

	p.x = p.x / l;
	p.y = p.y / l;
	p.z = p.z / l;
}

/*
 *	Returns normalized version of this vector (length one)
 */
Vector3D Vector3D::getNormalized()
{
	Vector3D v;

	float l = length();

	v.p.x = p.x / l;
	v.p.y = p.y / l;
	v.p.z = p.z / l;

	return v;
}

/*
 *	Returns this vector reflected about vector 'normal'
 */
Vector3D Vector3D::reflect(Vector3D normal ) const
{
	Vector3D r;

	normal.normalize();

	float dot = this->dotProduct(normal);

	// NORMAL VECTOR MUST BE NORMALIZED
	// r = v - (2 * ( v dot normal ) * n)
	r = *this - ((normal * dot) * 2.f);

	return r;
}

/*
 *  Rotate vector around Z axis (yaw) and return resulting Vector
 */
Vector3D Vector3D::rotateYaw( float theta ) // RADIANS
{
	/*
	 * Rotation Matrix :[ cos(t),-sin(t), 0 ] [ x ]
	 * 					[ sin(t), cos(t), 0 ] [ y ]
	 * 					[ 0 	, 0 	, 1 ] [ z ]
	 *
	 */

	Vector3D rotated;

	rotated.p.x = (p.x * cos(theta)) + (p.y * -sin(theta)) + (p.z * 0);
	rotated.p.y = (p.x * sin(theta)) + (p.y * cos(theta)) + (p.z * 0);
	rotated.p.z = (p.x * 0) + (p.y * 0) + (p.z * 1);

	if ( abs(rotated.p.x) < min ) rotated.p.x = 0;
	if ( abs(rotated.p.y) < min ) rotated.p.y = 0;
	if ( abs(rotated.p.z) < min ) rotated.p.z = 0;

	return rotated;
}

/*
 *  Rotate vector around X axis (pitch) and return resulting Vector
 */
Vector3D Vector3D::rotateRoll( float theta ) // RADIANS
{
	/*
	 * Rotation Matrix :[ 1, 	  0, 	  0 ] [ x ]
	 * 					[ 0, cos(t),-sin(t) ] [ y ]
	 * 					[-0, sin(t), cos(t) ] [ z ]
	 *
	 */

	Vector3D rotated;

	rotated.p.x = (p.x * 1) + (p.y * 0) + (p.z * 0);
	rotated.p.y = (p.x * 0) + (p.y * cos(theta)) + (p.z * -sin(theta));
	rotated.p.z = (p.x * 0) + (p.y * sin(theta)) + (p.z * cos(theta));

	if ( abs(rotated.p.x) < min ) rotated.p.x = 0;
	if ( abs(rotated.p.y) < min ) rotated.p.y = 0;
	if ( abs(rotated.p.z) < min ) rotated.p.z = 0;

	return rotated;
}

/*
 *  Rotate vector around Y axis (roll) and return resulting Vector
 */
Vector3D Vector3D::rotatePitch( float theta ) // RADIANS
{
	/*
	 * Rotation Matrix :[ cos(t), 0, sin(t) ] [ x ]
	 * 					[ 0     , 1, 0      ] [ y ]
	 * 					[-sin(t), 0, cos(t) ] [ z ]
	 *
	 */

	Vector3D rotated;

	rotated.p.x = (p.x * cos(theta)) + (p.y * 0) + (p.z * sin(theta));
	rotated.p.y = (p.x * 0) + (p.y * 1) + (p.z * 0);
	rotated.p.z = (p.x * -sin(theta)) + (p.y * 0) + (p.z * cos(theta));

	if ( abs(rotated.p.x) < min ) rotated.p.x = 0;
	if ( abs(rotated.p.y) < min ) rotated.p.y = 0;
	if ( abs(rotated.p.z) < min ) rotated.p.z = 0;

	return rotated;
}

/*
 *	Print out vector parameters to console
 */
void Vector3D::print()
{
	cout << "(" << p.x << ", " << p.y << ", " << p.z << ")";
}


// =================================================== Operators


Vector3D Vector3D::operator-()
{
	return Vector3D( -p.x, -p.y, -p.z );
}

Vector3D Vector3D::operator*( float scale ) const
{
	Vector3D v;

	v.p.x = p.x * scale;
	v.p.y = p.y * scale;
	v.p.z = p.z * scale;

	return v;//Vector( p.x * scale, p.y * scale, p.z * scale );
}

Vector3D Vector3D::operator/( float scale ) const
{
	return Vector3D( p.x / scale, p.y / scale, p.z / scale );
}

Vector3D Vector3D::operator-( const Vector3D v ) const
{
	return Vector3D( p.x - v.p.x, p.y - v.p.y, p.z - v.p.z );
}

Vector3D Vector3D::operator+( const Vector3D v ) const
{
	return Vector3D( p.x + v.p.x, p.y + v.p.y, p.z + v.p.z );
}

bool Vector3D::operator==( const Vector3D v ) const
{
	return ( p.x == v.p.x && p.y == v.p.y && p.z == v.p.z );
}

bool Vector3D::operator!=( const Vector3D v ) const
{
	return ( p.x != v.p.x || p.y != v.p.y | p.z != v.p.z );
}