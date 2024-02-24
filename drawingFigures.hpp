#include <iostream>
#include <fstream>
#include <vector>

void drawCircle(std::vector<uint8_t>& pixels, int centerX, int centerY, int radius, int width, int height) {
    for (int y = centerY - radius; y <= centerY + radius; ++y) {
        for (int x = centerX - radius; x <= centerX + radius; ++x) {
            if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius) {
                if (x >= 0 && y >= 0 && x < width && y < height) {
                    int index = y * width + x;
                    pixels[index] = 0; // R
                    pixels[index + 1] = 0; // G
                    pixels[index + 2] = 0; // B
                }
            }
        }
    }
}

void drawLine(std::vector<uint8_t>& pixels, int x1, int y1, int x2, int y2, int width, int height) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while(true) {
        int index = y1 * width + x1;
        if (x1 >= 0 && y1 >= 0 && x1 < width && y1 < height) {
            pixels[index] = 0; // R
            pixels[index + 1] = 0; // G
            pixels[index + 2] = 0; // B
        }

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}