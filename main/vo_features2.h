#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <opencv2/core/core.hpp>
#include <stdio.h>
#include "opencv2/core.hpp"

#include <iostream>
#include <ctype.h>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>
#include <string>


//特徴点抽出（手動）を代入
void features(vector<Point2f>& points1,vector<Point2f>& points2){

/*とま
Mat l1 = (Mat_<Point2f>(20,1) << Point2f(258.0,175.0),Point2f(185.0,362.0),Point2f(544.0,296.0),Point2f(95.0,226.0),Point2f(268.0,381.0),Point2f(342.0,358.0),Point2f(391.0,314.0),Point2f(100.0,279.0),Point2f(297.0,220.0),Point2f(189.0,218.0),Point2f(528.0,426.0),Point2f(304.0,415.0),Point2f(322.0,181.0),Point2f(118.0,168.0),Point2f(109.0,358.0),Point2f(52.0,366.0),Point2f(136,186),Point2f(257,246),Point2f(548,225),Point2f(390,221));
 

Mat l2 = (Mat_<Point2f>(20,1) << Point2f(311,177),Point2f(239,361),Point2f(613,303),Point2f(156,226),Point2f(321,383),Point2f(396,362),Point2f(447,319),Point2f(160,277),Point2f(350,222),Point2f(243,219),Point2f(594,439),Point2f(358,421),Point2f(375,183),Point2f(177,171),Point2f(168,354),Point2f(116,359),Point2f(198,190),Point2f(307,245),Point2f(618,228),Point2f(445,223));
*/

/*けん玉
Mat l1 = (Mat_<Point2f>(9,1) << Point2f(267.0,295.0),Point2f(264.0,256.0),Point2f(276.0,249.0),Point2f(361.0,168.0),Point2f(357.0,152.0),Point2f(367.0,146.0),Point2f(379.0,21.0),Point2f(381.0,61.0),Point2f(380.0,73.0));

Mat l2 = (Mat_<Point2f>(9,1) << Point2f(250.0,260.0),Point2f(258.0,212.0),Point2f(274.0,206.0),Point2f(418.0,229.0),Point2f(419.0,206.0),Point2f(430.0,202.0),Point2f(475.0,109.0),Point2f(470.0,147.0),Point2f(468.0,158.0));
*/
//ボードスライド
Mat l1 = (Mat_<Point2f>(8,1) << Point2f(213,54),Point2f(213,116),Point2f(214,176),Point2f(213,233),Point2f(333,61),Point2f(333,118),Point2f(332,179),Point2f(329,234));

Mat l2 = (Mat_<Point2f>(8,1) << Point2f(72,45),Point2f(72,107),Point2f(72,169),Point2f(68,228),Point2f(198,52),Point2f(196,112),Point2f(196,175),Point2f(196,234));
//

/*ボード回転
Mat l1 = (Mat_<Point2f>(8,1) << Point2f(168,74),Point2f(164,145),Point2f(161,216),Point2f(157,289),Point2f(28,76),Point2f(24,143),Point2f(18,213),Point2f(12,285));

Mat l2 = (Mat_<Point2f>(8,1) << Point2f(602,153),Point2f(603,221),Point2f(601,291),Point2f(600,365),Point2f(471,171),Point2f(468,226),Point2f(466,284),Point2f(462,346));
*/
l1.copyTo(points1);
l2.copyTo(points2);


}
