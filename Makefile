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

OUTPUTS := \
	out1.ppm \
	out2.ppm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

test: $(EXEC)
	./$(EXEC) trace1.txt out1.ppm
	./$(EXEC) trace2.txt out2.ppm

check: $(EXEC)
	@./$(EXEC) trace1.txt out1.ppm && diff -u baseline.ppm out1.ppm || (echo Fail; exit -1)
	@echo "Verified OK"

clean:
	rm -f $(EXEC) $(OBJS) $(OUTPUTS)

