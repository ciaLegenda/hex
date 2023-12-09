#include "Game.h"
#include "Table.h"
#include <iostream>

int main(){
    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");
    window.setFramerateLimit(30);
    Game game("level.in");
    Vector2<int> x = {-1,1};
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

            Vector2<int> mouse_coord = Mouse::getPosition(window);
            bool is_pressed = Mouse::isButtonPressed(Mouse::Left);
            game.mouse_update(mouse_coord,is_pressed);

        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        game.draw(&window);

        // end the current frame
        window.display();
    }

    return 0;
}
