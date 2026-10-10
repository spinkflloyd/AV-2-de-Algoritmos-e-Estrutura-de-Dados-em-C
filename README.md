<div align="center">

# 💻 AED I · Atividade Avaliativa II 💻

---

## 📖 Sobre o projeto

Implementações em C dos algoritmos de ordenação **Shellsort**, **Insertion Sort** e **Merge Sort**, além de técnicas de espalhamento (**Hashing**), desenvolvidas com base no livro *Estruturas de Dados Usando C* para a segunda avaliação da disciplina de **Algoritmos e Estruturas de Dados I**.

## 🎓 Sobre a avaliação

| | |
|---|---|
| 📘 **Disciplina** | Algoritmos e Estruturas de Dados I |
| 🗓️ **Semestre** | 2026.1 |
| 💡 **Linguagem** | C |

<div align="left">
  
### 🐚 Shellsort e Mergesort

O programa tem duas partes:

1. **Demonstração passo a passo:** o Shellsort ordena o vetor de exemplo do livro (`25, 57, 48, 37, 12, 92, 86, 33`) com incrementos `{5, 3, 1}`, mostrando o vetor ao final de cada rodada. O Mergesort (iterativo, por intercalação direta) ordena um vetor de 10 elementos mostrando cada passagem e cada intercalação.
2. **Comparação de desempenho:** os dois algoritmos ordenam o mesmo vetor aleatório, com 1.000, 5.000 e 20.000 elementos, e o programa compara o número de **comparações**, de **movimentos** e o **tempo** de execução. O Shellsort usa a sequência de incrementos de Knuth (1, 4, 13, 40, ...).

**Ligação com o Insertion Sort:** cada rodada do Shellsort é uma inserção direta feita sobre elementos separados por uma distância (o incremento). Quando o incremento chega a 1, ele se comporta exatamente como o Insertion Sort clássico.

### #️⃣ Tabela de espalhamento com encadeamento

- **Tamanho da tabela:** 13, um número primo.
- **Função de espalhamento:** *desdobramento* (soma dos códigos ASCII de cada caractere, ponderados pela posição na chave) seguido do *método da divisão* (resto da divisão por 13). A ponderação faz toda a chave influenciar o resultado e evita que anagramas colidam sempre.
- **Tratamento de colisões:** encadeamento separado. Cada posição da tabela guarda o início de uma lista encadeada com as chaves que caíram nela.
- **Operações:** inserir (atualiza o valor se a chave já existe), buscar (informando quantas provas foram necessárias), remover e imprimir o estado completo da tabela.
- Os *prints* mostram o bucket examinado e avisam quando há colisão.

<div align="center">
  
## 🛠️ Tecnologias

| Linguagem |
|---|
| ![C](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=black) |
