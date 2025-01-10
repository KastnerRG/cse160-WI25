CC = gcc 
CFLAGS = -O2 -Wall

ifeq ($(shell uname -s),Darwin)
    LDFLAGS = -framework OpenCL
else
    LDFLAGS = -L/usr/local/cuda/lib64 -lOpenCL
endif

# might not be necessary, uncomment if you need it
# INCFLAGS = -I/usr/local/cuda/include
MATHFLAG = -lm

all: raytracer_parallel
raytracer_parallel: main.c lib/vec_ops.c 
	$(CC) $(CFLAGS) -o raytracer_parallel main.c $(INCFLAGS) $(LDFLAGS) $(MATHFLAG)

gpu: raytracer_parallel
	./raytracer_parallel 1

cpu: raytracer_parallel
	./raytracer_parallel 0

clean:
	rm -f raytracer_parallel
