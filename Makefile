CFLAGS = -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_ilp64 -lmkl_gnu_thread -lmkl_core -lgomp -lpthread -lm -ldl -lopenblas


exe: main.o activations.o averagepool.o maxpool.o  convolute_openblas.o convolute_mkl.o convolute_pthread.o convolute_multiple.o sigmoid.o softmax.o
	g++ $(CFLAGS) main.o activations.o averagepool.o maxpool.o convolute_openblas.o convolute_mkl.o convolute_pthread.o convolute_multiple.o sigmoid.o softmax.o -o exe

activations.o: activations.cpp activations.h
	g++ -c activations.cpp

maxpool.o: maxpool.cpp pooling.h
	g++ -c maxpool.cpp

averagepool.o: averagepool.cpp pooling.h
	g++ -c averagepool.cpp

convolute_openblas.o: convolute_openblas.cpp convolute_openblas.h
	g++ ${CFLAGS} -c convolute_openblas.cpp

convolute_mkl.o: convolute_mkl.cpp convolute_mkl.h
	g++ ${CFLAGS} -c convolute_mkl.cpp

convolute_pthread.o: convolute_pthread.cpp convolute_pthread.h
	g++ ${CFLAGS} -c convolute_pthread.cpp

convolute_multiple.o: convolute_multiple.cpp convolute_multiple.h
	g++ $(CFLAGS) -c convolute_multiple.cpp

sigmoid.o: sigmoid.cpp sigmoid.h
	g++ -c sigmoid.cpp

softmax.o: softmax.cpp softmax.h
	g++ -c softmax.cpp

clean:
	rm *.o exe
