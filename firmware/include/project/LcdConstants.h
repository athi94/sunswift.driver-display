/** LCD constants **/

#ifndef __LCD_CONSTANTS_H__
#define __LCD_CONSTANTS_H__

//#define DEBUG_MODE  1    //several UART_printf() statements for debugging are executed

#define ERR_NONE 0
#define ERR_X_OUT_LIMITS 1
#define ERR_Y_OUT_LIMITS 2

//Font width and Font height
#define FONT_WIDTH  6
#define FONT_HEIGHT 8

//LCD character/pixels count
#define LCD_WIDTH   128
#define LCD_HEIGHT  64
#define LCD_LINE_WIDTH                   20

//LCD number of sliders and lines 
#define LCD_NO_OF_SLIDERS                2  //(64/8)-1
#define LCD_NO_OF_LINES                  5  //(64/8)-1
#define MAX_NUM_SLIDER_BARS                  10  

//Miscellaneous 
#define LCD_NO_OF_PAGES                 1
#define LCD_LABEL_LENGTH                5
#define LCD_PAGE_UPDATE_PERIOD         4000 //in milliseconds
#define LCD_SOURCE_CHOICE_TIMEOUT      5000
#define LCD_REFRESH_PERIOD           60000 //Refresh LCD after LCD_REFRESH_PERIOD - clean screen and start displaying again 


#define LCD_START_COORDINATE_X     1
#define LCD_START_COORDINATE_Y     67

//LCD writing commands
#define LCD_COMMAND_BYTE    0x7C
#define LCD_COMMAND_SET_X   0x18
#define LCD_COMMAND_SET_Y   0x19
#define LCD_CLEAR_SCREEN    0x00

#endif

