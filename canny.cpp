#include "canny.h"


canny::canny(String filename){

    sourceImage = imread(filename, 0);

    Mat grayscaled = sourceImage;
    Mat gFiltered = gaussianBlur(grayscaled);
    Mat sFiltered = sobel(gFiltered);
    Mat non = NonMaxSupp(sFiltered);

    namedWindow("GrayScaled");
    namedWindow("Gaussian Blur");
    namedWindow("Sobel Filtered");
    namedWindow("Edge Thining");

    imshow("GrayScaled", grayscaled);
    imshow("Gaussian Blur", gaussianBlur(grayscaled));
    imshow("Sobel Filtered", sobel(gFiltered));
    imshow("Edge Thining", NonMaxSupp(sFiltered));


    waitKey(0);
}

Mat canny::gaussianBlur(Mat grayscaled)
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

Mat canny::sobel(Mat gFiltered)
{
    int size = 1;

    double xFilter[3][3] = {
        -1.0, 0, 1.0,
        -2.0, 0, 2.0,
        -1.0, 0, 1.0,
    };

    double yFilter[3][3] = {
        -1.0, -2.0, -1.0,
        0, 0, 0,
        1.0, 2.0, 1.0,
    };

    Mat filteredImg = Mat(gFiltered.rows, gFiltered.cols, CV_8UC1);
    angles = Mat(gFiltered.rows, gFiltered.cols, CV_32FC1);

    for (int i = size; i < gFiltered.rows; i++)
    {
        for (int j = size; j < gFiltered.cols; j++)
        {
            double sumx = 0;
            double sumy = 0;

            for (int x = 0; x < 3; x++)
                for (int y = 0; y < 3; y++)
                {
                    sumx += xFilter[x][y] * gFiltered.at<uchar>(i + x, j + y);
                    sumy += yFilter[x][y] * gFiltered.at<uchar>(i + x, j + y);
                }

            double sumxsq = sumx * sumx;
            double sumysq = sumy * sumy;

            double sq2 = sqrt(sumxsq + sumysq);

            if (sq2 > 255)
            {
                sq2 = 255;
            }

            filteredImg.at<uchar>(i, j) = sq2;

            angles.at<float>(i, j) = atan(sumy / sumx);
        }
    }

    return filteredImg;
}


Mat canny::NonMaxSupp(Mat sFiltered)
{

    Mat nonMaxSupped = Mat(sFiltered.rows - 2, sFiltered.cols - 2, CV_8UC1);

    for (int i = 1; i < (sFiltered.rows - 1); i++)
    {
        for (int j = 1; j < (sFiltered.cols - 1); j++)
        {
            float Tangent = angles.at<float>(i, j);

            nonMaxSupped.at<uchar>(i - 1, j - 1) = sFiltered.at<uchar>(i, j);

            // Horizontal Edge
            if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i, j + 1)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i, j - 1)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }

            // Vertical Edge
            if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i + 1, j)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i - 1, j)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }

            //-45 Degree Edge
            if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i - 1, j + 1)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i + 1, j - 1)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }

            // 45 Degree Edge
            if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i + 1, j + 1)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i - 1, j - 1)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }
        }
    }
    return nonMaxSupped;
}