#include <scandal/stdio.h>
#include <project/Parameter.h>

int __init__Parameter__(Parameter* self, DataSource* src1, DataSource* src2, unsigned int value, 
                        unsigned int time, char* name, char* label, char* unit, Parameter* operand, int operator)
{
    self->src1 = src1;
    self->src2 = src2;    
    self->value = value; 
    self->time = time;
    strcpy(self->name, name);
    strcpy(self->label, label);
    strcpy(self->unit, unit);        
    self->operand = operand; 
    self->operator = operator; 
    return ERR_NONE;
}

int updateParameter(Parameter* self) {
    if(self->src2  != NULL) 
    {        
        if((self->src2->time > self->src1->time) && (self->src2->time - self->src1->time > LCD_SOURCE_CHOICE_TIMEOUT)) {         
            self->value = self->src2->value; 
            self->time  = self->src2->time;            
        } else {
            self->value = self->src1->value; 
            self->time  = self->src1->time;
        }    
    } else {       
        self->value = self->src1->value; 
        self->time  = self->src1->time;  
    }
    
    performOperation(self);
    
    return ERR_NONE;
}
                        
int __free__Parameter__(Parameter* self) { 
    return ERR_NONE;
}

void performOperation(Parameter* self) { 
    switch(self->operator) { 
        //Multiplication
        case 1: self->value = self->value * self->operand->value;        
            break;
        //Addition
        case 2: self->value = self->value + self->operand->value;
            break;
        //Division
        case 3: self->value = self->value / self->operand->value; 
            break;
        //Nothing
        default: self->value = self->value;
    }
}
