#include <iostream>
#include <cmath>
#include <cassert>
#include <chrono> //library for measuring time

#include "black_scholes.hpp"
#include "monte_carlo.hpp"

using namespace std;

int main(){
    int no_of_sim;   // Number of simulated asset paths 
    double S;        // Current stock price
    double K;        // Strike price
    double T;        // Time for the option to expire in years
    double r;        // Annualised risk-free interest rate
    double sd;       // Volatility

    //Entering the values
    cout << "Enter the values to calculate the European Call Option price using Monte Carlo method: \n";
    cout << "Enter the number of simulations: ";
    cin  >> no_of_sim;
    cout << "Enter the current stock price: $";
    cin  >> S;
    cout << "Enter the strike price: $";
    cin  >> K;
    cout << "Enter the time for the option to expire (years): ";
    cin  >> T;
    cout << "Enter the annualised risk-free interest rate (%): ";
    cin  >> r;
    cout << "Enter the volatility (%): ";
    cin >> sd;

    //Measuring the time to calculate the price using Black Scholes
    auto start_black_scholes_time = chrono::high_resolution_clock::now();

    double black_scholes_price = european_call_option_price(S,K,T, r*0.01, sd*0.01);
    
    auto end_black_scholes_time = chrono::high_resolution_clock::now();

    //Measuring the time to calculate the price using Monte Carlo
    auto start_monte_carlo_time = chrono::high_resolution_clock::now();

    double monte_carlo_price = monte_carlo_european_call_option_price(no_of_sim, S,K,T, r*0.01, sd*0.01);

    auto end_monte_carlo_time = chrono::high_resolution_clock::now();

    cout << "\nBlack-Scholes price: " << black_scholes_price << endl;
    cout << "Monte Carlo price: " << monte_carlo_price << endl;

    /*'
    Now we check if the Monte Carlo is accurate, it is based on random distribution so we will compare
    the output againt black scholes which is a closed form solution and we will determine if it is 
    accurate enough by comparing the difference between the two price to a fixed error (0.25) which
    is considered acceptable
    */
    double difference = abs(black_scholes_price - monte_carlo_price);
    if (difference < 0.25){
        cout << "\nMonte Carlo output is accurate enough" << endl;
    }
    else {
        cout << "Monte Carlo output is not accurate enough" << endl;
    };

    /*
    Now we calculate and compare the time for Black Scholes and and Monte Carlo
    */
    auto black_scholes_time = chrono::duration_cast<chrono::microseconds>(end_black_scholes_time - start_black_scholes_time);
    auto monte_carlo_time = chrono::duration_cast<chrono::microseconds>(end_monte_carlo_time - start_monte_carlo_time);

    cout << "\nBlack-Scholes time: " <<  black_scholes_time.count() << " microseconds" << endl;
    cout << "Monte Carlo time: " << monte_carlo_time.count() << " milliseconds" << endl;
    return 0;
}