/*
 * CNN demo for MNIST dataset
 * Author: Kai Han (kaihana@163.com)
 * Details in https://github.com/iamhankai/mini-dnn-cpp
 * Copyright 2018 Kai Han
 *
 * Modified for the ECE 408 class project
 * Fall 2020
 */

#include "ece408net.h"

Network createNetwork_OpenCL(OpenCL* opencl)
{
  Network dnn;

  Layer* conv1 = new Conv_Custom(1, 86, 86, 4, 7, 7);
  ((Conv_Custom*)conv1)->opencl = opencl;
  Layer* pool1 = new MaxPooling(4, 80, 80, 2, 2, 2);
  Layer* conv2 = new Conv_Custom(4, 40, 40, 16, 7, 7);
  ((Conv_Custom*)conv2)->opencl = opencl;
  Layer* pool2 = new MaxPooling(16, 34, 34, 4, 4, 4);
  Layer* fc3 = new FullyConnected(pool2->output_dim(), 32);
  Layer* fc4 = new FullyConnected(32, 10);
  Layer* relu1 = new ReLU;
  Layer* relu2 = new ReLU;
  Layer* relu3 = new ReLU;
  Layer* softmax = new Softmax;
  dnn.add_layer(conv1);
  dnn.add_layer(relu1);
  dnn.add_layer(pool1);
  dnn.add_layer(conv2);
  dnn.add_layer(relu2);
  dnn.add_layer(pool2);
  dnn.add_layer(fc3);
  dnn.add_layer(relu3);
  dnn.add_layer(fc4);
  dnn.add_layer(softmax);
  // loss
  Loss* loss = new CrossEntropy;
  dnn.add_loss(loss);

  //load weights
  dnn.load_parameters("./build/weights-86.bin");

  return dnn;
}
