import xml.etree.ElementTree as ET
import numpy as np
from filterpy.kalman import KalmanFilter
import matplotlib.pyplot as plt
import datetime

# parse the GPX file
tree = ET.parse('myown.gpx')
root = tree.getroot()

# extract the latitude, longitude, and timestamp information from each <trkpt> element
gps_data = []
for trkpt in root.findall('.//{http://www.topografix.com/GPX/1/1}trkpt'):
    lat = float(trkpt.get('lat'))
    lon = float(trkpt.get('lon'))
    time_str = trkpt.find('{http://www.topografix.com/GPX/1/1}time').text
    time = datetime.datetime.strptime(trkpt.find('{http://www.topografix.com/GPX/1/1}time').text, '%Y-%m-%dT%H:%M:%SZ')
    gps_data.append((time, lat, lon))

# define the Kalman filter
kf = KalmanFilter(dim_x=2, dim_z=1)
kf.x = np.array([gps_data[0][1], 0])
kf.F = np.array([[1, 1],
                 [0, 1]])
kf.H = np.array([[1, 0]])
kf.P *= 1000 * np.eye(2)
kf.R = np.array([[0.01]])
kf.Q = np.array([[0.01]])

# apply the Kalman filter to the latitude data
filtered_lat_data = []
for data in gps_data:
    kf.predict()
    kf.update(data[1])
    filtered_lat_data.append((data[0], kf.x[0]))

# apply the Kalman filter to the longitude data
kf = KalmanFilter(dim_x=2, dim_z=1)
kf.x = np.array([gps_data[0][2], 0])
kf.F = np.array([[1, 1],
                 [0, 1]])
kf.H = np.array([[1, 0]])
kf.P *= 1000 * np.eye(2)
kf.R = np.array([[0.01]])
kf.Q = np.array([[0.01]])

filtered_lon_data = []
for data in gps_data:
    kf.predict()
    kf.update(data[2])
    filtered_lon_data.append((data[0], kf.x[0]))

# plot unfiltered and filtered latitude vs time on one plot
fig, ax = plt.subplots()
ax.plot([data[0] for data in gps_data], [data[1] for data in gps_data], 'b.', label='Unfiltered')
ax.plot([data[0] for data in filtered_lat_data], [data[1] for data in filtered_lat_data], 'g-', label='Filtered')
ax.legend()
ax.set_title('Latitude vs Time')
ax.set_xlabel('Time (seconds)')
ax.set_ylabel('Latitude')
plt.show()

# plot unfiltered and filtered longitude vs time on one plot
fig, ax = plt.subplots()
ax.plot([data[0] for data in gps_data], [data[2] for data in gps_data], 'b.', label='Unfiltered')
ax.plot([data[0] for data in filtered_lon_data], [data[1] for data in filtered_lon_data], 'g-', label='Filtered')
ax.legend()
ax.set_title('Longitude vs Time')
ax.set_xlabel('Time (seconds)')
ax.set_ylabel('Longitude')
plt.show()
