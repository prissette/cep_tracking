#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main(int, char**)
{
VideoCapture cap(0); // Ouvrir camera par defaut =0 si sur un port --> 1 ou 2 ...
if(!cap.isOpened())  // Vérif si ouverture
    return -1;

Mat edges;

while(true)
    {
    Mat frame;
    cap >> frame; // Prise nouvelle photo de la camera
    GaussianBlur(frame, frame, Size(7,7), 3.5, 3.5);//flouter, lisser
    flip(frame, frame, 1);




    Mat HSV; 
    cvtColor(frame, HSV, CV_BGR2HSV);


    vector<Mat> hsv_planes;
    split(HSV, hsv_planes);
    Mat h = hsv_planes[0]; // H channel
    Mat s = hsv_planes[1]; // S channel
    Mat v = hsv_planes[2]; // V channel



    //// Detection Orange
    Scalar hsv_l(14, 97, 215);
    Scalar hsv_h(26, 255, 255);
    Mat bw;
    inRange(HSV, hsv_l, hsv_h, bw);
    imshow("Specific Colour", bw);


    int roi_size=64;
    int detect_x=bw.cols/2;
    int detect_y=bw.rows/2;
    int detect_fitness=-1;

    // Tirage de 100 positions, selection du meilleur fitness
    for(int i=0; i<100; i++)
        {
        int x = rand()%(bw.cols-2*roi_size)+roi_size;
        int y = rand()%(bw.rows-2*roi_size)+roi_size;
        rectangle( frame, Point(x-roi_size/2, y-roi_size/2 ), Point( x+roi_size/2, y+roi_size/2), Scalar( 0, 255, 0 ), +1, 4 );

        cout << x << " " << y << endl;
        Rect region_of_interest = Rect(x-roi_size/2, y-roi_size/2, roi_size, roi_size);
        cout << region_of_interest << endl;
        Mat image_roi = bw(region_of_interest);
        int fitness = countNonZero(image_roi);

        if (fitness>detect_fitness)
            {
            detect_x=x;
            detect_y=y;
            detect_fitness=fitness;
            }

        }

    rectangle( frame, Point(detect_x-roi_size/2, detect_y-roi_size/2 ), Point( detect_x+roi_size/2, detect_y+roi_size/2), Scalar( 255, 255, 0 ), +5, 4 );

    imshow("frame", frame);
    if(waitKey(30) >= 0) break;


    }

 
return 0;
}
