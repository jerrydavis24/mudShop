#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "operations.h"
#include "shop.h"


#define MAX_PATH_LENGTH 500

int main(int argc, char *argv[])
{
    char roomsPath[MAX_PATH_LENGTH];
   char itemsPath[MAX_PATH_LENGTH];

   roomsPath[0] = '\0';
   itemsPath[0] = '\0';

   const char *dir;
   if (argc > 1) {
    dir = argv[1];
   } else {
    dir = ".";
   }

   strcpy(roomsPath, dir);
   strcpy(itemsPath, dir);

   int dirLen = strlen(dir);
   if(dirLen > 0 && dir[dirLen - 1] != '/') {
    strcat(roomsPath, "/");
    strcat(itemsPath, "/");
   }

   strcat(roomsPath, "rooms.json");
   strcat(itemsPath, "items.json");

   //printf("roomsJson path: %s\n", roomsPath);
   //printf("itemsJson path: %s\n", itemsPath);
   
   int roomMax = findMax(roomsPath);
   int itemMax = findMax(itemsPath);
   

    // Find the maximum IDs
   

    if (roomMax < 0 || itemMax < 0) {
        printf("Error determining maximum room or item ID.\n");
        return 1;
    }

    printf("roomMaxId=%d\n", roomMax);
    printf("itemMaxId=%d\n", itemMax);

    // Load rooms and items
    Room *rooms_list = load_json_rooms(roomsPath, roomMax + 1);
    Item *items_list = load_json_items(itemsPath, itemMax + 1);

    if (rooms_list == NULL || items_list == NULL) {
        printf("Error loading rooms or items.\n");
        return 1;
    }

    
    printf("%d %s\n", rooms_list[5].id, rooms_list[5].name);
    printf("%d %s\n", rooms_list[359].id, rooms_list[359].name);
    printf("%d %s %d %d %d %d\n", items_list[0].id, items_list[0].name, items_list[0].value, items_list[0].damage, static_cast<int>(items_list[0].type), items_list[0].last);
    printf("%d %s %d %d %d %d\n", items_list[378].id, items_list[378].name,items_list[378].value, items_list[378].damage, static_cast<int>(items_list[378].type), items_list[378].last);
    printf("%d %s %d %d %d %d\n", items_list[1697].id, items_list[1697].name, items_list[1697].value, items_list[1697].damage, static_cast<int>(items_list[1697].type), items_list[1697].last);
    
    
    // Start the game loop
    gameLoop(rooms_list, items_list, 1);

    // Clean up allocated memory
    for (int i = 0; i <= roomMax; i++) {
        if (rooms_list[i].id != -1) {
            free(rooms_list[i].name);
            free(rooms_list[i].description);
        }
    }
    for (int i = 0; i <= itemMax; i++) {
        if (items_list[i].id != -1) {
            free(items_list[i].name);
            free(items_list[i].description);
        }
    }
    free(rooms_list);
    free(items_list);

    return 0;
}
