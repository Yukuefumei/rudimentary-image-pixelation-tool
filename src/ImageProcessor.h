#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <string>
#include <vector>
#include <memory>

struct Pixel
{
    unsigned char r, g, b, a;
    Pixel() : r(0), g(0), b(0), a(255) {}
    Pixel(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
};
class ImageProcessor
{
    std::vector<Pixel> pixels;
    int width,height,channels;
public:
    ImageProcessor();
    ~ImageProcessor();
    
    // 基础功能
    bool loadImage(const std::string& filename);
    bool saveImage(const std::string& filename) const;
    
    // 像素化效果
    void pixelate(int blockSize);
    void pixelateWithDithering(int blockSize);
    void pixelateWithPalette(int blockSize, const std::vector<Pixel>& palette);
    
    // 工具函数
    void resize(int newWidth, int newHeight);   // 最近邻插值法
    Pixel getPixel(int x, int y) const;     // 
    void setPixel(int x, int y, const Pixel& pixel);
    
    // 获取图片信息
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
};
#endif // IMAGE_PROCESSOR_H