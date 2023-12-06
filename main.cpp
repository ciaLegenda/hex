#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include "SFML\Graphics.hpp"
#include "Table.h"
#include "Block.h"
#include "Selector.h"

enum game_state{no_block_slected,block_selected,game_complete};


class Game{
    Table table;
    int no_of_blocks;
    std::vector<Block> b;
    int state = no_block_slected;
    int id_of_selected_block;
    Selector forward_move = Selector();
    Selector backward_move = Selector();
    Vector2<int> click_coord;
public:
    Game(char* level_file){
        table = Table();
        std::ifstream fin(level_file);
        fin>>no_of_blocks;
        b.reserve(no_of_blocks);
        //int id,Vector2<int> coord,Color color,int length,int orientation,Table *table;
        int i,j,colorR,colorG,colorB,length,orientation;
        for(int k=0;i<no_of_blocks;i++){
            fin>>i>>j>>colorR>>colorG>>colorB>>length>>orientation;
            b.push_back(Block(k,{i,j},Color(colorR,colorG,colorB),length,orientation,&table));
        }
    };
};

int main(){
    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");
    Table table = Table();
    std::vector<Block> b;
    int no_of_blocks;

    int state = no_block_slected;
    int id_of_selected_block;
    Selector forward_move = Selector();
    Selector backward_move = Selector();
    Vector2<int> click_coord;

    std::ifstream fin("level.in");
    fin>>no_of_blocks;
    b.reserve(no_of_blocks);
    //int id,Vector2<int> coord,Color color,int length,int orientation,Table *table;
    int i,j,colorR,colorG,colorB,length,orientation;
    b.emplace_back();
    for(int k=0;k<no_of_blocks;k++){
        fin>>i>>j>>colorR>>colorG>>colorB>>length>>orientation;
        b.push_back(Block(k+1,{i,j},Color(colorR,colorG,colorB),length,orientation,&table));
    }
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            //mouse interaction
            if(event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left){
                click_coord = Mouse::getPosition(window);
                Vector2 coord = table.transform_to_table_coord(click_coord);
                if(state == no_block_slected){
                    //checking if we clicked on a block
                    if(table.get_content(coord) > 0) {
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
                    state = no_block_slected;
                    b[id_of_selected_block].is_selected = false;
                    forward_move.enabled = backward_move.enabled = false;
                }
            }

        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        Table::draw(&window);
        for(auto itr: b)
            itr.draw(&window);
        forward_move.draw(&window);
        backward_move.draw(&window);
        // end the current frame
        window.display();
    }

    return 0;
}
