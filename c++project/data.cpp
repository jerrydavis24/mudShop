#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operations.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;


int findMax(char *filename)
{
    FILE *file = NULL;
    char *line = NULL;
    size_t len = 0;
    int max_id = -1;

    file = fopen(filename, "r");

    while ((getline(&line, &len, file) != -1))
    {
        char *find_id;

        if ((find_id = strstr(line, "\"id\": ")))
        {
            find_id += 6;

            int cur_id = 0;

            if (sscanf(find_id, "%d", &cur_id) == 1)
            {

                if (cur_id > max_id)
                {
                    max_id = cur_id;
                }
            }
        }
    }

    free(line);
    fclose(file);
    return max_id;
}

Room *load_json_rooms(char *filename, int roomMaxId)
{
    FILE *file = fopen(filename, "r");
    int capacity = roomMaxId + 1;
    Room *room_list = (Room *)malloc(capacity * sizeof(Room));
    char *line = NULL;
    size_t len = 0;
    int json_index = -1;
    char *value;

    for (int i = 0; i < capacity; i++)
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

    while (getline(&line, &len, file) != -1)
    {
        if ((value = strstr(line, "\"id\": ")) != NULL)
        {
            json_index = extract_int(value + strlen("\"id\": "));
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

            memset(&room_list[json_index], 0, sizeof(Room));

            room_list[json_index].id = json_index;
        }
        else if ((value = strstr(line, "\"name\": ")) != NULL)
        {
            if (json_index >= 0)
            {
                room_list[json_index].name = extract_string(value + strlen("\"name\": \""));
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
    }
    free(line);
    fclose(file);
    return room_list;
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

            memset(&item_list[json_index], 0, sizeof(Item));
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
