# ProjetoDadosCVM
O programa busca, processa e organiza dados do site da CVM (http://dados.cvm.gov.br/dados/CIA_ABERTA/) para análise contábil de empresas cadastradas na CVM.

O bash script BuscaDados.sh busca os dados com wget, os descomprimem, convertem para ASCII e organizam pelo tipo do relatório, que pode ser:
- Balanço Patrimonial Ativo Consolidado
- Balanço Patrimonial Passivo Consolidado
- Demonstração de Fluxo de Caixa - Método Direto Consolidado
- Demonstração das Mutações do Patrimônio Líquido Consolidado
- Demonstração de Resultado Consolidado
- Demonstrações de Valor Adicionado Consolidado

Para alterar de consolidado para individual, basta alterar o script BuscaDados.sh.

O bash script ProcessaArquivos.sh utiliza o módulo XSV (que processa arquivos como CSVs e SSVs) para processar dados cadastrais.
Ele seleciona as colunas (dados do cadastro) e linhas (empresas) baseado nos parâmetros passados para as funções do programa /SOURCE/CODIGOS/PRINCIPAL_CAD.C.
Podemos, por exemplo, mostrar somente a CNPJ e denominação comercial de empresas em fase operacional, com classificação A e permissão para comercializar em Bolsa ou Balcão.
Estas qualidades podem ser alteradas no arquivo /SOURCE/CODIGOS/PRINCIPAL_CAD.C.

O script ProcessaArquivos.sh também utiliza o módulo XSV para processar dados contábeis anuais e trimestrais selecionando colunas e linhas especificadas nas chamadas de função do programa SOURCE/CODIGOS/PRINCIPAL_IC.C.
Podemos mostrar, por exemplo, somente a denominação social das empresas, período de exercício social e valor da conta de cada dado contábil.
Estas qualidades podem ser alteradas no arquivo /SOURCE/CODIGOS/PRINCIPAL_IC.C.

Por fim, script ProcessaArquivos.sh utiliza o programa /SOURCE/CODIGOS/ORGANIZA_EMPRESAS_SETORES.C para percorrer os dados processados anteriormente e organizá-los por setor, denominação da empresa e tipo do relatório, produzindo um diretório para cada setor de operação das empresas listadas por /SOURCE/CODIGOS/PRINCIPAL_CAD.C e, dentro de cada diretório setorial, diretórios representando cada empresa daquele setor com os dados trimestrais e anuais listadas por SOURCE/CODIGOS/PRINCIPAL_IC.C organizados pela data do periodo de exercício social.
Alterações em /SOURCE/CODIGOS/PRINCIPAL_CAD.C. e /SOURCE/CODIGOS/PRINCIPAL_IC.C. podem impedir a execução correta de /SOURCE/CODIGOS/ORGANIZA_EMPRESAS_SETORES.C.

Para executar todos stes passos de uma vez, basta navegar para o diretório onde se encontram os bash scripts e a pasta SOURCE e executar
$bash BuscaDados.sh && ProcessaArquivos.sh

Para executar novamente, não é preciso deletar qualquer arquivo produzida pela execução anterior. 

Os resultados de /SOURCE/CODIGOS/PRINCIPAL_CAD.C e /SOURCE/CODIGOS/PRINCIPAL_IC.C sobrescrevem os dados buscados no site da CVM. Os resultados de /SOURCE/CODIGOS/ORGANIZA_EMPRESAS_SETORES.C serão colocados INFORMACOES_CONTABEIS/SETORES e não alteram ou sobrescrevem os dados resultantes da execução de /SOURCE/CODIGOS/PRINCIPAL_CAD.C e /SOURCE/CODIGOS/PRINCIPAL_IC.C, podendo ser executado sem necessitar outra busca no site da CVM.
