#ifndef FINE_CONTOURS_H
#define FINE_CONTOURS_H
#include <test.h>

class Detect_Line
{
public:
    cv::Mat *lines_image;
    cv::Mat *src_image;
    void get_contours(cv::Mat *lines_image,cv::Mat *src_image);
    enum line{ yellow_solod=0, yellow_dotted, white_solid, white_dotted };
private:
    cv::Mat m_src;
    cv::Mat m_dst;
    cv::Mat m_HSV;
    std::vector<cv::Mat> m_channels;
    cv::Mat_<uchar> m_H;
    cv::Mat_<uchar> m_S;
    cv::Mat_<uchar> m_V;
    bool m_color;//yellow:1  white:0
    std::vector<std::vector<cv::Point> > m_contours;
    std::vector<cv::Vec4i> m_hierarchy; //Vec4i is a vector contains four number of int
    cv::Rect m_aRect;
};

#endif // FINE_CONTOURS_H
