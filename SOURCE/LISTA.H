#ifndef LISTA_H
#define LISTA_H
/****************************************************************************
*   $MCI Módulo de definição: Módulo Lista
*
*   Arquivo gerado:              LISTA.H
*   Letras identificadoras:      LIS
*
*   Projeto: Disciplina INF1301
*   Departamento: DI/PUC-Rio
*   Autores: avc - Antônio Chaves
*            jpp - João Pedro Paiva
*            pc	 - Pedro Costa
*
*   $HA Histórico de evolução:
*       Versão  Autor   Data        Observações
*       1.00    jpp     27/11/2019  Início do desenvolvimento
*       2.00    jpp     28/11/2019  Deturpador
*       3.00    jpp     29/11/2019  Verificador
*       4.00    jpp     24/05/2020  Função de inserir conteúdo no nó
*                                   corrente.
*
*   $ED Descrição do módulo
*       Este módulo implementa um conjunto de funções para criar e explorar
*       listas duplamente encadeadas.
*       A lista possui uma cabeça que contém uma referência para o primeiro
*       nó da lista e outra referência para um nó corrente da lista.
*       O módulo permite a existência de múltiplas listas simultâneamente.
*       Ao iniciar a execução do programa não existe lista.
*       A lista pode estar vazia.
*
****************************************************************************/

/******************* Declarações exportadas pelo módulo ********************/


/* Tipo referência para uma lista */
typedef struct LIS_tagLista *LIS_tppCabecaLista;


/****************************************************************************
*
*  $TC Tipo de dados: LIS Condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções da lista
*
****************************************************************************/
typedef enum
{

    LIS_CondRetOK, /* Concluiu corretamente */

    LIS_CondRetListaVazia, /* A lista não contém nós */

    LIS_CondRetNoNaoExiste, /* Tentou caminhar para um nó inexistente */

    LIS_CondRetNaoAchou, /* Não encontrou o valor procurado */

    LIS_CondRetFaltouMemoria /* Faltou memória ao tentar criar um nó */


#ifdef _DEBUG

    ,
    LIS_CondRetListaNaoExiste,
    /* Tentou utlizar uma lista cuja cabeça é nula */

    LIS_CondRetNoVazio, /* Tentou obter conteúdo de um nó vazio */

	LIS_CondRetTamDesigual, /* Tamanho não bate */

    LIS_CondRetFuncaoDeExclusaoNaoExiste,
    /* Tentou utlizar uma função de exclusão do conteúdo de um nó
    inexistente */

    LIS_CondRetConteudoNaoExiste,
    /* Tentou incluir um conteúdo nulo em um nó da lista */

    LIS_CondRetNoPossuiConteudo,
    /* Tentou inserir conteúdo em um nó da lista que já possuia conteúdo */

#endif

} LIS_tpCondRet;


/****************************************************************************
*
*	$FC Função:
*       LIS Criar a cabeça da lista.
*
*
*   $AE Assertivas de entrada esperadas:
*		Função de excluir valor não é nula.
*
*
*  $EP Parâmetros
*       $P ExcluirValor - O parâmetro que receberá o ponteiro para a função
*           de destruição do conteúdo de um nó da lista.
*           Este parâmetro é passado por valor.
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Cabeça da lista foi criada.
*       Primeiro nó e nó corrente são nulos.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetFaltouMemoria
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_CriarLista(void (*ExcluirValor)(void *pDado), LIS_tppCabecaLista *pCabecaDaLista);


/****************************************************************************
*
*	$FC Função:
*       LIS Destruir lista nó por nó e liberar cabeça da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Função independe da posição do nó corrente.
*       Cabeça da lista != NULL.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Lista foi destruída.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_DestruirLista(LIS_tppCabecaLista pCabecaDaLista);


/****************************************************************************
*
*	$FC Função:
*       LIS Inserir novo nó antes do nó corrente.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó posterior à posição onde desejamos
*       inserir um nó novo ou lista é vazia.
*		Cabeça da lista não é nula.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pConteudo - O parâmetro que receberá o ponteiro para o conteúdo do
*           nó da lista.
*           Este parâmetro é passado por valor.
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Um novo nó foi inserido antes do nó corrente ou nó novo é o único nó
*       da lista.
*       Nó corrente agora aponta para o nó novo.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetFaltouMemoria
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_InserirNoAntes(LIS_tppCabecaLista pCabecaDaLista, void *pConteudo);


/****************************************************************************
*
*	$FC Função:
*       LIS Inserir novo nó após nó corrente da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó anterior à posição onde desejamos
*       inserir um nó novo ou lista é vazia.
*		Cabeça da lista não é nula.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pConteudo - O parâmetro que receberá o ponteiro para o conteúdo do
*           nó da lista.
*           Este parâmetro é passado por valor.
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Um novo nó foi inserido após o corrente ou nó novo é o único nó da
*       lista.
*       Nó corrente agora aponta para o nó novo.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetFaltouMemoria
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_InserirNoApos(LIS_tppCabecaLista pCabecaDaLista, void *pConteudo);


/****************************************************************************
*
*	$FC Função:
*       LIS Excluir nó apontado pelo nó corrente da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Ponteiro corrente aponta para o nó que desejamos excluir da lista.
*       Conteúdo do nó corrente não é nulo.
*		Cabeça da lista não é nula.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente da lista foi excluído ou lista é vazia (nó corrente é
*       nulo).
*       Novo nó corrente é o nó anterior ao nó excluído, ou, caso o nó
*       excluído foi o primeiro nó da lista, o novo nó corrente é o nó
*       posterior ao nó excluído (atualmente o primeiro nó da lista).
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetListaVazia
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_ExcluirNo(LIS_tppCabecaLista pCabecaDaLista);

/****************************************************************************
*
*	$FC Função:
*       LIS Obter conteúdo do nó corrente da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó de onde deseja-se obter o conteúdo.
*       Conteúdo do nó corrente não é nulo.
*		Cabeça da lista não é nula.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P ppConteudo - O parâmetro que receberá o endereço do ponteiro para
*           o conteúdo do nó.
*           Este parâmetro é passado por referência.
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Conteudo do nó corrente da lista foi obtido.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetListaVazia;
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_ObterConteudo(LIS_tppCabecaLista pCabecaDaLista, void **ppConteudo);


/****************************************************************************
*
*	$FC Função:
*       LIS Inserir conteúdo no nó corrente da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó onde deseja-se inserir o conteúdo.
*       Conteúdo do nó corrente é nulo.
*		Cabeça da lista não é nula.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pConteudo - O parâmetro que receberá o conteúdo a ser inserido no
*           nó.
*           Este parâmetro é passado por valor.
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Conteudo foi inserido no nó corrente da lista.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetListaVazia;
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_InserirConteudo(LIS_tppCabecaLista pCabecaDaLista, void *pConteudo);


/****************************************************************************
*
*	$FC Função:
*       LIS Ir para nó seguinte.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó anterior ao nó desejado.
*		Cabeça da lista não é nula.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o nó desejado.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetNoNaoExiste
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_IrParaProximoNo(LIS_tppCabecaLista pCabecaDaLista);


/****************************************************************************
*
*	$FC Função:
*       LIS Ir para nó anterior.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó seguinte ao nó desejado.
*		Cabeça da lista não é nula.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o nó desejado.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetNoNaoExiste
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_IrParaNoAnterior(LIS_tppCabecaLista pCabecaDaLista);

/****************************************************************************
*
*	$FC Função:
*       LIS Ir para primeiro nó.
*
*
*	$AE Assertivas de entrada esperadas:
*		Cabeça da lista não é nula, mas pode ou não ser vazia.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o primeiro nó.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetListaVazia
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_IrParaPrimeiroNo(LIS_tppCabecaLista pCabecaDaLista);


#ifdef _DEBUG
/****************************************************************************
*
*	$FC Função:
*       LIS Deturpador.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó que queremos deturpar.
*		Cabeça da lista não é nula.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*       $P deturpacao - O parâmetro que receberá o caso de deturpação.
*           Este parâmetro é passado por valor.
*
*
*	$AS Assertivas de saída esperadas:
*       Estrutura foi deturpada no nó corrente.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_Deturpador(LIS_tppCabecaLista pCabecaDaLista, int deturpacao);


/****************************************************************************
*
*	$FC Função:
*       LIS Verificador.
*
*
*	$AE Assertivas de entrada esperadas:
*		Cabeça da lista não é nula.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       O número correto de deturpações foi detectado ou programa voou.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       numFalhasObservadas
*
****************************************************************************/
int LIS_Verificador(LIS_tppCabecaLista pCabecaDaLista);
#endif

#endif /* LISTA_H */
