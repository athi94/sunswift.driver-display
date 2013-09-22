/** Coordinate.h - A coordinate on the LCD **/

#include <project/Coordinate.h>

/** Checks if the coordinates are within limits **/
int __init__Coordinate__(Coordinate* self, int x, int y)
{
    return setCoordinate(self, x, y);      
}

int setCoordinate(Coordinate* self, int x, int y)
{
    self->x = x;    
    self->y = y;     
    
    return ERR_NONE;
}

/** Change x **/
int changeX(Coordinate* self, int x)
{
    self->x=x;
    return ERR_NONE;
}

/** Change y **/
int changeY(Coordinate* self, int y)
{

    self->y = y;
    return ERR_NONE;
}

/** Increment x **/
int incrementX(Coordinate* self, int b)
{
    changeX(self, self->x+b);    
    return ERR_NONE; 
}

/** Increment y **/
int incrementY(Coordinate* self, int b)
{
    changeY(self, self->y+b);
    return ERR_NONE;
}


