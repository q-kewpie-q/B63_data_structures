import matplotlib.pyplot as plt

# Data
sizes = [10, 100, 1000, 5000, 10000, 50000, 100000, 500000, 1000000]
times_sajad = [0.000002, 0.000009, 0.000024, 0.000115, 0.000231, 0.001153, 0.002315, 0.011566, 0.023649]
times_elaheh = [0.000003, 0.000024, 0.000100, 0.000630, 0.001365, 0.008066, 0.017219, 0.097876, 0.207078]

# Normalize x-axis values to be equidistant
normalized_sizes = list(range(len(sizes)))

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(normalized_sizes, times_sajad, marker='o', label='Time_Sajad')
plt.plot(normalized_sizes, times_elaheh, marker='o', label='Time_Elaheh')

plt.xlabel('Input Size (normalized)')
plt.ylabel('Time (seconds)')
plt.title('Comparison of buildHeap_Sajad and buildHeap_Elaheh')
plt.legend()
plt.grid(True, which="both", ls="--")

# Set the xticks to the original sizes for better readability
plt.xticks(normalized_sizes, sizes)

# Save as JPG
plt.savefig('build_heap_comparison_normalized.jpg', format='jpg')
plt.show()
