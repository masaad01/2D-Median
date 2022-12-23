#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
using namespace std;


// class declarations
enum class PixelType
{
    Dark,
    Bright,
    Normal
};
class Image
{
private:
    vector<vector<int>> image;
    int width;
    int height;

public:
    Image(int width, int height);
    ~Image();

    int getHeight() { return height; }
    int getWidth() { return width; }

    int* getBox(int pixelX, int pixelY, int boxWidth, int boxHeight);
    int getMedian(int pixelX, int pixelY, int boxWidth, int boxHeight);

    int getPixel(int pixelX, int pixelY);
    void setPixel(int pixelX, int pixelY, int value);

    PixelType getPixelType(int pixelX, int pixelY);

    void ReadInputFile(ifstream& inputFile);
    void WriteOutputFile(ofstream& outputFile);
};

// function declarations
int cstringToInt(char* cstring);
void processImage(Image& inputImg, Image& outputImg, int workerThreads);

int main(int argc, char** argv)
{
    int workerThreads = 1;
    if (argc == 2)
        workerThreads = cstringToInt(argv[1]);
    
    ifstream inputFile("in.txt");
    ofstream outputFile("out.txt");

    if (!inputFile.is_open()){
        cout << "Could not open in.txt" << endl;
        return 1;
    }
    if (!outputFile.is_open()){
        cout << "Could not open out.txt" << endl;
        return 1;
    }

    int MatrixSize;
    inputFile >> MatrixSize;

    Image inputImg(MatrixSize, MatrixSize);
    Image outputImg(MatrixSize, MatrixSize);

    inputImg.ReadInputFile(inputFile);

    processImage(inputImg, outputImg, workerThreads);

    outputImg.WriteOutputFile(outputFile);

    return 0;
}

// function definitions
void processImage(Image& inputImg, Image& outputImg, int workerThreads)
{
    int height = inputImg.getHeight();
    int width = inputImg.getWidth();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // just copy the input image to the output image for now (for testing only)
            int pixel = inputImg.getPixel(j, i);
            outputImg.setPixel(j, i, pixel);
        }
    }
}

// class functions
Image::Image(int width, int height)
{
    this->width = width;
    this->height = height;
    for (int i = 0; i < height; i++) {
        vector<int> row;

        for (int j = 0; j < width; j++) {

            row.push_back(-1);
        }
        image.push_back(row);
    }
}

Image::~Image()
{
}
void Image::setPixel(int pixelX, int pixelY, int value) {

    image[pixelY][pixelX] = value;


};
int Image::getPixel(int pixelX, int pixelY) {
    if(pixelX < 0 || pixelY < 0 || pixelX >= width || pixelY >= height)
        return 0;
    return image[pixelY][pixelX];
};
int* Image::getBox(int pixelX, int pixelY, int boxWidth, int boxHeight) {
    int *arrBox = new int[boxWidth * boxHeight];
    for(int i = 0; i < boxHeight; i++)
        for (int j = 0; j < boxWidth; j++)
            arrBox[i * boxWidth + j] = getPixel(pixelX + j, pixelY + i);
    // pixelX--;
    // pixelY--;
    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < 3; j++) {
    //         if ((pixelX + j) >= boxWidth || (pixelY + i) >= boxHeight || (pixelX + j) < 0 || (pixelY + i) < 0)
    //             arrBox[i * 3 + j] = 0;
    //         else
    //             arrBox[i * 3 + j] = image[pixelY + i][pixelX + j];
    //     }
    // }
    return arrBox;
};
PixelType Image::getPixelType(int pixelX, int pixelY){
    int value = getPixel(pixelX, pixelY);
    if (value < 50)
        return PixelType::Dark;
    else if (value > 200)
        return PixelType::Bright;
    else
        return PixelType::Normal;
}
void Image::ReadInputFile(ifstream& inputFile)
{
    int value;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            inputFile >> value;
            setPixel(j, i, value);
        }
    }
};
void Image::WriteOutputFile(ofstream& outputFile)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            outputFile << getPixel(j, i) << " ";
        }
        outputFile << endl;
    }
};
// utility functions

int cstringToInt(char* cstring)
{
    int result = 0;
    int i = 0;
    while (cstring[i] != '\0')
    {
        result = result * 10 + (cstring[i] - '0');
        i++;
    }
    return result;
}


int Image::getMedian(int pixelX, int pixelY, int boxWidth, int boxHeight )
{
 
 int s = boxHeight*boxWidth ; 
 int med = s/2 +1 ; 
 int *p = getBox(int pixelX, int pixelY, int boxWidth, int boxHeight) ; 
 int arr[s] ; 
 for (int i =0 ; i <s ; i++ )
 {
    arr[i]=*(p);
    p++ ; 
 }
 sort(arr , arr +s ) ; 
 return arr[med] ; 
}
