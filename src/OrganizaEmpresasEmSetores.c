#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../lib/LISTA.h"

#define TAMVETOR 16

typedef enum enumCondRet
{

    CondRetOK, /* Concluiu corretamente */

    CondRetFaltouMemoria, /* Faltou memória para allocar */

    CondRetProblemaDeLista, /* Função de LIS não retornou condição de
    execução bem sucedida quando deveria */

    CondRetFalhaNaAberturaDoArquivo, /* Abertura de arquivo de input 
    falhou */

    CondRetFalhaNaAberturaDoDiretorio, /* Abertura do diretório falhou */

    CondRetDiretorioNaoFoiEcontrado, /* Diretorio buscado não foi encontrado
    na lista */

    CondRetEmpresaNaoFoiEcontrada /* Código de uma empresa específica não foi
    encontrado na lista */

} tpCondRet;

typedef struct LIS_tagLista *tppListaDeEmpresas;
typedef struct LIS_tagLista *tppListaDeDiretorios;
typedef struct LIS_tagLista *tppListaDeArquivos;

typedef struct tagEmpresa
{
    FILE *pArquivoEscrita;
    char *pSetor;
    char *pNome;
    unsigned int vCodigoCVM;

} * tppEmpresa;

typedef struct tagDiretorio
{
    char *pNomeDiretorio;
    tppListaDeArquivos pListaDeArquivos;

} * tppDiretorio;

typedef struct tagArquivo
{
    char *pNomeICA;
    char *pNomeITR;

} * tppArquivo;

void DestruirEmpresa(tppEmpresa pEmpresa)
{
    free(pEmpresa->pSetor);
    free(pEmpresa->pNome);
    free(pEmpresa);
}

tpCondRet AcrescentarEmpresaNaLista(tppListaDeEmpresas *ppListaDeEmpresas, tppEmpresa pEmpresa)
{
    if (!*ppListaDeEmpresas)
        if (LIS_CriarLista((void (*)(void *))DestruirEmpresa, ppListaDeEmpresas) != LIS_CondRetOK)
            return CondRetProblemaDeLista;

    if (LIS_InserirNoApos(*ppListaDeEmpresas, (void *)pEmpresa) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    return CondRetOK;
}

void DestruirDiretorio(tppDiretorio pDiretorio)
{
    free(pDiretorio->pNomeDiretorio);
    LIS_DestruirLista(pDiretorio->pListaDeArquivos);
    free(pDiretorio);
}

void DestruirArquivo(tppArquivo pArquivo)
{
    if (pArquivo->pNomeICA)
        free(pArquivo->pNomeICA);
    if (pArquivo->pNomeITR)
        free(pArquivo->pNomeITR);
    free(pArquivo);
}

tpCondRet AcrescentarDiretorioNaLista(tppListaDeDiretorios *ppListaDeDiretorios, tppDiretorio pDiretorio)
{
    if (!*ppListaDeDiretorios)
    {
        if (LIS_CriarLista((void (*)(void *))DestruirDiretorio, ppListaDeDiretorios) != LIS_CondRetOK)
            return CondRetProblemaDeLista;
    }

    if (LIS_InserirNoApos(*ppListaDeDiretorios, (void *)pDiretorio) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    return CondRetOK;
}

tpCondRet DestruirListaDeDiretorios(tppListaDeDiretorios pListaDeDiretorios)
{
    if (LIS_DestruirLista(pListaDeDiretorios) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    return CondRetOK;
}

tpCondRet AcrescentarArquivoICANaLista(tppListaDeArquivos *ppListaDeArquivos, const char *pNomeArquivo)
{
    LIS_tpCondRet vCondRetDeLista;
    tppArquivo pArquivoNovo, pArquivoComp;

    char *pNomeArquivoMalloc;
    if (!*ppListaDeArquivos)
    {
        if (LIS_CriarLista((void (*)(void *))DestruirArquivo, ppListaDeArquivos) != LIS_CondRetOK)
            return CondRetProblemaDeLista;
    }

    pNomeArquivoMalloc = (char *)malloc(strlen(pNomeArquivo) + 1);

    if (!pNomeArquivoMalloc)
        return CondRetFaltouMemoria;

    strcpy(pNomeArquivoMalloc, pNomeArquivo);

    pArquivoNovo = (tppArquivo)malloc(sizeof(struct tagArquivo));
    if (!pArquivoNovo)
        return CondRetFaltouMemoria;

    pArquivoNovo->pNomeICA = pNomeArquivoMalloc;
    pArquivoNovo->pNomeITR = NULL;

    vCondRetDeLista = LIS_IrParaPrimeiroNo(*ppListaDeArquivos);

    if (vCondRetDeLista == LIS_CondRetListaVazia)
    {
        if (LIS_InserirNoApos(*ppListaDeArquivos, (void *)pArquivoNovo) != LIS_CondRetOK)
            return CondRetProblemaDeLista;

        return CondRetOK;
    }

    else if (vCondRetDeLista != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    do
    {
        if (LIS_ObterConteudo(*ppListaDeArquivos, (void **)&pArquivoComp) != LIS_CondRetOK)
            return CondRetProblemaDeLista;

        if (strcmp(strstr(pArquivoComp->pNomeICA, "2"), strstr(pNomeArquivoMalloc, "2")) > 0)
        {
            if (LIS_InserirNoAntes(*ppListaDeArquivos, (void *)pArquivoNovo) != LIS_CondRetOK)
                return CondRetProblemaDeLista;
            if (LIS_IrParaPrimeiroNo(*ppListaDeArquivos) != LIS_CondRetOK)
                return CondRetProblemaDeLista;

            return CondRetOK;
        }

    } while (LIS_IrParaProximoNo(*ppListaDeArquivos) == LIS_CondRetOK);

    if (LIS_InserirNoApos(*ppListaDeArquivos, (void *)pArquivoNovo) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    if (LIS_IrParaPrimeiroNo(*ppListaDeArquivos) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    return CondRetOK;
}

tpCondRet AcrescentarArquivoITRNaLista(tppListaDeArquivos *ppListaDeArquivos, const char *pNomeArquivo)
{
    tppArquivo pArquivo;

    char *pNomeArquivoMalloc;

    int vResultadoComp;

    if (!*ppListaDeArquivos)
    {
        return CondRetProblemaDeLista;
    }

    pNomeArquivoMalloc = (char *)malloc(strlen(pNomeArquivo) + 1);

    if (!pNomeArquivoMalloc)
        return CondRetFaltouMemoria;

    strcpy(pNomeArquivoMalloc, pNomeArquivo);

    if (LIS_IrParaPrimeiroNo(*ppListaDeArquivos) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    do
    {
        if (LIS_ObterConteudo(*ppListaDeArquivos, (void **)&pArquivo) != LIS_CondRetOK)
            return CondRetProblemaDeLista;

        vResultadoComp = strcmp(strstr(pArquivo->pNomeICA, "2"), strstr(pNomeArquivoMalloc, "2"));

        if (vResultadoComp > 0)
        {
            pArquivo = (tppArquivo)malloc(sizeof(struct tagArquivo));
            if (!pArquivo)
                return CondRetFaltouMemoria;

            pArquivo->pNomeICA = NULL;
            pArquivo->pNomeITR = pNomeArquivoMalloc;

            if (LIS_InserirNoAntes(*ppListaDeArquivos, (void *)pArquivo) != LIS_CondRetOK)
                return CondRetProblemaDeLista;
            if (LIS_IrParaPrimeiroNo(*ppListaDeArquivos) != LIS_CondRetOK)
                return CondRetProblemaDeLista;

            return CondRetOK;
        }

        else if (!vResultadoComp)
        {
            pArquivo->pNomeITR = pNomeArquivoMalloc;
            if (LIS_IrParaPrimeiroNo(*ppListaDeArquivos) != LIS_CondRetOK)
                return CondRetProblemaDeLista;

            return CondRetOK;
        }

    } while (LIS_IrParaProximoNo(*ppListaDeArquivos) == LIS_CondRetOK);

    pArquivo = (tppArquivo)malloc(sizeof(struct tagArquivo));
    if (!pArquivo)
        return CondRetFaltouMemoria;

    pArquivo->pNomeICA = NULL;
    pArquivo->pNomeITR = pNomeArquivoMalloc;

    if (LIS_InserirNoApos(*ppListaDeArquivos, (void *)pArquivo) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    if (LIS_IrParaPrimeiroNo(*ppListaDeArquivos) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    return CondRetOK;
}

tpCondRet DestruirTabelaHashDeEmpresas(tppListaDeEmpresas *pTabelaHashDeEmpresas)
{
    for (int i = 0; i < TAMVETOR; i++)
        if (pTabelaHashDeEmpresas[i])
            if (LIS_DestruirLista(pTabelaHashDeEmpresas[i]) != LIS_CondRetOK)
                return CondRetProblemaDeLista;

    return CondRetOK;
}

tpCondRet ImprimirListaDeEmpresas(tppListaDeEmpresas pListaDeEmpresas)
{
    tppEmpresa pEmpresa;

    if (LIS_IrParaPrimeiroNo(pListaDeEmpresas) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    do
    {
        if (LIS_ObterConteudo(pListaDeEmpresas, (void **)&pEmpresa) != LIS_CondRetOK)
            return CondRetProblemaDeLista;

        printf("%u;%s;%s\n", pEmpresa->vCodigoCVM, pEmpresa->pSetor, pEmpresa->pNome);

    } while (LIS_IrParaProximoNo(pListaDeEmpresas) == LIS_CondRetOK);

    if (LIS_IrParaPrimeiroNo(pListaDeEmpresas) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    return CondRetOK;
}

unsigned char SomaReduzidaStringNum(const char *pInicioString)
{
    unsigned char vSoma;

    vSoma = (unsigned char)0;

    for (int i = 0; pInicioString[i]; i++)
        vSoma += pInicioString[i] - '0';

    return vSoma;
}

tpCondRet BuscarNaListaDeEmpresas(tppListaDeEmpresas pListaDeEmpresas, unsigned int vCodigoCVM, tppEmpresa *ppEmpresa)
{
    if (LIS_IrParaPrimeiroNo(pListaDeEmpresas) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    do
    {
        if (LIS_ObterConteudo(pListaDeEmpresas, (void **)ppEmpresa) != LIS_CondRetOK)
            return CondRetProblemaDeLista;

        if ((*ppEmpresa)->vCodigoCVM == vCodigoCVM)
        {
            if (LIS_IrParaPrimeiroNo(pListaDeEmpresas) != LIS_CondRetOK)
                return CondRetProblemaDeLista;

            return CondRetOK;
        }

    } while (LIS_IrParaProximoNo(pListaDeEmpresas) == LIS_CondRetOK);

    if (LIS_IrParaPrimeiroNo(pListaDeEmpresas) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    return CondRetEmpresaNaoFoiEcontrada;
}

tpCondRet FecharPonteirosParaArquivos(tppListaDeEmpresas *pTabelaHashDeEmpresas)
{

    tppListaDeEmpresas pListaDeEmpresas;
    tppEmpresa pEmpresa;

    FILE *pArquivo;

    for (int i = 0; i < TAMVETOR; i++)
    {

        pListaDeEmpresas = pTabelaHashDeEmpresas[i];

        if (LIS_IrParaPrimeiroNo(pListaDeEmpresas) != LIS_CondRetOK)
            return CondRetProblemaDeLista;

        do
        {
            if (LIS_ObterConteudo(pListaDeEmpresas, (void **)&pEmpresa) != LIS_CondRetOK)
                return CondRetProblemaDeLista;

            pArquivo = pEmpresa->pArquivoEscrita;

            if (pArquivo)
            {
                fclose(pArquivo);
                pEmpresa->pArquivoEscrita = NULL;
            }

        } while (LIS_IrParaProximoNo(pListaDeEmpresas) == LIS_CondRetOK);

        if (LIS_IrParaPrimeiroNo(pListaDeEmpresas) != LIS_CondRetOK)
            return CondRetProblemaDeLista;
    }

    return CondRetOK;
}

tpCondRet TratarArquivoICAeITR(const char *pCaminhoArquivoICA, const char *pCaminhoArquivoITR, tppListaDeEmpresas *pTabelaHashDeEmpresas, const char *pNomeFormulario)
{
    tpCondRet vCondRet;

    char pLinhaDoArquivoDeInputICA[1024], pLinhaDoArquivoDeInputITR[1024], cwd[256];

    tppEmpresa pEmpresa;

    FILE *pArquivoICA, *pArquivoITR;

    char *pCaminhaPorString;

    long vPosicaoDoArquivoITR;

    int i, vEncontrouNoITR;

    unsigned int vCodigoCVMICAAntigo, vCodigoCVMICANovo, vCodigoCVMITRNovo, vSomaCodigoCVM;

    if (!(pArquivoICA = fopen(pCaminhoArquivoICA, "r")))
        return CondRetFalhaNaAberturaDoArquivo;

    if (!(pArquivoITR = fopen(pCaminhoArquivoITR, "r")))
        return CondRetFalhaNaAberturaDoArquivo;

    fgets(pLinhaDoArquivoDeInputICA, 1024, pArquivoICA);
    fgets(pLinhaDoArquivoDeInputICA, 1024, pArquivoITR);

    vCodigoCVMICAAntigo = 0;

    while (fgets(pLinhaDoArquivoDeInputICA, 1024, pArquivoICA))
    {
        for (i = 0; pLinhaDoArquivoDeInputICA[i] != ';'; i++)
            ;

        pLinhaDoArquivoDeInputICA[i] = '\0';

        vSomaCodigoCVM = SomaReduzidaStringNum(pLinhaDoArquivoDeInputICA);

        vCodigoCVMICANovo = strtoul(pLinhaDoArquivoDeInputICA, &pCaminhaPorString, 10);

        pLinhaDoArquivoDeInputICA[i] = ';';

        if (vCodigoCVMICANovo != vCodigoCVMICAAntigo)
        {

            vCondRet = BuscarNaListaDeEmpresas(pTabelaHashDeEmpresas[vSomaCodigoCVM % TAMVETOR], vCodigoCVMICANovo, &pEmpresa);

            if (vCondRet == CondRetEmpresaNaoFoiEcontrada)
                continue;

            else if (vCondRet != CondRetOK)
            {
                fclose(pArquivoITR);
                fclose(pArquivoICA);
                return vCondRet;
            }

            if (!(pEmpresa->pArquivoEscrita))
            {
                mkdir(strcat(strcpy(cwd, "InformacoesContabeis/Setores/"), pEmpresa->pSetor), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                mkdir(strcat(strcat(cwd, "/"), pEmpresa->pNome), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                pEmpresa->pArquivoEscrita = fopen(strcat(strcat(strcat(cwd, "/"), pNomeFormulario), ".csv"), "a");
                if (!(pEmpresa->pArquivoEscrita))
                    return CondRetFalhaNaAberturaDoArquivo;
            }

            vEncontrouNoITR = 0;
            vPosicaoDoArquivoITR = ftell(pArquivoITR);
            while (fgets(pLinhaDoArquivoDeInputITR, 1024, pArquivoITR))
            {
                for (i = 0; pLinhaDoArquivoDeInputITR[i] != ';'; i++)
                    ;

                pLinhaDoArquivoDeInputITR[i] = '\0';

                vCodigoCVMITRNovo = strtoul(pLinhaDoArquivoDeInputITR, &pCaminhaPorString, 10);

                pLinhaDoArquivoDeInputITR[i] = ';';

                if (vCodigoCVMITRNovo != vCodigoCVMICANovo && vEncontrouNoITR)
                    break;

                else if (vCodigoCVMITRNovo == vCodigoCVMICANovo)
                {
                    vEncontrouNoITR = 1;
                    fprintf(pEmpresa->pArquivoEscrita, "%s", pLinhaDoArquivoDeInputITR);
                }
            }

            if (vEncontrouNoITR)
                fseek(pArquivoITR, -1 * sizeof(pLinhaDoArquivoDeInputITR), SEEK_CUR);

            else
                fseek(pArquivoITR, vPosicaoDoArquivoITR, SEEK_SET);
        }

        vCodigoCVMICAAntigo = vCodigoCVMICANovo;
        fprintf(pEmpresa->pArquivoEscrita, "%s", pLinhaDoArquivoDeInputICA);
    }

    fclose(pArquivoITR);
    fclose(pArquivoICA);
    return CondRetOK;
}

tpCondRet TratarArquivoICAouITR(const char *pCaminhoArquivo, tppListaDeEmpresas *pTabelaHashDeEmpresas, const char *pNomeFormulario)
{
    tpCondRet vCondRet;

    char pLinhaDoArquivoDeInput[1024], cwd[256];

    tppEmpresa pEmpresa;

    FILE *pArquivo;

    char *pCaminhaPorString;

    int i;

    unsigned int vCodigoCVMAntigo, vCodigoCVMNovo, vSomaCodigoCVM;

    if (!(pArquivo = fopen(pCaminhoArquivo, "r")))
        return CondRetFalhaNaAberturaDoArquivo;

    fgets(pLinhaDoArquivoDeInput, 1024, pArquivo);

    vCodigoCVMAntigo = 0;

    while (fgets(pLinhaDoArquivoDeInput, 1024, pArquivo))
    {
        for (i = 0; pLinhaDoArquivoDeInput[i] != ';'; i++)
            ;

        pLinhaDoArquivoDeInput[i] = '\0';

        vSomaCodigoCVM = SomaReduzidaStringNum(pLinhaDoArquivoDeInput);

        vCodigoCVMNovo = strtoul(pLinhaDoArquivoDeInput, &pCaminhaPorString, 10);

        pLinhaDoArquivoDeInput[i] = ';';

        if (vCodigoCVMNovo != vCodigoCVMAntigo)
        {

            vCondRet = BuscarNaListaDeEmpresas(pTabelaHashDeEmpresas[vSomaCodigoCVM % TAMVETOR], vCodigoCVMNovo, &pEmpresa);

            if (vCondRet == CondRetEmpresaNaoFoiEcontrada)
                continue;

            else if (vCondRet != CondRetOK)
            {
                fclose(pArquivo);
                return vCondRet;
            }

            if (!(pEmpresa->pArquivoEscrita))
            {
                mkdir(strcat(strcpy(cwd, "InformacoesContabeis/Setores/"), pEmpresa->pSetor), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                mkdir(strcat(strcat(cwd, "/"), pEmpresa->pNome), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                pEmpresa->pArquivoEscrita = fopen(strcat(strcat(strcat(cwd, "/"), pNomeFormulario), ".csv"), "a");
                if (!(pEmpresa->pArquivoEscrita))
                    return CondRetFalhaNaAberturaDoArquivo;
            }
        }

        vCodigoCVMAntigo = vCodigoCVMNovo;
        fprintf(pEmpresa->pArquivoEscrita, "%s", pLinhaDoArquivoDeInput);
    }

    fclose(pArquivo);
    return CondRetOK;
}

tpCondRet ProcessarArquivos(tppListaDeDiretorios pListaDeDiretorios, tppListaDeEmpresas *pTabelaHashDeEmpresas)
{
    tpCondRet vCondRet;

    tppDiretorio pDiretorio;
    tppArquivo pArquivo;
    tppListaDeArquivos pListaDeArquivos;

    if (LIS_IrParaPrimeiroNo(pListaDeDiretorios) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    do
    {
        if (LIS_ObterConteudo(pListaDeDiretorios, (void **)&pDiretorio) != LIS_CondRetOK)
            return CondRetProblemaDeLista;

        pListaDeArquivos = pDiretorio->pListaDeArquivos;

        if (LIS_IrParaPrimeiroNo(pListaDeArquivos) != LIS_CondRetOK)
            return CondRetProblemaDeLista;

        do
        {

            if (LIS_ObterConteudo(pListaDeArquivos, (void **)&pArquivo) != LIS_CondRetOK)
                return CondRetProblemaDeLista;

            if (pArquivo->pNomeICA && pArquivo->pNomeITR)
            {
                vCondRet = TratarArquivoICAeITR(pArquivo->pNomeICA, pArquivo->pNomeITR, pTabelaHashDeEmpresas, pDiretorio->pNomeDiretorio);

                if (vCondRet != CondRetOK)
                    return vCondRet;
            }

            else if (pArquivo->pNomeICA)
            {
                vCondRet = TratarArquivoICAouITR(pArquivo->pNomeICA, pTabelaHashDeEmpresas, pDiretorio->pNomeDiretorio);

                if (vCondRet != CondRetOK)
                    return vCondRet;
            }

            else
            {
                vCondRet = TratarArquivoICAouITR(pArquivo->pNomeITR, pTabelaHashDeEmpresas, pDiretorio->pNomeDiretorio);

                if (vCondRet != CondRetOK)
                    return vCondRet;
            }

        } while (LIS_IrParaProximoNo(pListaDeArquivos) == LIS_CondRetOK);

        vCondRet = FecharPonteirosParaArquivos(pTabelaHashDeEmpresas);

        if (vCondRet != CondRetOK)
            return vCondRet;

    } while (LIS_IrParaProximoNo(pListaDeDiretorios) == LIS_CondRetOK);

    return CondRetOK;
}

tpCondRet CriarListaDeDiretoriosICA(const char *pCaminhoDir, tppListaDeDiretorios *ppListaDeDiretorios)
{
    char cwd[256];

    tpCondRet vCondRet;

    tppDiretorio pDiretorio;

    DIR *pDir;
    struct dirent *dirp;

    pDiretorio = NULL;

    if (!(pDir = opendir(pCaminhoDir)))
        return CondRetFalhaNaAberturaDoDiretorio;

    while ((dirp = readdir(pDir)))
    {
        strcat(strcat(strcpy(cwd, pCaminhoDir), "/"), dirp->d_name);
        if (dirp->d_type == DT_DIR && !strstr(dirp->d_name, "."))
        {
            pDiretorio = (tppDiretorio)malloc(sizeof(struct tagDiretorio));

            if (!pDiretorio)
                return CondRetFaltouMemoria;

            pDiretorio->pNomeDiretorio = (char *)malloc(strlen(dirp->d_name) + 1);

            if (!pDiretorio->pNomeDiretorio)
                return CondRetFaltouMemoria;

            strcpy(pDiretorio->pNomeDiretorio, dirp->d_name);
            pDiretorio->pListaDeArquivos = NULL;

            vCondRet = AcrescentarDiretorioNaLista(ppListaDeDiretorios, pDiretorio);
            if (vCondRet != CondRetOK)
                return vCondRet;

            vCondRet = CriarListaDeDiretoriosICA(cwd, ppListaDeDiretorios);
            if (vCondRet != CondRetOK)
                return vCondRet;
        }
        else if (dirp->d_type == DT_REG)
        {
            if (!pDiretorio)
                if (LIS_ObterConteudo(*ppListaDeDiretorios, (void **)&pDiretorio) != LIS_CondRetOK)
                    return CondRetProblemaDeLista;

            vCondRet = AcrescentarArquivoICANaLista(&(pDiretorio->pListaDeArquivos), cwd);
            if (vCondRet != CondRetOK)
                return vCondRet;
        }
    }

    closedir(pDir);
    return CondRetOK;
}

tpCondRet BuscarDiretorioNaLista(tppListaDeDiretorios pListaDeDiretorios, const char *pNomeDir)
{
    tppDiretorio pDiretorio;

    if (LIS_IrParaPrimeiroNo(pListaDeDiretorios) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    do
    {
        if (LIS_ObterConteudo(pListaDeDiretorios, (void **)&pDiretorio) != LIS_CondRetOK)
            return CondRetProblemaDeLista;

        if (!strcmp(pDiretorio->pNomeDiretorio, pNomeDir))
            return CondRetOK;

    } while (LIS_IrParaProximoNo(pListaDeDiretorios) == LIS_CondRetOK);

    if (LIS_IrParaPrimeiroNo(pListaDeDiretorios) != LIS_CondRetOK)
        return CondRetProblemaDeLista;

    return CondRetDiretorioNaoFoiEcontrado;
}

tpCondRet CriarListaDeDiretoriosITR(const char *pCaminhoDir, tppListaDeDiretorios *ppListaDeDiretorios)
{
    char cwd[256];

    tpCondRet vCondRet;

    tppDiretorio pDiretorio;

    DIR *pDir;
    struct dirent *dirp;

    pDiretorio = NULL;

    if (!(pDir = opendir(pCaminhoDir)))
        return CondRetFalhaNaAberturaDoDiretorio;

    while ((dirp = readdir(pDir)))
    {
        strcat(strcat(strcpy(cwd, pCaminhoDir), "/"), dirp->d_name);
        if (dirp->d_type == DT_DIR && !strstr(dirp->d_name, "."))
        {
            vCondRet = BuscarDiretorioNaLista(*ppListaDeDiretorios, dirp->d_name);
            if (vCondRet != CondRetOK)
                return vCondRet;

            vCondRet = CriarListaDeDiretoriosITR(cwd, ppListaDeDiretorios);
            if (vCondRet != CondRetOK)
                return vCondRet;
        }
        else if (dirp->d_type == DT_REG)
        {
            if (!pDiretorio)
                if (LIS_ObterConteudo(*ppListaDeDiretorios, (void **)&pDiretorio) != LIS_CondRetOK)
                    return CondRetProblemaDeLista;

            vCondRet = AcrescentarArquivoITRNaLista(&(pDiretorio->pListaDeArquivos), cwd);
            if (vCondRet != CondRetOK)
                return vCondRet;
        }
    }

    closedir(pDir);
    return CondRetOK;
}

int main(int argc, char **argv)
{
    char pLinhaDoArquivoDeInput[1024];

    tppListaDeEmpresas pTabelaHashDeEmpresas[TAMVETOR];

    tppListaDeDiretorios pListaDeDiretorios;

    tppEmpresa pEmpresa;

    FILE *pArquivo;

    unsigned int vCodigoCVM, vSomaCodigoCVM;

    int i;

    char *pCaminhaPorString, *pSetor, *pNome;

    mkdir("InformacoesContabeis/Setores", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    pArquivo = fopen("InformacoesContabeis/CAD/CAD.csv", "r");

    if (!pArquivo)
    {
        fprintf(stderr, "Arquivo de informações cadastrais não pôde ser aberto.\n");
        return 1;
    }

    for (i = 0; i < TAMVETOR; i++)
        pTabelaHashDeEmpresas[i] = NULL;

    fgets(pLinhaDoArquivoDeInput, 1024, pArquivo);

    while (fgets(pLinhaDoArquivoDeInput, 1024, pArquivo))
    {
        for (i = 0; pLinhaDoArquivoDeInput[i] != ';'; i++)
            ;

        pLinhaDoArquivoDeInput[i] = '\0';

        vSomaCodigoCVM = SomaReduzidaStringNum(pLinhaDoArquivoDeInput);

        vCodigoCVM = strtoul(pLinhaDoArquivoDeInput, &pCaminhaPorString, 10);

        pEmpresa = (tppEmpresa)malloc(sizeof(struct tagEmpresa));
        if (!pEmpresa)
        {
            fprintf(stderr, "Problema na alocação da empresa %d.\n", vCodigoCVM);
            return 1;
        }

        pEmpresa->vCodigoCVM = vCodigoCVM;

        pCaminhaPorString++;

        for (i = 0; pCaminhaPorString[i] != ';'; i++)
            ;

        pCaminhaPorString[i] = '\0';

        pSetor = (char *)malloc(i + 1);
        if (!pSetor)
        {
            fprintf(stderr, "Problema na alocação do setor da empresa %d.\n", vCodigoCVM);
            return 1;
        }

        strcpy(pSetor, pCaminhaPorString);

        pEmpresa->pSetor = pSetor;

        pCaminhaPorString += i + 1;

        for (i = 0; pCaminhaPorString[i] && pCaminhaPorString[i] != '\n'; i++)
            ;

        pCaminhaPorString[i] = '\0';

        pNome = (char *)malloc(i + 1);
        if (!pSetor)
        {
            fprintf(stderr, "Problema na alocação do nome da empresa %d.\n", vCodigoCVM);
            return 1;
        }

        strcpy(pNome, pCaminhaPorString);

        pEmpresa->pNome = pNome;

        pEmpresa->pArquivoEscrita = NULL;

        if (AcrescentarEmpresaNaLista(pTabelaHashDeEmpresas + (vSomaCodigoCVM % TAMVETOR), pEmpresa) != CondRetOK)
        {
            fprintf(stderr, "Não foi possível acrescentar empresa %d na lista.\n", vCodigoCVM);
            DestruirTabelaHashDeEmpresas(pTabelaHashDeEmpresas);
            return 1;
        }
    }

    fclose(pArquivo);

    pListaDeDiretorios = NULL;

    if (CriarListaDeDiretoriosICA("InformacoesContabeis/ICA", &pListaDeDiretorios) != CondRetOK)
    {
        fprintf(stderr, "Não foi possível criar lista de diretórios ICA.\n");
        DestruirTabelaHashDeEmpresas(pTabelaHashDeEmpresas);
        DestruirListaDeDiretorios(pListaDeDiretorios);
        return 1;
    }

    if (CriarListaDeDiretoriosITR("InformacoesContabeis/ITR", &pListaDeDiretorios) != CondRetOK)
    {
        fprintf(stderr, "Não foi possível criar lista de diretórios ITR.\n");
        DestruirTabelaHashDeEmpresas(pTabelaHashDeEmpresas);
        DestruirListaDeDiretorios(pListaDeDiretorios);
        return 1;
    }

    if (ProcessarArquivos(pListaDeDiretorios, pTabelaHashDeEmpresas) != CondRetOK)
    {
        fprintf(stderr, "Não foi possível processar arquivos da lista de diretórios.\n");
        DestruirTabelaHashDeEmpresas(pTabelaHashDeEmpresas);
        DestruirListaDeDiretorios(pListaDeDiretorios);
        return 1;
    }

    if (DestruirTabelaHashDeEmpresas(pTabelaHashDeEmpresas) != CondRetOK)
    {
        fprintf(stderr, "Problema na destruição da tabela hash de empresas.\n");
        DestruirListaDeDiretorios(pListaDeDiretorios);
        return 1;
    }

    if (DestruirListaDeDiretorios(pListaDeDiretorios) != CondRetOK)
    {
        fprintf(stderr, "Problema na destruição da lista de diretórios.\n");
        return 1;
    }

    return 0;
}