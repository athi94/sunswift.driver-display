/** Line.h - represents a line displayed on the Lcd Page **/

#ifndef __LINE_H__
#define __LINE_H__

#include <project/Parameter.h>
#include <arch/uart.h>

#include <project/LcdConstants.h>
#include <scandal/stdio.h> 

typedef struct {
    Parameter* parameter; //E.g. current, voltage    
    char lineContent[LCD_LINE_WIDTH]; //Message to display      
} Line;

int __init__Line__(Line* self);

int addParameter(Line* self, Parameter* parameter);

int printLine(Line* self);

int __free__Line__(Line* self);

#endif
