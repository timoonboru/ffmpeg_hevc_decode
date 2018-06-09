#ifndef IMGSHOW_H
#define IMGSHOW_H
#include <QThread>
#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class imgShow : public QObject
{
    Q_OBJECT
public:
    explicit imgShow(QObject *parent = 0);

signals:

public slots:
    void slNewImg(cv::Mat mat);
    void slTest();

private:
    cv::Mat img;

};

#endif // IMGSHOW_H
