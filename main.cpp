#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
using namespace std;

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

    int* getBox(int pixelX, int pixelY, int boxWidth, int boxHeight);
    int getMedian(int pixelX, int pixelY, int boxWidth, int boxHeight);

    int getPixel(int pixelX, int pixelY);
    void setPixel(int pixelX, int pixelY, int value);

    PixelType getPixelType(int pixelX, int pixelY);

    void ReadInputFile(ifstream& inputFile);
};

/*int loadBalancer(int workerThreads, int rows) {
}*/
int main(int argc, char** argv)
{
    int workerThreads = cstringToInt(argv[1]);
    int MatrixSize;

    Image inputImg(MatrixSize, MatrixSize);
    Image outputImg(MatrixSize, MatrixSize);
    
    ifstream inputFile("in.txt");
    if (!inputFile.is_open())
        cout << "Could not open the file" << endl;
    
    inputFile >> MatrixSize;
    inputImg.ReadInputFile(inputFile);

    return 0;
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

    return image[pixelY][pixelX];

};
int* Image::getBox(int pixelX, int pixelY, int boxWidth, int boxHeight) {
    int arrBox[9];
    pixelX--;
    pixelY--;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++) {
            if ((pixelX + j) >= boxWidth || (pixelY + i) >= boxHeight || (pixelX + j) < 0 || (pixelY + i) < 0)
                arrBox[i * 3 + j] = 0;
            else
                arrBox[i * 3 + j] = image[pixelY + i][pixelX + j];
        }
    }
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