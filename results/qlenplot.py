#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
from multiprocessing import Pool
import numpy as np  # Import numpy for sqrt function

# Function to process each chunk of data
def process_chunk(chunk, start_index):
    # Extract the last column (excluding the header)
    last_column_data = chunk.iloc[:, -1]
    
    # Generate row numbers (starting from the appropriate index)
    row_numbers = range(start_index, start_index + len(last_column_data))
    
    # Calculate y-axis data (last column data divided by row numbers)
    y_data = last_column_data / row_numbers
    
    return list(row_numbers), list(y_data)

# Function to plot data
def plot_data(row_numbers, y_data):
    plt.figure(figsize=(10, 6))
    
    # Plot the original data
    plt.plot(row_numbers, y_data, label='Sum Queue Growth Rate', linewidth=2)
    
    # Calculate and plot sqrt(T) vs T
    sqrt_row_numbers = np.sqrt(np.array(row_numbers))
    sqrt_row_numbers_by_T = sqrt_row_numbers / row_numbers
    plt.plot(row_numbers, sqrt_row_numbers_by_T, label='$\sqrt{T}/T$', linestyle='--', color='red')
    
    # Setting grid, font size, and labels
    plt.grid(True)
    plt.xlabel('T', fontsize=12)
    plt.ylabel(r'$\sum_{e \in E} \frac{\tilde{X}_e(T) + \tilde{Y}_e(T)}{T}$', fontsize=12)
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)
    plt.legend(fontsize=12)  # Add a legend to distinguish the lines
    
    plt.show()

# Main function
def main(csv_file):
    chunksize = 10000  # Adjust this based on available memory
    start_index = 1
    
    with Pool(processes=12) as pool:
        results = []
        for chunk in pd.read_csv(csv_file, chunksize=chunksize):
            result = pool.apply_async(process_chunk, (chunk, start_index))
            results.append(result)
            start_index += len(chunk)
        
        all_row_numbers = []
        all_y_data = []
        
        for result in results:
            row_numbers, y_data = result.get()
            all_row_numbers.extend(row_numbers)
            all_y_data.extend(y_data)
    
    plot_data(all_row_numbers, all_y_data)

# Replace 'your_csv_file.csv' with the actual CSV file name
csv_file = 'qlen0.95,1.2,100.csv'
main(csv_file)

