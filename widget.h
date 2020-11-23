#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <wiringimx.h>
#include <QTimer>
#include <sys/time.h>

using namespace cv;

// 출력화면 사이즈
#define WIDTH 640
#define HEIGHT 480

// 핀 번호
#define	LED_CTL		122

/*Inserted By dhshim*/
#define EMPTY		0
#define FILLED		1
#define FAIL		2
#define SUB_STEP_1	0
#define SUB_STEP_2	1
#define SUB_STEP_3	2
#define SUB_STEP_4	3
#define SUB_STEP_5	4
#define SUB_STEP_6	5
#define SUB_STEP_7	6
#define EMPTY_DETECT	0
#define FILLED_DETECT	1
#define ONE_STEP	1
/*fin*/

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

public: //변수
    int state;
    int duty;
    QImage *img;
    QPixmap *pixmap;
    Mat raw, mat_img, gray;
    QTimer *videoTimer;

    /*Inserted By dhshim*/
    CascadeClassifier empty_cascade;
//    CascadeClassifier bubble_cascade;
    const char * text;
    int empty_count;
    int filled_count;
    int temp_count;
    int temp_state;
    int mode;
    struct timeval savedTime;
    struct timeval currentTime;
    struct timeval b4_tv;
    struct timeval af_tv;
    int subStep;
    int cfps;
    int fps;
    int isInit;
    /*fin*/

public: //함수
    void actuatorReset(void);
    void actuatorStart(void);
    int detection(int dmode);
    void oneStep();
    void subStep1(int stt);
    void subStep2();
    bool subStep3(int delay_sec);
    bool subStep4(int dmode);
    void subStep5(int stt);
    bool subStep6(int delay_sec);
    bool subStep7(int dmode);

public slots: //슬롯 함수
    void clickButton(void);
    void video_capture(void);
private slots:
    void on_prevStep_clicked();
    void on_nextStep_clicked();
    void on_pauseAndPlay_clicked(bool checked);
};
#endif // WIDGET_H
