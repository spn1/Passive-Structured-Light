/*
 * Geometry.h
 *
 *  Created on: 17 Mar 2015
 *      Author: Spencer Newton
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

typedef struct Point3D
{
	float x, y, z;
} Point3D;

class Vector3D {
public:
	// Variables
	Point3D p;

	// Constructors
	Vector3D();
	Vector3D( float, float, float );
	~Vector3D();

	// Operators
	Vector3D operator-();
	Vector3D operator*( float ) const;
	Vector3D operator/( float ) const;
	Vector3D operator-( const Vector3D ) const;
	Vector3D operator+( const Vector3D ) const;
	bool operator==( const Vector3D ) const;
	bool operator!=( const Vector3D ) const;
	//Vector operator=( const Vector );

	// Functions
	float dotProduct( const Vector3D ) const;
	Vector3D crossProduct( const Vector3D ) const;
	float length();
	void normalize();
	Vector3D getNormalized();
	Vector3D reflect( Vector3D ) const;
	Vector3D rotateYaw( float );
	Vector3D rotateRoll( float );
	Vector3D rotatePitch( float );
	void print();
};

void runGeometryCalculations();
void inputVector( Vector3D* );
void testReflection();
void testRotation();

#endif /* GEOMETRY_H_ */