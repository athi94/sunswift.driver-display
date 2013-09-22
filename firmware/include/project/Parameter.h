/** Parameter.h - Create parameter - e.g. Voltage, Current **/

#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include <string.h>
#include <project/LcdConstants.h>
#include <project/DataSource.h>
#include <project/Parameter.h>
#include <project/driver_config.h>

typedef struct Parameter_t Parameter; 

struct Parameter_t
{    
    DataSource* src1;
    DataSource* src2;       
    unsigned int value; 
    unsigned int time;
    char name[50]; 
    char label[LCD_LABEL_LENGTH+1]; //Must be 2 characters or less
    char unit[LCD_LABEL_LENGTH+1];  //Must be 2 characters         
    Parameter* operand;            //operand is the operation
    int operator;                  //1 for multiplication, 2 for addition, 0 for nothing
};

int __init__Parameter__(Parameter* self, DataSource* src1, DataSource* src2, unsigned int value, unsigned int time, 
                        char* name, char* label, char* unit, Parameter* operand, int operator);

int updateParameter(Parameter* self);                      

void performOperation(Parameter* self);

int __free__Parameter__(Parameter* self);

#endif