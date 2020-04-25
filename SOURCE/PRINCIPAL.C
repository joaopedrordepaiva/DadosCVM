#include "XSV.H"
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>

// #define SYS_IS_LINUX

// #ifdef SYS_IS_LINUX
// #include <termios.h>

// static struct termios old, current;

// /* Initialize new terminal i/o settings */
// void initTermios(int echo)
// {
//     tcgetattr(0, &old);         /* grab old terminal i/o settings */
//     current = old;              /* make new settings same as old settings */
//     current.c_lflag &= ~ICANON; /* disable buffered i/o */
//     if (echo)
//     {
//         current.c_lflag |= ECHO; /* set echo mode */
//     }
//     else
//     {
//         current.c_lflag &= ~ECHO; /* set no echo mode */
//     }
//     tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
// }

// /* Restore old terminal i/o settings */
// void resetTermios(void)
// {
//     tcsetattr(0, TCSANOW, &old);
// }

// /* Read 1 character - echo defines echo mode */
// char getch(void)
// {
//     char ch;
//     initTermios(0);
//     ch = getchar();
//     resetTermios();
//     return ch;
// }
// #endif

// #ifdef SYS_IS_WIN
// #include <conio.h>
// #endif

void stringUpper(char pString[])
{
    for (int i = 0; pString[i]; i++)
        pString[i] = towupper((wint_t)pString[i]);

    return;
}

int main(int argc, char **argv)
{

    XSV_tppHandleXSV pHandleXSV;

    char pSeparadorDeColunas[] = ";";
    char *pVetorDeColunasSelecionadas[] = {"DENOM_SOCIAL", "SIT", "CD_CVM"};

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

    for (int i = 0; i < 3; i++)
    {
        if (XSV_AcrescentarColunaNaListaDeColunas(pHandleXSV, pVetorDeColunasSelecionadas[i], NULL) != XSV_CondRetOK)
        {
            printf("Problema na adição de coluna na handle do arquivo XSV.\n");
            exit(1);
        }
        printf("Adição de coluna na handle do arquivo XSV bem sucedida.\n");
    }

    if (XSV_AcrescentarColunaNaListaDeColunas(pHandleXSV, "SETOR_ATIV", stringUpper) != XSV_CondRetOK)
    {
        printf("Problema na adição de coluna na handle do arquivo XSV.\n");
        exit(1);
    }
    printf("Adição de coluna na handle do arquivo XSV bem sucedida.\n");

    if (XSV_TransformarListaDeColunasEmConjunto(pHandleXSV) != XSV_CondRetOK)
    {
        printf("Problema na transformação da lista de colunas em conjunto de colunas.\n");
        exit(1);
    }
    printf("Transoformação da lista de colunas em conjunto de colunas bem sucedida.\n");

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