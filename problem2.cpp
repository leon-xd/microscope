#include "util.cpp"

#include <cstdlib>
#include <time.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cfloat>
#include <map>
#include <algorithm>
/*
1331
1 * 1 = 1
3 * 4 = 12
3 * 16 = 48
1 * 64 = 64
*/
void add_to_char(unsigned char* char_to_add, int* index, int num_to_add){
    *char_to_add += pow(4, *index) * num_to_add;
    (*index)++;
    if((*index) == 4){
        *index = 0;
    }
}


//generate an ellipse by using polar coordinates & rotation
//ellipse formula: 
//(x*cos(m) + y*sin(m))^2   (x*sin(m)-ycos(m))^2
//----------------------- + --------------------- = 1
//         a^2                        b^2
//where m is in radians, rotation from 0 to pi is relevant
//
//x(α)=acos(α)cos(θ)−bsin(α)sin(θ)+Cx
//y(α)=acos(α)sin(θ)+bsin(α)cos(θ)+Cy
//area of ellipse:  a x b x π; a, b are "radii" of minor & major vertex
Microbe_Image* generateMicrobeImage(){
    const double PI = 3.14159265358979323846;

    //requirements:
    /*
    -need a * b * pi > 2,500,000,000
    -a * b > 795,774,716
    -sqrt(795774716) = 28210
    -a < 50000, b < 50000
    -a or b > 28210
    */
    srand(time(NULL));

    //randomly generate a and b
    int a = (std::rand() % (49999-28210)) + 28210;
    long int min_product = 795774716;
    int b_min = min_product / a;
    int b = (std::rand() % (49999-b_min)) + b_min;

    //generate random rotation number between 0 and pi
    double theta = ((double)std::rand() / RAND_MAX) * PI;

    //get 1000 evenly spaced points on the ellipse
    int target_locs[360][2];
    const double tiny_pi = 2 * PI / 360;
    for(int i = 0; i < 360; i++){
        double curr_mult_pi = tiny_pi * i;
        int x = round(a * cos(curr_mult_pi) * cos(theta) - b * sin(curr_mult_pi) * sin(theta) + 50000);
        int y = round(a * cos(curr_mult_pi) * sin(theta) + b * sin(curr_mult_pi) * cos(theta) + 50000);
        // std::cout << i << " " << x << " " << y << " " << convert_to_small_grid(x) << " " << convert_to_small_grid(y) << std::endl;
        target_locs[i][0] = x;
        target_locs[i][1] = y;
    }

    //load 360 lines, one for every consecutive pair of points
    std::vector<unsigned char>* border_walk = new std::vector<unsigned char>();
    unsigned char current_char = 0;
    int char_index = 0;

    //get current x and y positions
    int curr_x = target_locs[0][0];
    int curr_y = target_locs[0][1];
    for(int i = 1; i < 361; i++){
        //get next x and y positions
        int next_x;
        int next_y;
        if(i == 360){
            next_x = target_locs[0][0];
            next_y = target_locs[0][1];
        }
        else{
            next_x = target_locs[i][0];
            next_y = target_locs[i][1];
        }

        //find difference between next and current, find ratio between them
        int diff_x = next_x - curr_x;
        int diff_y = next_y - curr_y;
        double begin_ratio;
        if(diff_x != 0){
            begin_ratio = ((double)diff_y)/diff_x;
        }
        else{
            begin_ratio = DBL_MAX;
        }
        int change_in_x = -1;
        if(diff_x > 0){
            change_in_x = 1;
        }
        int change_in_y = -1;
        if(diff_y > 0){
            change_in_y = 1;
        }
        // std::cout << "i: " << i << " diff_x: " << diff_x << " diff_y: " << diff_y << std::endl;
        while(!(diff_x == 0 && diff_y == 0)){
            //decide whether incrementing x or y would result in a closer ratio
            int new_x = change_in_x + curr_x;
            int new_y = change_in_y + curr_y;

            double change_x_ratio;
            if((next_x - new_x) != 0){
                change_x_ratio = ((double)(next_y - curr_y))/(next_x - new_x);
            }
            else{
                change_x_ratio = DBL_MAX;
            }

            double change_y_ratio;
            if((next_x - curr_x) != 0){
                change_y_ratio = ((double)(next_y - new_y))/(next_x - curr_x);
            }
            else{
                change_y_ratio = DBL_MAX;
            }

            if(diff_y == 0){
                curr_x += change_in_x;
                diff_x -= change_in_x;

                if(change_in_x > 0){
                    add_to_char(&current_char, &char_index, 1);
                }
                else{
                    add_to_char(&current_char, &char_index, 3);
                }
                if(char_index == 0){
                    border_walk->push_back(current_char);
                    current_char = 0;
                }
            }
            else if(diff_x == 0){
                curr_y += change_in_y;
                diff_y -= change_in_y;

                if(change_in_y > 0){
                    add_to_char(&current_char, &char_index, 2);
                }
                else{
                    add_to_char(&current_char, &char_index, 0);
                }
                if(char_index == 0){
                    border_walk->push_back(current_char);
                    current_char = 0;
                }
            }
            else if(abs(change_x_ratio - begin_ratio) < abs(change_y_ratio - begin_ratio)){
                curr_x += change_in_x;
                diff_x -= change_in_x;

                if(change_in_x > 0){
                    add_to_char(&current_char, &char_index, 1);
                }
                else{
                    add_to_char(&current_char, &char_index, 3);
                }
                if(char_index == 0){
                    border_walk->push_back(current_char);
                    current_char = 0;
                }
            }
            else{
                curr_y += change_in_y;
                diff_y -= change_in_y;

                if(change_in_y > 0){
                    add_to_char(&current_char, &char_index, 2);
                }
                else{
                    add_to_char(&current_char, &char_index, 0);
                }
                if(char_index == 0){
                    border_walk->push_back(current_char);
                    current_char = 0;
                }
            }
            
        }
    }
    if(char_index != 0){
        border_walk->push_back(current_char);
    }
    Microbe_Image* microbe = new Microbe_Image(target_locs[0][0], target_locs[0][1], border_walk);
    return microbe;
}


bool does_border_continue_above(std::vector<int> row, std::vector<int> row_above, int pos){
    std::vector<int>::iterator it = std::find(row_above.begin(), row_above.end(), row.at(pos));
    if(it == row_above.end()){
        return false;
    }
    else{
        return true;
    }
}


//"draw" line from point to end of the image. If it intersects the border
//an odd number of times, it's in the shape
//cannot pass in values of x = 0 or y=0 or this will not work.
bool is_point_in_microbe(int x, int y, std::vector<std::vector<int> >* border_vector){
    std::cout << "Checking if point x = " << x << " y = " << y << " is inside border_vector" << std::endl;
    std::vector<int> row = border_vector->at(x);
    std::vector<int> row_above = border_vector->at(x-1);
    int count = 0;
    int pos = 0;
    bool curr_investigating = false;
    bool starts_above;



    //I notated the border as the inside edge instead of the outside edge of the microbe
    //which makes finding if a point is within the border a lot harder. Anything to save space though!
    //Need to see if we are currently investigating consecutive border pixels. Because the border that
    //I designated is on the inside of the shape, we have to check if the border goes the same
    //general direction and doesn't go up then down to count as a "cross" of the border.
    //This situation most likely won't happen with my microbe images because the borders are all
    //perfect ellipses, but if we get a microbe shaped more like a bean, this situation will come into
    //play.
    std::cout << "row: ";
    for(size_t i = 0; i < row.size(); i++){
        std::cout << row.at(i) << " ";
    }
    std::cout << std::endl;
    while(pos != (int)row.size()){
        std::cout << "pos = " << pos << std::endl;
        //
        if(row.at(pos) < y){
            pos++;
            continue;
        }
        if(row.at(pos) == y){
            return true;
        }
        if(curr_investigating){
            //row of border is being investigated
            //if this is end of the row, investigate above/below and see whether border continues above
            if((pos + 1 >= (int)row.size()) || (row.at(pos + 1) > row.at(pos) + 1)){
                bool ends_above = does_border_continue_above(row, row_above, pos);
                //if the border starts and ends in different directions, this is a passthrough of the border, increase count.
                if(ends_above != starts_above){
                    count++;
                }
                curr_investigating = false;
            }
        }
        else{
            //start investigating the row, notate if the border starts below or above
            //if a border "row" starts here, then it must have another pixel above or below it.
            curr_investigating = true;
            starts_above = does_border_continue_above(row, row_above, pos);
        }
        pos++;
    }
    std::cout << "count = " << count << std::endl;
    if(count%2 == 0){
        return false;
    }
    return true;
}

std::pair<int, int> coords_inside_border(int min_x, int min_y, int max_x, int max_y, std::vector<std::vector<int> >* border_vector){
    while(true){
        //-2 and +1 to ensure that it is within the max_x/y and min_x/y,
        //ensures existence of row_above and row_below
        int start_x = (rand()%(max_x-min_x-2)) + min_x + 1;
        int start_y = (rand()%(max_y-min_y-2)) + min_y + 1;
        if(is_point_in_microbe(start_x, start_y, border_vector)){
            return std::make_pair(start_x, start_y);
        }
    }
}

std::pair<int, int> coords_right_of_border(int min_x, int min_y, int max_x, int max_y){
    //draw leaks to the right of everything to make things easier for us
    int diff = max_x - min_x;
    int x = rand()%diff-50 + min_x;
    int y = (rand()%(max_y-min_y-50)) + min_y + 1;
    return std::make_pair(x, y);
}


//not going to bother with going up vs. going down as I do in the above method
bool is_range_in_microbe(std::vector<int> row, int start_y, int end_y){
    bool hit_border = false;
    std::vector<int>::iterator it;
    for(int i = start_y; i < end_y + 1; i++){
        it = std::find(row.begin(), row.end(), i);
        if(it != row.end()){
            if(!hit_border){
                hit_border = true;
            }
        }
        else{
            if(hit_border){
                return false;
            }
        }
    }
    return true;
}



Dye_Image* generateDyeImage(Microbe_Image* microbe_image){
    std::cout << "Starting generation of dye image" << std::endl;
    srand(time(NULL));

    std::vector<std::vector<int> > border_vector(100000);
    
    int current_x = microbe_image->get_start_x();
    int current_y = microbe_image->get_start_y();

    std::vector<unsigned char>* walk = microbe_image->get_border_walk();
    int min_x = 100000;
    int max_x = 0;
    int min_y = 100000;
    int max_y = 0;
    for(size_t i = 0; i < walk->size(); i++){
        unsigned char new_dir = (*walk)[i];
        for(int j = 0; j < 4; j++){
            unsigned char current = new_dir % 4;
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
            if(min_x > current_x){
                min_x = current_x;
            }
            if(max_x < current_x){
                max_x = current_x;
            }
            if(min_y > current_y){
                min_y = current_y;
            }
            if(max_y < current_y){
                max_y = current_y;
            }

            border_vector.at(current_x).push_back(current_y);
        }
    }
    for(size_t i = 0; i < border_vector.size(); i++){
        std::sort(border_vector.at(i).begin(), border_vector.at(i).end());
    }

    std::cout << "Analyzed border vector and made vector for it" << std::endl;
    //average area of ellipse = a * b * pi
    //a * b * pi ~ 39105^2 * pi = 122852
    //122852 * .01 = 1330
    //find random point for cancer part to start
    const int area_100th = 1330;
    int distribution[15];
    for(int i = 0; i < 15; i++){
        distribution[i] = area_100th * i;
    }

    int area_index = rand() / 15;

    std::vector<std::vector<int> > veins(100000);

    //model veins as a graph, with edges as the actual veins
    //make veins 100px thick
    std::vector<std::pair<int, int> > vein_nodes;
    vein_nodes.push_back(coords_inside_border(min_x, min_y, max_x, max_y, &border_vector));

    int count = 0;
    std::cout << "\n\nNew goal for num of dye pixels: " << distribution[area_index] << std::endl;
    while(count < distribution[area_index]){
        std::cout << "New node! Count = " << count << " Goal: " << distribution[area_index] << std::endl;
        //get new set of coordinates
        std::pair<int, int> potential_node = coords_inside_border(min_x, min_y, max_x, max_y, &border_vector);
        std::pair<int, int> beginning_node = vein_nodes.at(rand()%vein_nodes.size());

        if(potential_node.first == beginning_node.first){
            continue;
        }
        double slope = ((double)(potential_node.second-beginning_node.second))/(potential_node.first-beginning_node.first);

        //use copy of the final structure in case this line doesn't work out
        std::vector<std::vector<int> > vein_to_add(100000);
        vein_to_add.at(beginning_node.first).push_back(beginning_node.second);

        int curr_x = beginning_node.first;
        int curr_y = beginning_node.second;
        
        int diff_x = 1;
        if(beginning_node.first > potential_node.first){
            diff_x = -1;
        }
        int diff_y = 1;
        if(beginning_node.second > potential_node.second){
            diff_y = -1;
        }

        //determine line within borders
        bool failed_line = false;
        double temp_slope;
        int temp_y;
        while(curr_x != potential_node.first){
            curr_x += diff_x;
            temp_y = curr_y;
            temp_slope = ((double)(temp_y-beginning_node.second))/(curr_x-beginning_node.first);
            double diff = abs(temp_slope - slope);
            bool keep_going = true;
            //edgecase where we've reached the end of the line, can prevent divided by 0 errors this way
            if(curr_x == potential_node.first){
                temp_y = potential_node.second;
                keep_going = false;
            }
            while(keep_going){
                //increment y until incrementing it more increases the difference in slopes
                double next_slope = ((double)(temp_y-beginning_node.second+diff_y))/(curr_x-beginning_node.first);
                double next_diff = abs(next_slope - slope);
                if(next_diff < diff){
                    diff = next_diff;
                    temp_y += diff_y;
                }
                else{
                    keep_going = false;
                }
            }
            curr_y = temp_y;
            //check if this point is in the microbe, if not break out of loop
            if(!is_point_in_microbe(curr_x, curr_y, &border_vector)){
                failed_line = true;
                break;
            }
            //found optimal point to continue line, put it in the vein to add
            vein_to_add.at(curr_x).push_back(temp_y);
        }
        if(failed_line){
            continue;
        }

        //draw thickness now that we have a line
        //do this by extending each line 50 pixels out from each space below, in the y direction
        bool failed_fill = false;
        for(size_t i = 0; i < vein_to_add.size(); i++){
            int last_size;
            if(vein_to_add.at(i).size() != 0){
                int start_of_vein = vein_to_add.at(i).at(0);
                if(vein_to_add.at(i+1).size() != 0){
                    int range = abs(start_of_vein - vein_to_add.at(i+1).at(0)) + 50;
                    if(is_range_in_microbe(border_vector.at(i), start_of_vein, start_of_vein + range)){
                        last_size = range;
                        vein_to_add.at(i).push_back(range);
                    }
                    else{
                        failed_fill = true;
                        break;
                    }
                }
                else{
                    if(is_range_in_microbe(border_vector.at(i), start_of_vein, start_of_vein + last_size)){
                        vein_to_add.at(i).push_back(last_size);
                    }
                    else{
                        failed_fill = true;
                        break;
                    }
                }
            }
        }
        if(failed_fill){
            continue;
        }

        //now, we know we have a valid vein in the system. Update the original veins vector with vein_to_add.
        //update vein_nodes & the count of pixels being added as well.
        vein_nodes.push_back(potential_node);
        for(size_t i = 0; i < veins.size(); i++){
            std::vector<int>* row = &veins.at(i);
            std::vector<int> row_to_add = vein_to_add.at(i);
            std::vector<int> new_row;
            if(row_to_add.size() == 0){
                continue;
            }
            if(row->size() == 0){
                *row = row_to_add;
                count += row_to_add.at(1);
            }
            else{
                bool row_is_vein = false;
                int row_sum = row->at(0);
                int row_index = 0;

                bool row_to_add_is_vein = false;
                int row_to_add_sum = row_to_add.at(i);
                int row_to_add_index = 0;
                
                int curr_string = 0;
                bool was_inputting_vein = false;
                for(int curr_pos = 0; curr_pos < 100000; curr_pos++){
                    if((row_sum < curr_pos) && (row_index != (int)row->size()-1)){
                        row_is_vein = !row_is_vein;
                        row_index++;
                        row_sum += row->at(row_index);
                    }

                    if((row_to_add_sum < curr_pos) && (row_to_add_index != (int)row_to_add.size()-1)){
                        row_to_add_is_vein = !row_is_vein;
                        row_to_add_index++;
                        row_to_add_sum += row_to_add.at(row_index);
                    }

                    if(row_to_add_is_vein && !row_is_vein){
                        count++;
                    }

                    if((row_is_vein || row_to_add_is_vein) != was_inputting_vein){
                        new_row.push_back(curr_string);
                        curr_string = 1;
                        was_inputting_vein = !was_inputting_vein;
                    }
                    else{
                        curr_string++;
                    }
                }

                *row = new_row;
            }
        }
    }
    //make random 50x50 square somewhere, as dye leak
    std::pair<int, int> upper_right_corner = coords_right_of_border(min_x, min_y, max_x, max_y);
    for(int i = 0; i < 50; i++){
        int row_sum = 0;
        for(size_t j = 0; j < veins.at(upper_right_corner.first + i).size(); j++){
            row_sum += veins.at(upper_right_corner.first + i).at(j);
        }
        veins.at(upper_right_corner.first + i).push_back(upper_right_corner.first-row_sum);
        veins.at(upper_right_corner.first + i).push_back(50);
    }
    
    //trim beginning and end rows so that the vectors of length 0 are not present
    std::vector<std::vector<int> >::iterator begin_it = veins.begin();
    std::vector<std::vector<int> >::iterator end_it = veins.end();
    while(begin_it->size() == 0 && begin_it != veins.end()){
        begin_it++;
    }
    int row_count = 0;
    while(end_it->size() == 0 && end_it != veins.begin()){
        end_it--;
    }
    end_it++;

    

    std::vector<std::vector<int> >* vein_ptr = new std::vector<std::vector<int> >(begin_it, end_it);
    return new Dye_Image(vein_ptr, row_count);
}




int main(){
    Microbe_Image* microbe_image = generateMicrobeImage();
    print_microbe(microbe_image);

    Dye_Image* dye_image = generateDyeImage(microbe_image);
    print_microbe_and_dye(microbe_image, dye_image);
    delete microbe_image;
    delete dye_image;
}