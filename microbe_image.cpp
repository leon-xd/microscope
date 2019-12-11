#include "microbe_image.h"
#include <cstdio>


Microbe_Image::Microbe_Image(int start_x, int start_y, std::vector<unsigned char>* border_walk){
    this->start_x = start_x;
    this->start_y = start_y;
    this->border_walk = border_walk;
}

Microbe_Image::~Microbe_Image(){
    delete this->border_walk;
}

int Microbe_Image::get_start_x(){
    return this->start_x;
}

int Microbe_Image::get_start_y(){
    return this->start_y;
}

std::vector<unsigned char>* Microbe_Image::get_border_walk(){
    return this->border_walk;
}


void Microbe_Image::set_start_x(int x){
    this->start_x = x;
}

void Microbe_Image::set_start_y(int y){
    this->start_y = y;
}

void Microbe_Image::set_border_walk(std::vector<unsigned char>* border_walk){
    this->border_walk = border_walk;
}