CC       = g++
CFLAGS   = -g -Wall
INCFLAGS := -I../helper_lib -I.
LDFLAGS  := ../helper_lib/helper_lib.a -lm -lclblast

ifeq ($(shell uname -o), Darwin)
	LDFLAGS += -framework OpenCL
else ifeq ($(shell uname -o), GNU/Linux) # Assumes NVIDIA GPU
	LDFLAGS  += -L/usr/local/cuda/lib64 -lOpenCL
	INCFLAGS += -I/usr/local/cuda/include
else # Android
	LDFLAGS += -lOpenCL
endif

all: m2 m1

m2:		../helper_lib/helper_lib.a m2.o ece408net.o src/network.o src/mnist.o layer.sentinel loss.sentinel custom.sentinel
		$(CC) $(CFLAGS) src/layer/custom/new-forward.cc m2.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o $(INCFLAGS) $(LDFLAGS) -o m2

m1:		../helper_lib/helper_lib.a m1.o ece408net.o src/network.o src/mnist.o layer.sentinel loss.sentinel custom.sentinel
		$(CC) $(CFLAGS) src/layer/custom/new-forward.cc m1.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o $(INCFLAGS) $(LDFLAGS) -o m1

# debug:	debug_m2

# debug_m1: m1.o ece408net.o src/network.o src/mnist.o layer.sentinel loss.sentinel src/layer/custom/cpu-new-forward.cc src/layer/custom/gpu-utils.cu src/layer/custom/new-forward.cu
# 		rm custom.sentinel || true
# 		nvcc -g -G -gencode arch=compute_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`,code=sm_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`  --compile src/layer/custom/cpu-new-forward.cc -o src/layer/custom/cpu-new-forward.o -I ../libgputk/ -I./
# 		nvcc -g -G -gencode arch=compute_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`,code=sm_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`  --compile src/layer/custom/gpu-utils.cu -o src/layer/custom/gpu-utils.o -I ../libgputk/ -I./
# 		nvcc -g -G -gencode arch=compute_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`,code=sm_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`  --compile src/layer/custom/new-forward.cu -o src/layer/custom/new-forward.o -I ../libgputk/ -I./
# 		nvcc -g -G -gencode arch=compute_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`,code=sm_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .` -o m1 -lm -lcuda -lrt m1.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o ../libgputk/lib/libgputk.a -I./ 

# debug_m2: m2.o ece408net.o src/network.o src/mnist.o layer.sentinel loss.sentinel src/layer/custom/cpu-new-forward.cc src/layer/custom/gpu-utils.cu src/layer/custom/new-forward.cu
# 		rm custom.sentinel || true
# 		nvcc -g -G -gencode arch=compute_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`,code=sm_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`  --compile src/layer/custom/cpu-new-forward.cc -o src/layer/custom/cpu-new-forward.o -I ../libgputk/ -I./
# 		nvcc -g -G -gencode arch=compute_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`,code=sm_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`  --compile src/layer/custom/gpu-utils.cu -o src/layer/custom/gpu-utils.o -I ../libgputk/ -I./
# 		nvcc -g -G -gencode arch=compute_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`,code=sm_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`  --compile src/layer/custom/new-forward.cu -o src/layer/custom/new-forward.o -I ../libgputk/ -I./
# 		nvcc -g -G -gencode arch=compute_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .`,code=sm_`/usr/local/nvidia/bin/nvidia-smi --query-gpu=compute_cap --format=csv | tail -n 1 | tr -d .` -o m2 -lm -lcuda -lrt m2.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o ../libgputk/lib/libgputk.a -I./ 

m2.o:		m2.cc
		$(CC) $(CFLAGS) -c m2.cc -o m2.o $(INCFLAGS)

m1.o:		m1.cc
		$(CC) $(CFLAGS) -c m1.cc -o m1.o $(INCFLAGS)

ece408net.o:    ece408net.cc
		$(CC) $(CFLAGS) -c ece408net.cc -o ece408net.o $(INCFLAGS)

src/network.o:	src/network.cc
		$(CC) $(CFLAGS) -c src/network.cc -o src/network.o $(INCFLAGS)

src/mnist.o:	src/mnist.cc
		$(CC) $(CFLAGS) -c src/mnist.cc -o src/mnist.o $(INCFLAGS)

layer.sentinel:		src/layer/conv.cc src/layer/ave_pooling.cc src/layer/conv_cust.cc src/layer/fully_connected.cc src/layer/max_pooling.cc src/layer/relu.cc src/layer/sigmoid.cc src/layer/softmax.cc 
		$(CC) $(CFLAGS) -c src/layer/ave_pooling.cc -o src/layer/ave_pooling.o $(INCFLAGS)
		$(CC) $(CFLAGS) -c src/layer/conv.cc -o src/layer/conv.o $(INCFLAGS)
		$(CC) $(CFLAGS) -c src/layer/conv_cust.cc -o src/layer/conv_cust.o $(INCFLAGS)
		$(CC) $(CFLAGS) -c src/layer/fully_connected.cc -o src/layer/fully_connected.o $(INCFLAGS)
		$(CC) $(CFLAGS) -c src/layer/max_pooling.cc -o src/layer/max_pooling.o $(INCFLAGS)
		$(CC) $(CFLAGS) -c src/layer/relu.cc -o src/layer/relu.o $(INCFLAGS)
		$(CC) $(CFLAGS) -c src/layer/sigmoid.cc -o src/layer/sigmoid.o $(INCFLAGS)
		$(CC) $(CFLAGS) -c src/layer/softmax.cc -o src/layer/softmax.o $(INCFLAGS)
		touch layer.sentinel

custom.sentinel: src/layer/custom/opencl.cc src/layer/custom/new-forward.cc
		$(CC) $(CFLAGS) -c src/layer/custom/opencl.cc -o src/layer/custom/opencl.o $(INCFLAGS)
		# $(CC) $(CFLAGS) -c src/layer/custom/new-forward.cc -o src/layer/custom/new-forward.o $(INCFLAGS)
		touch custom.sentinel

loss.sentinel:           src/loss/cross_entropy_loss.cc src/loss/mse_loss.cc
		$(CC) $(CFLAGS) -c src/loss/cross_entropy_loss.cc -o src/loss/new-cross_entropy_loss.o $(INCFLAGS)
		$(CC) $(CFLAGS) -c src/loss/mse_loss.cc -o src/loss/new-mse_loss.o $(INCFLAGS)
		touch loss.sentinel

../helper_lib/helper_lib.a: 
	cd ../helper_lib; make


clean:
		find . -name "*.o" -type f -delete
		rm *.sentinel
		rm m2 || true
		rm m1 || true
		cd ../helper_lib; make clean

cpu:		m1
		./m1 1000


gpu: 		m2
		./m2 1000

time: time_gpu

time_gpu: 		m2
		python3 ../utils/profile.py  --args ./m2 1000

time_cpu: 	m1
		python3 ../utils/profile.py  --args ./m1 1000
