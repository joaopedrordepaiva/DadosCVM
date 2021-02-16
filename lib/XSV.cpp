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
*       Separador de colunas pode ser conjunto de caracteres.
*       Linhas do arquivo de input têm, no máximo, 1022 caracteres
*       por casua da string de tamanho 1024 passada para fgets.
*       (1022 chars + \r + \n ou 1023 chars + \n).
*       Arquivo deve ter até 64 colunas por causa do conjunto de colunas
*       representado por um unsigned long long (64 bits).
*
*   $HA Histórico de evolução:
*       Versão  Autor   Data        Observações
*       1.00    jpp     18/04/2020  Início do desenvolvimento
*       2.00    jpp     20/04/2020  Funções parsing e impressão de colunas
*       3.00    jpp     24/04/2020  Nomes de variáveis, tranformações de 
*                                   colunas e adição desordenada de colunas
*       4.00    jpp     23/05/2020  Impressão de colunas com ordem
*                                   determinada pelo usuário, retirada das
*                                   funções de arquivo de output e impressão
*                                   condicional.
*       5.00    jpp     24/05/2020  Lista de colunas condicionais e para
*                                   impressão pode conter colunas que não
*                                   estão estão no arquivo ou ser vazia.
*       6.00    jpp     25/05/2020  Função de execução do processamento.
*                                   Documentação de funções encapsuladas.
*       7.00    jpp     07/06/2020  Detecção correta do final da linha. Não é
*                                   mais necessário testar a última coluna
*                                   separadamente.
*       8.00    jpp     19/07/2020  Ajuste da ordem da lista para impressão
*                                   corrigido. Agora é possível ter mais de
*                                   uma coluna inexistente na lista. strings
*                                   trocadas para const char* para poder
*                                   passar strings literais nas chamadas de
*                                   funções.
*       8.00    jpp     15/02/2021  Tipo lista de strings para impressão
*                                   convertido para std::list (versão c++).
*
****************************************************************************/

#include "XSV.hpp"

/********************* Variáveis encapuladas no módulo *********************/

/*********************** Tipos encapulados no módulo ***********************/

typedef std::list<XSV_tpColunaParaImpressao> XSV_tppListaDeColunasParaImpressao;
typedef std::list<XSV_tpColunaCondicional> XSV_tppListaDeColunasCondicionais;
typedef std::list<std::string> XSV_tppListaDeStringsParaImpressao;

struct XSV_tagHandleXSV
{
    XSV_tppListaDeColunasParaImpressao pListaDeColunasParaImpressao;
    XSV_tppListaDeColunasCondicionais pListaDeColunasCondicionais;

    std::bitset<256> vConjuntoDeColunasParaImpressao;
    std::bitset<256> vConjuntoDeColunasCondicionais;

    const char *pCaminhoParaArquivoDeInput;
    const char *pSeparadorDeColunas;

    uint8_t vNumNos;
};

typedef struct XSV_tpColunaParaImpressao
{
    const char *pNomeDaColunaParaImpressao;
    void (*funcaoDeTransformacao)(std::string);
    uint8_t vPosicaoRequerida;

} * XSV_tppColunaParaImpressao;

typedef struct XSV_tpColunaCondicional
{
    const char *pNomeDaColunaCondicional;
    void *vParametroCondicao;
    XSV_tpCondicaoDeColuna vCondicaoDaColuna;

} * XSV_tppColunaCondicional;

/************** Protótipos das funções encapuladas no módulo ***************/

static XSV_tpCondRet XSV_ImprimirDadosDeColunasSelecionadas(XSV_tppHandleXSV pHandleXSV);
static XSV_tpCondRet XSV_TransformarListaDeColunasParaImpressaoEmConjunto(XSV_tppHandleXSV pHandleXSV);
static XSV_tpCondRet XSV_TransformarListaDeColunasCondicionaisEmConjunto(XSV_tppHandleXSV pHandleXSV);
static XSV_tpCondRet XSV_AjustaOrdemDaListaDeColunasParaImpressao(XSV_tppListaDeColunasParaImpressao pListaDeColunasParaImpressaoOrdemRequerida, XSV_tppListaDeColunasParaImpressao pListaDeColunasParaImpressaoOrdemDoArquivo);
static XSV_tpCondRet XSV_VerificaColunaCondicional(void *pParametroCondicao, const char *pValorDaColuna, XSV_tpCondicaoDeColuna vCondicaoDaColuna, bool *pCondicaoFoiObedecida);
static XSV_tpCondRet XSV_AcrescentarColunaCondicionalNaLista(XSV_tppListaDeColunasCondicionais *ppListaDeColunasCondicionais, const char *pNomeDaColunaCondicional, void *vParametroCondicao, XSV_tpCondicaoDeColuna vCondicaoDaColuna);
static XSV_tpCondRet XSV_AcrescentarColunaParaImpressaoNaLista(XSV_tppListaDeColunasParaImpressao *ppListaDeColunasParaImpressao, const char *pNomeDaColunaParaImpressao, void (*funcaoDeTransformacao)(std::string), uint8_t vPosicaoRequerida);
static XSV_tpCondRet XSV_CriarListaDeColunas(LIS_tppCabecaLista *ppListaDeColunasSelecionadas);
static XSV_tpCondRet XSV_BuscaColunaNaListaDeColunasCondicionais(XSV_tppListaDeColunasCondicionais pListaDeColunasCondicionais, std::string pNomeDaColunaBuscada, bool *pResultadoDaBusca);
static XSV_tpCondRet XSV_BuscaColunaNaListaDeColunasParaImpressao(XSV_tppListaDeColunasParaImpressao pListaDeColunasParaImpressao, std::string pNomeDaColunaBuscada, bool *pResultadoDaBusca);

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

    // (*ppHandleXSV)->pListaDeColunasParaImpressao = NULL;
    // (*ppHandleXSV)->pListaDeColunasCondicionais = NULL;
    // (*ppHandleXSV)->vNumNos = 0;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Destruir handle de arquivo XSV.
*
****************************************************************************/
// XSV_tpCondRet XSV_DestruirHandleDeArquivoXSV(XSV_tppHandleXSV pHandleXSV)
// {
//     if (pHandleXSV->pListaDeColunasParaImpressao)
//         if (LIS_DestruirLista(pHandleXSV->pListaDeColunasParaImpressao) != LIS_CondRetOK)
//             return XSV_CondRetProblemaDeLista;

//     if (pHandleXSV->pListaDeColunasCondicionais)
//         if (LIS_DestruirLista(pHandleXSV->pListaDeColunasCondicionais) != LIS_CondRetOK)
//             return XSV_CondRetProblemaDeLista;

//     free(pHandleXSV);

//     return XSV_CondRetOK;
// }

/****************************************************************************
*
*	$FC Função:
*       XSV Definir string que separa as colunas do arquivo de input.
*
****************************************************************************/
XSV_tpCondRet XSV_DefinirSeparadorDoInput(XSV_tppHandleXSV pHandleXSV, const char *pSeparadorDeColunas)
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
XSV_tpCondRet XSV_DefinirOpArquivoInput(XSV_tppHandleXSV pHandleXSV, const char *pCaminhoParaArquivoDeInput)
{
    pHandleXSV->pCaminhoParaArquivoDeInput = pCaminhoParaArquivoDeInput;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Armazenar nome, conteúdo e condição de coluna à lista
*           de colunas condicionais do HandleXSV.
*
****************************************************************************/
XSV_tpCondRet XSV_AcrescentarColunaCondicionalAoHandle(XSV_tppHandleXSV pHandleXSV, const char *pNomeDaColunaCondicional, void *pObjetoCondicional, XSV_tpCondicaoDeColuna vCondicaoDaColuna)
{
    XSV_tpCondRet vCondRetDeXSV;

    vCondRetDeXSV = XSV_AcrescentarColunaCondicionalNaLista(&(pHandleXSV->pListaDeColunasCondicionais), pNomeDaColunaCondicional, pObjetoCondicional, vCondicaoDaColuna);

    if (vCondRetDeXSV != XSV_CondRetOK)
        return vCondRetDeXSV;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Armazenar nome e função de transformação de coluna à lista
*           de colunas para impressão do HandleXSV.
*
****************************************************************************/
XSV_tpCondRet XSV_AcrescentarColunaParaImpressaoAoHandle(XSV_tppHandleXSV pHandleXSV, const char *pNomeDaColunaParaImpressao, void (*funcaoDeTransformacao)(std::string))
{
    XSV_tpCondRet vCondRetDeXSV;

    vCondRetDeXSV = XSV_AcrescentarColunaParaImpressaoNaLista(&(pHandleXSV->pListaDeColunasParaImpressao), pNomeDaColunaParaImpressao, funcaoDeTransformacao, pHandleXSV->vNumNos);

    if (vCondRetDeXSV != XSV_CondRetOK)
        return vCondRetDeXSV;
    pHandleXSV->vNumNos++;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Processar o arquivo XSV de acordo com o que o usuário definiu
*           para o Handle. Resultado das operações é colocado em stdout.
*
****************************************************************************/
XSV_tpCondRet XSV_ExecutarProcessamentoDoArquivoXSV(XSV_tppHandleXSV pHandleXSV)
{
    XSV_tpCondRet vCondRetDeXSV;

    vCondRetDeXSV = XSV_TransformarListaDeColunasCondicionaisEmConjunto(pHandleXSV);
    if (vCondRetDeXSV != XSV_CondRetOK)
        return vCondRetDeXSV;

    vCondRetDeXSV = XSV_TransformarListaDeColunasParaImpressaoEmConjunto(pHandleXSV);
    if (vCondRetDeXSV != XSV_CondRetOK)
        return vCondRetDeXSV;

    vCondRetDeXSV = XSV_ImprimirDadosDeColunasSelecionadas(pHandleXSV);
    if (vCondRetDeXSV != XSV_CondRetOK)
        return vCondRetDeXSV;

    return XSV_CondRetOK;
}

/**************** Código das funções encapsuladas no módulo ****************/

/****************************************************************************
*
*	$FC Função:
*       XSV Transformar lista de colunas condicionais em conjunto de colunas
*           condicionais. Ordenar lista de colunas de acordo com a primeira
*           linha do arquivo XSV.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi inicializada (não é nula).
*       Conjunto pode já ter sido criado (Handle será reprocessado e o
*       conjunto sobrescrito).
*       Se alguma das colunas não pertencer às colunas do arquivo, esta
*       coluna será ignorada.
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
*       Lista de colunas condicionais foi transformada em conjunto.
*       Lista de colunas condicionais foi ordenada de acordo com a primeira
*       linha do arquivo XSV.
*       Colunas repetidas ou que não pertencem ao arquivo XSV foram
*       ignoradas.
*       
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
XSV_tpCondRet XSV_TransformarListaDeColunasCondicionaisEmConjunto(XSV_tppHandleXSV pHandleXSV)
{
    std::string pLinhaDoArquivoDeInput;
    std::string pSeparadorDeColunas;

    XSV_tppListaDeColunasCondicionais pListaDeColunasCondicionaisOrdemRequerida = pHandleXSV->pListaDeColunasCondicionais;

    pHandleXSV->vConjuntoDeColunasCondicionais.reset();

    if (!pListaDeColunasCondicionaisOrdemRequerida)
        return XSV_CondRetOK;

    std::bitset<256> *pConjuntoDeColunasCondicionais = &(pHandleXSV->vConjuntoDeColunasCondicionais);

    pSeparadorDeColunas = pHandleXSV->pSeparadorDeColunas;

    std::ifstream pArquivoDeInput(pHandleXSV->pCaminhoParaArquivoDeInput);

    if (!pArquivoDeInput.is_open())
        return XSV_CondRetFalhaNaAberturaDoArquivo;

    if (!std::getline(pArquivoDeInput, pLinhaDoArquivoDeInput))
    {
        pArquivoDeInput.close();
        return XSV_FalhaAoBuscarAPrimeiraLinhaDoArquivoDeInput;
    }

    pArquivoDeInput.close();

    uint8_t vContadorDeColunas = 0;

    XSV_tppListaDeColunasCondicionais pListaDeColunasCondicionaisOrdemDoArquivo = NULL;

    while (true)
    {

        size_t vPosFinal = pLinhaDoArquivoDeInput.find(pSeparadorDeColunas);

        std::string pNomeDaColuna = pLinhaDoArquivoDeInput.substr(0, vPosFinal);

        bool vEstaNaListaDeColunasSelecionadas;

        XSV_tpCondRet vCondRetDeXSV = XSV_BuscaColunaNaListaDeColunasCondicionais(pListaDeColunasCondicionaisOrdemRequerida, pNomeDaColuna, &vEstaNaListaDeColunasSelecionadas);

        if (vCondRetDeXSV == XSV_CondRetListaDeColunasVazia)
            break;

        else if (vCondRetDeXSV != XSV_CondRetOK)
            return vCondRetDeXSV;

        if (vEstaNaListaDeColunasSelecionadas)
        {
            (*pConjuntoDeColunasCondicionais).set(vContadorDeColunas, true);

            XSV_tppColunaCondicional pColunaCondional;

            if (LIS_ObterConteudo(pListaDeColunasCondicionaisOrdemRequerida, (void **)&pColunaCondional) != LIS_CondRetOK)
                return XSV_CondRetProblemaDeLista;

            vCondRetDeXSV = XSV_AcrescentarColunaCondicionalNaLista(&pListaDeColunasCondicionaisOrdemDoArquivo, pColunaCondional->pNomeDaColunaCondicional, pColunaCondional->vParametroCondicao, pColunaCondional->vCondicaoDaColuna);
            if (vCondRetDeXSV != XSV_CondRetOK)
                return vCondRetDeXSV;

            if (LIS_ExcluirNo(pListaDeColunasCondicionaisOrdemRequerida) != LIS_CondRetOK)
                return XSV_CondRetOK;
        }

        if (vPosFinal == std::string::npos)
            break;

        pLinhaDoArquivoDeInput = pLinhaDoArquivoDeInput.substr(pNomeDaColuna.length() + pSeparadorDeColunas.length());
        vContadorDeColunas++;
    }

    if (LIS_DestruirLista(pListaDeColunasCondicionaisOrdemRequerida) != LIS_CondRetOK)
        return XSV_CondRetProblemaDeLista;

    pHandleXSV->pListaDeColunasCondicionais = pListaDeColunasCondicionaisOrdemDoArquivo;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Transformar lista de colunas para impressão em conjunto de
*           colunas para impressão. Ordenar lista de colunas de acordo com a
*           primeira linha do arquivo XSV. A ordem de impressão das colunas nas linhas
*           do output proposta pelo usuário é armazenada.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi inicializada (não é nula).
*       Conjunto pode já ter sido criado (Handle será reprocessado e o
*       conjunto sobrescrito).
*       Se alguma das colunas não pertencer às colunas do arquivo, esta
*       coluna será ignorada.
*       A lista de colunas para impressão não deve estar vazia.
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
*       Lista de colunas para impressão foi transformada em conjunto.
*       Lista de colunas para impressão foi ordenada de acordo com a primeira
*       linha do arquivo XSV.
*       Ordem de impressão das colunas proposta pelo usuário foi armazenada.
*       Colunas repetidas ou que não pertencem ao arquivo XSV foram
*       ignoradas.
*       
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
XSV_tpCondRet XSV_TransformarListaDeColunasParaImpressaoEmConjunto(XSV_tppHandleXSV pHandleXSV)
{

    XSV_tpCondRet vCondRetDeXSV;

    bool vEstaNaListaDeColunasSelecionadas;

    XSV_tppListaDeColunasParaImpressao pListaDeColunasParaImpressaoOrdemRequerida = pHandleXSV->pListaDeColunasParaImpressao;

    if (!pListaDeColunasParaImpressaoOrdemRequerida)
        return XSV_CondRetOK;

    pHandleXSV->vConjuntoDeColunasParaImpressao.reset();

    std::bitset<256> *pConjuntoDeColunasParaImpressao = &(pHandleXSV->vConjuntoDeColunasParaImpressao);

    XSV_tppListaDeColunasParaImpressao pListaDeColunasParaImpressaoOrdemDoArquivo = NULL;

    std::string pSeparadorDeColunas = pHandleXSV->pSeparadorDeColunas;

    std::ifstream pArquivoDeInput(pHandleXSV->pCaminhoParaArquivoDeInput);

    if (!pArquivoDeInput.is_open())
        return XSV_CondRetFalhaNaAberturaDoArquivo;

    std::string pLinhaDoArquivoDeInput;

    if (!std::getline(pArquivoDeInput, pLinhaDoArquivoDeInput))
    {
        pArquivoDeInput.close();
        return XSV_FalhaAoBuscarAPrimeiraLinhaDoArquivoDeInput;
    }

    pArquivoDeInput.close();

    pHandleXSV->vNumNos = 0;

    uint8_t vContadorDeColunas = 0;

    while (true)
    {

        size_t vPosFinal = pLinhaDoArquivoDeInput.find(pSeparadorDeColunas);

        std::string pNomeDaColuna = pLinhaDoArquivoDeInput.substr(0, vPosFinal);

        vCondRetDeXSV = XSV_BuscaColunaNaListaDeColunasParaImpressao(pListaDeColunasParaImpressaoOrdemRequerida, pNomeDaColuna, &vEstaNaListaDeColunasSelecionadas);

        if (vCondRetDeXSV == XSV_CondRetListaDeColunasVazia)
            break;

        else if (vCondRetDeXSV != XSV_CondRetOK)
            return vCondRetDeXSV;

        if (vEstaNaListaDeColunasSelecionadas)
        {
            (*pConjuntoDeColunasParaImpressao).set(vContadorDeColunas, true);

            XSV_tppColunaParaImpressao pColunaParaImpressao;

            if (LIS_ObterConteudo(pListaDeColunasParaImpressaoOrdemRequerida, (void **)&pColunaParaImpressao) != LIS_CondRetOK)
                return XSV_CondRetProblemaDeLista;

            vCondRetDeXSV = XSV_AcrescentarColunaParaImpressaoNaLista(&pListaDeColunasParaImpressaoOrdemDoArquivo, pColunaParaImpressao->pNomeDaColunaParaImpressao, pColunaParaImpressao->funcaoDeTransformacao, pColunaParaImpressao->vPosicaoRequerida);
            if (vCondRetDeXSV != XSV_CondRetOK)
                return vCondRetDeXSV;

            pHandleXSV->vNumNos++;

            if (LIS_ExcluirNo(pListaDeColunasParaImpressaoOrdemRequerida) != LIS_CondRetOK)
                return XSV_CondRetOK;
        }

        if (vPosFinal == std::string::npos)
            break;

        pLinhaDoArquivoDeInput = pLinhaDoArquivoDeInput.substr(pNomeDaColuna.length() + pSeparadorDeColunas.length());
        vContadorDeColunas++;
    }

    if (!pConjuntoDeColunasParaImpressao)
        return XSV_CondRetListaDeColunasNaoCriada;

    vCondRetDeXSV = XSV_AjustaOrdemDaListaDeColunasParaImpressao(pListaDeColunasParaImpressaoOrdemRequerida, pListaDeColunasParaImpressaoOrdemDoArquivo);
    if (vCondRetDeXSV != XSV_CondRetOK)
        return vCondRetDeXSV;

    if (LIS_DestruirLista(pListaDeColunasParaImpressaoOrdemRequerida) != LIS_CondRetOK)
        return XSV_CondRetProblemaDeLista;

    pHandleXSV->pListaDeColunasParaImpressao = pListaDeColunasParaImpressaoOrdemDoArquivo;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Imprimir dados das colunas selecionadas.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da Handle de arquivo XSV.
*       Handle de XSV foi inicializada (não é nula).
*       Conjunto de colunas para impressão deve ter sido definido e não pode
*       ser vazio.
*       Conjunto de colunas condicionais deve ter sido definido e pode ser
*       vazio.
*       A lista de colunas para impressão não deve estar vazia e deve estar
*       ordenada.
*       A lista de colunas condicionais pode estar vazia, mas, caso não
*       esteja, deve estar ordenada.
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
*       foram impressas de acordo com o que o usuário definiu).*       
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
XSV_tpCondRet XSV_ImprimirDadosDeColunasSelecionadas(XSV_tppHandleXSV pHandleXSV)
{

    XSV_tppColunaParaImpressao pColunaParaImpressao;

    XSV_tppListaDeStringsParaImpressao pListaDeStringsParaImpressao;

    std::list<std::string>::iterator iteradorDeLista;

    uint8_t vContadorDeColunas;
    uint8_t vPosicaoRequeridaAnterior;

    std::ifstream pArquivoDeInput(pHandleXSV->pCaminhoParaArquivoDeInput);

    if (!pArquivoDeInput.is_open())
        return XSV_CondRetFalhaNaAberturaDoArquivo;

    XSV_tppListaDeColunasCondicionais pListaDeColunasCondicionais = pHandleXSV->pListaDeColunasCondicionais;
    XSV_tppListaDeColunasParaImpressao pListaDeColunasParaImpressao = pHandleXSV->pListaDeColunasParaImpressao;
    std::bitset<256> vConjuntoDeColunasCondicionais = pHandleXSV->vConjuntoDeColunasCondicionais;
    std::bitset<256> vConjuntoDeColunasParaImpressao = pHandleXSV->vConjuntoDeColunasParaImpressao;
    std::string pSeparadorDeColunas = pHandleXSV->pSeparadorDeColunas;

    std::string pLinhaDoArquivoDeInput;

    if (!std::getline(pArquivoDeInput, pLinhaDoArquivoDeInput))
    {
        pArquivoDeInput.close();
        return XSV_FalhaAoBuscarAPrimeiraLinhaDoArquivoDeInput;
    }

    if (LIS_IrParaPrimeiroNo(pListaDeColunasParaImpressao) != LIS_CondRetOK)
        return XSV_CondRetListaDeColunasVazia;

    pListaDeStringsParaImpressao.resize(pHandleXSV->vNumNos);

    iteradorDeLista = pListaDeStringsParaImpressao.begin();
    vPosicaoRequeridaAnterior = 0;

    do
    {

        if (LIS_ObterConteudo(pListaDeColunasParaImpressao, (void **)&pColunaParaImpressao) != LIS_CondRetOK)
            return XSV_CondRetProblemaDeLista;

        std::advance(iteradorDeLista, pColunaParaImpressao->vPosicaoRequerida - vPosicaoRequeridaAnterior);

        *iteradorDeLista = pColunaParaImpressao->pNomeDaColunaParaImpressao;

        vPosicaoRequeridaAnterior = pColunaParaImpressao->vPosicaoRequerida;

    } while (LIS_IrParaProximoNo(pListaDeColunasParaImpressao) == LIS_CondRetOK);

    iteradorDeLista = pListaDeStringsParaImpressao.begin();
    std::cout << *iteradorDeLista;

    for (++iteradorDeLista; iteradorDeLista != pListaDeStringsParaImpressao.end(); iteradorDeLista++)
        std::cout << pSeparadorDeColunas << *iteradorDeLista;

    std::cout << "\n";

    vPosicaoRequeridaAnterior = pHandleXSV->vNumNos;

    while (std::getline(pArquivoDeInput, pLinhaDoArquivoDeInput))
    {

        if (pListaDeColunasCondicionais && LIS_IrParaPrimeiroNo(pListaDeColunasCondicionais) != LIS_CondRetOK)
            return XSV_CondRetListaDeColunasVazia;

        if (LIS_IrParaPrimeiroNo(pListaDeColunasParaImpressao) != LIS_CondRetOK)
            return XSV_CondRetListaDeColunasVazia;

        vContadorDeColunas = 0;
        bool vCondicaoFoiObedecida = true;

        while (true)
        {

            size_t vPosFinal = pLinhaDoArquivoDeInput.find(pSeparadorDeColunas);

            std::string pValorDaColuna = pLinhaDoArquivoDeInput.substr(0, vPosFinal);

            LIS_tpCondRet vCondRetDeLista;

            if (vConjuntoDeColunasCondicionais.test(vContadorDeColunas))
            {
                XSV_tppColunaCondicional pColunaCondicional;

                if (LIS_ObterConteudo(pListaDeColunasCondicionais, (void **)&pColunaCondicional) != LIS_CondRetOK)
                    return XSV_CondRetProblemaDeLista;

                vCondRetDeLista = LIS_IrParaProximoNo(pListaDeColunasCondicionais);

                if (vCondRetDeLista != LIS_CondRetOK && vCondRetDeLista != LIS_CondRetNoNaoExiste)
                    return XSV_CondRetProblemaDeLista;

                XSV_tpCondRet vCondRetDeXSV = XSV_VerificaColunaCondicional(pColunaCondicional->vParametroCondicao, pValorDaColuna.c_str(), pColunaCondicional->vCondicaoDaColuna, &vCondicaoFoiObedecida);
                if (vCondRetDeXSV != XSV_CondRetOK)
                    return vCondRetDeXSV;

                if (!vCondicaoFoiObedecida)
                    break;
            }

            if (vConjuntoDeColunasParaImpressao.test(vContadorDeColunas))
            {
                if (LIS_ObterConteudo(pListaDeColunasParaImpressao, (void **)&pColunaParaImpressao) != LIS_CondRetOK)
                    return XSV_CondRetProblemaDeLista;

                vCondRetDeLista = LIS_IrParaProximoNo(pListaDeColunasParaImpressao);

                if (vCondRetDeLista != LIS_CondRetOK && vCondRetDeLista != LIS_CondRetNoNaoExiste)
                    return XSV_CondRetProblemaDeLista;

                if (pColunaParaImpressao->funcaoDeTransformacao)
                    pColunaParaImpressao->funcaoDeTransformacao(pValorDaColuna);

                std::advance(iteradorDeLista, pColunaParaImpressao->vPosicaoRequerida - vPosicaoRequeridaAnterior);

                *iteradorDeLista = pValorDaColuna;

                vPosicaoRequeridaAnterior = pColunaParaImpressao->vPosicaoRequerida;
            }

            if (vPosFinal == std::string::npos)
                break;

            pLinhaDoArquivoDeInput = pLinhaDoArquivoDeInput.substr(pValorDaColuna.length() + pSeparadorDeColunas.length());
            vContadorDeColunas++;
        }

        if (vCondicaoFoiObedecida)
        {
            iteradorDeLista = pListaDeStringsParaImpressao.begin();
            std::cout << *iteradorDeLista;

            for (++iteradorDeLista; iteradorDeLista != pListaDeStringsParaImpressao.end(); iteradorDeLista++)
                std::cout << pSeparadorDeColunas << *iteradorDeLista;

            std::cout << "\n";

            vPosicaoRequeridaAnterior = pHandleXSV->vNumNos;
        }
    }

    pArquivoDeInput.close();

    if (LIS_IrParaPrimeiroNo(pListaDeColunasParaImpressao) != LIS_CondRetOK)
        return XSV_CondRetListaDeColunasVazia;

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Ajusta posições requerdidas das colunas para impressão, uma vez
*       que colunas não pertencentes ao arquivo ou repetidas foram excluídas,
*       deixando lacunas na numeração.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da lista de colunas para impressão.
*       A posição corrente das listas não precisa estar no primeiro nó.
*       Lista de colunas para impressão na ordenação do usuário pode estar
*       vazia e deve estar inicializada.
*
*
*  $EP Parâmetros
*       $P pListaDeColunasParaImpressaoOrdemRequerida - O parâmetro que
*           receberá o ponteiro para lista de colunas para impressão na ordem
*           determinada pelo usuário.
*           Este parâmetro é passado por referência.
*       $P pListaDeColunasParaImpressaoOrdemDoArquivo - O parâmetro que
*           receberá o ponteiro para lista de colunas para impressão na ordem
*           determinada pelo arquivo de input.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*		Valem as assertivas estruturais da lista de colunas para impressão.
*       A posição corrente da lista de colunas para impressão na ordenação do
*       arquivo não precisa estar no primeiro nó.
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de teste bem sucedido.
*       XSV_CondRetProblemaDeLista - Função de lista chamada não retornou
*           condição de retorno bem sucedida.
*
****************************************************************************/
XSV_tpCondRet XSV_AjustaOrdemDaListaDeColunasParaImpressao(XSV_tppListaDeColunasParaImpressao pListaDeColunasParaImpressaoOrdemRequerida, XSV_tppListaDeColunasParaImpressao pListaDeColunasParaImpressaoOrdemDoArquivo)
{

    {
        LIS_tpCondRet vCondRetDeLista = LIS_IrParaPrimeiroNo(pListaDeColunasParaImpressaoOrdemRequerida);

        if (vCondRetDeLista == LIS_CondRetListaVazia)
            return XSV_CondRetOK;

        else if (vCondRetDeLista != LIS_CondRetOK)
            return XSV_CondRetProblemaDeLista;
    }

    uint8_t vAjustaValoresSubtracao = 0;

    do
    {

        XSV_tppColunaParaImpressao pColunaParaImpressao;

        if (LIS_ObterConteudo(pListaDeColunasParaImpressaoOrdemRequerida, (void **)&pColunaParaImpressao) != LIS_CondRetOK)
            return XSV_CondRetProblemaDeLista;

        uint8_t vValorNaoPertencenteAsColunas = pColunaParaImpressao->vPosicaoRequerida;

        if (LIS_IrParaPrimeiroNo(pListaDeColunasParaImpressaoOrdemDoArquivo) != LIS_CondRetOK)
            return XSV_CondRetListaDeColunasVazia;

        do
        {
            if (LIS_ObterConteudo(pListaDeColunasParaImpressaoOrdemDoArquivo, (void **)&pColunaParaImpressao) != LIS_CondRetOK)
                return XSV_CondRetProblemaDeLista;

            if ((int)pColunaParaImpressao->vPosicaoRequerida > vValorNaoPertencenteAsColunas - vAjustaValoresSubtracao)
                pColunaParaImpressao->vPosicaoRequerida--;

        } while (LIS_IrParaProximoNo(pListaDeColunasParaImpressaoOrdemDoArquivo) == LIS_CondRetOK);

        vAjustaValoresSubtracao++;

    } while (LIS_IrParaProximoNo(pListaDeColunasParaImpressaoOrdemRequerida) == LIS_CondRetOK);

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Verifica se o valor da coluna segue a condição determinada.
*
*
*   $AE Assertivas de entrada esperadas:
*		Condição de comparação não precisa ser uma das condições básicas.
*
*
*  $EP Parâmetros
*       $P pObjetoCondicional - A string à qual os valores da coluna serão
*           comparados ou a função que define se os valores da coluna
*           obedecem à condição.
*           Este parâmetro é passado por referência.
*       $P pValorDaColuna - O valor atual da coluna ao qual o objeto
*           condicional será comparado.
*           Este parâmetro é passado por referência.
*       $P vCondicaoDaColuna - A condição que descreve a comparação a ser
*           feita entre os valores da coluna e o objeto condicional.
*           Este parâmetro é passado por valor.
*       $P pResultadoDaComparacao - O parâmetro que receberá o resultado da
*           comparação entre o parâmetro e o valor atual da coluna.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Retornou corretamente (de acordo com a condição) o resultado da
*       comparação entre o valor atual da coluna e o parâmetro.
*       Se a condição de comparação não for uma das condições básicas, o
*       resultado da comparação será positivo.
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de teste bem sucedido.
*
****************************************************************************/
XSV_tpCondRet XSV_VerificaColunaCondicional(void *pObjetoCondicional, const char *pValorDaColuna, XSV_tpCondicaoDeColuna vCondicaoDaColuna, bool *pCondicaoFoiObedecida)
{

    switch (vCondicaoDaColuna)
    {
    case XSV_CondicaoIgual:
        if (!strcmp((char *)pObjetoCondicional, pValorDaColuna))
            *pCondicaoFoiObedecida = true;
        else
            *pCondicaoFoiObedecida = false;
        break;

    case XSV_CondicaoDiferente:
        if (strcmp((char *)pObjetoCondicional, pValorDaColuna))
            *pCondicaoFoiObedecida = true;
        else
            *pCondicaoFoiObedecida = false;
        break;

    case XSV_CondicaoContem:
        if (strstr(pValorDaColuna, (char *)pObjetoCondicional))
            *pCondicaoFoiObedecida = true;
        else
            *pCondicaoFoiObedecida = false;
        break;

    case XSV_CondicaoNaoContem:
        if (!strstr(pValorDaColuna, (char *)pObjetoCondicional))
            *pCondicaoFoiObedecida = true;
        else
            *pCondicaoFoiObedecida = false;
        break;

    case XSV_CondicaoPorFuncao:
    {
        if (((char (*)(const char *))pObjetoCondicional)(pValorDaColuna))
            *pCondicaoFoiObedecida = true;
        else
            *pCondicaoFoiObedecida = false;
        break;
    }

    default:
        *pCondicaoFoiObedecida = true;
        break;
    }

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Armazenar nome da coluna, valor da coluna e condição a uma lista
*       de colunas condicionais.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da lista de colunas condicionais.
*
*
*  $EP Parâmetros
*       $P ppListaDeColunasCondicionais - O parâmetro que receberá o endereço
*           do ponteiro para cabeça de lista de colunas condicionais.
*           Este parâmetro é passado por referência.
*       $P pNomeDaColunaCondicional - O parâmetro que receberá a string com
*           o nome da coluna cujos valores devem ser averiguados.
*           Este parâmetro é passado por referência.
*       $P vParametroCondicao - O parâmetro que receberá a string com o valor
*           de comparação para satisfazer ou não à condição ou a função que
*           determina se o valor da coluna obedece ou não à condição.
*           Este parâmetro é passado por referência.
*       $P vCondicaoDaColuna - O parâmetro que receberá a condição de
*           comparação entre os valores da coluna (pNomeDaColunaCondicional)
*           e o valor de comparação (pStringDaCondicao).
*           Este parâmetro é passado por valor.
*
*
*	$AS Assertivas de saída esperadas:
*       Lista possui novo nó com nome da coluna, valor da coluna e condição 
*       especificada.
*       Nó corrente da lista é o novo nó.
*		Valem as assertivas estruturais da lista de colunas condicionais.
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de teste bem sucedido.
*       XSV_CondRetProblemaDeLista - Função de lista chamada não retornou
*           condição de retorno bem sucedida.
*       XSV_CondRetFaltouMemoria - Alocação dinâmica de memória falhou.
*
****************************************************************************/
XSV_tpCondRet XSV_AcrescentarColunaCondicionalNaLista(XSV_tppListaDeColunasCondicionais *ppListaDeColunasCondicionais, const char *pNomeDaColunaCondicional, void *vParametroCondicao, XSV_tpCondicaoDeColuna vCondicaoDaColuna)
{

    XSV_tppColunaCondicional pColunaCondicional = (XSV_tppColunaCondicional)malloc(sizeof(struct XSV_tpColunaCondicional));
    if (!pColunaCondicional)
        return XSV_CondRetFaltouMemoria;

    pColunaCondicional->pNomeDaColunaCondicional = pNomeDaColunaCondicional;
    pColunaCondicional->vParametroCondicao = vParametroCondicao;
    pColunaCondicional->vCondicaoDaColuna = vCondicaoDaColuna;

    (*ppListaDeColunasCondicionais).push_back(pColunaCondicional);

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Armazenar nome e função de transformação de coluna em uma lista
*           de colunas para impressão.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da lista de colunas para impressão.
*
*
*  $EP Parâmetros
*       $P ppListaDeColunasParaImpressao - O parâmetro que receberá o endereço
*           do ponteiro para cabeça da lista de lista de colunas
*           para impressao.
*           Este parâmetro é passado por referência.
*       $P pNomeDaColunaParaImpressao - O parâmetro que receberá a string com
*           o nome da coluna cujos valores devem ser impressos.
*           Este parâmetro é passado por referência.
*       $P funcaoDeTransformacao - O parâmetro que receberá o ponteiro para
*           a função que modifica os valores da coluna que serâo impressos.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       Lista possui novo nó com nome da coluna e função especificada.
*       Nó corrente da lista é o novo nó.
*		Valem as assertivas estruturais da lista de colunas para impressão.
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de teste bem sucedido.
*       XSV_CondRetProblemaDeLista - Função de lista chamada não retornou
*           condição de retorno bem sucedida.
*       XSV_CondRetFaltouMemoria - Alocação dinâmica de memória falhou.
*
****************************************************************************/
XSV_tpCondRet XSV_AcrescentarColunaParaImpressaoNaLista(XSV_tppListaDeColunasParaImpressao *ppListaDeColunasParaImpressao, const char *pNomeDaColunaParaImpressao, void (*funcaoDeTransformacao)(std::string), uint8_t vPosicaoRequerida)
{
    XSV_tppColunaParaImpressao pColunaParaImpressao = (XSV_tppColunaParaImpressao)malloc(sizeof(struct XSV_tpColunaParaImpressao));
    if (!pColunaParaImpressao)
        return XSV_CondRetFaltouMemoria;

    pColunaParaImpressao->pNomeDaColunaParaImpressao = pNomeDaColunaParaImpressao;
    pColunaParaImpressao->funcaoDeTransformacao = funcaoDeTransformacao;
    pColunaParaImpressao->vPosicaoRequerida = vPosicaoRequerida;

    (*ppListaDeColunasParaImpressao).push_back(pColunaParaImpressao);

    return XSV_CondRetOK;
}

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
*           do ponteiro para cabeça da lista de colunas selecionadas.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       A lista de colunas selecionadas foi criada.
*		Valem as assertivas estruturais da lista de colunas selecionadas.
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de teste bem sucedido.
*       XSV_CondRetProblemaDeLista - Função de lista chamada não retornou
*           condição de retorno bem sucedida.
*
****************************************************************************/
// XSV_tpCondRet XSV_CriarListaDeColunas(LIS_tppCabecaLista *ppListaDeColunasSelecionadas)
// {
//     if (LIS_CriarLista(free, ppListaDeColunasSelecionadas) != LIS_CondRetOK)
//         return XSV_CondRetProblemaDeLista;

//     return XSV_CondRetOK;
// }

/****************************************************************************
*
*	$FC Função:
*       XSV Busca string na lista de colunas condicionais.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da lista de colunas condicionais.
*       Nó corrente não precisa ser o primeiro nó.
*
*
*  $EP Parâmetros
*       $P pListaDeColunasCondicionais - O parâmetro que receberá o endereço
*           do ponteiro para cabeça da lista de colunas condicionais.
*           Este parâmetro é passado por referência.
*       $P pNomeDaColunaBuscada - O parâmetro que receberá a string que
*           contém o nome da coluna buscada.
*           Este parâmetro é passado por valor.
*       $P vResultadoDaBusca - O parâmetro que receberá o endereço
*           da variável que contém a resposta se a busca encontrou o nome da
*           coluna buscada.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       O conteúdo do endereço em vResultadoDaBusca é 0 se não encontrou
*       a string. Se encontrou, o conteúdo é a posição onde encontrou. A
*       primeira posição da lista é 0. 
*       Nó corrente é o nó onde o valor foi encontrado. Se não foi
*       encontrado, nó corrente é o primeiro nó.
*       Função retornou corretamente se a coluna buscada estava na lista
*       ou não.
*		Valem as assertivas estruturais da lista de colunas condicionais.
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de teste bem sucedido.
*       XSV_CondRetProblemaDeLista - Função de lista chamada não retornou
*           condição de retorno bem sucedida.
*
****************************************************************************/
XSV_tpCondRet XSV_BuscaColunaNaListaDeColunasCondicionais(XSV_tppListaDeColunasCondicionais pListaDeColunasCondicionais, std::string pNomeDaColunaBuscada, bool *pResultadoDaBusca)
{

    *pResultadoDaBusca = false;

    if (pListaDeColunasCondicionais.empty())
        return XSV_CondRetListaDeColunasVazia;

    {
        XSV_tppListaDeColunasCondicionais::iterator iteradorDeListaDeColunasCondicionais = pListaDeColunasCondicionais.begin();

        if (*iteradorDeListaDeColunasCondicionais == pNomeDaColunaBuscada)
        {
            *pResultadoDaBusca = true;
            return XSV_CondRetOK;
        }
    }

    return XSV_CondRetOK;
}

/****************************************************************************
*
*	$FC Função:
*       XSV Busca string na lista de colunas para impressão.
*
*
*   $AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da lista de colunas para impressão.
*       Nó corrente não precisa ser o primeiro nó.
*
*
*  $EP Parâmetros
*       $P pListaDeColunasParaImpressao - O parâmetro que receberá o endereço
*           do ponteiro para cabeça da lista de colunas para impressão.
*           Este parâmetro é passado por referência.
*       $P pNomeDaColunaBuscada - O parâmetro que receberá a string que
*           contém o nome da coluna buscada.
*           Este parâmetro é passado por valor.
*       $P vResultadoDaBusca - O parâmetro que receberá o endereço
*           da variável que contém a resposta se a busca encontrou o nome da
*           coluna buscada.
*           Este parâmetro é passado por referência.
*
*
*	$AS Assertivas de saída esperadas:
*       O conteúdo do endereço em vResultadoDaBusca é 0 se não encontrou
*       a string. Se encontrou, o conteúdo é a posição onde encontrou. A
*       primeira posição da lista é 0. 
*       Nó corrente é o nó onde o valor foi encontrado. Se não foi
*       encontrado, nó corrente é o primeiro nó.
*       Função retornou corretamente se a coluna buscada estava na lista
*       ou não.
*		Valem as assertivas estruturais da lista de colunas para impressão.
*
*
*   $FV Valor retornado
*       XSV_CondRetOK - Condição de retorno de teste bem sucedido.
*       XSV_CondRetProblemaDeLista - Função de lista chamada não retornou
*           condição de retorno bem sucedida.
*
****************************************************************************/
XSV_tpCondRet XSV_BuscaColunaNaListaDeColunasParaImpressao(XSV_tppListaDeColunasParaImpressao pListaDeColunasParaImpressao, std::string pNomeDaColunaBuscada, bool *pResultadoDaBusca)
{
    *pResultadoDaBusca = false;

    if (!pListaDeColunasParaImpressao)
        return XSV_CondRetListaDeColunasNaoCriada;

    else if (LIS_IrParaPrimeiroNo(pListaDeColunasParaImpressao) != LIS_CondRetOK)
        return XSV_CondRetListaDeColunasVazia;

    do
    {
        XSV_tppColunaParaImpressao pColunaParaImpressao;

        if (LIS_ObterConteudo(pListaDeColunasParaImpressao, (void **)&pColunaParaImpressao) != LIS_CondRetOK)
            return XSV_CondRetProblemaDeLista;

        if (pNomeDaColunaBuscada == pColunaParaImpressao->pNomeDaColunaParaImpressao)
        {
            *pResultadoDaBusca = true;
            return XSV_CondRetOK;
        }

    } while (LIS_IrParaProximoNo(pListaDeColunasParaImpressao) == LIS_CondRetOK);

    if (LIS_IrParaPrimeiroNo(pListaDeColunasParaImpressao) != LIS_CondRetOK)
        return XSV_CondRetProblemaDeLista;

    return XSV_CondRetOK;
}
