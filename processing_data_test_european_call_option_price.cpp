#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <chrono> //library for measuring time

#include "black_scholes.hpp"
#include "monte_carlo.hpp"
#include "binomial_tree.hpp"

using namespace std;

//Create a specific type of struct call option_data to store the data in the csv each line
struct option_data {
    double S;                             //Current stock price
    double K;                             //Strike price
    double T;                             //Time until expiry date (in years)
    double sd;                            //Volatility (%)
    double r;                             //Annualised risk-free interest rate (%) 
    double call_option_price;    //European call option price
};

//Create a function in vector data type that break one line of csv file into separate pieces
vector<string> split_csv_line(string line){
    vector<string> result;
    string value;
    stringstream ss(line);

    while (getline(ss, value, ',')){
        result.push_back(value);
    };
    return result;
};


int main(){
    //Create a file object name call_option_data_file and connect it to the csv data file
    ifstream call_option_data_file("european_call_option_data_sample_10000_2016_2020.csv");

    //In the case the file can't be open:
    if (!call_option_data_file.is_open()){
        cout << "File can't be open";
        return 0;
    };

    //Now we read the csv file line by line and save it into option_data structure

    //save_call_option_data_for_manipulation
    vector <option_data> scodfm;
    string line;

    //Skip header row
    getline(call_option_data_file, line);
    int max_rows_to_read_and_test = 10000;
    int rows_read = 0;
    //Processing the data line by line and save it into struct
    while (getline(call_option_data_file, line) && rows_read < max_rows_to_read_and_test){
        vector<string> columns = split_csv_line(line);

        option_data row;
        row.S = stod(columns[2]);
        row.K = stod(columns[3]);
        row.T = stod(columns[5]);
        row.sd = stod(columns[6]);
        row.r = stod(columns[7]);
        row.call_option_price = stod(columns[8]);

        if (row.S <= 0 || row.K <= 0 || row.T <= 0 || row.sd <= 0) {continue;}
        scodfm.push_back(row);
        rows_read += 1;
    };
    call_option_data_file.close();

    /*
    We will now start calculating european call option price from given data 
    and compare it to the final result
    */

    // max number of rows for testing:
    int max_row_for_test = max_rows_to_read_and_test; // Can be subjected to change
    double count_black_scholes = 0.0;
    double total_absolute_error = 0.0;
    //Using Black-Scholes function
    auto start_black_scholes_time = chrono::high_resolution_clock::now();

    for(int i = 0; i < scodfm.size(); i+= 1){
        double black_scholes_price = european_call_option_price(scodfm[i].S,scodfm[i].K,scodfm[i].T, scodfm[i].r, scodfm[i].sd);
        double difference = abs(black_scholes_price - scodfm[i].call_option_price);
        total_absolute_error += difference;
    };
    auto end_black_scholes_time = chrono::high_resolution_clock::now();

    double black_scholes_absolute_error = total_absolute_error / static_cast<double>(scodfm.size());

    //Using Monte-Carlo method

    double monte_carlo_count = 0.0;
    double total_absolute_error_monte = 0.0;
    auto start_monte_carlo_time = chrono::high_resolution_clock::now();
    for(int i = 0; i < scodfm.size() && i < max_row_for_test; i+= 1){
        double monte_carlo_price = monte_carlo_european_call_option_price(1000,scodfm[i].S,scodfm[i].K,scodfm[i].T, scodfm[i].r, scodfm[i].sd);
        double difference = abs(monte_carlo_price - scodfm[i].call_option_price);
        total_absolute_error_monte += difference;
    };
    auto end_monte_carlo_time = chrono::high_resolution_clock::now();

    int monte_rows_used = min(static_cast<int>(scodfm.size()), max_row_for_test);
    double monte_carlo_absolute_error = total_absolute_error_monte / static_cast<double>(monte_rows_used);


    //Using binomial tree

    double binomial_tree_count = 0.0;
    double total_absolute_error_binomial = 0.0;
    auto start_binomial_tree_time = chrono::high_resolution_clock::now();
    for (int i = 0; i < scodfm.size() && i < max_row_for_test; i += 1){
        double binomial_tree_price = binomial_option_price(scodfm[i].S,scodfm[i].K,scodfm[i].T, scodfm[i].r, scodfm[i].sd, 100);
        double difference = abs(binomial_tree_price - scodfm[i].call_option_price);
        total_absolute_error_binomial += difference;
    };
    auto end_binomial_tree_time = chrono::high_resolution_clock::now();

    int binomial_rows_used = min(static_cast<int>(scodfm.size()), max_row_for_test);
    double binomial_tree_absolute_error = total_absolute_error_binomial / static_cast<double>(binomial_rows_used);
    
    //Calculate the time:
    chrono::duration<double> black_scholes_time = end_black_scholes_time - start_black_scholes_time;
    chrono::duration<double> monte_carlo_time = end_monte_carlo_time - start_monte_carlo_time;
    chrono::duration<double> binomial_tree_time = end_binomial_tree_time - start_binomial_tree_time;

    cout << "The number of lines of data we use for comparison is: " << scodfm.size() << endl;
    
    cout << "\nThe Black-Scholes function mean absolute error is: $" << black_scholes_absolute_error << endl;
    cout << "The time to run the Black-Scholes function is: " << black_scholes_time.count() << " seconds" << endl;
    
    cout << "\nThe Monte-Carlo method mean absolute error is: $" << monte_carlo_absolute_error << endl;
    cout << "Number of simulations per Monte-Carlo line: " << 1000 << endl;
    cout << "The time to run the Monte-Carlo method is: " << monte_carlo_time.count() << " seconds" << endl;

    cout << "\nThe Binomial tree method mean absolute error is: $" << binomial_tree_absolute_error << endl;
    cout << "Number of time steps in the mode: " << 100 << endl;
    cout <<"The time to run the binomial tree method is: " << binomial_tree_time.count() << "seconds" << endl;
    return 0;
}