#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;


/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    
    ifstream inFS;
    inFS.open(fileName);
    char character;

    // returns nullptr if file cant be opened
    if (!inFS.is_open()) {
        cout << "Could not open file fileName.txt." << endl;
        return nullptr; // nullptr indicates error
    }

    // reads in the max row and max col
    if (!(inFS >> character))
    {return nullptr;}
    maxRow = (int)character - 48; 
    if (!(inFS >> character))
    {return nullptr;}
    maxCol = (int)character - 48;    

    cout << "maxrow:" << maxRow << endl;
    cout << "maxcol:" << maxCol << endl;

    //test first line for correctness
    if(maxRow < 1 || maxCol < 1)
        {return nullptr;}

    // reads the player position
    if (!(inFS >> character))
    {return nullptr;}
    player.row = (int)character - 48;  
    if (!(inFS >> character))
    {return nullptr;}
    player.col = (int)character - 48;    

    cout << "prow:" << player.row << endl;
    cout << "pcol:" << player.col << endl;

    //test if second line out of bounds
    if(player.row < 0 || player.row > maxRow){
        return nullptr;
    }
    if(player.col < 0 || player.col > maxCol){
        return nullptr;
    }
    

    // reads all of the characters in the map
    char** map = new char*[maxRow];
    for (int i = 0; i < maxRow; i++){
        map[i] = new char[maxCol];
    }
    int exit = 0;
    for(int i = 0; i < maxRow; i++){
        for(int j = 0; j < maxCol; j++){
            inFS >> character;
            if((character != TILE_OPEN ) &&  (character != TILE_TREASURE) &&  (character != TILE_MONSTER) &&  (character != TILE_AMULET) &&  (character != TILE_PILLAR) &&  (character != TILE_DOOR) &&  (character != TILE_EXIT) ){
                cout << "this is the character:" << character << endl;
                cout << "character is wrong" << endl;
                return nullptr;
            }
            map[i][j] = character;
            if (character == TILE_EXIT){
                exit++;
            }

        }
    }
    // check if exit exists
        if(exit == 0){
            return nullptr;
        }
    //check  if too many tiles
        if(!inFS.eof()){
            return nullptr;
        }
    inFS.close();

    map[player.row][player.col] = TILE_PLAYER;

//test cases
    double tiles = maxRow * maxCol;
    if(tiles > 2147483647){
        return nullptr;
    }
    // for(int i = 0; i < maxRow; i++){
    //     for (int j = 0; j < maxCol; j++){
    //         if(map[i][j] != TILE_OPEN && map[i][j] != TILE_PLAYER &&  map[i][j] != TILE_TREASURE &&  map[i][j] != TILE_MONSTER &&  map[i][j] != TILE_AMULET &&  map[i][j] != TILE_PILLAR &&  map[i][j] != TILE_DOOR &&  map[i][j] != TILE_EXIT ){
    //             return nullptr;
    //         }
    //     }
    // }
    return map;
            
}
        

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if (input == MOVE_UP){
        nextRow--;
    }
    if (input == MOVE_DOWN){
        nextRow++;
    }
    if (input == MOVE_LEFT){
        nextCol--;
    }
    if (input == MOVE_RIGHT){
        nextCol++;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    char** ary = nullptr;
    for(int i = 0; i < maxRow; i++){
        for(int j = 0; j < maxCol; j++){
            ary[i][j] = TILE_OPEN;
        }
    }
    return ary;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    for (int i = 0; i < maxRow; i++) {
        delete[] map[i];
    }
    delete[] map;

    maxRow = 0;
    map = nullptr;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    

    int row = maxRow;
    int col = maxCol;

    //maxRow < 1 || maxRow == nullptr || maxCol < 1 || maxCol == nullptr
    if(maxRow < 1 || maxCol < 1){
        return nullptr;
    }
    if(!map){
        return nullptr;
    }
    long int check = maxRow * maxCol;
    if((check) > INT32_MAX){
        return nullptr;
    }

    maxRow *= 2;
    maxCol *= 2;
    char** newMap = new char*[maxRow];
    for (int i = 0; i < maxRow; i++){
        newMap[i] = new char[maxCol];
    }
    
    //Top left
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            newMap[i][j] = map[i][j];
        }
    }

    //Top right
    for(int i = 0; i < row; i++){
        for(int j = col; j < maxCol; j++){
            if(map[i][j - col] == TILE_PLAYER){
                newMap[i][j] = TILE_OPEN;
            }
            else{
                newMap[i][j] = map[i][j - col];
            }
        }
    }

    //Bottom left
    for(int i = row; i < maxRow; i++){
        for(int j = 0; j < col; j++){
            if(map[i - row][j] == TILE_PLAYER){
                newMap[i][j] = TILE_OPEN;
            }
            else{
                newMap[i][j] = map[i - row][j];
            }
        }
    }

    //Bottom right
    for(int i = row; i < maxRow; i++){
        for(int j = col; j < maxCol; j++){
            if(map[i - row][j - col] == TILE_PLAYER){
                newMap[i][j] = TILE_OPEN;
            }
            else{
                newMap[i][j] = map[i - row][j - col];
            }
        }
    }

    deleteMap(map, row);
    return newMap;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    if(nextRow >= maxRow || nextRow < 0){
        return STATUS_STAY;
    }
    else if(nextCol >= maxCol || nextCol < 0){
        return STATUS_STAY;
    }
    else if(map[nextRow][nextCol] == TILE_PILLAR){
        return STATUS_STAY;
    }
    else if(map[nextRow][nextCol] == TILE_MONSTER){
        return STATUS_STAY;
    }
    else if(map[nextRow][nextCol] == TILE_EXIT && player.treasure == 0){
        return STATUS_STAY;
    }
    else if(map[nextRow][nextCol] == TILE_TREASURE){
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.treasure += 1;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_TREASURE;
    }
    else if(map[nextRow][nextCol] == TILE_EXIT && player.treasure > 0){
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_ESCAPE;
    }
    else if(map[nextRow][nextCol] == TILE_AMULET){
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_AMULET;
    }
    else if(map[nextRow][nextCol] == TILE_DOOR){
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_LEAVE;
    }
    else{
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_MOVE;
    }
    return 0;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {

    // int row = player.row;
    // int col = player.col;

    // int numMonster;
    // int mrow;
    // int mcol;
    // int mcount = 0;

    // // gets number of monsters on the map
    // for(int i = 0; i < maxRow; i++){
    //     for (int j = 0; j < maxCol; j++){
    //         if(map[i][j] = TILE_MONSTER){
    //             numMonster++;
    //         }
    //     }
    // }
    // //creates a 2d array monster data storing the row and col for each monster
    // char** mdata = new char*[numMonster];
    // for (int i = 0; i < numMonster; i++){
    //         mdata[i] = new char[2];
    // }

    // //adds the rows and col for all of the monsters
    // for(int i = 0; i < maxRow; i++){
    //     for (int j = 0; j < maxCol; j++){
    //         if(map[i][j] = TILE_MONSTER){
    //             mdata[mcount][0] = i;
    //             mdata[mcount][1] = j;
    //             mcount++;
    //         }
    //     }
    // }
    // // checking if monster is right next to player, moves monster if there is
    // for(int i = 0; i < numMonster; i++){
    //     if((mdata[i][0] == player.row - 1) && (mdata[i][1] == player.col)){
    //         map[mdata[i][0]][mdata[i][1]] = TILE_OPEN;
    //         map[player.row][player.col] = TILE_MONSTER;
    //     }
    //     if((mdata[i][0] == player.row + 1) && (mdata[i][1] == player.col)){
    //         map[mdata[i][0]][mdata[i][1]] = TILE_OPEN;
    //         map[player.row][player.col] = TILE_MONSTER;
    //     }
    //     if((mdata[i][0] == player.row) && (mdata[i][1] == player.col - 1)){
    //         map[mdata[i][0]][mdata[i][1]] = TILE_OPEN;
    //         map[player.row][player.col] = TILE_MONSTER;
    //     }
    //     if((mdata[i][0] == player.row + 1) && (mdata[i][1] == player.col + 1)){
    //         map[mdata[i][0]][mdata[i][1]] = TILE_OPEN;
    //         map[player.row][player.col] = TILE_MONSTER;
    //     }
    // }

    // //checking if monster is within line of sight
    // //checking for each monster
    // for(int i = 0; i < numMonster; i++){
    //     //if a monster is in the same row 
    //     if( mdata[i][0] == player.row){

    //         //if monster is right and in the same row
    //         if(player.col > mdata[i][1]){ 

    //             //checking for barrier
    //             for(int j = mdata[i][1] + 1; j < player.col ; j++ ){ //j = current col, looks between mcol to pcol 
    //                 if(map[player.row][j] == TILE_PILLAR){
    //                     break;
    //                 }  
    //                 map[player.row][mdata[i][1]] = TILE_OPEN;
    //                 map[player.row][mdata[i][1] + 1] = TILE_MONSTER;
    //             }
    //         }

    //         // if monster is left and same row
    //         if(player.col < mdata[i][1]){ 

    //             //checking for barrier
    //             for(int j = mdata[i][1]; j < player.col ; j-- ){ //j = current col, looks between 
    //                 if(map[player.row][j] == TILE_PILLAR){
    //                     break;
    //                 } 
    //                 map[player.row][mdata[i][1]] = TILE_OPEN;
    //                 map[player.row][mdata[i][1] - 1] = TILE_MONSTER;
                    
    //             }
    //         }


    //     }
    //     // if monster is same column
    //     if( mdata[i][1] == player.col){

    //         //if monster is down and in the same column
    //         if(player.row > mdata[i][0]){ 

    //             //checking for barrier
    //             for(int j = mdata[i][0] + 1; j < player.row ; j++ ){ //j = current row, looks between mrow to prow 
    //                 if(map[j][player.col] == TILE_PILLAR){
    //                     break;
    //                 }  
    //                 map[mdata[i][0]][mdata[i][1]] = TILE_OPEN;
    //                 map[mdata[i][0]][mdata[i][1] + 1] = TILE_MONSTER;
    //             }
    //         }
    //         // if monster is down and same column
    //         if(player.row < mdata[i][0]){ 

    //             //checking for barrier
    //             for(int j = mdata[i][0] + 1; j < player.row ; j++ ){ //j = current row, looks between mrow to prow 
    //                 if(map[j][player.col] == TILE_PILLAR){
    //                     break;
    //                 }  
    //                 map[mdata[i][0]][mdata[i][1]] = TILE_OPEN;
    //                 map[mdata[i][0]][mdata[i][1] + 1] = TILE_MONSTER;
    //             }
    //         }
    //     }
    // }           


    int row = player.row;
    int col = player.col;

    if(player.row > 0){
        if(map[player.row-1][player.col] == TILE_MONSTER){
            map[player.row-1][player.col] = TILE_OPEN;
            map[player.row][player.col] = TILE_MONSTER;
        }
    }
    if(player.row < maxRow-1){
        if(map[player.row+1][player.col] == TILE_MONSTER){
            map[player.row+1][player.col] = TILE_OPEN;
            map[player.row][player.col] = TILE_MONSTER;
        }
    }
    if(player.col > 0){
        if(map[player.row][player.col-1] == TILE_MONSTER){
            map[player.row][player.col-1] = TILE_OPEN;
            map[player.row][player.col] = TILE_MONSTER;
        }
    }
    if(player.col < maxCol-1){
        if(map[player.row][player.col+1] == TILE_MONSTER){
            map[player.row][player.col+1] = TILE_OPEN;
            map[player.row][player.col] = TILE_MONSTER;
        }
    }

    //Up
    for(int i = player.row-1; i >= 0; i--){
        if(map[i][player.col] == TILE_PILLAR){
            break;
        }
        if(map[i][player.col] == TILE_MONSTER){
            map[i][player.col] = TILE_OPEN;
            map[i+1][player.col] = TILE_MONSTER;
        }        
    }

    //Down
    for(int i = player.row+1; i < maxRow; i++){
        if(map[i][player.col] == TILE_PILLAR){
            break;
        }
        if(map[i][player.col] == TILE_MONSTER){
            map[i][player.col] = TILE_OPEN;
            map[i-1][player.col] = TILE_MONSTER;
        }        
    }

    //Left
    for(int i = player.col-1; i >= 0; i--){
        if(map[player.row][i] == TILE_PILLAR){
            break;
        }
        if(map[player.row][i] == TILE_MONSTER){
            map[player.row][i] = TILE_OPEN;
            map[player.row][i+1] = TILE_MONSTER;
        }        
    }

    //Right
    for(int i = player.col+1; i < maxCol; i++){
        if(map[player.row][i] == TILE_PILLAR){
            break;
        }
        if(map[player.row][i] == TILE_MONSTER){
            map[player.row][i] = TILE_OPEN;
            map[player.row][i-1] = TILE_MONSTER;
        }        
    }

    if(map[row][col] == TILE_MONSTER){
        return true;
    }

    return false;
}
