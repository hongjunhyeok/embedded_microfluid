#include "widget.h"
#include "ui_widget.h"
/*Inserted By dhshim*/
#include <vector>
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/objdetect.hpp>
#include <time.h>
#include <stdio.h>

using namespace cv;
using namespace std;

/*fim*/

VideoCapture cap("imxv4l2videosrc device=\"/dev/video0\" ! videoconvert ! appsink");

/*Inserted By dhshim*/
String empty_model = "filled_201123.xml";
//String bubble_model = "perfect_bubble_20201015_1.xml";

vector<Rect> empty_objects;
//vector<Rect> bubble_objects;

int cropped_x=0;
int cropped_y=0;
int cropped_w=0;
int cropped_h=0;
int copy_x=275;
int copy_y=195;
int copy_w=90;
int copy_h=70;
/*fin*/

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    /*Inserted by dhshim*/
    if(!empty_cascade.load(empty_model)){
        qDebug()<<"invailid model file";
    }
//    if(!bubble_cascade.load(bubble_model)){
//        qDebug()<<"invailid model file";
//    }

    mode = 0;
    subStep = 0;
    gettimeofday(&b4_tv, NULL);
    cfps = 0;
    empty_count = 0;
    filled_count = 0;
    temp_count = 0;
    temp_state = 0;
    isInit = 1;
    ui->label->setPixmap(QPixmap(WIDTH,HEIGHT));
    ui->pauseAndPlay->setEnabled(false);
    ui->prevStep->setEnabled(false);
    ui->nextStep->setEnabled(false);
    /*fin*/

    img = new QImage();
    pixmap = new QPixmap();
    videoTimer = new QTimer(this);

    // open video device
    if (!cap.isOpened()) {
        printf("error : can't open camera\n");
        exit(1);
    }

    // LED, set direction
    pinModeImx(LED_CTL, OUTPUT); 

    // connect slot
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clickButton()));
    QObject::connect(videoTimer, SIGNAL(timeout()), this, SLOT(video_capture()));

    actuatorReset();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::clickButton(void)
{
    //start
    if(ui->pushButton->isChecked()) {
        ui->pushButton->setText("RESET!!");
        videoTimer->start(33);
        digitalWriteImx(LED_CTL, HIGH);//LED On
	actuatorStart();
    }
    else { //stop, reset
        ui->pushButton->setText("START!!");
	//ui->pauseAndPlay->setText("▶");
	ui->pauseAndPlay->setText("Mode");
	ui->pauseAndPlay->setChecked(false);
	ui->pauseAndPlay->setEnabled(false);
	ui->prevStep->setEnabled(false);
        ui->nextStep->setEnabled(false);
	ui->label->setPixmap(QPixmap(WIDTH,HEIGHT));
        videoTimer->stop();
        digitalWriteImx(LED_CTL, LOW) ; //LED Off
        actuatorReset();
    }
}

void Widget::video_capture(void)
{
    cap.read(raw);
    if(raw.empty()) {
        printf("error : frame is empty.\n");
        exit(1);
    }

    gettimeofday(&af_tv, NULL);
    if((((double)(af_tv.tv_usec - b4_tv.tv_usec)/1000000)+(double)(af_tv.tv_sec - b4_tv.tv_sec)) >= 1)
    {
	fps=cfps;
        gettimeofday(&b4_tv, NULL);
        cfps = 0;
    }
    cfps++;
    putText(raw, "FPS: "+to_string(fps), Point(35, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);

    gettimeofday(&currentTime, NULL);

    switch(mode){
            case 0:
                    putText(raw, "STEP: "+to_string(mode+1), Point(35, 90), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);

		    if( subStep == SUB_STEP_1 ){
			    oneStep();
			    subStep1(FILLED);
		    }

		    //Filled Detection
                    if( subStep == SUB_STEP_2 ){
                            subStep2();
                    }

                    if( subStep == SUB_STEP_3 ){
			    if( isInit == 1 )
				    if( !subStep3(5) ) break;
			    ui->pauseAndPlay->setText("Automatic\nmode");
			    ui->pauseAndPlay->setEnabled(true);
			    ui->pauseAndPlay->setChecked(true);
			    isInit = 0;
			    if( isInit == 0 )
	                            if( !subStep3(1) ) break;
                    }

                    if( subStep == SUB_STEP_4 ){
                    	    putText(raw, "Filled detection required.", Point(330, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
			    if( temp_state == ONE_STEP ){
				    subStep -= 2;
				    oneStep();
			    }
                            if( !subStep4(FILLED_DETECT) ){
				    break;
			    }
			    oneStep();
                    }

		    //Empty Detection
		    if( subStep == SUB_STEP_5 ){
                            subStep5(EMPTY);
                    }

                    if( subStep == SUB_STEP_6 ){
                            if( !subStep6(1) ) break;
                    }

		    if( subStep == SUB_STEP_7 ){
                    	    putText(raw, "Empty detection required.", Point(318, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
			    if( temp_state == ONE_STEP ){
                                    subStep -= 2;
                                    oneStep();
                            }
                            if( !subStep7(EMPTY_DETECT) ){
                                    break;
                            }
		    }

                    mode ++;
                    subStep=0;
                    break;

            case 1:
		    putText(raw, "STEP: "+to_string(mode+1), Point(35, 90), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);

                    if( subStep == SUB_STEP_1 ){
                            oneStep();
                            subStep1(FILLED);
                    }

                    //Filled Detection
                    if( subStep == SUB_STEP_2 ){
                            subStep2();
                    }

                    if( subStep == SUB_STEP_3 ){
                            if( !subStep3(1) ) break;
                    }

                    if( subStep == SUB_STEP_4 ){
                    	    putText(raw, "Filled detection required.", Point(330, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
                            if( temp_state == ONE_STEP ){
                                    subStep -= 2;
                                    oneStep();
                            }
                            if( !subStep4(FILLED_DETECT) ){
                                    break;
                            }
			    oneStep();
                    }

                    //Empty Detection
                    if( subStep == SUB_STEP_5 ){
                            subStep5(EMPTY);
                    }

                    if( subStep == SUB_STEP_6 ){
                            if( !subStep6(1) ) break;
                    }

                    if( subStep == SUB_STEP_7 ){
                    	    putText(raw, "Empty detection required.", Point(318, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
                            if( temp_state == ONE_STEP ){
                                    subStep -= 2;
                                    oneStep();
                            }
                            if( !subStep7(EMPTY_DETECT) ){
                                    break;
                            }
                    }

                    mode ++;
                    subStep=0;
                    break;
            case 2:
		    putText(raw, "STEP: "+to_string(mode+1), Point(35, 90), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);

                    if( subStep == SUB_STEP_1 ){
                            oneStep();
                            subStep1(FILLED);
                    }

                    //Filled Detection
                    if( subStep == SUB_STEP_2 ){
                            subStep2();
                    }

                    if( subStep == SUB_STEP_3 ){
                            if( !subStep3(1) ) break;
                    }

                    if( subStep == SUB_STEP_4 ){
                    	    putText(raw, "Filled detection required.", Point(330, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
                            if( temp_state == ONE_STEP ){
                                    subStep -= 2;
                                    oneStep();
                            }
                            if( !subStep4(FILLED_DETECT) ){
                                    break;
                            }
			    oneStep();
                    }

                    //Empty Detection
                    if( subStep == SUB_STEP_5 ){
                            subStep5(EMPTY);
                    }

                    if( subStep == SUB_STEP_6 ){
                            if( !subStep6(1) ) break;
                    }

                    if( subStep == SUB_STEP_7 ){
                    	    putText(raw, "Empty detection required.", Point(318, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
                            if( temp_state == ONE_STEP ){
                                    subStep -= 2;
                                    oneStep();
                            }
                            if( !subStep7(EMPTY_DETECT) ){
                                    break;
                            }
                    }

                    mode ++;
                    subStep=0;
                    break;
            case 3:
		    putText(raw, "STEP: "+to_string(mode+1), Point(35, 90), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);

                    if( subStep == SUB_STEP_1 ){
                            oneStep();
                            subStep1(FILLED);
                    }

                    //Filled Detection
                    if( subStep == SUB_STEP_2 ){
                            subStep2();
                    }

                    if( subStep == SUB_STEP_3 ){
                            if( !subStep3(1) ) break;
                    }

                    if( subStep == SUB_STEP_4 ){
                    	    putText(raw, "Filled detection required.", Point(330, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
                            if( temp_state == ONE_STEP ){
                                    subStep -= 2;
                                    oneStep();
                            }
                            if( !subStep4(FILLED_DETECT) ){
                                    break;
                            }
			    oneStep();
                    }

                    //Empty Detection
                    if( subStep == SUB_STEP_5 ){
                            subStep5(EMPTY);
                    }

                    if( subStep == SUB_STEP_6 ){
                            if( !subStep6(1) ) break;
                    }

                    if( subStep == SUB_STEP_7 ){
                    	    putText(raw, "Empty detection required.", Point(318, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
                            if( temp_state == ONE_STEP ){
                                    subStep -= 2;
                                    oneStep();
                            }
                            if( !subStep7(EMPTY_DETECT) ){
                                    break;
                            }
                    }

                    mode ++;
                    subStep=0;
                    break;
            case 4:
		    putText(raw, "STEP: "+to_string(mode+1), Point(35, 90), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);

                    if( subStep == SUB_STEP_1 ){
                            oneStep();
                            subStep1(FILLED);
                    }

                    //Filled Detection
                    if( subStep == SUB_STEP_2 ){
                            subStep2();
                    }

                    if( subStep == SUB_STEP_3 ){
                            if( !subStep3(1) ) break;
                    }

                    if( subStep == SUB_STEP_4 ){
                    	    putText(raw, "Filled detection required.", Point(330, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
                            if( temp_state == ONE_STEP ){
                                    subStep -= 2;
                                    oneStep();
                            }
                            if( !subStep4(FILLED_DETECT) ){
                                    break;
                            }
			    oneStep();
                    }

                    mode ++;
                    subStep=0;
                    break;
            case 5:
                    putText(raw, "Finish", Point(450, 65), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);

    }

    putText(raw, "Actuator_Level: "+to_string(duty), Point(35, 430), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,0), 2);
    cv::resize(raw, raw, Size(WIDTH, HEIGHT), 0, 0, CV_INTER_LINEAR);
    /*fin*/

    cvtColor(raw, mat_img, COLOR_BGR2RGB);
    //mat 포맷을 QImage 포맷으로 변환.
    *img = QImage((uchar*) mat_img.data, mat_img.cols, mat_img.rows, mat_img.step, QImage::Format_RGB888);
    *pixmap = QPixmap::fromImage(*img);

    ui->label->setPixmap(*pixmap);
}

/* 액추에이터 동작
 * 두번째 인자 duty :  10~20 값--> 1ms ~ 2ms 값.
 액추에이터에 1ms 펄스를 주면 완전 수축,
 2ms 펄스를 주면 완전 확장.
 세번째 인자 Period 는 펄스와 펄스 사이의 간격.단위는 ms
 ex)100값 --> 100ms  이정도가 적당...
 */

int Widget::detection(int dmode)
{
	cvtColor(raw, gray, COLOR_BGR2GRAY);
	empty_cascade.detectMultiScale(gray, empty_objects, 1.017, 2, 0, Size(50,30), Size(100,60));
	if( empty_objects.size() != 0 ){
		empty_count ++;
		filled_count = 0;
		for(vector<int>::size_type i=0; i<empty_objects.size(); i++){
			cropped_x = (int)empty_objects[i].x;
			cropped_y = (int)empty_objects[i].y;
			cropped_w = (int)empty_objects[i].width;
			cropped_h = (int)empty_objects[i].height;

			copy_x = cropped_x;
			copy_y = cropped_y;
			copy_w = cropped_w;
			copy_h = cropped_h;

			Point from(cropped_x, cropped_y);
			Point to(cropped_x+cropped_w, cropped_y+cropped_h);
			if( dmode == EMPTY_DETECT ){
				rectangle(raw, from, to, Scalar(255,0,0), 2);
				putText(raw, "Empty", Point(cropped_x-5, cropped_y-5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,0,0));
			}
			else if( dmode == FILLED_DETECT ){
				rectangle(raw, from, to, Scalar(0,0,255), 2);
			}
		}
	}
	else if( empty_objects.size() == 0 ){
		empty_count = 0;
		filled_count ++;
		Point from(copy_x,copy_y);
		Point to(copy_x+copy_w, copy_y+copy_h);
		if( dmode == FILLED_DETECT ){
			rectangle(raw, from, to, Scalar(0,255,0), 2);
			putText(raw, "Filled", Point(copy_x-5, copy_y-5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));
		}
		else if( dmode == EMPTY_DETECT ){
			rectangle(raw, from, to, Scalar(0,0,255), 2);
		}
	}

	if( !(ui->pauseAndPlay->isChecked()) ){
		temp_count = 0;
		//empty_count = 0;
		//filled_count = 0;
	}

	if( temp_count < 25 ){
		temp_state = 0;
		temp_count ++;
	}
	else if( temp_count == 25 ){
		temp_state = ONE_STEP;
		temp_count = 0;
	}

	if( empty_count == 18 ){
		temp_state = 0;
		temp_count = 0;
		return EMPTY;
	}
	else if( filled_count == 18 ){
		temp_state = 0;
		temp_count = 0;
		return FILLED;
	}
	else
		return FAIL;

}

void Widget::actuatorReset(void)
{
	puts("actuator reset");
	duty = 41;
	setPwmLevel(duty);
	mode = 0;
	subStep = 0;
	empty_count = 0;
	filled_count = 0;
	temp_count = 0;
	temp_state = 0;
	isInit = 1;
//	state = FILLED;
}

void Widget::actuatorStart(void)
{
	puts("close arm");
	duty = 28;
	setPwmLevel(duty);
}

/*Inserted by dhshim*/
void Widget::oneStep(){
    duty-=1;
    setPwmLevel(duty);
}

void Widget::subStep1(int stt){
    printf("===========================================\n");
    printf("mode: %d, sub_step: %d\n", mode, subStep+1);
    state=stt;
    subStep ++;
}

void Widget::subStep2(void){
    printf("===========================================\n");
    printf("mode: %d, sub_step: %d\n", mode, subStep+1);
    gettimeofday(&savedTime, NULL);
    subStep ++;
}

bool Widget::subStep3(int delay_sec){
    printf("===========================================\n");
    printf("mode: %d, sub_step: %d\n", mode, subStep+1);
    if( currentTime.tv_sec - savedTime.tv_sec < delay_sec )
        return false;
    else{
	if( isInit == 0 )
            subStep ++;
    }
    return true;
}

bool Widget::subStep4(int dmode){
    printf("===========================================\n");
    printf("[DETECTING]\n");
    printf("mode: %d, sub_step: %d\n", mode, subStep+1);
    if( detection(dmode) != state )
        return false;
    else{
        subStep ++;
    }
    return true;
}

void Widget::subStep5(int stt){
    state = stt;
    subStep2();
}

bool Widget::subStep6(int delay_sec){
    return subStep3(delay_sec);
}

bool Widget::subStep7(int dmode){
    return subStep4(dmode);
}

void Widget::on_prevStep_clicked()
{
	if( duty < 41 ){
		duty+=1;
		setPwmLevel(duty);
		printf("prev button, duty: %d\n", duty);
	}
}

void Widget::on_nextStep_clicked()
{
	if( duty > 2 ){
		duty-=1;
		setPwmLevel(duty);
        	printf("next button, duty: %d\n", duty);
	}
}

void Widget::on_pauseAndPlay_clicked(bool checked)
{
	if( checked ){
		ui->pauseAndPlay->setText("Automatic\nmode");
		ui->prevStep->setEnabled(false);
		ui->nextStep->setEnabled(false);
	}
	else{
		//ui->pauseAndPlay->setText("▶");
		ui->pauseAndPlay->setText("Manual\nmode");
		ui->prevStep->setEnabled(true);
		ui->nextStep->setEnabled(true);
	}
}
/*fin*/
