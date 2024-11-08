#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "shop.h"

#define MAX_COMMAND_LENGTH 200
#define MAX_INVENTORY_SIZE 200

void remove_char_from_end(char *str, char char_to_remove)
{
    if (str[strlen(str) - 1] == char_to_remove)
    {
        str[strlen(str) - 1] = '\0';
    }
}

int extract_int(const char *str)
{
    int value = -1;
    sscanf(str, "%d", &value);
    return value;
}

char *extract_string(const char *str)
{
    // printf("%s", str);
    char *value = NULL;
    const char *start = str;
    if (start != NULL)
    {
        const char *end = strchr(start, '\"');
        if (end != NULL)
        {
            size_t len = end - start;
            value = (char*)malloc(len + 1);
            if (value != NULL)
            {
                strncpy(value, start, len);
                value[len] = '\0';
            }
        }
    }
    return value;
}

void gameLoop(Room rooms[], Item items[], int startingRoomId)
{
    char command[MAX_COMMAND_LENGTH];
    int currentRoomId = startingRoomId;
    int *inventory = (int*)malloc(MAX_INVENTORY_SIZE * sizeof(int));
    int inventorySize = 0;

    // initialize inventory
    memset(inventory, -1, MAX_INVENTORY_SIZE * sizeof(int));

    
    while (true)
    {
        // Print the current room details
        printf("%d : %s\n", rooms[currentRoomId].id, rooms[currentRoomId].name);
        printf("%s\n", rooms[currentRoomId].description);

        // print item if it exists in the room
        if (rooms[currentRoomId].item != -1)
        {
            Item currentItem = items[rooms[currentRoomId].item];
            printf("You see a %s\n", currentItem.name);
        }

        // Print available exits
        printf("[ ");
        int exitsPrinted = 0;

        if (rooms[currentRoomId].north != -1)
        {
            printf("[n]orth");
            exitsPrinted = 1;
        }
        if (rooms[currentRoomId].south != -1)
        {
            if (exitsPrinted)
                printf(", ");
            printf("[s]outh");
            exitsPrinted = 1;
        }
        if (rooms[currentRoomId].west != -1)
        {
            if (exitsPrinted)
                printf(", ");
            printf("[w]est");
            exitsPrinted = 1;
        }
        if (rooms[currentRoomId].east != -1)
        {
            if (exitsPrinted)
                printf(", ");
            printf("[e]ast");
        }

        printf(", [l]ook, [g]et, [i]nventory, [d]rop, [q]uit ]\n");
        printf("> ");

        // Read user command
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            printf("Error reading command.\n");
            continue; // Optionally handle errors or exit
        }

        // Remove newline character from input
        command[strcspn(command, "\n")] = '\0';

        // Handle user command
        if (strcmp(command, "n") == 0 && rooms[currentRoomId].north != -1)
        {
            currentRoomId = rooms[currentRoomId].north;
        }
        else if (strcmp(command, "s") == 0 && rooms[currentRoomId].south != -1)
        {
            currentRoomId = rooms[currentRoomId].south;
        }
        else if (strcmp(command, "w") == 0 && rooms[currentRoomId].west != -1)
        {
            currentRoomId = rooms[currentRoomId].west;
        }
        else if (strcmp(command, "e") == 0 && rooms[currentRoomId].east != -1)
        {
            currentRoomId = rooms[currentRoomId].east;
        }
        else if (strcmp(command, "l") == 0 && rooms[currentRoomId].item != -1)
        {
            Item currentItem = items[rooms[currentRoomId].item];
            printf("You see a %s: %s\n", currentItem.name, currentItem.description);
        }
        else if ((strcmp(command, "g") == 0 || strcmp(command, "get") == 0) && rooms[currentRoomId].item != -1)
        {
            // Add item to inventory
            if (inventorySize < MAX_INVENTORY_SIZE)
            {
                inventory[inventorySize++] = rooms[currentRoomId].item;
                printf("You picked up a %s and put it in your backpack\n", items[rooms[currentRoomId].item].name);
                rooms[currentRoomId].item = -1; // Remove item from the room)
            }
            else
            {
                printf("Your backpack is full.\n");
            }
        }
        else if (strcmp(command, "i") == 0)
        {
            if (inventorySize == 0)
            {
                printf("you currently have nothing in your backpack.\n");
            }
            else
            {
                printf("Your backpack of infinite holding contains:\n");
                for (int i = 0; i < inventorySize; i++)
                {
                    printf("+ [%d] %s\n", inventory[i], items[inventory[i]].name);
                }
            }
        }
        else if ((strcmp(command, "d") == 0) || strcmp(command, "drop") == 0)
        {
            if (inventorySize == 0)
            {
                printf("nothing in your backpack\n");
            }
            else
            { 
                printf("Your backpack of infinite holding contains:\n");
                for (int i = 0; i < inventorySize; i++){
                    printf("+ [%d] %s\n", inventory[i], items[inventory[i]].name);
                }
                printf("Enter the item ID to drop: ");
                int dropId;
                if (scanf("%d", &dropId) != 1)
                {
                    printf("Invalid input.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }
                int found = 0;
                for (int i = 0; i < inventorySize; i++)
                {
                    if (inventory[i] == dropId)
                    {
                        rooms[currentRoomId].item = dropId;
                        printf("You dropped the [%d] %s.\n", items[dropId].id, items[dropId].name);

                        for (int x = i; x < inventorySize - 1; x++)
                        {
                            inventory[x] = inventory[x + 1];
                        }
                        inventorySize -= 1;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                {
                    printf("Item not found in your backpack.\n");
                }
            }
            while (getchar() != '\n');
        }
        else if (strcmp(command, "sh") == 0 || strcmp(command, "shop") == 0){
            printf("Youre in the shop\n");
            Shop shop(items, ITEM_TYPE_WEAPON);
            shop.enter(inventory, inventorySize);
        }
        else if (strcmp(command, "q") == 0 || strcmp(command, "quit") == 0)
        {
            printf("Exiting game loop. Goodbye!\n");
            break; // Exit the game loop
        }
        else
        {
            printf("Invalid command or exit.\n");
        }
    }
    free(inventory);
}
