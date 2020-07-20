#include "../MODULOS/XSV.H"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void stringUpper(char pString[])
{
    for (int i = 0; pString[i]; i++)
        pString[i] = toupper((unsigned char)pString[i]);

    return;
}

int main(int argc, char **argv)
{
    XSV_tppHandleXSV pHandleXSV;

    const char pSeparadorDeColunas[] = ";";
    const char *pVetorDeColunasSelecionadas[] = {"CD_CVM", "DENOM_SOCIAL"};

    const char pNomeDoArquivoDeInput[] = "INFORMACOES_CONTABEIS/CAD/CAD_CIA_ABERTA.CSV";

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

    if (XSV_DefinirOpArquivoInput(pHandleXSV, pNomeDoArquivoDeInput) != XSV_CondRetOK)
    {
        fprintf(stderr, "Problema na definição do arquivo de input da handle do arquivo XSV.\n");
        if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
            fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }

    for (int i = 0; i < 2; i++)
        if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, pVetorDeColunasSelecionadas[i], NULL) != XSV_CondRetOK)
        {
            fprintf(stderr, "Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
            if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
                fprintf(stderr, "Problema na destruição da handle do arquivo XSV.\n");
            exit(1);
        }

    if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, "SETOR_ATIV", stringUpper) != XSV_CondRetOK)
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