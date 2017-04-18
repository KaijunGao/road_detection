#ifndef SIGN_DETECT_H
#define SIGN_DETECT_H
#include <test.h>

class Detect_Sign
{
public:
    cv::Mat sign_image;
    cv::Mat detect_image;
    void sign_classify(cv::Mat sign_image, cv::Mat detect_image, cv::Ptr<cv::ml::SVM> svm);//SVM
    void sign_classify(cv::Mat sign_image, cv::Mat detect_image, cv::Ptr<cv::ml::ANN_MLP> ann);//ANN
    enum roadsign{ Diamond=0, Left, Others, Right, Straight, Straight_Left, Straight_Right,Zebra_Crossing };
    float m_thresh;
private:
    cv::Ptr<cv::ml::SVM> svm;
    cv::Ptr<cv::ml::ANN_MLP> ann;
    cv::Mat sampleMat;

    cv::Mat sampleFeatureMat;//所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数
    std::vector<std::vector<cv::Point> > m_contours;
    std::vector<cv::Vec4i> m_hierarchy; //Vec4i is a vector contains four number of int
    std::vector<float> descriptors;//HOG描述子向量

    cv::Rect m_Rect;//
    std::vector<cv::Rect> m_srcRects;//非极大值抑制处理前的rect向量
    std::vector<cv::Rect> m_resRects;//非极大值抑制处理后的rect向量
    int m_response;//标识符所属类别
    float m_confidence;//置信度

    void nonMaximumSuppression(const std::vector<cv::Rect>& srcRects,  std::vector<cv::Rect>& resRects,  float thresh);//非极大值抑制算法,过滤重叠的框
    void setHOG(cv::Mat srcImage,cv::Mat *sampleMat);
    void drawRectangle(cv::Mat dstImage, cv::Rect Rect, int m_response);
};

#endif // SIGN_DETECT_H
