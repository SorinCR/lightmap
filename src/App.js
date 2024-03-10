import "./App.css";
import "leaflet/dist/leaflet.css";
import "react-leaflet-markercluster/dist/styles.min.css";
import { useEffect, useState } from "react";

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
import Controls from "./components/Controls";

function App() {
  useEffect(() => {}, []);

  const [tile, setTile] = useState("sattelite");

  return (
    <div className="w-full h-full flex items-center justify-center flex-col relative">
      <Map tile={tile} />
      <Controls tile={tile} setTile={setTile} />
    </div>
  );
}

export default App;
