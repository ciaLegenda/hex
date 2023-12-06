//
// Created by Calin on 12/7/2023.
//

#ifndef HEX_SELECTOR_H
#define HEX_SELECTOR_H

#include "Table.h"
namespace sf {

    class Selector{
        Vector2<int> coord;
        sf::CircleShape icon;
    public:
        bool enabled;
        Selector();
        void set_position(Vector2<int> table_coord);
        void draw(sf::RenderWindow *window)
        bool is_at(Vector2<int> pos){
            return pos == coord;
        }
    };


} // sf

#endif //HEX_SELECTOR_H
