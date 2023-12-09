#ifndef HEX_BLOCK_H
#define HEX_BLOCK_H

#include "SFML/Graphics.hpp"
#include "Table.h"
using namespace sf;

enum orientation {Upper_diagonal,  Horizontal, Lower_diagonal};

class Block{
public:
    bool legal_placement;
    int id;
    int length;
    sf::Color color;
    sf::Vector2<int> coord;
    sf::Vector2<int> moveVector;
    bool is_selected;

    Block(){};
    Block(int id,Vector2<int> coord,Color color,int length,int orientation,Table *table);
    bool has_legal_placement(){return legal_placement;};
    //static bool intersects_middle_row();
    void draw(sf::RenderWindow *window);
    bool is_leagal_move(int direction,Table table);
    void move(int direction,Table *table);
};

#endif //HEX_BLOCK_H
