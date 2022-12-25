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

    void getBox(int arrBox[], int pixelX, int pixelY, int boxWidth, int boxHeight);
    int getMedian(int pixelX, int pixelY, int boxWidth, int boxHeight);

    int getPixel(int pixelX, int pixelY);
    void setPixel(int pixelX, int pixelY, int value);

    PixelType getPixelType(int pixelX, int pixelY);

    void ReadInputFile(ifstream &inputFile);
    void WriteOutputFile(ofstream &outputFile);
};

// function declarations
int cstringToInt(char *cstring);
void processImage(Image &inputImg, Image &outputImg, int workerThreads);

int main(int argc, char **argv)
{
    int workerThreads = 1;
    if (argc == 2)
        workerThreads = cstringToInt(argv[1]);

    ifstream inputFile("in.txt");
    ofstream outputFile("out.txt");

    if (!inputFile.is_open())
    {
        cout << "Could not open in.txt" << endl;
        return 1;
    }
    if (!outputFile.is_open())
    {
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
void processPixels(int id, Image &inputImg, Image &outputImg, int startPixel, int endPixel)
{
    int bright = 0;
    int dark = 0;
    int normal = 0;

    cout <<"ThreadID="<<id<<", startRow="<<startPixel / inputImg.getWidth()<<", startCol="<<startPixel % inputImg.getWidth()
        <<", endRow="<<endPixel / inputImg.getWidth()<<", endCol="<<endPixel % inputImg.getWidth()<<endl;

    for (int i = startPixel; i < endPixel;i++)
    {
        int y = i / inputImg.getWidth(); // row
        int x = i % inputImg.getWidth(); // col
        
        int newValue = inputImg.getMedian(x, y, 3, 3);
        outputImg.setPixel(x, y, newValue);

        switch (inputImg.getPixelType(x, y))
        {
        case PixelType::Bright :
            bright++;
            break;
        case PixelType::Dark :
            dark++;
            break;
        case PixelType::Normal :
            normal++;
            break;
        }
    }

    cout << "ThreadID=" << id << ", numOfBright=" << bright << ", numOfDark=" << dark << ", numOfNormal=" << normal << endl;
}

void processImage(Image &inputImg, Image &outputImg, int workerThreads)
{
    int height = inputImg.getHeight();
    int width = inputImg.getWidth();

    int totalPixels = height * width;

    for(int i = 0; i < totalPixels; i++)
    {
        int startPixel;
        int endPixel;

    }
    // processPixels(0, inputImg, outputImg, 0, totalPixels - 1);
}

int loadblancing(int workThreads, int pixelSize, int index)
{
    int arr[workThreads+1] ; 
    int start , end ; 
    float r = pixelSize / workThreads;
    int ch = pixelSize % workThreads;
    if (arr[1] == 0)
    {
        int arr[workThreads];
        arr[0] = 0;


        if (!ch)
        {
            for (int i = 1; i <= workThreads; i++)
            {
                arr[i] = arr[i - 1] + r;
            }
            if (index == 0)
            {
                return start = 0, end = r;
            }
            int start = arr[index - 1] + 1, end = arr[index];
            return start, end;
        }
        else
        {

            for (int i = 1; i <= ch; i++)
                arr[i] += 1;
        }
    }
    else
    {
        if (index == 0)
        {
            return start = 0, end = r;
        }
         start = arr[index - 1] + 1 , end = arr[index];
        return start, end;
    }
}



// class functions
Image::Image(int width, int height)
{
    this->width = width;
    this->height = height;
    for (int i = 0; i < height; i++)
    {
        vector<int> row;

        for (int j = 0; j < width; j++)
        {

            row.push_back(-1);
        }
        image.push_back(row);
    }
}

Image::~Image()
{
}
void Image::setPixel(int pixelX, int pixelY, int value)
{

    image[pixelY][pixelX] = value;
};
int Image::getPixel(int pixelX, int pixelY)
{
    if (pixelX < 0 || pixelY < 0 || pixelX >= width || pixelY >= height)
        return 0;
    return image[pixelY][pixelX];
};
void Image::getBox(int arrBox[], int pixelX, int pixelY, int boxWidth, int boxHeight)
{
    pixelX--;
    pixelY--;
    for (int i = 0; i < boxHeight; i++)
    {
        for (int j = 0; j < boxWidth; j++)
        {
            if ((pixelX + j) >= this->width || (pixelY + i) >= this->height || (pixelX + j) < 0 || (pixelY + i) < 0)
                arrBox[i * boxWidth + j] = 0;
            else
                arrBox[i * boxWidth + j] = image[pixelY + i][pixelX + j];
        }
    }
};
PixelType Image::getPixelType(int pixelX, int pixelY)
{
    int value = getPixel(pixelX, pixelY);
    if (value < 50)
        return PixelType::Dark;
    else if (value > 200)
        return PixelType::Bright;
    else
        return PixelType::Normal;
}
void Image::ReadInputFile(ifstream &inputFile)
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
void Image::WriteOutputFile(ofstream &outputFile)
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
int Image::getMedian(int pixelX, int pixelY, int boxWidth, int boxHeight)
{

    int s = boxHeight * boxWidth;
    int med = s / 2 + 1;
    int arr[s];
    getBox(arr, pixelX, pixelY, boxWidth, boxHeight);
    sort(arr, arr + s);
    return arr[med];
}

// utility functions

int cstringToInt(char *cstring)
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
