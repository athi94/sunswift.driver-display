#ifndef __DATASOURCE_H__
#define __DATASOURCE_H__

#include <project/LcdConstants.h>

typedef struct {
    int source; 
    int channel;
    unsigned int value;
    unsigned int time;    
} DataSource;

int __init__DataSource__(DataSource* self, int source, int channel, unsigned int value, unsigned int time) ;


#endif
