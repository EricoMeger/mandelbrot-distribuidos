2_dynamic:
	g++ mandelbrot_2_dynamic.cpp -o mandelbrot_2_dynamic -fopenmp

4_dynamic:
	g++ mandelbrot_4_dynamic.cpp -o mandelbrot_4_dynamic -fopenmp

8_dynamic:
	g++ mandelbrot_8_dynamic.cpp -o mandelbrot_8_dynamic -fopenmp

2_guided:
	g++ mandelbrot_2_guided.cpp -o mandelbrot_2_guided -fopenmp

4_guided:
	g++ mandelbrot_4_guided.cpp -o mandelbrot_4_guided -fopenmp

8_guided:
	g++ mandelbrot_8_guided.cpp -o mandelbrot_8_guided -fopenmp

test: 2_dynamic 4_dynamic 8_dynamic 2_guided 4_guided 8_guided

clean:
	rm -f mandelbrot_2_dynamic mandelbrot_4_dynamic mandelbrot_8_dynamic mandelbrot_2_guided mandelbrot_4_guided mandelbrot_8_guided *.ppm

