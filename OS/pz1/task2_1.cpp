#include <iostream>
#include <cmath>

using namespace std;

float f(float z) { return 3 * z + sin(z); }
float f_1(float z) { return 3 * pow(tan(z), 3); }
float f_2(float z) { return 1 / pow(cos(z), 4); }
float f_3(float z) { return cos(z) + 2 * z; }

int main() {
    const float C = 1.6;
    float z = cos(C);
    float x = z < 0 ? f_1(z) : z > 8 ? f_3(z) : f_2(z);
    float y = f(x);
    
    cout << "y = f(x) = " << y << endl;

    return 0;
}