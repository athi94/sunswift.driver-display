/** Lcd.c - Lcd Functions to update **/

#include <project/Lcd.h>

int __init__Lcd__(Lcd* self,  int numberOfPages) 
{
    self->numberOfPages = numberOfPages;   
    self->currentPage = 0; 
    
    return ERR_NONE;    
}

int updateLcdScreen(Lcd* self) {
    printPage(&(self->pages[self->currentPage]));
   
    if(self->currentPage >= self->numberOfPages-1) { 
        self->currentPage = 0;         
    } else { 
        self->currentPage++;
    }
    
    return ERR_NONE;
}

int __free__Lcd__(Lcd* self) { 
    return ERR_NONE;
}

