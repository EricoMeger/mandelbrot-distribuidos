#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>

int i4_min(int i1, int i2) {
    if(i1 < i2) return i1;
    return i2;
}

int main() {
    int m = 1000;
    int n = 1000;

    timespec ts_start{}, ts_end{};
    double time_total;
    int c;
    int count_max = 1000;

    char const *filename = "mandelbrot.ppm";

    double x_max = 1.25;
    double x_min = -2.25;
    double y_max = 1.75;
    double y_min = -1.75;

    std::vector<std::vector<int>> r(m, std::vector<int>(n));
    std::vector<std::vector<int>> g(m, std::vector<int>(n));
    std::vector<std::vector<int>> b(m, std::vector<int>(n));
    std::vector<std::vector<int>> count(m, std::vector<int>(n));
    
    clock_gettime(CLOCK_MONOTONIC, &ts_start);

    for (int i = 0; i < m; i++) {
        double y = ((i - 1) * y_max + (m - i) * y_min) / (m - 1);
        for (int j = 0; j < n; j++) {
            double x = ((j - 1) * x_max + (n - j) * x_min) / (n - 1);
            count[i][j] = 0;
            double x1 = x;
            double y1 = y;

            for (int k = 1; k <= count_max; k++) {
                double x2 = x1 * x1 - y1 * y1 + x;
                double y2 = 2 * x1 * y1 + y;

                if (x2 < -2.0 || 2.0 < x2 || y2 < -2.0 || 2.0 < y2) {
                    count[i][j] = k;
                    break;
                }
                x1 = x2;
                y1 = y2;
            }

            if (count[i][j] == 0) {
                r[i][j] = 0;
                g[i][j] = 0;
                b[i][j] = 0;
            } else if ((count[i][j] % 2) == 1) {
                r[i][j] = 255;
                g[i][j] = 255;
                b[i][j] = 255;
            } else {
                c = static_cast<int>(255.0 * (1 - std::log(count[i][j]) / std::log(count_max)));
                r[i][j] = c;
                g[i][j] = c;
                b[i][j] = c;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    time_total = (ts_end.tv_sec - ts_start.tv_sec) * 1e9 + (ts_end.tv_nsec - ts_start.tv_nsec);

    std::cout << "\nTotal time is " << time_total / 1e6 << " ms\n";

    std::ofstream output_unit(filename);
    // para o SO entender o arquivo como imagem PPM ele espera:
    // Tipo do arquvio
    output_unit << "P3\n";
    // width e height
    output_unit << n << " " << m << "\n";
    // valor maximo
    output_unit << "255\n";

    // R G B R G B... valor de cada pixel
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            output_unit << r[i][j] << " " << g[i][j] << " " << b[i][j];
            if (j < n - 1) output_unit << " ";
        }
        output_unit << "\n";
    }

    output_unit.close();
    std::cout << "\nGraphics data written to \"" << filename << "\".\n\n";

    return 0;
}