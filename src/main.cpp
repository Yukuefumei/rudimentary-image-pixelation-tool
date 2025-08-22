#include <iostream>
#include <string>
#include <vector>
#include "ImageProcessor.h"

// 预定义调色盘
std::vector<Pixel> createRetroGamePalette() {
    return {
        Pixel(0, 0, 0),         // 黑色
        Pixel(255, 255, 255),   // 白色
        Pixel(255, 0, 0),       // 红色
        Pixel(0, 255, 0),       // 绿色
        Pixel(0, 0, 255),       // 蓝色
        Pixel(255, 255, 0),     // 黄色
        Pixel(255, 0, 255),     // 品红
        Pixel(0, 255, 255),     // 青色
        Pixel(128, 128, 128),   // 灰色
        Pixel(128, 0, 0),       // 深红
        Pixel(0, 128, 0),       // 深绿
        Pixel(0, 0, 128),       // 深蓝
        Pixel(128, 128, 0),     // 橄榄色
        Pixel(128, 0, 128),     // 紫色
        Pixel(0, 128, 128),     // 青绿色
        Pixel(192, 192, 192)    // 浅灰色
    };
}

void printUsage() {
    std::cout << "用法：pixel_art_converter <输入图片> <输出文件> [设置]\n"
              << "设置：\n"
              << " -b, --block-size <块大小> 像素化块大小（默认 8）\n"
              << " -p, --palette 使用预定义调色盘（复古游戏风格）\n"
              << " -h, --help 显示帮助信息\n"
              << "示例：\n"
              << " pixel_art_converter input.png output.png -b 16 -p\n";
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printUsage();
        return 1;
    }
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    int blockSize = 8;
    bool usePalette = false;

    for(int i = 3; i < argc; i++) {
        std::string arg = argv[i];
        if(arg == "-b" || arg == "--block-size") {
            if(i + 1 < argc) {
                blockSize = std::stoi(argv[i + 1]);
                i++;
            }
        }
        else if(arg == "-p" || arg == "--palette") {
            usePalette = true;
        }
        else if(arg == "-h" || arg == "--help") {
            printUsage();
            return 0;
        }
    }

    ImageProcessor processor;
    if(!processor.loadImage(inputFile)) {
        return 1;
    }

    if(usePalette) {
        auto palette = createRetroGamePalette();
        processor.pixelateWithPalette(blockSize, palette);
    }
    else {
        processor.pixelate(blockSize);
    }

    if(!processor.saveImage(outputFile)) {
        return 1;
    }

    std::cout << "转换完成！" << std::endl;
    return 0;
}