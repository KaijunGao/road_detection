#ifndef GET_LINES_LABEL_H
#define GET_LINES_LABEL_H
#include <test.h>

class get_label
{
public:
    cv::Mat label_image;
    cv::Mat *get_label_image;
    void setsplit(cv::Mat label_image, cv::Mat *get_label_image ,std::string keyword);

private:
    void setchannels(std::vector<cv::Mat> channels,cv::Mat_<uchar> *m_BGR, std::string keyword);
    std::string keyword;
    std::vector<cv::Mat> m_channels;
    cv::Mat_<uchar> m_BGR;
    cv::Mat_<uchar> m_B;//color B/G/R
    cv::Mat_<uchar> m_R;
};

#endif // GET_LINES_LABEL_H
