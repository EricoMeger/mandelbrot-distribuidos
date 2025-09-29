# Parallel Mandelbrot Set

Este projeto implementa e compara versões sequencial e paralela do cálculo do conjunto de Mandelbrot usando OpenMP.

## O que é o Conjunto de Mandelbrot?

O conjunto de Mandelbrot é um fractal matemático definido pela iteração da função complexa:

```
z_{n+1} = z_n² + c
```

Onde `z` e `c` são números complexos. Um ponto `c` pertence ao conjunto se a sequência não diverge para o infinito após infinitas iterações. Na prática, utilizamos um número máximo de iterações para determinar se um ponto diverge ou não.

A visualização do conjunto de Mandelbrot requer cálculos intensivos para cada pixel da imagem, tornando-se um excelente caso de uso para demonstrar técnicas de paralelização, já que cada pixel pode ser calculado independentemente.

## Estratégias para paralelizar

asfsdgsdgsdg

# Como rodar o projeto

O projeto utiliza Makefiles para facilitar a compilação e execução dos testes.

## 1. Compilação

Para compilar as versões do programa, acesse a pasta `code/`:

```bash
$ cd code
```

## 2. Executar versões

- **Para rodar a versão sequencial:**

```bash
$ make linear
$ ./mandelbrot_linear
```

- **Para rodar a versão paralela:**

```bash
$ make threaded
$ ./mandelbrot_paralelizado
```

## 3. Executar testes de performance

Para executar todos os testes comparativos:

```bash
$ cd tests
$ make test
$ ./run_tests.sh
```

Para compilar e executar testes individuais:

```bash
$ cd tests
$ make 2_dynamic    # 2 threads com scheduling dinâmico
$ make 4_guided     # 4 threads com scheduling guiado
# ...
```

## Saída do Programa

O programa gera uma imagem do conjunto de Mandelbrot no formato PPM (1000×1000 pixels). Quanto mais iterações utilizadas, mais definidas e suaves ficam as bordas do conjunto, revelando os fractais.

------- adicioanr imagem

## Resultados de Performance

Todos os testes foram executados com 850.000 iterações em uma imagem de 1000×1000 pixels:

| Configuração | Threads | Scheduling | Tempo Médio | Speedup |
|--------------|---------|------------|-------------|---------|
| **Sequencial** | 1 | - | 626.87s (10min44s) | 1.00x |
| **Paralelo** | 2 | Dynamic | 312.29s (5min20s) | **2.01x** |
| **Paralelo** | 2 | Guided | 311.91s (5min19s) | **2.01x** |
| **Paralelo** | 4 | Dynamic | 183.88s (3min06s) | **3.41x** |
| **Paralelo** | 4 | Guided | 330.35s (5min50s) | 1.90x |
| **Paralelo** | 8 | Dynamic | 187.18s (3min11s) | **3.35x** |
| **Paralelo** | 8 | Guided | 209.16s (3min48s) | 3.00x |

### Especificações do Sistema de Teste

Os benchmarks foram executados no seguinte hardware:
- **Processador**: Intel Core i3-7100
- **Núcleos/Threads**: 2 Cores 4 Threads
- **Memória RAM**: 8gb DDR4
- **Sistema Operacional**: Ubuntu 24.04
- **Compilador**: GCC com OpenMP

### Análise dos Resultados

- **Performance**: 4 threads com scheduling dinâmico (3min06s, speedup 3.41x)
- **Escalabilidade**: Com 8 threads, observamos diminuição do ganho devido ao overhead de sincronização
- **Redução de Tempo**: Conseguimos reduzir o tempo de processamento de **10min42s para 3min06s** (uma melhoria de **71%**)

## Estrutura do Projeto

```
├── code/
│   ├── mandelbrot_linear.cpp      # Versão sequencial
│   ├── mandelbrot_paralelizado.cpp # Versão paralela principal
│   ├── makefile
│   └── tests/                     # Testes de performance
│       ├── mandelbrot_*_*.cpp     # Variações de teste
│       ├── run_tests.sh           # Script de execução
│       └── makefile
└── report/                        # Relatório técnico em LaTeX
    ├── main.tex
    └── main.pdf
```

## Referências

- [Fractal Geometry - Yale](https://gauss.math.yale.edu/public_html/People/frame/Fractals/MandelSet/welcome.html)
- [Parallel Mandelbrot with OpenMP - Sergey Vassiliev](https://ssvassiliev.github.io/Summer_School_OpenMP/08-mandel/index.html)
- [OpenMP API 6.0 Reference Guide](https://www.openmp.org/wp-content/uploads/OpenMP-RefGuide-6.0-OMP60SC24-web.pdf)
- [OpenMP Dynamic vs Guided Scheduling](https://stackoverflow.com/questions/42970700/openmp-dynamic-vs-guided-scheduling)

