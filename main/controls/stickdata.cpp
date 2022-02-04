#include "stickdata.h"

StickData::StickData() {}

StickData::StickData(int minY, int maxY, int minX, int maxX, int minRY, int maxRY, int minRX, int maxRX) {
    this->minY = minY;
    this->maxY = maxY;
    this->minX = minX;
    this->maxX = maxX;
    this->minRY = minRY;
    this->maxRY = maxRY;
    this->minRX = minRX;
    this->maxRX = maxRX;
}

StickData StickData::Zeroed() {
    return StickData(0, 0, 0, 0, 0, 0, 0, 0);
}