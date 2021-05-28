/****************************************************************************
*   $MCI Módulo de implementação: Módulo Lista
*
*   Arquivo gerado:              LISTA.C
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
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "LISTA.h"

/****************************************************************************
*
*   $TC Tipo de dados:
*       LIS Descritor do nó da lista.
*
*
*   $ED Descrição do tipo:
*       Descreve a organização do nó.
*
****************************************************************************/
typedef struct LIS_tagNoLista
{

    void *pConteudo;
    /* Ponteiro para conteúdo do nó */

    struct LIS_tagNoLista *pNoAnterior;
    /* Ponteiro para o nó predecessor */

    struct LIS_tagNoLista *pNoProximo;
    /* Ponteiro para o nó sucessor */

#ifdef _DEBUG
    LIS_tppCabecaLista pCabeca; /* Ponteiro para a cabeca da estrutura */
    char tipoEstrutura;         /* Campo que assinala o tipo da estrutura */
    int tamNo;                  /* Tamanho em bytes de cada nó da lista */
    int tamLista;
    /* Tamanho em bytes da lista (soma dos tamanhos dos nós) */
#endif

} LIS_tpNoLista;

/****************************************************************************
*
*   $TC Tipo de dados:
*       LIS Descritor da cabeça de uma lista.
*
*
*   $ED Descrição do tipo:
*       A cabeça da lista é o ponto de acesso para uma determinada lista.
*       Por intermédio da referência para o nó corrente e dos ponteiros
*       adjascentes pode-se navegar a lista sem necessitar de uma pilha.
*
****************************************************************************/
typedef struct LIS_tagLista
{

    LIS_tpNoLista *pNoPrimeiro;
    /* Ponteiro para o primeiro nó da lista */

#ifdef _DEBUG
    LIS_tpNoLista *pNoUltimo;
    /* Ponteiro para o último nó da lista */
#endif

    LIS_tpNoLista *pNoCorrente;
    /* Ponteiro para o nó corrente da lista */

    void (*ExcluirValor)(void *pConteudo);
    /* Ponteiro para a função de destruição do valor contido em um nó */

#ifdef _DEBUG
    char tipoEstrutura; /* Campo que assinala o tipo da estrutura */
    int numNos;         /* Número de nós da lista */
    int tamNo;          /* Tamanho em bytes de cada nó da lista */
    int tamLista;
    /* Tamanho em bytes da lista (soma dos tamanhos dos nós) */
#endif

} LIS_tpCabecaLista;

/********************* Variáveis encapuladas no módulo *********************/

/************** Protôtipos das funções encapuladas no módulo ***************/

static LIS_tpCondRet LIS_LiberarNo(LIS_tppCabecaLista pCabecaDaLista, LIS_tpNoLista *pNo);

static LIS_tpNoLista *LIS_CriarNo(void *pConteudo
#ifdef _DEBUG
                                 ,
                                 LIS_tppCabecaLista pCabecaDaLista
#endif
);

/**************** Código das funções exportadas pelo módulo ****************/

/****************************************************************************
*
*	$FC Função:
*       LIS Criar a cabeça da lista.
*
****************************************************************************/
LIS_tpCondRet LIS_CriarLista(void (*ExcluirValor)(void *pDado), LIS_tppCabecaLista *pCabecaDaLista)
{

#ifdef _DEBUG
    if (!ExcluirValor)
        return LIS_CondRetFuncaoDeExclusaoNaoExiste;
#endif

    *pCabecaDaLista = (LIS_tpCabecaLista *)malloc(sizeof(LIS_tpCabecaLista));

    if (!pCabecaDaLista)
        return LIS_CondRetFaltouMemoria;

    (*pCabecaDaLista)->pNoPrimeiro = NULL;
    (*pCabecaDaLista)->pNoCorrente = NULL;

#ifdef _DEBUG
    (*pCabecaDaLista)->tipoEstrutura = 'l';
    (*pCabecaDaLista)->numNos = 0;
    (*pCabecaDaLista)->tamLista = 0;
    (*pCabecaDaLista)->tamNo = sizeof(LIS_tpNoLista);
    (*pCabecaDaLista)->pNoUltimo = NULL;
#endif

    (*pCabecaDaLista)->ExcluirValor = ExcluirValor;

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/****************************************************************************
*
*	$FC Função:
*       LIS Destruir lista nó por nó e liberar cabeça da lista.
*
****************************************************************************/
LIS_tpCondRet LIS_DestruirLista(LIS_tppCabecaLista pCabecaDaLista)
{
    LIS_tpCondRet condRetDeLista;
    LIS_tpNoLista *pNo;
    LIS_tpNoLista *pNoProximo;

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    pNo = pCabecaDaLista->pNoPrimeiro;
    while (pNo)
    {
        pNoProximo = pNo->pNoProximo;
        condRetDeLista = LIS_LiberarNo(pCabecaDaLista, pNo);
        if (condRetDeLista != LIS_CondRetOK)
            return condRetDeLista;
        pNo = pNoProximo;
    }

    pCabecaDaLista->pNoPrimeiro = NULL;

#ifdef _DEBUG
    pCabecaDaLista->pNoUltimo = NULL;
#endif

    pCabecaDaLista->pNoCorrente = NULL;

    free(pCabecaDaLista);

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/****************************************************************************
*
*	$FC Função:
*       LIS Insere novo nó antes do nó corrente.
*
****************************************************************************/
LIS_tpCondRet LIS_InserirNoAntes(LIS_tppCabecaLista pCabecaDaLista, void *pConteudo)
{

    LIS_tpNoLista *pNo;

#ifdef _DEBUG
    if (!pConteudo)
        return LIS_CondRetConteudoNaoExiste;
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    /* Criar nó a inerir antes */

    pNo = LIS_CriarNo(pConteudo
#ifdef _DEBUG
                     ,
                     pCabecaDaLista
#endif
    );

    if (!pNo)
        return LIS_CondRetFaltouMemoria;

    /* Encadear o nó antes do nó corrente */

    if (!pCabecaDaLista->pNoCorrente) /* Lista não possui nós */
    {
        pCabecaDaLista->pNoPrimeiro = pNo; /* Nó novo é o primeiro nó */
#ifdef _DEBUG
        pCabecaDaLista->pNoUltimo = pNo;
#endif
    }

    else /* Lista possui nós. Vamos inserir antes do nó corrente */
    {
        if (pCabecaDaLista->pNoCorrente->pNoAnterior)
        /* Nó corrente possui anterior */
        {
            pNo->pNoAnterior = pCabecaDaLista->pNoCorrente->pNoAnterior;
            /* Nó anterior do nó novo é o anterior do corrente */
            pCabecaDaLista->pNoCorrente->pNoAnterior->pNoProximo = pNo;
            /* Nó posterior ao anterior do corrente é o novo nó */
        }
        else /* Nó corrente é o primeiro nó */
            pCabecaDaLista->pNoPrimeiro = pNo;
        /* Primeiro nó da lista é o nó novo */

        pNo->pNoProximo = pCabecaDaLista->pNoCorrente;
        /* Nó posterior ao nó novo é o corrente */
        pCabecaDaLista->pNoCorrente->pNoAnterior = pNo;
        /* Nó anterior ao corrente é o nó novo */
    }

    pCabecaDaLista->pNoCorrente = pNo;
    /* Nó corrente é o nó novo */

#ifdef _DEBUG
    if (pCabecaDaLista->tamLista != pNo->tamLista)
        return LIS_CondRetTamDesigual;
#endif

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/****************************************************************************
*
*	$FC Função:
*       LIS Insere novo nó após nó corrente da lista.
*
****************************************************************************/
LIS_tpCondRet LIS_InserirNoApos(LIS_tppCabecaLista pCabecaDaLista, void *pConteudo)
{
    LIS_tpNoLista *pNo;

#ifdef _DEBUG
    if (!pConteudo)
        return LIS_CondRetConteudoNaoExiste;
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    /* Criar nó a inerir antes */

    pNo = LIS_CriarNo(pConteudo
#ifdef _DEBUG
                     ,
                     pCabecaDaLista
#endif
    );

    if (!pNo)
        return LIS_CondRetFaltouMemoria;

    /* Encadear o nó antes do nó corrente */

    if (!pCabecaDaLista->pNoCorrente) /* Lista não possui nós */
    {
        pCabecaDaLista->pNoPrimeiro = pNo; /* Nó novo é o primeiro nó */
#ifdef _DEBUG
        pCabecaDaLista->pNoUltimo = pNo;
#endif
    }

    else /* Lista possui nós. Vamos inserir depois do nó corrente */
    {

        if (pCabecaDaLista->pNoCorrente->pNoProximo)
        /* Nó corrente possui sucessor */
        {
            pNo->pNoProximo = pCabecaDaLista->pNoCorrente->pNoProximo;
            /* Nó sucessor do nó novo é o sucessor do corrente */
            pCabecaDaLista->pNoCorrente->pNoProximo->pNoAnterior = pNo;
            /* Nó anterior do sucessor do corrente é o novo nó */
        }

#ifdef _DEBUG
        else /* Nó corrente é o último nó */
            pCabecaDaLista->pNoUltimo = pNo;
/* Último nó da lista é o nó novo */
#endif

        pNo->pNoAnterior = pCabecaDaLista->pNoCorrente;
        /* Nó anterior ao nó novo é o corrente */
        pCabecaDaLista->pNoCorrente->pNoProximo = pNo;
        /* Nó sucessor do corrente é o nó novo */
    }

    pCabecaDaLista->pNoCorrente = pNo;
    /* Nó corrente é o nó novo */

#ifdef _DEBUG
    if (pCabecaDaLista->tamLista != pNo->tamLista)
        return LIS_CondRetTamDesigual;
#endif

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/****************************************************************************
*
*	$FC Função:
*       LIS Exclui nó apontado pelo nó corrente da lista.
*
****************************************************************************/
LIS_tpCondRet LIS_ExcluirNo(LIS_tppCabecaLista pCabecaDaLista)
{

    LIS_tpNoLista *pNo;

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    if (!pCabecaDaLista->pNoCorrente) /* Lista não possui nós */
        return LIS_CondRetListaVazia; /* Retorna condição de lista vazia */

    pNo = pCabecaDaLista->pNoCorrente;

    /* Desencadeia à esquerda */

    if (pNo->pNoAnterior) /* Nó a ser excluído possui anterior */
    {
        pNo->pNoAnterior->pNoProximo = pNo->pNoProximo;
        /* Conecta nó anterior ao nó posterior do nó a ser excluído */
        pCabecaDaLista->pNoCorrente = pNo->pNoAnterior;
        /* Nó corrente passa a ser o nó anterior do que será excluído */
    }
    else /* Queremos excluir o primeiro nó da lista */
    {
        pCabecaDaLista->pNoCorrente = pNo->pNoProximo;
        /* Nó corrente passa a ser o nó posterior ao que será excluído */
        pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoCorrente;
        /* Primeiro nó da lista passa a ser o nó posterior ao que será 
        excluído */
    }

    /* Desencadeia à direita */

    if (pNo->pNoProximo) /* Nó a ser excluído não é o último nó da lista */
        pNo->pNoProximo->pNoAnterior = pNo->pNoAnterior;
    /* Conecta nó posterior ao nó anterior do nó a ser excluído */

    return LIS_LiberarNo(pCabecaDaLista, pNo);
    /* Retorna condição de teste bem sucedido ou condição de erro */
}

/****************************************************************************
*
*	$FC Função:
*       LIS Inserir conteúdo no nó corrente da lista.
*
****************************************************************************/
LIS_tpCondRet LIS_InserirConteudo(LIS_tppCabecaLista pCabecaDaLista, void *pConteudo)
{

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
    if (pCabecaDaLista->pNoCorrente->pConteudo)
        /* Nó corrente não possui conteúdo */
        return LIS_CondRetNoPossuiConteudo;
        /* Retorna condição de falha na obtenção do conteúdo do nó corrente */
#endif

    if (!pCabecaDaLista->pNoCorrente)
        /* Não há nó corrente */
        return LIS_CondRetListaVazia;
        /* Retorna condição de lista vazia */

    pCabecaDaLista->pNoCorrente->pConteudo = pConteudo;
    /* Insere conteúdo no nó corrente */

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/****************************************************************************
*
*	$FC Função:
*       LIS Obter conteúdo do nó corrente da lista.
*
****************************************************************************/
LIS_tpCondRet LIS_ObterConteudo(LIS_tppCabecaLista pCabecaDaLista, void **ppConteudo)
{

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
    if (!pCabecaDaLista->pNoCorrente->pConteudo)
        /* Nó corrente não possui conteúdo */
        return LIS_CondRetNoVazio;
        /* Retorna condição de falha na obtenção do conteúdo do nó corrente */
#endif

    if (!pCabecaDaLista->pNoCorrente)
        /* Não há nó corrente */
        return LIS_CondRetListaVazia;
        /* Retorna condição de lista vazia */

    *ppConteudo = pCabecaDaLista->pNoCorrente->pConteudo;
    /* Passa, por referência, o ponteiro do conteúdo do nó corrente */

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/****************************************************************************
*
*	$FC Função:
*       LIS Ir para nó seguinte.
*
****************************************************************************/
LIS_tpCondRet LIS_IrParaProximoNo(LIS_tppCabecaLista pCabecaDaLista)
{

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    if (!pCabecaDaLista->pNoCorrente->pNoProximo)
        /* Sucessor do nó corrente não existe (é nulo) */
        return LIS_CondRetNoNaoExiste;
        /* Retorna condição de nó inexistente */

    pCabecaDaLista->pNoCorrente = pCabecaDaLista->pNoCorrente->pNoProximo;
    /* Nó corrente caminha para frente */

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/****************************************************************************
*
*	$FC Função:
*       LIS Ir para nó anterior.
*
****************************************************************************/
LIS_tpCondRet LIS_IrParaNoAnterior(LIS_tppCabecaLista pCabecaDaLista)
{

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    if (!pCabecaDaLista->pNoCorrente->pNoAnterior)
        /* Nó corrente é o primeiro nó da lista (não possui antecessor) */
        return LIS_CondRetNoNaoExiste;
        /* Retorna condição de nó inexistente */

    pCabecaDaLista->pNoCorrente = pCabecaDaLista->pNoCorrente->pNoAnterior;
    /* Nó corrente caminha para trás */

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/****************************************************************************
*
*	$FC Função:
*       LIS Ir para primeiro nó.
*
****************************************************************************/
LIS_tpCondRet LIS_IrParaPrimeiroNo(LIS_tppCabecaLista pCabecaDaLista)
{

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    if (!pCabecaDaLista->pNoCorrente || !pCabecaDaLista->pNoPrimeiro)
        /* Lista não possui nós */
        return LIS_CondRetListaVazia; /* Retorna condição de lista vazia */

    pCabecaDaLista->pNoCorrente = pCabecaDaLista->pNoPrimeiro;
    /* Ponteiro corrente aponta para o primeiro nó */

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

#ifdef _DEBUG
/****************************************************************************
*
*	$FC Função:
*       LIS Deturpador.
*
****************************************************************************/
LIS_tpCondRet LIS_Deturpador(LIS_tppCabecaLista pCabecaDaLista, int deturpacao)
{

    int lixo;

    LIS_tpNoLista *pNoCorrente;

    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;

    pNoCorrente = pCabecaDaLista->pNoCorrente;

    switch (deturpacao)
    {
    case 1: /* Elimina o elemento corrente da estrutura escolhida */
        LIS_LiberarNo(pCabecaDaLista, pNoCorrente);
        break;

    case 2: /* Atribui NULL ao ponteiro para o próximo nó */
        pNoCorrente->pNoProximo = NULL;
        break;

    case 3: /* Atribui NULL ao ponteiro para o nó anterior */
        pNoCorrente->pNoAnterior = NULL;
        break;

    case 4: /* Atribui lixo ao ponteiro para o próximo nó */
        pNoCorrente->pNoProximo = (LIS_tpNoLista *)&lixo;
        break;

    case 5: /* Atribui lixo ao ponteiro o nó anterior */
        pNoCorrente->pNoAnterior = (LIS_tpNoLista *)&lixo;
        break;

    case 6: /* Atribui NULL ao ponteiro para o conteúdo do nó */
        pNoCorrente->pConteudo = NULL;
        break;

    case 7: /* Altera o tipo de estrutura apontado no nó */
        pNoCorrente->tipoEstrutura = '0';
        break;

    case 8: /* Desencadeia nó sem liberá-lo com free */

        /* Desencadeia à esquerda */

        if (pNoCorrente->pNoAnterior) /* Nó a ser excluído possui anterior */
        {
            pNoCorrente->pNoAnterior->pNoProximo = pNoCorrente->pNoProximo;
            /* Conecta nó anterior ao nó posterior do nó a ser excluído */
            pCabecaDaLista->pNoCorrente = pNoCorrente->pNoAnterior;
            /* Nó corrente passa a ser o nó anterior do que será excluído */
        }
        else /* Queremos excluir o primeiro nó da lista */
        {
            pCabecaDaLista->pNoCorrente = pNoCorrente->pNoProximo;
            /* Nó corrente passa a ser o nó posterior ao que será excluído */
            pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoCorrente;
            /* Primeiro nó da lista passa a ser o nó posterior ao que será 
        excluído */
        }

        /* Desencadeia à direita */

        if (pNoCorrente->pNoProximo)
            /* Nó a ser excluído não é o último nó da lista */
            pNoCorrente->pNoProximo->pNoAnterior = pNoCorrente->pNoAnterior;
        /* Conecta nó posterior ao nó anterior do nó a ser excluído */

        break;

    case 9: /* Atribui NULL ao ponteiro corrente */
        pCabecaDaLista->pNoCorrente = NULL;
        break;

    case 10: /* Atribui NULL ao ponteiro de origem */
        pCabecaDaLista->pNoPrimeiro = NULL;
        break;

    case 11: /* Atribui lixo ao ponteiro corrente */
        pCabecaDaLista->pNoCorrente = (LIS_tpNoLista *)&lixo;
        break;

    case 12: /* Atribui lixo ao ponteiro de origem */
        pCabecaDaLista->pNoPrimeiro = (LIS_tpNoLista *)&lixo;
        break;

    case 13: /* Atribui NULL ao ponteiro para função de destruição do conteúdo
    de um nó */
        pCabecaDaLista->ExcluirValor = NULL;
        break;

    case 14: /* Altera tamanho do nó */
        pCabecaDaLista->pNoCorrente->tamNo = sizeof(LIS_tpNoLista) * 2;
        break;

    default:
        break;
    }

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/****************************************************************************
*
*	$FC Função:
*       LIS Verificador.
*
****************************************************************************/
int LIS_Verificador(LIS_tppCabecaLista pCabecaDaLista)
{

    int numFalhasObservadas, numNos, numSucessosObservados;

    LIS_tpNoLista *pNo, *ultimoNoDaVolta;

    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;

    numFalhasObservadas = 0;
    /* Inicialização do contador de número de falhas   */
    numSucessosObservados = 0;
    /* Inicialização do contador de número de sucessos */

    if (!pCabecaDaLista->ExcluirValor)
    /* Função de destruição do conteúdo de um nó é nula */
        ++numFalhasObservadas;


    if (pCabecaDaLista->numNos && !pCabecaDaLista->pNoCorrente)
    /* Lista não é vazia (numNos != 0) mas nó corrente é nulo */
        ++numFalhasObservadas;

    else
    {
        ++numSucessosObservados;

        if (pCabecaDaLista->numNos && pCabecaDaLista->pNoCorrente->pCabeca != pCabecaDaLista)
        /* Lista não é vazia (numNos != 0) e ponteiro corrente aponta para
        lixo */
            ++numFalhasObservadas;

        else
            ++numSucessosObservados;

    }

    if (pCabecaDaLista->numNos && !pCabecaDaLista->pNoPrimeiro)
    /* Lista não é vazia (numNos != 0) mas primeiro nó é nulo */
    {
        ++numFalhasObservadas;

        pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoUltimo;

        while (pCabecaDaLista->pNoPrimeiro->pNoAnterior)
            pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoPrimeiro->pNoAnterior;
    }
    else
    {
        ++numSucessosObservados;
        if (pCabecaDaLista->numNos && pCabecaDaLista->pNoPrimeiro->pCabeca != pCabecaDaLista)
        /* Lista não é vazia (numNos != 0) e ponteiro do primeiro nó aponta para
    lixo */
        {
            ++numFalhasObservadas;
            pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoUltimo;

            while (pCabecaDaLista->pNoPrimeiro->pNoAnterior)
                pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoPrimeiro->pNoAnterior;
        }
        else
            ++numSucessosObservados;

    }
    /* Vamos caminhar do primeiro nó até o último nó checando cada nó */

    pNo = pCabecaDaLista->pNoPrimeiro;

    numNos = pCabecaDaLista->numNos;

    ultimoNoDaVolta = pCabecaDaLista->pNoPrimeiro;

    while (1) /* Para sempre */
    {

        if (!pNo)
            break;

        if (pCabecaDaLista->tipoEstrutura != pNo->tipoEstrutura)
        /* Tipo da estrutura apontado pela cabeça é diferente do apontado pelo
        nó */
            ++numFalhasObservadas;

        else
            ++numSucessosObservados;

        if (pCabecaDaLista->tamNo != pNo->tamNo)
        /* Tamanho do nó apontado pela cabeça é diferente do apontado pelo
        nó */
            ++numFalhasObservadas;

        else
            ++numSucessosObservados;

        if (!pNo->pConteudo) /* Conteúdo do nó é nulo */
            ++numFalhasObservadas;

        else
            ++numSucessosObservados;

        if (!pNo->pNoProximo && numNos == 1)
        /* Chegou ao último nó e o número de nós restantes é 1 */
            break;

        if (!pNo->pNoProximo && numNos != 1)
        /* Chegou ao último nó mas o número de nós restantes não é 1,
        prox nó é nulo sem estar no final da lista */
        {
            ++numFalhasObservadas;
            ultimoNoDaVolta = pNo; /* Marca último nó analisado */
            break;
        }

        else
            ++numSucessosObservados;

        if (pNo->pNoProximo && numNos == 0)
        /* Número de nós restantes é 0, mas não chegou a um nó nulo */
        {
            ++numFalhasObservadas;
            break;
        }

        else
            ++numSucessosObservados;

        if (!pNo->pNoProximo->pNoAnterior)
        /* Nó anterior ao próximo é nulo */
            ++numFalhasObservadas;

        else
        {
            ++numSucessosObservados;

            if (pNo->pNoProximo->pNoAnterior != pNo)
            /* Nó anterior ao próximo é nulo */
                ++numFalhasObservadas;

            else
                ++numSucessosObservados;
        }

        pNo = pNo->pNoProximo; /* Vai para próximo nó */
        --numNos;
    }

    if (pNo != pCabecaDaLista->pNoUltimo) /* Não chegou nó último nó da lista */
    {

        /* Vamos caminhar do último nó até o primeiro nó checando cada nó */
        pNo = pCabecaDaLista->pNoUltimo;

        numNos = pCabecaDaLista->numNos;

        while (pNo != ultimoNoDaVolta) /* Para sempre */
        {

            if (pCabecaDaLista->tipoEstrutura != pNo->tipoEstrutura)
            /* Tipo da estrutura apontado pela cabeça é diferente do apontado pelo
        nó */
                ++numFalhasObservadas;

            else
                ++numSucessosObservados;

            if (pCabecaDaLista->tamNo != pNo->tamNo)
            /* Tamanho do nó apontado pela cabeça é diferente do apontado pelo
        nó */
                ++numFalhasObservadas;

            else
                ++numSucessosObservados;

            if (!pNo->pConteudo) /* Conteúdo do nó é nulo */
                ++numFalhasObservadas;

            else
                ++numSucessosObservados;

            if (!pNo->pNoAnterior && numNos == 1)
            /* Chegou ao primeiro nó e o número de nós restantes é 1 */
                break;

            if (!pNo->pNoAnterior && numNos != 1)
            /* Chegou ao primeiro nó mas o número de nós restantes não é 1,
        nó anterior é nulo sem estar no começo da lista */
            {
                ++numFalhasObservadas;
                break;
            }

            else
                ++numSucessosObservados;

            if (pNo->pNoAnterior && numNos == 0)
            /* Número de nós restantes é 0, mas não chegou a um nó nulo */
            {
                ++numFalhasObservadas;
                break;
            }

            else
                ++numSucessosObservados;

            if (pNo->pNoAnterior != ultimoNoDaVolta && !pNo->pNoAnterior->pNoProximo)
            /* Nó sucessor do anterior é nulo */
                ++numFalhasObservadas;

            else
            {
                ++numSucessosObservados;

                if (pNo->pNoAnterior != ultimoNoDaVolta && pNo->pNoAnterior->pNoProximo != pNo)
                /* Nó anterior ao próximo é nulo */
                    ++numFalhasObservadas;

                else
                    ++numSucessosObservados;
            }

            pNo = pNo->pNoAnterior; /* Vai para próximo nó */
            --numNos;
        }
    }

    return numFalhasObservadas;
}
#endif

/**************** Código das funções encapsuladas no módulo ****************/

/****************************************************************************
*
*	$FC Função:
*       LIS Libera espaço alocado apontado pelo nó
*       corrente da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Ponteiro corrente aponta para o nó
*       que deseja-se liberar.
*       Nó a ser liberado não é nulo.
*		Cabeça da lista não é nula.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
*
*  $EP Parâmetros
*       $P pCabecaDaLista - O parâmetro que receberá o endereço do ponteiro
*           para a cabeça da lista.
*           Este parâmetro é passado por referência.
*       $P pNo - O parâmetro que receberá o ponteiro para o nó a ser
*           liberado.
*           Este parâmetro é passado por valor.
*
*
*	$AS Assertivas de saída esperadas:
*       Espaço alocado apontado pelo nó corrente da lista foi liberado.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       LIS_CondRetOK
*
****************************************************************************/
LIS_tpCondRet LIS_LiberarNo(LIS_tppCabecaLista pCabecaDaLista, LIS_tpNoLista *pNo)
{

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
    if (!pNo)
        return LIS_CondRetNoNaoExiste;
#endif

    if (pCabecaDaLista->ExcluirValor)
        pCabecaDaLista->ExcluirValor(pNo->pConteudo); /* Libera conteúdo do nó */

    free(pNo); /* Libera espaço apontado pelo endereço do nó */

#ifdef _DEBUG
    pCabecaDaLista->numNos--;
#endif

    return LIS_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       LIS Cria nó da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Conteúdo a ser inserido no nó não é nulo. 
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*  $EP Parâmetros
*       $P pConteudo - O parâmetro que receberá o ponteiro para o conteúdo a
*           ser inserido no nó criado.
*           Este parâmetro é passado por valor.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó foi criado.
*		Valem as assertivas estruturais da lista duplamente encadeada com
*       cabeça.
*
*
*   $FV Valor retornado
*       pNo (ponteiro para nó criado)
*       NULL
*
****************************************************************************/
LIS_tpNoLista *LIS_CriarNo(void *pConteudo
#ifdef _DEBUG
                          ,
                          LIS_tppCabecaLista pCabecaDaLista
#endif
)
{

    LIS_tpNoLista *pNo;

#ifdef _DEBUG
    if (!pConteudo)
        return NULL;
    if (!pCabecaDaLista)
        return NULL;
#endif

    pNo = (LIS_tpNoLista *)malloc(sizeof(LIS_tpNoLista));
    /* Malloc do nó novo */

    if (!pNo) /* Malloc do nó falhou. Retorna nulo */
        return NULL;

    pNo->pConteudo = pConteudo;
    pNo->pNoAnterior = NULL;
    pNo->pNoProximo = NULL;

#ifdef _DEBUG
    pCabecaDaLista->numNos++;      /* Incrementa número de nós da lista */
    pNo->pCabeca = pCabecaDaLista; /* Ponteiro para a cabeca da estrutura */
    pNo->tipoEstrutura = 'l';
    pNo->tamNo = sizeof(LIS_tpNoLista);
    pCabecaDaLista->tamLista += pNo->tamNo;
    pNo->tamLista = pCabecaDaLista->tamLista;
#endif

    return pNo;
}
