#ofxRPiTouch
Addon for using the RaspberryPi Touch Dipsplays 

Use "evtest /dev/input/event0" (or whatever your input device is enumerated as) to get the Max. and Min. values for your calibration. 

* Position
* Button
* TouchCount
* absPosition
* absTrackingID

Provides an interface with: struct *input_event ev;*
