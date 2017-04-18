#ifndef PROJECTION_H
#define PROJECTION_H
#include <test.h>

class Projection
{
public:
    cv::Mat *src;
    cv::Mat *dst;
    void projection(cv::Mat *src,cv::Mat *dst);
private:
    int value;
};

#endif // PROJECTION_H
