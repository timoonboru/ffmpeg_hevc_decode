#include "imgshow.h"

imgShow::imgShow(QObject *parent) : QObject(parent)
{
   // cv::namedWindow("desktop");
}

void imgShow::slNewImg(cv::Mat mat)
{
    img = mat;
    cv::imshow("desktop",mat);

    cv::waitKey(1);

}
void imgShow::slTest()
{
    qDebug()<<"HERE";
}

