#include "Game.h"
#include "Table.h"

int main(){
    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");
    window.setFramerateLimit(30);
    Game game("level.in");
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
                Vector2<int>click_coord = Mouse::getPosition(window);
                game.click_update(click_coord);
            }

        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        game.draw(&window);

        // end the current frame
        window.display();
    }

    return 0;
}
