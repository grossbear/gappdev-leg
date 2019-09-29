

class CCamera
{
public:
	CCamera();
	virtual ~CCamera(){};

	inline float Near() const { return znear; }
	inline void Near(float znear) { this->znear=znear; } 

	inline float Far() const { return zfar; }
	inline void Far(float zfar) { this->zfar=zfar; }

	inline float FOV() const { return fov; } //in degs
	inline void FOV(float fov) { this->fov=fov; }//field of view in degs

	float AspectRatio()  { return aspect; }
	float AspectRatio(float aspect){ this->aspect = (aspect<0)?(0):(aspect); }

	inline Vector3D Position() const { return Pos; }
	inline void Position(float x, float y, float z) { Pos.Set(x,y,z); }

	inline Vector3D ViewVec() const { return View; }
	inline void ViewVec(float x, float y, float z) { View.Set(x,y,z); }

	inline Vector3D UpVec() const { return Up; }
	inline void UpVec(float x, float y, float z) { Up.Set(x,y,z); }

	inline void RotSpeed(float speed) { this->rotspeed=speed; }

	CCamera& operator=(const CCamera & cam);

	void	RotateLeft(float dt);
	void	RotateRight(float dt);
	void	RotateUp(float dt);
	void	RotateDown(float dt);

	

protected:
	float znear;
	float zfar;
	float fov; //in degs
	float rotspeed;
	float aspect;//Aspect Ratio

	float radiusXY;
	float radius;
	float alpha;
	float beta;

	Vector3D Pos;
	Vector3D View;
	Vector3D Up;	
};

