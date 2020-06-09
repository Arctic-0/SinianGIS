#pragma once

#include <vector>


struct OneLevel 
{
	int tilesNumX;
	int tilesNumY;
	int pixelSizeX;
	int pixelSizeY;
	double dX;
	double dY;
};

struct OneTile 
{
	size_t x;
	size_t y;
	int z;
	std::string uri;
};

class WebTilesClass
{
public:
	WebTilesClass();
	virtual ~WebTilesClass();

	virtual bool  GetTiles(double left, double top, double right, double bottom, int level, std::vector<OneTile> &tiles);

	virtual bool WriteGeoInfo(std::string imgName, OneTile& tile);
	
protected:	
	//��ͼ���Ͻǵĵ�����ʼλ�ã�λ�����Ͻ�
	double geoStartX;		//��ʼλ��X		
	double geoStartY;		//��ʼλ��Y
	double geoEndX;			//�յ�λ��X
	double geoEndY;			//�յ�λ��Y

	int tilePixelSizeX;
	int tilePixelSizeY;

	int levelNum;
	std::vector<OneLevel> levelList;	
};

int DownloadTiles(std::string address, std::string outfile, std::string proxy = "");
