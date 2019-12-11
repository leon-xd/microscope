#ifndef MICROBE_IMAGE_H
#define MICROBE_IMAGE_H


#include <cstdio>
#include <vector>

class Microbe_Image {
    public:
    Microbe_Image(int start_x, int start_y, std::vector<unsigned char>* border_walk);
    ~Microbe_Image();
    /*
    Define top left corner as 0, 0
    */
    int get_start_x();
    int get_start_y();
    std::vector<unsigned char>* get_border_walk();

    void set_start_x(int x);
    void set_start_y(int y);
    void set_border_walk(std::vector<unsigned char>* border_walk);

    private:
    int start_x;
    int start_y;
    std::vector<unsigned char>* border_walk;
};

#endif