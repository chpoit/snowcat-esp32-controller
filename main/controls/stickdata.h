#ifndef STICKDATA_H
#define STICKDATA_H

class StickData {
   public:
    StickData();
    StickData(int minY, int maxY, int minX, int maxX, int minRY, int maxRY, int minRX, int maxRX);
    int minY = -512;
    int maxY = 511;
    int minX = -512;
    int maxX = 511;
    int minRY = -512;
    int maxRY = 511;
    int minRX = -512;
    int maxRX = 511;

    static StickData Zeroed();
};

#endif