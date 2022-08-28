# Ray (shadow)Casting

Interactive 2D-ray-caster visualizer. Utilizes the GJK collision procedure to march each ray. Interacts correctly with any complex convex polygon in 2D space. Implemented using SFML. Future versions will have options to use different collision procedures (SDF, etc.).

![Alt Text](https://github.com/brax-p/2D-Raytracer/blob/main/src/gifs/2D-RayTracing_Demonstration.gif "Demo")


### How To Run:
On Ubuntu-based linux, install SFML.

```bash
sudo apt-get install libsfml-dev
```
Clone the repo. \
Then in the root directory of the repo, run:

```base
make && ./run/app
```

Move the mouse to control the Star (the light source).

Enjoy :D.


## Credits/Resources

GJK implementation takes inspiration from [here](https://github.com/kroitor/gjk.c). <br /> SFML can be found [here](https://www.sfml-dev.org/).
