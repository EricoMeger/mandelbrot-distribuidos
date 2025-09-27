#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <omp.h>

const uint8_t THREADS = 2;

int main() {
    const short int image_height = 1000;
    const short int image_width = 1000;

    const int max_iterations = 700000;
    // 650000 iteracoes -> 10min12s
    // 700000 iteracoes -> 11min20s
    const char* output_filename = "mandelbrot.ppm";

    // limites do plano complexo
    float plane_x_min = -2.25;
    float plane_x_max = 1.25;
    float plane_y_min = -1.75;
    float plane_y_max = 1.75;

    // matriz de cor e iterações
    std::vector<std::vector<int>> red(image_height, std::vector<int>(image_width));
    std::vector<std::vector<int>> green(image_height, std::vector<int>(image_width));
    std::vector<std::vector<int>> blue(image_height, std::vector<int>(image_width));
    std::vector<std::vector<int>> iteration_count(image_height, std::vector<int>(image_width));

    auto start = std::chrono::high_resolution_clock::now();

    omp_set_num_threads(THREADS);

    // calculo do mandelbrot
    #pragma omp parallel for schedule(dynamic, 2)
    for (int row = 0; row < image_height; row++) {
        double c_imag = plane_y_min + (row / static_cast<double>(image_height - 1)) * (plane_y_max - plane_y_min);
        
        // esse cara nao faz sentido pararelizar pq gera + overhead
        // pra cada thread do for externo, cria THREADS pro for interno
        for (int col = 0; col < image_width; col++) {
            double c_real = plane_x_min + (col / static_cast<double>(image_width - 1)) * (plane_x_max - plane_x_min);

            double z_real = c_real;
            double z_imag = c_imag;
            iteration_count[row][col] = 0;

            // itera z = z^2 + c até divergir ou atingir o maximo de iterações
            // esse cara nao dá pra paralelizar pq tem dependencia dos externos
            for (int k = 1; k <= max_iterations; k++) {
                double z_real_next = z_real * z_real - z_imag * z_imag + c_real;
                double z_imag_next = 2.0 * z_real * z_imag + c_imag;

                // se o valor passar de 2 com certeza vai divergir ao infinito
                if(z_real_next * z_real_next + z_imag_next * z_imag_next > 4.0)  {
                    iteration_count[row][col] = k;
                    break;
                }

                z_real = z_real_next;
                z_imag = z_imag_next;
            }

            if (iteration_count[row][col] == 0) {
                red[row][col] = green[row][col] = blue[row][col] = 0;  // se faz parte do Mandelbrot
            } else if (iteration_count[row][col] % 2 == 1) {
                red[row][col] = green[row][col] = blue[row][col] = 255; // white
            } else {
                int color_value = static_cast<int>(
                    255.0 * (1.0 - std::log(iteration_count[row][col]) / std::log(max_iterations))
                );
                red[row][col] = green[row][col] = blue[row][col] = color_value;
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    double time_total = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "\nTotal time is " << time_total << " ms\n";

    std::ofstream output(output_filename);
    // Header do PPM
    // tipo da imagem
    output << "P3\n";
    // tamanho width e height
    output << image_width << " " << image_height << "\n";
    // valor max do pixel
    output << "255\n";

    // R G B R G B... valor de cada pixel
    for (int row = 0; row < image_height; row++) {
        for (int col = 0; col < image_width; col++) {
            output << red[row][col] << " " << green[row][col] << " " << blue[row][col] << " ";
        }
        output << "\n";
    }

    output.close();
    std::cout << "\nImage written to \"" << output_filename << "\".\n\n";

    return 0;
}
