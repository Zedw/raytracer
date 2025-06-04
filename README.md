# raytracer

This project is a very small example ray tracer implemented in C++. It now opens an SDL2 window showing a reflective sphere hovering above a floor. A directional "sun" lights the scene and objects cast smooth shadows.

## Dependencies

The project depends on SDL2. On Debian based distributions you can install the
development libraries together with the MinGW toolchain for cross compilation:

```bash
sudo apt-get update
sudo apt-get install -y libsdl2-dev mingw-w64
```

## Building and running

The provided `Makefile` supports building both natively and with MinGW. To
produce a Windows executable on Linux run:

```bash
make CXX=x86_64-w64-mingw32-g++ SDL_CONFIG=x86_64-w64-mingw32-sdl2-config
```

The command compiles `raytracer.exe` and copies it to the `releases` folder.
You can run the program on Windows or with Wine:

```bash
wine ./raytracer.exe
```

Close the window to exit the program.
Use `W`, `A`, `S`, `D` to move forward, left, back and right relative to the
current view. `Q` moves up, `E` moves down. Hold multiple keys at once to move
diagonally. Look around with the arrow keys (`←`/`→` yaw, `↑`/`↓` pitch).
An ImGui panel lets you tweak the light color and direction while the window is
running. The window can be resized freely and the sky now shows moving clouds.


## GitHub Releases

Push a tag starting with `v` (for example `v1.0.0`) to trigger the GitHub Actions workflow. The workflow compiles the executable and attaches `raytracer.exe` to the release.
