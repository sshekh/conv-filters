#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include <cassert>
#include <tuple>
#include <algorithm>
#include "filter.hpp"

class conv_layer {
  int in_width, in_height, in_depth,
      n_filters, window, stride, padding,
      out_width, out_height, out_depth;
public:
  conv_layer(int _width,
             int _height,
             int _depth,
             int _window,
             int _stride = 1,
             int _padding = 0, 
             int n_filters = 1)
    : in_width(_width), 
      in_height(_height),
      in_depth(_depth),
      window(_window),
      stride(_stride),
      padding(_padding),
      n_filters(n_filters) {
        // in_width + 2*padding - window should be divisible by stride
        out_width = (in_width + 2*padding - window) / stride + 1;
        out_height = (in_height + 2*padding - window) / stride + 1;
        out_depth = n_filters;
      }

  ~conv_layer() {}

  // Applies convolutional filters in filters to input volume x
  // x treated as in_width * in_height * in_depth 
  // assumed n_filters elements in filters vector
  // returns shape of the output volume and pointer to the memory block
  std::tuple<int, int, int, double ***> 
    conv2d(double ***x, const std::vector<filter*> &filters) {
    double ***y = get_tensor(out_width, out_height, out_depth);
    assert (filters.size() == n_filters);

    for (int k = 0; k < n_filters; ++k) {     // k_th activation map
      
      for (int i = 0; i < out_width; ++i) {
        for (int j = 0; j < out_height; ++j) {
          // fill y[i][j] with kernel computation filters[k]->x + b 
          // compute boundaries inside original matrix after padding
          int i_start = -padding + i*stride,  
              j_start = -padding + j*stride;
          int i_end = i_start + window,
              j_end = j_start + window;

          for (int i_pt = std::max(0, i_start); i_pt < std::min(in_width, i_end); ++i_pt) {
            for (int j_pt = std::max(0, j_start); j_pt < std::min(in_height, j_end); ++j_pt) {
              for (int k_pt = 0; k_pt < in_depth; ++k_pt) {
                y[i][j][k] += x[i_pt][j_pt][k_pt] * filters[k]->w[i_pt - i_start][j_pt - j_start][k_pt]; 
              }
            }
          }
          y[i][j][k] += filters[k]->b;  // bias term
        }
      }
    }
    return std::make_tuple(out_width, out_height, out_depth, y);
  }

};

#endif
