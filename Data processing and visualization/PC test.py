import serial
import numpy as np
import csv
import folium
import pandas as pd
import os
import json
import webbrowser

def create_csv():
    path = "GPS.csv"
    with open(path,'wb') as f:
        csv_write = csv.writer(f)
    
ser=serial.Serial()
ser.baudrate=9600
ser.port='COM3'
ser.open()
create_csv()

lon=[]
lat=[]

m = folium.Map(
    location=[45.45451171,126.404873],
    zoom_start=12,
    tiles='Stamen Terrain')

os.chdir(r"C:\Users\DELL\Desktop\桌面文件夹\大创\程序\HC-12无线433Mhz串口模块使用资料包\python")
for i in range(1,10):
    data=ser.read(37)
    data=data.decode()
    latitude=data.split(',')[1]
    longitude=data.split(',')[2]
    Deep=data.split(',')[3]
  
    lon=float(longitude)/100
    lat=float(latitude)/100
    
    with open("GPS.csv",'a+') as f:
        csv_write = csv.writer(f)
        csv_data = [lat,lon,Deep]
        csv_write.writerow(csv_data)

    df = pd.DataFrame(data=[Deep], columns=['Deep'])
    html = df.to_html()
    folium.Marker(
        location=[lat,lon],
        popup=folium.Popup(html,max_width=450)).add_to(m)

file_path = r"C:\Users\DELL\Desktop\marker_chart.html"    
m.save(file_path)
    
webbrowser.open(file_path)


    
