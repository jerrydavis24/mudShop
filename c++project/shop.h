#ifndef SHOP_H
#define SHOP_H
#include "data.h"
#include <string>

using namespace std;

class ShopItem
{
private: // private means that these variables belong to the ShopItem object, but code outside the ShopItem class cannot directly access them
    int id;
    string name;
    int damage;
    int value;
    int shopIndex;
    ShopItem *next;
    ShopItem *prev;
public:
    //ShopItem represents a single node in the doubly linked list
    ShopItem() : id(0), name(""), damage(0), value(0) {
        next = nullptr;
        prev = nullptr;
    }
    ShopItem(const int& id, const string& name, const int& damage, const int& value, const int& shopIndex)
        : id(id), name(name), damage(damage), value(value), shopIndex(shopIndex) {
            next = nullptr;
            prev = nullptr;
        };

        //Getters
        int getShopIndex(){
            return shopIndex;
        }

        string getName(){
            return name;
        }
        int getId(){
            return id;
        }
        int getDamage(){
            return damage;
        }
        int getValue(){
            return value;
        }
        ShopItem * getNext(){
            return next;
        }
        ShopItem * getPrev(){
            return prev;
        }

        //Setters
        void setIndex(int indexNum){
            shopIndex= indexNum;
        }

        void setNext(ShopItem * itemN){//N for node
            next = itemN;
        }
        void setPrev(ShopItem * itemN){
            prev = itemN;
        }
};

class Shop
{
private:
    ShopItem* head;
    ShopItem* tail;
    int itemCount;

    void setHead(ShopItem* newHead){
        head = newHead;
    }
    void setTail(ShopItem* newTail){
        tail = newTail;
    }
    //private method to add a ShopItem (node)
    void addShopItem(const Item& item);

    //private method to list items
    ShopItem* listItems(ShopItem * start, const std::string &searchTerm) const;
public:
    Shop(Item* item_list, ItemType objtype);
    void enter(int inventory[], int& inventoryCount);
    ShopItem* buyItem(ShopItem* pageStart, int inventory[], int& inventoryCount);
    ShopItem* removeItem(ShopItem* removedItem);
};





#endif
