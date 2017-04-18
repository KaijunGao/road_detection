#include "projection.h"
using namespace cv;
using namespace std;
using namespace cv::ml;

void Projection::projection(Mat *src,Mat *dst)
{
    HOGDescriptor hog(Size(64,64),Size(16,16),Size(8,8),Size(8,8),9);//HOG检测器，用来计算HOG描述子的
    int DescriptorDim=900;//HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定
    Mat sampleFeatureMat;//所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数

    Mat srcImage1 = *src;
    Mat dstImage1 = *dst;
    Mat srcImage(srcImage1,Rect(40,0,432,230));
    Mat dstImage(dstImage1,Rect(40,0,432,230));

    int *colswidth = new int[srcImage.cols];  //申请src.image.cols个int型的内存空间
    memset(colswidth, 0, srcImage.cols * 4);  //数组必须赋初值为零，否则出错。无法遍历数组。

    for (int i = 0; i != srcImage.cols; ++i)
    {
        for (int j = 0; j != srcImage.rows; ++j)
        {
            //value=cvGet2D(src,j,i);
            value = srcImage.at<uchar>(j, i);
            if (value == 255)
            {
                ++colswidth[i]; //统计每列的白色像素点
            }
        }
    }

    Mat histogramImage(srcImage.rows, srcImage.cols, CV_8UC1);
    for (int i = 0; i != srcImage.rows; ++i)
        for (int j = 0; j != srcImage.cols; ++j)
        {
            value = 255;  //背景设置为白色。
            histogramImage.at<uchar>(i, j) = value;
        }
    for (int i = 0; i != srcImage.cols; ++i)
        for (int j = 0; j != colswidth[i]; ++j)
        {
            value = 0;  //直方图设置为黑色
            histogramImage.at<uchar>(srcImage.rows - 1 - j, i) = value;
        }

    //寻找车道线
    vector<int> vector1;// all point

    for(int xx=10;xx != srcImage.cols-10;++xx)
    {
        int a=1;
        if(colswidth[xx]>=45)//50
        {
            while(colswidth[xx+a]<45)//50
            {
                a=a+1;
            }
            vector1.push_back(xx);
        }
    }

    int sign=0;
    int signnum=0;
    Size dst_cvsize=Size(20,160);

    for(unsigned int xx=1;xx<=vector1.size();++xx)//根据跳变点计算标识线数量
    {
        if(vector1[xx]-vector1[xx-1]>2)
        {
            signnum=xx-sign;
            if(signnum>=3 && signnum<=20)//3~18
            {
                Rect aRect(vector1[sign]-5,0,signnum+10,230);
                Mat srcImage3(dstImage,aRect);

                Mat src;
                resize(srcImage3,src,dst_cvsize);
                vector<float> descriptors;//HOG描述子向量
                hog.compute(src,descriptors,Size(10,10));//计算HOG描述子，检测窗口移动步长(10,10)
                //cout<<"test样本描述子维数："<<descriptors.size()<<endl;

                //处理第一个样本时初始化特征向量矩阵和类别矩阵，因为只有知道了特征向量的维数才能初始化特征向量矩阵
                DescriptorDim = descriptors.size();//HOG描述子的维数
                //初始化所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数sampleFeatureMat
                sampleFeatureMat = Mat::zeros(1, DescriptorDim, CV_32FC1);
                //sampleFeatureMat1 = Mat::zeros(PosSamNO+NegSamNO, DescriptorDim, CV_32FC1);

                //将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
                for(int i=0; i != DescriptorDim; ++i)
                {
                    sampleFeatureMat.at<float>(0,i) = descriptors[i];//第num个样本的特征向量中的第i个元素
                }

                Ptr<SVM> model =SVM::create();
                model=SVM::load("/home/gkj/Downloads/road_segmentation/test_img/roadline_pic_warp_ROI/SVM_HOG.xml");//Load SVM Model

                Mat sampleMat1 (1,DescriptorDim,CV_32FC1);
                int response=0;

                sampleMat1=sampleFeatureMat.rowRange(0,1);
                response = model->predict(sampleMat1);


                if(response==1)
                {
                    rectangle(dstImage,aRect,Scalar(100,100,255), 2);
                }
                else
                {
                    rectangle(dstImage,aRect,Scalar(255,100,100), 2);
                }

                //sprintf(file_dst,"/home/gkj/Downloads/road_segmentation/test_img/roadline_pic_warp_ROI/image%d.png",k);
                //k = k + 1;
                //imwrite(file_dst,srcImage3);
                sign=xx;
            }
            else
            {
                sign=xx;
            }
        }
    }
    imshow("histogramImage",histogramImage);
    //imshow("src",srcImage1);
    //imshow("dst",dstImage);

    //*dst=dstImage;

}


/*
void Projection::projection(Mat *src,Mat *dst)
{
    HOGDescriptor hog(Size(20,160),Size(20,20),Size(10,10),Size(10,10),9);//HOG检测器，用来计算HOG描述子的
    int DescriptorDim=900;//HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定
    Mat sampleFeatureMat;//所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数

    Mat srcImage1 = *src;
    Mat dstImage1 = *dst;
    Mat srcImage(srcImage1,Rect(40,0,432,230));
    Mat dstImage(dstImage1,Rect(40,0,432,230));

    int *colswidth = new int[srcImage.cols];  //申请src.image.cols个int型的内存空间
    memset(colswidth, 0, srcImage.cols * 4);  //数组必须赋初值为零，否则出错。无法遍历数组。

    int value;
    for (int i = 0; i < srcImage.cols; i++)
    {
        for (int j = 0; j < srcImage.rows; j++)
        {
            //value=cvGet2D(src,j,i);
            value = srcImage.at<uchar>(j, i);
            if (value == 255)
            {
                colswidth[i]++; //统计每列的白色像素点
            }
        }
    }

    Mat histogramImage(srcImage.rows, srcImage.cols, CV_8UC1);
    for (int i = 0; i < srcImage.rows; i++)
        for (int j = 0; j < srcImage.cols; j++)
        {
            value = 255;  //背景设置为白色。
            histogramImage.at<uchar>(i, j) = value;
        }
    for (int i = 0; i < srcImage.cols; i++)
        for (int j = 0; j < colswidth[i]; j++)
        {
            value = 0;  //直方图设置为黑色
            histogramImage.at<uchar>(srcImage.rows - 1 - j, i) = value;
        }

    //寻找车道线
    vector<int> vector1;// all point
    vector<int> vector2;

    for(int xx=10;xx!=srcImage.cols-10;xx++)
    {
        if(colswidth[xx]>=170)//考虑积分图特别长的情况
        {
            int b=1;
            while (colswidth[xx+b]<=170)
            {
                b=b+1;
            }
            vector2.push_back(xx);
        }
    }

    int sign=0;
    int signnum=0;

    for(unsigned int xx=1;xx<=vector2.size();xx++)//根据跳变点计算标识线数量
    {
        if(vector2[xx]-vector2[xx-1]>2)
        {
            Rect aRect(vector1[sign]-3,0,signnum+6,230);
            rectangle(dstImage,aRect,Scalar(100,100,255), 2);
            sign=xx;
        }
        else
        {
            sign=xx;
        }
    }

    Size dst_cvsize=Size(20,160);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy; //Vec4i is a vector contains four number of int

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat m_ResImg;
    //进行kai操作
    dilate(srcImage, m_ResImg, element);
    erode(m_ResImg,m_ResImg,element);

    // 提取轮廓
    findContours( srcImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    for(unsigned int i=0;i<contours.size();i++)
    {
        Rect aRect(boundingRect(contours[i]).tl().x,0,boundingRect(contours[i]).width,230);

        Mat srcImage3(dstImage,aRect);

        Mat src;
        resize(srcImage3,src,dst_cvsize);
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(10,10));//计算HOG描述子，检测窗口移动步长(10,10)
        //cout<<"test样本描述子维数："<<descriptors.size()<<endl;

        //处理第一个样本时初始化特征向量矩阵和类别矩阵，因为只有知道了特征向量的维数才能初始化特征向量矩阵
        DescriptorDim = descriptors.size();//HOG描述子的维数
        //初始化所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数sampleFeatureMat
        sampleFeatureMat = Mat::zeros(1, DescriptorDim, CV_32FC1);
        //sampleFeatureMat1 = Mat::zeros(PosSamNO+NegSamNO, DescriptorDim, CV_32FC1);

        //将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
        for(int i=0; i<DescriptorDim; i++)
        {
            sampleFeatureMat.at<float>(0,i) = descriptors[i];//第num个样本的特征向量中的第i个元素
        }

        Ptr<SVM> model =SVM::create();
        model=SVM::load("/home/gkj/Downloads/road_segmentation/test_img/roadline_pic_warp_ROI/SVM_HOG.xml");//Load SVM Model

        Mat sampleMat1 (1,DescriptorDim,CV_32FC1);
        int response=0;

        sampleMat1=sampleFeatureMat.rowRange(0,1);
        response = model->predict(sampleMat1);


        if(response==1)
        {
            rectangle(dstImage,aRect,Scalar(100,100,255), 2);
        }
        else
        {
            rectangle(dstImage,aRect,Scalar(255,100,100), 2);
        }

        //sprintf(file_dst,"/home/gkj/Downloads/road_segmentation/test_img/roadline_pic_warp_ROI/image%d.png",k);
        //k = k + 1;
        //imwrite(file_dst,srcImage3);

    }
    imshow("histogramImage",histogramImage);
    //imshow("src",srcImage1);
    //imshow("dst",dstImage);

    // *dst=dstImage;
}*/
