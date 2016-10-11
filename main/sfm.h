#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include "vo_features.h"
#include "vo_features2.h"

/*PCL関連
#include <pcl/io/pcd_io.h>
#include <pcl/io/io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
*/



using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;




void sfm(Mat R_f,Mat t_f,float scale,int frameNo)
{

//	Mat K=(Mat_<double>(3,3,CV_64FC1) <<490.2152,0,295.0464,0,487.5284,221.9754,0,0,1);//内部パラメータ行列(usbカメラ)
	Mat K=(Mat_<double>(3,3,CV_64FC1) <<440.9741,0,290.2624,0,443.2639,242.4028,0,0,1);//内部パラメータ行列(本番カメラ)
//	Mat distortionCoefficient=(cv::Mat_<float>(4,1) <<0.12736,-0.20937,0.003467,-0.002242,0.008104);//歪み係数（usbカメラ）
	Mat distortionCoefficient=(cv::Mat_<float>(4,1) <<-0.39703,0.14661,0.001369,-0.0075874,-0.016642);//歪み係数(本番カメラ）
//	double focal = 489;//焦点距離（usbカメラ）
	double focal = 441.5;//焦点距離（本番カメラ）
//	Point2d pp(295.0464, 221.9754);//中心座標(usbカメラ)
	Point2d pp(290.2624, 242.4028);//中心座標(本番カメラ)

	char filename1[100];//3次元復元画像1
	char filename2[100];//3次元復元画像2
	sprintf(filename1, "/media/itolab/disk2/work/sasai/cv/3dworld/imagewrite/surf%04d.png",frameNo-1);
	sprintf(filename2, "/media/itolab/disk2/work/sasai/cv/3dworld/imagewrite/surf%04d.png",frameNo);
	
	//比較用画像を読み込む 
	Mat img_1_c = imread(filename1);
	Mat img_2_c = imread(filename2);

	// グレーにする
	Mat img_1, img_2;//グレー画像保存用
	cvtColor(img_1_c, img_1, COLOR_BGR2GRAY);
	cvtColor(img_2_c, img_2, COLOR_BGR2GRAY);
	   
	//特徴抽出準備
	vector<KeyPoint> keypoints1;
	vector<KeyPoint> keypoints2;
	vector<KeyPoint> keypoints11;
	vector<KeyPoint> keypoints22;
	vector<Point2f> points1;
	vector<Point2f> points2;
	
	// 特徴点抽出
	SURFdesu(img_1, points1,keypoints1);
	SURFdesu(img_2, points2,keypoints2);
	//
	
	// 特徴記述
	Mat descriptor1, descriptor2;//特徴量記述用
	SURF::create()->compute(img_1, keypoints1, descriptor1);
	SURF::create()->compute(img_2, keypoints2, descriptor2);
	//

	// マッチング (アルゴリズムにはBruteForceを使用)
	Ptr<cv::DescriptorMatcher> knnmatcher = cv::DescriptorMatcher::create("BruteForce");
	vector<cv::DMatch> match;
	vector< vector<DMatch> > knnmatch12,knnmatch21;
	knnmatcher->knnMatch(descriptor1, descriptor2, knnmatch12, 2); //上位2位までの点を探す
	knnmatcher->knnMatch(descriptor2, descriptor1, knnmatch21, 2);
	//knntest
	vector<DMatch> bestMatches12;
	vector<KeyPoint> bestkey112,bestkey212;
	vector<Point2f> p1;
	vector<Point2f> p2;
	vector<Point2f> p11;
	vector<Point2f> p22;


	float match12_par = .5f; //対応点のしきい値
	for (int i=0; i<knnmatch12.size(); i++)
	{
		float dist1 = knnmatch12[i][0].distance;
		float dist2 = knnmatch12[i][1].distance;
		//良い点を残す（最も類似する点と次に類似する点の類似度から）
		if (dist1 <= dist2 * match12_par && dist1<=0.2f)
		{
		        bestMatches12.push_back(knnmatch12[i][0]);
		        bestkey112.push_back(keypoints1[knnmatch12[i][0].queryIdx]);
		        bestkey212.push_back(keypoints2[knnmatch12[i][0].trainIdx]);
			p1.push_back(keypoints1[knnmatch12[i][0].queryIdx].pt);
			p2.push_back(keypoints2[knnmatch12[i][0].trainIdx].pt);
		}
	}

	//E行列からR,tを求める
	vector<cv::DMatch> inlinerMatch;
	Mat E,R,t,mask;
  	E = findEssentialMat(p1, p2, focal, pp, RANSAC, 0.999, 1.0, mask);
  	recoverPose(E, p1, p2, R, t, focal, pp, mask);  
	
	//maskを使って精度を高める
	for (size_t i = 0; i < mask.rows; ++i)
	{
	        uchar *inliner = mask.ptr<uchar>(i);
	        if (inliner[0] == 1)
		{
			inlinerMatch.push_back(bestMatches12[i]);
			p11.push_back(keypoints1[bestMatches12[i].queryIdx].pt);
			p22.push_back(keypoints2[bestMatches12[i].trainIdx].pt);
        	}
	}


	//移動量計算
	t=t*scale;

	//Rt作
	Mat Rt=cv::Mat::eye(3, 4, CV_64FC1);
  	for(double m=0 ; m < 3; m++)
  		{
  		for(double b=0 ; b < 3; b++)
  			{
  			Rt.at<double>(m,b)=R.at<double>(m,b);
  			}
		}
	Rt.at<double>(0,3)=t.at<double>(0);
	Rt.at<double>(1,3)=t.at<double>(1);
	Rt.at<double>(2,3)=t.at<double>(2);

	//透視射影行列作成
	Mat Meye = cv::Mat::eye(3, 4, CV_64FC1);
	Mat M0 = cv::Mat::eye(3, 4, CV_64FC1);
	Mat M1=cv::Mat::eye(3, 4, CV_64FC1);
	M0=K*Meye;
  	M1=K*Rt;

	//三角測量による3次元復元
	Mat point4D;
	Mat point4Dh=(Mat_<float>(1,p11.size()*4,CV_32FC1));
	vector<Point3f> point3D;
	triangulatePoints(M0, M1, p11, p22, point4D);


	//ch変更1to4(convertPointsFromHomogeneousのため)		
	int n=0;
	for(int i=0;i<p11.size()/*match.size()*/;i++)
	{
		point4Dh.at<float>(0,n)=point4D.at<float>(0,i);
		point4Dh.at<float>(0,n+1)=point4D.at<float>(1,i);
		point4Dh.at<float>(0,n+2)=point4D.at<float>(2,i);
		point4Dh.at<float>(0,n+3)=point4D.at<float>(3,i);
		n=n+4;
	}
	//3次元座標にする
	convertPointsFromHomogeneous(point4Dh.reshape(4,1),point3D);

	//原点からの座標に変換
	for(int i=0;point3D.size()>i;i++)
	{
		point3D[i].x=point3D[i].x-t_f.at<double>(0);
		point3D[i].y=point3D[i].y-t_f.at<double>(1);
		point3D[i].z=point3D[i].z-t_f.at<double>(2);
	}

	//総移動量計算
	t_f = t_f + R_f*t;
	R_f = R*R_f;

	//座標情報を貯める
	vector<Point3f> point3DW;
	vector<Point3f> pointme;
	for(int i=0;point3D.size()>i;i++)
	{
		point3DW.push_back(point3D[i]);
	}
	pointme.push_back(Point3f(t_f));


	//txtに書き込む
	char data[255];
	sprintf(data,"/media/itolab/disk2/work/sasai/cv/3dworld/dataset2/3Ddata%04d.txt",frameNo-1);
	char ddata[255];
	ofstream fs(data);

	for(int i=0;i<point3D.size();i++)
	{
		sprintf(ddata,"%lf %lf %lf",point3D[i].x,point3D[i].y,point3D[i].z);
		fs<<ddata<<endl;
	}
	fs.close();


}			
