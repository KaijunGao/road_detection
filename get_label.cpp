#include "get_label.h"

using namespace cv;
using namespace std;

void get_label::setchannels(vector<Mat> channels, cv::Mat_<uchar> *m_BGR, string keyword)
{
    m_channels = channels;
    if(keyword == "line")
    {
        *m_BGR = m_channels.at(0);
    }
    else if(keyword == "sign")
    {
        *m_BGR = m_channels.at(2);
    }
    else
    {
        cerr<<"get_label's keyword error"<<endl;
    }
}

void get_label::setsplit(Mat label_image,Mat *get_label_image, string keyword)//split the lines
{
    vector<Mat> channels;

    Mat label_image_copy=label_image;
    split(label_image_copy,channels);
    if(!channels.data()) cerr<<"error->channels"<<endl;

    if(keyword == "line")
    {
        setchannels(channels,&m_BGR,keyword);

        m_B=m_BGR;
        for(int i=0;i != m_B.cols;++i)//line
        {
            for(int j=0;j != m_B.rows;++j)
            {
                if(m_B(j,i)==204 || m_B(j,i)==51 || m_B(j,i)==0)
                {
                    m_B(j,i)=0;
                }
                else
                {
                    m_B(j,i)=255;
                }
            }
        }
        *get_label_image=m_B;
        if(!get_label_image->data) cerr<<"get label error"<<endl;
    }

    if(keyword == "sign")
    {
        setchannels(channels,&m_BGR,keyword);

        m_R=m_BGR;
        for(int i=0;i != m_R.cols;++i)//road sign
        {
            for(int j=0;j != m_R.rows;++j)
            {
                if(m_R(j,i)==153)
                {
                    m_R(j,i)=255;
                }
                else if (m_R(j,i)==255 || m_R(j,i)==102)
                {
                    m_R(j,i)=0;
                }
            }
        }
        *get_label_image=m_R;
        if(!get_label_image->data) cerr<<"get label error"<<endl;
    }
}
