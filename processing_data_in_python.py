import csv
import time

from black_scholes import european_call_option_price
from monte_carlo import monte_carlo_european_call_option_price
from binomial_tree import european_call_binomial


class OptionData:
    def __init__(self, S, K, T, sd, r, call_option_price):
        self.S = S
        self.K = K
        self.T = T
        self.sd = sd
        self.r = r
        self.call_option_price = call_option_price


def read_option_data(filename, max_rows_to_read_and_test):
    data = []

    with open(filename, "r", newline="") as file:
        reader = csv.reader(file)

        # Skip the header row
        next(reader)

        for columns in reader:
            if len(data) >= max_rows_to_read_and_test:
                break

            try:
                row = OptionData(
                    S=float(columns[2]),
                    K=float(columns[3]),
                    T=float(columns[5]),
                    sd=float(columns[6]),
                    r=float(columns[7]),
                    call_option_price=float(columns[8])
                )

                if row.S <= 0 or row.K <= 0 or row.T <= 0 or row.sd <= 0:
                    continue

                data.append(row)

            except ValueError:
                continue

    return data


def main():
    filename = "european_call_option_data_2016_2020.csv"

    max_rows_to_read_and_test = 30000
    monte_carlo_simulations = 1000
    binomial_steps = 100

    option_data = read_option_data(filename, max_rows_to_read_and_test)

    if len(option_data) == 0:
        print("No valid data was found.")
        return

    print(f"The number of lines of data we use for comparison is: {len(option_data)}")

    # Black-Scholes method
    total_absolute_error_black_scholes = 0.0

    start_black_scholes_time = time.perf_counter()

    for row in option_data:
        black_scholes_price = european_call_option_price(
            row.S,
            row.K,
            row.T,
            row.r,
            row.sd
        )

        difference = abs(black_scholes_price - row.call_option_price)
        total_absolute_error_black_scholes += difference

    end_black_scholes_time = time.perf_counter()

    black_scholes_absolute_error = (
        total_absolute_error_black_scholes / len(option_data)
    )

    black_scholes_time = end_black_scholes_time - start_black_scholes_time

    # Monte Carlo method
    total_absolute_error_monte_carlo = 0.0

    start_monte_carlo_time = time.perf_counter()

    for row in option_data:
        monte_carlo_price = monte_carlo_european_call_option_price(
            monte_carlo_simulations,
            row.S,
            row.K,
            row.T,
            row.r,
            row.sd
        )

        difference = abs(monte_carlo_price - row.call_option_price)
        total_absolute_error_monte_carlo += difference

    end_monte_carlo_time = time.perf_counter()

    monte_carlo_absolute_error = (
        total_absolute_error_monte_carlo / len(option_data)
    )

    monte_carlo_time = end_monte_carlo_time - start_monte_carlo_time

    # Binomial tree method
    total_absolute_error_binomial = 0.0

    start_binomial_tree_time = time.perf_counter()

    for row in option_data:
        binomial_tree_price = european_call_binomial(
            row.S,
            row.K,
            row.T,
            row.r,
            row.sd,
            binomial_steps
        )

        difference = abs(binomial_tree_price - row.call_option_price)
        total_absolute_error_binomial += difference

    end_binomial_tree_time = time.perf_counter()

    binomial_tree_absolute_error = (
        total_absolute_error_binomial / len(option_data)
    )

    binomial_tree_time = end_binomial_tree_time - start_binomial_tree_time

    print()
    print(f"The Black-Scholes function mean absolute error is: ${black_scholes_absolute_error}")
    print(f"The time to run the Black-Scholes function is: {black_scholes_time} seconds")

    print()
    print(f"The Monte-Carlo method mean absolute error is: ${monte_carlo_absolute_error}")
    print(f"Number of simulations per Monte-Carlo line: {monte_carlo_simulations}")
    print(f"The time to run the Monte-Carlo method is: {monte_carlo_time} seconds")

    print()
    print(f"The Binomial tree method mean absolute error is: ${binomial_tree_absolute_error}")
    print(f"Number of time steps in the model: {binomial_steps}")
    print(f"The time to run the binomial tree method is: {binomial_tree_time} seconds")


if __name__ == "__main__":
    main()