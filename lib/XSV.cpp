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
*       8.00    jpp     15/02/2021  Tipo string convertidas para std::string,
*                                   tipo lista de strings para impressão
*                                   convertido para std::list e bitsets com
*                                   tipo inteiro convertidos para std::bitset
*                                   (início da conversão para C++).
*       8.00    jpp     17/02/2021  Listas de colunas convertidas para
*                                   std::list.
*
****************************************************************************/

#include "XSV.hpp"

/********************* Variáveis encapuladas no módulo *********************/

/*********************** Tipos encapulados no módulo ***********************/

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

typedef std::list<XSV_tpColunaParaImpressao> XSV_tpListaDeColunasParaImpressao;
typedef std::list<XSV_tpColunaParaImpressao>::iterator XSV_tpIteradorDeListaDeColunasParaImpressao;
typedef std::list<XSV_tpColunaCondicional> XSV_tpListaDeColunasCondicionais;
typedef std::list<XSV_tpColunaCondicional>::iterator XSV_tpIteradorDeListaDeColunasCondicionais;
typedef std::list<std::string> XSV_tpListaDeStringsParaImpressao;

struct XSV_tagHandleXSV
{
    XSV_tpListaDeColunasParaImpressao *pListaDeColunasParaImpressao;
    XSV_tpListaDeColunasCondicionais *pListaDeColunasCondicionais;

    std::bitset<256> vConjuntoDeColunasParaImpressao;
    std::bitset<256> vConjuntoDeColunasCondicionais;

    const char *pCaminhoParaArquivoDeInput;
    const char *pSeparadorDeColunas;
};

/************** Protótipos das funções encapuladas no módulo ***************/

static XSV_tpCondRet XSV_ImprimirDadosDeColunasSelecionadas(XSV_tppHandleXSV pHandleXSV);
static XSV_tpCondRet XSV_TransformarListasDeColunasEmConjuntos(XSV_tppHandleXSV pHandleXSV);
static XSV_tpCondRet XSV_AjustaOrdemDaListaDeColunasParaImpressao(XSV_tpListaDeColunasParaImpressao pListaDeColunasParaImpressaoOrdemRequerida, XSV_tpListaDeColunasParaImpressao pListaDeColunasParaImpressaoOrdemDoArquivo);
static XSV_tpCondRet XSV_VerificaColunaCondicional(void *pParametroCondicao, const char *pValorDaColuna, XSV_tpCondicaoDeColuna vCondicaoDaColuna, bool *pCondicaoFoiObedecida);
static XSV_tpCondRet XSV_AcrescentarColunaCondicionalNaLista(XSV_tpListaDeColunasCondicionais *ppListaDeColunasCondicionais, const char *pNomeDaColunaCondicional, void *vParametroCondicao, XSV_tpCondicaoDeColuna vCondicaoDaColuna);
static XSV_tpCondRet XSV_AcrescentarColunaParaImpressaoNaLista(XSV_tpListaDeColunasParaImpressao *ppListaDeColunasParaImpressao, const char *pNomeDaColunaParaImpressao, void (*funcaoDeTransformacao)(std::string), uint8_t vPosicaoRequerida);

/**************** Código das funções exportadas pelo módulo ****************/

/****************************************************************************
*
*	$FC Função:
*       XSV Criar handle de arquivo XSV.
*
****************************************************************************/
XSV_tpCondRet XSV_CriarHandleDeArquivoXSV(XSV_tppHandleXSV *ppHandleXSV)
{
    *ppHandleXSV = new XSV_tagHandleXSV;

    (*ppHandleXSV)->pListaDeColunasCondicionais = new XSV_tpListaDeColunasCondicionais;
    (*ppHandleXSV)->pListaDeColunasParaImpressao = new XSV_tpListaDeColunasParaImpressao;

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
    XSV_tpListaDeColunasParaImpressao *pListaDeColunasParaImpressao = pHandleXSV->pListaDeColunasParaImpressao;
    XSV_tpListaDeColunasCondicionais *pListaDeColunasCondicionais = pHandleXSV->pListaDeColunasCondicionais;

    if (!pListaDeColunasCondicionais->empty())
        pListaDeColunasCondicionais->clear();

    delete pListaDeColunasParaImpressao;

    if (!pListaDeColunasCondicionais->empty())
        pListaDeColunasCondicionais->clear();

    delete pListaDeColunasCondicionais;

    delete pHandleXSV;

    return XSV_CondRetOK;
}

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

    vCondRetDeXSV = XSV_AcrescentarColunaCondicionalNaLista(pHandleXSV->pListaDeColunasCondicionais, pNomeDaColunaCondicional, pObjetoCondicional, vCondicaoDaColuna);

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
    XSV_tpListaDeColunasParaImpressao *pListaDeColunasParaImpressao = pHandleXSV->pListaDeColunasParaImpressao;

    XSV_tpCondRet vCondRetDeXSV = XSV_AcrescentarColunaParaImpressaoNaLista(pListaDeColunasParaImpressao, pNomeDaColunaParaImpressao, funcaoDeTransformacao, pListaDeColunasParaImpressao->size());

    if (vCondRetDeXSV != XSV_CondRetOK)
        return vCondRetDeXSV;

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

    vCondRetDeXSV = XSV_TransformarListasDeColunasEmConjuntos(pHandleXSV);
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
XSV_tpCondRet XSV_TransformarListasDeColunasEmConjuntos(XSV_tppHandleXSV pHandleXSV)
{
    std::string pLinhaDoArquivoDeInput;
    std::string pSeparadorDeColunas;

    XSV_tpListaDeColunasCondicionais *pListaDeColunasCondicionaisOrdemRequerida = pHandleXSV->pListaDeColunasCondicionais;
    XSV_tpListaDeColunasParaImpressao *pListaDeColunasParaImpressaoOrdemRequerida = pHandleXSV->pListaDeColunasParaImpressao;

    std::bitset<256> *pConjuntoDeColunasCondicionais = &(pHandleXSV->vConjuntoDeColunasCondicionais);
    std::bitset<256> *pConjuntoDeColunasParaImpressao = &(pHandleXSV->vConjuntoDeColunasParaImpressao);

    pConjuntoDeColunasCondicionais->reset();
    pConjuntoDeColunasParaImpressao->reset();

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

    auto pListaDeColunasCondicionaisOrdemDoArquivo = new XSV_tpListaDeColunasCondicionais;
    auto pListaDeColunasParaImpressaoOrdemDoArquivo = new XSV_tpListaDeColunasParaImpressao;

    while (true)
    {

        size_t vPosFinal = pLinhaDoArquivoDeInput.find(pSeparadorDeColunas);

        std::string pNomeDaColuna = pLinhaDoArquivoDeInput.substr(0, vPosFinal);

        for (XSV_tpIteradorDeListaDeColunasCondicionais iteradorDeListaDeColunasCondicionais = pListaDeColunasCondicionaisOrdemRequerida->begin(); iteradorDeListaDeColunasCondicionais != pListaDeColunasCondicionaisOrdemRequerida->end(); iteradorDeListaDeColunasCondicionais++)
            if (iteradorDeListaDeColunasCondicionais->pNomeDaColunaCondicional == pNomeDaColuna)
            {
                pListaDeColunasCondicionaisOrdemDoArquivo->splice(pListaDeColunasCondicionaisOrdemDoArquivo->end(), (*pListaDeColunasCondicionaisOrdemRequerida), iteradorDeListaDeColunasCondicionais);
                (*pConjuntoDeColunasCondicionais).set(vContadorDeColunas, true);
                break;
            }

        for (XSV_tpIteradorDeListaDeColunasParaImpressao iteradorDeListaDeColunasParaImpressao = pListaDeColunasParaImpressaoOrdemRequerida->begin(); iteradorDeListaDeColunasParaImpressao != pListaDeColunasParaImpressaoOrdemRequerida->end(); iteradorDeListaDeColunasParaImpressao++)
            if (iteradorDeListaDeColunasParaImpressao->pNomeDaColunaParaImpressao == pNomeDaColuna)
            {
                pListaDeColunasParaImpressaoOrdemDoArquivo->splice(pListaDeColunasParaImpressaoOrdemDoArquivo->end(), (*pListaDeColunasParaImpressaoOrdemRequerida), iteradorDeListaDeColunasParaImpressao);
                (*pConjuntoDeColunasParaImpressao).set(vContadorDeColunas, true);
                break;
            }

        if (vPosFinal == std::string::npos)
            break;

        pLinhaDoArquivoDeInput = pLinhaDoArquivoDeInput.substr(pNomeDaColuna.length() + pSeparadorDeColunas.length());
        vContadorDeColunas++;
    }

    XSV_tpCondRet vCondRetDeXSV = XSV_AjustaOrdemDaListaDeColunasParaImpressao(*pListaDeColunasParaImpressaoOrdemRequerida, *pListaDeColunasParaImpressaoOrdemDoArquivo);
    if (vCondRetDeXSV != XSV_CondRetOK)
        return vCondRetDeXSV;

    pListaDeColunasCondicionaisOrdemRequerida->clear();
    pListaDeColunasParaImpressaoOrdemRequerida->clear();

    delete pListaDeColunasCondicionaisOrdemRequerida;
    delete pListaDeColunasParaImpressaoOrdemRequerida;

    pHandleXSV->pListaDeColunasCondicionais = pListaDeColunasCondicionaisOrdemDoArquivo;
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
    XSV_tpListaDeStringsParaImpressao vListaDeStringsParaImpressao;

    std::list<std::string>::iterator iteradorDeListaDeStringsParaImpressao;

    uint8_t vContadorDeColunas;
    uint8_t vPosicaoRequeridaAnterior;

    std::ifstream pArquivoDeInput(pHandleXSV->pCaminhoParaArquivoDeInput);

    if (!pArquivoDeInput.is_open())
        return XSV_CondRetFalhaNaAberturaDoArquivo;

    XSV_tpListaDeColunasCondicionais *pListaDeColunasCondicionais = pHandleXSV->pListaDeColunasCondicionais;
    XSV_tpListaDeColunasParaImpressao *pListaDeColunasParaImpressao = pHandleXSV->pListaDeColunasParaImpressao;

    std::bitset<256> vConjuntoDeColunasCondicionais = pHandleXSV->vConjuntoDeColunasCondicionais;
    std::bitset<256> vConjuntoDeColunasParaImpressao = pHandleXSV->vConjuntoDeColunasParaImpressao;
    std::string pSeparadorDeColunas = pHandleXSV->pSeparadorDeColunas;

    std::string pLinhaDoArquivoDeInput;

    if (!std::getline(pArquivoDeInput, pLinhaDoArquivoDeInput))
    {
        pArquivoDeInput.close();
        return XSV_FalhaAoBuscarAPrimeiraLinhaDoArquivoDeInput;
    }

    vListaDeStringsParaImpressao.resize(pListaDeColunasParaImpressao->size());

    iteradorDeListaDeStringsParaImpressao = vListaDeStringsParaImpressao.begin();
    vPosicaoRequeridaAnterior = 0;

    for (XSV_tpIteradorDeListaDeColunasParaImpressao iteradorDeListaDeColunasParaImpressao = pListaDeColunasParaImpressao->begin(); iteradorDeListaDeColunasParaImpressao != pListaDeColunasParaImpressao->end(); iteradorDeListaDeColunasParaImpressao++)
    {
        XSV_tpColunaParaImpressao vColunaParaImpressao = *iteradorDeListaDeColunasParaImpressao;

        std::advance(iteradorDeListaDeStringsParaImpressao, vColunaParaImpressao.vPosicaoRequerida - vPosicaoRequeridaAnterior);

        *iteradorDeListaDeStringsParaImpressao = vColunaParaImpressao.pNomeDaColunaParaImpressao;

        vPosicaoRequeridaAnterior = vColunaParaImpressao.vPosicaoRequerida;
    }

    iteradorDeListaDeStringsParaImpressao = vListaDeStringsParaImpressao.begin();
    std::cout << *iteradorDeListaDeStringsParaImpressao;

    for (++iteradorDeListaDeStringsParaImpressao; iteradorDeListaDeStringsParaImpressao != vListaDeStringsParaImpressao.end(); iteradorDeListaDeStringsParaImpressao++)
        std::cout << pSeparadorDeColunas << *iteradorDeListaDeStringsParaImpressao;

    std::cout << std::endl;

    vPosicaoRequeridaAnterior = pListaDeColunasParaImpressao->size();

    while (std::getline(pArquivoDeInput, pLinhaDoArquivoDeInput))
    {
        XSV_tpIteradorDeListaDeColunasParaImpressao iteradorDeListaDeColunasParaImpressao = pListaDeColunasParaImpressao->begin();
        XSV_tpIteradorDeListaDeColunasCondicionais iteradorDeListaDeColunasCondicionais = pListaDeColunasCondicionais->begin();

        vContadorDeColunas = 0;
        bool vCondicaoFoiObedecida = true;

        while (true)
        {

            size_t vPosFinal = pLinhaDoArquivoDeInput.find(pSeparadorDeColunas);

            std::string pValorDaColuna = pLinhaDoArquivoDeInput.substr(0, vPosFinal);

            if (vConjuntoDeColunasCondicionais.test(vContadorDeColunas))
            {
                XSV_tpColunaCondicional vColunaCondicional = *iteradorDeListaDeColunasCondicionais;

                iteradorDeListaDeColunasCondicionais++;

                XSV_tpCondRet vCondRetDeXSV = XSV_VerificaColunaCondicional(vColunaCondicional.vParametroCondicao, pValorDaColuna.c_str(), vColunaCondicional.vCondicaoDaColuna, &vCondicaoFoiObedecida);
                if (vCondRetDeXSV != XSV_CondRetOK)
                    return vCondRetDeXSV;

                if (!vCondicaoFoiObedecida)
                    break;
            }

            if (vConjuntoDeColunasParaImpressao.test(vContadorDeColunas))
            {

                XSV_tpColunaParaImpressao vColunaParaImpressao = *iteradorDeListaDeColunasParaImpressao;

                iteradorDeListaDeColunasParaImpressao++;

                if (vColunaParaImpressao.funcaoDeTransformacao)
                    vColunaParaImpressao.funcaoDeTransformacao(pValorDaColuna);

                std::advance(iteradorDeListaDeStringsParaImpressao, vColunaParaImpressao.vPosicaoRequerida - vPosicaoRequeridaAnterior);

                *iteradorDeListaDeStringsParaImpressao = pValorDaColuna;

                vPosicaoRequeridaAnterior = vColunaParaImpressao.vPosicaoRequerida;
            }

            if (vPosFinal == std::string::npos)
                break;

            pLinhaDoArquivoDeInput = pLinhaDoArquivoDeInput.substr(pValorDaColuna.length() + pSeparadorDeColunas.length());
            vContadorDeColunas++;
        }

        if (vCondicaoFoiObedecida)
        {
            iteradorDeListaDeStringsParaImpressao = vListaDeStringsParaImpressao.begin();
            std::cout << *iteradorDeListaDeStringsParaImpressao;

            for (++iteradorDeListaDeStringsParaImpressao; iteradorDeListaDeStringsParaImpressao != vListaDeStringsParaImpressao.end(); iteradorDeListaDeStringsParaImpressao++)
                std::cout << pSeparadorDeColunas << *iteradorDeListaDeStringsParaImpressao;

            std::cout << "\n";

            vPosicaoRequeridaAnterior = pListaDeColunasParaImpressao->size();
        }
    }

    pArquivoDeInput.close();

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
XSV_tpCondRet XSV_AjustaOrdemDaListaDeColunasParaImpressao(XSV_tpListaDeColunasParaImpressao pListaDeColunasParaImpressaoOrdemRequerida, XSV_tpListaDeColunasParaImpressao pListaDeColunasParaImpressaoOrdemDoArquivo)
{
    uint8_t vAjustaValoresSubtracao = 0;

    for(XSV_tpIteradorDeListaDeColunasParaImpressao iteradorDeListaDeColunasParaImpressaoOrdemRequerida = pListaDeColunasParaImpressaoOrdemRequerida.begin(); iteradorDeListaDeColunasParaImpressaoOrdemRequerida != pListaDeColunasParaImpressaoOrdemRequerida.end(); iteradorDeListaDeColunasParaImpressaoOrdemRequerida++)
    {

        for(XSV_tpIteradorDeListaDeColunasParaImpressao iteradorDeListaDeColunasParaImpressaoOrdemDoArquivo = pListaDeColunasParaImpressaoOrdemDoArquivo.begin(); iteradorDeListaDeColunasParaImpressaoOrdemDoArquivo != pListaDeColunasParaImpressaoOrdemDoArquivo.end(); iteradorDeListaDeColunasParaImpressaoOrdemDoArquivo++)
        {

            if ((int)(*iteradorDeListaDeColunasParaImpressaoOrdemDoArquivo).vPosicaoRequerida > (*iteradorDeListaDeColunasParaImpressaoOrdemRequerida).vPosicaoRequerida - vAjustaValoresSubtracao)
                (*iteradorDeListaDeColunasParaImpressaoOrdemDoArquivo).vPosicaoRequerida--;

        }

        vAjustaValoresSubtracao++;

    }

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
XSV_tpCondRet XSV_AcrescentarColunaCondicionalNaLista(XSV_tpListaDeColunasCondicionais *pListaDeColunasCondicionais, const char *pNomeDaColunaCondicional, void *vParametroCondicao, XSV_tpCondicaoDeColuna vCondicaoDaColuna)
{

    XSV_tpColunaCondicional vColunaCondicional;

    vColunaCondicional.pNomeDaColunaCondicional = pNomeDaColunaCondicional;
    vColunaCondicional.vParametroCondicao = vParametroCondicao;
    vColunaCondicional.vCondicaoDaColuna = vCondicaoDaColuna;

    (*pListaDeColunasCondicionais).push_back(vColunaCondicional);

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
XSV_tpCondRet XSV_AcrescentarColunaParaImpressaoNaLista(XSV_tpListaDeColunasParaImpressao *pListaDeColunasParaImpressao, const char *pNomeDaColunaParaImpressao, void (*funcaoDeTransformacao)(std::string), uint8_t vPosicaoRequerida)
{
    XSV_tpColunaParaImpressao vColunaParaImpressao;

    vColunaParaImpressao.pNomeDaColunaParaImpressao = pNomeDaColunaParaImpressao;
    vColunaParaImpressao.funcaoDeTransformacao = funcaoDeTransformacao;
    vColunaParaImpressao.vPosicaoRequerida = vPosicaoRequerida;

    (*pListaDeColunasParaImpressao).push_back(vColunaParaImpressao);

    return XSV_CondRetOK;
}
