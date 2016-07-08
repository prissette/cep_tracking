#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <tuple>

using namespace cv;
using namespace std;


 
 
int main(int, char**)
{
VideoCapture cap(0); // Ouvrir camera par defaut =0 si sur un port --> 1 ou 2 ...
if(!cap.isOpened())  // Vérif si ouverture
    return -1;
namedWindow("frame", 1);
int roi_size=64;
Mat bw;
cap >> bw; 

vector<tuple<int,int,int>> individu(100);

for(int i=0; i<individu.size(); ++i)
    {
    int x = rand()%(bw.cols-2*roi_size)+roi_size;
    int y = rand()%(bw.rows-2*roi_size)+roi_size;
    individu[i] = make_tuple(x,y,-1);
    }




while(true)
    {
    Mat frame;
    cap >> frame; // Prise nouvelle photo de la camera
    //GaussianBlur(frame, frame, Size(7,7), 3.5, 3.5);//flouter, lisser

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


   
    

    // recalcul des fitness de la population
    for(int ind=0; ind<individu.size(); ++ind)
    {
       int x=get<0>(individu[ind]);
        int y=get<1>(individu[ind]);
        Rect region_of_interest = Rect(x-roi_size/2, y-roi_size/2, roi_size, roi_size);
        Mat image_roi = bw(region_of_interest);
        get<2>(individu[ind]) = countNonZero(image_roi);
    }

    // 100 iterations de CEP
    for(int i=0; i<100; i++)
        {
          
        int ind1=rand()%individu.size();
        int ind2=(ind1+rand()%(individu.size()-1))%individu.size();
        
        // si ind1 meilleur
        if ( get<2>(individu[ind1]) > get<2>(individu[ind2]) )
            individu[ind2]=individu[ind1]; // ind2 copie ind1
        else  
            individu[ind1]=individu[ind2]; // sinon ind1 copie ind2

        // ind1 et ind2 identiques. ind2 se deplace de 50 pas
        // (ce serait mieux avec un tirage gaussien..)
        for (int k=0; k<10; ++k)
            {
            get<0>(individu[ind2]) += rand()%23-11;
            get<1>(individu[ind2]) += rand()%23-11;
            }
        // on fait gaffe à ne pas sortir de l'ecran..
        get<0>(individu[ind2]) = max(get<0>(individu[ind2]), roi_size/2);
        get<0>(individu[ind2]) = min(get<0>(individu[ind2]), bw.cols-roi_size/2);
        get<1>(individu[ind2]) = max(get<1>(individu[ind2]), roi_size/2);
        get<1>(individu[ind2]) = min(get<1>(individu[ind2]), bw.rows-roi_size/2);

        // recalcul du fitness de ind2
        int x=get<0>(individu[ind2]);
        int y=get<1>(individu[ind2]);
        Rect region_of_interest = Rect(x-roi_size/2, y-roi_size/2, roi_size, roi_size);
        Mat image_roi = bw(region_of_interest);
        get<2>(individu[ind2]) = countNonZero(image_roi);
        
        }
        
        int detect_x=bw.cols/2;
        int detect_y=bw.rows/2;
        int detect_fitness=-1;          
        
        for(int i=0; i<individu.size(); ++i)
            {
            int x = get<0>(individu[i]);
            int y = get<1>(individu[i]);
            rectangle( frame, Point(x-roi_size/2, y-roi_size/2 ), Point( x+roi_size/2, y+roi_size/2), Scalar( 255, 0, 0 ), +1, 4 );

            if (get<2>(individu[i])>detect_fitness)
                {
                detect_x=get<0>(individu[i]);
                detect_y=get<1>(individu[i]);
                detect_fitness=get<2>(individu[i]);
                }
            }
        

    rectangle( frame, Point(detect_x-roi_size/2, detect_y-roi_size/2 ), Point( detect_x+roi_size/2, detect_y+roi_size/2), Scalar( 255, 255, 0 ), +5, 4 );

    imshow("frame", frame);
    if(waitKey(30) >= 0) break;
    

    }

 
return 0;
}
