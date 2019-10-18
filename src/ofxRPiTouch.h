#pragma once

#include "ofMain.h"
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <syslog.h>
#include <fcntl.h>

// SYN
#define EVENT_TYPE_0    EV_SYN

// KEY
#define EVENT_TYPE_1    EV_KEY
#define EVENT_CODE_330  BTN_TOUCH
// Min  0
// Max  1

// TOUCH
#define EVENT_TYPE      EV_ABS
#define EVENT_CODE_0    ABS_X
#define EVENT_CODE_1    ABS_Y
#define EVENT_CODE_47   ABS_MT_SLOT
// Min  0
// Max  9

#define EVENT_CODE_53   ABS_MT_POSITION_X
#define EVENT_CODE_54   ABS_MT_POSITION_Y

#define EVENT_CODE_57   ABS_MT_TRACKING_ID
// Min  0
// Max  65535


class ofxRPiTouch : public ofThread {
    public:

    struct input_event ev;
    int fd, size;
    char name[256];
    char *device;

    int x,y;
    int button;
    ofVec2f pos;
    int mtSlot;
    ofVec2f absPos;
    int absTrackingID;

    int screenW = 1920;
    int screenH = 1080;
    
    /* Replace these values with the ones from evtest on your screen */
    int event_x_max = 16384;
    int event_y_max = 9600;

//-----------------------------------------------------------------------
    int init(string d) {
        size = sizeof (struct input_event);
        name[0]='U';
        name[1]='n';
        name[2]='k';
        name[3]='n';
        name[4]='o';
        name[5]='w';
        name[6]='n';
        device = &d[0];

        if ((fd = open(device, O_RDONLY)) < 0) {
            return 1;
        }
        startThread();

        return 0;
    }
//-----------------------------------------------------------------------
    string getName(){
        ioctl (fd, EVIOCGNAME (sizeof (name)), name);
        string str(name);
        return str;
    }
//-----------------------------------------------------------------------
    void exit(){
        stopThread();
    }
//-----------------------------------------------------------------------
    void threadedFunction(){
        while(isThreadRunning()) {
            const size_t ev_size = sizeof(struct input_event);
            ssize_t size;
            size = read(fd, &ev, ev_size);
            if (size < (ssize_t)ev_size) {
                ofLog()<<"Error size!\n";
            }

            //KEY
            if (ev.type == EVENT_TYPE_1 && ev.code == EVENT_CODE_330){
                if(ev.code == EVENT_CODE_330)
                    button = ev.value;
                btn = button;
            }

            //TOUCH
            if (ev.type == EVENT_TYPE && ( ev.code == EVENT_CODE_0 || ev.code == EVENT_CODE_1 || ev.code == EVENT_CODE_47 || ev.code == EVENT_CODE_53 || ev.code == EVENT_CODE_54 || ev.code == EVENT_CODE_57 )) {
                if(ev.code == EVENT_CODE_0) {
                    x = ofMap(ev.value, 0,event_x_max,0,screenW);
                }

                if(ev.code == EVENT_CODE_1) {
                    y = ofMap(ev.value, 0,event_y_max,0,screenH);
                }

                pos.set(x,y);

                if (ev.code == EVENT_CODE_47) {
                    mtSlot = ev.value;
                }

                if (ev.code == EVENT_CODE_53) {
                    absPos.x = ev.value;
                }

                if (ev.code == EVENT_CODE_54) {
                    absPos.y = ev.value;
                }

                if (ev.code == EVENT_CODE_57) {
                    absTrackingID = ev.value;
                }
            }
        }
    }
//----------------------------
    // Position
    ofVec2f getCoordTouch(){
        return pos;
    }
//----------------------------
    //Clicked
    int getButton(){
        return button;
    }
//----------------------------
    //Finger ID
    int getMTSlot(){
        return mtSlot;
    }
//----------------------------
    //Absolute Position
    ofVec2f getAbsPos(){
        return absPos;
    }

//----------------------------
    //Tracking ID
    int getAbsTrackingID(){
        return absTrackingID;
    }
};
