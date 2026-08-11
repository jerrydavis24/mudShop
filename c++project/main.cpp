#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "operations.h"
#include "shop.h"


#define MAX_PATH_LENGTH 500

int main(int argc, char *argv[]) //argument count & argument vector ./mudShop data argc= 2 argv[0]= ./mudshop argv[1]= data
{
    char roomsPath[MAX_PATH_LENGTH]; //create a string of up to 500 characters
   char itemsPath[MAX_PATH_LENGTH]; //create a string of up to 500 characters

   roomsPath[0] = '\0'; //this makes the string start out as a valid empty string
   itemsPath[0] = '\0'; //this makes the string start out as a valid empty string

   //lines 20-37 setup ./rooms.json and ./items.json this works since we are running the program from the same directory containing rooms.json and items.json
   const char *dir; //the way this is setup I could for instance create a directory and add rooms.json into it and items.json into it and it would run the same
   if (argc > 1) {
    dir = argv[1];
   } else {
    dir = ".";
   }

   strcpy(roomsPath, dir);//if no directory argument is provided, dir defaults to "." representing the current working directory
   strcpy(itemsPath, dir);//if no directory argument is provided, dir defaults to "." representing the current working directory

   int dirLen = strlen(dir);//following the above assumptions dirLen = 1
   if(dirLen > 0 && dir[dirLen - 1] != '/') { //dirLen > 0 is true && dir[0] != '/' is also true so execute the body
    strcat(roomsPath, "/"); //concatanate / to roomsPath giving "./"
    strcat(itemsPath, "/"); //concatanate / to itemsPath giving "./"
   }

   strcat(roomsPath, "rooms.json");//finish the path
   strcat(itemsPath, "items.json");//finish the path
   
   int roomMax = findMax(roomsPath);//findMax is a function written in data.cpp that gives the maxId
   int itemMax = findMax(itemsPath);//findMax is a function written in data.cpp that gives the maxId
   

   
   

    if (roomMax < 0 || itemMax < 0) {//if roomMax or itemMax returns a negative it means that the findMax function failed, so we print an error message
        printf("Error determining maximum room or item ID.\n");
        return 1;
    }

    printf("roomMaxId=%d\n", roomMax);//assumming the above error is false print the maximum room id
    printf("itemMaxId=%d\n", itemMax);//assumming the above error is false print the maximum item id

    Room *rooms_list = load_json_rooms(roomsPath, roomMax + 1);//read rooms.json, create a dynamic array of Room structs, fill each Room with data from the file, and return a pointer to that array
    Item *items_list = load_json_items(itemsPath, itemMax + 1);//same idea for items.json: create and fill a dynamic array of Item structs, then return its pointer

    if (rooms_list == NULL || items_list == NULL) {//error message for if either of the json loads fail
        printf("Error loading rooms or items.\n");
        return 1;
    }

    //the below prints are sanity checks
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