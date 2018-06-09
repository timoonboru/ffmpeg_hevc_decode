#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QUdpSocket>
#include <QEventLoop>
#include <QDateTime>

#include <imgshow.h>


using namespace std;

#define SIZE_W 800
#define SIZE_H 500


#define __STDC_CONSTANT_MACROS
//#ifdef __cplusplus
extern "C" {
//#endif
    #include <libavutil/avutil.h>
    #include <libavutil/imgutils.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
    #include <libavcodec/version.h>
    #include <libavutil/time.h>
    #include <libavutil/mathematics.h>
    #include "libavcodec/avcodec.h"
//#ifdef __cplusplus
}
//#endif

#define FF_INPUT_BUFFER_PADDING_SIZE 16

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int DecodeInit();
signals:
    void snNewImg(cv::Mat mat);
    void snTest();

private slots:
    void HEVCDecode();


private:
    Ui::MainWindow *ui;

    AVCodec *pCodec;
    AVCodecContext *pCodecCtx= NULL;
    AVCodecParserContext *pCodecParserCtx=NULL;

    FILE *fp_in;
    FILE *fp_out;
    AVFrame	*pFrame;

    int in_buffer_size=4096*16;
    uint8_t *in_buffer;
    uint8_t *cur_ptr;
    int cur_size;
    AVPacket packet;
    int ret, got_picture;

    char *filepath_out;

    int first_time;
    QUdpSocket *frameReceiver;

    enum AVCodecID codec_id=AV_CODEC_ID_HEVC;

    cv::Mat mat_yuv;
    cv::Mat mat;

    QDateTime current_date_time;
    QString current_data;

    imgShow *IMGshow;
    QThread *thread;


};

#endif // MAINWINDOW_H
