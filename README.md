# Parallel Mandelbrot Set

Este projeto implementa e compara versões sequencial e paralela do cálculo do conjunto de Mandelbrot usando OpenMP.

## O que é o Conjunto de Mandelbrot?

O conjunto de Mandelbrot é um fractal matemático definido pela iteração da função complexa:

```
z_{n+1} = z_n² + c
```

Onde `z` e `c` são números complexos. Um ponto `c` pertence ao conjunto se a sequência não diverge para o infinito após infinitas iterações. Na prática, utilizamos um número máximo de iterações para determinar se um ponto diverge ou não.

A visualização do conjunto de Mandelbrot requer cálculos intensivos para cada pixel da imagem, tornando-se um excelente caso de uso para demonstrar técnicas de paralelização, já que cada pixel pode ser calculado independentemente.

## Estratégias de Paralelização

Nós exploramos algumas abordagens para paralelizar o cálculo do conjunto Mandelbrot. Analisamos que o gargalo computacional está no triplo loop aninhado (linhas 33-65), onde cada pixel requer até 850.000 iterações da fórmula complexa.

### 1. Análise Inicial dos Loops

```cpp
// Loop externo, linhas da imagem (PARALELIZÁVEL)
for (int row = 0; row < image_height; row++) {
    // Loop intermediário, colunas da imagem (OVERHEAD > GANHO)
    for (int col = 0; col < image_width; col++) {
        // Loop interno, iterações do Mandelbrot (DEPENDÊNCIA SEQUENCIAL)
        for (int k = 1; k <= max_iterations; k++) {
            // resto do código
        }
    }
}
```
- **Loop externo** (linhas): Ideal para paralelização (cada linha é independente)
- **Loop intermeddiário** (colunas): Paralelizável, mas overhead > benefício (o custo de gerenciar threads supera o ganho de paralelizar os 1000 pixels)
- **Loop interno** (iterações): **Impossível paralelizar**, cada iteração depende da anterior.

### 2. Paralelização Básica com OpenMP

Primeiro tentamos um simples `#pragma omp parallel for` no loop externo, para validar nossas observações:

```cpp
#pragma omp parallel for
for (int row = 0; row < image_height; row++) {
    // resto do código
}
```

**Resultado:** Speedup significativo, confirmando que esta é a estratégia correta.

### 3. Otimização do Scheduling

Testamos diferentes estratégias de escalonamento devido à carga desequilibrada do Mandelbrot (pixels próximos às bordas convergem mais rápido):

- **`schedule(static)`**: Divisão fixa; Justamente pela carga desequilibrada não era uma boa opção, já que threads ficavam ociosas
- **`schedule(dynamic)`**: Redistribuição dinâmica; Diferentemente do static, assim que uma thread termina o que ela estava fazendo, ela pega mais trabalho, foi o melhor approach em altas iterações
- **`schedule(guided)`**: Chunks adaptativos; O guided divide um grande número de iterações entre as threads inicialmente e depois vai reduzindo esse número, trás um bom balanceamento inicial, mas não trouxe mais performance nos nossos testes

**Descoberta:** Com 850.000 iterações, `dynamic` superou significativamente `guided` devido à redistribuição constante do trabalho.

### 4. Otimização de Chunk Size

Uma vez que validamos o uso do schedule, testamos alguns tamanhos de chunk (quantidade de iterações que a thread vai pegar, no caso do Dynamic, e mínimo de iterações, no caso do Guided) para `schedule(dynamic, X)`:

| Chunk Size | Tempo | Observação |
|------------|-------|------------|
| 1 (default) | Baseline | Balanceamento máximo, overhead alto |
| **2** | **~-2s** | **Melhor resultado** |
| 4 | ~+0.5s | Um pouco pior |
| 8, 16, 100 | ~+1-3s | Overhead crescente |

### 5. Tentativa com Collapse

Uma vez que o schedule estava validado, tentamos ir para outros caminhos: adaptamos o código e experimentamos `collapse(2)` para paralelizar ambos os loops externos simultaneamente:

```cpp
#pragma omp parallel for schedule(dynamic) collapse(2)
for (int row = 0; row < image_height; row++) {
    for (int col = 0; col < image_width; col++) {
        // resto do código
    }
}
```

**Resultado:** 
- Com poucas iterações (10.000-50.000), obtivemos ganho de aproximadamente ~5 segundos. 
- Em testes com mais de 800.000 iterações, onde o tempo linear supera 10 minutos, o desempenho piorou significativamente, aumentando o tempo de execução em ~1-2 minutos comparado à paralelização sem collapse.

### 6. Divisão em Blocos (Block-based Parallelization)

Por fim, nos inspiramos nas técnicas de divisão de vetores mostradas em aula, e tentamos dividir a imagem em blocos, pensando que pudesse melhorar o desempenho. Validamos com blocos 20x20 e 50x50:

```cpp
#pragma omp parallel for schedule(dynamic)
for (int block_row = 0; block_row < image_height; block_row += 50) {
    for (int block_col = 0; block_col < image_width; block_col += 50) {
        // resto do código, com os for's limitados a block_row e block_col, ao invés de image_height e image_width
    }
}
```

**Resultado:** Performance neutra (nem melhorou, nem piorou)

### 7. Estratégia Final

A melhor configuração que encontramos foi:

```cpp
#pragma omp parallel for schedule(dynamic, 2)
for (int row = 0; row < image_height; row++) {
    // resto do código
}
```

Esta abordagem resultou em speedup de 3.41x com 4 threads (de 10min44s para 3min06s).

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

O programa gera uma imagem do conjunto de Mandelbrot no formato PPM (1000×1000 pixels). Quanto mais iterações utilizadas, mais definidas e suaves ficam as bordas do conjunto.

![Conjunto de Mandelbrot gerado com 850.000 iterações](images/mandelbrot.png)

*Imagem do conjunto de Mandelbrot gerada pelo programa (1000×1000 pixels, 850.000 iterações)*

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

