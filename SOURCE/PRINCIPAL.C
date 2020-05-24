#include "XSV.H"
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
    char *pVetorDeColunasSelecionadas[] = {"DENOM_SOCIAL", "CD_CVM"};

    char pNomeDoArquivoDeInput[] = "/home/johnnyriver/Documents/Projetos/ProjetoDadosDaBolsa/CAD/CAD_CIA_ABERTA.CSV";

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

    for (int i = 0; i < 2; i++)
    {
        if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, pVetorDeColunasSelecionadas[i], NULL) != XSV_CondRetOK)
        {
            printf("Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
            exit(1);
        }
        printf("Adição de coluna para impressão na handle do arquivo XSV bem sucedida.\n");
    }

    if (XSV_AcrescentarColunaParaImpressaoAoHandle(pHandleXSV, "SETOR_ATIV", stringUpper) != XSV_CondRetOK)
    {
        printf("Problema na adição de coluna para impressão na handle do arquivo XSV.\n");
        exit(1);
    }
    printf("Adição de coluna para impressão na handle do arquivo XSV bem sucedida.\n");

    if (XSV_AcrescentarColunaCondicionalAoHandle(pHandleXSV, "SIT", (void*)"ATIVO", XSV_CondicaoIgual) != XSV_CondRetOK)
    {
        printf("Problema na adição de coluna condicional na handle do arquivo XSV.\n");
        exit(1);
    }
    printf("Adição de coluna condicional na handle do arquivo XSV bem sucedida.\n");

    if (XSV_TransformarListaDeColunasParaImpressaoEmConjunto(pHandleXSV) != XSV_CondRetOK)
    {
        printf("Problema na transformação da lista de colunas em conjunto de colunas.\n");
        exit(1);
    }
    printf("Transformação da lista de colunas em conjunto de colunas bem sucedida.\n");

    if (XSV_TransformarListaDeColunasCondicionaisEmConjunto(pHandleXSV) != XSV_CondRetOK)
    {
        printf("Problema na transformação da lista de colunas em conjunto de colunas.\n");
        exit(1);
    }
    printf("Transformação da lista de colunas em conjunto de colunas bem sucedida.\n");

    if (XSV_ImprimirDadosDeColunasSelecionadas(pHandleXSV) != XSV_CondRetOK)
    {
        printf("Problema na impressão dos dados de colunas selecionadas.\n");
        exit(1);
    }
    printf("Impressão  dos dados de colunas selecionadas bem sucedida.\n");

    if (XSV_DestruirHandleDeArquivoXSV(pHandleXSV) != XSV_CondRetOK)
    {
        printf("Problema na destruição da handle do arquivo XSV.\n");
        exit(1);
    }
    printf("Destruição da handle do arquivo XSV bem sucedida.\n");

    return 0;
}