#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

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
public:
    Image(int width, int height);
    ~Image();

    int *getBox(int pixelX, int pixelY, int boxWidth, int boxHeight);
    int getMedian(int size);
    
    int getPixel(int pixelX, int pixelY);
    void setPixel(int pixelX, int pixelY, int value);

    PixelType pixelType(int pixelX, int pixelY);

};

int loadBalancer(int workerThreads, int rows){
}
int main(int argc, char const *argv[])
{
    Image inputImg;
    Image outputImg;
    return 0;
}



Image::Image(int width, int height)
{
    for(int i = 0; i < height; i++){
        vector<int> row;
        for(int j = 0; j < width; j++){
            row.push_back(-1);
        }
        image.push_back(row);
    }
}

Image::~Image()
{
}
