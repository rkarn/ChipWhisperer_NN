# Side-channel Attack on Neural Network: Implementation on Chip Whisperer


## Overview
This repository contains the framework and resources for performing power side-channel analysis on neural network (NN) implementations using the ChipWhisperer platform. This work is designed to be an educational tool for university students, providing hands-on experience with side-channel attacks and hardware security.

## Prerequisites
Before you begin, ensure you have the following prerequisites installed:

1. **ChipWhisperer-Lite Board**: You can purchase it from NewAE Technology.
2. **ChipWhisperer Software**: The software should be downloaded from `https://github.com/newaetech/chipwhisperer`. Follow their documentation to get started. Specifically, perform the instructions given at `https://chipwhisperer.readthedocs.io/en/latest/index.html#install` for installation.
3. **Python 3.6+**: Ensure you have Python installed. You can download it from python.org.
4. **Jupyter Notebook**: Install Jupyter Notebook for running the provided notebooks. You can install it from `https://docs.anaconda.com/anaconda/install/`.
5. **GCC Compiler**:  C code for microcontroller programming, preferably the HAL library.
6. **Large Language Model**: Any LLM including OpenAI's GPT model, Microsoft Co-pilot, Google's Gemini, RapidGPT, or others. These are used for translation of Python-based nueral network code to C. 


## Code structure:
- Random Expt: It contains a Jupyter Notebook that trains a Fully Connected Neural Network (FCNN) on the MNIST dataset using TensorFlow/Keras. After training, the model's weights and biases are extracted, and corresponding C code is generated for inference. The same setup is applied to several datasets.
- Activities: The neural network is implemented for `Activities` dataset. It's csv file is attached. This folder contain the C code for the inference. The Makefile is also given. The jupyter notebook file contaisn the code to run on the chip whisperer and capture the power traces. It is well annotated.
- MNIST: It contains the same information as in `Activities` folder. The input samples are binarized due to small resource of the Chip Whisperer. THe non-binary input samples are unable to fit.
- Wireless: Same as `Activities` and `MNIST`.


## Ease of Use
This framework is designed with ease of use in mind, particularly for educational purposes:
- Comprehensive Documentation: Each Jupyter notebook includes detailed explanations and comments to guide you through the process.
- Integrated Hardware: The ChipWhisperer-Lite board integrates all necessary components for side-channel analysis, eliminating the need for additional equipment like oscilloscopes and power meters.
- Open-Source: The entire framework is open-source, allowing you to explore, modify, and extend the code to suit your educational needs.

## Tutorial Links:
1. Embedded System: Run C code in microcontroller.
   - `https://medium.com/geekculture/how-to-learn-c-c-for-microcontroller-programming-60324c4fec09`.
   - `https://documents.uow.edu.au/~phung/avr/guides/Getting-started-C-programming-Microchip-Studio-7.pdf`.
   - `https://www.youtube.com/watch?v=YJTF662ugVs`

2.Neural Network Training and Validation:
  - `https://www.allaboutcircuits.com/technical-articles/training-dataset-neural-network-how-to-train-validate-python-neural-network/`.
  - `https://kitchell.github.io/DeepLearningTutorial/8evaluatingnns.html`.
  - `https://towardsdatascience.com/neural-network-for-software-engineers-2-mini-batch-training-and-validation-46ee0a1269a0`.

3. Power Side-channel Analysis:
  - `https://github.com/newaetech/chipwhisperer-jupyter/tree/master/courses/sca101`
  - `https://github.com/newaetech/chipwhisperer-jupyter/tree/master/courses/sca201`

4. Large Language Model:
  - `https://medium.com/thedeephub/llm-prompt-engineering-for-beginners-what-it-is-and-how-to-get-started-0c1b483d5d4f`.
  - `https://learn.microsoft.com/en-us/ai/playbook/technology-guidance/generative-ai/working-with-llms/prompt-engineering` 
