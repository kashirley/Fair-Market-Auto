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