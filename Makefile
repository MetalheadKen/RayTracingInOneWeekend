EXEC = raytracing

.PHONY: all check clean
all: $(EXEC)

CC = gcc
CFLAGS = -std=gnu99 -Wall -O0 -g
LDFLAGS = -lm

OBJS := \
	material.o \
	dielectric.o \
	lambertian.o \
	metal.o \
	hitable_list.o \
	camera.o \
	sphere.o \
	ray.o \
	vector.o \
	main.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

check: $(EXEC)
	@./$(EXEC) && diff -u baseline.ppm out.ppm || (echo Fail; exit)
	@echo "Verified OK"

clean:
	rm -f $(EXEC) $(OBJS) out.ppm
