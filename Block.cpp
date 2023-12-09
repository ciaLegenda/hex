//
// Created by Calin on 12/6/2023.
//

#include "Block.h"

Block::Block(int id,Vector2<int> coord,Color color,int length,int orientation,Table *table){
    this->id = id;
    this->color = color;
    this->coord = coord;
    this->length = length;
    is_selected = false;
    moveVector = table->moves[orientation];
    legal_placement = true;
    for(int k=0;legal_placement && k<length;k++){
        if(table->get_content(this->coord + k*this->moveVector) != EMPTY)
            legal_placement = false;
    }
    if(legal_placement)
        for(int k=0;k<length;k++){
            table->set_content(this->coord + k*this->moveVector,id);
        }
}

void Block::draw(sf::RenderWindow *window) {
    CircleShape hex(RADIUS,6);
    hex.setOutlineColor(sf::Color::White);
    if(is_selected)
        hex.setOutlineThickness(5);
    else
        hex.setOutlineThickness(0);
    hex.setFillColor(color);
    for(int k=0;k<length;k++){
        hex.setPosition(Table::get_xy_coord(this->coord + k*this->moveVector));
        window->draw(hex);
    }
}
/*
bool Block::intersects_middle_row() {
    for(int k=0;k<length;k++)
        if((coord + k*moveVector).x == (coord + k*moveVector).y)
            return false;
    return true;
}
*/
bool Block::is_leagal_move(int direction,Table table){
    sf::Vector2<int> cell_to_check =  coord;
    if(direction>0)
        cell_to_check += length*moveVector;
    else
        cell_to_check -= moveVector;
    return table.get_content(cell_to_check) == EMPTY;
}

void Block::move(int direction,Table *table){
    if(direction>0){
        table->set_content(coord,EMPTY);
        table->set_content(coord + moveVector*length,id);
    }else{
        table->set_content(coord + moveVector*(length-1),EMPTY);
        table->set_content(coord-moveVector,id);
    }
    coord += moveVector*direction;
}