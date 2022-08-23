#ifndef _CANNY_
#define _CANNY_
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

class Canny
{
    private:
        Mat sourceImage;

    public:
        Canny(String);
        Mat gaussianBlur(Mat);

};

#endif
