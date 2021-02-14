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
*       2.00    jpp     23/05/2020  Enumeração de condição e funções
*                                   condicionais. Remoção das funções de
*                                   arquivo de output.
*       3.00    jpp     25/05/2020  Função de execução do processamento no
*                                   no lugar das de de transformação em
*                                   conjunto e impressão. Documentação de
*                                   funções anteriores e da função nova.
*       4.00    jpp     19/07/2020  Troca de char* para const char* em certas
*                                   funções para poder passar strings
*                                   literais nas chamadas.
*
*   $ED Descrição do módulo
*       Este módulo implementa um conjunto de funções para operar arquivos
*       do tipo <Algo> Separated Values, como CSVs (Comma Separated Value) e
*       TSVs (Tab Separated Values).
*
****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cassert>
#include <list>
#include <bitset>
#include <string.h>
#include "LISTA.h"

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
    execução bem sucedida quando deveria */

    XSV_CondRetListaDeColunasNaoCriada, /* Tentou transformar uma lista de
    colunas não inicializada em conjunto */

    XSV_CondRetListaDeColunasVazia, /* Tentou transformar uma lista de
    colunas vazia em conjunto */

    XSV_CondRetFalhaNaAberturaDoArquivo, /* Abertura de arquivo de input 
    falhou */

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
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV não foi inicializada.
*
*
*  $EP Parâmetros
*       $P ppHandleXSV - O parâmetro que receberá o endereço do Handle de
*           arquivo XSV.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi inicializada.
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de execução bem sucedida.
*       XSV_CondRetFaltouMemoria - Não houve memória para alocação dinâmica
*           da Handle.
*
****************************************************************************/
XSV_tpCondRet XSV_CriarHandleDeArquivoXSV(XSV_tppHandleXSV *ppHandleXSV);

/****************************************************************************
*
*	$FC Função:
*       XSV Destruir handle de arquivo XSV.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi inicializada (não é nula).
*
*
*  $EP Parâmetros
*       $P pHandleXSV - O parâmetro que receberá a Handle de arquivo XSV.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi destruído (nós, listas e handle liberados).
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de execução bem sucedida.
*       XSV_CondRetProblemaDeLista - Chamada de função de lista não retornou
*           condição de execução bem sucedida.
*
****************************************************************************/
XSV_tpCondRet XSV_DestruirHandleDeArquivoXSV(XSV_tppHandleXSV pHandleXSV);

/****************************************************************************
*
*	$FC Função:
*       XSV Definir string que separa as colunas do arquivo de input.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi inicializada (não é nula).
*       Separador pode já ter sido definido (será redefinido com esta
*       função).
*
*
*  $EP Parâmetros
*       $P pHandleXSV - O parâmetro que receberá a Handle de arquivo XSV.
*           Este parâmetro é passado por referência.
*       $P pSeparadorDeColunas - O parâmetro que receberá a string que separa
*           as colunas do arquivo XSV.
*           Este parâmetro é passado por valor.
*
*
*	$AS Assertivas de saída esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Separador foi definido (ou redefinido).
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de execução bem sucedida.
*
****************************************************************************/
XSV_tpCondRet XSV_DefinirSeparadorDoInput(XSV_tppHandleXSV pHandleXSV, std::string pSeparadorDeColunas);

/****************************************************************************
*
*	$FC Função:
*       XSV Definir arquivo de tipo XSV de onde os dados serão extraídos.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi inicializada (não é nula).
*       Arquivo de input pode já ter sido definido (será redefinido com esta
*       função).
*
*
*  $EP Parâmetros
*       $P pHandleXSV - O parâmetro que receberá a Handle de arquivo XSV.
*           Este parâmetro é passado por referência.
*       $P pCaminhoRelativoParaArquivoDeInput - O parâmetro que receberá a
*           string que descreve o caminho para o arquivo XSV que servirá como
*           input.
*           Este parâmetro é passado por valor.
*
*
*	$AS Assertivas de saída esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Arquivo de input foi definido (ou redefinido).
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de execução bem sucedida.
*
****************************************************************************/
XSV_tpCondRet XSV_DefinirOpArquivoInput(XSV_tppHandleXSV pHandleXSV, std::string pCaminhoParaArquivoDeInput);

/****************************************************************************
*
*	$FC Função:
*       XSV A impressão de uma linha específica do arquivo XSV dependerá do
*           valor de suas colunas condicionais. Se a condição da coluna não
*           for obedecida pelo valor contido na linha, a linha não será
*           impressa.
*           Armazenar nome, conteúdo e condição de coluna à lista
*           de colunas condicionais da HandleXSV.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi inicializada (não é nula).
*       Se o nome da coluna não pertencer às colunas do arquivo, esta coluna
*       condicional será ignorada.
*       Colunas repetidas também serão ignoradas durante o processamento da
*       Handle.
*       A lista de colunas condicionais pode estar vazia quando processar o
*       arquivo XSV.
*
*
*  $EP Parâmetros
*       $P pHandleXSV - O parâmetro que receberá a Handle de arquivo XSV.
*           Este parâmetro é passado por referência.
*       $P pNomeDaColunaCondicional - O parâmetro que receberá a string que
*           descreve o nome da coluna condicional.
*           Este parâmetro é passado por valor.
*       $P pObjetoCondicional - O parâmetro que receberá a string à qual os
*           valores da coluna serão comparados ou a função que define se os
*           valores da coluna obedecem à condição.
*           Este parâmetro é passado por referência.
*       $P vCondicaoDaColuna - O parâmetro que receberá a condição que
*           descreve a comparação a ser feita entre os valores da coluna e o
*           objeto condicional.
*           Este parâmetro é passado por valor.
*
*
*	$AS Assertivas de saída esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Coluna condicional e seus atributos foram adicionados ao HandleXSV.
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de execução bem sucedida.
*       XSV_CondRetFaltouMemoria - Não houve memória para alocação dinâmica
*           da Handle.
*       XSV_CondRetProblemaDeLista - Chamada de função de lista não retornou
*           condição de execução bem sucedida.
*
****************************************************************************/
XSV_tpCondRet XSV_AcrescentarColunaCondicionalAoHandle(XSV_tppHandleXSV pHandleXSV, std::string pNomeDaColunaCondicional, void *pObjetoCondicional, XSV_tpCondicaoDeColuna vCondicaoDaColuna);

/****************************************************************************
*
*	$FC Função:
*       XSV Somente as colunas presentes na lista de colunas para impressão
*           serão impressas durante o processamento do arquivo XSV.
*           Armazenar nome e função de transformação de coluna à lista
*           de colunas para impressão da HandleXSV.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi inicializada (não é nula).
*       Se a coluna não pertencer às colunas do arquivo, esta coluna
*       será ignorada durante o processamento da Handle.
*       Colunas repetidas também serão ignoradas durante o processamento da
*       Handle.
*       Se a função de tranformação for nula (valor NULL), então os valores
*       da coluna serão impressos da forma que estão no arquivo de input.
*       A lista de colunas para impressão não deve estar vazia quando
*       processar o arquivo XSV.
*
*
*  $EP Parâmetros
*       $P pHandleXSV - O parâmetro que receberá a Handle de arquivo XSV.
*           Este parâmetro é passado por referência.
*       $P pNomeDaColunaParaImpressao - O parâmetro que receberá a string que
*           descreve o nome da coluna para impressão.
*           Este parâmetro é passado por valor.
*       $P funcaoDeTransformacao - O parâmetro que receberá a função que
*           tranforma os valores da coluna para impressão.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Coluna para impressão e seus atributos foram adicionados ao
*       HandleXSV.
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de execução bem sucedida.
*       XSV_CondRetFaltouMemoria - Não houve memória para alocação dinâmica
*           da Handle.
*       XSV_CondRetProblemaDeLista - Chamada de função de lista não retornou
*           condição de execução bem sucedida.
*
****************************************************************************/
XSV_tpCondRet XSV_AcrescentarColunaParaImpressaoAoHandle(XSV_tppHandleXSV pHandleXSV, std::string pNomeDaColunaParaImpressao, void (*funcaoDeTransformacao)(char *));

/****************************************************************************
*
*	$FC Função:
*       XSV Processar o arquivo XSV de acordo com o que o usuário definiu
*           para a Handle. Resultado das operações (impressão das linhas) é
*           colocado em stdout.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi inicializada (não é nula).
*       A lista de colunas para impressão não deve estar vazia.
*       A lista de colunas condicionais pode estar vazia.
*       Arquivo XSV de input e separador de colunas já devem ter sido
*       definidos.
*
*
*  $EP Parâmetros
*       $P pHandleXSV - O parâmetro que receberá a Handle de arquivo XSV.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Arquivo XSV correspondente à Handle foi processado (linhas do arquivo
*       foram impressas de acordo com o que o usuário definiu).
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de execução bem sucedida.
*       XSV_CondRetFaltouMemoria - Não houve memória para alocação dinâmica
*           da Handle.
*       XSV_CondRetProblemaDeLista - Chamada de função de lista não retornou
*           condição de execução bem sucedida.
*       XSV_FalhaAoBuscarAPrimeiraLinhaDoArquivoDeInput - Busca da primeira
*           linha do aqruivo de input falhou.
*       XSV_CondRetFalhaNaAberturaDoArquivo - Abertura de arquivo de input 
*           falhou.
*       XSV_CondRetListaDeColunasNaoCriada - Tentou transformar uma lista de
*           colunas não inicializada em conjunto.
*       XSV_CondRetListaDeColunasVazia - Tentou transformar uma lista de
*           colunas vazia em conjunto.
*
****************************************************************************/
XSV_tpCondRet XSV_ExecutarProcessamentoDoArquivoXSV(XSV_tppHandleXSV pHandleXSV);


#endif /* XSV_H */