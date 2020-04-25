/****************************************************************************
*   $MCI Módulo de implementação: Módulo XSV
*
*   Arquivo gerado:              XSV.C
*   Letras identificadoras:      XSV
*
*   Projeto: Projeto XSV
*   Autores: jpp - João Pedro Paiva
*
*   $LIM Limites da implementação:
*       Separador de colunas é um único caractere.
*       Linhas dos arquivos de input e output tem, no máximo, 1022
*       caracteres.
*
*   $HA Histórico de evolução:
*       Versão  Autor   Data        Observações
*       1.00    jpp     18/04/2020  Início do desenvolvimento
*       2.00    jpp     20/04/2020  Funções parsing e impressão de colunas
*       2.00    jpp     24/04/2020  Nomes de variáveis, tranformações de 
*                                   colunas e adição desordenada de colunas
*
****************************************************************************/

#include "LISTA.H"
#include "XSV.H"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/********************* Variáveis encapuladas no módulo *********************/

typedef struct LIS_tagLista *XSV_tppListaDeColunasSelecionadas;

struct XSV_tagHandleXSV
{
    XSV_tppListaDeColunasSelecionadas pListaDeColunasSelecionadas;

    size_t vConjuntoDeColunasSelecionadas;

    char *pNomeDoArquivoDeInput;
    char *pNomeDoArquivoDeOutput;
    char *pSeparadorDeColunas;
};

typedef struct XSV_tpColunaSelecionada
{
    char *pNomeDaColunaSelecionada;
    void (*funcaoDeTransformacao)(char *);
} * XSV_tppColunaSelecionada;

/************** Protótipos das funções encapuladas no módulo ***************/

static XSV_tpCondRet XSV_CriarListaDeColunas(XSV_tppListaDeColunasSelecionadas *ppListaDeColunasSelecionadas);
static XSV_tpCondRet XSV_BuscaStringNaListaDeColunas(XSV_tppListaDeColunasSelecionadas pListaDeColunasSelecionadas, char pNomeDaColunaBuscada[], char *vResultadoDaBusca);

/**************** Código das funções exportadas pelo módulo ****************/

/****************************************************************************
*
*	$FC Função:
*       XSV Criar handle de arquivo XSV.
*
****************************************************************************/
XSV_tpCondRet XSV_CriarHandleDeArquivoXSV(XSV_tppHandleXSV *ppHandleXSV)
{

    *ppHandleXSV = (XSV_tppHandleXSV)malloc(sizeof(struct XSV_tagHandleXSV));

    if (!*ppHandleXSV)
        return XSV_CondRetFaltouMemoria;

    if (XSV_CriarListaDeColunas(&((*ppHandleXSV)->pListaDeColunasSelecionadas)) != XSV_CondRetOK)
        return XSV_CondRetProblemaDeLista;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Destruir handle de arquivo XSV.
*
****************************************************************************/
XSV_tpCondRet XSV_DestruirHandleDeArquivoXSV(XSV_tppHandleXSV pHandleXSV)
{

    if (LIS_DestruirLista(pHandleXSV->pListaDeColunasSelecionadas) != LIS_CondRetOK)
        return XSV_CondRetProblemaDeLista;

    free(pHandleXSV);

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Definir caractere que separa as colunas do arquivo de input.
*
****************************************************************************/
XSV_tpCondRet XSV_DefinirSeparadorDoInput(XSV_tppHandleXSV pHandleXSV, char *pSeparadorDeColunas)
{

    pHandleXSV->pSeparadorDeColunas = pSeparadorDeColunas;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Definir arquivo de tipo XSV de onde os dados serão extraídos.
*
****************************************************************************/
XSV_tpCondRet XSV_DefinirOpArquivoInput(XSV_tppHandleXSV pHandleXSV, char pNomeDoArquivoDeInput[])
{

    pHandleXSV->pNomeDoArquivoDeInput = pNomeDoArquivoDeInput;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Definir arquivo de tipo XSV para onde os dados serão gravados.
*
****************************************************************************/
XSV_tpCondRet XSV_DefinirOpArquivoOutput(XSV_tppHandleXSV pHandleXSV, char pNomeDoArquivoDeOutput[])
{

    pHandleXSV->pNomeDoArquivoDeOutput = pNomeDoArquivoDeOutput;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Armazenar nome e função de transformação de coluna em uma lista
*           de colunas selecionadas.
*
****************************************************************************/
XSV_tpCondRet XSV_AcrescentarColunaNaListaDeColunas(XSV_tppHandleXSV pHandleXSV, char pNomeDaColunaSelecionada[], void (*funcaoDeTransformacao)(char *))
{
    XSV_tppColunaSelecionada pColunaSelecionada;

    pColunaSelecionada = (XSV_tppColunaSelecionada)malloc(sizeof(struct XSV_tpColunaSelecionada));

    if (!pColunaSelecionada)
        return XSV_CondRetFaltouMemoria;

    pColunaSelecionada->pNomeDaColunaSelecionada = pNomeDaColunaSelecionada;
    pColunaSelecionada->funcaoDeTransformacao = funcaoDeTransformacao;

    if (LIS_InserirNoApos(pHandleXSV->pListaDeColunasSelecionadas, (void *)pColunaSelecionada) != LIS_CondRetOK)
        return XSV_CondRetProblemaDeLista;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Transformar lista de colunas selecionadas em conjunto de colunas
*           selcionadas.
*
****************************************************************************/
XSV_tpCondRet XSV_TransformarListaDeColunasEmConjunto(XSV_tppHandleXSV pHandleXSV)
{
    FILE *pArquivoDeInput;

    XSV_tppListaDeColunasSelecionadas pListaDeColunasSelecionadasInput, pListaDeColunasSelecionadasNova;

    XSV_tppColunaSelecionada pColunaSelecionada, pColunaSelecionadaNova;

    size_t vConjuntoDeColunasSelecionadas;

    XSV_tpCondRet vCondRetDeXSV;

    int vContadorDeColunas, vLenSeparador, i;

    char pLinhaDoArquivoDeInput[1024];

    char *pInicioColunaParaComparar, *pFinalColunaParaComparar, *pSeparadorDeColunas;

    char vCaractereAux, vEstaNaListaDeSelecionadas;

    if (XSV_CriarListaDeColunas(&pListaDeColunasSelecionadasNova) != XSV_CondRetOK)
        return XSV_CondRetProblemaDeLista;

    pListaDeColunasSelecionadasInput = pHandleXSV->pListaDeColunasSelecionadas;

    pSeparadorDeColunas = pHandleXSV->pSeparadorDeColunas;

    vConjuntoDeColunasSelecionadas = 0ULL;

    pArquivoDeInput = fopen(pHandleXSV->pNomeDoArquivoDeInput, "r");

    if (!pArquivoDeInput)
        return XSV_CondRetFalhaNaAberturaDoArquivo;

    if (!fgets(pLinhaDoArquivoDeInput, 1024, pArquivoDeInput))
        return XSV_FalhaAoBuscarAPrimeiraLinhaDoArquivoDeInput;

    fclose(pArquivoDeInput);

    vContadorDeColunas = 0;
    pInicioColunaParaComparar = pLinhaDoArquivoDeInput;
    pFinalColunaParaComparar = pLinhaDoArquivoDeInput;
    vLenSeparador = strlen(pSeparadorDeColunas);

    while ((pFinalColunaParaComparar = strstr(pFinalColunaParaComparar, pSeparadorDeColunas)))
    {

        *(pFinalColunaParaComparar) = '\0';

        vCondRetDeXSV = XSV_BuscaStringNaListaDeColunas(pListaDeColunasSelecionadasInput, pInicioColunaParaComparar, &vEstaNaListaDeSelecionadas);

        if (vCondRetDeXSV == XSV_CondRetListaDeColunasVazia)
            break;

        else if (vCondRetDeXSV != XSV_CondRetOK)
            return XSV_CondRetProblemaDeLista;

        if (vEstaNaListaDeSelecionadas)
        {

            vConjuntoDeColunasSelecionadas |= 1ULL << vContadorDeColunas;

            if (LIS_ObterConteudo(pListaDeColunasSelecionadasInput, (void **)&pColunaSelecionada) != LIS_CondRetOK)
                return XSV_CondRetProblemaDeLista;

            pColunaSelecionadaNova = (XSV_tppColunaSelecionada)malloc(sizeof(struct XSV_tpColunaSelecionada));

            if (!pColunaSelecionadaNova)
                return XSV_CondRetFaltouMemoria;

            pColunaSelecionadaNova->pNomeDaColunaSelecionada = pColunaSelecionada->pNomeDaColunaSelecionada;
            pColunaSelecionadaNova->funcaoDeTransformacao = pColunaSelecionada->funcaoDeTransformacao;

            if (LIS_InserirNoApos(pListaDeColunasSelecionadasNova, (void *)pColunaSelecionadaNova) != LIS_CondRetOK)
                return XSV_CondRetProblemaDeLista;

            if (LIS_ExcluirNo(pListaDeColunasSelecionadasInput) != LIS_CondRetOK)
                return XSV_CondRetProblemaDeLista;
        }

        *(pFinalColunaParaComparar) = pSeparadorDeColunas[0];
        vContadorDeColunas++;
        pFinalColunaParaComparar += vLenSeparador;
        pInicioColunaParaComparar = pFinalColunaParaComparar;
    }

    if (!pFinalColunaParaComparar)
    {
        for (i = 0; ' ' <= pInicioColunaParaComparar[i] && pInicioColunaParaComparar[i] <= '~'; i++)
            ;

        vCaractereAux = pInicioColunaParaComparar[i];
        pInicioColunaParaComparar[i] = '\0';

        vCondRetDeXSV = XSV_BuscaStringNaListaDeColunas(pListaDeColunasSelecionadasInput, pInicioColunaParaComparar, &vEstaNaListaDeSelecionadas);

        if (vCondRetDeXSV != XSV_CondRetOK)
            return XSV_CondRetProblemaDeLista;

        if (vCondRetDeXSV != XSV_CondRetListaDeColunasVazia && vEstaNaListaDeSelecionadas)
        {
            vConjuntoDeColunasSelecionadas |= 1ULL << vContadorDeColunas;

            if (LIS_ObterConteudo(pListaDeColunasSelecionadasInput, (void **)&pColunaSelecionada) != LIS_CondRetOK)
                return XSV_CondRetProblemaDeLista;

            pColunaSelecionadaNova = (XSV_tppColunaSelecionada)malloc(sizeof(struct XSV_tpColunaSelecionada));

            if (!pColunaSelecionadaNova)
                return XSV_CondRetFaltouMemoria;

            pColunaSelecionadaNova->pNomeDaColunaSelecionada = pColunaSelecionada->pNomeDaColunaSelecionada;
            pColunaSelecionadaNova->funcaoDeTransformacao = pColunaSelecionada->funcaoDeTransformacao;

            if (LIS_InserirNoApos(pListaDeColunasSelecionadasNova, (void *)pColunaSelecionadaNova) != LIS_CondRetOK)
                return XSV_CondRetProblemaDeLista;

            if (LIS_ExcluirNo(pListaDeColunasSelecionadasInput) != LIS_CondRetOK)
                return XSV_CondRetProblemaDeLista;
        }

        pInicioColunaParaComparar[i] = vCaractereAux;
    }

    if (LIS_DestruirLista(pListaDeColunasSelecionadasInput) != LIS_CondRetOK)
        return XSV_CondRetProblemaDeLista;

    pHandleXSV->pListaDeColunasSelecionadas = pListaDeColunasSelecionadasNova;

    pHandleXSV->vConjuntoDeColunasSelecionadas = vConjuntoDeColunasSelecionadas;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Imprimir dados das colunas selcionadas.
*
****************************************************************************/
XSV_tpCondRet XSV_ImprimirDadosDeColunasSelecionadas(XSV_tppHandleXSV pHandleXSV)
{
    FILE *pArquivoDeInput;

    XSV_tppListaDeColunasSelecionadas pListaDeColunasSelecionadas;

    XSV_tppColunaSelecionada pColunaSelecionada;

    size_t vConjuntoDeColunasSelecionadas;

    LIS_tpCondRet vCondRetDeLista;

    int vContadorDeColunas, vLenSeparador, i;

    char pLinhaDoArquivoDeInput[1024];

    char *pInicioColunaParaComparar, *pFinalColunaParaComparar, *pSeparadorDeColunas;

    char vCaractereAux;

    pArquivoDeInput = fopen(pHandleXSV->pNomeDoArquivoDeInput, "r");

    if (!pArquivoDeInput)
        return XSV_CondRetFalhaNaAberturaDoArquivo;

    pListaDeColunasSelecionadas = pHandleXSV->pListaDeColunasSelecionadas;

    if (LIS_IrParaPrimeiroNo(pListaDeColunasSelecionadas) != LIS_CondRetOK)
        return XSV_CondRetListaDeColunasVazia;

    vConjuntoDeColunasSelecionadas = pHandleXSV->vConjuntoDeColunasSelecionadas;
    pSeparadorDeColunas = pHandleXSV->pSeparadorDeColunas;
    vLenSeparador = strlen(pSeparadorDeColunas);
    vCaractereAux = 1;

    while (fgets(pLinhaDoArquivoDeInput, 1024, pArquivoDeInput))
    {

        vContadorDeColunas = 0;
        pInicioColunaParaComparar = pLinhaDoArquivoDeInput;
        pFinalColunaParaComparar = pLinhaDoArquivoDeInput;

        while ((pFinalColunaParaComparar = strstr(pFinalColunaParaComparar, pSeparadorDeColunas)))
        {

            if(!(vConjuntoDeColunasSelecionadas >> vContadorDeColunas))
                break;

            else if ((vConjuntoDeColunasSelecionadas >> vContadorDeColunas) & 1ULL)
            {

                *(pFinalColunaParaComparar) = '\0';

                if (LIS_ObterConteudo(pListaDeColunasSelecionadas, (void **)&pColunaSelecionada) != LIS_CondRetOK)
                    return XSV_CondRetProblemaDeLista;

                vCondRetDeLista = LIS_IrParaProximoNo(pListaDeColunasSelecionadas);

                if (vCondRetDeLista != LIS_CondRetOK && vCondRetDeLista != LIS_CondRetNoNaoExiste)
                    return XSV_CondRetProblemaDeLista;

                if (pColunaSelecionada->funcaoDeTransformacao)
                    pColunaSelecionada->funcaoDeTransformacao(pInicioColunaParaComparar);

                if(vCaractereAux)
                {
                    printf("%s", pInicioColunaParaComparar);
                    vCaractereAux = !vCaractereAux;
                }

                printf("%s%s", pSeparadorDeColunas, pInicioColunaParaComparar);

                *(pFinalColunaParaComparar) = pSeparadorDeColunas[0];
            }

            vContadorDeColunas++;
            pFinalColunaParaComparar += vLenSeparador;
            pInicioColunaParaComparar = pFinalColunaParaComparar;
        }

        if ((vConjuntoDeColunasSelecionadas >> vContadorDeColunas) & 1ULL)
        {
            for (i = 0; ' ' <= pInicioColunaParaComparar[i] && pInicioColunaParaComparar[i] <= '~'; i++)
                ;

            vCaractereAux = pInicioColunaParaComparar[i];
            pInicioColunaParaComparar[i] = '\0';

            if (LIS_ObterConteudo(pListaDeColunasSelecionadas, (void **)&pColunaSelecionada) != LIS_CondRetOK)
                return XSV_CondRetProblemaDeLista;

            if (pColunaSelecionada->funcaoDeTransformacao)
                pColunaSelecionada->funcaoDeTransformacao(pInicioColunaParaComparar);

            printf("%s%s", pSeparadorDeColunas, pInicioColunaParaComparar);

            pInicioColunaParaComparar[i] = vCaractereAux;
        }

        printf("\n");

        if (LIS_IrParaPrimeiroNo(pListaDeColunasSelecionadas) != LIS_CondRetOK)
            return XSV_CondRetListaDeColunasVazia;

        vCaractereAux = 1;
    }

    fclose(pArquivoDeInput);

    if (LIS_IrParaPrimeiroNo(pListaDeColunasSelecionadas) != LIS_CondRetOK)
        return XSV_CondRetListaDeColunasVazia;

    return XSV_CondRetOK;
}

/**************** Código das funções encapsuladas no módulo ****************/

/****************************************************************************
*
*	$FC Função:
*       XSV Criar lista de colunas selecionadas.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da lista de colunas selecionadas.
*
*
*  $EP Parâmetros
*       $P ppListaDeColunasSelecionadas - O parâmetro que receberá o endereço
*           do ponteiro para cabeça da lista de lista de colunas
*           selecionadas.
*           Este parâmetro é passado por referência.
*
****************************************************************************/
XSV_tpCondRet XSV_CriarListaDeColunas(XSV_tppListaDeColunasSelecionadas *ppListaDeColunasSelecionadas)
{
    if (LIS_CriarLista(free, ppListaDeColunasSelecionadas) != LIS_CondRetOK)
        return XSV_CondRetProblemaDeLista;

    return XSV_CondRetOK;
}
/****************************************************************************
*
*	$AS Assertivas de saída esperadas:
*       A lista de colunas selecionadas foi criada.
*		Valem as assertivas estruturais da lista de colunas selecionadas.
*
*
*   $FV Valor retornado
*       CID_CondRetOK - Condição de retorno de teste bem sucedido.
*       XSV_CondRetProblemaDeLista - Função de lista chamada não retornou
*           condição de retorno bem sucedida.
*
****************************************************************************/

/****************************************************************************
*
*	$FC Função:
*       XSV Busca string na lista de colunas selecionadas.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da lista de colunas selecionadas.
*       Nó corrente não precisa ser o primeiro nó.
*
*
*  $EP Parâmetros
*       $P ppListaDeColunasSelecionadas - O parâmetro que receberá o endereço
*           do ponteiro para cabeça da lista de lista de colunas
*           selecionadas.
*           Este parâmetro é passado por referência.
*       $P pNomeDaColunaBuscada - O parâmetro que receberá a string que
*           contém o nome da coluna buscada.
*           Este parâmetro é passado por valor.
*       $P vResultadoDaBusca - O parâmetro que receberá o endereço
*           da variável que contém a resposta se a busca encontrou o nome da
*           coluna buscada.
*           Este parâmetro é passado por referência.
*
****************************************************************************/
XSV_tpCondRet XSV_BuscaStringNaListaDeColunas(XSV_tppListaDeColunasSelecionadas pListaDeColunasSelecionadas, char pNomeDaColunaBuscada[], char *vResultadoDaBusca)
{

    XSV_tppColunaSelecionada pColunaSelecionada;

    *vResultadoDaBusca = 0;

    if (LIS_IrParaPrimeiroNo(pListaDeColunasSelecionadas) != LIS_CondRetOK)
        return XSV_CondRetListaDeColunasVazia;

    do
    {
        if (LIS_ObterConteudo(pListaDeColunasSelecionadas, (void **)&pColunaSelecionada) != LIS_CondRetOK)
            return XSV_CondRetProblemaDeLista;

        if (!strcmp(pNomeDaColunaBuscada, pColunaSelecionada->pNomeDaColunaSelecionada))
        {
            *vResultadoDaBusca = 1;
            return XSV_CondRetOK;
        }

    } while (LIS_IrParaProximoNo(pListaDeColunasSelecionadas) == LIS_CondRetOK);

    if (LIS_IrParaPrimeiroNo(pListaDeColunasSelecionadas) != LIS_CondRetOK)
        return XSV_CondRetProblemaDeLista;

    return XSV_CondRetOK;
}
/****************************************************************************
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente é o nó onde o valor foi encontrado. Se não foi
*       encontrado, nó corrente é o primeiro nó.
*       Função retornou corretamente se a coluna buscada estava na lista.
*		Valem as assertivas estruturais da lista de colunas selecionadas.
*
*
*   $FV Valor retornado
*       CID_CondRetOK - Condição de retorno de teste bem sucedido.
*       XSV_CondRetProblemaDeLista - Função de lista chamada não retornou
*           condição de retorno bem sucedida.
*
****************************************************************************/