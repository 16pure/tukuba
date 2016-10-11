/***********************************************************************/
/*                                                                     */
/* GSM-MG100受信用プログラム                                           */
/* シリアルポート受信サンプルプログラム                                */
/* Y.Ebihara (SiliconLinux)さんのプログラムを改造して                  */
/* GSM-MG100からのデータを受信する簡易プログラム                       */
/*                                                                     */
/* このプログラムはシリアルポートをopenして、データを16進数表示する    */
/* サンプルプログラムです。                                            */
/*   Ubuntuで動作検証をしています。                                    */
/*                                                                     */
/* 2016.9 Kouhei Ito @ KTC                                             */
/*                                                                     */
/***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <string.h>


using namespace std;

#define DEV_NAME    "/dev/ttyUSB0"        // デバイスファイル名　ここは環境に合わせて変える必要がある
#define BAUD_RATE    B921600              // RS232C通信ボーレート
#define BUFF_SIZE    4096                 // 適当


    int fd; //global
    int len;
    unsigned char buffer[BUFF_SIZE];    // データ受信バッファ
    char *argv[1000];
   

    double latitude,longitude;
    float ramuda0_Ptr,h_Ptr,one_scale;
    int *addrX,*addrY,*addrZ,*addrRrate;
    long *addrido,*addrkeido;
    unsigned char *dummyX,*dummyY,*dummyZ,*dummyido,*dummykeido,*dummyRrate; 

    float dt,kt,ut;
    unsigned long previoustime, currenttime;
    struct timeval tv;


void set_TKK(){
  
  // デバイスファイル（シリアルポート）オープン
  fd = open(DEV_NAME,O_RDWR);
  if(fd<0){
    // デバイスの open() に失敗したら
    perror(argv[1]);
    exit(1);
  }
}

// シリアルポートの初期化
void serial_init(int fd)
{
  struct termios tio;
  memset(&tio,0,sizeof(tio));
  tio.c_cflag = CS8 | CLOCAL | CREAD;
  tio.c_cc[VTIME] = 100;
  // ボーレートの設定
  cfsetispeed(&tio,BAUD_RATE);
  cfsetospeed(&tio,BAUD_RATE);
  // デバイスに設定を行う
  tcsetattr(fd,TCSANOW,&tio);
}

void get_time(){
    gettimeofday(&tv,NULL);  //	時間を測定
    previoustime = currenttime;
    currenttime = 1000000 * tv.tv_sec + tv.tv_usec;
    dt = (currenttime - previoustime) / 1000000.0;
 //   printf("dt=%06f\n",dt);	
    kt=kt+dt;
    
}


void csm(double & latitude,double & longitude,float & ramuda0_Ptr,float & h_Ptr,float & one_scale)
{
    int i,t,kouho=0,INS=0,j;
    int rcounter=0;

    int aX,aY,aZ,Rrate1;
    float accelX=0.0,accelY=0.0,accelZ=0.0,Rrate2;
    float vX1=0.0,vY1=0.0,vZ1=0.0,vX2,vY2,vZ2;
    float pX1,pY1,pZ1,pX2,pY2,pZ2,pX3,pY3,pZ3;
    float scale;

    long ido2[1000],keido2[1000];
    long ido1,keido1;
    double ido3,keido3,ido4,keido4;
    double idoscalex,keidoscaley;

    float pi=3.1415926535897;
   
    addrX=&aX;
    addrY=&aY;
    addrZ=&aZ;

    addrRrate=&Rrate1;

    addrido=&ido1;
    addrkeido=&keido1;

    dummyX= (unsigned char *)addrX;
    dummyY= (unsigned char *)addrY;
    dummyZ= (unsigned char *)addrZ;

    dummyRrate=(unsigned char *)addrRrate;

    dummyido= (unsigned char *)addrido;
    dummykeido= (unsigned char *)addrkeido;


 // ここで受信待ち
    len=read(fd,buffer,BUFF_SIZE);
    if(len==0){
      // read()が0を返したら、end of file
      // 通常は正常終了するのだが今回は無限ループ
 //     continue;
    }
    if(len<0){
      printf("%s: ERROR\n",argv[0]);
      // read()が負を返したら何らかのI/Oエラー
      perror("");
      exit(2);
    }
    // read()が正を返したら受信データ数

    // 受信したデータを 16進数形式で表示    
    for(i=0; i<len; i++){
      unsigned char data=buffer[i];
  //    printf("%02X ",data);
    
      //受信データは意味の塊ごとに先頭に16進数で　16 16 06 02　と言うデータがつくことになっているので
      //以下でそのデータを受信するごとに改行して表示するようになっている．
      switch(rcounter){
      case 0:
        if (data==0x16)rcounter++;
        else rcounter=0;
        break;
      case 1:
        if (data==0x16)rcounter++;
        else rcounter=0;
        break;
      case 2:
        if (data==0x06)rcounter++;
        else rcounter=0;
        break;
      case 3:
        if (data==0x02){

//	  printf("\n");

	  if(buffer[i+7]==0x40){
		kouho=i+1;
	  }
	  else if(buffer[i+7]==0x20){
		INS=i+1;
	  }

        }
        rcounter=0;
        break;
      }
    } 

//if(buffer[INS+7+38]!=0x04 || INS==0 || buffer[INS+8+31]!=0x01 )continue; //GPSが有効,屋外モードになるまでループ

  if(buffer[kouho+6]==0x40 ){ 
	for(i=0;i<=3;i++){  		//加速度を抽出

		*dummyX=buffer[kouho+i+7+17];
		*dummyY=buffer[kouho+i+7+21];
		*dummyZ=buffer[kouho+i+7+25];
;
		*dummyRrate=buffer[kouho+i+7+13];

		*dummyX++;
		*dummyY++;
		*dummyZ++;

		*dummyRrate++;
	
	}

	for(i=0;i<=7;i++){  		//緯度,経度を抽出

		*dummyido=buffer[kouho+i+7+41];
		*dummykeido=buffer[kouho+i+7+49];

		*dummyido++;
		*dummykeido++;
	
	}
			

//	printf("ido=%ld ",ido1);
//	printf("keido=%ld \n",keido1);

//	printf("\n");

	aX=aX; 		       	  	  //加速度の分解能0.001
	aY=aY;
	aZ=aZ-9.80665;

	vX2=(accelX+aX)*0.01/2;  	  //加速度→速度
	vY2=(accelY+aY)*0.01/2;
	vZ2=(accelZ+aZ)*0.01/2;

	pX1=(vX1+vX2)*0.01/2;             //速度→位置
	pY1=(vY1+vY2)*0.01/2;
	pZ1=(vZ1+vZ2)*0.01/2;

	accelX=aX;  
	accelY=aY;
	accelZ=aZ; 

	vX1=vX2;
	vY1=vY2;
	vZ1=vZ2;

	pX2=pX2+pX1;
	pY2=pY2+pY1;
	pZ2=pZ2+pZ1;
	
	pX3=pX2;
	pY3=pY2;
	pZ3=pZ2;

	scale=sqrt(pX3*pX3+pY3*pY3+pZ3*pZ3);	

	ido3= (double)ido1;		  //long→double
	keido3= (double)keido1;
	Rrate2=(double)Rrate1;

	ido3=ido3*(180/pi);		  //rad→度
	keido3=keido3*(180/pi);

	idoscalex=ido3-ido4;
	keidoscaley=keido4-keido4;
	

	latitude=ido3;
	longitude=keido3;
	ramuda0_Ptr=Rrate2;
	h_Ptr=ut;
	one_scale=scale;

//	printf("加速度x=%d 加速度y=%d 加速度z=%d\n",aX,aY,aZ);

//	printf("速度x=%d 速度y=%d 速度z=%d\n",vX2,vY2,vZ2);
	
//	printf("px=%f py=%f pz=%f\n",pX1,pY1,pZ1);

//	printf("変位x=%f 変位y=%f 変位z=%f\n",pX3,pY3,pZ3);

//	printf("ido=%ld keido=%ld \n",ido1,keido1);
//	printf("緯度=%ld 経度=%ld \n",ido3,keido3);
	
	ut=ut+kt;
//	printf("kt2=%06f\n",kt);
//	printf("ut=%f\n",ut);
	
	kt=0.0;
		
 	if(ut>3.0 && ido1!=0 && keido1!=0 && buffer[INS+7+38]==0x04){		//3秒経過

	//	scale1=sqrt(pX3*pX3+pY3*pY3+pZ3*pZ3);	

		ido2[j]=ido1;
		keido2[j]=keido1;

		char gps[255];		//緯度,経度データをtxt化
		sprintf(gps,"/home/itolab/work/toma/TKK/TKK/gps/gps.txt");
		char dgps[255];
		ofstream GPS(gps);

		for(i=0;i<=j;i++){			
			sprintf(dgps,"%ld %ld",ido2[i],keido2[i]);
			GPS<<dgps<<endl;
		}
		GPS.close();

		j=j+1;

	//	printf("scale=%f\n",scale1);
	//	printf("変位x=%f 変位y=%f 変位z=%f\n",pX2,pY2,pZ2);
	
		pX3=0.0;
		pY3=0.0;
		pZ3=0.0;
		
		ut=0.0;
	}

   }
}

/* --------------------------------------------------------------------- */
/* メイン                                                                */
/* --------------------------------------------------------------------- */

int main(int argc,char *argv[]){

  //デバイスオープン
  set_TKK();

  // シリアルポートの初期化
  serial_init(fd);
   
  // メインの無限ループ
  while(1){

    get_time();
    csm(latitude,longitude,ramuda0_Ptr,h_Ptr,one_scale);


  }
}
