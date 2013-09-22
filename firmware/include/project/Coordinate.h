/** Coordinate.h - A coordinate on the LCD **/

#ifndef __COORDINATE_H__
#define __COORDINATE_H__

#include <project/LcdConstants.h>

typedef struct { 
    int x; 
    int y;     
} Coordinate;

/** Checks if the coordinates are within limits **/
int __init__Coordinate__(Coordinate* self, int x, int y); 

/** set coordinate directly */
int setCoordinate(Coordinate* self, int x, int y);

/** Change x **/
int changeX(Coordinate* self, int x); 

/** Change y **/
int changeY(Coordinate* self, int y);

/** Increment x **/
int incrementX(Coordinate* self, int b);

/** Increment y **/
int incrementY(Coordinate* self, int b);

int __free__Coordinate__(Coordinate* self);

#endif
