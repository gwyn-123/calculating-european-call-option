import numpy as np
import matplotlib.pyplot as plt



# Black-Scholes runtime in microseconds
black_scholes_cpp_microseconds = 13400
black_scholes_python_microseconds = 74800

# Monte Carlo and Binomial runtime in seconds
methods_seconds = ["Monte Carlo", "Binomial"]

cpp_times_seconds = [5.158, 3.17]
python_times_seconds = [0.937, 59.2]


# 
# GRAPH 1: BLACK-SCHOLES
# 

languages = ["C++", "Python"]
black_scholes_times = [
    black_scholes_cpp_microseconds,
    black_scholes_python_microseconds
]

plt.figure(figsize=(7, 5))

plt.bar(
    languages,
    black_scholes_times,
    color=["purple", "green"]
)

plt.xlabel("Language")
plt.ylabel("Time (microseconds)")
plt.title("Black-Scholes Runtime Comparison")

plt.tight_layout()
plt.show()


# 
# GRAPH 2: MONTE CARLO AND BINOMIAL
# 

x = np.arange(len(methods_seconds))
bar_width = 0.35

plt.figure(figsize=(8, 5))

plt.bar(
    x - bar_width / 2,
    cpp_times_seconds,
    width=bar_width,
    label="C++",
    color="purple"
)

plt.bar(
    x + bar_width / 2,
    python_times_seconds,
    width=bar_width,
    label="Python",
    color="green"
)

plt.xlabel("Method")
plt.ylabel("Time (seconds)")
plt.title("Monte Carlo and Binomial Runtime Comparison")

plt.xticks(x, methods_seconds)

plt.legend(loc="upper left")

plt.tight_layout()
plt.show()