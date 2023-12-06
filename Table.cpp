#include "Table.h"

using namespace sf;

Table::Table() {
    for (int i = 1; i <= TABLE_WIDTH; i++)
        for (int j = 1; j <= TABLE_WIDTH; j++) {
            contents[i][j] = EMPTY;
            centers_xy_coord[i][j].x = XORIGIN + 2 * (i + j) * RADIUS * COS60 + RADIUS * COS60;
            centers_xy_coord[i][j].y = YORIGIN - 2 * (i - j) * RADIUS * SIN60 + 3 * RADIUS / 2;

        }
    for (int i = 0; i <= TABLE_WIDTH; i++)
        contents[0][i] = contents[TABLE_WIDTH + 1][i] = contents[i][0] = contents[i][TABLE_WIDTH + 1] = WALL;
}

int Table::get_content(sf::Vector2<int> coord) {
    if(coord.y<0 ||coord.y>TABLE_WIDTH || coord.x<0 ||coord.x>TABLE_WIDTH)
        return WALL;
    return contents[coord.x][coord.y];
}

void Table::set_content(sf::Vector2<int> coord,int value){
    contents[coord.x][coord.y] = value;
}

sf::Vector2<float> Table::get_xy_coord(sf::Vector2<int> table_coord){
    int i=table_coord.x, j = table_coord.y;
    sf::Vector2<float> xy_coord;
    //taking into consideration that indexing starts with 1
    xy_coord.x = XORIGIN + 2 * (i + j) * RADIUS * COS60;
    xy_coord.y = YORIGIN - 2 * (i - j) * RADIUS * SIN60;
    return xy_coord;
}

Vector2<int> Table::transform_to_table_coord(Vector2<int> window_coord){
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
}

void Table::draw(sf::RenderWindow *window){
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