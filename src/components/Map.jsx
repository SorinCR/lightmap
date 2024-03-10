import "../App.css";
import "leaflet/dist/leaflet.css";
import "react-leaflet-markercluster/dist/styles.min.css";
import "leaflet-polylinedecorator";
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
    if (zoom >= 17) {
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
          if (
            road.properties.id == "way/496423878" ||
            road.properties.id == "way/496423875"
          )
            console.log(road);
          pointsArr.map((p) => {
            if (p) pointList.push([p[1], p[0]]);
          });

          var line = L.polyline(pointList, {
            color:
              road.properties.oneway != "yes"
                ? "black"
                : direction == "forward"
                ? "lime"
                : "red",
          }).addTo(map);

          // if (road.properties.lanes > 1) {
          //   const geometryOffset = 0.000025;
          //   for (var i = 0; i < pointList.length - 1; i++) {
          //       pointList[i][0] += geometryOffset;
          //       pointList[i][1] += geometryOffset;
          //   }

          //   L.polyline(pointList, {
          //     color:
          //       road.properties.oneway != "yes"
          //         ? "black"
          //         : direction == "forward"
          //         ? "lime"
          //         : "red",
          //   }).addTo(map);
          // }

          var popup = L.popup().setContent(`ID: ${road.properties.id}`);

          line.bindPopup(popup);

          if (road.properties.oneway == "yes")
            L.polylineDecorator(line, {
              patterns: [
                {
                  offset: 25,
                  repeat: "50px",
                  symbol: L.Symbol.arrowHead({
                    pixelSize: 15,
                    pathOptions: { color: "blue", fillOpacity: 1, weight: 0 },
                  }),
                },
              ],
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

export default function Map({ tile }) {
  const [roads, setRoads] = useState([]);
  const tiles = {
    traffic:
      "https://{s}.google.com/vt?lyrs=h@159000000,traffic|seconds_into_week:-1&style=3&x={x}&y={y}&z={z}",
    terrain: "http://{s}.google.com/vt/lyrs=p&x={x}&y={y}&z={z}",
    streets: "http://{s}.google.com/vt/lyrs=m&x={x}&y={y}&z={z}",
    sattelite: "http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}",
  };
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
          url={tiles[tile]}
          subdomains={["mt0", "mt1", "mt2", "mt3"]}
          maxZoom={20}
        />
      </MapContainer>
    </div>
  );
}
