#include <test.h>

using namespace cv;
using namespace std;
using namespace cv::ml;

int main()
{
    Ptr<SVM> svm=SVM::load("/home/em-gkj/devdata/DownLoads/road_segmentation/test_img/pic02250228_roadsign_warp/SVM_HOG.xml");//Load SVM Model
    Ptr<ANN_MLP> ann = ANN_MLP::load("/home/em-gkj/devdata/DownLoads/road_segmentation/test_img/pic02250228_roadsign_warp/ANN.xml");

    ifstream file("/home/em-gkj/devdata/DownLoads/road_segmentation/test_img/pic0225_label/name.txt");
    while (!file.eof())
    {
        char txt_cont[1000];
        file.getline(txt_cont,1000);
        char label_file[1000],src_file[1000];

        sprintf(label_file, "/home/em-gkj/devdata/DownLoads/road_segmentation/test_img/pic0225_label/%s", txt_cont);
        sprintf(src_file, "/home/em-gkj/devdata/DownLoads/road_segmentation/test_img/pic0225_resized/%s", txt_cont);

        Mat label_image = imread(label_file);
        Mat src_image = imread(src_file);
        //imshow("label_image",label_image);
        //imshow("src_image",src_image);
        Mat line_label_image;
        Mat sign_label_image;
        Mat line_image_warp;
        Mat sign_image_warp;
        Mat src_image_warp;

        /* get line or sign label */
        get_label linelabel;
        linelabel.setsplit(label_image,&line_label_image, "line");//keyword "line"&&"sign"
        //imshow("line label",line_label_image);
        get_label signlabel;
        signlabel.setsplit(label_image,&sign_label_image, "sign");
        //imshow("road sign label",sign_label_image);

        /* warp perspective for line_label_image, sign_label_image and src_image */
        Warp_Perspective warppersective;
        warppersective.getWarp_Perspective(&line_label_image,&line_image_warp);//line_label_image
        warppersective.getWarp_Perspective(&sign_label_image,&sign_image_warp);//sign_label_image
        warppersective.getWarp_Perspective(&src_image,&src_image_warp);//src
        //Mat src_image_warp_copy=src_image_warp.clone();

        /* detect lines */
        imshow("src_image_warp",src_image_warp);
        Detect_Line detectline;
        detectline.get_contours(&line_image_warp,&src_image_warp);
        //imshow("detect_image",src_image_warp);

        //Projection pro;//积分投影+HOG+SVM
        //pro.projection(&line_image_warp,&src_image_warp);
        //pro.projection(&Contours.dotted_lines_image,&src_image_warp);
        //imshow("src_image_warp",src_image_warp);

        /* detect road sign */
        Detect_Sign detectsign;
        //detectsign.sign_classify(sign_image_warp,src_image_warp, svm);//svm
        detectsign.sign_classify(sign_image_warp,src_image_warp, ann);//ann
        //imshow("src_image_warp_copy",src_image_warp_copy);
        waitKey();
    }

    return 0;
}
