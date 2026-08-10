#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long comparacoes_shell = 0, movimentos_shell = 0;
long comparacoes_merge = 0, movimentos_merge = 0;

void imprimeVetor(int v[], int n) {
    printf("[ ");
    for (int i = 0; i < n; i++) {
        printf("%d", v[i]);
        if (i < n - 1) printf(", ");
    }
    printf(" ]\n");
}

//   shellsort
//   para cada incremento "span" da sequência, o vetor é dividido em "span", sub-vetores entrelacados (posições i, i+span, i+2*span, ...) e cada um deles é ordenado por inserção direta. quando o span chega a 1, a rotina se comporta exatamente como o insertion sort clássico
void shellsort(int x[], int n, int incrementos[], int numinc, int mostrarPassos) {
    int incr, span, i, j, y;

    for (incr = 0; incr < numinc; incr++) {
        span = incrementos[incr];

        if (mostrarPassos) {
            printf("\n/// rodada %d: incremento (span) = %d ///\n", incr + 1, span);
        }

        // inserção direta tratando "span" como a distância entre os vizinhos  
        for (i = span; i < n; i++) {
            y = x[i];
            movimentos_shell++;
            j = i - span;

            while (j >= 0) {
                comparacoes_shell++;
                if (x[j] > y) {
                    x[j + span] = x[j];
                    movimentos_shell++;
                    j = j - span;
                } else {
                    break;
                }
            }
            x[j + span] = y;
            movimentos_shell++;
        }

        if (mostrarPassos) {
            printf("vetor no final da rodada com o span = %d: ", span);
            imprimeVetor(x, n);
        }
    }
}

// gera a sequência de incrementos de knuth: h(1)=1, h(k) = 3*h(k-1)+1, mantendo apenas os valores menores que n, em ordem decrescente (o vetor resultante já termina em 1, como o algoritmo exige). retorna o numero de incrementos gerados (numinc)
int geraIncrementosKnuth(int n, int incrementos[]) {
    int valores[40];
    int qtd = 0;
    valores[qtd++] = 1;
    while (1) {
        int proximo = 3 * valores[qtd - 1] + 1;
        if (proximo >= n) break;
        valores[qtd++] = proximo;
    }
    // copia em ordem decrescente
    for (int i = 0; i < qtd; i++) {
        incrementos[i] = valores[qtd - 1 - i];
    }
    return qtd;
}

//  mergesort 
//   usa um vetor auxiliar "aux" do mesmo tamanho de x, a variavel "size" começa em 1 e dobra a cada passagem, em cada passagem, percorremos o vetor intercalando pares adjacentes de sub-vetores de tamanho "size" [i1 .. u1] (tamanho size) com [i2 .. u2] (tamanho até size) ao final da passagem, o vetor auxiliar é copiado de volta para x, e "size" é dobrado para a proxima passagem
void mergesort(int x[], int n, int mostrarPassos) {
    int *aux = (int *) malloc(n * sizeof(int));
    int size = 1;
    int passagem = 1;

    while (size < n) {
        int i1 = 0;

        if (mostrarPassos) {
            printf("\n/// passagem %d: intercalando sub-vetores de tamanho %d ///\n",
                   passagem, size);
        }

        while (i1 + size < n) {
            int i2 = i1 + size;
            int u1 = i2 - 1;
            int u2 = (i2 + size - 1 < n) ? (i2 + size - 1) : (n - 1);

            int i = i1, j = i2, k = i1;

            if (mostrarPassos) {
                printf("  intercalando x[%d..%d] com x[%d..%d]\n", i1, u1, i2, u2);
            }

            while (i <= u1 && j <= u2) {
                comparacoes_merge++;
                if (x[i] <= x[j]) {
                    aux[k++] = x[i++];
                } else {
                    aux[k++] = x[j++];
                }
                movimentos_merge++;
            }
            while (i <= u1) { aux[k++] = x[i++]; movimentos_merge++; }
            while (j <= u2) { aux[k++] = x[j++]; movimentos_merge++; }

            i1 = i2 + size;
        }
        // copia qualquer sub-vetor isolado que sobrou sem par nessa passagem  
        for (int k = i1; k < n; k++) { aux[k] = x[k]; movimentos_merge++; }

        // copia o aux de volta para x  
        for (int k = 0; k < n; k++) x[k] = aux[k];

        if (mostrarPassos) {
            printf("  vetor no final da passagem: ");
            imprimeVetor(x, n);
        }

        size *= 2;
        passagem++;
    }

    free(aux);
}

// funções auxiliares para a comparação de desempenho
void copiaVetor(int origem[], int destino[], int n) {
    for (int i = 0; i < n; i++) destino[i] = origem[i];
}

void geraVetorAleatorio(int v[], int n) {
    for (int i = 0; i < n; i++) v[i] = rand() % 100000;
}

int estaOrdenado(int v[], int n) {
    for (int i = 1; i < n; i++) if (v[i - 1] > v[i]) return 0;
    return 1;
}

int main(void) {
    // demonstração do passo a passo (mesmo exemplo do livro-texto)
    printf("==========================================================\n");
    printf(" PARTE 1: DEMONSTRACAO PASSO A PASSO\n");
    printf("==========================================================\n");

    // shellsort: exemplo do livro-texto (seção 6.4)
    int demoShell[]   = {25, 57, 48, 37, 12, 92, 86, 33};
    int incrementosDemo[] = {5, 3, 1};   // sequência de incrementos do exemplo  
    int n_demo = sizeof(demoShell) / sizeof(demoShell[0]);
    int numincDemo = sizeof(incrementosDemo) / sizeof(incrementosDemo[0]);

    printf("\nshellsort: (vetor de exemplo, incrementos = {5, 3, 1})\n");
    printf("vetor original: ");
    imprimeVetor(demoShell, n_demo);
    shellsort(demoShell, n_demo, incrementosDemo, numincDemo, 1);
    printf("\nvetor ordenado (shellsort): ");
    imprimeVetor(demoShell, n_demo);

    // mergesort: vetor de demonstração (10 elementos)
    int demoMerge[] = {23, 4, 78, 12, 56, 9, 33, 1, 45, 67};
    int n_demo2 = sizeof(demoMerge) / sizeof(demoMerge[0]);

    printf("\n\nmergesort: (classificacao por intercalacao direta, iterativa)\n");
    printf("vetor original: ");
    imprimeVetor(demoMerge, n_demo2);
    mergesort(demoMerge, n_demo2, 1);
    printf("\nvetor ordenado (mergesort): ");
    imprimeVetor(demoMerge, n_demo2);

    // comparação de desempenho com vetores maiores
    int tamanhos[] = {1000, 5000, 20000};
    int qtd_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("\n\n==========================================================\n");
    printf(" PARTE 2: COMPARACAO DE DESEMPENHO (SHELLSORT x MERGESORT)\n");
    printf(" shellsort usa a sequencia de incrementos de knuth (1,4,13,40,...)\n");
    printf("==========================================================\n");

    srand((unsigned) time(NULL));

    for (int t = 0; t < qtd_tamanhos; t++) {
        int n = tamanhos[t];
        int *original = (int *) malloc(n * sizeof(int));
        int *vetorA   = (int *) malloc(n * sizeof(int));
        int *vetorB   = (int *) malloc(n * sizeof(int));
        int incrementos[40];
        int numinc;

        geraVetorAleatorio(original, n);
        copiaVetor(original, vetorA, n);
        copiaVetor(original, vetorB, n);

        numinc = geraIncrementosKnuth(n, incrementos);

        comparacoes_shell = movimentos_shell = 0;
        comparacoes_merge = movimentos_merge = 0;

        clock_t inicio, fim;
        double tempo_shell, tempo_merge;

        inicio = clock();
        shellsort(vetorA, n, incrementos, numinc, 0);
        fim = clock();
        tempo_shell = (double)(fim - inicio) / CLOCKS_PER_SEC;

        inicio = clock();
        mergesort(vetorB, n, 0);
        fim = clock();
        tempo_merge = (double)(fim - inicio) / CLOCKS_PER_SEC;

        printf("\n/// tamanho do vetor: n = %d  (numinc = %d, maior incremento = %d) ///\n",
               n, numinc, incrementos[0]);
        printf("shellsort  -> ordenado: %s | comparacoes: %10ld | movimentos: %10ld | tempo: %.6f s\n",
               estaOrdenado(vetorA, n) ? "sim" : "não", comparacoes_shell, movimentos_shell, tempo_shell);
        printf("mergesort  -> ordenado: %s | comparacoes: %10ld | movimentos: %10ld | tempo: %.6f s\n",
               estaOrdenado(vetorB, n) ? "sim" : "não", comparacoes_merge, movimentos_merge, tempo_merge);

        free(original);
        free(vetorA);
        free(vetorB);
    }

    printf("\nobservacao: com uma sequencia de incrementos primos entre si (knuth),\n");
    printf("o shellsort se aproxima de O(n * (log n)^2). o mergesort mantem sempre\n");
    printf("O(n log n), mas exige memoria auxiliar O(n), enquanto o shellsort ordena\n");
    printf("\"in place\" (praticamente sem memoria extra).\n");

    return 0;
}
