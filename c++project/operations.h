#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "data.h"
#include<stdbool.h>


void gameLoop(Room rooms[], Item items[], int startingRoomId);
/** DO NOT ADD CODE INSIDE THIS FILE ADD to operations.c **/

/** you may put more functions in here if necessary **/
void remove_char_from_end(char * str, char char_to_remove);

int extract_int(const char *str);

char *extract_string(const char *str);
#endif 

