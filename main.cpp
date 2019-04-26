#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <array>
#include <fstream>
#include <string>
using namespace std;

class annealing {
private:
    float temperature;
    vector<vector<int> > data;
    array<int, 2> shape;
    double old_energy;
    double new_energy;
    unsigned int step_num;
    double threshold;
public:
    annealing() {
        temperature = 0;
        threshold = 1;
        step_num = 0;
        shape[0] = 0;
        shape[1] = 0;
    }
    annealing(float init_temp, vector<vector<int> >in_data): temperature(init_temp) {
        shape[0] = in_data.size();
        shape[1] = in_data[0].size();
        threshold = 1;
        step_num = 0;
        for (int i = 0; i < shape[0]; i++) {
            data.push_back(vector<int>(shape[1]));
            for (int j = 0; j < shape[1]; j++) {
                data[i][j] = in_data[i][j];
            }
        }
    }

    void decrease_temp() {
        if (temperature > 20) {
            temperature -= 0.001;
        }
        else if ((temperature <= 20) && (temperature >3))
            temperature -= 0.0001;
        else
            temperature -= 0.00003;
    }

    double count_energy() {
        double total_energy = 0;
        for (int i = 0; i < shape[0] - 1; i++) {
            total_energy += sqrt((data[i+1][0] - data[i][0])*(data[i+1][0] - data[i][0]) + (data[i+1][1] - data[i][1])*(data[i+1][1] - data[i][1]));
        }
        return total_energy;
    }

    double reverse(int i, int j) {
        vector<int> temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }

    void optimize() {
        while(temperature > 0) {
            step();
            if (step_num % 10000 == 0) {
                print();
            }
        }
    }

    void step() {
        step_num += 1;
        if (step_num == 1) {
            old_energy = count_energy();
            decrease_temp();
            return;
        }
        int i = rand() % shape[0];
        int j = rand() % shape[0];
        //cout << "----------------------------------------------------------------------" << endl;
        //cout << "i: " << i << ", j: " << j << endl;
        //cout << "----------------------------------------------------------------------" << endl;
        reverse(i, j);
        new_energy = count_energy();
        if (new_energy > old_energy) {
            threshold = exp(-1.0 * (new_energy - old_energy) / temperature);
            float p = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            //cout << endl << "new energy > old energy, probability: " << p << endl;
            if (p > threshold) {
                reverse(i, j);
                //cout << "p = " << p << ", threshold = " << threshold << endl;
            }
            else
                old_energy = new_energy;
        }
        else
            old_energy = new_energy;
        decrease_temp();
    }

    void print() {
        cout << "STEP: " << step_num <<" ENERGY: " << old_energy << " TEMPERATURE: " << temperature << "PROBA: " << threshold << endl;
    }

    void print_data() {
        cout << "----------------------------------------------------------------------" << endl;
        for (int i = 0; i < shape[0]; i++) {
            for (int j = 0; j<shape[1]; j++) {
                cout << data[i][j] << " ";
            }
            cout << endl;
        }
        cout << "----------------------------------------------------------------------" << endl;
    }

    void write_data_to_csv(string filename) {
        ofstream myfile;
        myfile.open(filename);
        for (int i = 0; i < shape[0]; i++) {
            for (int j = 0; j < shape[1] - 1; j++) {
                myfile << data[i][j] << ";";
            }
            myfile << data[i][shape[1] - 1] << "\n";
        }
    }
};


int main() {
    vector<vector<int> > A;
    int shape [2] = {500, 2};
    for (int i = 0; i < shape[0]; i++) {
        A.push_back(vector<int>());
        for (int j = 0; j < shape[1]; j++) {
            A[i].push_back(rand() % 100);
        }
    }
    annealing toy(60, A);
    toy.write_data_to_csv("start.csv");
    toy.optimize();
    toy.write_data_to_csv("finish.csv");
    return 0;
}
