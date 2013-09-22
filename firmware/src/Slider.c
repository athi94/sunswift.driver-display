/** Slider chart - can display a sliding chart based on one parameter **/ 

#include <project/Slider.h>

int __init__Slider__(Slider* self, unsigned int min, unsigned int max) 
{
    self->min = min; 
    self->max = max;     
    
    return ERR_NONE;
}

int setMin(Slider* self, unsigned int min) {
    self->min = min;    
    return ERR_NONE;
}

int setMax(Slider* self, unsigned int max) {
    self->max = max;
    return ERR_NONE;
}

int addParameterToSlider(Slider* self, Parameter* param) {
    self->parameter = param;
    return ERR_NONE;
}

int drawSlider(Slider* self)
{   
    //updateParameter(self->parameter);     
    int ratio = (int)((self->parameter->value - self->min)*MAX_NUM_SLIDER_BARS)/self->max;
    if(ratio > MAX_NUM_SLIDER_BARS) { 
        ratio = MAX_NUM_SLIDER_BARS; 
    } else if(ratio <= 0) { 
        ratio = 0;         
    }

    UART_printf("%s:", self->parameter->label);
    int i;
    for(i=0; i<ratio; ++i) {
        UART_printf("-");
    }

    //Some space after the slider drawing if the ratio is too small.. 
    for(i=0; i<MAX_NUM_SLIDER_BARS-ratio; ++i) {
        UART_printf(" ");
    }

    UART_printf(":");    
    UART_printf("%u", self->parameter->value);            

    UART_printf("     ");
   
    return ERR_NONE;
}

int freeSlider(Slider* self)
{
    return ERR_NONE;
}
