# gi – global illumination
gi is a custom dependency-free path tracer inspired by and based on the work found in pbrt v3 and raytracing series by Shirley et al.

## Compile / Install

```sh
git clone https://github.com/marvrez/gi.git
cd gi/
make -j
```

### Example
```sh
./gi
```

### Accelerating ray-triangle intersections

Install [Embree](https://www.embree.org/) to make the ray-triangle intersection tests A LOT quicker. Once that's done, you need to set `EMBREE` to `1` in the `Makefile`, then rebuild the program.
