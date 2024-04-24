#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    int argNum = 30;
    if(argc > 1) argNum = atoi(argv[1]);


    // Read file
    Mat im1 = imread("..\\..\\img\\vase1.jpg");
    Mat im2 = imread("..\\..\\img\\vase2.jpg");

    if (im1.empty())
    {
        cout << "First Image loading has failed" << endl;
        system("pause");
        return -1;
    }

    if (im2.empty())
    {
        cout << "Second Image loading has failed" << endl;
        system("pause");
        return -1;
    }

    Mat imConcat;
    hconcat(im1, im2, imConcat);

    // Display file
    cv::namedWindow("Image Display", WINDOW_AUTOSIZE);
    cv::imshow("Image Display", imConcat);

    waitKey(0); // wait for a key in the window


    // Creation of feature vector and matrix

    vector<KeyPoint> keypoints1;
    vector<KeyPoint> keypoints2;
    Mat descriptors1;
    Mat descriptors2;
    Mat imFeatPt1;
    Mat imFeatPt2;
    Mat imFeatConcat;

    // Creation of the matcher
    BFMatcher matcher(NORM_HAMMING);
    std::vector<DMatch> matches;
    Mat img_matches;



// // // SIFT // // //
    Ptr<SIFT> sift = SIFT::create(argNum);
    sift->detectAndCompute(im1, noArray(), keypoints1, descriptors1);
    sift->detectAndCompute(im2, noArray(), keypoints2, descriptors2);

    drawKeypoints(im1, keypoints1, imFeatPt1);
    drawKeypoints(im2, keypoints2, imFeatPt2);

    hconcat(imFeatPt1, imFeatPt2, imFeatConcat);
    cv::imshow("Image with keypoints - SIFT", imFeatConcat);
    waitKey(0);
    // Comments : more suitable to take perspective into consideration :
    // but here we care more about local points

    // We can't apply the matcher function on the SIFT case



// // // AKAZE // // //
    Ptr<AKAZE> akaze = AKAZE::create();
    akaze->setMaxPoints(argNum);
    akaze->detectAndCompute(im1, noArray(), keypoints1, descriptors1);
    akaze->detectAndCompute(im2, noArray(), keypoints2, descriptors2);

    drawKeypoints(im1, keypoints1, imFeatPt1);
    drawKeypoints(im2, keypoints2, imFeatPt2);

    hconcat(imFeatPt1, imFeatPt2, imFeatConcat);
    cv::imshow("Image with keypoints - AKAZE", imFeatConcat);
    waitKey(0);

    // Match of AKAZE
    matcher.match(descriptors1, descriptors2, matches);

    std::sort(matches.begin(), matches.end());
    
    cv::drawMatches(im1, keypoints1, im2, keypoints2, matches, img_matches);

    cv::imshow("Image with keypoints - AKAZE", img_matches);
    waitKey(0);

    // Comments : more suitable for change of scaling, but not really the case here



// // // ORB // // //
    Ptr<ORB> orb = ORB::create(argNum);
    orb->detectAndCompute(im1, noArray(), keypoints1, descriptors1);
    orb->detectAndCompute(im2, noArray(), keypoints2, descriptors2);

    drawKeypoints(im1, keypoints1, imFeatPt1);
    drawKeypoints(im2, keypoints2, imFeatPt2);

    hconcat(imFeatPt1, imFeatPt2, imFeatConcat);
    cv::imshow("Image with keypoints - ORB", imFeatConcat);
    waitKey(0);

    // Match of ORB
    matcher.match(descriptors1, descriptors2, matches);

    std::sort(matches.begin(), matches.end());
    
    cv::drawMatches(im1, keypoints1, im2, keypoints2, matches, img_matches);

    cv::imshow("Image with keypoints - ORB", img_matches);
    waitKey(0);

    // Comments : most suitable --> focuses more on the vase, and higher speed


    // Destroy
    im1.release();
    im2.release();
    imConcat.release();

    descriptors1.release();
    descriptors2.release();
    imFeatPt1.release();
    imFeatPt2.release();
    imFeatConcat.release();

    img_matches.release();

    return 0;
}