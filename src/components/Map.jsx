import "../App.css";
import "leaflet/dist/leaflet.css";
import "react-leaflet-markercluster/dist/styles.min.css";
import { useEffect, useState } from "react";

import osmtogeojson from "osmtogeojson";

import {
  MapContainer,
  TileLayer,
  useMap,
  Marker,
  Popup,
  marker,
  useMapEvents,
  Polyline,
} from "react-leaflet";

import L, { point } from "leaflet";

function MapEvents({ roads, setRoads }) {
  const map = useMap();

  const colors = ["lime", "white", "blue", "red", "green", "pink"];

  const updateRoads = (e) => {
    var zoom = e.target._zoom;
    if (zoom >= 15) {
      const bounds = map.getBounds();

      const overpassQuery = `[out:json][timeout:25];
      (
        way["highway"~"motorway|trunk|primary|motorway_link|trunk_link|primary_link|unclassified|tertiary|secondary|track|residential|secondary_link|tertiary_link"](${bounds._southWest.lat},${bounds._southWest.lng},${bounds._northEast.lat},${bounds._northEast.lng}); 
      );
      out body;
      >;
      out skel qt;`;

      var q = "data=" + encodeURIComponent(overpassQuery);

      var uri = "https://overpass-api.de/api/interpreter";

      fetch(uri, { method: "POST", body: q }).then(async (res) => {
        let geoJSONData = osmtogeojson(await res.json());

        let _roads = [];
        geoJSONData.features.map((road) => {
          let pointsArr = road.geometry.coordinates;
          let pointList = [];
          let direction =
            pointsArr[0][0] < pointsArr[pointsArr.length - 1][0]
              ? "forward"
              : "backward";
          // console.log(road.properties);
          pointsArr.map((p) => {
            if (p) pointList.push([p[1], p[0]]);
          });

          L.polyline(pointList, {
            color: road.properties.oneway != "yes" ? "black" : "lime",
          }).addTo(map);

          // _roads.push(
          //   <Polyline
          //     key={road.properties.id}
          //     pathOptions={{
          //       color: road.properties.oneway != "yes" ? "black" : "lime",
          //     }}
          //     positions={pointList}
          //   >
          //     <Popup>{`Direction: ${direction}\nOneway: ${road.properties.oneway}\nId: ${road.properties.id}`}</Popup>
          //   </Polyline>
          // );
        });
        setRoads([..._roads]);
      });
    }
  };

  useMapEvents({
    zoomend(e) {
      updateRoads(e);
    },
    dragend(e) {
      updateRoads(e);
    },
  });
  return null;
}

export default function Map() {
  const [roads, setRoads] = useState([]);

  return (
    <div className="w-full h-full">
      <MapContainer
        center={[45.7494, 21.2272]}
        zoom={15}
        maxZoom={20}
        scrollWheelZoom={true}
        className="w-full h-full z-0"
        id="map"
        doubleClickZoom={false}
      >
        <MapEvents roads={roads} setRoads={setRoads} />
        {roads}
        <TileLayer
          attribution="Google Maps"
          url="http://{s}.google.com/vt?lyrs=s&x={x}&y={y}&z={z}"
          subdomains={["mt0", "mt1", "mt2", "mt3"]}
          maxZoom={20}
        />
      </MapContainer>
    </div>
  );
}
