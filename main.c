#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "metal.h"
#include "dielectric.h"
#include "lambertian.h"

Vector color(const Ray r, Hitable_List *world_list, int depth)
{
    Hit_Record rec;

    if (world_list->hitable.hit((const void *) world_list, r, 0.001, FLT_MAX, &rec)) {
        Ray scattered;
        Vector attenuation;

        if (depth < 50 && rec.mat_ptr.scatter((const void *) rec.mat_ptr.inst, r, rec, &attenuation, &scattered)) {
            return attenuation.mul(attenuation, color(scattered, world_list, depth + 1));
        } else {
            return new_vector(new_point(0, 0, 0));
        }
    } else {
        Vector unit_direction = unit_vector(r.direction(r));
        float t = 0.5 * (unit_direction.point.y + 1.0);

        Vector v1 = new_vector(new_point(1.0, 1.0, 1.0));
        Vector v2 = new_vector(new_point(0.5, 0.7, 1.0));

        return v1.add(v1.mul_scalar(v1, 1.0 - t), v1.mul_scalar(v2, t));
    }
}

Hitable_List random_scene()
{
    int n = 500;
    Hitable **list = (Hitable **) malloc((n + 1) * sizeof(Hitable *));

    Sphere *sphere;
    Lambertian *lam;
    Metal *metal;
    Dielectric *diel;

    sphere = (Sphere *) malloc(sizeof(Sphere));
    lam = (Lambertian *) malloc(sizeof(Lambertian));
    *lam = new_lambertian(new_vector(new_point(0.5, 0.5, 0.5)));
    *sphere = new_sphere(
                 new_vector(new_point(0, -1000, 0)), 
                 1000, 
                 (void *) lam,
                 lam->mat.scatter
              );
    list[0] = &sphere->hitable;
    list[0]->inst = (void *) sphere;

    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            Vector center = new_vector(new_point(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48()));
            sphere = (Sphere *) malloc(sizeof(Sphere));

            if (center.length(center.sub(center, new_vector(new_point(4, 0.2, 0)))) > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    lam = (Lambertian *) malloc(sizeof(Lambertian));
                    *lam = new_lambertian(new_vector(new_point(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
                    *sphere = new_sphere(
                                   center,
                                   0.2,
                                   (void *) lam,
                                   lam->mat.scatter
                              );
                } else if (choose_mat < 0.95) {
                    // metal
                    metal = (Metal *) malloc(sizeof(Metal));
                    *metal = new_metal(new_vector(new_point(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48()))), 0.5 * drand48());
                    *sphere = new_sphere(
                                    center,
                                    0.2,
                                    (void *) metal,
                                    metal->mat.scatter
                              );
                } else {
                    // glass
                    diel = (Dielectric *) malloc(sizeof(Dielectric));
                    *diel = new_dielectric(1.5);
                    *sphere = new_sphere(center, 0.2, (void *) diel, diel->mat.scatter);
                }

                list[i] = &sphere->hitable;
                list[i++]->inst = (void *) sphere;
            }
        }
    }

    sphere  = (Sphere *) malloc(sizeof(Sphere));
    diel = (Dielectric *) malloc(sizeof(Dielectric));
    *diel = new_dielectric(1.5);
    *sphere = new_sphere(new_vector(new_point(0, 1, 0)), 1.0, (void *) diel, diel->mat.scatter);
    list[i] = &sphere->hitable;
    list[i++]->inst = (void *) sphere;

    sphere  = (Sphere *) malloc(sizeof(Sphere));
    lam = (Lambertian *) malloc(sizeof(Lambertian));
    *lam = new_lambertian(new_vector(new_point(0.4, 0.2, 0.1))); 
    *sphere = new_sphere(new_vector(new_point(-4, 1, 0)), 1.0, (void *) lam, lam->mat.scatter);
    list[i] = &sphere->hitable;
    list[i++]->inst = (void *) sphere;

    sphere  = (Sphere *) malloc(sizeof(Sphere));
    metal = (Metal *) malloc(sizeof(Metal));
    *metal = new_metal(new_vector(new_point(0.7, 0.6, 0.5)), 0.0);
    *sphere = new_sphere(new_vector(new_point(4, 1, 0)), 1.0, (void *) metal, metal->mat.scatter);
    list[i] = &sphere->hitable;
    list[i++]->inst = (void *) sphere;

    return new_hitable_list(list, i);
}

int main(void) {
    clock_t start_time = clock();

    int nx = 1200; // length
    int ny = 800;  // width
    int ns = 10;   // sampling times
    float R = cos(M_PI / 4);

    FILE *outfile = fopen("out.ppm", "wb");

    fprintf(outfile, "P3\n%d %d \n255\n", nx, ny);

    Hitable *list[5];

    Sphere *sphere;
    Lambertian *lam;
    Dielectric *diel;
    Metal *metal;

    sphere  = (Sphere *) malloc(sizeof(Sphere));
    lam = (Lambertian *) malloc(sizeof(Lambertian));
    *lam = new_lambertian(new_vector(new_point(0.1, 0.2, 0.5))); 
    *sphere = new_sphere(new_vector(new_point(0, 0, -1)), 0.5, (void *) lam, lam->mat.scatter);
    list[0] = &sphere->hitable;
    list[0]->inst = (void *) sphere;

    sphere  = (Sphere *) malloc(sizeof(Sphere));
    lam = (Lambertian *) malloc(sizeof(Lambertian));
    *lam = new_lambertian(new_vector(new_point(0.8, 0.8, 0.0)));
    *sphere = new_sphere(new_vector(new_point(0, -100.5, -1)), 100, (void *) lam, lam->mat.scatter);
    list[1] = &sphere->hitable;
    list[1]->inst = (void *) sphere;

    sphere  = (Sphere *) malloc(sizeof(Sphere));
    metal = (Metal *) malloc(sizeof(Metal));
    *metal = new_metal(new_vector(new_point(0.8, 0.6, 0.2)), 0.0); 
    *sphere = new_sphere(new_vector(new_point(1, 0, -1)), 0.5, (void *) metal, metal->mat.scatter);
    list[2] = &sphere->hitable;
    list[2]->inst = (void *) sphere;

    sphere  = (Sphere *) malloc(sizeof(Sphere));
    diel = (Dielectric *) malloc(sizeof(Dielectric));
    *diel = new_dielectric(1.5);
    *sphere = new_sphere(new_vector(new_point(-1, 0, -1)), 0.5, (void *) diel, diel->mat.scatter);
    list[3] = &sphere->hitable;
    list[3]->inst = (void *) sphere;

    sphere  = (Sphere *) malloc(sizeof(Sphere));
    diel = (Dielectric *) malloc(sizeof(Dielectric));
    *diel = new_dielectric(1.5);
    *sphere = new_sphere(new_vector(new_point(-1, 0, -1)), -0.45, (void *) diel, diel->mat.scatter);
    list[4] = &sphere->hitable;
    list[4]->inst = (void *) sphere;

    Hitable_List world = new_hitable_list(list, 5);
    world = random_scene();

    Vector lookfrom = new_vector(new_point(13, 2, 3));
    Vector lookat = new_vector(new_point(0, 0, 0));
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    Camera cam = new_camera(lookfrom, lookat, new_vector(new_point(0, 1, 0)), 20, (float) nx / (float) ny, aperture, dist_to_focus);

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            Vector col = new_vector(new_point(0, 0, 0));

            /* Sampling ns times per pixel area */
            for (int s = 0; s < ns; s++) {
                /* Centered at the center point of the pixel, the pixel outward distance is (0.0, 1.0] */
                float u = (float) (i + drand48()) / (float) nx;
                float v = (float) (j + drand48()) / (float) ny;

                /* Obtain the color value of the random sampling point in this pixel area */
                Ray r = cam.get_ray(cam, u, v);
                Vector p = r.point_at_parameter(r, 2.0);

                /* Accumulate the color values of all ns random sample points of this point area */
                col = col.add(col, color(r, &world, 0));
            }

            /* Divide the color accumulated value of all ns random sampling points of this pixel area by ns to obtain the average value */
            col = col.div_scalar(col, (float) ns);
            col = new_vector(new_point(sqrt(col.point.x), sqrt(col.point.y), sqrt(col.point.z)));
            int ir = (int) (255.99 * col.point.x);
            int ig = (int) (255.99 * col.point.y);
            int ib = (int) (255.99 * col.point.z);

            fprintf(outfile, "%d %d %d\n", ir, ig, ib);
        }
    }

    for (int i = 0; i < 5; i ++) {
        free(list[i]->inst);
    }
    for (int i = 0; i < world.list_size; i++) {
        free(world.list[i]->inst);
    }

    clock_t end_time = clock();
    printf("Elapsed: %f seconds\n", (double) (end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}
