#include "util/utils.h"
#include "object/objects.h"
#include "texture/textures.h"
#include "material/materials.h"
#include "unit/units.h"

#include <iostream>
#include <string>
#include <mutex>
#include <time.h>
#include <omp.h>

color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

int main() {
    scene sce;
    scene_gen(sce, 6);

    image img(sce.image_width, sce.image_height);

std::mutex mux;
int finished = 0;
clock_t start, end;

start = clock();
    std::cerr << "row finished: "+std::to_string(finished)+"/"+std::to_string(sce.image_height)+" | "+std::to_string(((double)finished)/sce.image_height)+"%"  << std::flush;
// omp_set_num_threads(30);
#pragma omp parallel for schedule(dynamic, 10)
    for (int n = 0; n < sce.image_height; n++) {
        int j = sce.image_height - 1 - n;
        for (int i = 0; i < sce.image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < sce.samples_per_pixel; ++s) {
                auto u = (i + random_double()) / sce.image_width;
                auto v = (j + random_double()) / sce.image_height;
                ray r = sce.cam->get_ray(u, v);
                pixel_color += ray_color(r, sce.background, sce.objs, sce.max_depth);
            }
            write_color(img, pixel_color, sce.samples_per_pixel, n, i);
        }

        mux.lock(); finished++; mux.unlock();
        std::cerr << "\rRow finished: "+std::to_string(finished)+"/"+std::to_string(sce.image_height)+" | "+std::to_string(100.0*finished/sce.image_height)+"%"  << std::flush;
    }
end = clock();

    std::cerr << "\nDown. Time = "+std::to_string(double(end-start)/CLOCKS_PER_SEC)+"s\n" << std::flush;

    img.print_ppm();
}