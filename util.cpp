#include "microbe_image.h"
#include "dye_image.h"
#include <cstdio>
#include <iostream>


int convert_to_small_grid(int large_coord){
    //0-99999 => 0-49
    return (((double)large_coord)/100000)*50;
}

//prints out border of microbe on a 50x50 grid
std::string get_microbe_string(Microbe_Image* img){
    std::vector<unsigned char>* walk = img->get_border_walk();
    char grid[50][50];
    int current_x = img->get_start_x();
    int current_y = img->get_start_y();

    for(int i = 0; i < 50; i++){
        for(int j = 0; j < 50; j++){
            grid[i][j] = '|';
        }
    }

    for(size_t i = 0; i < walk->size(); i++){
        /*
        0 = up
        1 = right
        2 = down
        3 = left
        */
        unsigned char new_dir = (*walk)[i];
        for(int j = 0; j < 4; j++){
            char current = new_dir % 4;
            new_dir /= 4;

            switch(current){
                case 0:
                    current_y--;
                    break;
                case 1:
                    current_x++;
                    break;
                case 2:
                    current_y++;
                    break;
                case 3:
                    current_x--;
                    break;
            }
            grid[convert_to_small_grid(current_y)][convert_to_small_grid(current_x)] = 'X';
        }
    }
    std::string microbe_str;
    for(int i = 0; i < 50; i++){
        for(int j = 0; j < 50; j++){
            microbe_str.push_back(grid[i][j]);
            microbe_str.push_back(' ');
        }
        microbe_str.push_back('\n');
    }
    return microbe_str;

}

void print_microbe(Microbe_Image* img){
    std::cout << "\n\nMicrobe Image:" << std::endl;
    std::cout << get_microbe_string(img);
}

std::string get_microbe_and_dye_string(Microbe_Image* microbe_image, Dye_Image* dye_image){
    std::string microbe_string = get_microbe_string(microbe_image);
    std::vector<std::vector<int> >* veins = dye_image->get_rows();
    int first_row_index = dye_image->get_first_row_index();
    char dye_indicator = 'D';

    for(size_t i = 0; i < veins->size(); i++){
        int sum = 0;
        for(size_t j = 0; j < veins->at(i).size(); j++){
            sum += veins->at(i).at(j);
            if(j%2 == 0){
                for(int k = 0; k < veins->at(i).at(j); k++){
                    //each row has 101 chars: 50 spaces, 50 potential indicators, and 1 return
                    int char_index = 101 * convert_to_small_grid(first_row_index) + 2 * convert_to_small_grid(sum + k);
                    microbe_string.replace(microbe_string.begin() + char_index, microbe_string.begin() + char_index + 1, &dye_indicator);
                }
            }
        }
    }

    return microbe_string;
}

void print_microbe_and_dye(Microbe_Image* microbe_image, Dye_Image* dye_image){
    std::cout << "\n\nMicrobe and Dye Image:" << std::endl;
    std::cout << get_microbe_and_dye_string(microbe_image, dye_image);
}






// int main(){
//     std::cout << convert_to_small_grid(0);
//     std::cout << convert_to_small_grid(99999);
//     std::cout << convert_to_small_grid(12345);
//     std::cout << convert_to_small_grid (2349);
// }