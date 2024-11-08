#include "data.h"
#include "operations.h"
#include "shop.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <cctype>
#define MAX_INVENTORY_SIZE 5000

using namespace std;

std::string to_lower(const std::string &str)
{
    std::string lowerStr;
    for (char c : str)
    {
        lowerStr += std::tolower(static_cast<unsigned char>(c));
    }
    return lowerStr;
}

Shop::Shop(Item *item_list, ItemType objtype)
{
    int i;
    head = nullptr;
    tail = nullptr;
    // iterate and make sure we are not at the last
    for (i = 0; item_list[i].last != 1; i++)
    {
        // if the items type matches the objtype we are looking for...
        if (item_list[i].type == objtype)
        {

            // then call addShopItem with the items list index
            addShopItem(item_list[i]);
        }
        // if we are at the last item in the list then stop this
        if (item_list[i].last == true)
        {

            break;
        }
    }
    // edge case for the very last item in the case because the loop is checking if last is not true
    addShopItem(item_list[i]);
}

void Shop::addShopItem(const Item &item)
{
    // initializing the node with the values we care about
    ShopItem *newItem = new ShopItem(item.id, item.name, item.damage, item.value);
    if (head == nullptr)
    {
        // cout << "Setting head and tail to new item." << endl;

        setHead(newItem);
        setTail(newItem);
    }
    else
    {
        // cout << "Adding new item to the end of the list." << endl;

        tail->setNext(newItem);
        newItem->setPrev(tail);
        setTail(newItem);
    }
}

void Shop::enter(int inventory[], int &inventoryCount)
{
    char command[10];
    bool inShop = true;
    std::string searchTerm = "";
    ShopItem *currentPageStart = head; // the head of the list is initializied to the nextItemToShow
    ShopItem *currentListStart = nullptr;
    while (inShop)
    {
        currentListStart = currentPageStart;
        listItems(currentPageStart, searchTerm);

        printf("Press n to go forward, 'p' to go back, 'b' to buy, 's' to search, 'r' to reset list, or 'e' to exit: \n");

        if (fgets(command, sizeof(command), stdin) == nullptr)
        {
            printf("Error reading input.\n");
        }
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "b") == 0)
        {
            //printf("Before buying: head = %p, pageStart = %p\n", (void *)head, (void *)currentListStart);

            ShopItem *removedItem = buyItem(currentListStart, inventory, inventoryCount);
            if (removedItem == currentListStart)
            {
                currentPageStart = removedItem->getNext();
            }
            else
            {
                currentPageStart = currentListStart;
            }
            if (removedItem == head)
            {
                head = removedItem->getNext();
                if (head != nullptr)
                {
                    head->setPrev(nullptr);
                }
            }
        }

        else if (strcmp(command, "e") == 0)
        {
            printf("Exiting..... \n");
            inShop = false;
        }

        else if (strcmp(command, "n") == 0) //
        {
            if (currentPageStart != nullptr)
            {
                currentPageStart = listItems(currentPageStart, searchTerm);
            }
        }
        else if (strcmp(command, "p") == 0)
        {
            ShopItem *temp = currentPageStart;
            int cnt = 0;
            while (temp != nullptr && cnt < 10)
            {
                temp = temp->getPrev();
                if(temp != nullptr && (searchTerm.empty() || to_lower(temp->getName()).find(to_lower(searchTerm)) != std::string::npos))
                {
                    cnt++;
                }
            }
            if (temp == nullptr)
            {
                currentPageStart = head;
            }
            else
            {
                currentPageStart = temp;
            }
        }
        else if (strcmp(command, "s") == 0)
        {
            cout << "Enter search term: ";
            if (fgets(command, sizeof(command), stdin) != nullptr)
            {
                command[strcspn(command, "\n")] = '\0';
                searchTerm = command;
                currentPageStart = head;
            }
        }
    }
}

ShopItem *Shop::listItems(ShopItem *start, const std::string &searchTerm) const // we start this loop with the head bx nextItemToShow was initialized to head
{
    ShopItem *current = start;
    int itemNumber = 1;
    int itemsShown = 0;
    //printf("Listing items: starting from %p\n", (void *)start);

    cout << left << setw(7) << "Item#" << setw(40) << "Name" << setw(10) << "Damage" << setw(5) << "Value" << endl;
    cout << left << setw(7) << "-----" << setw(40) << "--------------------" << setw(10) << "-----" << setw(5) << "-----" << endl;

    std::string lowerSearchTerm = to_lower(searchTerm);
    // populates the first 10 items
    while (current != nullptr && itemsShown < 10)
    {
        std::string lowerItemName = to_lower(current->getName());
        if (lowerSearchTerm.empty() || lowerItemName.find(lowerSearchTerm) != std::string::npos)
        {
        cout << left << setw(7) << itemNumber
             << setw(40) << current->getName()
             << setw(10) << current->getDamage()
             << setw(5) << current->getValue() << endl;

        //printf("%-7d%-20p\n", itemNumber, (void *)current);

        itemNumber++;
        itemsShown++;
        }
        current = current->getNext();
    }
    //printf("End of page. Returning pointer: %p\n", (void *)current);

    return current;
}

ShopItem *Shop::buyItem(ShopItem *pageStart, int inventory[], int &inventoryCount)
{
    int shopItemNumber;
    printf("Enter the ShopItem number to buy: ");
    if (scanf("%d", &shopItemNumber) != 1)
    {
        printf("Invalid input.\n");
        return nullptr;
    }

    // Clear the input buffer
    while (getchar() != '\n')
    {
    }

    ShopItem *temp = pageStart;
    int currentShopItemNumber = 1;

    // Navigate to the desired item
    while (temp != nullptr && currentShopItemNumber < shopItemNumber)
    {
        temp = temp->getNext();
        currentShopItemNumber++;
    }

    // If the item number is invalid, return nullptr
    if (temp == nullptr)
    {
        printf("Invalid item number.\n");
        return nullptr;
    }

    // If there's space in the inventory, add the item and remove it from the shop
    if (inventoryCount < MAX_INVENTORY_SIZE)
    {
        inventory[inventoryCount++] = temp->getId();
        printf("You bought %d: %s\n", temp->getId(), temp->getName().c_str());

        ShopItem *removedItem = removeItem(temp);
        printf("Removed item: %d. New head: %p, pageStart: %p\n", removedItem->getId(), (void *)head, (void *)pageStart);
        printf("After removal, new pageStart: %p\n", (void *)pageStart);

        // Handle case when removing the head
        if (removedItem == head)
        {
            head = removedItem->getNext();
            if (head != nullptr)
            {
                head->setPrev(nullptr);
            }
            pageStart = head;
        }
        else if (removedItem == pageStart)
        {
            pageStart = removedItem->getNext();
        }
        printf("After removal, pageStart updated to: %p\n", (void *)pageStart);
        return removedItem;
    }

    printf("Inventory is full.\n");
    return nullptr;
}
ShopItem *Shop::removeItem(ShopItem *removedItem)
{
    if (removedItem == head)
    {
        head = removedItem->getNext();
        if (head != nullptr)
        {
            head->setPrev(nullptr);
        }
        else
        {
            tail = nullptr;
        }
    }
    else if (removedItem == tail)
    {
        tail = removedItem->getPrev();
        if (tail != nullptr)
        {
            tail->setNext(nullptr);
        }
    }

    else
    {
        ShopItem *prev = removedItem->getPrev();
        ShopItem *next = removedItem->getNext();
        if (prev != nullptr)
        {
            prev->setNext(next);
        }
        if (next != nullptr)
        {
            next->setPrev(prev);
        }
    }
    printf("Removed item: %d\n", removedItem->getId());
    printf("New head: %p\n", (void *)head);
    printf("New tail: %p\n", (void *)tail);

    return removedItem;
}

