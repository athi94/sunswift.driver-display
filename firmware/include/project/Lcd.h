/** Lcd Functions **/

#ifndef __LCD_H__
#define __LCD_H__

#include <project/Page.h>
#include <project/LcdConstants.h>
#include <arch/uart.h>
#include <project/Coordinate.h>

typedef struct {      
    int currentPage;
    int numberOfPages;
    Page pages[LCD_NO_OF_PAGES];       
} Lcd;

int __init__Lcd__(Lcd* self, int numberOfPages);

int updateLcdScreen(Lcd* self);

int __free__Lcd__(Lcd* self);

#endif