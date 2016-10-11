#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <sysexits.h>
#include "sfm.h"
#include "move.h"
#include "data.h"
//#include "get_distance.h"

#include <string>
#include <math.h>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;


void set_TKK(){
printf("set");
}

void set_goal()
{
	ifstream ifs1("latitude_goal.txt");
	copy(istream_iterator<double>(ifs1), istream_iterator<double>(), back_inserter(latitudeGoal));

	ifstream ifs("longitude_goal.txt");
	copy(istream_iterator<double>(ifs), istream_iterator<double>(), back_inserter(longitudeGoal));

}

void csm(double &latitude,double &latitude_goal,double &longitude,double &longitude_goal,float &ramuda0,float &h,float &one_scale)
{
	one_scale=100;
	latitude=100;
	longitude=100;
	latitude_goal=1000;
	longitude_goal=1000;
	
}

/*
int Laser_Range_Scanner(){
	
	return 0;	
}
*/


void Avoidance()
{
	V=200;
	sirial1(V);
	sirial1(V);
}

int capture(VideoCapture cap)
{
	Mat frame;
	cap >> frame;
	//フレーム画像を保存する．
	frameNo++;
	sprintf(str,"/media/itolab/disk2/work/sasai/cv/3dworld/imagewrite/surf%04d.png",frameNo);
        imwrite(str, frame);
}


int wait(){

	//if(Control()==1){
				
	return 0;
	}

int main ()
{
	wait();//選択待機モード	

	int count=0;
	float error=0.0;//誤差
	
	set_TKK();
	VideoCapture cap(0);
	if(!cap.isOpened())//カメラデバイスが正常にオープンしたか確認．
	{
		//読み込みに失敗したときの処理
		cout<<"読み込みエラー"<<endl;
		return -1;
	}

	set_goal();
	for(int i=0;i<100;i++)
	{
		capture(cap);//写真取る
	}
	cout<<"ok"<<endl;

	while(1)
	{
		if(wait()==-1)break;//選択待機モードへ戻る		
		csm(latitude,latitude_goal,longitude,longitude_goal,ramuda0,h,one_scale);//CSMを読む
		
		if(scale>=100/*&&曲がるとき*/)
		{
			capture(cap);//写真取る
			sfm(R_f,t_f,scale,frameNo);//３次元復元
			scale=0;
		}		
		
		else
		{
			scale=scale+one_scale;
		}

		if(latitude_goal-error<latitude&&latitude<latitude_goal+error&&longitude_goal-error<longitude&&longitude<longitude_goal+error)
		{
			if(count==latitudeGoal.size())break;
				
				latitude_goal=latitudeGoal[latitudeGoal.size()-count];
				longitude_goal=longitudeGoal[longitudeGoal.size()-count];
				count++;
			
		}
		/*else if(Laser_Range_Scanner()!=0)
		{
			Avoidance();//回避
		}
		*/
		else
		{
			move(latitude_goal,latitude,longitude_goal,longitude,ramuda0,h);//目標に向けて移動
		}
	}
}	
