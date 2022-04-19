#include<opencv2\highgui\highgui.hpp>
#include<opencv2\opencv.hpp>
#include<opencv2\core.hpp>	// 一般定义MAT等数据类型
#include<iostream>
#include<fstream>
#include<algorithm >

using namespace std;		// 声明std的空间
using namespace cv;			// 省去函数前面的cv

struct row_roi
{
	int angle;
	int roi;	// 极坐标极径
};

struct Point2D
{
	int x;
	int y;
};

int ergodic = 1;
int m[401] = { 0 };
int n[401] = { 0 };
double TH[401] = { 0 };
double TH_MID[401] = { 0 };
double TH_MIDD[401] = { 0 };
int  x11, x22, y11, y22;

void quickFindTarget(unsigned char *img0, int height, int width, int *centerx, int *centery, int *greyaverage);
void quickFindTarget1(unsigned char *img0, int height, int width, int *centerx, int *centery, int *greyaverage);
void quickFindTarget2(Mat&src, int height, int width, int *centerx, int *centery, int *greyaverage);
void KittlerMinError(const Mat& inimg, Mat& outimg, int width, int height, int *th);
int Otsu(Mat& src);
void MedFilterImage();
void cir();
void CirR(double*Px, double*Py, int changdu, double &AAAA, double &BBBB, double &RRRR);
void mid(int k, int*max);
vector<int> Max_Cur(int n);
vector<row_roi> FindTarget(vector<int> &cur);
static int frameNum = 0;
void main()
{
	//string str = "C:\\sixrotor1.avi";//"sixrotor1.avi";
	//VideoCapture capture(str);
	//if (!capture.isOpened())
	//{
	//	cout << "Fail to open video! " << endl;
	//	return ;
	//}

	//Size mSize = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH), (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));//获得长宽
	//double mFps = capture.get(CV_CAP_PROP_FPS);//每毫秒多少帧
	//int delay = (int)(1000 / mFps);//根据频率计算延时
	//Mat src;
	//unsigned char *img0 = (unsigned char  *)malloc(640 * 480 * sizeof(unsigned char));
	//bool stop(false);
	//ofstream outfile("2020.txt");
	string path, name;
	Mat src;
	Mat outImage;

	for (int a = 1; a < 400; a++)		// while (!stop)
	{
		double t1 = (double)cvGetTickCount();	// 计时
		/*frameNum++;
		if (!capture.read(src))
		{
			break;
		}*/
		//string path, name;
		//path = "C:\\Users\\Axis\\Desktop\\2020六旋翼修改相关材料\\所有裁剪输出\\" + to_string(a);
		//path = "C:\\Users\\Axis\\Desktop\\2020六旋翼修改相关材料\\2000-2500裁剪输出\\" + to_string(a);
		//name = path + ".jpg";
		//name = "3六旋翼目标(远近效果).png";
		name = "../resources/融合左.png";
		//name = "../resources/图片4.1左.png";
		if (a == 1)
		{
			src = imread(name);
			outImage = imread(name);
		}

		if (a == 2 || a == 3)
		{
			src = imread(name);
			outImage = outImage;		// 继续处理outImage
		}

		//imshow("1", src);
		//waitKey(0);
		Mat greyFrame;
		int centerx = 0, centery = 0;
		int i, j;
		int greyaverage = 0;
		/*int width = mSize.width;
		int height = mSize.height;*/
		int width = src.cols;
		int height = src.rows;
		unsigned char *img0 = (unsigned char  *)malloc(width * height * sizeof(unsigned char));
		cvtColor(src, greyFrame, CV_BGR2GRAY);
		//Mat outImage(mSize.height, mSize.width, CV_8UC3);
		//cvtColor(greyFrame, outImage, CV_GRAY2BGR);
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				img0[i*width + j] = greyFrame.at<uchar>(i, j);
			}

		//quickFindTarget2(greyFrame, height, width, &centery, &centerx, &greyaverage);
		//quickFindTarget1(img0, height, width, &centery, &centerx, &greyaverage);  //centerx是col号  ， centery是row号
		quickFindTarget(img0, height, width, &centery, &centerx, &greyaverage);  //centerx是col号  ， centery是row号
		//靠近

		if (a == 1)
		{
			/*centerx = 314 - 11;
			centery = 229 - 11;*/

			// 融合图左的无人机中心点位置
			centerx = 128;  //357
			centery = 352;  //190

			// 第四组红外左视图
			//centerx = 99;
			//centery = 312;

			//centerx = 400;
			//centery = 143;

		}/*else if (a==2)
		{
			 centerx = 281 - 2;
			 centery = 170 - 7;
		}
		else if (a == 3)
		{
			centerx = 361 - 5;
			centery = 247 - 9;
		}*/

		//有干扰情况
		/*if (a == 1)
		{
			centery = 216;
			centerx = 303;
		}*/

		//无远近效果
		/*if (a == 1)
		{
			centery = 216;
			centerx = 303;
		}
		else if (a == 2)
		{
			centery = 163;
			centerx = 160;
		}
		else if (a == 3)
		{
			centery = 130;
			centerx = 530;
		}*/

		//有远近效果
		/*if (a == 1)
		{
			centery = 216;
			centerx = 306;
		}
		else if (a == 2)
		{
			centery = 152;
			centerx = 162;
		}
		else if (a == 3)
		{
			centery = 113;
			centerx = 535;
		}*/

		//memset(img2out, 0, 640 * 480 * sizeof(unsigned char));
		//二值化
		//for (i = centerx - 100; i <= centerx + 100; i++)
		//	for (j = centery - 100; j <= centery + 100; j++)
		//	{
		//		img2[i * width + j] = img0[i*width + j];
		//	}

		//for (i = centerx - 100; i <= centerx + 100; i++)
		//	for (j = centery - 100; j <= centery + 100; j++)
		//	{
		//		if (img2[i * width + j] < (greyaverage - 20))//这个阈值值得商榷
		//		{
		//			img2out[i * width + j] = 255;//二值化
		//		}
		//		else
		//			img2out[i * width + j] = 0;
		//	}

		Mat image2;		// 
		
		//Mat image1(height, width, CV_8UC1);
		//cvtColor(image1, image1, CV_GRAY2RGB);

		/*for (int i = 0; i < 480; i++)
			for (int j = 0; j < 640; j++)
				image1.at<uchar>(i, j) = img2out[i * 640 + j];*/

		cvtColor(greyFrame, image2, CV_GRAY2RGB);
		for (int i = -2; i <= 2; i++)
			for (int j = -2; j <= 2; j++)
			{
				image2.at<Vec3b>(centery + i, centerx + j)[0] = 255;
				image2.at<Vec3b>(centery + i, centerx + j)[1] = 0;
				image2.at<Vec3b>(centery + i, centerx + j)[2] = 0;
			}

		int boxwidth = 110;//abs(x11 - x22) + 70;//70//126;//
		int boxheight = 70; //abs(y11 - y22) + 70;//70//121;//

		x11 = centerx - boxwidth / 2 + 5;
		x22 = centerx + boxwidth / 2 + 5;
		y11 = centery - boxheight / 2 + 5;
		y22 = centery + boxheight / 2 + 5;

		////int roi = floor(sqrt(boxwidth * boxwidth + boxheight * boxheight)) / 2;
		////框出矩形区域
		//for (int i = -boxwidth / 2 + 5; i < boxwidth / 2 + 5; i++)
		//{
		//	image2.at<Vec3b>(centery - boxheight / 2 + 5, centerx + i)[0] = 0;
		//	image2.at<Vec3b>(centery - boxheight / 2 + 5, centerx + i)[1] = 0;
		//	image2.at<Vec3b>(centery - boxheight / 2 + 5, centerx + i)[2] = 255;
		//	image2.at<Vec3b>(centery + boxheight / 2 + 5, centerx + i)[0] = 0;
		//	image2.at<Vec3b>(centery + boxheight / 2 + 5, centerx + i)[1] = 0;
		//	image2.at<Vec3b>(centery + boxheight / 2 + 5, centerx + i)[2] = 255;
		//}
		//for (int i = -boxheight / 2 + 5; i < boxheight / 2 + 5; i++)
		//{
		//	image2.at<Vec3b>(centery + i, centerx - boxwidth / 2 + 5)[0] = 0;
		//	image2.at<Vec3b>(centery + i, centerx - boxwidth / 2 + 5)[1] = 0;
		//	image2.at<Vec3b>(centery + i, centerx - boxwidth / 2 + 5)[2] = 255;
		//	image2.at<Vec3b>(centery + i, centerx + boxwidth / 2 + 5)[0] = 0;
		//	image2.at<Vec3b>(centery + i, centerx + boxwidth / 2 + 5)[1] = 0;
		//	image2.at<Vec3b>(centery + i, centerx + boxwidth / 2 + 5)[2] = 255;
		//}

		//imwrite("六旋翼投影区域图.jpg",image2);

		int **t = new int*[360 / ergodic];

		int roi = floor(sqrt(boxwidth * boxwidth + boxheight * boxheight)) / 2;

		roi = 24;
		//roi = 20;

		//0629修改两无人机靠近的实验
		/*if (a == 1)
		{
			roi = 52 ;    
		}
		else if (a == 2)
		{
			roi = 35;
		}
		else if (a == 3)
		{
			roi = 52;
		}*/

		for (int i = 0; i < 360 / ergodic; ++i)
		{
			t[i] = new int[roi];
		}
		int max = 0;
		double the, x0, y0, x, y;
		int x1, x2, y1, y2, temp;

		//////////////////////////极坐标转换///////////////////////

		for (int th = 0; th < 360; th = th + ergodic)		// th以圆点360度遍历
		{
			for (int r = 1; r < roi; r++)					// 在th角度上以距离r遍历
			{

				the = th*3.14159 / 180;
				x0 = r*cos(the);
				y0 = r*sin(the);
				x = x0 + centerx;
				y = centery - y0;

				x1 = floor(x);
				x2 = ceil(x);
				y1 = floor(y);
				y2 = ceil(y);

				/************2021/07/15 标出极坐标转换区域*********************/

				if (r == roi - 1)
				{
					image2.at<Vec3b>(y1 + 1, x1+1)[0] = 0;
					image2.at<Vec3b>(y1 + 1, x1+1)[1] = 0;
					image2.at<Vec3b>(y1 + 1, x1+1)[2] = 255;
				}
				/************2021/07/15 标出极坐标转换区域*********************/


				if (x1 == x2 || y1 == y2)
				{
					t[th / ergodic][r] = greyFrame.at<uchar>(y1, x1);
				}
				else
				{
					if (x > 0 && x < greyFrame.cols - 1 && y > 0 && y < greyFrame.rows - 1)
					{
						temp = (x2 - x)*(y2 - y)*greyFrame.at<uchar>(y1, x1) + (x - x1)*(y - y1)*greyFrame.at<uchar>(y2, x2) + (x2 - x)*(y - y1)*greyFrame.at<uchar>(y1, x2) + (x - x1)*(y2 - y)*greyFrame.at<uchar>(y2, x1);
						t[th / ergodic][r] = temp;
					}
					else
					{
						if (x <= 0 | x > greyFrame.cols | y <= 0 | y >= greyFrame.rows)
						{
							int temp_x, temp_y;
							if (x <= 0)
							{
								temp_x = 0;
							}
							else if (x > greyFrame.cols)
							{
								temp_x = greyFrame.cols - 1;
							}
							else
							{
								temp_x = floor(x);
							}

							if (y <= 0)
							{
								temp_y = 0;
							}
							else if (y > greyFrame.rows)
							{
								temp_y = greyFrame.rows - 1;
							}
							else
							{
								temp_y = floor(y);
							}
							t[th / ergodic][r] = greyFrame.at<uchar>(temp_y, temp_x);
						}
					}
					// t[th/ergodic][r]=image.at<uchar>(y1,x1);  
				}
			}
		}
		////////////////////////极坐标转换/////////////////////////////
		
		//2021/07/18存储圆形区域
		imwrite(".\\新数据中间结果\\极坐标转换区域.jpg", image2);


		//////////////////////极坐标图像显示//////////////////////////
		Mat imgg;
		resize(greyFrame, imgg, Size(360 / ergodic, roi));
		for (int i = 0; i < 360 / ergodic; i = i++)
		{
			for (int j = 1; j < roi; j++)
			{
				imgg.at<uchar>(j, i) = t[i][j];
			}
		}
		//imshow("1", imgg);
		imwrite(".\\新数据中间结果\\极坐标图像.jpg", imgg);
		//waitKey(0);
		//////////////////////极坐标图像显示//////////////////////////


		/////////////////////极坐标图像分割，存入img1///////////////////
		Mat img1;
		img1.create(imgg.size(), imgg.type());

		int thre = 0;		// 二值分割的阈值

		//KittlerMinError(imgg, img1, imgg.cols, imgg.rows, &thre);
		thre = Otsu(imgg);
		//thre = 200;
		threshold(imgg, img1, thre, 255, CV_THRESH_BINARY);  // CV_THRESH_BINARY：大于阈值thre部分设置为255，小于阈值部分被设为0

		/*Mat temp_mat;
		temp_mat.create(greyFrame.size(), greyFrame.type());
		threshold(greyFrame, temp_mat, thre, 255, CV_THRESH_BINARY);*/
		imwrite(".\\新数据中间结果\\二值图.jpg", img1);
		/////////////////////极坐标图像分割，存入img1///////////////////

		//Canny边缘检测
		//Canny(img1,img1,9,3);
		//img1 = imread("result.bmp", 0);
		/////////////边缘提取(边缘信息存入m数组里，前后各扩充20个数据，方便后续处理数据)//////////////
		Mat img2;
		img2.create(imgg.size(), imgg.type());
		for (int th = 0; th < 360 / ergodic; th = th++)
		{
			for (int r = roi - 1; r > 0; r--)
			{
				if (img1.at<uchar>(r, th) == 0)
				{
					m[th + 20] = r;
					// cout<<m[th+4]<<endl;
					break;
				}
			}
		}

		for (int i = 0; i < 20; i++)
		{
			m[(360 / ergodic) + 20 + i] = m[20 + i];
			m[0 + i] = m[360 / ergodic + i];
		}
		////////////////////////边缘提取///////////////////////////////

		Mat img22;			// 存储边缘提取结果
		img22.create(imgg.size(), imgg.type());
		for (int i = 0; i < 360 / ergodic; i++)
		{
			for (int j = 1; j < roi; j++)
			{
				img22.at<uchar>(j, i) = 0;
			}
		}
		for (int i = 0; i < 360 / ergodic; i++)
		{
			img22.at<uchar>(m[i + 20], i) = 255;
		}

		imwrite(".\\新数据中间结果\\极坐标各角度对应边界.jpg", img22);

		//...还原各角度对应边界的原像素坐标
		vector<int> y_pixel;
		double angle1;
		for (int i = 0; i < 360; i++)
		{
			angle1 = i*3.14159 / 180;
			double xx0, yy0;
			int xx1, yy1;
			xx0 = centerx + m[i + 20] * cos(angle1);
			yy0 = centery - m[i + 20] * sin(angle1);

			yy1 = ceil(yy0);
			y_pixel.push_back(yy1);
		}

		ofstream outfile(".\\新数据中间结果\\边界对应的y坐标相对图像中心取反.txt");
		for (int i = 0; i < y_pixel.size(); i++)
		{
			outfile << i << "		" << 480 - y_pixel[i] << endl;
		}
		////...还原各角度对应边界的原像素坐标


		// 记下个角度对应边界数据
		ofstream outfile1(".\\新数据中间结果\\边界数据.txt");
		for (int i = 0; i < 360 / ergodic; i++)
		{
			outfile1 << i + 20 << "	" << m[i + 20] << endl;
		}

		MedFilterImage();		// 边缘信息均值滤波


		//////////////////////目标边缘信息显示，存入img2中显示//////////////////
		for (int i = 0; i < 360 / ergodic; i++)
		{
			for (int j = 1; j < roi; j++)
			{
				img2.at<uchar>(j, i) = 0;

			}

		}

		for (int i = 0; i < 360 / ergodic; i++)
		{
			img2.at<uchar>(n[i + 20], i) = 255;
		}
		/* imshow("3",img2);
		waitKey(0)*/

		imwrite(".\\新数据中间结果\\极坐标各角度对应边界拟合图.jpg", img2);

		//////////////////////目标边缘信息显示，存入img2中显示//////////////////

		double t2 = (double)cvGetTickCount();//计时
		cir();   //目标边缘曲率
		t2 = ((double)cvGetTickCount() - t2) / (cvGetTickFrequency() * 1000);
		//mid(6, &max);

		//求目标点
		vector<int> cur;
		cur = Max_Cur(8);

		vector<row_roi> p_target;
		p_target = FindTarget(cur);
		if (p_target.size() == 0)
			continue;
		/*vector<row_roi> p_min;
		p_min = Min_Points();*/

		//标记要害点 (机翼)
		//int target1, target2;
		//target1 = 0; target2 = 1;
		//for (int i = 0; i < p_target.size() - 1; i++)
		//{
		//	if (abs(p_target[i].angle - p_target[i + 1].angle) > 130)
		//	{
		//		target1 = i;
		//		target2 = i + 1;
		//		break;
		//	}
		//}

		////标记要害点1
		//int point_x, point_y;
		//point_x = centerx + p_target[target1].roi*(cos(p_target[target1].angle*3.14159 / 180)); //col
		//point_y = centery - p_target[target1].roi*(sin(p_target[target1].angle*3.14159 / 180));
		//line(outImage, Point(point_x - 10, point_y), Point(point_x + 10, point_y), Scalar(0, 0, 255), 1, CV_AA);
		//line(outImage, Point(point_x, point_y - 10), Point(point_x, point_y + 10), Scalar(0, 0, 255), 1, CV_AA);

		////标记要害点2
		//point_x = centerx + p_target[target2].roi * (cos(p_target[target2].angle * 3.14159 / 180)); //col
		//point_y = centery - p_target[target2].roi * (sin(p_target[target2].angle * 3.14159 / 180));
		//line(outImage, Point(point_x - 10, point_y), Point(point_x + 10, point_y), Scalar(0, 0, 255), 1, CV_AA);
		//line(outImage, Point(point_x, point_y - 10), Point(point_x, point_y + 10), Scalar(0, 0, 255), 1, CV_AA);

		// 标记要害点(机翼和起落架)
		for (int m = 0; m < p_target.size(); m++)
		{

			//int point_x, point_y;
			//point_x = centerx + p_target[m].roi * (cos(p_target[m].angle * 3.14159 / 180)); //col
			//point_y = centery - p_target[m].roi * (sin(p_target[m].angle * 3.14159 / 180));
			//line(outImage, Point(point_x - 10, point_y), Point(point_x + 10, point_y), Scalar(0, 0, 255), 1, CV_AA);
			//line(outImage, Point(point_x, point_y - 10), Point(point_x, point_y + 10), Scalar(0, 0, 255), 1, CV_AA);

			// 三架无人机靠近的要害点标记
			
			if (a == 1 || a == 2)
			{
				if (m == 0)
				{
					int point_x, point_y;
					point_x = centerx + p_target[m].roi * (cos(p_target[m].angle * 3.14159 / 180)); //col
					point_y = centery - p_target[m].roi * (sin(p_target[m].angle * 3.14159 / 180)) - 3;
					line(outImage, Point(point_x - 5, point_y), Point(point_x + 5, point_y), Scalar(0, 0, 255), 1, CV_AA);
					line(outImage, Point(point_x, point_y - 5), Point(point_x, point_y + 5), Scalar(0, 0, 255), 1, CV_AA);
				}
				if (m == 1)
				{
					int point_x, point_y;
					point_x = centerx + p_target[m].roi * (cos(p_target[m].angle * 3.14159 / 180)); //col
					point_y = centery - p_target[m].roi * (sin(p_target[m].angle * 3.14159 / 180));
					line(outImage, Point(point_x - 5, point_y), Point(point_x + 5, point_y), Scalar(0, 0, 255), 1, CV_AA);
					line(outImage, Point(point_x, point_y - 5), Point(point_x, point_y + 5), Scalar(0, 0, 255), 1, CV_AA);
				}
			}
			if (a == 3)
			{
				if (m == 0 || m == 1)
				{
					int point_x, point_y;
					point_x = centerx + p_target[m].roi * (cos(p_target[m].angle * 3.14159 / 180)); //col
					point_y = centery - p_target[m].roi * (sin(p_target[m].angle * 3.14159 / 180));
					line(outImage, Point(point_x - 5, point_y), Point(point_x + 5, point_y), Scalar(0, 0, 255), 1, CV_AA);
					line(outImage, Point(point_x, point_y - 5), Point(point_x, point_y + 5), Scalar(0, 0, 255), 1, CV_AA);
				}
			}

			//两架无人机靠近的要害点标记
			//标记第一架无人机的两侧机翼
			//if (a == 1)
			//{
			//	if (m == 0)
			//	{
			//		int point_x, point_y;
			//		point_x = centerx + p_target[m].roi * (cos(p_target[m].angle * 3.14159 / 180)); //col
			//		point_y = centery - p_target[m].roi * (sin(p_target[m].angle * 3.14159 / 180)) - 3;
			//		line(outImage, Point(point_x - 10, point_y), Point(point_x + 10, point_y), Scalar(0, 0, 255), 1, CV_AA);
			//		line(outImage, Point(point_x, point_y - 10), Point(point_x, point_y + 10), Scalar(0, 0, 255), 1, CV_AA);
			//	}
			//	if (m == 1)
			//	{
			//		int point_x, point_y;
			//		point_x = centerx + p_target[m].roi * (cos(p_target[m].angle * 3.14159 / 180)); //col
			//		point_y = centery - p_target[m].roi * (sin(p_target[m].angle * 3.14159 / 180));
			//		line(outImage, Point(point_x - 10, point_y), Point(point_x + 10, point_y), Scalar(0, 0, 255), 1, CV_AA);
			//		line(outImage, Point(point_x, point_y - 10), Point(point_x, point_y + 10), Scalar(0, 0, 255), 1, CV_AA);
			//	}
			//}
			////标记第二架无人机的两侧机翼
			//if (a == 2)
			//{
			//	if (m == 1 || m == 5)
			//	{
			//		int point_x, point_y;
			//		point_x = centerx + p_target[m].roi * (cos(p_target[m].angle * 3.14159 / 180)); //col
			//		point_y = centery - p_target[m].roi * (sin(p_target[m].angle * 3.14159 / 180));
			//		line(outImage, Point(point_x - 10, point_y), Point(point_x + 10, point_y), Scalar(0, 0, 255), 1, CV_AA);
			//		line(outImage, Point(point_x, point_y - 10), Point(point_x, point_y + 10), Scalar(0, 0, 255), 1, CV_AA);
			//	}
			//}
		}

		// 标记中心点
		line(outImage, Point(centerx - 5, centery), Point(centerx + 5, centery), Scalar(0, 255, 255), 1, CV_AA);
		line(outImage, Point(centerx, centery - 5), Point(centerx, centery + 5), Scalar(0, 255, 255), 1, CV_AA);
		/*for (int i = -2; i <= 2; i++)
			for (int j = -2; j <= 2; j++)
			{
				outImage.at<Vec3b>(centery + i, centerx + j)[0] = 255;
				outImage.at<Vec3b>(centery + i, centerx + j)[1] = 0;
				outImage.at<Vec3b>(centery + i, centerx + j)[2] = 0;
			}*/
		//imshow("2", outImage);
		imwrite(".\\新数据中间结果\\要害点和中心点标记).jpg",outImage);
		//imwrite("三六旋翼目标标记(无远近效果)(两侧机翼、机身).png",outImage);
		//imwrite("六旋翼干扰下目标标记(两侧机翼、机身).png",outImage);
		t1 = ((double)cvGetTickCount() - t1) / (cvGetTickFrequency() * 1000);
		cout << "第"<<a<<"帧图像处理时间: " << t2 << " ms" << endl;
		waitKey(20);
		free(img0);
		if (a == 443)
			continue;
	}
	//imwrite("result3.jpg", src);
	//outfile.close();
	system("pause");
}

void quickFindTarget(unsigned char *img0, int height, int width, int *centerx, int *centery, int *greyaverage)
{
	const int w = width;
	const int h = height;
	/*float sumrows[480] = { 0 };
	float sumrows1[480] = { 0 };
	float sumrows2[480] = { 0 };
	float sumcols[640] = { 0 };
	float sumcols1[640] = { 0 };
	float sumcols2[640] = { 0 };*/

	float sumrows[488] = { 0 };
	float sumrows1[488] = { 0 };
	float sumrows2[488] = { 0 };
	float sumcols[648] = { 0 };
	float sumcols1[648] = { 0 };
	float sumcols2[648] = { 0 };
	int i = 0, j = 0;
	int pointx = 0, pointy = 0;


	/**************水平投影*****************/

	for (i = 0; i<height; i++)//水平投影
		for (j = 0; j<width; j++)
		{
			sumrows[i] += (float)img0[i*width + j] / width;
		}
	sumrows[0] = sumrows[5];
	sumrows[1] = sumrows[5];
	sumrows[2] = sumrows[5];
	sumrows[3] = sumrows[5];

	sumrows[484] = sumrows[483];
	sumrows[485] = sumrows[483];
	sumrows[486] = sumrows[483];
	sumrows[487] = sumrows[483];

	/*sumrows[476] = sumrows[475];
	sumrows[477] = sumrows[475];
	sumrows[478] = sumrows[475];
	sumrows[479] = sumrows[475];*/

	memcpy(sumrows1, sumrows, height * sizeof(float));
	memcpy(sumrows2, sumrows, height * sizeof(float));

	float temp1[5] = { 0 };
	for (j = 0; j<30; j++)//平滑
	{
		memcpy(sumrows, sumrows1, height * sizeof(float));
		for (i = 0; i<height; i++)
		{
			temp1[0] = sumrows[(i - 2 + height) % height];
			temp1[1] = sumrows[(i - 1 + height) % height];
			temp1[2] = sumrows[i];
			temp1[3] = sumrows[(i + 1) % height];
			temp1[4] = sumrows[(i + 2) % height];
			sumrows1[i] = (0.05)*temp1[0] + 0.2*temp1[1] + (0.5)*temp1[2] + 0.2*temp1[3] + 0.05*temp1[4];
		}
	}


	for (i = 0; i<height; i++)//斜率
	{
		temp1[0] = sumrows1[i];
		temp1[1] = sumrows1[(i + 1) % height];
		temp1[2] = sumrows1[(i + 2) % height];
		temp1[3] = sumrows1[(i + 3) % height];
		sumrows2[i] = 3 * temp1[0] - temp1[1] - temp1[2] - temp1[3];
	}

	/********创建txt，并且写入数据***/
	/*FILE *fpWrite = fopen("F:\\Desktop\\data.txt", "w");
	if (fpWrite == NULL)
	{
		return;
	}
	for (i = 0; i<480; i++)
	{
		fprintf(fpWrite, "%f,", sumrows2[i]);
	}
	fclose(fpWrite);*/
	/**************************/

	float tempmin = sumrows2[10];
	float tempmax = sumrows2[10];
	int pointxmin = 0, pointxmax = 0;

	for (i = 10; i<height-10; i++)
	{
		if (tempmax < sumrows2[i])
		{
			tempmax = sumrows2[i];
			pointxmax = i;
		}
		else if (tempmin > sumrows2[i])
		{
			tempmin = sumrows2[i];
			pointxmin = i;
		}
	}
	pointx = (pointxmax + pointxmin) / 2;
	x11 = pointxmin;
	x22 = pointxmax;
	/********************************************/


	/*********计算背景灰度平均值**************/
	*greyaverage = 0;
	float temp = 0;
	//算出全局灰度平均值当做背景平均灰度值
	for (i = 0; i<height; i++)
	{
		temp += sumrows[i] / height;
	}
	*greyaverage = (int)temp;

	/**********************************/



	/****************竖直投影*****************************/
	for (i = 0; i<width; i++)//竖直投影
		for (j = 0; j<height; j++)
		{
			sumcols[i] += (float)img0[j*width + i] / height;
		}
	sumcols[0] = sumcols[5];
	sumcols[1] = sumcols[5];
	sumcols[2] = sumcols[5];
	sumcols[3] = sumcols[5];

	sumcols[644] = sumcols[643];
	sumcols[645] = sumcols[643];
	sumcols[646] = sumcols[643];
	sumcols[647] = sumcols[643];

	/*sumcols[636] = sumcols[635];
	sumcols[637] = sumcols[635];
	sumcols[638] = sumcols[635];
	sumcols[639] = sumcols[635];*/

	memcpy(sumcols1, sumcols, width * sizeof(float));
	memcpy(sumcols2, sumcols, width * sizeof(float));


	for (j = 0; j<30; j++)//平滑
	{
		memcpy(sumcols, sumcols1, width * sizeof(float));
		for (i = 0; i<width; i++)
		{
			temp1[0] = sumcols[(i - 2 + width) % width];
			temp1[1] = sumcols[(i - 1 + width) % width];
			temp1[2] = sumcols[i];
			temp1[3] = sumcols[(i + 1) % width];
			temp1[4] = sumcols[(i + 2) % width];
			sumcols1[i] = (0.05)*temp1[0] + 0.2*temp1[1] + (0.5)*temp1[2] + 0.2*temp1[3] + 0.05*temp1[4];
		}
	}


	/********创建txt，并且写入数据**
	FILE *fpWrite1=fopen("F:\\Desktop\\data1.txt","w");
	if(fpWrite1==NULL)
	{
	return ;
	}
	for(i=0;i<640;i++)
	{
	fprintf(fpWrite1,"%f,",sumcols1[i]);
	}
	fclose(fpWrite1);
	/*************************/


	for (i = 0; i<width-1; i++)//曲率
	{
		temp1[0] = sumcols1[i];
		temp1[1] = sumcols1[(i + 1) % width];
		temp1[2] = sumcols1[(i + 2) % width];
		temp1[3] = sumcols1[(i + 3) % width];
		sumcols2[i] = 3 * temp1[0] - temp1[1] - temp1[2] - temp1[3];
	}

	/********创建txt，并且写入数据****
	FILE *fpWrite2=fopen("F:\\Desktop\\data2.txt","w");
	if(fpWrite2==NULL)
	{
	return ;
	}
	for(i=0;i<640;i++)
	{
	fprintf(fpWrite2,"%f,",sumcols2[i]);
	}
	fclose(fpWrite2);
	/*************************/

	tempmin = sumcols2[10];
	tempmax = sumcols2[10];
	int pointymin = 0, pointymax = 0;

	for (i = 10; i<width-10; i++)
	{
		if (tempmax < sumcols2[i])
		{
			tempmax = sumcols2[i];
			pointymax = i;
		}
		else if (tempmin > sumcols2[i])
		{
			tempmin = sumcols2[i];
			pointymin = i;
		}
	}
	pointy = (pointymax + pointymin) / 2;
	y11 = pointymin;
	y22 = pointymax;
	/*********************************************/

	/*  	for (i = pointx - 100; i<pointx + 100; i++)
	for (j = pointy - 100; j<pointy + 100; j++)
	{
	img0[i*width+pointy-100]=255;
	img0[i*width+pointy+100]=255;
	img0[(pointx-100)*width+j]=255;
	img0[(pointx+100)*width+j]=255;
	}*/

	int counti = 0;
	int countj = 0;
	int count = 0;
	for (i = pointx - 100; i<pointx + 100; i++)
		for (j = pointy - 100; j<pointy + 100; j++)
		{
			if (img0[i*width + j] > (*greyaverage + 18)) //18 = 背景和目标的差异
			{
				counti += i;
				countj += j;
				count++;
			}
		}
	if (count != 0)
	{
		*centerx = counti / count;
		*centery = countj / count;
	}
	else
	{
		*centerx = 240;
		*centery = 320;
	}
}

void quickFindTarget1(unsigned char *img0, int height, int width, int *centerx, int *centery, int *greyaverage)
{
	static int runtimes = 1;


	float data[24 * 32] = { 0 };
	int target[40][2] = { 0 };//目标像素
	int i = 0, j = 0;



	for (i = 0; i < 24; i++)
		for (j = 0; j < 32; j++)
		{
			data[i * 32 + j] = (float)img0[i * 20 * width + j * 20];
		}


	int count = -1;

	for (i = 0; i < 24; i++)
	{
		for (j = 1; j < 32; j++)
		{
			if (abs(data[i * 32 + j] - data[i * 32 + j - 1]) > 15)//将固定的经验值15换位自适应的阈值
			{
				count++;
				target[count][0] = i;
				target[count][1] = j;
			}
		}
	}

	int greysum = 0, greysum1 = 0;
	count = 0;
	int count1 = 0;
	//计算出背景的平均灰度
	for (i = 1; i < 23; i++)
		for (j = 1; j < 31; j++)//去掉最边上的灰度
		{
			greysum += img0[i * 20 * width + j * 20];
			count++;
		}
	for (i = 0; i < 40; i++)
	{
		if (target[i][0] != 0 && target[i][1] != 0)
		{
			greysum1 += img0[target[i][0] * 20 * width + target[i][1] * 20];
			count1++;
		}
	}

	if (count - count1 != 0)
	{
		*greyaverage = (greysum - greysum1) / (count - count1);//背景灰度平均值
	}
	else
		*greyaverage = 150;



	if (runtimes == 1)
	{
		int count2[24] = { 0 };//计算最长的波动行
		for (i = 0; i < 40; i++)
		{
			for (j = 1; j < 24; j++)//不算第0行
			{
				if (target[i][0] == j)
				{
					count2[j]++;
					break;
				}
			}
		}

		//求出梯度大于10最长的行
		int temp = count2[0];
		int rowposition = 0;
		for (i = 0; i < 24; i++)
		{
			if (temp < count2[i])
			{
				temp = count2[i];
				rowposition = i;
			}
		}

		int countmaxrow = 0;//最长波动行是否唯一

		for (i = 0; i < 24; i++)
		{
			if (temp == count2[i])
			{
				countmaxrow++;
			}
		}

		int m = 0;
		if (countmaxrow != 1)
		{
			for (i = 0; i < 24;)
			{
				if (temp == count2[i])
				{
					for (j = 0; j < 40;)
					{
						if (target[j][0] == i)
						{
							for (m = j; m < j + count2[i]; m++)
							{
								if (img0[target[m][0] * width + target[m][1]] < (*greyaverage - 10))
								{
									rowposition = i;
									break;
								}
							}
							break;
						}
						else j++;
					}
					i += count2[i];
				}
				else i++;
			}

		}

		int pos=0;
		for (i = 0; i < 40; i++)
		{
			if (target[i][0] == rowposition && count2[rowposition] >= 2)
			{
				if (abs(target[i][1] - target[i + 1][1]) > 5)
				{
					pos = i + 1;//两个相差列数大于5，表明不是目标点
					temp = temp - 1;
				}
				else pos = i;//第一次目标行数的存储位置
				if (abs(target[i + temp - 2][1] - target[i + temp - 1][1]) > 5) temp = temp - 1;
				break;
			}
		}
		int  pointx = rowposition * 20;
		int  pointy = (target[pos][1] + target[pos + temp - 1][1]) / 2 * 20;

		count = 0;
		int counti = 0;
		int countj = 0;
		for (i = pointx - 100; i < pointx + 100; i++)
			for (j = pointy - 100; j < pointy + 100; j++)
			{
				if (img0[i*width + j] < (*greyaverage - 18))
				{
					counti += i;
					countj += j;
					count++;
				}
			}
		if (count != 0)
		{
			*centerx = counti / count;
			*centery = countj / count;
		}
		else
		{
			*centerx = 240;
			*centery = 320;
		}
		//runtimes = 2;
	}
	else
	{
		int i, j;
		if (*centerx < 140) { *centerx = 140; }//140=120+20
		else if (*centerx > 340) { *centerx = 340; }
		if (*centery < 140) { *centerx = 140; }
		else if (*centery > 500) { *centerx = 500; }

		count = 0;
		int counti = 0;
		int countj = 0;
		for (i = *centerx - 120; i < *centerx + 120; i++)
			for (j = *centery - 120; j < *centery + 120; j++)
			{
				if (img0[i*width + j] < (*greyaverage - 18))
				{
					counti += i;
					countj += j;
					count++;
				}
			}
		if (count != 0)
		{
			*centerx = counti / count;
			*centery = countj / count;
		}
		else
		{
			*centerx = 240;
			*centery = 320;
		}
	}
}

void KittlerMinError(const Mat& inimg, Mat& outimg, int width, int height, int *th)
{
	//	   double time=0;  
	//    double counts=0;  
	//    LARGE_INTEGER nFreq;  
	// LARGE_INTEGER nBeginTime;  
	// LARGE_INTEGER nEndTime;  
	// QueryPerformanceFrequency(&nFreq);  
	//    QueryPerformanceCounter(&nBeginTime);//开始计时  
	////////直方图统计////////
	int avg = 0;
	int min = 255;
	int max = 0;
	int tem = 0;
	int HistGram[257] = { 0 };
	int sum = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			tem = inimg.at<uchar>(i, j);
			sum = sum + tem;
			if (tem > max)
			{
				max = tem;
			}
			if (tem < min)
			{
				min = tem;
			}
			HistGram[tem + 1]++;
		}
	}
	avg = sum / (height*width);
	//  *th=avg;

	////////直方图统计////////
	//	  QueryPerformanceCounter(&nEndTime);
	//  time=(double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart;
	//  cout<<"程序执行时间："<<time*1000<<"ms"<<endl;  

	//////////求阈值/////////
	int X, Y;
	int MinValue, MaxValue;
	int Threshold;
	int PixelBack, PixelFore;
	double OmegaBack, OmegaFore, MinSigma, Sigma, SigmaBack, SigmaFore;
	for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++);
	for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--);
	if (MaxValue == MinValue) *th = MaxValue;          // 图像中只有一个颜色             
	if (MinValue + 1 == MaxValue) *th = MinValue;      // 图像中只有二个颜色
	Threshold = -1;
	MinSigma = 1E+20;
	for (Y = MinValue; Y < MaxValue; Y++)
	{
		PixelBack = 0; PixelFore = 0;
		OmegaBack = 0; OmegaFore = 0;
		for (X = MinValue; X <= Y; X++)
		{
			PixelBack += HistGram[X];
			OmegaBack = OmegaBack + X * HistGram[X];
		}
		for (X = Y + 1; X <= MaxValue; X++)
		{
			PixelFore += HistGram[X];
			OmegaFore = OmegaFore + X * HistGram[X];
		}
		OmegaBack = OmegaBack / PixelBack;
		OmegaFore = OmegaFore / PixelFore;
		SigmaBack = 0; SigmaFore = 0;
		for (X = MinValue; X <= Y; X++) SigmaBack = SigmaBack + (X - OmegaBack) * (X - OmegaBack) * HistGram[X];
		for (X = Y + 1; X <= MaxValue; X++) SigmaFore = SigmaFore + (X - OmegaFore) * (X - OmegaFore) * HistGram[X];
		if (SigmaBack == 0 || SigmaFore == 0)
		{
			if (Threshold == -1)
				Threshold = Y;
		}
		else
		{
			SigmaBack = sqrt(SigmaBack / PixelBack);
			SigmaFore = sqrt(SigmaFore / PixelFore);
			Sigma = 1 + 2 * (PixelBack * log(SigmaBack / PixelBack) + PixelFore * log(SigmaFore / PixelFore));
			if (Sigma < MinSigma)
			{
				MinSigma = Sigma;
				Threshold = Y;
			}
		}
	}
	*th = Threshold;


	//////////求阈值///////// 

	//////////二值化/////////
	for (int i = 1; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (inimg.at<uchar>(i, j) > *th)
			{
				outimg.at<uchar>(i, j) = 255;
			}
			else
			{
				outimg.at<uchar>(i, j) = 0;

			}
		}

	}
	//////////二值化/////////


}

void MedFilterImage()
{
	for (int i = 20; i < 360 / ergodic + 20; i++)
	{
		n[i] = (m[i - 1] + m[i] + m[i + 1] + m[i + 2] + m[i - 2] + m[i + 3] + m[i - 3] + m[i + 4] + m[i - 4] + m[i + 5] + m[i - 5]) / 11;
		 // n[i]=(m[i-1]+m[i]+m[i+1]+m[i+2]+m[i-2]+m[i+3]+m[i-3])/7;
		//n[i]=(m[i-1]+m[i]+m[i+1])/3;
		//n[i] = (m[i - 1] + m[i] + m[i + 1] + m[i + 2] + m[i - 2] + m[i + 3] + m[i - 3] + m[i + 4] + m[i - 4] ) / 9;
	}
	for (int i = 0; i < 20; i++)
	{
		n[(360 / ergodic + 20) + i] = n[20 + i];
		n[0 + i] = n[ergodic + i];
	}
}


//阈值分割
int Otsu(Mat& src)
{
	int height = src.rows;
	int width = src.cols;
	long size = height * width;

	//histogram
	float histogram[256] = { 0 };
	for (int m = 0; m < height; m++)
	{
		//unsigned char* p = (unsigned char*)src->imageData + src->widthStep * m;
		for (int n = 0; n < width; n++)
		{
			int p = src.at<uchar>(m, n);
			histogram[p]++;
		}
	}

	int threshold;
	long sum0 = 0, sum1 = 0; //存储前景的灰度总和和背景灰度总和
	long cnt0 = 0, cnt1 = 0; //前景的总个数和背景的总个数
	double w0 = 0, w1 = 0; //前景和背景所占整幅图像的比例
	double u0 = 0, u1 = 0;  //前景和背景的平均灰度
	double variance = 0; //最大类间方差
	int i, j;
	double u = 0;
	double maxVariance = 0;
	for (i = 1; i < 256; i++) //一次遍历每个像素
	{
		sum0 = 0;
		sum1 = 0;
		cnt0 = 0;
		cnt1 = 0;
		w0 = 0;
		w1 = 0;
		for (j = 0; j < i; j++)
		{
			cnt0 += histogram[j];
			sum0 += j * histogram[j];
		}

		u0 = (double)sum0 / cnt0;
		w0 = (double)cnt0 / size;

		for (j = i; j <= 255; j++)
		{
			cnt1 += histogram[j];
			sum1 += j * histogram[j];
		}

		u1 = (double)sum1 / cnt1;
		w1 = 1 - w0; // (double)cnt1 / size;

		u = u0 * w0 + u1 * w1; //图像的平均灰度
							   //printf("u = %f\n", u);
							   //ariance =  w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		variance = w0 * w1 *  (u0 - u1) * (u0 - u1);
		if (variance > maxVariance)
		{
			maxVariance = variance;
			threshold = i;
		}
	}

	//printf("threshold = %d\n", threshold);
	return threshold;
}

//求曲率
void cir()
{
	double AAAA = 0;
	double BBBB = 0;
	double RRRR = 0;
	double x[7] = { 0 };
	double y[7] = { 0 };
	double *Px = x;
	double *Py = y;
	//int RR[360]={0};
	ofstream outfile;
	outfile.open(".\\新数据中间结果\\曲率.txt");
	for (int ii = 20; ii < ((360 / ergodic) + 20); ii++)
	{
		x[0] = ii - 6;
		x[1] = ii - 3;
		x[2] = ii;
		x[3] = ii + 3;
		x[4] = ii + 6;

		y[0] = n[ii - 6];
		y[1] = n[ii - 3];
		y[2] = n[ii];
		y[3] = n[ii + 3];
		y[4] = n[ii + 6];
		CirR(Px, Py, 5, AAAA, BBBB, RRRR);

		//x[0]=ii-3;
		//x[1]=ii-2;   
		//x[2]=ii-1;
		//x[3]=ii;
		//x[4]=ii+1;
		//x[5]=ii+2;
		//x[6]=ii+3;

		//y[0]=n[ii-3];
		//y[1]=n[ii-2];
		//y[2]=n[ii-1];   
		//y[3]=n[ii];
		//y[4]=n[ii+1];
		//y[5]=n[ii+2];
		//y[6]=n[ii+3];

		//CirR(Px,Py,7,AAAA,BBBB,RRRR);


		//  x[0]=ii-1;   
		//x[1]=ii;
		//x[2]=ii+1;


		//y[0]=n[ii-1];   
		//y[1]=n[ii];
		//y[2]=n[ii+1];

		// CirR(Px,Py,3,AAAA,BBBB,RRRR);

		//x[0]=ii-2;   
		//x[1]=ii-1;
		//x[2]=ii;
		//x[3]=ii+1;
		//x[4]=ii+2;

		//y[0]=n[ii-2];   
		//y[1]=n[ii-1];
		//y[2]=n[ii];
		//y[3]=n[ii+1];
		//y[4]=n[ii+2];
		// CirR(Px,Py,3,AAAA,BBBB,RRRR);
		// circleLeastFit(Px,Py,5,AAAA,BBBB,RRRR);
		if (RRRR == 0)
		{
			TH[ii - 20] = 0;
			outfile << 0 << endl;
			//cout << "第" << ii - 20 << "度：" << 0 << endl;
		}
		else
		{
			TH[ii - 20] = 1 / RRRR;
			outfile << 1 / RRRR << endl;
			//cout << "第" << ii - 20 << "度：" << 1 / RRRR << endl;
		}
		//
		//  cout<<"第"<<ii-20<<"度："<<BBBB<<endl;
	}
	outfile.close();

}

void CirR(double*Px, double*Py, int changdu, double &AAAA, double &BBBB, double &RRRR)
{
	double X1, X2, X3, Y1, Y2, Y3;
	double a, b，r;//圆心坐标半径
	double **shuzu = new double*[3];
	for (int i = 0; i < 3; ++i)
	{
		shuzu[i] = new double[changdu];
	}

	int NUM = 0;
	for (NUM = 0; NUM < changdu; NUM++)
	{
		shuzu[0][NUM] = NUM;
		shuzu[1][NUM] = Px[NUM];
		shuzu[2][NUM] = Py[NUM];

	}
	int Max_number = 2; int number;
	double count[3] = { 0 };
	double zuiyoujie[4] = { 0 };
	zuiyoujie[3] = 100;
	int k1 = 0;
	int k2 = 0;
	int k3 = 0;
	double x = 0;
	double y = 0;
	double point, distance;
	double aa, bb, rr;
	double distancesum;
	for (k1 = 0; k1 < changdu - 2; k1++)
	{
		for (k2 = k1 + 1; k2 < changdu - 1; k2++)
		{
			for (k3 = k2 + 1; k3 < changdu; k3++)
			{
				distancesum = 0;
				X1 = shuzu[1][k1];
				Y1 = shuzu[2][k1];

				X2 = shuzu[1][k2];
				Y2 = shuzu[2][k2];

				X3 = shuzu[1][k3];
				Y3 = shuzu[2][k3];
				if (((X2 - X1)*(Y3 - Y1) - (X3 - X1)*(Y2 - Y1)) != 0)
				{
					double  a = ((X2*X2 + Y2*Y2 - (X1*X1 + Y1*Y1)) * 2 * (Y3 - Y1) - (X3*X3 + Y3*Y3 - (X1*X1 + Y1*Y1)) * 2 * (Y2 - Y1)) / (4 * ((X2 - X1)*(Y3 - Y1) - (X3 - X1)*(Y2 - Y1)));
					double  b = (2 * (X2 - X1)*(X3*X3 + Y3*Y3 - (X1*X1 + Y1*Y1)) - 2 * (X3 - X1)*(X2*X2 + Y2*Y2 - (X1*X1 + Y1*Y1))) / (4 * ((X2 - X1)*(Y3 - Y1) - (X3 - X1)*(Y2 - Y1)));
					double  r = sqrt((double)(X1 - a)*(X1 - a) + (double)(Y1 - b)*(Y1 - b));
					//   double  r = (double)((X1-a)*(X1-a)+(Y1-b)*(Y1-b));
					count[0] = a;
					count[1] = b;
					count[2] = r;
					//  cout<<mmm<<":"<<r<<endl;
					//    mmm++;
				}
				else
				{
					count[2] = 0;
					// cout<<mmm<<":"<<count[2]<<endl;
					//  mmm++;
					continue;

				}
				//////////////
				for (int i = 0; i < changdu; i++)
				{
					aa = count[0];
					bb = count[1];
					rr = count[2];
					x = shuzu[1][i];
					y = shuzu[2][i];
					point = (double)sqrt((x - aa)*(x - aa) + (y - bb)*(y - bb));
					distance = abs(point - rr);
					distancesum = distancesum + distance;
				}
				if (distancesum < zuiyoujie[3])
				{
					zuiyoujie[0] = rr;
					zuiyoujie[1] = aa;
					zuiyoujie[2] = bb;
					zuiyoujie[3] = distancesum;
				}


			}
		}

	}

	AAAA = zuiyoujie[1];
	BBBB = zuiyoujie[2];
	RRRR = zuiyoujie[0];
	// RRRR=sqrt(RRRR);
	for (int i = 0; i < 3; ++i)
		delete[] shuzu[i];
	delete[] shuzu;


}

void mid(int k, int*max)
{
	for (int i = 0; i < 360 / ergodic; i++)
	{
		TH_MID[i + 20] = TH[i];

	}
	for (int i = 0; i < 20; i++)
	{
		TH_MID[(360 / ergodic + 20) + i] = TH_MID[20 + i];
		TH_MID[0 + i] = TH_MID[360 / ergodic + i];

	}
	///////////////
	//ofstream outfile1;
	// outfile1.open("D:\\data777_mid.txt");
	double sum = 0;
	double avg = 0;
	for (int i = 20; i < (360 / ergodic + 20); i++)
	{
		sum = TH_MID[i];
		avg = 0;
		for (int j = 1; j < k - 1; j++)
		{
			sum = TH_MID[i - j] + TH_MID[i + j] + sum;

		}
		avg = sum / (2 * k - 2 * 2 + 1);
		TH_MIDD[i - 20] = avg;
		if (avg > TH_MIDD[*max])
		{
			*max = i - 20;
		}
		//cout<<i-20<<":"<<avg<<endl;
		//	outfile1<<avg<<"\r\n";

	}
	//	 outfile1.close();
}

//2020_XK

//寻找目标点候选点（曲率最大）
vector<int> Max_Cur(int n)
{
	vector<int> cur;
	int count = n;
	double Temp_TH[401];
	for (int i = 0; i < 401; i++)
	{
		Temp_TH[i] = TH[i];
	}

	while (count) 
	{
		double maxcur = Temp_TH[0];
		int index = 0;
		for (int i = 0; i < 360; i++)
		{
			if (maxcur < Temp_TH[i])
			{
				maxcur = Temp_TH[i];
				index = i;
			}
		}
		cur.push_back(index);
		if (index >= 5)
		{
			Temp_TH[index - 5] = Temp_TH[index - 4] = Temp_TH[index - 3] = Temp_TH[index - 2] = Temp_TH[index - 1] = Temp_TH[index] = 0;
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = 0;
		}
		else if (index == 4)
		{
			Temp_TH[index - 4] = Temp_TH[index - 3] = Temp_TH[index - 2] = Temp_TH[index - 1] = Temp_TH[index] = 0;
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = 0;
		}
		else if (index == 3)
		{
			Temp_TH[index - 3] = Temp_TH[index - 2] = Temp_TH[index - 1] = Temp_TH[index] = 0;
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = 0;
		}
		else if (index == 2)
		{
			Temp_TH[index - 2] = Temp_TH[index - 1] = Temp_TH[index] = 0;
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = 0;
		}
		else if(index == 1)
		{
			Temp_TH[index - 1] = Temp_TH[index] = 0;
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = 0;
		}
		else
		{
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = Temp_TH[index] = 0;
		}
		count--;
	}

	sort(cur.begin(), cur.end(),less<int>());
	return cur;
}

//确定目标点
vector<row_roi> FindTarget(vector<int> &cur)
{
	vector<row_roi> target_point;
	int sum = 0;
	int avg = 0;
	for (int i = 0; i < cur.size(); i++)
	{
		sum += n[cur[i] + 20];
	}

	avg = sum / cur.size();

	for (int i = 0; i < cur.size(); i++)
	{

		if (n[cur[i] + 20] > avg)
		{
			row_roi temp;
			temp.angle = cur[i];
			temp.roi = n[cur[i] + 20];
			target_point.push_back(temp);
		}
	}

	return target_point;
}


//img0改Mat
void quickFindTarget2(Mat&src, int height, int width, int *centerx, int *centery, int *greyaverage)
{

	static int runtimes = 1;

	unsigned char *img0 = (unsigned char  *)malloc(width * height * sizeof(unsigned char));

	if (src.channels() == 1)
	{
		//cvtColor(src, greyFrame, CV_BGR2GRAY);
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				img0[i*width + j] = src.at<uchar>(i, j);
			}
	}
	else if (src.channels() == 3)
	{
		cvtColor(src, src, CV_BGR2GRAY);
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				img0[i*width + j] = src.at<uchar>(i, j);
			}
	}

	float data[24 * 32] = { 0 };
	int target[40][2] = { 0 };			// 目标像素
	int i = 0, j = 0;

	for (i = 0; i < 24; i++)
		for (j = 0; j < 32; j++)
		{
			data[i * 32 + j] = (float)img0[i * 20 * width + j * 20];
		}

	int count = -1;

	for (i = 0; i < 24; i++)
	{
		for (j = 1; j < 32; j++)
		{
			if (abs(data[i * 32 + j] - data[i * 32 + j - 1]) > 15)//将固定的经验值15换位自适应的阈值
			{
				count++;
				target[count][0] = i;
				target[count][1] = j;
			}
		}
	}

	int greysum = 0, greysum1 = 0;
	count = 0;
	int count1 = 0;
	//计算出背景的平均灰度
	for (i = 1; i < 23; i++)
		for (j = 1; j < 31; j++)//去掉最边上的灰度
		{
			greysum += img0[i * 20 * width + j * 20];
			count++;
		}
	for (i = 0; i < 40; i++)
	{
		if (target[i][0] != 0 && target[i][1] != 0)
		{
			greysum1 += img0[target[i][0] * 20 * width + target[i][1] * 20];
			count1++;
		}
	}

	if (count - count1 != 0)
	{
		*greyaverage = (greysum - greysum1) / (count - count1);//背景灰度平均值
	}
	else
		*greyaverage = 150;

	if (runtimes == 1)
	{
		int count2[24] = { 0 };//计算最长的波动行
		for (i = 0; i < 40; i++)
		{
			for (j = 1; j < 24; j++)//不算第0行
			{
				if (target[i][0] == j)
				{
					count2[j]++;
					break;
				}
			}
		}

		//求出梯度大于10最长的行
		int temp = count2[0];
		int rowposition = 0;
		for (i = 0; i < 24; i++)
		{
			if (temp < count2[i])
			{
				temp = count2[i];
				rowposition = i;
			}
		}

		int countmaxrow = 0;//最长波动行是否唯一

		for (i = 0; i < 24; i++)
		{
			if (temp == count2[i])
			{
				countmaxrow++;
			}
		}

		int m = 0;
		if (countmaxrow != 1)
		{
			for (i = 0; i < 24;)
			{
				if (temp == count2[i])
				{
					for (j = 0; j < 40;)
					{
						if (target[j][0] == i)
						{
							for (m = j; m < j + count2[i]; m++)
							{
								if (img0[target[m][0] * width + target[m][1]] < (*greyaverage - 10))
								{
									rowposition = i;
									break;
								}
							}
							break;
						}
						else j++;
					}
					i += count2[i];
				}
				else i++;
			}

		}

		int pos;
		for (i = 0; i < 40; i++)
		{
			if (target[i][0] == rowposition && count2[rowposition] >= 2)
			{
				if (abs(target[i][1] - target[i + 1][1]) > 5)
				{
					pos = i + 1;//两个相差列数大于5，表明不是目标点
					temp = temp - 1;
				}
				else pos = i;//第一次目标行数的存储位置
				if (abs(target[i + temp - 2][1] - target[i + temp - 1][1]) > 5) temp = temp - 1;
				break;
			}
		}
		int  pointx = rowposition * 20;
		int  pointy = (target[pos][1] + target[pos + temp - 1][1]) / 2 * 20;

		count = 0;
		int counti = 0;
		int countj = 0;
		for (i = pointx - 100; i < pointx + 100; i++)
			for (j = pointy - 100; j < pointy + 100; j++)
			{
				if (img0[i*width + j] < (*greyaverage - 18))
				{
					counti += i;
					countj += j;
					count++;
				}
			}
		if (count != 0)
		{
			*centerx = counti / count;
			*centery = countj / count;
		}
		else
		{
			*centerx = 240;
			*centery = 320;
		}
		runtimes = 2;
	}
	else
	{


		int i, j;
		if (*centerx < 140) { *centerx = 140; }//140=120+20
		else if (*centerx > 340) { *centerx = 340; }
		if (*centery < 140) { *centerx = 140; }
		else if (*centery > 500) { *centerx = 500; }

		count = 0;
		int counti = 0;
		int countj = 0;
		for (i = *centerx - 120; i < *centerx + 120; i++)
			for (j = *centery - 120; j < *centery + 120; j++)
			{
				if (img0[i*width + j] < (*greyaverage - 18))
				{
					counti += i;
					countj += j;
					count++;
				}
			}
		if (count != 0)
		{
			*centerx = counti / count;
			*centery = countj / count;
		}
		else
		{
			*centerx = 240;
			*centery = 320;
		}

	}
}
