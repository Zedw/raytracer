# raytracer

This project is a very small example ray tracer implemented in C++. It now opens an SDL2 window showing a reflective sphere hovering above a floor.

## Building and running

This project uses a small `Makefile`. Ensure SDL2 is installed and simply run:

```bash
make
```

The command compiles `raytracer.exe`, launches it, and copies the executable to
the `releases` folder. Afterwards you can run the program again with:

```bash
./raytracer.exe
```

Close the window to exit the program.
