#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void printMatrixDimensions(const Mat &matrix) {
    cout << "Rows: " << matrix.rows << endl;
    cout << "Cols: " << matrix.cols << endl;
    cout << "Channels: " << matrix.channels() << endl;
}

Mat openImage(string &path){
    Mat image = imread(path);
    if(image.empty()){
        cout<<"Image file can't be opened or is empty";
        exit(EXIT_FAILURE);
    }
    return image;
}
void toGrayScale(Mat &image,Mat& grayScale){
    cvtColor(image,grayScale,COLOR_RGB2GRAY);
}

Mat createEnergyMatrix(Mat &image){
    Mat grayScale; toGrayScale(image,grayScale);
    Mat gradX,gradY;
    Sobel(grayScale,gradX,CV_32F,1,0,3);
    Sobel(grayScale,gradY,CV_32F,0,1,3);

    Mat energyMatrix;
    magnitude(gradX,gradY,energyMatrix);
    

    // convert to 8-bit matrix for visulizing only
    // Mat outEnergyMatrix;energyMatrix.convertTo(outEnergyMatrix,CV_8U);
    // imshow("image",outEnergyMatrix);
    // waitKey(0);


    return energyMatrix;
}
Mat findSeam(Mat &energyMat){
    int rows = energyMat.rows;
    int cols = energyMat.cols;

    Mat path(rows, cols, CV_32S, Scalar(-1));
    Mat dp = Mat::zeros(rows, cols, CV_32F);
    for (int j = 0; j < cols; ++j) {
        dp.at<float>(0, j) = energyMat.at<float>(0, j);
    }

    //calculating dp
    for(int i = 1 ; i < rows ; i++){
        for(int j = 0 ; j < cols ; j++){
            
            dp.at<float>(i,j) = dp.at<float>(i-1,j);
            path.at<int>(i,j) = j;
            if(j>0 && dp.at<float>(i-1,j-1) < dp.at<float>(i,j)){
                dp.at<float>(i,j) = dp.at<float>(i-1,j-1);
                path.at<int>(i,j) = j-1;
            }
            if(j<cols-1 && dp.at<float>(i-1,j+1) < dp.at<float>(i,j)){
                dp.at<float>(i,j) = dp.at<float>(i-1,j+1);
                path.at<int>(i,j) = j+1;
            }

            dp.at<float>(i,j)+=energyMat.at<float>(i,j);
        }
    }
    // calc min element
    int minCol = 0;
    float minEnergy = dp.at<float>(rows - 1, 0);
    for (int j = 1; j < cols; ++j) {
        if (dp.at<float>(rows - 1, j) <= minEnergy) {
            minEnergy = dp.at<float>(rows - 1, j);
            minCol = j;
        }
    }

    // backtrack to get the path
    Mat seamPath(rows, 1, CV_32S);
    for (int i = rows - 1; i >= 0; --i) {
        seamPath.at<int>(i, 0) = minCol;
        minCol = path.at<int>(i, minCol);
    }
    
    return seamPath;

}
void removeSeam(Mat &seam, Mat &image, Mat &newImage) {
    int rows = image.rows;
    int cols = image.cols;

    newImage = Mat(rows, cols - 1, image.type());

    for (int i = 0; i < rows; ++i) {
        int seamCol = seam.at<int>(i, 0);

        //setting seam color to red for visulization
        image.at<Vec3b>(i, seamCol) = Vec3b(0, 0, 255);

        for (int j = 0; j < seamCol; ++j) {
            newImage.at<Vec3b>(i, j) = image.at<Vec3b>(i, j);
        }
        for (int j = seamCol + 1; j < cols; ++j) {
            newImage.at<Vec3b>(i, j - 1) = image.at<Vec3b>(i, j);
        }
    }
}


int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Input should be of format <executable> <image_path>";
        return 0;
    }

    namedWindow("image", WINDOW_NORMAL);
    string image_path = argv[1];

    auto image = openImage(image_path);

    cout << "Current image size: " << image.cols << "x" << image.rows << endl;

    int targetWidth, targetHeight;
    cout << "Enter the target width: ";
    cin >> targetWidth;
    cout << "Enter the target height: ";
    cin >> targetHeight;

    int currentWidth = image.cols;
    int currentHeight = image.rows;

    while (currentWidth > targetWidth) {
        auto energyImage = createEnergyMatrix(image);
        auto seam = findSeam(energyImage);
        Mat newImage;
        removeSeam(seam, image, newImage);
        
        // for visuliaztion only
        imshow("image",image);
        waitKey(0);
        imshow("image",newImage);
        waitKey(0);
        
        image = newImage;
        currentWidth = image.cols;
    }

    transpose(image,image);
    while (currentHeight > targetHeight) {
        auto energyImage = createEnergyMatrix(image);
        auto seam = findSeam(energyImage);
        Mat newImage;
        removeSeam(seam, image, newImage);
        
        // for visuliaztion only
        imshow("image",image);
        waitKey(0);
        imshow("image",newImage);
        waitKey(0);
        
        image = newImage;
        currentHeight = image.cols;
    }
    transpose(image,image);

    // For visulizing only
    imshow("image", image);
    waitKey(0);
    destroyAllWindows();
    imwrite("out_image.jpg",image);

    return 0;
}
