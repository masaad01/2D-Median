#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <pthread.h>
#include <mutex>

using namespace std;

/*******************************
    Class definitions
*******************************/

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

    void readInputFile(ifstream &inputFile);
    void writeOutputFile(ofstream &outputFile);
};

struct ThreadInfo
{
    int id;
    Image *inputImg;
    Image *outputImg;
    int startPixel;
    int endPixel;
};

/*******************************
    Function declarations
*******************************/

int cstringToInt(char *cstring);
void *processPixels(void *Info);
void loadBalancing(int endPixelsArr[], int startPixelsArr[], int workerThreads, int totalPixels);
void loadblancing(int *start, int *end, int workThreads, int pixelSize, int index);
void processImage(Image &inputImg, Image &outputImg, int workerThreads);

/*******************************
    Global variables
*******************************/

mutex brightLock, darkLock, normalLock;
int totalBright = 0;
int totalDark = 0;
int totalNormal = 0;

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

    inputImg.readInputFile(inputFile);
    processImage(inputImg, outputImg, workerThreads);
    outputImg.writeOutputFile(outputFile);
    cout << "Main: numOfBright=" << totalBright << ", numOfDark=" << totalDark << ", numOfNormal=" << totalNormal << endl;
    return 0;
}

/*******************************
    Main Function definitions
*******************************/

void *processPixels(void *Info)
{
    ThreadInfo *info = (ThreadInfo *)Info;

    int id = info->id;
    Image *inputImg = info->inputImg;
    Image *outputImg = info->outputImg;
    int startPixel = info->startPixel;
    int endPixel = info->endPixel;

    int bright = 0;
    int dark = 0;
    int normal = 0;
    printf("ThreadID=%d, startRow=%d, startCol=%d, endRow=%d, endCol=%d\n", id, startPixel / inputImg->getWidth(), startPixel % inputImg->getWidth(), (endPixel - 1) / inputImg->getWidth(), (endPixel - 1) % inputImg->getWidth());

    for (int i = startPixel; i < endPixel; i++)
    {
        int y = i / inputImg->getWidth(); // row
        int x = i % inputImg->getWidth(); // col

        int newValue = inputImg->getMedian(x, y, 3, 3);

        outputImg->setPixel(x, y, newValue);

        switch (outputImg->getPixelType(x, y))
        {
        case PixelType::Bright:
            bright++;
            break;
        case PixelType::Dark:
            dark++;
            break;
        case PixelType::Normal:
            normal++;
            break;
        }
    }
    printf("ThreadID=%d, numofBright=%d,numOfDark=%d, numofNormal=%d\n", id, bright, dark, normal);

    brightLock.lock();
    totalBright += bright;
    brightLock.unlock();

    darkLock.lock();
    totalDark += dark;
    darkLock.unlock();

    normalLock.lock();
    totalNormal += normal;
    normalLock.unlock();

    pthread_exit(NULL);
}

void loadBalancing(int endPixelsArr[], int startPixelsArr[], int workerThreads, int totalPixels)
{
    int range = totalPixels / workerThreads;
    int remainder = totalPixels % workerThreads;

    endPixelsArr[0] = range;
    startPixelsArr[0] = 0;
    for (int i = 1; i < workerThreads; i++)
    {
        endPixelsArr[i] = range + endPixelsArr[i - 1];
        startPixelsArr[i] = endPixelsArr[i - 1];
        if (remainder > 0)
        {
            endPixelsArr[i]++;
            remainder--;
        }
    }
}

void processImage(Image &inputImg, Image &outputImg, int workerThreads)
{
    pthread_t *threads = new pthread_t[workerThreads];
    int *startPixel = new int[workerThreads];
    int *endPixel = new int[workerThreads];
    ThreadInfo *Info = new ThreadInfo[workerThreads];
    loadBalancing(endPixel, startPixel, workerThreads, (inputImg.getWidth() * inputImg.getHeight()));
    /*int start;
    int end;*/

    for (int i = 0; i < workerThreads; i++)
    {
        /*loadblancing(&start,&end,workerThreads, inputImg.getWidth() * inputImg.getHeight(), i);
        printf("start=%d end=%d  %d\n",start,end,i);*/
        // if (startPixel[i] == endPixel[i])
        // {
        //     continue;
        // }
        Info[i].id = i;
        Info[i].inputImg = &inputImg;
        Info[i].outputImg = &outputImg;
        Info[i].startPixel = startPixel[i];
        Info[i].endPixel = endPixel[i];

        pthread_create(&threads[i], NULL, processPixels, (void *)&Info[i]);
    }

    for (int i = 0; i < workerThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

/*******************************
    Image class implementation
*******************************/

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

void Image::readInputFile(ifstream &inputFile)
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

void Image::writeOutputFile(ofstream &outputFile)
{
    outputFile << "The output image is" << endl;
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
    int med = s / 2;
    int *arr = new int[s];
    getBox(arr, pixelX, pixelY, boxWidth, boxHeight);
    sort(arr, arr + s);
    return arr[med];
}

/*******************************
    Utility functions
*******************************/

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
