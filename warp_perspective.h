#ifndef WARP_PERSPECTIVE_H
#define WARP_PERSPECTIVE_H

#include <test.h>
class Warp_Perspective
{
public:
    cv::Mat *dst;
    void getWarp_Perspective(cv::Mat *src,cv::Mat *dst);
private:
    cv::Mat *src;
    std::vector<cv::Point> not_a_rect_shape;
    cv::Point2f src_vertices[4];
    cv::Point2f dst_vertices[4];
    static int Col1;
    static int Col2;
    static int Col3;
    static int Col4;
    static int Row1;
    static int Row2;
};

#endif // WARP_PERSPECTIVE_H
