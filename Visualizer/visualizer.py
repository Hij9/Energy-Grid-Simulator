# Python for the frontend visualizer of the simulation
# Use python3 Visualizer/visualizer.py to run this file - or just use the button in the top right
# Because I'm very new to python data science capabilities, I'm going to over-comment to understand what I'm doing

import sys
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Catching from C++
baseGraphPath = f"External Files/Simulator Output/{sys.argv[1]}"

# Differentiate between baseline graph and simulated graphs (and not have the same filename)
baseOrSim = "Baseline"
if (sys.argv[1] == "Simulated_Graphs"):
    baseOrSim = "Simulated"

# Tables/vars

columns = ["Tick","Date","Timestamp","Net Load","Entity Name","Square Footage"]
relativeFilePath = "External Files/Simulator Output/Non-Visualizer Files/SimulatorOutputFile.csv"    # Camels > Snakes python pedants
daysOfWeek = ["Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"]
monthsOfYear = ["January","February","March","April","May","June","July","August","September","October","November","December"]

# Setup of Visualization/Graphs

df = pd.read_csv(relativeFilePath,names=columns)    # Create DataFrame (df) object to work with

df["Timestamp"] = df["Timestamp"].str.strip()   # Strip the whitespace off of the timestamp column

df["Timestamp"] = pd.to_datetime(df["Timestamp"],format="%m/%d/%Y %I:%M%p")    # Convert timestamp column to native datetime for easier processing

# Create Hour and Day Of Week Columns at the right side of the table
df["Hour"] = df["Timestamp"].dt.hour
df["Day Of Week"] = df["Timestamp"].dt.day_name()
df["Month Of Year"] = df["Timestamp"].dt.month_name()

# Use Boolean Masking to create two versions of df, one with the grid and one without the grid
schoolsTable = df[df["Entity Name"] != "PUSD_Grid"].copy()
gridTable = df[df["Entity Name"] == "PUSD_Grid"].copy()

# Use vectorization to get scaled EUI per school and create pivot tables for graphs
schoolsTable["sEUI"] = (schoolsTable["Net Load"] / schoolsTable["Square Footage"])*1000

weekHour = pd.pivot_table(schoolsTable,index="Day Of Week", columns="Hour",values="sEUI",aggfunc="mean")
monthWeek = pd.pivot_table(schoolsTable,index="Day Of Week", columns="Month Of Year",values="sEUI",aggfunc="mean")

# Ensure all is chronological
weekHour = weekHour.reindex(daysOfWeek)
monthWeek = monthWeek.reindex(index=daysOfWeek,columns=monthsOfYear)

# Use groupby() to create a leaderboard of schools after averaging and sorting the values
Leaderboard = schoolsTable.groupby("Entity Name")["sEUI"].mean().sort_values(ascending=False)


# Graphing

# District weekHour heatmap generation
plt.figure(figsize=(12,6))
sns.heatmap(weekHour,cmap="coolwarm",center=0.0,cbar_kws={"label" : "sEUI (1000*(kWh / sq ft))"})
plt.title(f"{baseOrSim} District Hour & Day Of Week vs. Scaled EUI (sEUI)")
plt.savefig(f"{baseGraphPath}/District Graphs/{baseOrSim}_District_Weekly_Heatmap.png")
plt.close()

# District monthWeek heatmap generation
plt.figure(figsize=(12,8))
sns.heatmap(monthWeek,cmap="coolwarm",center=0.0,cbar_kws={"label" : "sEUI (1000*(kWh / sq ft))"})
plt.title(f"{baseOrSim} District Day Of Week & Month Of Year vs. Scaled EUI (sEUI)")
plt.savefig(f"{baseGraphPath}/District Graphs/{baseOrSim}_District_Monthly_Heatmap.png")
plt.close()

# gridTable line chart
gridChart = gridTable.groupby("Hour")["Net Load"].mean()
plt.figure(figsize=(12,6))
sns.lineplot(gridChart)
plt.xlabel("Hour")
plt.ylabel("Net Load (kWh)")
plt.title(f"{baseOrSim} District Hours vs. Net Load")
plt.savefig(f"{baseGraphPath}/District Graphs/{baseOrSim}_District_Line_Chart.png")
plt.close()

# Campus sEUI Ranking
plt.figure(figsize=(18,9))
sns.barplot(x=Leaderboard.values,y=Leaderboard.index)
plt.title(f"{baseOrSim} District Leaderboard of sEUI of Schools")
plt.savefig(f"{baseGraphPath}/District Graphs/{baseOrSim}_District_Leaderboard.png")
plt.close()

# Graphs for each school
for i in schoolsTable.groupby("Entity Name"):
    # Save vars, one for name of school other for data belonging to that school
    schoolName = i[0]
    schoolData = i[1]
    schoolFolderPath = f"{baseGraphPath}/Schools/{schoolName}"

    # Make folders based on how many schools show up in the csv report thingy
    os.makedirs(schoolFolderPath,exist_ok=True)

    # Pivot tabe for graphs
    schoolWeekHour = pd.pivot_table(schoolData,index="Day Of Week", columns="Hour",values="sEUI",aggfunc="mean")
    schoolMonthWeek = pd.pivot_table(schoolData,index="Day Of Week", columns="Month Of Year",values="sEUI",aggfunc="mean")
    schoolWeekHour = schoolWeekHour.reindex(daysOfWeek)
    schoolMonthWeek = schoolMonthWeek.reindex(index=daysOfWeek,columns=monthsOfYear)

    # Graphing and saving
    plt.figure(figsize=(12,6))
    sns.heatmap(schoolWeekHour,cmap="coolwarm",center=0.0,cbar_kws={"label" : "sEUI (1000*(kWh / sq ft))"})
    plt.title(f"{schoolName} {baseOrSim} Hour & Day Of Week vs. Scaled EUI (sEUI)")
    plt.savefig(f"{schoolFolderPath}/{schoolName}_{baseOrSim}_Weekly_Heatmap.png")
    plt.close()

    plt.figure(figsize=(12,8))
    sns.heatmap(schoolMonthWeek,cmap="coolwarm",center=0.0,cbar_kws={"label" : "sEUI (1000*(kWh / sq ft))"})
    plt.title(f"{schoolName} {baseOrSim} Day of Week & Month Of Year vs. Scaled EUI (sEUI)")
    plt.savefig(f"{schoolFolderPath}/{schoolName}_{baseOrSim}_Monthly_Heatmap.png")
    plt.close()

    schoolChart = schoolData.groupby("Hour")["Net Load"].mean()
    plt.figure(figsize=(12,6))
    sns.lineplot(schoolChart)
    plt.xlabel("Hour")
    plt.ylabel("Net Load (kWh)") 
    plt.title(f"{schoolName} {baseOrSim} Hours vs. Net Load")
    plt.savefig(f"{schoolFolderPath}/{schoolName}_{baseOrSim}_Line_Chart.png")
    plt.close()