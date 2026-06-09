import math

#Calculating the height of the probability density function at x in standard normal distribution:
def norm_pdf(x: float) -> float:
    return 1 / math.sqrt(2 * math.pi) * math.exp(-x**2 / 2)

"""
Calculating the value of the integration N(d), using the Abramowitz and Stegun approximation
of the CDF of the standard normal function using polynomial:
For x >= 0:
big_phi(x) = 1- small_phi(x)(0.319381530*k − 0.356563782*pow(k,2) + 1.781477937*pow(k,3)
            −1.821255978*pow(k,4) + 1.330274429*pow(k,5))

For x < 0:
big_phi(x) = 1 - big_phi(-x)
note: small_phi(x) is the height of the PDF at x
k = 1/(1 + 0.2316419*x)
"""

def norm_cdf(y: float) -> float:
    k = 1 / (1 + 0.2316419 * y)
    sum_k = (  0.319381530 * k
             - 0.356563782 * k**2
             + 1.781477937 * k**3
             - 1.821255978 * k**4
             + 1.330274429 * k**5)
    if y >= 0:
        return 1 - norm_pdf(y) * sum_k
    else:
        return 1.0 - norm_cdf(-y)

#Calcualting d_1 and d_2 value in general (d_j)
def d_j(j: int, S: float, K: float, T: float, r: float, σ: float) -> float:
    d1 = (1 / (σ * math.sqrt(T))) * (math.log(S / K) + (r + σ**2 / 2) * T)
    d2 = d1 - σ * math.sqrt(T)
    return (2 - j) * d1 + (j - 1) * d2
    #σ is the standard deviation or σ in the above note

#Calculating the European Call Option price:

def european_call_option_price(S: float, K: float, T: float, r: float, σ: float) -> float:
    PV_K = K * math.exp(-r * T)
    return norm_cdf(d_j(1, S, K, T, r, σ)) * S - norm_cdf(d_j(2, S, K, T, r, σ)) * PV_K

