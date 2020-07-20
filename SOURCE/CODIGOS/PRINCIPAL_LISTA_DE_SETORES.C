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

    char pSeparadorDeColunas[] = ";";

    char pNomeDoArquivoDeInput[] = "../CAD.CSV";

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

    if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, "SETOR_ATIV", NULL) != XSV_CondRetOK)
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