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

import L from "leaflet";

function MapEvents({ setRoads }) {
  const map = useMap();

  const colors = ["red", "green", "lime", "purple", "pink", "black", "white"];

  useMapEvents({
    zoomend(e) {
      var zoom = e.target._zoom;
      if (zoom >= 18) {
        console.log(e);
        const bounds = map.getBounds();
        console.log();

        const overpassQuery = `[out:json][timeout:25];
        (
            way["highway"](${bounds._southWest.lat},${bounds._southWest.lng},${bounds._northEast.lat},${bounds._northEast.lng}); 
        );
        out body;
        >;
        out skel qt;`;

        console.log(overpassQuery);

        var q = "data=" + encodeURIComponent(overpassQuery);

        var uri = "https://overpass-api.de/api/interpreter";

        fetch(uri, { method: "POST", body: q }).then(async (res) => {
          let geoJSONData = osmtogeojson(await res.json());

          console.log(geoJSONData.features[0].geometry.coordinates[0]);

          geoJSONData.features.map((road) => {
            let pointsArr = road.geometry.coordinates;
            let pointList = [];

            pointsArr.map((p) => {
              if (p) pointList.push([p[1], p[0]]);
            });
            console.log(pointList);

            // console.log(pointList);
            // setRoads([
            //   ...(
            //     <Polyline
            //       pathOptions={{
            //         color:
            //           colors[Math.floor(Math.random() * colors.length - 1)],
            //       }}
            //       positions={pointList}
            //     />
            //   ),
            // ]);
          });
        });
      }
    },
    // click(e) {
    //   console.log(e);
    // },
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
        minZoom={13}
        scrollWheelZoom={true}
        className="w-full h-full z-0"
        id="map"
        maxBounds={[
          [45.683825, 21.111682],
          [45.834174, 21.327244],
        ]}
        doubleClickZoom={false}
      >
        <MapEvents setRoads={setRoads} />
        {roads}
        <TileLayer
          attribution="Google Maps"
          url="http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}"
          subdomains={["mt0", "mt1", "mt2", "mt3"]}
        />
      </MapContainer>
    </div>
  );
}
