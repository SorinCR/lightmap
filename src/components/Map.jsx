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

function MapEvents({ roads, setRoads, selectAreaMode, setSelectAreaMode }) {
  const map = useMap();

  const colors = ["lime", "white", "blue", "red", "green", "pink"];

  if (selectAreaMode) {
    map.dragging.disable();
  } else map.dragging.enable();

  const updateRoads = (e) => {
    var zoom = e.target._zoom;
    if (zoom >= 17) {
      const bounds = map.getBounds();
      console.log(bounds);

      const overpassQuery = `[out:json][timeout:25];
      (
        way["highway"~"motorway|trunk|primary|motorway_link|trunk_link|primary_link|unclassified|tertiary|secondary|track|residential|secondary_link|tertiary_link"](${bounds._southWest.lat},${bounds._southWest.lng},${bounds._northEast.lat},${bounds._northEast.lng}); 
      );
      out body;
      >;
      out skel qt;`;

      console.log(overpassQuery);

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
        });
        setRoads([..._roads]);
      });
    }
  };

  const updateRoadsByPoints = (selected) => {
    let northEast = { lat: -90, lng: -180 };
    let southWest = { lat: 90, lng: 180 };

    selected.sort((a, b) => b.lat - a.lat);

    for (const point of selected) {
      // Update northEast
      northEast.lat = Math.max(northEast.lat, point.lat);
      northEast.lng = Math.max(northEast.lng, point.lng);

      // Update southWest
      southWest.lat = Math.min(southWest.lat, point.lat);
      southWest.lng = Math.min(southWest.lng, point.lng);
    }

    const overpassQuery = `[out:json][timeout:25];
    (
      way["highway"~"motorway|trunk|primary|motorway_link|trunk_link|primary_link|unclassified|tertiary|secondary|track|residential|secondary_link|tertiary_link"](${southWest.lat},${southWest.lng},${northEast.lat},${northEast.lng}); 
      );
      out body;
      >;
      out skel qt;`;

    // if(selected)
    console.log(overpassQuery);

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
      });
      setRoads([..._roads]);
    });
  };

  let rectangleDiv = null;
  let startX, startY;

  let selectedLatLng = [null, null];

  const mapEl = document.querySelector("#map");

  let mouseDownListener = (e) => {
    const width = Math.abs(e.clientX - startX);
    const height = Math.abs(e.clientY - startY);
    const left = Math.min(e.clientX, startX);
    const top = Math.min(e.clientY, startY);

    rectangleDiv.style.left = left + "px";
    rectangleDiv.style.top = top + "px";
    rectangleDiv.style.width = width + "px";
    rectangleDiv.style.height = height + "px";
  };

  useMapEvents({
    zoomend(e) {
      // updateRoads(e);
    },
    dragend(e) {
      // updateRoads(e);
    },
    mousedown(e) {
      if (selectAreaMode) {
        // console.log(e.latlng);
        selectedLatLng[0] = e.latlng;
        rectangleDiv = document.createElement("div");
        rectangleDiv.id = "rectangle";
        mapEl.appendChild(rectangleDiv);

        // console.log(e);
        startX = e.containerPoint.x;
        startY = e.containerPoint.y;
        mapEl.addEventListener("mousemove", mouseDownListener);
      }
    },
    mouseup(e) {
      if (selectAreaMode) {
        mapEl.removeEventListener("mousemove", mouseDownListener);
        document.querySelector("#rectangle").remove();
        selectedLatLng[1] = e.latlng;
        setSelectAreaMode(false);
        updateRoadsByPoints(selectedLatLng);
        console.log(selectedLatLng);
      }
    },
    // click(e) {
    //   console.log(e);
    // },
  });
  return null;
}

export default function Map({ tile, selectAreaMode, setSelectAreaMode }) {
  const [roads, setRoads] = useState([]);

  const tiles = {
    traffic:
      "https://{s}.google.com/vt?lyrs=h@159000000,traffic|seconds_into_week:-1&style=3&x={x}&y={y}&z={z}",
    terrain: "http://{s}.google.com/vt/lyrs=p&x={x}&y={y}&z={z}",
    streets: "http://{s}.google.com/vt/lyrs=m&x={x}&y={y}&z={z}",
    sattelite: "http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}",
  };
  return (
    <div className="w-full h-full relative">
      <MapContainer
        center={[45.7494, 21.2272]}
        zoom={15}
        maxZoom={20}
        scrollWheelZoom={true}
        className="w-full h-full z-0"
        id="map"
        doubleClickZoom={false}
      >
        <MapEvents
          roads={roads}
          setRoads={setRoads}
          selectAreaMode={selectAreaMode}
          setSelectAreaMode={setSelectAreaMode}
        />
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
