


class CBMP:public CImageIO
{
private:

public:
	CBMP(){};
	~CBMP(){};

	char *GetLoadString();
	char *GetSaveString();

	CImage* LoadImage(char *file_name, char* errors);

/*#ifdef _WIN32
	bool SaveImage(HWND hWnd,CImage* Img,char* file_name);
#endif*/

	bool SaveImage(CImage* Image,char* file_name,bool swapBGR=false);
	char *GetExt(){return "bmp";}
};