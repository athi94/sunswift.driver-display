/** A new line on the LCD screen **/ 

#include <project/Line.h>

int __init__Line__(Line* self) {       
    return ERR_NONE;
}

int addParameter(Line* self, Parameter* parameter) {
    self->parameter = parameter; 
    return ERR_NONE;
}

int printLine(Line* self) {           
    if(self->parameter != NULL) {        
        updateParameter(self->parameter);
        UART_printf("%s: %u %s", self->parameter->label, self->parameter->value, self->parameter->unit);        
        UART_printf("         "); //Clear any remaining data on the line
    } else {
        int i;
        for(i=0; i<LCD_LINE_WIDTH; ++i) {
            UART_printf(" ");
        }
    }
    
    return ERR_NONE; 
}

int __free__Line__(Line* self) {    
    return ERR_NONE;
}
