#include "arvoreB.h"

void inicializaArvoreB(Pagina** arvore) {
    *arvore = NULL;
}

void desalocaArvoreB(Pagina **raiz)
{
    if(*raiz == NULL)
        return;

    for(int i = 0 ; i <= (*raiz)->tamanho ; i++)
        desalocaArvoreB(&(*raiz)->apontadores[i]);

    free(*raiz);
}


bool pesquisaArvore(Registro* x, Pagina* apontador) {
    long int i = 1;
    bool achou;

    if (apontador == NULL)
        return false;

    while (i < apontador->tamanho && x->chave > apontador->registros[i - 1].chave)
        i++;
    
    if (x->chave == apontador->registros[i - 1].chave)
    {
        *x = apontador->registros[i - 1];
        return true;
    }
    
    if (x->chave < apontador->registros[i - 1].chave)
        achou = pesquisaArvore(x, apontador->apontadores[i - 1]);
    
    else achou = pesquisaArvore(x, apontador->apontadores[i]);

    return achou;
}

void insereRegistros(FILE* arquivo, Pagina** arvore, unsigned int tamanho) {
    Registro item;

    inicializaArvoreB(arvore);

    for (int i = 0; i < tamanho; i++) {
        fseek(arquivo, i * sizeof(Registro), 0);
        fread(&item, sizeof(item), 1, arquivo);
        insere(item, arvore);
    }
}

void insere(Registro registro, Pagina** apontador) {
    bool cresceu;
    Registro registroRetorno;
    Pagina *apontadorRetorno, *apontadorTemporario;

    ins(registro, *apontador, &cresceu, &registroRetorno, &apontadorRetorno);

    if(cresceu) {
        apontadorTemporario = (Pagina *)malloc(sizeof(Pagina));
        apontadorTemporario->tamanho = 1;
        apontadorTemporario->registros[0] = registroRetorno;
        apontadorTemporario->apontadores[1] = apontadorRetorno;
        apontadorTemporario->apontadores[0] = *apontador;
        *apontador = apontadorTemporario;
    }
}


void ins(Registro registro, Pagina* apontador, bool *cresceu, Registro* registroRetorno, Pagina** apontadorRetorno) {
    int i = 1;
    Pagina* apTemp;

    if(apontador == NULL){
        (*cresceu) = true;
        *registroRetorno = registro;
        *apontadorRetorno = NULL;
        return;
    }

    while(i < apontador->tamanho && registro.chave > apontador->registros[i - 1].chave)
        i++;

    if(registro.chave == apontador->registros[i - 1].chave) {
        (*cresceu) = false;
        return;
    }

    if(registro.chave < apontador->registros[i - 1].chave)
        i--;

    ins(registro, apontador->apontadores[i], cresceu, registroRetorno, apontadorRetorno);

    if(!(*cresceu)) {
        return;
    }

    if(apontador->tamanho < MM) {
        insereNaPagina(apontador, *registroRetorno, *apontadorRetorno);
        (*cresceu) = false;
        return;
    }

    apTemp = (Pagina*)malloc(sizeof(Pagina));
    apTemp->tamanho = 0;
    apTemp->apontadores[0]=NULL;

    if(i < M+1) {
        insereNaPagina(apTemp, apontador->registros[MM - 1], apontador->apontadores[MM]);
        apontador->tamanho--;
        insereNaPagina(apontador, *registroRetorno, *apontadorRetorno);
    } else {
        insereNaPagina(apTemp, *registroRetorno, *apontadorRetorno);
    }

    for(int j = M+2; j <= MM; j++)
        insereNaPagina(apTemp, apontador->registros[j - 1], apontador->apontadores[j]);

    apontador->tamanho = M;
    apTemp->apontadores[0] = apontador->apontadores[M + 1];
    *registroRetorno = apontador->registros[M];
    *apontadorRetorno = apTemp;
}


void insereNaPagina(Pagina* apontador, Registro registro, Pagina* apontadorDireita) {
    bool naoEncontrou=true;
    int k;

    k = apontador->tamanho;
    naoEncontrou = (k > 0);

    while(naoEncontrou) {
        if(registro.chave >= apontador->registros[k - 1].chave) {
            naoEncontrou = false;
        } else {
            apontador->registros[k] = apontador->registros[k - 1];
            apontador->apontadores[k + 1] = apontador->apontadores[k];              
            k--;                                                                  
            
            if (k < 1)
                naoEncontrou = false;                                               
        }                                                                           
    }
    apontador->registros[k] = registro;
    apontador->apontadores[k + 1] = apontadorDireita;
    apontador->tamanho++;
}