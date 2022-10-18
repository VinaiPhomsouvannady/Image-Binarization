#include <string>

#include <vector>

#include <iostream>

#include <fstream>

#include <algorithm>

#include <sstream>

#include <stdlib.h>

#include <math.h>



//Visual Studio adds a argument for the file name.

void ReadFile(std::string file_name, std::vector<std::vector<double>>* image_data);

void WriteFile(std::string file_name, std::vector<std::vector<double>>* image_data);

double average(std::vector<double> a);

double deviation(std::vector<double> a, double ave);





int main(int argc, char* argv[]) {

	if (std::string(argv[1]) == "global") {



		std::string input_file_name = std::string(argv[2]);

		std::string output_file_name = std::string(argv[3]);

		std::vector<std::vector<double>> data;

		std::vector<double> pixel_vector;

		ReadFile(input_file_name, &data);



		for (unsigned int i = 0; i < data.size(); i++) {

			// Loop columns

			for (unsigned int j = 0; j < data[i].size(); j++) {

				//adds data to 1d so we can sort it.

				pixel_vector.push_back(data[i][j]);

			}

		}

		//sorts pixel_vector

		std::sort(pixel_vector.begin(), pixel_vector.end());


		//finds global median

		double t;
		t = pixel_vector[pixel_vector.size() / 2];


		for (unsigned int i = 0; i < data.size(); i++) {

			// Loop columns

			for (unsigned int j = 0; j < data[i].size(); j++) {

				//changes data in the vector to reflect global imaging..

				if (data[i][j] < t) {

					data[i][j] = 0;

				}

				else {

					data[i][j] = 255;

				}

			}

		}

		WriteFile(output_file_name, &data);

	}

	else {

		std::string input_file_name = std::string(argv[2]);

		std::string output_file_name = std::string(argv[3]);

		std::vector<std::vector<double>> data;
		std::vector<double> nvalues;
		std::vector<double> tvalues;
		std::vector < double> pixel_vector;
		ReadFile(input_file_name, &data);
		//converts neighborhood into movement
		int n = std::atoi(argv[4]);
		n = n / 2;
		for (unsigned int i = 0; i < data.size(); i++) {

			// Loop columns

			for (unsigned int j = 0; j < data[i].size(); j++) {

				//adds data to 1d so we can sort it.

				pixel_vector.push_back(data[i][j]);

			}

		}
		std::sort(pixel_vector.begin(), pixel_vector.end());
		double pmin = pixel_vector[0];
		double pmax = pixel_vector[pixel_vector.size() - 1];

		for (int i = 0; i < data.size(); i++) {

			// Loop columns

			for (int j = 0; j < data[i].size(); j++) {
				//at each pixel creates boundries for the neighborhood
				int ab = i + n;
				int ba = j + n;
				//std::cout <<"AB" << ab << std::endl;
				//std::cout <<"BA" << ab << std::endl;
				for (int a = i - n; a <= ab; a++) {
					//std::cout << "A: " << a << std::endl;
					for (int b = j - n; b <= ba; b++) {
						//std::cout << "B: " << b << std::endl;

						//goes through the neighborhood starting at the top left most point
						if (a > -1 && b > -1 && a < data.size() && b < data[i].size()) {
							//checks to see if the value exists. If it does then it is added to nvalues.
							nvalues.push_back(data[a][b]);
						}
					}

				}
				//makes all the calulations for t adds it then clears the neighborhood values for the next pixel
				double ave = average(nvalues);
				//std::cout << "Ave: " << ave <<std::endl ;
				double dev = deviation(nvalues, ave);
				//std::cout << "Dev: " << dev << std::endl;
				double r = .5 * (pmax - pmin);
				double temp = ave * (1 + (.2 * ((dev / r) - 1)));
				//std::cout << "thresh: " << ave * (1 + (.2 * ((dev / r) - 1))) << std::endl;
				tvalues.push_back(temp);
				nvalues.clear();
			}
		}

		int c = 0;
		for (unsigned int i = 0; i < data.size(); i++) {

			// Loop columns

			for (unsigned int j = 0; j < data[i].size(); j++) {

				//changes data in the vector to reflect global imaging..
				if (data[i][j] < tvalues[c]) {

					data[i][j] = 0;

				}

				else {

					data[i][j] = 255;

				}
				c++;

			}

		}

		WriteFile(output_file_name, &data);

	}
}






//finds average
double average(std::vector<double> a) {
	double sum = 0;
	for (unsigned int i = 0; i < a.size(); i++) {
		sum += a[i];
	}
	return sum / a.size();
}
//finds deviation
double deviation(std::vector<double> a, double ave)
{
	double E = 0;
	for (int i = 0; i < a.size(); i++) {
		E += ((a[i] - ave) * (a[i] - ave));
		//std::cout << E << std::endl;
		//::cout << (a[i] - ave) << std::endl;

	}
	E = E / a.size();
	return std::sqrt(E);
}


// Read 2D array from a file

void ReadFile(std::string file_name, std::vector<std::vector<double>>* image_data) {

	// Create the input filestream - opens the file & prepares it for reading

	std::ifstream file(file_name);



	// Creates a temporary vector to represent one row

	std::vector<double> new_row;



	// Temporary string to hold a single line of the file

	std::string str;



	// Reads all lines in the file, 1 at at time

	while (std::getline(file, str)) {

		// Converts our string into a stringstream

		std::istringstream ss(str);

		// Temp double to store a converted value from a line

		double token;



		// Reads all values from the stringstream (current row), converts to double

		while (ss >> token) {

			// Adds the converted value to the row

			new_row.push_back(token);

		}

		// Pushes our constructed vector of doubles to the 2D vector

		image_data->push_back(new_row);

		new_row.clear();

	}

}



void WriteFile(std::string file_name, std::vector<std::vector<double>>* image_data) {

	// Opens the outfile file, prepares it for writing

	std::ofstream output_file(file_name);



	// Loop rows

	for (unsigned int i = 0; i < image_data->size(); i++) {

		// Loop columns

		for (unsigned int j = 0; j < (*image_data)[i].size(); j++) {

			// Output each value and a space

			output_file << (*image_data)[i][j] << " ";

		}

		// Output a newline character after every row

		output_file << std::endl;

	}

}