# MudShop

MudShop is a command-line C/C++ adventure game that loads room and item
information from JSON files. The player can navigate between rooms, collect
and drop items, manage an inventory, and interact with a shop.

I worked on this project to practice C/C++ concepts including file handling,
pointers, dynamic memory, classes, arrays, string manipulation, and doubly
linked lists.

## Project Structure

### main.cpp

`main.cpp` sets up the program and constructs the paths to `rooms.json` and
`items.json`.

This demonstrates how relative paths such as:

./rooms.json
./items.json

can be constructed and passed to functions that need to access files.

The program uses `findMax()` to determine the largest ID contained in each
data file before creating the room and item arrays.

### data.h / data.cpp

These files handle loading and extracting data from the JSON files.

`data.h` defines the `Room` and `Item` structures used by the program.

`data.cpp` contains functions such as `load_json_rooms()` and
`load_json_items()`, which read the JSON data and populate arrays of rooms
and items that the rest of the program can manipulate.

This part of the project demonstrates:

- File I/O
- Parsing data
- C strings
- `sscanf()`
- Dynamic memory allocation
- Converting file data into usable program data

### operations.cpp

`operations.cpp` contains the main game loop.

The game loop handles:

- Displaying room information
- Player commands
- Navigation between rooms
- Picking up items
- Dropping items
- Inventory management
- Entering the shop

### shop.h / shop.cpp

These files implement the shop system.

The shop uses a doubly linked list of `ShopItem` objects. Each node contains
item information as well as pointers to the previous and next nodes.

The shop demonstrates:

- Classes
- Pointers
- Doubly linked lists
- Traversing forward and backward
- Searching/filtering items
- Buying and removing items from the list
- Pagination

## Building the Project

Compile the project with:

make

Then run:

./main.bin
