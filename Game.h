#ifndef HEX_GAME_H
#define HEX_GAME_H

#include "Table.h"
#include "Block.h"
#include "Selector.h"
#include <fstream>

#define MAX_ITERATIONS 10

namespace sf{
    enum game_state{no_block_slected,block_selected,game_complete};
    class Game{
    public:
        Table table;
        int no_of_blocks;
        std::vector<Block> b;
        int state = no_block_slected;
        int id_of_selected_block;
        Selector forward_move = Selector();
        Selector backward_move = Selector();
        Font win_font;
        Font instruction_font;
        char* level_file;

        Game(char* level_file);
        void restart();
        void generate_random_level();
        void click_update(Vector2<int> click_coord);
        bool is_complete();
        void draw(RenderWindow *window);
        void show_win_message(RenderWindow *window);
        void show_instructions(RenderWindow *window);
    };

}

#endif //HEX_GAME_H
