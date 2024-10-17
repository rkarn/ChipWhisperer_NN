# MNIST Fully Connected Neural Network (FCNN) with C Code Generation

This repository contains a Jupyter Notebook that trains a Fully Connected Neural Network (FCNN) on the MNIST dataset using TensorFlow/Keras. After training, the model's weights and biases are extracted, and corresponding C code is generated for inference.

## Overview of Experiments

### 1. Training the FCNN
We first train a simple neural network on the MNIST dataset. The network consists of:
- A single hidden layer
- ReLU activation function
- Softmax output layer for classification

The model is trained for 10 epochs, and the test accuracy is evaluated at the end of training. On average, the test accuracy achieves around **92.5%**.

### 2. C Code Generation for Inference
Once the model is trained, we extract its weights and biases. These are used to generate C code that can perform inference on new data. The generated C code (`mnist_inference.c`) includes:
- The trained weights and biases
- A function `predict()` that performs the forward pass of the network

The C code allows you to make predictions on MNIST data using any C environment, making it useful for embedded system deployment.

### 3. Validation of C Code
A Python script is also included, which validates the accuracy of the C code by comparing its output to predictions from the trained Keras model. This ensures that the C code performs inference correctly.

## Files in the Repository

- `Python_C_FCNN_MNIST.ipynb`: The main notebook that trains the FCNN, generates the C code, and validates it.
- `mnist_inference.c`: Generated C code containing the FCNN's weights and biases, and a function to perform inference.

## Instructions

1. **Run the Jupyter Notebook**: Execute the notebook to train the model and generate the C code.
2. **Validate the C Code**: Use the validation script in the notebook to confirm the C code's accuracy on the MNIST test dataset.
3. **Deploy the C Code**: Use the generated `mnist_inference.c` in your C environment to perform inference on new data.

## Dependencies

- Python 3.x
- TensorFlow 2.x
- Numpy
- Jupyter Notebook
