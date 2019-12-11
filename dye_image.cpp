#include "dye_image.h"

Dye_Image::Dye_Image(std::vector<std::vector<int> >* rows, int index){
    this->rows = rows;
    this->first_row_index = index;
}

Dye_Image::~Dye_Image(){
    delete this->rows;
}

std::vector<std::vector<int> >* Dye_Image::get_rows(){
    return this->rows;
}

int Dye_Image::get_first_row_index(){
    return this->first_row_index;
}
    
void Dye_Image::set_rows(std::vector<std::vector<int> >* rows){
    this->rows = rows;
}
void Dye_Image::set_first_row_index(int index){
    this->first_row_index = index;
}
