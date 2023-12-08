#include "Game.h"
#include <random>
#include <time.h>
#include <iostream>

Game::Game(char* level_file){
    this->level_file = level_file;
    win_font.loadFromFile("boston_traffic.ttf");
    instruction_font.loadFromFile("SF_Cartoonist_Hand.ttf");
    //restart();
    generate_random_level();
};

void Game::restart() {
    table = Table();
    std::ifstream fin(level_file);
    fin>>no_of_blocks;
    b.resize(no_of_blocks);
    int i,j,colorR,colorG,colorB,length,orientation;
    for(int k=0;k<no_of_blocks;k++){
        fin>>i>>j>>colorR>>colorG>>colorB>>length>>orientation;
        b[k] = Block(k,{i,j},Color(colorR,colorG,colorB),length,orientation,&table);
    }
    fin.close();
}

void Game::draw(RenderWindow *window){
    if(state == game_complete)
        show_win_message(window);
    else
        show_instructions(window);
    Table::draw(window);
    for(auto itr: b)
        itr.draw(window);
    forward_move.draw(window);
    backward_move.draw(window);
}

void Game::show_win_message(sf::RenderWindow *window) {
    Text msg;
    msg.setFont(win_font);
    msg.setCharacterSize(HEIGHT/15);

    msg.setPosition(0.6*WIDTH,0.3*HEIGHT);
    msg.setString("Lelvel");
    window->draw(msg);

    msg.move(0,HEIGHT/15);
    msg.setString("completed");
    window->draw(msg);

    msg.move(0,HEIGHT/9);
    msg.setCharacterSize(HEIGHT/30);
    msg.setString("click to restart");
    window->draw(msg);
}

void Game::show_instructions(sf::RenderWindow *window) {
    Text msg;
    msg.setFont(instruction_font);
    msg.setCharacterSize(HEIGHT/15);

    msg.setPosition(WIDTH/2,HEIGHT/4);
    msg.setString("Mission:");
    window->draw(msg);

    msg.move(50,HEIGHT/15);
    msg.setString("Free the red block!");
    window->draw(msg);
}

void Game::click_update(Vector2<int> click_coord){
    Vector2 coord = table.transform_to_table_coord(click_coord);
    if(state == no_block_slected){
        //checking if we clicked on a block
        if(table.get_content(coord) > EMPTY) {
            //selecting the block
            id_of_selected_block = table.get_content(coord);
            b[id_of_selected_block].is_selected = true;
            state = block_selected;
            //marking the posibble move directions
            Block current_block = b[id_of_selected_block];
            forward_move.enabled=backward_move.enabled = false;
            if(current_block.is_leagal_move(1,table)) {
                forward_move.set_position(
                        current_block.coord + current_block.moveVector * current_block.length);
                forward_move.enabled = true;
            }
            if(current_block.is_leagal_move(-1,table)) {
                backward_move.set_position(current_block.coord - current_block.moveVector);
                backward_move.enabled = true;
            }
        }
    }else if(state == block_selected){
        if(forward_move.is_at(coord) && forward_move.enabled)
            b[id_of_selected_block].move(1,&table);
        else if(backward_move.is_at(coord) && backward_move.enabled)
            b[id_of_selected_block].move(-1,&table);
        b[id_of_selected_block].is_selected = false;
        forward_move.enabled = backward_move.enabled = false;
        if(is_complete())
            state = game_complete;
        else
            state = no_block_slected;
    }else{
        generate_random_level();
        state = no_block_slected;
    }
}

bool Game::is_complete(){
    Vector2<int> cell = b[0].coord + b[0].length*b[0].moveVector;
    while(table.get_content(cell) != WALL) {
        if(table.get_content(cell) != EMPTY)
            return false;
        cell += b[0].moveVector;
    }
    return true;
}

int rand_between(int min,int max){
    return rand()%(max-min+1)+min;
}

void Game::generate_random_level(){
    std::srand(time(nullptr));
    table = Table();
    b = std::vector<Block>();
    no_of_blocks = 1;
    //place red block
    int pos = rand_between(1,3);
    b.push_back(Block(0,{pos ,pos},Color(200,10,10), 3,Horizontal,&table));
    //place a few blocks such that they block the red block
    //for(int i=3)
    //place the other blocks such that they don't intersect the path of the red block
    int placing_tentatives=0;
    for(int i=0;i<20 && placing_tentatives<MAX_ITERATIONS;i++){
        placing_tentatives=0;
        Block aux;
        do {
            placing_tentatives++;
            Vector2<int> coord = {rand_between(1, TABLE_WIDTH-1), rand_between(1, 8)};
            Color color = Color(rand_between(50,150), rand_between(50,250), rand_between(50,200));
            int length = rand_between(2,5);
            int orientation;
            if(coord.x - coord.y == 1 || coord.y - coord.x == 1)
                orientation = rand_between(0,1)? Upper_diagonal:Lower_diagonal;
            else
                orientation = rand_between(0,2);
            aux = Block(no_of_blocks,coord,color,length, orientation,&table);

        }while(!aux.has_legal_placement() && placing_tentatives<MAX_ITERATIONS);
        b.push_back(aux);
        no_of_blocks++;
    }
    //shuflle the blocks around until the red block is blocked
    /*
    for(int i=0;i<MAX_ITERATIONS;i++){
        int index = rand_between(1,no_of_blocks);
        int dir = rand_between(0,1)? -1 : 1;
        int steps = rand_between(1,5);
        while(b[index].is_leagal_move(dir,table) && steps>0){
            b[index].move(dir, &table);
            steps--;
        }
    }
     */
};