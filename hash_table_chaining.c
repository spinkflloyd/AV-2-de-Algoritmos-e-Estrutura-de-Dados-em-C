#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_TABELA 13   // numero primo

// cada nó da lista encadeada equivale ao "nodetype" do livro: campos k (chave), r (registro/valor) e next (proximo nó).  
typedef struct No {
    char k[50];        // chave (key)    
    int  r;            // registro/valor (record) - ex: nota, matricula  
    struct No *next;   // ponteiro para o proximo nó da lista  
} No;

// a tabela de espalhamento e um vetor de cabecalhos (buckets)  
typedef struct {
    No *bucket[TAMANHO_TABELA];
} TabelaHash;

void inicializaTabela(TabelaHash *t) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        t->bucket[i] = NULL;
    }
}

// função de espalhamento: desdobramento (folding) + método da divisao
int funcaoEspalhamento(const char *chave) {
    unsigned long soma = 0;

    // desdobramento: soma o codigo ASCII de cada caractere, ponderado pela posição (i+1), para que toda a chave influencie o resultado e para que os anagramas não colidam sempre
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += (unsigned char) chave[i] * (i + 1);
    }
    return (int) (soma % TAMANHO_TABELA);
}

// busca e inserção: segue o mesmo roteiro do livro: percorre a lista bucket[i] procurando a chave; se encontrar, retorna/atualiza; se não, insere um novo no no final da lista
void inserir(TabelaHash *t, const char *chave, int valor) {
    int i = funcaoEspalhamento(chave);
    No *p = t->bucket[i];
    No *anterior = NULL;

    printf("\n[INSERIR] chave=\"%s\" valor=%d\n", chave, valor);
    printf("  -> h(\"%s\") = %d  (bucket a examinar)\n", chave, i);

    // percorre a lista ate achar a chave ou chegar no final 
    while (p != NULL && strcmp(p->k, chave) != 0) {
        anterior = p;
        p = p->next;
    }

    if (p != NULL) {
        printf("  -> chave ja existia no bucket %d. valor atualizado de %d para %d.\n",
               i, p->r, valor);
        p->r = valor;
        return;
    }

    // chave não encontrada: cria o novo No e insere no final da lista  
    No *novo = (No *) malloc(sizeof(No));
    strcpy(novo->k, chave);
    novo->r = valor;
    novo->next = NULL;

    if (anterior == NULL) {
        printf("  -> bucket %d estava vazio. insercao direta (sem colisao).\n", i);
        t->bucket[i] = novo;
    } else {
        printf("  -> colisao detectada no bucket %d! cadeia atual:", i);
        No *tmp = t->bucket[i];
        while (tmp != NULL) { printf(" \"%s\"", tmp->k); tmp = tmp->next; }
        printf(". nova chave encadeada no final da lista.\n");
        anterior->next = novo;
    }
}

// busca 
int buscar(TabelaHash *t, const char *chave) {
    int i = funcaoEspalhamento(chave);
    int provas = 0;

    printf("\n[BUSCAR] chave=\"%s\"\n", chave);
    printf("  -> h(\"%s\") = %d  (bucket a inspecionar)\n", chave, i);

    No *p = t->bucket[i];
    if (p == NULL) {
        printf("  -> bucket %d esta vazio. chave nao encontrada (0 provas).\n", i);
        return -1;
    }

    while (p != NULL) {
        provas++;
        printf("  -> prova %d: comparando com \"%s\" no bucket %d...\n", provas, p->k, i);
        if (strcmp(p->k, chave) == 0) {
            printf("  -> encontrado. valor = %d (%d prova(s))\n", p->r, provas);
            return p->r;
        }
        p = p->next;
    }

    printf("  -> lista do bucket %d percorrida por completo (%d prova(s)). Chave nao encontrada.\n",
           i, provas);
    return -1;
}

// remoção 
int remover(TabelaHash *t, const char *chave) {
    int i = funcaoEspalhamento(chave);
    No *p = t->bucket[i];
    No *anterior = NULL;

    printf("\n[remover] chave=\"%s\" (bucket %d)\n", chave, i);

    while (p != NULL && strcmp(p->k, chave) != 0) {
        anterior = p;
        p = p->next;
    }
    if (p == NULL) {
        printf("  -> chave nao encontrada, nada a remover.\n");
        return 0;
    }
    if (anterior == NULL) {
        t->bucket[i] = p->next;
    } else {
        anterior->next = p->next;
    }
    printf("  -> removido com sucesso.\n");
    free(p);
    return 1;
}

// imprime o estado completo da tabela 
void imprimeTabela(TabelaHash *t) {
    printf("\n/// estado atual da tabela de espalhamento ///\n");
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        printf("bucket[%2d]: ", i);
        No *p = t->bucket[i];
        if (p == NULL) {
            printf("(vazio)");
        }
        while (p != NULL) {
            printf("[\"%s\":%d] -> ", p->k, p->r);
            p = p->next;
        }
        if (t->bucket[i] != NULL) printf("NULL");
        printf("\n");
    }
    printf("==================================================================\n");
}

void liberaTabela(TabelaHash *t) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        No *p = t->bucket[i];
        while (p != NULL) {
            No *tmp = p;
            p = p->next;
            free(tmp);
        }
    }
}

int main(void) {
    TabelaHash tabela;
    inicializaTabela(&tabela);

    printf("//////////////////////////////////////////////////////\n");
    printf("// tabela de espalhamento com encadeamento separado //\n");
    printf("// tamanho da tabela (primo): %d                    //\n", TAMANHO_TABELA);
    printf("//////////////////////////////////////////////////////\n");

    inserir(&tabela, "Spinkfloyd", 90);
    inserir(&tabela, "Songa", 75);
    inserir(&tabela, "Aoberica", 88);
    inserir(&tabela, "Guzin", 60);
    inserir(&tabela, "Sennin", 95);
    inserir(&tabela, "Gokulandia", 70);
    inserir(&tabela, "Jojoliano", 82);

    imprimeTabela(&tabela);

    // atualização de uma chave ja existente  
    inserir(&tabela, "Songa", 100);

    // buscas de uma chave existente e uma inexistente  
    buscar(&tabela, "Aoberica");
    buscar(&tabela, "Penaldo");   // não inserida -> demonstra busca sem sucesso  

    imprimeTabela(&tabela);

    // remoção de uma chave e nova busca para confirmar  
    remover(&tabela, "Guzin");
    buscar(&tabela, "Guzin");

    imprimeTabela(&tabela);

    liberaTabela(&tabela);
    return 0;
}