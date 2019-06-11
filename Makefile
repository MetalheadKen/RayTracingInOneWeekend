EXEC = raytracing

.PHONY: all test check clean

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
	vector.o

OUTPUTS := \
	out1.ppm \
	out2.ppm

ifeq ($(strip $(PROFILE)), 1)
	PRO_FLAGS = -pg
	CFLAGS += $(PRO_FLAGS)
	LDFLAGS += $(PRO_FLAGS)
endif

ifeq ($(strip $(OPT)), 1)
	OPT_FLAGS = -fopenmp
	EXEC = raytracing_opt
	OBJS += main_opt.o
	CFLAGS += $(OPT_FLAGS)
	LDFLAGS += $(OPT_FLAGS)
else
	OBJS += main.o
endif

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

test: $(EXEC)
	./$(EXEC) trace1.txt out1.ppm
	./$(EXEC) trace2.txt out2.ppm

check: $(EXEC)
	@./$(EXEC) trace1.txt out1.ppm && diff -qu baseline.ppm out1.ppm || (echo Fail; exit -1)
	@echo "Verified OK"

best: $(EXEC)

clean:
	rm -f $(EXEC) raytracing_opt $(OBJS) main_opt.o $(OUTPUTS)

