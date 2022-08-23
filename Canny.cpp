#include "Canny.h"


Canny::Canny(String filename){

    sourceImage = imread(filename, 0);

    Mat grayscaled = sourceImage;
    Mat gFiltered = gaussianBlur(grayscaled);

    namedWindow("GrayScaled");
    namedWindow("Gaussian Blur");

    imshow("GrayScaled", grayscaled);
    imshow("Gaussian Blur", gaussianBlur(grayscaled));

    waitKey(0);
}

Mat Canny::gaussianBlur(Mat grayscaled)
{
    Mat Gaussiankernel = Mat(Size(3, 3), CV_8UC1);

    double sigma = 3;
    double e = 2.7182818;
    int size = 3;
    double sum_g = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Gaussiankernel.at<uchar>(i, j) = (1 / (M_PI)*pow(sigma, 2)) - ((pow(i, 2) + pow(j, 2))) / (2 * pow(sigma, 2));
            sum_g += Gaussiankernel.at<uchar>(i, j);
        }
    }

    Mat blurImage = Mat((sourceImage.rows - size + 1), (sourceImage.cols - size + 1), sourceImage.type());

    int dx = size / 2;
    int dy = size / 2;
    int sumIndex;

    for (int i = 0; i < sourceImage.rows; i++)
    {
        for (int j = 0; j < sourceImage.cols; j++)
        {
            sumIndex = 0;
            for (int k = 0; k < size; k++)
            {
                for (int l = 0; l < size; l++)
                {
                    int x = j - dx + l;
                    int y = i - dy + k;

                    if (x >= 0 && x < blurImage.cols && y >= 0 && y < blurImage.rows)
                    {
                        sumIndex += (Gaussiankernel.at<uchar>(k, l) * sourceImage.at<uchar>(y, x)) / sum_g;
                    }
                }
            }
            blurImage.at<uchar>(i, j) = saturate_cast<uchar>(sumIndex);
        }
    }
    return blurImage;
}