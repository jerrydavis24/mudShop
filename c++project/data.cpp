#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operations.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;


int findMax(char *filename)// takes a filename as an argument
{
    FILE *file = NULL;
    char *line = NULL;
    size_t len = 0;
    int max_id = -1;

    file = fopen(filename, "r");//open the file that was passed 

    while ((getline(&line, &len, file) != -1))//keep reading one line at a time until we reach the end of the file
    {
        char *find_id;//creates a pointer that will eventually point to where "id": appears in the line

        if ((find_id = strstr(line, "\"id\": ")))//this searches the current line for "id":
        { 
            find_id += 6; // "id:" 2 +6 puts the pointer pointing at the integer, which is the id

            int cur_id = 0; //creates a variable to hold the ID found on this line

            if (sscanf(find_id, "%d", &cur_id) == 1)//this one is checking to see if we sucessfully parse an integer from find_id and pass it into cur_id
            {

                if (cur_id > max_id)//if the cur_id is greater than the max id then change the max id to the current id 
                {
                    max_id = cur_id;
                }
            }
        }
    }
    
    free(line);
    fclose(file);
    return max_id; //return the max id from the file
}

Room *load_json_rooms(char *filename, int roomMaxId)
{
    FILE *file = fopen(filename, "r");//open rooms.json to read
    int capacity = roomMaxId + 1;// this is going to be 361 even though roomMaxId is 359
    //printf("capacity=%d\n", capacity);
    Room *room_list = (Room *)malloc(capacity * sizeof(Room));//this is allocating memory for 361 rooms
    char *line = NULL;//this is for getline
    size_t len = 0;//also for getline
    int json_index = -1;//this is just declaring json_index
    char *value;//a string

    for (int i = 0; i < capacity; i++)//initializes the rooms values
    {
        room_list[i].id = -1;
        room_list[i].name = NULL;
        room_list[i].description = NULL;
        room_list[i].north = -1;
        room_list[i].south = -1;
        room_list[i].west = -1;
        room_list[i].east = -1;
        room_list[i].item = -1;
        room_list[i].starting = false;
    }

    while (getline(&line, &len, file) != -1)//as long as getline is reading in lines from rooms.json
    {
        if ((value = strstr(line, "\"id\": ")) != NULL)//this is an example of whats in value for id value= "id": 46,
        {
            //printf("value=%s", value);
            json_index = extract_int(value + strlen("\"id\": ")); // keep in mind that \ is how you put a quotation in a string, so we know what value is above then add 6 and our pointer is right where the id integer is 
            if (json_index >= capacity)
            {
                capacity = json_index + 1;
                room_list = (Room *)realloc(room_list, capacity * sizeof(Room));

                for (int i = roomMaxId + 1; i < capacity; i++)
                {
                    room_list[i].id = -1;
                    room_list[i].name = NULL;
                    room_list[i].description = NULL;
                    room_list[i].north = -1;
                    room_list[i].south = -1;
                    room_list[i].west = -1;
                    room_list[i].east = -1;
                    room_list[i].item = -1;
                    room_list[i].starting = false;
                }
            }

            //memset(&room_list[json_index], 0, sizeof(Room));//this line is actually redundant since we initialized everything earlier

            room_list[json_index].id = json_index;//keep in mind that index 0 on this is a room with the initialized values and essentially starts at index 1
        }
        else if ((value = strstr(line, "\"name\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                room_list[json_index].name = extract_string(value + strlen("\"name\": \""));// notice this is looking for "name": " so we start at the beggining of "name": " and add 9 which gets us to the first character of the name
            }
        }
        else if ((value = strstr(line, "\"description\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                room_list[json_index].description = extract_string(value + strlen("\"description\": \""));
            }
        }
        else if ((value = strstr(line, "\"north\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                room_list[json_index].north = extract_int(value + strlen("\"north\": "));
            }
        }
        else if ((value = strstr(line, "\"south\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                room_list[json_index].south = extract_int(value + strlen("\"south\": "));
            }
        }
        else if ((value = strstr(line, "\"west\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                room_list[json_index].west = extract_int(value + strlen("\"west\": "));
            }
        }
        else if ((value = strstr(line, "\"east\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                room_list[json_index].east = extract_int(value + strlen("\"east\": "));
            }
        }
        else if ((value = strstr(line, "\"item\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                room_list[json_index].item = extract_int(value + strlen("\"item\": "));
            }
        }
        else if ((value = strstr(line, "\"starting\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                room_list[json_index].starting = extract_int(value + strlen("\"starting\": ")) != 0;
            }
        }
    }//this while loop finishes once we run out of rooms in rooms.json
    free(line);
    fclose(file);
    return room_list;// room_list now holds all of the rooms
}

ItemType toItemType(const string &type)
{
    if (type == "general")
    {
        return ItemType::ITEM_TYPE_GENERAL;
    }
    else if (type == "quest")
    {
        return ItemType::ITEM_TYPE_QUEST;
    }
    else if (type == "potion")
    {
        return ItemType::ITEM_TYPE_POTION;
    }
    else if (type == "weapon")
    {
        return ItemType::ITEM_TYPE_WEAPON;
    }
    else if (type == "armor")
    {
        return ItemType::ITEM_TYPE_ARMOR;
    }
    else
    {
        return ItemType::ITEM_TYPE_NONE;
    }
}

Item *load_json_items(char *filename, int itemMaxId)
{
    FILE *file = fopen(filename, "r");
    int capacity = itemMaxId + 1;
    Item *item_list = (Item *)malloc(capacity * sizeof(Item));
    char *line = NULL;
    size_t len = 0;
    int json_index = -1;
    int lastValidItemId = -1;
    char *value;
    
    for (int i = 0; i < capacity; i++)
    {
        item_list[i].id = -1;
        item_list[i].name = NULL;
        item_list[i].description = NULL;
        item_list[i].last = false;
    }

    while (getline(&line, &len, file) != -1)
    {
        if ((value = strstr(line, "\"id\": ")) != NULL)
        {
            json_index = extract_int(value + strlen("\"id\": "));

            if (json_index >= capacity)
            {
                capacity = json_index + 1;
                item_list = (Item *)realloc(item_list, capacity * sizeof(Item));

                for (int i = itemMaxId + 1; i < capacity; i++)
                {
                    item_list[i].id = -1;
                    item_list[i].name = NULL;
                    item_list[i].description = NULL;
                }
            }

            //memset(&item_list[json_index], 0, sizeof(Item));
            item_list[json_index].id = json_index;
            lastValidItemId = json_index;
        }

        else if ((value = strstr(line, "\"name\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                item_list[json_index].name = extract_string(value + strlen("\"name\": \""));
            }
        }
        else if ((value = strstr(line, "\"description\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                item_list[json_index].description = extract_string(value + strlen("\"description\": \""));
            }
        }
        else if ((value = strstr(line, "\"value\": ")) != nullptr)
        {
            if (json_index >= 0)
            {
                item_list[json_index].value = extract_int(value + strlen("\"value\": "));
            }
        }
        else if ((value = strstr(line, "\"damage\": ")) != nullptr)
        {
            if (json_index >= 0)
            {
                item_list[json_index].damage = extract_int(value + strlen("\"damage\": "));
            }
        }
        else if ((value = strstr(line, "\"type\": ")) != nullptr)
        {
            if (json_index >= 0)
            {
                string type_str = extract_string(value + strlen("\"type\": \""));
                item_list[json_index].type = toItemType(type_str);
            }
        }
        

    }
        if (lastValidItemId >= 0) {
            item_list[lastValidItemId].last = true;
        }
    free(line);
    fclose(file);

    return item_list;
}
