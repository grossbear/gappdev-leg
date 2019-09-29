

class CImageIO
{
private:

public:
	CImageIO(){}
	virtual ~CImageIO(){}

	virtual char* GetLoadString(){return NULL;}
	virtual char* GetSaveString(){return NULL;}

	virtual CImage* LoadImage(char* file_name,char* errors){return NULL;}

/*#ifdef _WIN32
	virtual bool SaveImage(HWND hWnd,CImage* Img,char* file_name){return false;}
#endif*/

	virtual bool SaveImage(CImage* Image,char* file_name,bool swapBGR=false){return false;}
	virtual char* GetExt(){return NULL;}
#ifdef _WIN32
	virtual bool SaveImage(HWND hWnd,CImage* Img,char* file_name){return false;}
#endif
};


class CBMIFile
{
private:
	CImage* tmpimg;	
	uint ImgIONum;
	CImageIO** ppImageIO;
	char* LoadString;
	char* SaveString;
	
public:
	CBMIFile();
	virtual ~CBMIFile();
	CImage* Load(HWND hWnd);
	CImage* LoadBitmapImage(char* file_name);
	bool Save(HWND hWnd,CImage* Img);
};