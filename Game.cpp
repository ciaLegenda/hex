#include "Game.h"
#include <random>
#include <time.h>
#include <iostream>

Game::Game(char* level_file){
    this->level_file = level_file;
    win_font.loadFromFile("boston_traffic.ttf");
    instruction_font.loadFromFile("SF_Cartoonist_Hand.ttf");
   // restart();
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
   // debug(window);
    if(state == game_complete)
        show_win_message(window);
    else
        //show_win_message(window);
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

    msg.setPosition(0.15*WIDTH,0);
    msg.setString("Lelvel");
    window->draw(msg);

    msg.move(WIDTH/2,0);
    msg.setString("completed");
    window->draw(msg);

    msg.setPosition(WIDTH/2.5,735);
    msg.setCharacterSize(HEIGHT/30);
    msg.setString("click to restart");
    window->draw(msg);
}

void Game::show_instructions(sf::RenderWindow *window) {
    Text msg;
    msg.setFont(instruction_font);
    msg.setCharacterSize(HEIGHT/15);

    msg.setPosition(WIDTH/10,725);
    msg.setString("Mission: Move the red block to the right!");
    window->draw(msg);
}

void Game::debug(sf::RenderWindow *window){
    Text msg;
    msg.setFont(instruction_font);
    msg.setCharacterSize(HEIGHT/30);

    char out[1000];
    sprintf(out,"mouse x: %3d,y: %3d  cell: i:%2d, j: %2d",mouse_coord.x,mouse_coord.y,coord.x,coord.y);

    msg.setPosition(20,20);
    msg.setString(out);
    window->draw(msg);
}

void Game::mouse_update(Vector2<int> mouse_coord,bool is_pressed){
    this->mouse_coord = mouse_coord;
    this->is_mouse_pressed = is_pressed;
    coord = table.transform_to_table_coord(mouse_coord);
    if(state == no_block_slected){
        //checking if we clicked on a block
        if(table.get_content(coord) > EMPTY && is_pressed) {
            //selecting the block
            id_of_selected_block = table.get_content(coord);
            b[id_of_selected_block].is_selected = true;
            state = block_selected;

        }
    }else if(state == block_selected){
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
        if(forward_move.is_at(coord) && forward_move.enabled)
            b[id_of_selected_block].move(1,&table);
        else if(backward_move.is_at(coord) && backward_move.enabled)
            b[id_of_selected_block].move(-1,&table);
        if(is_pressed)
            state = block_selected;
        else {
            state = no_block_slected;
            b[id_of_selected_block].is_selected = false;
            forward_move.enabled = backward_move.enabled = false;
        }
        if(is_complete())
            state = game_complete;
    }else if(state == game_complete){
        generate_random_level();
        state = no_block_slected;
    }
}

bool Game::is_complete(){
    Vector2<int> cell = b[0].coord + b[0].length*b[0].moveVector;
    return table.get_content(cell) == WALL;
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
    b.push_back(Block(0,{pos ,pos},Color(250,5,10), 3,Horizontal,&table));


    //place the other blocks such that they don't intersect the path of the red block
    for(int i=1;i<=40;i++){
        Vector2<int> cell;
        bool intesects_middle_row=true;
        int distance_to_intersection, good_dir,length;
        int iter=0;
        do{
            //find an empty cell and see in which direction is best to place the new block
            distance_to_intersection = 0;
            do {
                iter++;
                cell = table.get_rand_free_cell();
                distance_to_intersection = 0;
                intesects_middle_row = false;
                for (int dir = 0; dir < 3; dir++) {
                    //don't place horizontal blocks near the middle row
                    if(dir == 1 && cell.x-cell.y < 2 && cell.x-cell.y > -2)
                        continue;
                    Vector2<int> move = table.moves[dir];
                    int k = 1;
                    while (table.get_content(cell + k * move) == EMPTY)
                        k++;
                    if (distance_to_intersection < k) {
                        distance_to_intersection = k;
                        good_dir = dir;
                    }
                }
            }while (distance_to_intersection < 2 && iter<MAX_ITERATIONS);
            if(iter==MAX_ITERATIONS)
                break;
            distance_to_intersection = std::min(distance_to_intersection,5);//limiting the blocks length
            length = rand_between(2,distance_to_intersection);
            for(int k=0;k<length;k++)
                if((cell + k*table.moves[good_dir]).x == (cell + k*table.moves[good_dir]).y)
                    intesects_middle_row = true;
        }while(intesects_middle_row && iter<MAX_ITERATIONS);
        if(iter==MAX_ITERATIONS)
            break;
        no_of_blocks++;
        b.push_back(Block(i,cell,Color(rand()%150,150+rand()%100,rand()%250),length,good_dir,&table));
    }

    //shuflle the blocks around until the red block is blocked

    for(int i=0;i<100;i++){
        int index = rand_between(1,no_of_blocks);
        int dir = rand_between(0,1)? -1 : 1;
        int steps = rand_between(1,5);
        while(b[index].is_leagal_move(dir,table) && steps>0){
            b[index].move(dir, &table);
            steps--;
        }
    }

};