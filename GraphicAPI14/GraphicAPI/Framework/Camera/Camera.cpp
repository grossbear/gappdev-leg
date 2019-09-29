#include "../Math/MathLib.h"
#include "../Types/types.h"
#include "Camera.h"



CCamera::CCamera()
{
	znear=0.001f;
	zfar=80;
	fov=45;
	aspect=0.0f;
	rotspeed=float(M_PI/2);

	Pos.Set(0, -3.0f, 0.0f);	
	View.Set(0, 0.0f, 0.0f);
	Up.Set(0, 0, 1);
	
	radiusXY = 3.0f;
	radius = 3.0f;
	alpha = -float(M_PI/2);
	beta = 0.0f;
}

CCamera& CCamera::operator = (const CCamera& cam)
{
	znear = cam.znear;
	zfar  = cam.zfar;
	fov   = cam.fov;
	Pos   = cam.Pos;
	View  = cam.View;
	Up    = cam.Up;

	radiusXY = cam.radiusXY;
	radius= cam.radius;
	alpha = cam.alpha;
	beta  = cam.beta;

	
	return *this;
}

void CCamera::RotateLeft(float dt)
{
	alpha-=rotspeed*dt;

	if(alpha<-float(M_PI)*2.0f)
		alpha=alpha+(float(M_PI)*2.0f);
	Pos.x=Cosin(alpha)*radiusXY;
	Pos.y=Sinus(alpha)*radiusXY;	
}

void CCamera::RotateRight(float dt)
{
	alpha+=rotspeed*dt;

	if(alpha>float(M_PI)*2.0f)
		alpha=alpha-(float(M_PI)*2.0f);
	Pos.x=Cosin(alpha)*radiusXY;
	Pos.y=Sinus(alpha)*radiusXY;
}

void CCamera::RotateDown(float dt)
{
	beta-=rotspeed*dt;
	if(beta<0)
		beta=0;

	radiusXY = radius*Cosin(beta);

	Pos.x=Cosin(alpha)*radiusXY;
	Pos.y=Sinus(alpha)*radiusXY;
	Pos.z=Sinus(beta)*radius;

	return ;
}

void CCamera::RotateUp(float dt)
{
	beta+=rotspeed*dt;
	if(beta>1.57f)
		beta=1.57f;

	radiusXY = radius*Cosin(beta);

	Pos.x=Cosin(alpha)*radiusXY;
	Pos.y=Sinus(alpha)*radiusXY;
	Pos.z=Sinus(beta)*radius;
	return ;
}