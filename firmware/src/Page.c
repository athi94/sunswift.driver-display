/** Page.c - Page object defines a single LCD page **/

#include <project/Page.h>

/** Create a page with a given number of characters and characters per line **/
int __init__Page__(Page* self, Coordinate pageStart) 
{
        self->pageStart = pageStart;        
        
        return ERR_NONE;
}

int printPage(Page* self) 
{
    int i;
    Coordinate c=self->pageStart;    

#if LCD_NO_OF_SLIDERS > 0
    for(i=0; i<LCD_NO_OF_SLIDERS; ++i)
    {        
        c.y = c.y-FONT_HEIGHT;
        setNextWriteCoordinate(c);    
        drawSlider(&(self->sliders[i]));
    }
#endif    

    for(i=0; i<LCD_NO_OF_LINES; ++i)
    {        
        c.y = c.y - FONT_HEIGHT;
        setNextWriteCoordinate(c);            
        printLine(&(self->lines[i]));
    }   
    return ERR_NONE;
}

int setPageStart(Page* self, Coordinate c) { 
    self->pageStart = c;    
    return ERR_NONE;    
}

int setNextWriteCoordinate(Coordinate coordinate) { 
    UART_putchar(LCD_COMMAND_BYTE);
    UART_putchar(LCD_COMMAND_SET_X);
    UART_putchar(coordinate.x);

    UART_putchar(LCD_COMMAND_BYTE);
    UART_putchar(LCD_COMMAND_SET_Y);
    UART_putchar(coordinate.y);    
    
    return ERR_NONE;
}

int clearScreen() {
    UART_putchar(LCD_COMMAND_BYTE);
    UART_putchar(LCD_CLEAR_SCREEN);
    
    return ERR_NONE;
}

int displayeVeLogo() { 
    UART_printf("       x     x       ");
    UART_printf(" xxx   x     x   xxx ");    
    UART_printf("x   x  x     x  x   x");
    UART_printf("xxxxx  x     x  xxxxx");
    UART_printf("x       x   x   x    ");
    UART_printf("x   x    xxx    x   x");
    UART_printf(" xxx      x      xxx ");
        
    return ERR_NONE;
}

/** Free the page - specifically memory for text **/ 
int __free__Page__(Page* self) {    
    return ERR_NONE;
}


