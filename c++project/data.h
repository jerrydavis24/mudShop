#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <map>

using namespace std;




typedef struct Room
{
    int id;
    char *name;
    char *description;
    int north;
    int south;
    int west;
    int east;
    int item;
    bool starting;
} Room;

typedef enum ItemType
{
    ITEM_TYPE_NONE = -1,
    ITEM_TYPE_GENERAL = 0,
    ITEM_TYPE_QUEST = 1,
    ITEM_TYPE_POTION = 2,
    ITEM_TYPE_WEAPON = 3,
    ITEM_TYPE_ARMOR = 4
} ItemType;

typedef struct Item
{
    int id;
    char *name;
    char *description;
    int value;     // the value of the item
    int damage;    // damage the item does when used as a weapon
    ItemType type; // the type of item, general, quest, potion, weapon, armor
    bool last;     // marks the last item of the items list

} Item;



int findMax(char *filename);
/** DO NOT ADD CODE INSIDE THIS FILE ADD to data.c **/

Room *load_json_rooms(char *filename, int roomMaxId);
/** DO NOT ADD CODE INSIDE THIS FILE ADD to data.c **/

Item *load_json_items(char *filename, int itemMaxId);
/** DO NOT ADD CODE INSIDE THIS FILE ADD to data.c **/

ItemType toItemType(const string& type);

#endif

