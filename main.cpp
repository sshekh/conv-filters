#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <execution>
#include <thread>

#include "filter.hpp"
#include "conv2d_layer.hpp"

using namespace std;

void job() {
 
}

int main(int argc, char *argv[]) {
    int w_size, bias; 
    cin >> w_size >> bias;

    /**
     * Core per N threads
     */ 
    // unsigned nb_threads_hint = std::thread::hardware_concurrency();
    // unsigned nb_threads = nb_threads_hint ? 16 : (nb_threads_hint);
    // std::vector< std::thread > my_threads(nb_threads);

    // unsigned batch_size = nb_elements / nb_threads;
    // unsigned batch_remainder = nb_elements % nb_threads;

    /**
     * input the image kernel matrix
     */ 
    matrix kernel(w_size, v(w_size));
    trans_tensor([](auto v) -> auto {
                cin >> v;
                return v;
            }, kernel);

    // for_tensor([](auto v) {
    //             cout << v << endl;
    //         }, kernel);

    std::vector<filter*> filters;

    array<unsigned int, 3> rgb = {0, 1, 2};
    for_each(std::execution::par, rgb.begin(), rgb.end(),
        [w_size, bias, kernel, &filters](auto idx) {
            // make edge detector for color idx 
            tensor ed(w_size, matrix(w_size, v(3)));

            for (int i = 0; i < w_size; ++i) {
                for (int j = 0; j < w_size; ++j) {
                    ed[i][j][idx] = kernel[i][j];
                }
            }
            // std::unique_ptr<filter> f(new filter(ed, w_size, 3, bias));

            // filter f(ed, w_size, 3, bias);
            // f.normalize();
            // filters.at(idx) = f;
            filter *f = new filter(ed, w_size, 3, bias);
            f->normalize();
            filters.push_back(f);
        });

        // cout << "test" << endl;
        // for_each(filters.begin(), filters.end(), [](auto v) {
        //     cout << v.b << endl;
        // });

    if (argc < 3) {
        cerr << "usage <input_data file name> <output file name>" << endl;
        return 1;
    }
    // output file will be written in the same format as input file
    ifstream ifile (argv[1]);   
    ofstream ofile (argv[2]);
    if (!ofile.is_open()) {
        cerr << "Unable to open " << argv[1] << endl;
        return 1;
    }
    if (!ifile.is_open()) {
        cerr << "Unable to open " << argv[0] << endl;
        return 1;
    }

    int width, num_images, height, depth;
    int stride = 1, padding = 1; 
    ifile >> num_images >> width >> height >> depth;
    ofile << num_images << " "; 
    cerr << num_images << " "; 

    conv_layer clayer(width, height, depth, w_size, stride, 
                                padding, filters.size()); 
    tensor input(width, matrix(height, v(depth)));


    for (int id = 0; id < num_images; ++id) {

        // read one image
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                for (int k = 0; k < depth; ++k) {
                    ifile >> input[i][j][k];
                    if (ifile.peek() == ',')
                        ifile.ignore();
                }
            }
        }
        // trans_matrix([ifile](auto v) -> auto {
        //     ifile >> v;
        //     if (ifile.peek() == ',')
        //         ifile.ignore();
        //     return v;
        // }, kernel);

        auto output = clayer.conv2d(input, filters);
        int o_width = get<0>(output), o_height = 
                get<1>(output), o_depth = get<2>(output);
        tensor out_volume = get<3>(output);
        if (id == 0) {
            // print image dimensions only the first time
            ofile << o_width << " " << o_height 
                    << " " << o_depth << "\n";
            cerr << o_width << " " 
                    << o_height << " " << o_depth << "\n";
        }

        for (int i = 0; i < o_width; ++i) {
            for (int j = 0; j < o_height; ++j) {
                ofile << out_volume[i][j][0] << "," 
                    << out_volume[i][j][1] 
                    << "," << out_volume[i][j][2] << " ";      
            }
            ofile << "\n";
        }
        ofile << "\n";
    }
    ifile.close();
    ofile.close();

    // std::for_each(my_threads.begin(), my_threads.end(), std::mem_fn(&std::thread::join));

    return 0; 
}
