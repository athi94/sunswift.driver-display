/** page.h - Page object defines a single LCD page **/

#ifndef __PAGE_H__
#define __PAGE_H__

#include <project/LcdConstants.h>
#include <project/Coordinate.h>
#include <project/Line.h>
#include <project/Coordinate.h>
#include <project/Slider.h>

typedef struct {
    Coordinate pageStart;
    
#if LCD_NO_OF_SLIDERS > 0   
    Slider sliders[LCD_NO_OF_SLIDERS];
#endif    
    Line lines[LCD_NO_OF_LINES];       
}Page;

/** Create a page with a given number of characters and characters per line **/
int __init__Page__(Page* self, Coordinate pageStart);

int printPage(Page* self);

int setPageStart(Page* self, Coordinate c);

int setNextWriteCoordinate(Coordinate coordinate);

int displayeVeLogo();

int clearScreen();

/** Free the page - specifically memory for text **/ 
int __free__Page__(Page* self);

#endif

