#ifndef HEX_TABLE_H
#define HEX_TABLE_H

#include "SFML\Graphics.hpp"
#include "Window size.h"
#include <cmath>

#define RADIUS 25
#define TABLE_SIDE 8
#define XORIGIN (-RADIUS)
#define YORIGIN (HEIGHT/2-RADIUS)
#define SIN60 0.86602
#define COS60 0.5

#define EMPTY (-1)
#define WALL (-2)

using namespace sf;

class Table{
    Vector2<int> centers_xy_coord[2*TABLE_SIDE+1][2*TABLE_SIDE+1];
    int contents[2*TABLE_SIDE+2][2*TABLE_SIDE+2];
public:
    Vector2<int> moves[6]={{1,0},{1,1},{0,1},{-1,0},{0,-1},{-1,-1}};
    Table();
    int get_content(sf::Vector2<int> coord);
    void set_content(sf::Vector2<int> coord,int value);
    static sf::Vector2<float> get_xy_coord(sf::Vector2<int> table_coord);
    Vector2<int> transform_to_table_coord(Vector2<int> window_coord);
    static void draw(sf::RenderWindow *window);
    Vector2<int> get_rand_free_cell();
};

#endif //HEX_TABLE_H
