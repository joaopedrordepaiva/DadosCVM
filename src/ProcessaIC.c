#include "../lib/XSV.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

void stringRemoveZeros(char pString[])
{
    int i;

    for (i = 0; pString[i] == '0'; i++)
        ;

    if (i)
        strcpy(pString, pString + i);

    return;
}

int main(int argc, char **argv)
{
    XSV_tppHandleXSV pHandleXSV;

    const char pSeparadorDeColunas[] = ";";
    const char *pVetorDeColunasSelecionadas[] = {"DT_INI_EXERC", "DT_FIM_EXERC", "COLUNA_DF", "CD_CONTA", "DS_CONTA", "VL_CONTA"};

    if (argc != 2)
    {
        fprintf(stderr, "Utilização incorreta. O argumento da chamada deve ser o caminho para o arquivo a ser processado.\n");
        exit(1);
    }

    if (XSV_CriarHandleDeArquivoXSV(&pHandleXSV) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na criação da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_DefinirSeparadorDoInput(pHandleXSV, pSeparadorDeColunas) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na definição do separador na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_DefinirOpArquivoInput(pHandleXSV, argv[1]) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na definição do arquivo de input da handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, "CD_CVM", stringRemoveZeros) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    for (int i = 0; i < 6; i++)
        if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, pVetorDeColunasSelecionadas[i], NULL) != XSV_CondRetOK)
        {
            fprintf(stderr, "Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
            if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
                fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
            exit(1);
        }

    if (XSV_AcrescentarColunaCondicionalAoHandle(pHandleXSV, "ORDEM_EXERC", (void *)"PENULTIMO", XSV_CondicaoIgual) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na adição de coluna condicional na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_ExecutarProcessamentoDoArquivoXSV(pHandleXSV) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema no processamento do Handle de XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    return 0;
}