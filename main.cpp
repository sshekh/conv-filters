#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include "filter.hpp"
#include "conv2d_layer.hpp"

using namespace std;

int w_size, bias;     // kernel window size
double** kernel; // image kernel 
vector<filter*> filters;

void push_filter(int idx) {     
  // make edge detector for color idx 
  double ***ed = get_tensor(w_size, w_size, 3); 
  for (int i = 0; i < w_size; ++i) {
    for (int j = 0; j < w_size; ++j) {
      ed[i][j][idx] = kernel[i][j];
    }
  }
  filter *f = new filter(ed, w_size, 3, bias);
  f->normalize();
  filters.push_back(f);
}

int main(int argc, char *argv[]) {

  //  input the kernel matrix
  cin >> w_size >> bias;
  kernel = new double*[w_size]; 
  for (int i = 0; i < w_size; i++) {
    kernel[i] = new double[w_size];
    for (int j = 0; j < w_size; j++) {
      cin >> kernel[i][j]; 
    }
  }

  push_filter(0); // R
  push_filter(1); // G
  push_filter(2); // B
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
  
  conv_layer clayer(width, height, depth, w_size, stride, padding, filters.size()); 
  double ***input;
  input = get_tensor(width, height, depth); 
  
  for (int id = 0; id < num_images; ++id) {

    // read one image
    for (int i = 0; i < width; ++i) {
      for (int j = 0; j < height; ++j) {
        for (int k = 0; k < depth; ++k) {
          ifile >> input[i][j][k];
          if (ifile.peek() == ',') ifile.ignore();
        }
      }
    }
    auto output = clayer.conv2d(input, filters);
    double ***out_volume = get<3>(output);
    int o_width = get<0>(output), o_height = get<1>(output), o_depth = get<2>(output);
    if (id == 0) {
      // print image dimensions only the first time
      ofile << o_width << " " << o_height << " " << o_depth << "\n";
      cerr << o_width << " " << o_height << " " << o_depth << "\n";
    }

    for (int i = 0; i < o_width; ++i) {
      for (int j = 0; j < o_height; ++j) {
        ofile << out_volume[i][j][0] << "," << out_volume[i][j][1] 
                << "," << out_volume[i][j][2] << " ";      
      }
      ofile << "\n";
    }
    ofile << "\n";
  }
  ifile.close();
  ofile.close();

  return 0; 
}
