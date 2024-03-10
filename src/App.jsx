import "./App.css";
import { BrowserRouter, Route, Routes } from "react-router-dom";

import Home from "./pages/Home";
import Enlighten from "./pages/Enlighten";

function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/enlighten" element={<Enlighten />} />
      </Routes>
    </BrowserRouter>
  );
}

export default App;
