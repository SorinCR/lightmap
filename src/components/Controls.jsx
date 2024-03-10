import ReactDOM from "react-dom";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

import {
  faLayerGroup,
  faCropSimple,
  faTrafficLight,
  faXmark,
  faWeightScale,
  faCamera,
} from "@fortawesome/free-solid-svg-icons";
import { Layer } from "leaflet";

import { useMap } from "react-leaflet";

function LayerChanger({ tile, setTile }) {
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
          {tile == "sattelite" ? (
            <>
              <FontAwesomeIcon icon={faLayerGroup} color="white" />
              <p className="text-sm text-white">Layers</p>
            </>
          ) : (
            <>
              <FontAwesomeIcon icon={faLayerGroup} color="#3d3d3d" />
              <p className="text-sm text-[#3d3d3d]">Layers</p>
            </>
          )}
        </div>
      </div>
    </div>
  );
}

function ActionMenu({ tile, toogleSemaphore }) {
  return (
    <div className="flex flex-col gap-5">
      <div
        onClick={() => toogleSemaphore(true)}
        className={
          tile == "sattelite"
            ? "bg-[rgba(255,255,255,0.4)] hover:bg-[rgba(255,255,255,0.5)] text-[#030303] cursor-pointer w-10 h-10 flex justify-center items-center rounded-full ml-5"
            : "bg-[rgba(0,0,0,0.4)] hover:bg-[rgba(0,0,0,0.5)] text-white cursor-pointer w-10 h-10 flex justify-center items-center rounded-full ml-5"
        }
      >
        <FontAwesomeIcon icon={faTrafficLight} />
      </div>
      <div
        className={
          tile == "sattelite"
            ? "bg-[rgba(255,255,255,0.4)] hover:bg-[rgba(255,255,255,0.5)] text-[#030303] cursor-pointer w-10 h-10 flex justify-center items-center rounded-full ml-5"
            : "bg-[rgba(0,0,0,0.4)] hover:bg-[rgba(0,0,0,0.5)] text-white cursor-pointer w-10 h-10 flex justify-center items-center rounded-full ml-5"
        }
      >
        <FontAwesomeIcon icon={faWeightScale} />
      </div>
      <div
        className={
          tile == "sattelite"
            ? "bg-[rgba(255,255,255,0.4)] hover:bg-[rgba(255,255,255,0.5)] text-[#030303] cursor-pointer w-10 h-10 flex justify-center items-center rounded-full ml-5"
            : "bg-[rgba(0,0,0,0.4)] hover:bg-[rgba(0,0,0,0.5)] text-white cursor-pointer w-10 h-10 flex justify-center items-center rounded-full ml-5"
        }
      >
        <FontAwesomeIcon icon={faCamera} />
      </div>
    </div>
  );
}

function CropButton({
  selectAreaMode,
  setSelectAreaMode,
  tile,
  editing,
  setEditing,
  clearRoutes,
}) {
  return (
    <div
      onClick={() => {
        if (!editing) {
          setSelectAreaMode(true);
          setEditing(true);
        } else {
          setEditing(false);
          setSelectAreaMode(false);
          window.location.reload();
        }
      }}
      className={
        tile == "sattelite"
          ? "bg-[rgba(255,255,255,0.4)] hover:bg-[rgba(255,255,255,0.5)] text-[#030303] cursor-pointer w-10 h-10 flex justify-center items-center rounded-full ml-5"
          : "bg-[rgba(0,0,0,0.4)] hover:bg-[rgba(0,0,0,0.5)] text-white cursor-pointer w-10 h-10 flex justify-center items-center rounded-full ml-5"
      }
    >
      <FontAwesomeIcon icon={!editing ? faCropSimple : faXmark} />
    </div>
  );
}

export default function Controls({
  tile,
  setTile,
  selectAreaMode,
  setSelectAreaMode,
  editing,
  setEditing,
  clearRoutes,
  toogleSemaphore,
}) {
  return (
    <div className="absolute h-full left-0 w-20 flex items-start justify-start gap-5 pt-10 flex-col">
      <LayerChanger tile={tile} setTile={setTile} />
      <CropButton
        setSelectAreaMode={setSelectAreaMode}
        selectAreaMode={selectAreaMode}
        tile={tile}
        editing={editing}
        setEditing={setEditing}
        clearRoutes={clearRoutes}
      />
      {editing ? (
        <ActionMenu tile={tile} toogleSemaphore={toogleSemaphore} />
      ) : (
        <></>
      )}
    </div>
  );
}
