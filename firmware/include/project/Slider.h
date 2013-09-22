/** Slider.h - Slider on a line**/

#ifndef __SLIDER_H__
#define __SLIDER_H__

#include <project/LcdConstants.h>
#include <project/Coordinate.h>
#include <project/Line.h>
#include <arch/uart.h>
#include <scandal/stdio.h> 
#include <project/driver_config.h>

typedef struct
{
    unsigned int min;
    unsigned int max;           
    Parameter* parameter; 
} Slider;

int __init__Slider__(Slider* self, unsigned int min, unsigned int max);

int setMin(Slider* self, unsigned int min); 

int setMax(Slider* self, unsigned int max); 

int addParameterToSlider(Slider* self, Parameter* param);

int drawSlider(Slider* self);

int __free__Slider__(Slider* self);

#endif
