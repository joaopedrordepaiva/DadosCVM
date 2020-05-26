#include "SOURCE/XSV.H"
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
    char *pVetorDeColunasSelecionadas[] = {"DT_FIM_EXERC", "DENOM_CIA", "CD_CVM", "CD_CONTA", "DS_CONTA", "VL_CONTA"};

    char pNomeDoArquivoDeInput[] = "ITR/BPA/ITR_CIA_ABERTA_BPA_CON_2019.CSV";

    if (XSV_CriarHandleDeArquivoXSV(&pHandleXSV) != XSV_CondRetOK)
    {
        printf("Problema na criação da handle do arquivo XSV.\n");
        exit(1);
    }
    printf("Criação da handle do arquivo XSV bem sucedida.\n");

    if (XSV_DefinirSeparadorDoInput(pHandleXSV, pSeparadorDeColunas) != XSV_CondRetOK)
    {

        printf("Problema na definição do separador na handle do arquivo XSV.\n");
        exit(1);
    }
    printf("Definição do separador na handle do arquivo XSV bem sucedida.\n");

    if (XSV_DefinirOpArquivoInput(pHandleXSV, pNomeDoArquivoDeInput) != XSV_CondRetOK)
    {
        printf("Problema na definição do arquivo de input da handle do arquivo XSV.\n");
        exit(1);
    }
    printf("Definição do arquivo de input da handle do arquivo XSV bem sucedida.\n");

    for (int i = 0; i < 6; i++)
    {
        if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, pVetorDeColunasSelecionadas[i], NULL) != XSV_CondRetOK)
        {
            printf("Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
            exit(1);
        }
        printf("Adição de coluna para impressão na handle do arquivo XSV bem sucedida.\n");
    }

    if (XSV_AcrescentarColunaCondicionalAoHandle(pHandleXSV, "DT_FIM_EXERC", (void*)"2019", XSV_CondicaoContem) != XSV_CondRetOK)
    {
        printf("Problema na adição de coluna condicional na handle do arquivo XSV.\n");
        exit(1);
    }
    printf("Adição de coluna condicional na handle do arquivo XSV bem sucedida.\n");

    if (XSV_ExecutarProcessamentoDoArquivoXSV(pHandleXSV) != XSV_CondRetOK)
    {
        printf("Problema no processamento do Handle de XSV.\n");
        exit(1);
    }
    printf("Processamento do Handle de XSV bem sucedido.\n");

    if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
    {
        printf("Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }
    printf("Destruição da handle do arquivo XSV bem sucedida.\n");

    return 0;
}