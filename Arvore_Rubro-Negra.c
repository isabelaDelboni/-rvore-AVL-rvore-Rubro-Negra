#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição das cores
#define RED 1
#define BLACK 0

// Estrutura do produto
typedef struct Produto {
    int codigo;
    char nome[100];
    int quantidade;
    float preco;
} Produto;

// Estrutura do nó da Árvore Rubro-Negra
typedef struct Node {
    Produto produto;
    int cor;
    struct Node *esq, *dir, *pai;
} Node;

// Estrutura da Árvore Rubro-Negra
typedef struct {
    Node *raiz;
    Node *nulo;
} RBTree;

// Função para criar um novo nó
Node* novo_no(RBTree *arvore, Produto produto) {
    Node *no = (Node *)malloc(sizeof(Node));
    no->produto = produto;
    no->cor = RED;
    no->esq = arvore->nulo;
    no->dir = arvore->nulo;
    no->pai = arvore->nulo;
    return no;
}

// Função para criar a árvore
RBTree* criar_arvore() {
    RBTree *arvore = (RBTree *)malloc(sizeof(RBTree));
    arvore->nulo = (Node *)malloc(sizeof(Node));
    arvore->nulo->cor = BLACK;
    arvore->raiz = arvore->nulo;
    return arvore;
}

// Função para exibir o produto
void exibir_produto(Node *no) {
    printf("Código: %d\n", no->produto.codigo);
    printf("Nome: %s\n", no->produto.nome);
    printf("Quantidade: %d\n", no->produto.quantidade);
    printf("Preço: %.2f\n", no->produto.preco);
    printf("Cor do nó: %s\n", no->cor == RED ? "Vermelho" : "Preto");
    printf("-------------------------\n");
}

// Função para listar produtos
void listar_produtos(Node *no, Node *nulo) {
    if (no != nulo) {
        listar_produtos(no->esq, nulo);
        exibir_produto(no);
        listar_produtos(no->dir, nulo);
    }
}

// Função para buscar produto
Node* buscar_produto(RBTree *arvore, int codigo) {
    Node *atual = arvore->raiz;
    while (atual != arvore->nulo) {
        if (codigo == atual->produto.codigo)
            return atual;
        else if (codigo < atual->produto.codigo)
            atual = atual->esq;
        else
            atual = atual->dir;
    }
    return arvore->nulo;
}

// Rotações esquerda e direita
void rotacao_esquerda(RBTree *arvore, Node *x) {
    Node *y = x->dir;
    x->dir = y->esq;
    if (y->esq != arvore->nulo)
        y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == arvore->nulo)
        arvore->raiz = y;
    else if (x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;
    y->esq = x;
    x->pai = y;
}

void rotacao_direita(RBTree *arvore, Node *y) {
    Node *x = y->esq;
    y->esq = x->dir;
    if (x->dir != arvore->nulo)
        x->dir->pai = y;
    x->pai = y->pai;
    if (y->pai == arvore->nulo)
        arvore->raiz = x;
    else if (y == y->pai->dir)
        y->pai->dir = x;
    else
        y->pai->esq = x;
    x->dir = y;
    y->pai = x;
}

// Corrigir inserção para manter propriedades da RBTree
void corrigir_insercao(RBTree *arvore, Node *no) {
    while (no->pai->cor == RED) {
        if (no->pai == no->pai->pai->esq) {
            Node *tio = no->pai->pai->dir;
            if (tio->cor == RED) {
                no->pai->cor = BLACK;
                tio->cor = BLACK;
                no->pai->pai->cor = RED;
                no = no->pai->pai;
            } else {
                if (no == no->pai->dir) {
                    no = no->pai;
                    rotacao_esquerda(arvore, no);
                }
                no->pai->cor = BLACK;
                no->pai->pai->cor = RED;
                rotacao_direita(arvore, no->pai->pai);
            }
        } else {
            Node *tio = no->pai->pai->esq;
            if (tio->cor == RED) {
                no->pai->cor = BLACK;
                tio->cor = BLACK;
                no->pai->pai->cor = RED;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esq) {
                    no = no->pai;
                    rotacao_direita(arvore, no);
                }
                no->pai->cor = BLACK;
                no->pai->pai->cor = RED;
                rotacao_esquerda(arvore, no->pai->pai);
            }
        }
    }
    arvore->raiz->cor = BLACK;
}

// Inserir produto na árvore
void inserir_produto(RBTree *arvore, Produto produto) {
    Node *no = novo_no(arvore, produto);
    Node *pai = arvore->nulo;
    Node *atual = arvore->raiz;
    while (atual != arvore->nulo) {
        pai = atual;
        if (produto.codigo < atual->produto.codigo)
            atual = atual->esq;
        else if (produto.codigo > atual->produto.codigo)
            atual = atual->dir;
        else {
            printf("Erro: Código já existe na árvore.\n");
            free(no);
            return;
        }
    }
    no->pai = pai;
    if (pai == arvore->nulo)
        arvore->raiz = no;
    else if (produto.codigo < pai->produto.codigo)
        pai->esq = no;
    else
        pai->dir = no;
    corrigir_insercao(arvore, no);
    printf("Produto cadastrado com sucesso!\n");
}

// Funções para corrigir remoção
void corrigir_remocao(RBTree *arvore, Node *x) {
    while (x != arvore->raiz && x->cor == BLACK) {
        if (x == x->pai->esq) {
            Node *w = x->pai->dir;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                rotacao_esquerda(arvore, x->pai);
                w = x->pai->dir;
            }
            if (w->esq->cor == BLACK && w->dir->cor == BLACK) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->dir->cor == BLACK) {
                    w->esq->cor = BLACK;
                    w->cor = RED;
                    rotacao_direita(arvore, w);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->dir->cor = BLACK;
                rotacao_esquerda(arvore, x->pai);
                x = arvore->raiz;
            }
        } else {
            Node *w = x->pai->esq;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                rotacao_direita(arvore, x->pai);
                w = x->pai->esq;
            }
            if (w->dir->cor == BLACK && w->esq->cor == BLACK) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->esq->cor == BLACK) {
                    w->dir->cor = BLACK;
                    w->cor = RED;
                    rotacao_esquerda(arvore, w);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->esq->cor = BLACK;
                rotacao_direita(arvore, x->pai);
                x = arvore->raiz;
            }
        }
    }
    x->cor = BLACK;
}

// Função auxiliar para encontrar o menor nó da subárvore
Node* minimo(RBTree *arvore, Node *no) {
    while (no->esq != arvore->nulo)
        no = no->esq;
    return no;
}

// Transplante para remoção
void transplantar(RBTree *arvore, Node *u, Node *v) {
    if (u->pai == arvore->nulo)
        arvore->raiz = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else
        u->pai->dir = v;
    v->pai = u->pai;
}

// Remover produto da árvore
void remover_produto(RBTree *arvore, int codigo) {
    Node *z = buscar_produto(arvore, codigo);
    if (z == arvore->nulo) {
        printf("Produto com código %d não encontrado.\n", codigo);
        return;
    }

    Node *y = z;
    int cor_original = y->cor;
    Node *x;

    if (z->esq == arvore->nulo) {
        x = z->dir;
        transplantar(arvore, z, z->dir);
    } else if (z->dir == arvore->nulo) {
        x = z->esq;
        transplantar(arvore, z, z->esq);
    } else {
        y = minimo(arvore, z->dir);
        cor_original = y->cor;
        x = y->dir;
        if (y->pai == z)
            x->pai = y;
        else {
            transplantar(arvore, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        transplantar(arvore, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }
    free(z);
    if (cor_original == BLACK)
        corrigir_remocao(arvore, x);
    printf("Produto removido com sucesso!\n");
}

// Função para exibir o menu e interagir com o usuário
void menu(RBTree *arvore) {
    int opcao, codigo;
    Produto produto;
    Node *resultado;
    int proximo_codigo = 1;  

    while (1) {
        printf("\n--- Sistema de Inventário ---\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Remover Produto\n");
        printf("3. Buscar Produto\n");
        printf("4. Listar Produtos\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 
        switch (opcao) {
            case 1:
                produto.codigo = proximo_codigo++; 
                printf("Código do produto será: %d\n", produto.codigo);

                printf("Digite o nome do produto: ");
                fgets(produto.nome, sizeof(produto.nome), stdin);
                produto.nome[strcspn(produto.nome, "\n")] = '\0'; 

                printf("Digite a quantidade: ");
                scanf("%d", &produto.quantidade);

                printf("Digite o preço: ");
                scanf("%f", &produto.preco);

                inserir_produto(arvore, produto);
                break;

            case 2:
                printf("Digite o código do produto a remover: ");
                scanf("%d", &codigo);
                remover_produto(arvore, codigo);
                break;

            case 3:
                printf("Digite o código do produto a buscar: ");
                scanf("%d", &codigo);
                resultado = buscar_produto(arvore, codigo);
                if (resultado != arvore->nulo) {
                    printf("\nProduto encontrado:\n");
                    exibir_produto(resultado);
                } else {
                    printf("Produto não encontrado.\n");
                }
                break;

            case 4:
                printf("\nLista de produtos:\n");
                listar_produtos(arvore->raiz, arvore->nulo);
                break;

            case 5:
                printf("Saindo...\n");
                exit(0);

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// Função principal
int main() {
    RBTree *arvore = criar_arvore();
    menu(arvore);
    return 0;
}

