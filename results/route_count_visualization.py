#!/usr/bin/env python3

import os
import matplotlib.pyplot as plt
from collections import defaultdict

# Define the absolute path to the log files directory
log_dir = '/home/krishna/Documents/omnetpp-5.6.2/samples/TQDSimModule/results'
vulnerable_nodes = {0}

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
            count = int(count_str)
            if route not in route_counts or count > route_counts[route]:
                route_counts[route] = count

        except ValueError:
            print(f"Skipping invalid line: {line}")
    
    return route_counts

def classify_routes(route_counts, vulnerable_nodes):
    vulnerable = defaultdict(int)
    reliable = defaultdict(int)
    
    for route, count in route_counts.items():
        nodes = [int(node) for node in route.split('->')]
        if any(node in vulnerable_nodes for node in nodes):
            vulnerable[route] = count
        else:
            reliable[route] = count
    
    return vulnerable, reliable

def plot_route_counts(vulnerable, reliable, log_file_path):
    vulnerable_routes = list(vulnerable.keys())
    reliable_routes = list(reliable.keys())
    
    total_counts = sum(vulnerable.values()) + sum(reliable.values())
    vulnerable_counts = [vulnerable[route] / total_counts * 100 for route in vulnerable_routes]
    reliable_counts = [reliable[route] / total_counts * 100 for route in reliable_routes]
    print(total_counts,vulnerable_counts,reliable_counts)
    plt.figure(figsize=(12, 6))
    
    # Plot vulnerable routes
    plt.bar(vulnerable_routes, vulnerable_counts, color='red', label='Vulnerable Routes')
    # Plot reliable routes
    plt.bar(reliable_routes, reliable_counts, color='green', label='Reliable Routes')
    
    plt.xlabel('Routes choosen for $(s,t)=(1,4)$ traffic', fontsize=12)
    plt.ylabel('Selection Percentage', fontsize=12)
    plt.title('Route Selection Frequency', fontsize=12)
    plt.legend(fontsize=12)
    plt.xticks(rotation=90)
    
    # Add transparent backdrop
    plt.axvspan(-0.5, len(vulnerable_routes) - 0.5, color='red', alpha=0.1, lw=0)
    plt.axvspan(len(vulnerable_routes) - 0.5, len(vulnerable_routes) + len(reliable_routes) - 0.5, color='green', alpha=0.1, lw=0)
    
    plt.grid(True)
    
    # Save the plot to a file
    plot_file_path = log_file_path.replace('route_log', 'route_count').replace('.txt', '.png')
    plt.tight_layout()
    plt.savefig(plot_file_path)
    plt.close()
    
    print(f"Saved plot to {plot_file_path}")

def main():
    log_files = [f for f in os.listdir(log_dir) if f.startswith('route_log') and f.endswith('.txt')]
    
    for log_file in log_files:
        log_file_path = os.path.join(log_dir, log_file)
        print(f"Processing {log_file_path}")
        
        route_counts = parse_log_file(log_file_path)
        
        vulnerable, reliable = classify_routes(route_counts, vulnerable_nodes)
        
        plot_route_counts(vulnerable, reliable, log_file_path)

if __name__ == "__main__":
    main()

