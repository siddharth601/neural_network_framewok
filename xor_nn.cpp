#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// Activation functions and their derivatives
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_deriv(double x) {
    double s = sigmoid(x);
    return s * (1 - s);
}

double relu(double x) {
    return max(0.0, x);
}

double relu_deriv(double x) {
    return x > 0 ? 1 : 0;
}

// Layer class
class Layer {
public:
    int input_size, output_size;
    vector<vector<double>> weights;
    vector<double> biases;
    vector<double> outputs;
    vector<double> inputs;
    vector<double> deltas;

    Layer(int in_size, int out_size) {
        input_size = in_size;
        output_size = out_size;
        weights.resize(out_size, vector<double>(in_size));
        biases.resize(out_size);
        outputs.resize(out_size);
        deltas.resize(out_size);

        // Random weight initialization
        srand(time(0));
        for (int i = 0; i < out_size; ++i) {
            biases[i] = ((double) rand() / RAND_MAX) - 0.5;
            for (int j = 0; j < in_size; ++j) {
                weights[i][j] = ((double) rand() / RAND_MAX) - 0.5;
            }
        }
    }

    vector<double> forward(const vector<double>& input) {
        inputs = input;
        for (int i = 0; i < output_size; ++i) {
            outputs[i] = biases[i];
            for (int j = 0; j < input_size; ++j) {
                outputs[i] += weights[i][j] * input[j];
            }
            outputs[i] = sigmoid(outputs[i]);  // You can switch to ReLU here
        }
        return outputs;
    }

    void backward(const vector<double>& grad_output, double learning_rate) {
        for (int i = 0; i < output_size; ++i) {
            double grad = grad_output[i] * sigmoid_deriv(outputs[i]);
            deltas[i] = grad;
            for (int j = 0; j < input_size; ++j) {
                weights[i][j] -= learning_rate * grad * inputs[j];
            }
            biases[i] -= learning_rate * grad;
        }
    }

    vector<double> backward_output_gradient() {
        vector<double> grad_input(input_size, 0.0);
        for (int i = 0; i < output_size; ++i) {
            for (int j = 0; j < input_size; ++j) {
                grad_input[j] += weights[i][j] * deltas[i];
            }
        }
        return grad_input;
    }
};

// Loss function: Mean Squared Error
double mean_squared_error(const vector<double>& pred, const vector<double>& target) {
    double sum = 0.0;
    for (size_t i = 0; i < pred.size(); ++i) {
        sum += pow(pred[i] - target[i], 2);
    }
    return sum / pred.size();
}

vector<double> mse_derivative(const vector<double>& pred, const vector<double>& target) {
    vector<double> grad(pred.size());
    for (size_t i = 0; i < pred.size(); ++i) {
        grad[i] = 2 * (pred[i] - target[i]) / pred.size();
    }
    return grad;
}

// Training
int main() {
    Layer l1(2, 4);  // Input layer with 2 neurons, hidden layer with 4 neurons
    Layer l2(4, 1);  // Hidden layer with 4 neurons, output layer with 1 neuron

    vector<vector<double>> X = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}
    };
    vector<vector<double>> Y = {
        {0}, {1}, {1}, {0}  // XOR problem
    };

    double lr = 0.1;
    for (int epoch = 0; epoch < 10000; ++epoch) {
        double loss = 0.0;
        for (size_t i = 0; i < X.size(); ++i) {
            vector<double> out1 = l1.forward(X[i]);
            vector<double> out2 = l2.forward(out1);

            loss += mean_squared_error(out2, Y[i]);

            vector<double> grad2 = mse_derivative(out2, Y[i]);
            l2.backward(grad2, lr);
            vector<double> grad1 = l2.backward_output_gradient();
            l1.backward(grad1, lr);
        }
        if (epoch % 1000 == 0)
            cout << "Epoch " << epoch << ", Loss: " << loss / X.size() << endl;
    }

    // Inference
    cout << "\nPredictions:\n";
    for (const auto& x : X) {
        vector<double> out1 = l1.forward(x);
        vector<double> out2 = l2.forward(out1);
        cout << x[0] << " XOR " << x[1] << " = " << out2[0] << endl;
    }

    return 0;
}