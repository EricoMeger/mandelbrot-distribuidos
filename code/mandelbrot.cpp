#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>

int main() {
    int image_height = 1000;
    int image_width = 1000;

    const int max_iterations = 700000;
    const char* output_filename = "mandelbrot.ppm";

    // limites do plano complexo
    double plane_x_min = -2.25;
    double plane_x_max = 1.25;
    double plane_y_min = -1.75;
    double plane_y_max = 1.75;

    // matriz de cor e iterações
    std::vector<std::vector<int>> red(image_height, std::vector<int>(image_width));
    std::vector<std::vector<int>> green(image_height, std::vector<int>(image_width));
    std::vector<std::vector<int>> blue(image_height, std::vector<int>(image_width));
    std::vector<std::vector<int>> iteration_count(image_height, std::vector<int>(image_width));

    timespec ts_start{}, ts_end{};
    clock_gettime(CLOCK_MONOTONIC, &ts_start);

    // calculo do mandelbrot
    for (int row = 0; row < image_height; row++) {
        // mapeia a coluna de pixels para o y imaignario
        double c_imag = plane_y_min + (row / static_cast<double>(image_height - 1)) * (plane_y_max - plane_y_min);

        for (int col = 0; col < image_width; col++) {
            // mapaeia a coluna de pixels para o x real
            double c_real = plane_x_min + (col / static_cast<double>(image_width - 1)) * (plane_x_max - plane_x_min);

            double z_real = c_real;
            double z_imag = c_imag;
            iteration_count[row][col] = 0;

            // itera z = z^2 + c até divergir ou atingir o maximo de iterações
            for (int k = 1; k <= max_iterations; k++) {
                double z_real_next = z_real * z_real - z_imag * z_imag + c_real;
                double z_imag_next = 2.0 * z_real * z_imag + c_imag;

                // se o valor passar de 2 com certeza vai divergir ao infinito
                if(std::abs(z_real_next) > 2.0 || std::abs(z_imag_next) > 2.0) {
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

    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    double total_time_ns = (ts_end.tv_sec - ts_start.tv_sec) * 1e9 + (ts_end.tv_nsec - ts_start.tv_nsec);
    std::cout << "\nTotal time: " << total_time_ns / 1e6 << " ms\n";

    std::ofstream output(output_filename);
    // Header do PPM
    // tipo da imagem
    output << "P3\n";
    // tamanho width e height
    output << image_width << " " << image_height << "\n";
    // valor max do pixel
    output << "255\n";

    for (int row = 0; row < image_height; row++) {
        for (int col = 0; col < image_width; col++) {
            output << red[row][col] << " " << green[row][col] << " " << blue[row][col];
            if (col < image_width - 1) output << " ";
        }
        output << "\n";
    }

    output.close();
    std::cout << "\nImage written to \"" << output_filename << "\".\n\n";

    return 0;
}
