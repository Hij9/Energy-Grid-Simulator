# Python file that gets the weather data to solve the behind-the-meter gap and Thermal Capacitance ODE
import sys
import pandas as pd

# Grab the data from C++
startDate = sys.argv[1]
endDate = sys.argv[2]
path = sys.argv[3]

# Convret to workable date/time datatype
startDate = pd.to_datetime(startDate).strftime("%Y-%m-%d")
endDate = pd.to_datetime(endDate).strftime("%Y-%m-%d")

# Initialize dataframe and create the CSV file
df = pd.read_csv(f"https://historical-forecast-api.open-meteo.com/v1/forecast?latitude=36.0652&longitude=-119.0168&start_date={startDate}&end_date={endDate}&minutely_15=temperature_2m,shortwave_radiation&timezone=America%2FLos_Angeles&format=csv",skiprows=3, storage_options={"User-Agent" : "PUSD-Energy-Simulator/1.0"})
df.to_csv(path,index=False,header=False)