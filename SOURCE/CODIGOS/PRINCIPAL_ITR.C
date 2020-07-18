#include "SOURCE/XSV.H"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void stringRemoveZeros(char pString[])
{
    if (pString[0] != '0')
        return;

    for (int i = 1; pString[i]; i++)
        if(pString[i] != '0')
        {
            strcpy(pString,pString+i);
            return;
        }
}

int main(int argc, char **argv)
{
    XSV_tppHandleXSV pHandleXSV;

    char pSeparadorDeColunas[] = ";";
    char *pVetorDeColunasSelecionadas[] = {"CD_CONTA", "DS_CONTA", "VL_CONTA"};

    char pNomeDoArquivoDeInput[] = "ITR/DVA/ITR_CIA_ABERTA_DVA_CON_2019.CSV";

    if (XSV_CriarHandleDeArquivoXSV(&pHandleXSV) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na criação da handle do arquivo XSV.\n");
        exit(1);
    }
    fprintf(stderr, "Criação da handle do arquivo XSV bem sucedida.\n");

    if (XSV_DefinirSeparadorDoInput(pHandleXSV, pSeparadorDeColunas) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na definição do separador na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
        {
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
            exit(1);
        }
        fprintf(stderr, "Destruição da handle do arquivo XSV bem sucedida.\n");
        exit(1);
    }
    fprintf(stderr, "Definição do separador na handle do arquivo XSV bem sucedida.\n");

    if (XSV_DefinirOpArquivoInput(pHandleXSV, pNomeDoArquivoDeInput) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na definição do arquivo de input da handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
        {
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
            exit(1);
        }
        fprintf(stderr, "Destruição da handle do arquivo XSV bem sucedida.\n");
        exit(1);
    }
    fprintf(stderr, "Definição do arquivo de input da handle do arquivo XSV bem sucedida.\n");

    if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, "CD_CVM", stringRemoveZeros) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
        {
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
            exit(1);
        }
        fprintf(stderr, "Destruição da handle do arquivo XSV bem sucedida.\n");
        exit(1);
    }
    fprintf(stderr, "Adição de coluna para impressão na handle do arquivo XSV bem sucedida.\n");

    for (int i = 0; i < 3; i++)
    {
        if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, pVetorDeColunasSelecionadas[i], NULL) != XSV_CondRetOK)
        {
            fprintf(stderr, "Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
            if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            {
                fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
                exit(1);
            }
            fprintf(stderr, "Destruição da handle do arquivo XSV bem sucedida.\n");
            exit(1);
        }
        fprintf(stderr, "Adição de coluna para impressão na handle do arquivo XSV bem sucedida.\n");
    }

    if (XSV_AcrescentarColunaCondicionalAoHandle(pHandleXSV, "ORDEM_EXERC", (void *)"PENULTIMO", XSV_CondicaoIgual) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na adição de coluna condicional na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
        {
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
            exit(1);
        }
        fprintf(stderr, "Destruição da handle do arquivo XSV bem sucedida.\n");
        exit(1);
    }
    fprintf(stderr, "Adição de coluna condicional na handle do arquivo XSV bem sucedida.\n");

    if (XSV_ExecutarProcessamentoDoArquivoXSV(pHandleXSV) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema no processamento do Handle de XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
        {
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
            exit(1);
        }
        fprintf(stderr, "Destruição da handle do arquivo XSV bem sucedida.\n");
        exit(1);
    }
    fprintf(stderr, "Processamento do Handle de XSV bem sucedido.\n");

    if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }
    fprintf(stderr, "Destruição da handle do arquivo XSV bem sucedida.\n");

    return 0;
}