#ifndef  __SEQUENCIAL__
#define __SEQUENCIAL__

#include <stdio.h>
#include <stdlib.h>
#include "sequencial.h"
#include "utilitarios.h"

void pesquisaIndexada(int chave, FILE *arq_bin)
{
    FILE *index_bin;
    if (index_bin = fopen(tabind.bin, "rb") == NULL)
    {
        int criaTabelaIndeces(Index *indexes, FILE *arq_bin)
        FILE *file = fopen(tabind.bin, "wb");

        if (file == NULL)
        {
            printf("[-] Erro ao abrir o arquivo\n");
            return 0;
        }

    }
}

Registro *alocarPagina(int qtde_registros_arquivo)
{
    Registro *registros = (Registro *) malloc(qtde_registros_arquivo * sizeof(Registro));

    return registros;
}

int calculaItensPorPagina(int qtde_registros_arquivo)
{
    return (qtde_registros_arquivo / MAX_TABELA) + 1;
}

void mergesort(int* v, int l, int r){
    if(l<r){
        int m = (l + r)/2;
        mergesort(v,l,m);
        mergesort(v, m+1, r);
        //merge(v,l,m,r);
    }
}

int sequencial(Registro tab[], int qtde_registros_arquivo, Entrada *item, FILE *arq)
{ //
    int ITENSPAGINA;
    int i, quant_itens;
    long desloc;
    Registro pagina;

    ITENSPAGINA = calculaItensPorPagina(qtde_registros_arquivo);


    i = 0;
    while (i < qtde_registros_arquivo && tab[i].chave <= item->chave_buscada)
        i++;

    if (i == 0)
        return 0;
    else
    {
        if (i < qtde_registros_arquivo)
            quant_itens = ITENSPAGINA;
        else
        {
            fseek(arq, 0, SEEK_END);
            quant_itens = (ftell(arq) / sizeof(Entrada)) % ITENSPAGINA;
            if (quant_itens == 0)
                quant_itens = ITENSPAGINA;
        }

        desloc = (tab[i-1].chave-1) * ITENSPAGINA * sizeof(Entrada);
        fseek (arq, desloc, SEEK_SET);
        fread (&pagina, sizeof(Entrada), quant_itens, arq);


    }
}

int criaTabelaIndeces (Index *indexes, FILE *dataFile)
{
  FILE *file = fopen(INDEXES_FILE, "wb");

  if (file == NULL)
  {
    printf("[-] Erro ao abrir o arquivo\n");
    return 0;
  }

  Data item;

  // int count = 0;
  int pos = 0;

  while (fread(&item, sizeof(Data), 1, dataFile) == 1)
  {
    indexes[pos].key = item.key;
    indexes[pos].pos = pos + 1;
    fwrite(&indexes[pos], sizeof(Index), 1, file);
    pos++;
    fseek(dataFile, sizeof(Data) * (PAGE_ITEMS - 1), SEEK_CUR);

    clear();
    printf("[+] %d índices criados.\n", pos);
  }

  printf("[+] Tabela de índices criada com sucesso!\n");
  fclose(file);
  rewind(dataFile);
  return pos;
}
#endif