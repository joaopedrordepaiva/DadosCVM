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
typedef enum
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
*       XSV Definir a string que separa as colunas do arquivo de input.
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
*       XSV Definir arquivo de tipo XSV para onde os dados serão gravados.
*
****************************************************************************/
XSV_tpCondRet XSV_DefinirOpArquivoOutput(XSV_tppHandleXSV pHandleXSV, char pNomeDoArquivoDeOutput[]);

/****************************************************************************
*
*	$FC Função:
*       XSV Armazenar nome e função de transformação de coluna em uma lista
*           de colunas selecionadas.
*
****************************************************************************/
XSV_tpCondRet XSV_AcrescentarColunaNaListaDeColunas(XSV_tppHandleXSV pHandleXSV, char pNomeDaColunaSelecionada[], void(*funcaoDeTransformacao)(char*));

/****************************************************************************
*
*	$FC Função:
*       XSV Transformar lista de colunas selecionadas em conjunto de colunas
*           selcionadas.
*
****************************************************************************/
XSV_tpCondRet XSV_TransformarListaDeColunasEmConjunto(XSV_tppHandleXSV pHandleXSV);

/****************************************************************************
*
*	$FC Função:
*       XSV Imprimir dados das colunas selcionadas.
*
****************************************************************************/
XSV_tpCondRet XSV_ImprimirDadosDeColunasSelecionadas(XSV_tppHandleXSV pHandleXSV);



#endif /* XSV_H */