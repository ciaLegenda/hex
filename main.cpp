#include <iostream>
#include <cmath>
#include "SFML\Graphics.hpp"

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
    static Vector2<int> centers_xy_coord[TABLE_WIDTH+1][TABLE_WIDTH+1];
public:
    int contents[TABLE_WIDTH+2][TABLE_WIDTH+2];
    Table(){
        for(int i=1;i<=TABLE_WIDTH;i++)
            for(int j=1;j<=TABLE_WIDTH;j++) {
                contents[i][j] = EMPTY;
               // centers_xy_coord[i][j].x = XORIGIN + 2 * (i + j) * RADIUS * COS60 + RADIUS * COS60;
               // centers_xy_coord[i][j].y = YORIGIN - 2 * (i - j) * RADIUS * SIN60 + 3 * RADIUS / 2;

            }
        for(int i=0;i<=TABLE_WIDTH;i++)
            contents[0][i]=contents[TABLE_WIDTH+1][i]=contents[i][0]=contents[i][TABLE_WIDTH+1] = WALL;
    }
    int get_content(sf::Vector2<int> coord){
        if(coord.y<0 ||coord.y>TABLE_WIDTH || coord.x<0 ||coord.x>TABLE_WIDTH)
            return WALL;
        return contents[coord.x][coord.y];
    }
    void set_content(sf::Vector2<int> coord,int value){
        contents[coord.x][coord.y] = value;
    }
    static sf::Vector2<float> get_xy_coord(sf::Vector2<int> table_coord){
        int i=table_coord.x, j = table_coord.y;
        sf::Vector2<float> xy_coord;
        //taking into consideration that indexing starts with 1
        xy_coord.x = XORIGIN + 2 * (i + j) * RADIUS * COS60;
        xy_coord.y = YORIGIN - 2 * (i - j) * RADIUS * SIN60;
        return xy_coord;
    }
    static Vector2<int> transform_to_table_coord(Vector2<int> window_coord){
        /*
        double dist_to_centers[TABLE_WIDTH+1][TABLE_WIDTH+1];
        for(int i=1;i<=TABLE_WIDTH;i++)
            for(int j=1;j<=TABLE_WIDTH;j++)
                dist_to_centers[i][j] = pow(centers_xy_coord[i][j].x - window_coord.x,2)+pow(centers_xy_coord[i][j].y - window_coord.y,2);

        double min_dist=100*RADIUS;
        Vector2<int> click_coord;

        for(int i=1;i<=TABLE_WIDTH;i++)
            for(int j=1;j<=TABLE_WIDTH;j++)
                if(dist_to_centers[i][j] < min_dist){
                    min_dist = dist_to_centers[i][j];
                    click_coord={i,j};
                }
        return click_coord;
         */
        return{1,1};
    };
    static void draw(sf::RenderWindow *window){
        sf::CircleShape hex(RADIUS, 6);
        hex.setFillColor(sf::Color(2,2,45));
        hex.setOutlineColor(sf::Color(100,100,200));
        hex.setOutlineThickness(2);
        for(int i=1;i<=TABLE_WIDTH;i++){
            for(int j=1;j<=TABLE_WIDTH;j++){
                hex.setPosition(get_xy_coord({i,j}));
                window->draw(hex);
            }
        }
    }

};

enum orientation {Horizontal = 0, Upper_diagonal = 1, Lower_diagonal = 2};

class Block{
public:
    int id{};
    int length{};
    bool is_selected{};
    sf::Color color;
    sf::Vector2<int> coord;
    sf::Vector2<int> moveVector;
public:
    Block(){};
    Block(int id,Vector2<int> coord,Color color,int length,int orientation,Table *table){
        this->id = id;
        this->color = color;
        this->coord = coord;
        this->length = length;
        is_selected = false;
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
        hex.setOutlineColor(sf::Color::White);
        if(is_selected)
            hex.setOutlineThickness(5);
        else
            hex.setOutlineThickness(0);
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

class Selector{
    Vector2<int> coord;
    sf::CircleShape icon;
public:
    bool enabled;
    Selector(){
        enabled=false;
        icon = CircleShape(RADIUS/3,30);
    }
    void set_position(Vector2<int> table_coord){
        coord = table_coord;
        Vector2<float> offset ={2*RADIUS/3,2*RADIUS/3};
        icon.setPosition(Table::get_xy_coord(table_coord)+offset);
    }
    void draw(sf::RenderWindow *window){
        if(enabled)
            window->draw(icon);
    }
    bool is_at(Vector2<int> pos){
        return pos == coord;
    }
};

enum game_state{no_block_slected,block_selected,game_complete};

int main(){
    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");
    Table table = Table();
    Block b[100];
    int no_of_blocks=3;

    int state = no_block_slected;
    int id_of_selected_block;
    Selector forward_move = Selector();
    Selector backward_move = Selector();
    Vector2<int> click_coord;

    b[1] = Block(1,{1,2},Color::Cyan,2,Horizontal,&table);
    b[2] = Block(2,{2,4},Color::Cyan,3,Lower_diagonal,&table);
    b[3] = Block(3,{5,2},Color::Yellow,4,Upper_diagonal,&table);
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
                click_coord = Mouse::getPosition(window);
                Vector2 coord = Table::transform_to_table_coord(click_coord);
                if(state == no_block_slected){
                    //checking if we clicked on a block
                    if(table.get_content(coord) > 0) {
                        id_of_selected_block = table.get_content(coord);
                        b[id_of_selected_block].is_selected = true;
                        state = block_selected;
                        //marking the posibble move directions
                        Block current_block = b[id_of_selected_block];
                        forward_move.enabled=backward_move.enabled = false;
                        if(current_block.is_leagal_move(1,table)) {
                            forward_move.set_position(
                                    current_block.coord + current_block.moveVector * current_block.length);
                            forward_move.enabled = true;
                        }
                        if(current_block.is_leagal_move(-1,table)) {
                            backward_move.set_position(current_block.coord - current_block.moveVector);
                            backward_move.enabled = true;
                        }
                    }
                }else if(state == block_selected){
                    if(forward_move.is_at(coord) && forward_move.enabled)
                        b[id_of_selected_block].move(1,&table);
                    else if(backward_move.is_at(coord) && backward_move.enabled)
                        b[id_of_selected_block].move(-1,&table);
                    state = no_block_slected;
                    b[id_of_selected_block].is_selected = false;
                    forward_move.enabled = backward_move.enabled = false;
                }
            }

        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        Table::draw(&window);
        b[1].draw(&window);
        b[2].draw(&window);
        b[3].draw(&window);
        forward_move.draw(&window);
        backward_move.draw(&window);
        // end the current frame
        window.display();
    }

    return 0;
}
