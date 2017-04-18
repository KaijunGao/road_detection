#include "detect_line.h"

using namespace cv;
using namespace std;

#define NorArea 300//判定的标准区域面积
#define NorDistance 150//判定的标准距离
#define LongDistance 180//判定的长距离
#define Minarea 50
#define Min_length_width_ratio 2.0

#define color_yellow_solid Scalar(0, 255, 255 )
#define color_yellow_dotted Scalar(0, 120, 120 )
#define color_white_solid Scalar(255,255, 255 )
#define color_white_dotted Scalar(150, 150, 150 )

void Detect_Line::get_contours(Mat *lines_image,Mat *src_image)
{
    m_src = *lines_image;
    m_dst = *src_image;

    if (!m_src.data || !m_dst.data)
    {
        cerr<<"find contours input error"<<endl;
    }

    // 提取轮廓
    findContours( m_src, m_contours, m_hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    cout<<"The num of contour is:"<<m_contours.size()<<endl;//输出轮廓的数量

    float length_width_ratio=0.0;

    for(size_t i=0;i !=m_contours.size();++i)
    {
        m_aRect=boundingRect( m_contours[i] );
        length_width_ratio=m_aRect.height/m_aRect.width;

        Mat m_dst_resized(m_dst,m_aRect);

        if(contourArea(m_contours[i])<Minarea || length_width_ratio<Min_length_width_ratio)//面积小于设定值的轮廓or长宽比
        {
            continue;
        }

        cout<<"height:"<<m_aRect.height<<" width:"<<m_aRect.width<<" Area:"<<contourArea(m_contours[i])<<endl;

        cvtColor(m_dst_resized,m_HSV,CV_BGR2HSV);
        split(m_HSV,m_channels);
        m_H = m_channels.at(0);
        m_S = m_channels.at(1);
        m_V = m_channels.at(2);
        m_color=0;
        int num=0;
        for(int k=0;k<m_HSV.cols;++k)
        {
            for(int j=0;j<m_HSV.rows;++j)
            {
                if(m_H(j,k)>50 && m_H(j,k)<62)//yellow
                {
                    ++num;
                    if(num>50)//number of yellow pixels
                    {
                        m_color=1;
                        break;
                    }
                }
            }
        }
        //imshow("m_H",m_H);

        if(1 == m_color)//yellow
        {
            if(contourArea(m_contours[i])>NorArea && (m_aRect.height>NorDistance || m_aRect.width>NorDistance))//实线和虚线的判定条件
            {
                drawContours( m_dst, m_contours, i, color_yellow_solid, CV_FILLED, 8, m_hierarchy );
            }
            else if((m_aRect.height>LongDistance || m_aRect.width>LongDistance))//若线比较长则判为实线
            {
                drawContours( m_dst, m_contours, i, color_yellow_solid, CV_FILLED, 8, m_hierarchy );
            }
            else//剩余为虚线
            {
                drawContours( m_dst, m_contours, i, color_yellow_dotted, CV_FILLED, 8, m_hierarchy );
            }
        }
        else//white
        {
            if(contourArea(m_contours[i])>NorArea && (m_aRect.height>NorDistance || m_aRect.width>NorDistance))//实线和虚线的判定条件
            {
                drawContours( m_dst, m_contours, i, color_white_solid, CV_FILLED, 8, m_hierarchy );
            }
            else if ((m_aRect.height>LongDistance || m_aRect.width>LongDistance))//若线比较长则判为实线
            {
                drawContours( m_dst, m_contours, i, color_white_solid, CV_FILLED, 8, m_hierarchy );
            }
            else//剩余为虚线
            {
                drawContours( m_dst, m_contours, i, color_white_dotted, CV_FILLED, 8, m_hierarchy );
            }
        }
    }
}
