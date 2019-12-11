#ifndef DYE_IMAGE_H
#define DYE_IMAGE_H

#include <vector>

class Dye_Image{
    public:
    Dye_Image(std::vector<std::vector<int> >* rows, int index);
    ~Dye_Image();
    
    std::vector<std::vector<int> >* get_rows();
    int get_first_row_index();
    
    void set_rows(std::vector<std::vector<int> >* rows);
    void set_first_row_index(int index);

    private:
    std::vector<std::vector<int> >* rows;
    int first_row_index;
};

#endif