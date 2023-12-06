#ifndef HEX_BLOCK_H
#define HEX_BLOCK_H

#include "SFML/Graphics.hpp"
#include "Table.h"
using namespace sf;

enum orientation {Horizontal = 0, Upper_diagonal = 1, Lower_diagonal = 2};

class Block{
private:
    int id{};
    int length{};
    bool is_selected{};
    sf::Color color;
    sf::Vector2<int> coord;
    sf::Vector2<int> moveVector;
public:
    Block(){};
    Block(int id,Vector2<int> coord,Color color,int length,int orientation,Table *table);
    void draw(sf::RenderWindow *window);
    bool is_leagal_move(int direction,Table table);
    void move(int direction,Table *table);
};

#endif //HEX_BLOCK_H
