# Attack on Neural Network: Implementation on Chip Whisperer

The code structure is as follows:
- Random Expt: It contains a Jupyter Notebook that trains a Fully Connected Neural Network (FCNN) on the MNIST dataset using TensorFlow/Keras. After training, the model's weights and biases are extracted, and corresponding C code is generated for inference. The same setup is applied to several datasets.
- Activities: The neural network is implemented for `Activities` dataset. It's csv file is attached. This folder contain the C code for the inference. The Makefile is also given. The jupyter notebook file contaisn the code to run on the chip whisperer and capture the power traces. It is well annotated.
- MNIST: It contains the same information as in `Activities` folder. The input samples are binarized due to small resource of the Chip Whisperer. THe non-binary input samples are unable to fit.
