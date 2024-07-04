from flask import Flask, request, jsonify
import geopandas as gpd
from shapely.geometry import Point

app = Flask(__name__)

@app.route('/update_location', methods=['POST'])
def update_location():
    data = request.json
    latitude = data.get('latitude')
    longitude = data.get('longitude')
    # Create a GeoDataFrame with a single Point geometry
    data = {
        'id': [1],
        'latitude': [latitude],
        'longitude': [longitude]
    }
    # Create Point geometry
    geometry = [Point(longitude, latitude)]

    # Create the GeoDataFrame
    gdf = gpd.GeoDataFrame(data, geometry=geometry, crs='EPSG:4326')
    # Define the output GeoPackage file path
    output_gpkg = 'example.gpkg'

    # Export GeoDataFrame to GeoPackage
    gdf.to_file(output_gpkg, driver='GPKG')
    if latitude is not None and longitude is not None:
        with open("gps_data.txt", "a") as file:
            file.write(f"Latitude: {latitude}, Longitude: {longitude}\n")
        print(f"Received location - Latitude: {latitude}, Longitude: {longitude}")
        return jsonify({"status": "success", "latitude": latitude, "longitude": longitude}), 200
    else:
        return jsonify({"status": "error", "message": "Invalid data"}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
