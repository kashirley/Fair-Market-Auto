import kagglehub
import pandas as pd
import numpy as np

path = kagglehub.dataset_download("zain280/car-dataset")
path = path + "\Car_Data.csv"
print("Path to dataset: ", path)

df = pd.read_csv(path)
df = df.drop('ID', axis=1)
df_CKs = df.set_index(['Brand','Model','Year','Color','Mileage', 'Price','Condition'])
print(df_CKs.head(10))


#Questions: is it okay to be using python and pulling from Kaggle - ask in slack

#get data set from kaggle
#clean data and put into database as composite keys (looks like SQL is needed)
#once cleaned load into main.cpp
#once loaded send "success" bool var to call on UI and main.cpp
#UI_called = T/F, main_called = T/F
#UI fetch menu from main
#UI takes in user input and delivers to main as string delimited by "/n"
#main parses string input 

#test input - start with small subset of data and try to load into 
