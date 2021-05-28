#include "../lib/XSV.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void stringUpper(char pString[])
{
    int i, j;

    i = j = 0;

    while (pString[j])
    {
        if (pString[j] != '/' && pString[j] != '.')
        {
            pString[i] = toupper(pString[j]);
            i++;
        }

        j++;
    }
    pString[i] = pString[j];
    return;
}

void stringRemoveCaracteresEspeciais(char pString[])
{
    int i, j;

    i = j = 0;

    while (pString[j])
    {
        if (pString[j] != '/' && pString[j] != '.')
        {
            pString[i] = pString[j];
            i++;
        }

        j++;
    }
    pString[i] = pString[j];
    return;
}

int main(int argc, char **argv)
{
    XSV_tppHandleXSV pHandleXSV;

    if (XSV_CriarHandleDeArquivoXSV(&pHandleXSV) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na criação da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_DefinirSeparadorDoInput(pHandleXSV, ";") != XSV_CondRetOK)
    {

        fprintf(stderr, "Problema na definição do separador na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_DefinirOpArquivoInput(pHandleXSV, "InformacoesContabeis/CAD/CAD.csv") != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na definição do arquivo de input da handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, "CD_CVM", NULL) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, "SETOR_ATIV", NULL) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, "DENOM_SOCIAL", NULL) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_AcrescentarColunaCondicionalAoHandle(pHandleXSV, "SIT", (void *)"ATIVO", XSV_CondicaoIgual) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na adição de coluna condicional na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_AcrescentarColunaCondicionalAoHandle(pHandleXSV, "TP_MERC", (void *)"BOLSA", XSV_CondicaoIgual) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na adição de coluna condicional na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_AcrescentarColunaCondicionalAoHandle(pHandleXSV, "CATEG_REG", (void *)"Categoria A", XSV_CondicaoIgual) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na adição de coluna condicional na handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    if (XSV_AcrescentarColunaCondicionalAoHandle(pHandleXSV, "SIT_EMISSOR", (void *)"FASE OPERACIONAL", XSV_CondicaoIgual) != XSV_CondRetOK)
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