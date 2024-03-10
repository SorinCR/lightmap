import "../App.css";

export default function Home() {
  return (
    <div className="videobg w-full h-full flex relative">
      <iframe
        src="https://www.youtube.com/embed/lg58A48cyr8?autoplay=1&mute=1&controls=0&showinfo=0&rel=0&loop=1&playlist=lg58A48cyr8&modestbranding=1&end==38"
        frameborder="0"
        allow="autoplay; encrypted-media"
        allowfullscreen
      ></iframe>
      <div className="w-full h-full absolute flex justify-center items-center">
        <div className="w-full h-full gradientbg flex">
          <div className="w-full"></div>
          <div className="w-full h-full p-24 flex flex-col gap-10 justify-center items-start text-white">
            <div className="w-full">
              <h1 className="text-5xl mb-2">LightMap</h1>
              <p>It's possible because of you!</p>
            </div>
            <div className="flex w-full flex justify-start items-center gap-10">
              <a
                href="/enlighten"
                className="px-4 py-2 border-white border-2 rounded-sm hover:bg-[rgba(255,255,255,0.1)]"
              >
                Create your project
              </a>
              <a
                href="#"
                className="px-4 py-2 border-white border-2 rounded-sm hover:bg-[rgba(255,255,255,0.1)]"
              >
                Continue your work - Coming Soon!
              </a>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
