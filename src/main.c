#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int valor;
    int altura;
    struct no *esquerda, *direita;
} ArvNo;

ArvNo *CriarArvNo(int num) {
    ArvNo *novo = (ArvNo*)malloc(sizeof(ArvNo));
    if(novo != NULL) {
        novo->valor = num;
        novo->esquerda = NULL;
        novo->direita = NULL;
        novo->altura = 0;
    } else {
        printf("Erro ao alocar memoria!");
    }
    return novo;
}

int Maior(int a, int b) {
    return (a > b) ? a : b;
}

int Altura(ArvNo *no) {
    if(no == NULL) {
        return -1;
    } else {
        return no->altura;
    }
}

int FatorDeBalanceamento(ArvNo *no) {
    if( no != NULL ){
        return (Altura(no->esquerda)) - Altura(no->direita);
    } else {
        return 0;
    }
}

ArvNo *RotacaoEsquerda(ArvNo *raiz) {
    ArvNo *novaRaiz, *filho;

    novaRaiz = raiz->direita;
    filho = novaRaiz->esquerda;

    novaRaiz->esquerda = raiz;
    raiz->direita = filho;

    raiz->altura = Maior(Altura(raiz->esquerda), Altura(raiz->direita)) + 1;
    novaRaiz->altura = Maior(Altura(novaRaiz->esquerda), Altura(novaRaiz->direita)) + 1;

    return novaRaiz;
}

ArvNo *RotacaoDireita(ArvNo *raiz) {
    ArvNo *novaRaiz, *filho;

    novaRaiz = raiz->esquerda;
    filho = novaRaiz->direita;

    novaRaiz->direita = raiz;
    raiz->esquerda = filho;
    
    raiz->altura = Maior(Altura(raiz->esquerda), Altura(raiz->direita)) + 1;
    novaRaiz->altura = Maior(Altura(novaRaiz->esquerda), Altura(novaRaiz->direita)) + 1;

    return novaRaiz;
}

ArvNo *RotacaoDireitaEsquerda(ArvNo *raiz) {
    raiz->direita = RotacaoDireita(raiz->direita);
    return RotacaoEsquerda(raiz);
}

ArvNo *RotacaoEsquerdaDireita(ArvNo *raiz) {
    raiz->esquerda = RotacaoEsquerda(raiz->esquerda);
    return RotacaoDireita(raiz);
}

ArvNo *Balancear(ArvNo *raiz) {

    int fb = FatorDeBalanceamento(raiz);

    if(fb < -1 && FatorDeBalanceamento(raiz->direita) <= 0) {
        raiz = RotacaoEsquerda(raiz);
    } else if ( fb > 1 && FatorDeBalanceamento(raiz->esquerda) >= 0) {
        raiz = RotacaoDireita(raiz);
    } else if(fb > 1 && FatorDeBalanceamento(raiz->esquerda) < 0) {
        raiz = RotacaoEsquerdaDireita(raiz);
    } else if(fb < -1 && FatorDeBalanceamento(raiz->direita) > 0) {
        raiz = RotacaoDireitaEsquerda(raiz);
    }

    return raiz;
}

ArvNo *Inserir(ArvNo *raiz, int num) {
    if(raiz == NULL) {
        return CriarArvNo(num);
    } else {
        if(num < raiz->valor) {
            raiz->esquerda = Inserir(raiz->esquerda, num);
        } else if(num > raiz->valor) {
            raiz->direita = Inserir(raiz->direita, num);
        } else {
            printf("\nO elemento ja esta na arvore!");
        }
    }

    raiz->altura = Maior(Altura(raiz->direita), Altura(raiz->esquerda)) + 1;

    raiz = Balancear(raiz);

    return raiz;
}

ArvNo *Remover(ArvNo *raiz, int num) {
    if(raiz == NULL) {
        printf("Valor nao encontrado!\n");
    } else {
        if(raiz->valor == num) {
            if(raiz->esquerda == NULL && raiz->direita == NULL) { //remove nós folhas (sem filhos)
                free(raiz); 
                return NULL;
            } else {    
                if(raiz->esquerda != NULL && raiz->direita != NULL) { //remove nós com 2 filhos
                    ArvNo *aux = raiz->esquerda;
                    while(aux->direita != NULL) {
                        aux = aux->direita;
                    }
                    raiz->valor = aux->valor;
                    aux->valor = num;           //troca de lugar o nó que eu achei com o nó mais a direita da esquerda dele
                    raiz->esquerda= Remover(raiz->esquerda, num);
                    return raiz;
                } else { //remove nós que possuem apenas 1 filho
                    ArvNo *aux;
                    if(raiz->esquerda != NULL) {
                        aux = raiz->esquerda;
                    } else {
                        aux = raiz->direita;
                    }
                    free(raiz);
                    return aux;
                }
            }
        } else {
            if(num < raiz->valor) {
                raiz->esquerda = Remover(raiz->esquerda, num);
            } else {
                raiz->direita = Remover(raiz->direita, num);
            }
        
        raiz->altura = Maior(Altura(raiz->direita), Altura(raiz->esquerda)) + 1;
        raiz = Balancear(raiz);
    }
    return raiz;
  }
}

void Imprimir(ArvNo *raiz, int nivel) {
    int i;
    if( raiz != NULL ) {

        Imprimir(raiz->direita, nivel + 1);
        printf("\n\n");

        for(i = 0; i < nivel; i++) {
            printf("\t");
        }

        printf("%d", raiz->valor);
        Imprimir(raiz->esquerda, nivel + 1);
    }
}

int main() {
    int sel = -1, num;
    ArvNo *raiz = NULL;

    do {
        printf("\n1 - Inserir\n2 - Remover\n3 - Imprimir\n");
        scanf("%d", &sel);

        switch(sel) {
            case 1: 
                printf("Digite o numero a ser inserido: ");
                scanf("%d", &num);
                raiz = Inserir(raiz, num);
                break;
            case 2:
                printf("Digite o numero a ser removido: ");
                scanf("%d", &num);
                raiz = Remover(raiz, num);
                break;
            case 3:
                Imprimir(raiz, 0);
                break;
        }

    } while( sel != 0 );

    return 0;
}
