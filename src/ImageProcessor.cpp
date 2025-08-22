#include "ImageProcessor.h"
#include <iostream>
#include <algorithm>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"

ImageProcessor::ImageProcessor() : width(0), height(0), channels(0) {}
ImageProcessor::~ImageProcessor() {}

bool ImageProcessor::loadImage(const std::string& filename) {
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 4);
    if(!data) {
        std::cerr << "错误：无法读取图片！" << std::endl;
        return false;
    }
    // 将数据转化为Pixel格式
    pixels.clear();
    pixels.reserve(width * height);
    for(int i = 0; i < width * height * 4;i += 4) {
        pixels.emplace_back(data[i], data[i+1], data[i+2], data[i+3]);
    }
    stbi_image_free(data);
    channels = 4;    // 强制为RGBA格式
    std::cout << "加载图片：" << width << 'x' << height << "pixels" << std::endl;
    return true;
}

bool ImageProcessor::saveImage(const std::string& filename) const {
    if(pixels.empty()){
        std::cerr << "错误：没有可保存的图片数据！" << std::endl;
        return false;
    }
    std::vector<unsigned char> data;
    data.reserve(width * height * 4);
    for(const auto& pixel : pixels) {
        data.push_back(pixel.r);
        data.push_back(pixel.g);
        data.push_back(pixel.b);
        data.push_back(pixel.a);
    }
    // 根据文件扩展名选择保存格式
    std::string ext=filename.substr(filename.find_last_of('.'));
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    bool success = false;
    if(ext == ".png") {
        success = stbi_write_png(filename.c_str(), width, height, 4, data.data(), width * 4);
    } 
    else if(ext == ".jpg" || ext == ".jpeg") {
        success = stbi_write_jpg(filename.c_str(), width, height, 4, data.data(), 100);
    } 
    else if(ext == ".bmp") {
        success = stbi_write_bmp(filename.c_str(), width, height, 4, data.data());
    } 
    
    if(success) {
        std::cout << "保存图片：" << filename << " 成功！" << std::endl;
    } else {
        std::cerr << "错误：无法保存图片！" << std::endl;
    }
    return success;
}

void ImageProcessor::pixelate(int blockSize) {
    // 简单的像素化效果
    if(blockSize <= 1 || pixels.empty()) return;
    std::vector<Pixel> newPixels = pixels;

    // 遍历每个块
    for(int y = 0; y < height; y += blockSize) {
        for(int x = 0; x < width; x += blockSize) {
            // 计算块内像素平均颜色
            int totalR = 0, totalG = 0, totalB = 0, totalA = 0;
            int pixelCount = 0;
            int blockEndX = std::min(x + blockSize, width);
            int blockEndY = std::min(y + blockSize, height);
            for(int by = y; by < blockEndY; by++) {
                for(int bx = x; bx < blockEndX; bx++) {
                    Pixel p = getPixel(bx, by);
                    totalR += p.r;
                    totalG += p.g;
                    totalB += p.b;
                    totalA += p.a;
                    pixelCount++;
                }
            }
            if(pixelCount > 0) {
                Pixel avgPixel(totalR / pixelCount, totalG / pixelCount, totalB / pixelCount, totalA / pixelCount);
                // 将块内所有像素设置为平均颜色
                for(int by = y; by < blockEndY; by++) {
                    for(int bx = x; bx < blockEndX; bx++) {
                        int index = by * width + bx;
                        newPixels[index] = avgPixel;
                    }
                }
            }
        }
    }
    pixels = newPixels;
    std::cout << "应用像素化效果，块大小：" << blockSize << std::endl;
}

void ImageProcessor::pixelateWithPalette(int blockSize, const std::vector<Pixel>& palette) {
    // 简单的像素化并映射到调色板
    if(palette.empty() || blockSize <= 1) {
        pixelate(blockSize);
        return;
    }
    // 先进行普通的像素化
    pixelate(blockSize);
    // 然后将每个像素映射到最接近的调色
    for(auto& pixel : pixels) {
        int minDist = INT_MAX;
        Pixel closestColor = palette[0];
        for(const auto& paletteColor : palette) {
            int dr = pixel.r - paletteColor.r;
            int dg = pixel.g - paletteColor.g;
            int db = pixel.b - paletteColor.b;
            int dist = dr * dr + dg * dg + db * db;
            if(dist < minDist) {
                minDist = dist;
                closestColor = paletteColor;
            }
        }
        pixel = closestColor;
    }
    std::cout << "应用调色板像素化效果，块大小：" << blockSize << ", 调色板大小：" << palette.size() << std::endl;
}

Pixel ImageProcessor::getPixel(int x, int y) const {
    if(x >= 0 && x < width && y >= 0 && y < height) {
        return pixels[y * width + x];
    } else {
        std::cerr << "错误：获取像素时坐标越界！" << std::endl;
        return Pixel(); // 返回默认像素
    }
}

void ImageProcessor::setPixel(int x, int y, const Pixel& pixel) {
    if(x >= 0 && x < width && y >= 0 && y < height) {
        pixels[y * width + x] = pixel;
    } else {
        std::cerr << "错误：设置像素时坐标越界！" << std::endl;
    }
}
