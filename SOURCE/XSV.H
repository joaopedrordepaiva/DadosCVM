#ifndef XSV_H
#define XSV_H
/****************************************************************************
*   $MCI Módulo de definição: Módulo XSV
*
*   Arquivo gerado:              XSV.H
*   Letras identificadoras:      XSV
*
*   Projeto: Projeto XSV
*   Autores: jpp - João Pedro Paiva
*
*   $HA Histórico de evolução:
*       Versão  Autor   Data        Observações
*       1.00    jpp     18/04/2020  Início do desenvolvimento
*       2.00    jpp     24/05/2020  Enumeração de condição e funções
*                                   condicionais. Remoção das funções de
*                                   arquivo de output.
*
*
*   $ED Descrição do módulo
*       Este módulo implementa um conjunto de funções para operar arquivos
*       do tipo <Algo> Separated Values, como CSVs e TSVs.
*
****************************************************************************/

/******************* Declarações exportadas pelo módulo ********************/

/* Tipo referência para uma handle de arquivo XSV */
typedef struct XSV_tagHandleXSV *XSV_tppHandleXSV;

/****************************************************************************
*
*  $TC Tipo de dados: XSV Condições de retorno.
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções de XSV.
*
****************************************************************************/
typedef enum XSV_enumCondRet
{

    XSV_CondRetOK, /* Concluiu corretamente */

    XSV_CondRetFaltouMemoria, /* Faltou memória para allocar */

    XSV_CondRetProblemaDeLista, /* Função de LIS não retornou condição de
    teste bem sucedido quando deveria */

    XSV_CondRetListaDeColunasNaoCriada, /* Tentou transformar uma lista de
    colunas não inicializada em conjunto */

    XSV_CondRetListaDeColunasVazia, /* Tentou transformar uma lista de
    colunas vazia em conjunto */

    XSV_CondRetFalhaNaAberturaDoArquivo, /* Abertura de arquivo de input ou
    output falhou */

    XSV_FalhaAoBuscarAPrimeiraLinhaDoArquivoDeInput /* Busca da primeira
    linha do aqruivo de input falhou */

} XSV_tpCondRet;

typedef enum XSV_enumCondicaoDeColuna
{

    XSV_CondicaoIgual,
    /* Determinar se o valor da coluna é igual ao parâmetro */

    XSV_CondicaoDiferente,
    /* Determinar se o valor da coluna não é igual ao parâmetro */

    XSV_CondicaoContem,
    /* Determinar se o valor da coluna contém o parâmetro */

    XSV_CondicaoNaoContem,
    /* Determinar se o valor da coluna não contém o parâmetro */

    XSV_CondicaoPorFuncao
    /* Função passada como parâmetro determinará se valor da coluna obedece
    ou não à condição. A função deve retornar 0 se o valor não obedece à
    condição e um valor diferente de 0 caso contrário. */

} XSV_tpCondicaoDeColuna;

/****************************************************************************
*
*	$FC Função:
*       XSV Criar handle de arquivo XSV.
*
****************************************************************************/
XSV_tpCondRet XSV_CriarHandleDeArquivoXSV(XSV_tppHandleXSV *ppHandleXSV);

/****************************************************************************
*
*	$FC Função:
*       XSV Destruir handle de arquivo XSV.
*
****************************************************************************/
XSV_tpCondRet XSV_DestruirHandleDeArquivoXSV(XSV_tppHandleXSV pHandleXSV);

/****************************************************************************
*
*	$FC Função:
*       XSV Definir caractere que separa as colunas do arquivo de input.
*
****************************************************************************/
XSV_tpCondRet XSV_DefinirSeparadorDoInput(XSV_tppHandleXSV pHandleXSV, char *pSeparadorDeColunas);

/****************************************************************************
*
*	$FC Função:
*       XSV Definir arquivo de tipo XSV de onde os dados serão extraídos.
*
****************************************************************************/
XSV_tpCondRet XSV_DefinirOpArquivoInput(XSV_tppHandleXSV pHandleXSV, char pNomeDoArquivoDeInput[]);

/****************************************************************************
*
*	$FC Função:
*       XSV Armazenar nome, conteúdo e condição de coluna à lista
*           de colunas condicionais do HandleXSV.
*
****************************************************************************/
XSV_tpCondRet XSV_AcrescentarColunaCondicionalAoHandle(XSV_tppHandleXSV pHandleXSV, char pNomeDaColunaCondicional[], void *pParametroCondicao, XSV_tpCondicaoDeColuna vCondicaoDaColuna);

/****************************************************************************
*
*	$FC Função:
*       XSV Armazenar nome e função de transformação de coluna à lista
*           de colunas para impressão do HandleXSV.
*
****************************************************************************/
XSV_tpCondRet XSV_AcrescentarColunaParaImpressaoAoHandle(XSV_tppHandleXSV pHandleXSV, char pNomeDaColunaParaImpressao[], void (*funcaoDeTransformacao)(char *));

/****************************************************************************
*
*	$FC Função:
*       XSV Transformar lista de colunas condicionais em conjunto de colunas
*           condicionais.
*
****************************************************************************/
XSV_tpCondRet XSV_TransformarListaDeColunasCondicionaisEmConjunto(XSV_tppHandleXSV pHandleXSV);

/****************************************************************************
*
*	$FC Função:
*       XSV Transformar lista de colunas para impressão em conjunto de
*           colunas para impressão.
*
****************************************************************************/
XSV_tpCondRet XSV_TransformarListaDeColunasParaImpressaoEmConjunto(XSV_tppHandleXSV pHandleXSV);

/****************************************************************************
*
*	$FC Função:
*       XSV Imprimir dados das colunas selcionadas.
*
****************************************************************************/
XSV_tpCondRet XSV_ImprimirDadosDeColunasSelecionadas(XSV_tppHandleXSV pHandleXSV);


#endif /* XSV_H */