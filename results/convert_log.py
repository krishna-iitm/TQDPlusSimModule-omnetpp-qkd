#!/usr/bin/env python3

import os
import glob

# Define the path pattern for the log files
log_file_pattern = 'route_log_*.txt'

def parse_log_file(log_file_path):
    route_counts = {}
    
    with open(log_file_path, 'r') as log_file:
        lines = log_file.readlines()
    
    # Process lines from bottom to top
    for line in reversed(lines):
        line = line.strip()
        if not line:
            continue
        
        try:
            # Split the line into route and count parts
            route, count_str = line.split(':', 1)
            # Convert count part to integer
            count = int(count_str)
            # Update the dictionary with the latest count for each route
            # Only update if the new count is greater (ensure latest count is stored)
            if route not in route_counts or count > route_counts[route]:
                route_counts[route] = count

        except ValueError:
            print(f"Skipping invalid line: {line}")
    
    return route_counts

def write_txt_file(route_counts, txt_file_path):
    with open(txt_file_path, 'w') as txt_file:
        for route, count in route_counts.items():
            txt_file.write(f"{route}:{count}\n")

def main():
    # Find all log files matching the pattern
    log_files = glob.glob(log_file_pattern)
    
    for log_file in log_files:
        # Generate the corresponding output file name
        base_name = os.path.splitext(os.path.basename(log_file))[0]
        txt_file_name = f"route_count_{base_name}.txt"
        
        # Parse the log file and write to the corresponding output file
        route_counts = parse_log_file(log_file)
        
        # Debugging: Print the parsed route counts
        print(f"Parsed Route Counts for {log_file}:")
        for route, count in route_counts.items():
            print(f"{route}: {count}")
        
        write_txt_file(route_counts, txt_file_name)
        print(f"Text file '{txt_file_name}' has been created with route counts.")

if __name__ == "__main__":
    main()

