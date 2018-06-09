#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

static QString ByteArrayToHexStr(QByteArray data)
{
   QString temp="";
   QString hex=data.toHex();
   for(int i=0;i<hex.length();i=i+2)
   {
      temp+=hex.mid(i+2)+" ";
   }
   return temp.trimmed().toUpper();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<cv::Mat>("cv::Mat");

    IMGshow = new imgShow();
    thread = new QThread(this);
    IMGshow->moveToThread(thread);
    connect( thread, SIGNAL(finished()),IMGshow,SLOT(deleteLater()));
    connect(this, SIGNAL(snNewImg(cv::Mat)) , IMGshow , SLOT(slNewImg(cv::Mat)));
    connect(this, SIGNAL(snTest()) , IMGshow , SLOT(slTest()));

    thread->start();

    frameReceiver = new QUdpSocket();
    frameReceiver->bind(QHostAddress::Any,6652);
    connect(frameReceiver,SIGNAL(readyRead()),this,SLOT(HEVCDecode()));
    DecodeInit();
}

MainWindow::~MainWindow()
{
    free(in_buffer);

    //fclose(fp_out);

    av_parser_close(pCodecParserCtx);

    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    av_free(pCodecCtx);
    delete ui;
}

int MainWindow::DecodeInit()
{

    in_buffer_size=4096*16;
    //in_buffer[in_buffer_size + FF_INPUT_BUFFER_PADDING_SIZE]={0};
    in_buffer = (uint8_t *)malloc(in_buffer_size + FF_INPUT_BUFFER_PADDING_SIZE);

    //filepath_out= "out_akiyo_cif.yuv";
    first_time=1;

    avcodec_register_all();
    pCodec = avcodec_find_decoder(codec_id);
    if (!pCodec) {
        printf("Codec not found\n");
        return -1;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx){
        printf("Could not allocate video codec context\n");
        return -1;
    }

    pCodecParserCtx=av_parser_init(codec_id);
    if (!pCodecParserCtx){
        printf("Could not allocate video parser context\n");
        return -1;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec\n");
        return -1;
    }

    //Output File
    //fp_out = fopen(filepath_out, "wb");
    //if (!fp_out) {
    //    printf("Could not open output YUV file\n");
    //    return -1;
   // }

    pFrame = av_frame_alloc();
    av_init_packet(&packet);


    mat_yuv = Mat::zeros(SIZE_H*1.5,SIZE_W,CV_8UC1);
    mat = Mat::zeros(SIZE_H,SIZE_W,CV_8UC1);

    //cv::namedWindow("desktop");

    //imshow("desktop", mat);

    //cv::waitKey(10);

    return 1;
}

void MainWindow::HEVCDecode()
{
    while (frameReceiver->hasPendingDatagrams())
    {
        current_date_time =QDateTime::currentDateTime();
        current_data =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
        //qDebug()<< "here hasPendingDatagrams" <<current_data;


        QByteArray datagram;
        //qDebug()<<"pendingDatagramSize = " <<frameReceiver->pendingDatagramSize();
        datagram.resize(frameReceiver->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        frameReceiver->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        //QString qstring = ByteArrayToHexStr(datagram);

        //memcpy(in_buffer,datagram.data(),datagram.size());
        QDataStream dsOut(&datagram, QIODevice::ReadWrite);
        dsOut.readRawData((char *) (in_buffer) ,datagram.size() );

        cur_size = datagram.size();
        if (cur_size == 0)
            break;
        cur_ptr=in_buffer;

        while (cur_size>0){

            int len = av_parser_parse2(
                pCodecParserCtx, pCodecCtx,
                &packet.data, &packet.size,
                cur_ptr , cur_size ,
                AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

            cur_ptr += len;
            cur_size -= len;

            if(packet.size==0)
                continue;

            //Some Info from AVCodecParserContext
            printf("[Packet]Size:%6d\t",packet.size);
            switch(pCodecParserCtx->pict_type){
                case AV_PICTURE_TYPE_I: printf("Type:I\t");break;
                case AV_PICTURE_TYPE_P: printf("Type:P\t");break;
                case AV_PICTURE_TYPE_B: printf("Type:B\t");break;
                default: printf("Type:Other\t");break;
            }
            printf("Number:%4d\n",pCodecParserCtx->output_picture_number);

            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
            if (ret < 0) {
                printf("Decode Error.\n");
            }
            if (got_picture) {

                /*
                //Y, U, V
                for(int i=0;i<pFrame->height;i++){
                    fwrite(pFrame->data[0]+pFrame->linesize[0]*i,1,pFrame->width,fp_out);
                }
                for(int i=0;i<pFrame->height/2;i++){
                    fwrite(pFrame->data[1]+pFrame->linesize[1]*i,1,pFrame->width/2,fp_out);
                }
                for(int i=0;i<pFrame->height/2;i++){
                    fwrite(pFrame->data[2]+pFrame->linesize[2]*i,1,pFrame->width/2,fp_out);
                }
*/

                //int in_w = 1366, in_h = 768;
                int y_size = SIZE_H*SIZE_W;

                for(int k = 0; k < y_size; k++ )
                {
                    int quotient  = k / pCodecCtx->width;
                    int remainder  = k % pCodecCtx->width;
                    *(mat_yuv.data + k) =
                            *(pFrame->data[0] + pFrame->linesize[0]*quotient +remainder);
                }

                for(int k = 0; k < y_size/4; k++ )
                {
                    int quotient  = k / (pCodecCtx->width/2);
                    int remainder  = k % (pCodecCtx->width/2);
                    *(mat_yuv.data + y_size + k) =
                            *(pFrame->data[1] + pFrame->linesize[1]*quotient +remainder);
                }

                for(int k = 0; k < y_size/4; k++ )
                {
                    int quotient  = k / (pCodecCtx->width/2);
                    int remainder  =  k % (pCodecCtx->width/2);
                    *(mat_yuv.data + y_size + y_size/4 + k) =
                            *(pFrame->data[2] + pFrame->linesize[2]*quotient +remainder);
                }

                cv::cvtColor(mat_yuv, mat, CV_YUV2RGB_I420);

                if(first_time){
                    //printf("\nCodec Full Name:%s\n",pCodecCtx->codec->long_name);
                    //printf("width:%d\nheight:%d\n\n",pCodecCtx->width,pCodecCtx->height);
                    qDebug()<<"pCodecCtx->width ="<<pCodecCtx->width<<"pCodecCtx->height"<<pCodecCtx->height;

                    current_date_time =QDateTime::currentDateTime();
                    current_data =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
                    qDebug()<< "here first_time" <<current_data;
                    first_time=0;
                    //QString time_bmp = current_data +".bmp";
                    //imwrite(time_bmp.toStdString(),mat);
                }
                current_date_time =QDateTime::currentDateTime();
                current_data =current_date_time.toString("hh_mm_ss_zzz");
                //qDebug()<< "here imshow" <<current_data;


                imshow("desktop",mat);
                cv::waitKey(10);

                //QString time_bmp = current_data +".bmp";
                //imwrite(time_bmp.toStdString(),mat);

                current_date_time =QDateTime::currentDateTime();
                current_data =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
                //qDebug()<< "here imshow" <<current_data;

          //      emit snNewImg(mat);
      //          emit snTest();

                printf("Succeed to decode 1 frame!\n");
            }
        }

    }

}
