#ifndef _CANNY_
#define _CANNY_
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class canny
{
    public:
        canny(String);
        Mat gaussianBlur(Mat);
        Mat sobel(Mat ); 

    private:
        Mat sourceImage;
        Mat angles;

};

#endif
