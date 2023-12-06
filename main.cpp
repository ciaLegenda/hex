#include <iostream>
#include "SFML\Graphics.hpp"

#define HEIGHT 800
#define WIDTH 800
#define RADIUS 25
#define TABLE_WIDTH 8
#define XORIGIN RADIUS
#define YORIGIN (HEIGHT/2-RADIUS)
#define SIN60 0.86602
#define COS60 0.5

#define EMPTY 0
#define WALL -1

class Table{
public:
    int contents[TABLE_WIDTH+2][TABLE_WIDTH+2];
    Table(){
        for(int i=0;i<=TABLE_WIDTH;i++)
            contents[0][i]=contents[TABLE_WIDTH+1][i]=contents[i][0]=contents[i][TABLE_WIDTH+1] = WALL;
    }
    int get_content(sf::Vector2<int> coord){
        return contents[coord.x][coord.y];
    }
    void set_content(sf::Vector2<int> coord,int value){
        contents[coord.x][coord.y] = value;
    }
    static sf::Vector2<float> get_xy_coord(sf::Vector2<int> table_coord){
        int i=table_coord.x, j = table_coord.y;
        sf::Vector2<float> xy_coord;
        xy_coord.x = XORIGIN + 2 * (i + j) * RADIUS * COS60;
        xy_coord.y = YORIGIN - 2 * (i - j) * RADIUS * SIN60;
        return xy_coord;
    }

    static void draw(sf::RenderWindow *window){
        sf::CircleShape hex(RADIUS, 6);
        hex.setFillColor(sf::Color(2,2,45));
        hex.setOutlineColor(sf::Color(100,100,200));
        hex.setOutlineThickness(2);
        for(int i=0;i<TABLE_WIDTH;i++){
            for(int j=0;j<TABLE_WIDTH;j++){
                hex.setPosition(get_xy_coord({i,j}));
                window->draw(hex);
            }
        }
    }

};

enum orientation {Horizontal = 0, Upper_diagonal = 1, Lower_diagonal = 2};

class Block{
public:
    int id;
    int length;
    sf::Vector2<int> coord;
    sf::Vector2<int> moveVector;

    Block(int id,int i,int j,int length,int orientation,Table *table){
        this->id = id;
        this->coord ={i,j};
        this->length = length;
        switch (orientation) {
            case Horizontal:
                this->moveVector = {1,1};
                break;
            case Upper_diagonal:
                this->moveVector = {1,0};
                break;
            default:
                this->moveVector = {0,1};
        }
        for(int k=0;k<length;k++)
            table->set_content(this->coord + k*this->moveVector,id);

    }
    void draw(sf::RenderWindow *window){
        sf::CircleShape hex(RADIUS,6);
        hex.setFillColor(sf::Color::Cyan);
        for(int k=0;k<length;k++){
            hex.setPosition(Table::get_xy_coord(this->coord + k*this->moveVector));
            window->draw(hex);
        }
    }
    bool is_leagal_move(int direction,Table table){
        sf::Vector2<int> cell_to_check =  coord;
        if(direction>0)
            cell_to_check += length*moveVector;
        else
            cell_to_check -= moveVector;
        return table.get_content(cell_to_check) == EMPTY;
    };
    void move(int direction,Table *table){
        if(direction>0){
            table->set_content(coord,EMPTY);
            table->set_content(coord + moveVector*length,id);
        }else{
            table->set_content(coord + moveVector*(length-1),EMPTY);
            table->set_content(coord-moveVector,id);
        }
        coord += moveVector*direction;
    };
};


int main(){
    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");
    Table table;
    Block b1(1,1,2,2,Horizontal,&table);
    Block b2(2,2,4,3,Lower_diagonal,&table);
    std::cout<<b2.is_leagal_move(-1,table);
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
        }
        // clear the window with black color
        window.clear(sf::Color::Black);

        Table::draw(&window);
        b1.draw(&window);
        b2.draw(&window);
        // end the current frame
        window.display();
    }

    return 0;
}