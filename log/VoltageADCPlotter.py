# -*- coding: utf-8 -*-
"""
Created on Tue Nov 13 21:37:07 2024

@author: Ahron Ramos, Adrian Co, Zaira Ramji

@description: ADC Data Processor and Visualizer. 
              Reads ADC and duty cycle data from serial port, 
              processes into DataFrame, saves to CSV, 
              and creates interactive plots.
              
This script performs four main functions:
1. Reading raw ADC and duty cycle data from a serial connection
2. Processing and structuring the raw data
3. Saving the processed data to a CSV file
4. Creating interactive visualizations of the data
"""

# Import required libraries
from plotly.subplots import make_subplots
import plotly.graph_objects as go
import plotly.io as pio
import pandas as pd
import time 
import serial

# Set plotly to open plots in browser
pio.renderers.default = "browser"

def read_serial_data(serial_conn: serial.Serial, duration: float) -> tuple[str, list]:
    """
    Read serial data for specified duration and collect timestamps.
    
    The function:
    1. Reads data line by line from the serial port
    2. Filters out empty lines and whitespace
    3. Records timestamp for each reading
    4. Continues reading until specified duration is reached
    
    Args:
        serial_conn: Serial connection object for communication
        duration: How long to record data, in seconds
    
    Returns:
        Tuple containing:
        - Raw data string with all readings
        - List of timestamps for each reading
    """
    data_lines = []      # Store raw data lines
    time_stamps = []     # Store corresponding timestamps
    start_time = time.time()
    
    # Read data until duration is reached
    while (time.time() - start_time < duration):
        if line := serial_conn.readline():  # Read a line if available
            if line not in [b' \n', b'\n']: # Skip empty lines
                data_lines.append(line.decode("ascii"))
                time_stamps.append(time.time() - start_time)
    
    serial_conn.close()
    return ''.join(data_lines).strip(), time_stamps


def process_data(data_str: str, time_stamps: list) -> tuple[list, list, list]:
    """
    Process raw serial data into structured data arrays.
    
    The function:
    1. Splits raw data into lines
    2. Extracts duty cycle and ADC values from each line
    3. Converts string values to integers
    4. Removes first and last readings to avoid partial data
    
    Args:
        data_str: Raw data string containing all readings
        time_stamps: List of timestamps for each reading
    
    Returns:
        Tuple containing:
        - List of timestamps
        - List of duty cycle values (LED intensity)
        - List of ADC buffer values (light sensor readings)
    """
    data_lines = data_str.split('\n')[1:-1]  # Split into lines, remove first/last
    duty_cycle_values = []                   # Store LED intensity values
    adc_buffer_values = []                   # Store light sensor readings
    
    # Process each line of data
    for line in data_lines:
        if values := line.strip().split():  # Split line into values
            if len(values) >= 2:            # Ensure both values exist
                duty_cycle_values.append(int(values[0]))  # First value is duty cycle
                adc_buffer_values.append(int(values[1]))  # Second value is ADC reading
    
    return time_stamps[1:-1], duty_cycle_values, adc_buffer_values


def save_to_csv(time_stamps: list, duty_cycle_values: list, 
                adc_buffer_values: list, filename: str = "Group_26.csv") -> None:
    """
    Save processed data to CSV file for later analysis.
    
    Creates a DataFrame with columns:
    - Index: Sequential number for each reading
    - Time: Timestamp of reading
    - Intensity: LED duty cycle percentage
    - ADC Value: Raw ADC reading from light sensor
    
    Args:
        time_stamps: List of reading timestamps
        duty_cycle_values: List of LED intensity values
        adc_buffer_values: List of ADC readings
        filename: Name of output CSV file
    """
    df = pd.DataFrame({
        "Time": time_stamps,
        "Intensity": duty_cycle_values,
        "ADC Value": adc_buffer_values
    })
    df.to_csv(filename, index=True, index_label="Index")


def plot_data(time_stamps: list, duty_cycle_values: list, adc_buffer_values: list) -> None:
    """
    Create interactive dual plots of ADC readings and LED intensity.
    
    Creates two side-by-side plots:
    1. ADC Reading vs Time: Shows raw light sensor readings
    2. LED Intensity vs Time: Shows duty cycle percentage
    
    Args:
        time_stamps: List of reading timestamps
        duty_cycle_values: List of LED intensity values
        adc_buffer_values: List of ADC readings
    """
    # Create DataFrame for plotting
    df = pd.DataFrame({
        "Time": time_stamps,
        "Intensity": duty_cycle_values,
        "ADC Value": adc_buffer_values
    })
    
    # Create subplot layout
    fig = make_subplots(
        rows=1, cols=2,
        subplot_titles=("ADC Reading [Raw]", "LED Intensity"),
    )
    
    # Add ADC Value trace (blue line)
    fig.add_trace(
        go.Scatter(x=df["Time"], y=df["ADC Value"], 
                  mode="lines", name="ADC Value"),
        row=1, col=1
    )
    
    # Add Intensity trace (red line)
    fig.add_trace(
        go.Scatter(x=df["Time"], y=df["Intensity"], 
                  mode="lines", name="Intensity", 
                  line=dict(color="red")),
        row=1, col=2
    )
    
    # Update axis labels
    fig.update_xaxes(title_text="Time (s)", row=1, col=1)
    fig.update_xaxes(title_text="Time (s)", row=1, col=2)
    fig.update_yaxes(title_text="ADC Reading", row=1, col=1)
    fig.update_yaxes(title_text="Intensity (Duty Cycle %)", row=1, col=2)
    
    fig.show()


if __name__ == "__main__":
    serial_conn = serial.Serial(
        port="COM5",          
        baudrate=4800,      
        bytesize=8,
        timeout=2,
        stopbits=serial.STOPBITS_ONE
    )
    
    raw_data, time_stamps = read_serial_data(serial_conn, 60)
    time_stamps, duty_cycle_values, adc_buffer_values = process_data(raw_data, time_stamps)
    save_to_csv(time_stamps, duty_cycle_values, adc_buffer_values)
    plot_data(time_stamps, duty_cycle_values, adc_buffer_values)