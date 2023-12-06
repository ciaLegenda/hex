#ifndef HEX_TABLE_H
#define HEX_TABLE_H

#include "SFML\Graphics.hpp"
#include <cmath>

#define HEIGHT 800
#define WIDTH 800
#define RADIUS 25
#define TABLE_WIDTH 8
#define XORIGIN (-RADIUS)
#define YORIGIN (HEIGHT/2-RADIUS)
#define SIN60 0.86602
#define COS60 0.5

#define EMPTY 0
#define WALL (-1)

using namespace sf;

class Table{
    Vector2<int> centers_xy_coord[TABLE_WIDTH+1][TABLE_WIDTH+1];
public:
    int contents[TABLE_WIDTH+2][TABLE_WIDTH+2]{};

    Table();
    int get_content(sf::Vector2<int> coord);
    void set_content(sf::Vector2<int> coord,int value);
    static sf::Vector2<float> get_xy_coord(sf::Vector2<int> table_coord);
    Vector2<int> transform_to_table_coord(Vector2<int> window_coord);
    static void draw(sf::RenderWindow *window);

};

#endif //HEX_TABLE_H
