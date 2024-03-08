import "./App.css";
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

import Map from "./components/Map";

function App() {
  useEffect(() => {}, []);

  return (
    <div className="w-full h-full flex items-center justify-center flex-col">
      <Map />
    </div>
  );
}

export default App;
