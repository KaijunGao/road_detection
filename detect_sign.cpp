#include "detect_sign.h"

using namespace cv;
using namespace std;
using namespace cv::ml;

#define SignMinArea 350

void Detect_Sign::setHOG(Mat srcImage,Mat *sampleMat)//
{
    HOGDescriptor hog(Size(48,48),Size(16,16),Size(8,8),Size(8,8),9);//HOG检测器，用来计算HOG描述子的
    Size dst_cvsize=Size(48,48);
    resize(srcImage,srcImage,dst_cvsize);

    hog.compute(srcImage,descriptors,Size(8,8));//计算HOG描述子，检测窗口移动步长(8,8)
    int DescriptorDim;//HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定
    DescriptorDim = descriptors.size();
    sampleFeatureMat = Mat::zeros(1, DescriptorDim, CV_32FC1);

    //将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
    for(int i=0; i != DescriptorDim; ++i)
    {
        sampleFeatureMat.at<float>(0,i) = descriptors[i];//样本的特征向量中的第i个元素
    }

    Mat sampleMat1 (1,DescriptorDim,CV_32FC1);
    *sampleMat = sampleMat1;
    *sampleMat = sampleFeatureMat.rowRange(0,1);
}

void Detect_Sign::drawRectangle(Mat dstImage, Rect Rect, int m_response)
{
    switch (m_response)
    {
    case Diamond:
        rectangle(dstImage,Rect,Scalar(255,0,0), 2);//blue
        break;
    case Left:
        rectangle(dstImage,Rect,Scalar(0,255,0), 2);//green
        break;
    case Others:
        break;
    case Right:
        rectangle(dstImage,Rect,Scalar(0,255,255), 2);//yellow
        break;
    case Straight:
        rectangle(dstImage,Rect,Scalar(255,0,255), 2);//fense
        break;
    case Straight_Left:
        rectangle(dstImage,Rect,Scalar(0,0,0), 2);//black
        break;
    case Straight_Right:
        rectangle(dstImage,Rect,Scalar(255,255,255), 2);//white
        break;
    case Zebra_Crossing:
        rectangle(dstImage,Rect,Scalar(0,0,255), 2);//red
        break;
    default:
        break;
    }
}

void Detect_Sign::nonMaximumSuppression(const std::vector<cv::Rect>& srcRects,  std::vector<cv::Rect>& resRects,  float thresh)// 非极大值抑制算法
{
    resRects.clear();

    const size_t size = srcRects.size();
    if (!size)
    {
        cerr<<"nms input error"<<endl;
    }

    // Sort the bounding boxes by the bottom - right y - coordinate of the bounding box
    std::multimap<int, size_t> idxs;
    for (size_t i = 0; i < size; ++i)
    {
        idxs.insert(std::pair<int, size_t>(srcRects[i].br().y, i));
    }

    // keep looping while some indexes still remain in the indexes list
    while (idxs.size() > 0)
    {
        // grab the last rectangle
        auto lastElem = --std::end(idxs);
        const cv::Rect& rect1 = srcRects[lastElem->second];

        resRects.push_back(rect1);

        idxs.erase(lastElem);

        for (auto pos = std::begin(idxs); pos != std::end(idxs); )
        {
            // grab the current rectangle
            const cv::Rect& rect2 = srcRects[pos->second];

            float intArea = (rect1 & rect2).area();
            float unionArea = rect1.area() + rect2.area() - intArea;
            float overlap = intArea / unionArea;

            // if there is sufficient overlap, suppress the current bounding box
            if (overlap > thresh)
            {
                pos = idxs.erase(pos);
            }
            else
            {
                ++pos;
            }
        }
    }
}

void Detect_Sign::sign_classify(Mat sign_image,Mat detect_image, Ptr<SVM> svm)
{
    Mat dstImage = detect_image;
    if (!sign_image.data || !detect_image.data)
    {
        cerr<<"find contours input error"<<endl;
    }

    findContours( sign_image, m_contours, m_hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    cout<<"The num of contour is:"<<m_contours.size()<<endl;//输出轮廓的数量

    for(auto i : m_contours)
    {
        m_Rect = boundingRect( i );
        if(contourArea( i ) > SignMinArea)//skip面积小于设定值的轮廓
        {
            m_srcRects.push_back(m_Rect);
        }
    }

    nonMaximumSuppression(m_srcRects, m_resRects, 0.2f);

    for (auto r : m_resRects)
    {
        Mat srcImage_rect(dstImage,r);
        setHOG(srcImage_rect,&sampleMat);
        m_response = svm->predict(sampleMat);
        drawRectangle(dstImage, r, m_response);
    }
    imshow("svm_dst",dstImage);
}

void Detect_Sign::sign_classify(Mat sign_image,Mat detect_image, Ptr<ANN_MLP> ann)
{
    Mat dstImage = detect_image;
    if (!sign_image.data || !detect_image.data)
    {
        cerr<<"find contours input error"<<endl;
    }

    findContours( sign_image, m_contours, m_hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    cout<<"The num of contour is:"<<m_contours.size()<<endl;//输出轮廓的数量

    for(auto i : m_contours)
    {
        m_Rect = boundingRect( i );
        if(contourArea( i ) > SignMinArea)//skip面积小于设定值的轮廓
        {
            m_srcRects.push_back(m_Rect);
        }
    }

    m_thresh=0.2f;
    nonMaximumSuppression(m_srcRects, m_resRects, m_thresh);//非极大值抑制,过滤重叠的框

    const int num=8;
    for (auto r : m_resRects)
    {
        Mat responseMat;

        Mat srcImage_rect(dstImage,r);
        setHOG(srcImage_rect,&sampleMat);
        m_response = ann->predict(sampleMat, responseMat);

        float* p=responseMat.ptr<float>(0);
        float max= -1,min =0;
        for(int k=0;k !=num;++k)
        {
            cout<<(float)(*(p+k))<<" ";
            if(k == num-1)
                cout<<endl;
            if((float)(*(p+k))>max)
            {
                min = max;
                max = (float)(*(p+k));
            }
            else
            {
                if(min < (float)(*(p+k)))
                {
                    min = (float)(*(p+k));
                }
            }
        }

        m_confidence = ((max-min)*100) > 100 ? 100:((max-min)*100);
        cout<<"label:"<<m_response<<" Confidence is:"<<m_confidence<<endl;
        if(m_confidence >60)
        {
            drawRectangle(dstImage, r, m_response);
        }
    }
    imshow("ann_dst",dstImage);
}
