#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include "filter.hpp"
#include "conv2d_layer.hpp"

const int W_SIZE = 3;
const double edge_detect[3][3] = { {0, 1, 0}, {1, -4, 1}, {0, 1, 0} };

using namespace std;

vector<filter*> filters;

void push_filter(int idx) {     // make edge detector for color idx
  double ***ed = get_tensor(3, 3, 3); 
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      for (int k = 0; k < 3; ++k) {
        ed[i][j][k] = 0;
      }
    }
  }
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      ed[i][j][idx] = edge_detect[i][j];
    }
  }
  filter *f = new filter(ed, 3, W_SIZE);
  f->normalize();
  filters.push_back(f);
}

int main(int argc, char *argv[]) {
  push_filter(0); // R
  push_filter(1); // G
  push_filter(2); // B
  //cerr << "Printing first\n";
  //for (int k = 0; k < 3; ++k) {
  //  for (int i = 0; i < 3; ++i) {
  //    for (int j = 0; j < 3; ++j) {
  //      for (int k_pt = 0; k_pt < 3; ++k_pt) 
  //        std::cerr << filters[k]->w[i][j][k_pt] << ",";
  //      std::cerr << " ";
  //    }
  //    std::cerr << "\n";
  //  }
  //  std::cerr << "\n";
  //} 
  if (argc < 3) {
    cerr << "usage <input_data file name> <output file name>" << endl;
    return 1;
  }
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

  int WIDTH, NUM_IMAGES, HEIGHT, DEPTH;
  int STRIDE = 1, PADDING = 1; 
  ifile >> NUM_IMAGES >> WIDTH >> HEIGHT >> DEPTH;
  ofile << NUM_IMAGES << " "; 
  cerr << NUM_IMAGES << " "; 
  
  conv_layer clayer(WIDTH, HEIGHT, DEPTH, W_SIZE, STRIDE, PADDING, filters.size()); 
  double ***input;
  input = get_tensor(WIDTH, HEIGHT, DEPTH); 
  
  for (int id = 0; id < NUM_IMAGES; ++id) {
    for (int i = 0; i < WIDTH; ++i) {
      for (int j = 0; j < HEIGHT; ++j) {
        for (int k = 0; k < DEPTH; ++k) {
          ifile >> input[i][j][k];
          if (ifile.peek() == ',') ifile.ignore();
        }
      }
    }
    auto output = clayer.conv2d(input, filters);
    double ***out_volume = get<3>(output);
    int o_width = get<0>(output), o_height = get<1>(output), o_depth = get<2>(output);
    if (get<2>(output) != 3) {
      cerr << "ERR!! #channels is " << get<2>(output) << "\n";
      return 1;
    }
    if (id == 0) {
      ofile << o_width << " " << o_height << " " << o_depth << "\n";
      cerr << o_width << " " << o_height << " " << o_depth << "\n";
    }

    for (int i = 0; i < o_width; ++i) {
      for (int j = 0; j < o_height; ++j) {
        ofile << out_volume[i][j][0] << "," << out_volume[i][j][1] << "," << out_volume[i][j][2]
          << " ";      
      }
      ofile << "\n";
    }
    ofile << "\n";
  }
  ifile.close();
  ofile.close();

  return 0; 
}
