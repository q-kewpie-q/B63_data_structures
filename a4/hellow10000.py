import random
import matplotlib.pyplot as plt
from collections import Counter

def hello_world_simulation(n, k):
    count = 0
    while True:
        print("Hello World")
        count += 1
        i = random.randint(1, n)
        if i <= k:
            break
    return count

def run_simulation(num_simulations, n, k):
    results = []
    for _ in range(num_simulations):
        count = hello_world_simulation(n, k)
        results.append(count)
    return results

def plot_results(results):
    # Count the frequency of each result
    result_counts = Counter(results)
    
    # Create the bar plot
    plt.bar(result_counts.keys(), result_counts.values())
    plt.xlabel('Number of "Hello World" Prints')
    plt.ylabel('Frequency')
    plt.title('Printing Frequency of "Hello World" Over 10000 Simulations')
    plt.show()

if __name__ == "__main__":
    n = 10  # You can set n to any reasonable value
    k = 3   # You can set k to any reasonable value
    
    num_simulations = 10000
    results = run_simulation(num_simulations, n, k)
    plot_results(results)
