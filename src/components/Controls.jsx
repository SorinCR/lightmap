import ReactDOM from "react-dom";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

import { faLayerGroup } from "@fortawesome/free-solid-svg-icons";

export default function Controls({ tile, setTile }) {
  return (
    <div
      onClick={() => {
        switch (tile) {
          case "sattelite":
            setTile("terrain");
            break;
          case "terrain":
            setTile("traffic");
            break;
          case "traffic":
            setTile("sattelite");
            break;
        }
      }}
      className="absolute bottom-0 left-0 p-6 flex justify-center items-center cursor-pointer"
    >
      <div className="border-radius w-24 h-24 relative shadow-xl outline-white outline outline-2 rounded-2xl hover:outline-[3px]">
        <img
          src={`/assets/${tile}.png`}
          className="w-full h-full object-cover rounded-2xl shadow-xl"
        />
        <div className="flex w-full p-2 absolute bottom-0 flex justify-center items-center gap-2">
          <FontAwesomeIcon icon={faLayerGroup} color="white" />
          <p className="text-sm text-white">Layers</p>
        </div>
      </div>
    </div>
  );
}
