#include <algorithm>


#include <iostream>
#include <vector>
#include "nn.cpp"
#include "test_dataset.c"

int main() {
    NeuralNetwork nn;
    int correct_predictions = 0;

    if (sizeof(test_data) / sizeof(test_data[0]) != 15745 || sizeof(test_data[0]) / sizeof(float) != 8) {
        std::cerr << "Error: Incorrect test data dimensions" << std::endl;
        return 1;
    }

    if (sizeof(test_labels) / sizeof(int) != 15745) {
        std::cerr << "Error: Incorrect test labels dimensions" << std::endl;
        return 1;
    }

    for (int i = 0; i < 15745; ++i) {
        std::vector<float> input(test_data[i], test_data[i] + 8);
        try {
            std::vector<float> output = nn.forward(input);
            int predicted_class = std::max_element(output.begin(), output.end()) - output.begin();
            int true_class = test_labels[i];
            if (predicted_class == true_class) {
                correct_predictions++;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error processing sample " << i << ": " << e.what() << std::endl;
            return 1;
        }
    }

    float accuracy = static_cast<float>(correct_predictions) / 15745 * 100;
    std::cout << "Accuracy: " << accuracy << "%" << std::endl;

    return 0;
}


