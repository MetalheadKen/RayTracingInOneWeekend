#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <string.h>
#include <omp.h>
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

struct timespec diff_time(struct timespec start, struct timespec end)
{
    struct timespec elapsed;

    if ((end.tv_nsec - start.tv_nsec) < 0) {
        elapsed.tv_sec = end.tv_sec - start.tv_sec - 1;
        elapsed.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        elapsed.tv_sec = end.tv_sec - start.tv_sec;
        elapsed.tv_nsec = end.tv_nsec - start.tv_nsec;
    }

    return elapsed;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "%d\n", argc);
        fprintf(stderr, "%s [input file] [output file]\n", argv[0]);
        exit(-1);
    }

    const char *infile = argv[1];
    const char *outfile = argv[2];

    FILE *fin = fopen(infile, "r");
    FILE *fout = fopen(outfile, "wb");

    int nx;      // length
    int ny;      // width
    int ns = 10; // sampling times
    int sphere_count;
    // float R = cos(M_PI / 4);

    fscanf(fin, "%d %d", &nx, &ny);
    fscanf(fin, "%d", &sphere_count);

    fprintf(fout, "P3\n%d %d \n255\n", nx, ny);

    Hitable *list[sphere_count];

    Sphere *sphere;
    Lambertian *lam;
    Dielectric *diel;
    Metal *metal;

    for (int i = 0; i < sphere_count; i++) {
        float radius;
        char material_str[11];
        Vector sphere_v = new_vector(new_point(0, 0, 0));
        Vector material_v = new_vector(new_point(0, 0, 0));

        fscanf(fin, "%f %f %f", &sphere_v.point.x, &sphere_v.point.y, &sphere_v.point.z);
        fscanf(fin, "%f", &radius);
        fscanf(fin, "%s", material_str);

        if (!strcmp(material_str, "metal")) {
            fscanf(fin, "%f %f %f", &material_v.point.x, &material_v.point.y, &material_v.point.z);

            sphere = (Sphere *) malloc(sizeof(Sphere));
            metal = (Metal *) malloc(sizeof(Metal));

            *metal = new_metal(material_v, 0.0);
            *sphere = new_sphere(sphere_v, radius, (void *) metal, metal->mat.scatter);
        } else if (!strcmp(material_str, "lambertian")) {
            fscanf(fin, "%f %f %f", &material_v.point.x, &material_v.point.y, &material_v.point.z);

            sphere = (Sphere *) malloc(sizeof(Sphere));
            lam = (Lambertian *) malloc(sizeof(Lambertian));

            *lam = new_lambertian(material_v);
            *sphere = new_sphere(sphere_v, radius, (void *) lam, lam->mat.scatter);
        } else if (!strcmp(material_str, "dielectric")) {
            float ref_idx;
            fscanf(fin, "%f", &ref_idx);

            sphere = (Sphere *) malloc(sizeof(Sphere));
            diel = (Dielectric *) malloc(sizeof(Dielectric));

            *diel = new_dielectric(ref_idx);
            *sphere = new_sphere(sphere_v, radius, (void *) diel, diel->mat.scatter);
        } else {
            fprintf(stderr, "Unkown Material\n");
            exit(-1);
        }

        list[i] = &sphere->hitable;
        list[i]->inst = (void *) sphere;
    }

    Hitable_List world = new_hitable_list(list, sphere_count);

    Vector lookfrom = new_vector(new_point(13, 2, 3));
    Vector lookat = new_vector(new_point(0, 0, 0));
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    Camera cam = new_camera(lookfrom, lookat, new_vector(new_point(0, 1, 0)), 20, (float) nx / (float) ny, aperture, dist_to_focus);

    //omp_set_num_threads(8);//omp_get_num_procs());
    Color *output = (Color *) malloc(nx * ny * sizeof(Color));

    #pragma omp parallel firstprivate(nx, ny, ns, cam, world)
    {
        #pragma omp for schedule(dynamic, 10) collapse(2)
        for (int j = ny - 1; j >= 0; j--) {
            for (int i = 0; i < nx; i++) {
                Vector col = new_vector(new_point(0, 0, 0));
                float point_x = 0.0, point_y = 0.0, point_z = 0.0;

                /* Sampling ns times per pixel area */
                #pragma omp parallel for reduction(+:point_x, point_y, point_z) default(none) firstprivate(i, j, nx, ny, ns, cam, world) schedule(dynamic, 1)
                for (int s = 0; s < ns; s++) {
                    /* Centered at the center point of the pixel, the pixel outward distance is (0.0, 1.0] */
                    float u = (float) (i + drand48()) / (float) nx;
                    float v = (float) (j + drand48()) / (float) ny;

                    /* Obtain the color value of the random sampling point in this pixel area */
                    Ray r = cam.get_ray(cam, u, v);
                    // Vector p = r.point_at_parameter(r, 2.0);

                    /* Accumulate the color values of all ns random sample points of this point area */
                    Vector temp = color(r, &world, 0);
                    point_x += temp.point.x;
                    point_y += temp.point.y;
                    point_z += temp.point.z;
                }

                col.point.x = point_x;
                col.point.y = point_y;
                col.point.z = point_z;
                
                /* Divide the color accumulated value of all ns random sampling points of this pixel area by ns to obtain the average value */
                col = col.div_scalar(col, (float) ns);
                col = new_vector(new_point(sqrt(col.point.x), sqrt(col.point.y), sqrt(col.point.z)));
                int ir = (int) (255.99 * col.point.x);
                int ig = (int) (255.99 * col.point.y);
                int ib = (int) (255.99 * col.point.z);

                output[i * ny + j] = new_color(ir, ig, ib);
            }
        }
    }

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            int ir = output[i * ny + j].r;
            int ig = output[i * ny + j].g;
            int ib = output[i * ny + j].b;

            fprintf(fout, "%d %d %d\n", ir, ig, ib);
        }
    }

    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    struct timespec elapsed = diff_time(start_time, end_time);
    double elapsed_time = elapsed.tv_sec + (double) elapsed.tv_nsec / 1000000000.0;

    printf("Elapsed: %f seconds\n", elapsed_time);

    fclose(fin);
    fclose(fout);

    for (int i = 0; i < sphere_count; i++) {
        Sphere *sphere = (Sphere *) list[i]->inst;

        free(sphere->mat_ptr.inst);
        free(sphere);
    }

    return 0;
}
