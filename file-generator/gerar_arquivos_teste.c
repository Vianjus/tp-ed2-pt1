#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "gerar_arquivos_teste.h"

void gerarArquivoTxt(FILE *arq)
{
    Registro reg;
    FILE *aux;

    aux = fopen("registros.txt", "w");

    rewind(arq);

    while(fread(&reg, sizeof(Registro), 1, arq) == 1)
        fprintf(aux, "%-10d\t%-15ld\t%s\n", reg.chave, reg.valor, reg.string);

    fclose(aux);
}

 void printB(FILE *arq)
 {
     Registro reg;

     rewind(arq);

     while(fread(&reg, sizeof(Registro), 1, arq) == 1)
         printf("%-10d\t%-15ld\t%s\n", reg.chave, reg.valor, reg.string);
}

Registro* alocarRegistros(long n)
{
    Registro *registros = (Registro*) malloc(n * sizeof(Registro));

    return registros;
}

void liberaRegistros(Registro** registros)
{
    free(*registros);
}

void gerarStringAleatoria(char titulo[TAM_TITULO])
{
    for(int i = 0 ; i < TAM_TITULO - 1 ; i++)
        titulo[i] = caracteres_possiveis[rand() % qtde_caracteres_possiveis];

    titulo[TAM_TITULO - 1] = '\0';
}

void preencheArquivoBinario(FILE* arq, Registro *registros, int n, char* ordenacao)
{
    int i, j;

    if(!strcmp(ordenacao, "asc"))
    {
        for(i = 0 ; i < n ; i++)
        {
            if(i != 0 && i % TAM_REGISTRO == 0)
                fwrite(registros, sizeof(Registro), TAM_REGISTRO, arq);

            registros[i % TAM_REGISTRO].chave = i + 1;
            registros[i % TAM_REGISTRO].valor = (long int)rand() << sizeof (long int);
            gerarStringAleatoria(registros[i % TAM_REGISTRO].string);
        }

        fwrite(registros, sizeof(Registro), i % TAM_REGISTRO, arq);
    }
    else if(!strcmp(ordenacao, "desc"))
    {
        j = 0;

        for(i = n ; i > 0 ; i--)
        {
            if(j != 0 && j % TAM_REGISTRO == 0)
                fwrite(registros, sizeof(Registro), TAM_REGISTRO, arq);

            registros[j % TAM_REGISTRO].chave = i;
            registros[j % TAM_REGISTRO].valor = (long int)rand() << sizeof (long int);
            gerarStringAleatoria(registros[j % TAM_REGISTRO].string);

            j++;
        }

        fwrite(registros, sizeof(Registro), j % TAM_REGISTRO, arq);
    }
    else
    {
        bool *esta_em_uso = (bool*) malloc(n * sizeof(bool));

        if(esta_em_uso == NULL)
            return;

        memset(esta_em_uso, false, n * sizeof(bool));

        for(i = 0 ; i < n ; i++)
        {
            int posicao_chave;

            if(i != 0 && i % TAM_REGISTRO == 0)
                fwrite(registros, sizeof(Registro), TAM_REGISTRO, arq);

            posicao_chave = rand() % n;

            while(esta_em_uso[posicao_chave] == true)
            {
                posicao_chave++;

                if(posicao_chave % n == 0)
                    posicao_chave = 0;
            }

            registros[i % TAM_REGISTRO].chave = posicao_chave + 1;
            registros[i % TAM_REGISTRO].valor = (long int)rand() << sizeof (long int);
            gerarStringAleatoria(registros[i % TAM_REGISTRO].string);

            esta_em_uso[posicao_chave] = true;
        }

        free(esta_em_uso);

        fwrite(registros, sizeof(Registro), i % TAM_REGISTRO, arq);
    }

}

bool verificaEntrada(int argc, char** argv, int* quantidade_linhas, char* ordenacao)
{
    if(argc != 3 && argc != 4)
    {
        printf("Utilizacao errada do programa! Execute de acordo:\n\n");
        printf("\"%s <quantidade de linhas para o arquivo gerado> <asc|desc|random> [B | b]\"\n", argv[0]);
        return false;
    } 
    
    if(atoi(argv[1]) <= 0)
    {
        printf("O primeiro argumento deve ser um valor numerico valido!\n");
        return false;
    }
    else
        *quantidade_linhas = atoi(argv[1]);

    strcpy(ordenacao, argv[2]);

    if(strcmp(ordenacao, "asc") && strcmp(ordenacao, "desc") && strcmp(ordenacao, "random"))
    {
        printf("O segundo argumento deve ser um dos seguintes: <asc|desc|random>!\n");
        return false;
    }

    if(argc == 4 && tolower(argv[3][0]) != 'b')
    {
        printf("O terceiro argumento deve ser o valor 'b' ou 'B'.\n");
        return false;
    }

    return true;
}

int main(int argc, char** argv)
{
    int quantidade_linhas;
    char ordenacao[7];
    FILE *arquivo_binario;
    Registro *registros;

    if(!verificaEntrada(argc, argv, &quantidade_linhas, ordenacao))
        return 1;

    if((registros = alocarRegistros(TAM_REGISTRO)) == NULL)
    {
        printf("Erro na alocacao de registros!\n");
        return 1;
    }

    if((arquivo_binario = fopen("registros_teste.bin", "w+b")) == NULL)
    {
        printf("Erro na abertura do arquivo binario!\n");
        return 1;
    }

    srand(time(NULL));

    if(argc == 4)
        printf("Later.\n");
    else
        preencheArquivoBinario(arquivo_binario, registros, quantidade_linhas, ordenacao);

    printB(arquivo_binario);

    gerarArquivoTxt(arquivo_binario);

    liberaRegistros(&registros);

    fclose(arquivo_binario);

    printf("\n[] Arquivo criado com sucesso!");

    return 0;
}