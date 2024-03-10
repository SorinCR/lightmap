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
          <div className="w-full h-full p-24 flex flex-col justify-center items-start text-white">
            <h1 className="text-5xl">LightMap</h1>
            <p>It's possible because of you!</p>
            <div className="flex w-full flex justify-start items-center gap-10">
              <a href="/enlighten">Create your project</a>
              <a href="#">Continue your work(not possible)</a>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
