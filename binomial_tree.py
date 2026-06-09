import numpy as np

def european_call_binomial(S, K, T, r, σ, N):
    """
    Binomial Option Pricing Model (CRR Model) for a European Call Option.
    Optimized using a 1D array to minimize memory overhead.
    
    Parameters:
    S     : float - Current stock price (spot price)
    K     : float - Strike price (exercise price)
    T     : float - Time to expiration in years
    r     : float - Annualized risk-free interest rate (continuously compounded)
    σ     : float - Annualized stock volatility
    N     : int   - Number of time steps
    """
    if S <= 0 or K <= 0 or T <= 0 or N <= 0:
        return 0.0
        
    # 1. Calculate time step and model parameters
    dt = T / N
    u = np.exp(σ * np.sqrt(dt))
    d = 1.0 / u
    p = (np.exp(r * dt) - d) / (u - d)
    discount = np.exp(-r * dt)
    
    # Boundary defense: If volatility is near zero or probabilities distort,
    # fallback to the deterministic intrinsic value discounting.
    if σ < 1e-4 or p < 0.0 or p > 1.0:
        return max(S - K * np.exp(-r * T), 0.0)
        
    # 2. Initialize 1D array with option payoffs at expiration (Step N)
    # European Call Payoff formula: Max(S_expiry - K, 0)
    option_values = np.zeros(N + 1)
    for i in range(N + 1):
        stock_at_expiry = S * (u ** (N - i)) * (d ** i)
        option_values[i] = max(stock_at_expiry - K, 0.0)
        
    # 3. Backward Induction
    # Roll back from step N-1 down to step 0, overwriting the array in place.
    for step in range(N - 1, -1, -1):
        for i in range(step + 1):
            option_values[i] = discount * (p * option_values[i] + (1.0 - p) * option_values[i + 1])
            
    # The first element now represents the root node (the option price today)
    return option_values[0]
