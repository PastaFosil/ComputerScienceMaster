#include <iostream>

using namespace std;

int main(int argc, char **argv){
	double num = stof(argv[1]);
	double sup = num, inf = 0.0, center = (inf+sup)/2;
	double error = 0.00001;
	double squared = center*center;
	
	while (squared<num-error || squared>num+error){
		if (squared>num)
			sup = center;
		else
			inf = center;
		center = (inf+sup)/2;
        squared = center*center;
	}
	cout << "Raiz cuadrada de " << num << " = " << center << endl;
}