#include "warp_perspective.h"

using namespace cv;
using namespace std;

int Warp_Perspective::Col1=153;
int Warp_Perspective::Col2=344;//339
int Warp_Perspective::Col3=133;
int Warp_Perspective::Col4=361;
int Warp_Perspective::Row1=184;
int Warp_Perspective::Row2=200;

void Warp_Perspective::getWarp_Perspective(Mat *src,Mat *dst)
{
    Mat srcImage = *src;

    //  topLeft, topRight, bottomRight, bottomLeft
    //vector<Point> not_a_rect_shape;
    not_a_rect_shape.push_back(Point(Col1,Row1));
    not_a_rect_shape.push_back(Point(Col2,Row1));
    not_a_rect_shape.push_back(Point(Col3,Row2));
    not_a_rect_shape.push_back(Point(Col4,Row2));

    //Point2f src_vertices[4];
    src_vertices[0] = not_a_rect_shape[0];
    src_vertices[1] = not_a_rect_shape[1];
    src_vertices[2] = not_a_rect_shape[2];
    src_vertices[3] = not_a_rect_shape[3];

    //Point2f dst_vertices[4];
    dst_vertices[0] = Point(Col1,Row1);
    dst_vertices[1] = Point(Col2,Row1);
    dst_vertices[2] = Point(Col1,Row2);
    dst_vertices[3] = Point(Col2,Row2);

    Mat warpMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
    Mat rotated;
    warpPerspective(srcImage, rotated, warpMatrix, rotated.size(), INTER_LINEAR, BORDER_CONSTANT);

    //imshow("rotated", rotated);

    *dst = rotated;
}
