import "../App.css";
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

import Map from "../components/Map";
import Controls from "../components/Controls";

function Enlighten() {
  const [tile, setTile] = useState("sattelite");
  const [selectAreaMode, setSelectAreaMode] = useState(false);
  const [editing, setEditing] = useState(false);
  const [roads, setRoads] = useState([]);
  const [clearRoutes, setClearRoutes] = useState(() => {});
  const [semaphore, toogleSemaphore] = useState(false);
  const [weight, toogleWeight] = useState(false);
  const [camera, toogleCamera] = useState(false);

  return (
    <div className="w-full h-full flex items-center justify-center flex-col relative">
      <Map
        tile={tile}
        selectAreaMode={selectAreaMode}
        setSelectAreaMode={setSelectAreaMode}
        roads={roads}
        setRoads={setRoads}
        setClearRoutes={setClearRoutes}
        semaphore={semaphore}
      />
      <Controls
        tile={tile}
        setTile={setTile}
        selectAreaMode={selectAreaMode}
        setSelectAreaMode={setSelectAreaMode}
        editing={editing}
        setEditing={setEditing}
        clearRoutes={clearRoutes}
        toogleSemaphore={toogleSemaphore}
      />
    </div>
  );
}

export default Enlighten;
