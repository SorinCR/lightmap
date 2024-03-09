import "../App.css";
import "leaflet/dist/leaflet.css";
import "react-leaflet-markercluster/dist/styles.min.css";
import { useEffect } from "react";

import {
  MapContainer,
  TileLayer,
  useMap,
  Marker,
  Popup,
  marker,
  useMapEvents,
} from "react-leaflet";

import L from "leaflet";

function MapEvents() {
  const map = useMap();

  function processIntersectionData(data) {
    const intersections = data.elements;

    for (let intersection of intersections) {
      if (intersection.type === "node") {
        L.marker([intersection.lat, intersection.lon])
          .addTo(map) // Replace 'yourMap' with your Leaflet map object
          .bindPopup("Intersection");
      }
    }
  }

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
      }
    },
    // click(e) {
    //   console.log(e);
    // },
  });
  return null;
}

export default function Map() {
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
        <MapEvents />
        <TileLayer
          attribution="Google Maps"
          url="http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}"
          subdomains={["mt0", "mt1", "mt2", "mt3"]}
        />
      </MapContainer>
    </div>
  );
}
