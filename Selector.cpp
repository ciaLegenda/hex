#include "Selector.h"

namespace sf {
    Selector::Selector() {
        enabled=false;
        icon = CircleShape(RADIUS/3,30);
    }
    void Selector::set_position(Vector2<int> table_coord) {
        coord = table_coord;
        Vector2<float> offset ={2*RADIUS/3,2*RADIUS/3};
        icon.setPosition(Table::get_xy_coord(table_coord)+offset);
    }
    void Selector::draw(sf::RenderWindow *window) {
        if(enabled)
            window->draw(icon);
    }

} // sf