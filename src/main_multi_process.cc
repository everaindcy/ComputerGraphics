#include "util/utils.h"
#include "object/objects.h"
#include "texture/textures.h"
#include "material/materials.h"
#include "unit/units.h"

#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <mutex>
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
    std::cerr << "Begin scene parse...\n" << std::flush;
    scene sce;
    scene_gen(sce, 20);

    std::cerr << "generate processes...\n" << std::flush;

    const int num_process = 30;
    const int task_per_process = (sce.image_height+num_process-1)/num_process;
    int taskbegin = 0;
    int taskend = sce.image_height;

    while(taskend-taskbegin > task_per_process) {
        pid_t pid = fork();
        if (pid == -1) {
            std::cerr << "Error: failed to create process\n" << std::flush;
            return -1;
        } else if (pid > 0) {
            // std::cerr << "printed from parent process "+std::to_string(getpid())+"\n" << std::flush;
            taskbegin = taskend - task_per_process;
        } else {
            // std::cerr << "printed from child process "+std::to_string(getpid())+"\n" << std::flush;
            taskend -= task_per_process;
        }
    }
    // std::cerr << std::to_string(getpid())+" task = ["+std::to_string(taskbegin)+", "+std::to_string(taskend)+")\n" << std::flush; //]
    image img(sce.image_width, taskend-taskbegin);

if(taskend==sce.image_height-num_process/2*task_per_process) std::cerr << "Begin Path tracing...\n" << std::flush;
auto start = std::chrono::system_clock::now();
int finished = 0;
if(taskend==sce.image_height-num_process/2*task_per_process) std::cerr << "\rPixels finished: "+std::to_string(finished)+"/"+std::to_string(sce.image_height*sce.image_width)+" | "+std::to_string(100.0*finished/sce.image_height/sce.image_width)+"%"  << std::flush;
    for (int n = taskbegin; n < taskend; n++) {
        int j = sce.image_height - 1 - n;
        for (int i = 0; i < sce.image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < sce.samples_per_pixel; ++s) {
                auto u = (i + random_double()) / sce.image_width;
                auto v = (j + random_double()) / sce.image_height;
                ray r = sce.cam->get_ray(u, v);
                pixel_color += ray_color(r, sce.background, sce.objs, sce.max_depth);
            }
            write_color(img, pixel_color, sce.samples_per_pixel, n-taskbegin, i);
finished += num_process;
finished = finished > sce.image_height*sce.image_width ? sce.image_height*sce.image_width : finished;
if(taskend==sce.image_height-num_process/2*task_per_process) std::cerr << "\rPixels finished: "+std::to_string(finished)+"/"+std::to_string(sce.image_height*sce.image_width)+" | "+std::to_string(100.0*finished/sce.image_height/sce.image_width)+"%"  << std::flush;
        }
    }

    if (taskbegin == 0) {
        std::cout << "P3\n" << sce.image_width << " " << sce.image_height << "\n255\n";
    } else {
        wait(nullptr);
    }

    img.print();

auto end = std::chrono::system_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
if(taskend==sce.image_height) {
std::cerr << "\nDown. Time = "+std::to_string(double(duration.count())*std::chrono::milliseconds::period::num/std::chrono::milliseconds::period::den)+"s\n" << std::flush;
}
}