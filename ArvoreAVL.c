#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//nó da árvore com dados
typedef struct NO {
    char nome[100];   
    int id;          
    char email[100];
    int altura;
    struct NO *esq;
    struct NO *dir;
} NO;

//calcula altura do nó
int altura_NO(NO *no) {
    if (no == NULL)
        return -1;
    return no->altura;
}

//retorna o maior entre dois valores
int maior(int a, int b) {
    return (a > b) ? a : b;
}

//cria um novo nó
NO *novoNO(char nome[], int id, char email[]) {
    NO *no = (NO *)malloc(sizeof(NO));
    strcpy(no->nome, nome);
    no->id = id;
    strcpy(no->email, email);
    no->altura = 0;
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

//rotação à esquerda
NO *rotacaoRR(NO *raiz) {
    NO *no = raiz->dir;
    raiz->dir = no->esq;
    no->esq = raiz;
    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    no->altura = maior(altura_NO(no->esq), altura_NO(no->dir)) + 1;
    return no;
}

//rotação à direita
NO *rotacaoLL(NO *raiz) {
    NO *no = raiz->esq;
    raiz->esq = no->dir;
    no->dir = raiz;
    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    no->altura = maior(altura_NO(no->esq), altura_NO(no->dir)) + 1;
    return no;
}

//rotação dupla esquerda-direita
NO *rotacaoLR(NO *raiz) {
    raiz->esq = rotacaoRR(raiz->esq);
    return rotacaoLL(raiz);
}

//rotação dupla direita-esquerda
NO *rotacaoRL(NO *raiz) {
    raiz->dir = rotacaoLL(raiz->dir);
    return rotacaoRR(raiz);
}

//calcula o fator de balanceamento
int fatorBalanceamento(NO *no) {
    if (no == NULL)
        return 0;
    return altura_NO(no->esq) - altura_NO(no->dir);
}

//insere um usuário 
NO *inserir(NO *raiz, char nome[], int id, char email[]) {
    if (raiz == NULL)
        return novoNO(nome, id, email);

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = inserir(raiz->esq, nome, id, email);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = inserir(raiz->dir, nome, id, email);
    else
        return raiz; 

    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;

    int fb = fatorBalanceamento(raiz);

    //desbalanceamento
    if (fb > 1 && strcmp(nome, raiz->esq->nome) < 0)
        return rotacaoLL(raiz);

    if (fb < -1 && strcmp(nome, raiz->dir->nome) > 0)
        return rotacaoRR(raiz);

    if (fb > 1 && strcmp(nome, raiz->esq->nome) > 0)
        return rotacaoLR(raiz);

    if (fb < -1 && strcmp(nome, raiz->dir->nome) < 0)
        return rotacaoRL(raiz);

    return raiz;
}

//encontra o nó com valor menor
NO *menorValor(NO *no) {
    NO *atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

//remove um usuário
NO *remover(NO *raiz, char nome[]) {
    if (raiz == NULL)
        return raiz;

    //busca o nó para remover
    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = remover(raiz->esq, nome);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = remover(raiz->dir, nome);
    else {
        //nó com zero ou um filho
        if (raiz->esq == NULL || raiz->dir == NULL) {
            NO *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        }
        //nó com dois filhos
        else {
            NO *temp = menorValor(raiz->dir);
            strcpy(raiz->nome, temp->nome);
            raiz->id = temp->id;
            strcpy(raiz->email, temp->email);
            raiz->dir = remover(raiz->dir, temp->nome);
        }
    }

    //árvore vazia depois de remover
    if (raiz == NULL)
        return raiz;

    //muda altura
    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;

    //rebalancea
    int fb = fatorBalanceamento(raiz);

    // LL
    if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0)
        return rotacaoLL(raiz);

    // LR
    if (fb > 1 && fatorBalanceamento(raiz->esq) < 0)
        return rotacaoLR(raiz);

    // RR
    if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0)
        return rotacaoRR(raiz);

    // RL
    if (fb < -1 && fatorBalanceamento(raiz->dir) > 0)
        return rotacaoRL(raiz);

    return raiz;
}

//busca o usuário pelo nome
NO *buscar(NO *raiz, char nome[]) {
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0)
        return raiz;

    if (strcmp(nome, raiz->nome) < 0)
        return buscar(raiz->esq, nome);
    else
        return buscar(raiz->dir, nome);
}

// imprime em ordem alfabética
void imprimirEmOrdem(NO *raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->esq);
        printf("Nome: %s | ID: %d | Email: %s\n", raiz->nome, raiz->id, raiz->email);
        imprimirEmOrdem(raiz->dir);
    }
}

int main() {
    NO *raiz = NULL;

    raiz = inserir(raiz, "Isabela", 1, "isabela@email.com");
    raiz = inserir(raiz, "David", 2, "david@email.com");
    raiz = inserir(raiz, "Pedro", 3, "pedro@email.com");

    printf("Árvore AVL em ordem:\n");
    imprimirEmOrdem(raiz);

    //remove um usuário
    raiz = remover(raiz, "Isabela");

    printf("\nÁrvore após remover Isabela:\n");
    imprimirEmOrdem(raiz);

    //buscar usuário
    NO *encontrado = buscar(raiz, "David");
    if (encontrado != NULL)
        printf("\nUsuário encontrado: %s | ID: %d | Email: %s\n", encontrado->nome, encontrado->id, encontrado->email);
    else
        printf("\nUsuário não encontrado.\n");

    return 0;
}
