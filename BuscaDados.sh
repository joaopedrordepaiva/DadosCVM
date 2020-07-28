echo ""

echo "Removendo diretório de destino das Informações Contábeis (IC)..."
rm -r INFORMACOES_CONTABEIS > /dev/null
echo "Diretório removido."
echo ""

echo "Criando diretório de destino das Informações Contábeis (IC)..."
mkdir INFORMACOES_CONTABEIS > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretórios de destino das Informações Contábeis Anuais (ICA), Informações Contábeis Trimestrais (ITR) e Informações Cadastrais (CAD)..."
mkdir INFORMACOES_CONTABEIS/ICA INFORMACOES_CONTABEIS/ITR INFORMACOES_CONTABEIS/CAD > /dev/null
echo "Diretórios criados."
echo ""

echo "Iniciando Processamento das Informações Contábeis Anuais (ICA)."
echo ""

echo "Criando diretório de destino do Balanço Patrimonial Ativo (BPA)..."
mkdir INFORMACOES_CONTABEIS/ICA/BPA > /dev/null
echo "Diretório criado."
echo ""

echo "Buscando Informações do Balanço Patrimonial Ativo no site da CVM..."
wget -P INFORMACOES_CONTABEIS/ICA/BPA/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/DOC/DFP/BPA/DADOS/
echo "Informações do Balanço Patrimonial Ativo foram copiadas para o diretório BPA."
echo ""

echo "Extraindo arquivos..."
for file in INFORMACOES_CONTABEIS/ICA/BPA/*.zip
do
    unzip -a -C -o -d "INFORMACOES_CONTABEIS/ICA/BPA/" "$file" "*con*"
    rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in INFORMACOES_CONTABEIS/ICA/BPA/*.csv
do
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ICA/BPA/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
    rm "$file"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

echo "Criando diretório de destino do Balanço Patrimonial Passivo (BPP)..."
mkdir INFORMACOES_CONTABEIS/ICA/BPP > /dev/null
echo "Diretório criado."
echo ""

echo "Buscando Informações do Balanço Patrimonial Passivo no site da CVM..."
wget -P INFORMACOES_CONTABEIS/ICA/BPP/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/DOC/DFP/BPP/DADOS/
echo "Informações do Balanço Patrimonial Passivo foram copiadas para o diretório BPP."
echo ""

echo "Extraindo arquivos..."
for file in INFORMACOES_CONTABEIS/ICA/BPP/*.zip
do
    unzip -a -C -o -d "INFORMACOES_CONTABEIS/ICA/BPP/" "$file" "*con*"
    rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in INFORMACOES_CONTABEIS/ICA/BPP/*.csv
do
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ICA/BPP/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
    rm "$file"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

echo "Criando diretório de destino das Demonstrações de Fluxo de Caixa - Método Direto (DFC_MD)..."
mkdir INFORMACOES_CONTABEIS/ICA/DFC_MD > /dev/null
echo "Diretório criado."
echo ""

echo "Buscando Informações das Demonstrações de Fluxo de Caixa - Método Direto no site da CVM..."
wget -P INFORMACOES_CONTABEIS/ICA/DFC_MD/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/DOC/DFP/DFC_MD/DADOS/
echo "Informações das Demonstrações de Fluxo de Caixa - Método Direto foram copiadas para o diretório DFC_MD."
echo ""

echo "Extraindo arquivos..."
for file in INFORMACOES_CONTABEIS/ICA/DFC_MD/*.zip
do
    unzip -a -C -o -d "INFORMACOES_CONTABEIS/ICA/DFC_MD/" "$file" "*con*"
    rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in INFORMACOES_CONTABEIS/ICA/DFC_MD/*.csv
do
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ICA/DFC_MD/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
    rm "$file"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

echo "Criando diretório de destino da Demonstração das Mutações do Patrimônio Líquido (DMPL)..."
mkdir INFORMACOES_CONTABEIS/ICA/DMPL > /dev/null
echo "Diretório criado."
echo ""

echo "Buscando Informações da Demonstração das Mutações do Patrimônio Líquido no site da CVM..."
wget -P INFORMACOES_CONTABEIS/ICA/DMPL/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/DOC/DFP/DMPL/DADOS/
echo "Informações da Demonstração das Mutações do Patrimônio Líquido foram copiadas para o diretório DMPL."
echo ""

echo "Extraindo arquivos..."
for file in INFORMACOES_CONTABEIS/ICA/DMPL/*.zip
do
    unzip -a -C -o -d "INFORMACOES_CONTABEIS/ICA/DMPL/" "$file" "*con*"
    rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in INFORMACOES_CONTABEIS/ICA/DMPL/*.csv
do
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ICA/DMPL/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
    rm "$file"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

echo "Criando diretório de destino do Demonstrativo de Resultados do Exercício (DRE)..."
mkdir INFORMACOES_CONTABEIS/ICA/DRE > /dev/null
echo "Diretório criado."
echo ""

echo "Buscando Informações do Demonstrativo de Resultados do Exercício no site da CVM..."
wget -P INFORMACOES_CONTABEIS/ICA/DRE/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/DOC/DFP/DRE/DADOS/
echo "Informações do Demonstrativo de Resultados do Exercício foram copiadas para o diretório DRE."
echo ""

echo "Extraindo arquivos..."
for file in INFORMACOES_CONTABEIS/ICA/DRE/*.zip
do
    unzip -a -C -o -d "INFORMACOES_CONTABEIS/ICA/DRE/" "$file" "*con*"
    rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in INFORMACOES_CONTABEIS/ICA/DRE/*.csv
do
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ICA/DRE/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
    rm "$file"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

echo "Criando diretório de destino da Demonstração do Valor Adicionado (DVA)..."
mkdir INFORMACOES_CONTABEIS/ICA/DVA > /dev/null
echo "Diretório criado."
echo ""

echo "Buscando Informações da Demonstração do Valor Adicionado no site da CVM..."
wget -P INFORMACOES_CONTABEIS/ICA/DVA/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/DOC/DFP/DVA/DADOS/
echo "Informações da Demonstração do Valor Adicionado foram copiadas para o diretório DVA."
echo ""

echo "Extraindo arquivos..."
for file in INFORMACOES_CONTABEIS/ICA/DVA/*.zip
do
    unzip -a -C -o -d "INFORMACOES_CONTABEIS/ICA/DVA/" "$file" "*con*"
    rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in INFORMACOES_CONTABEIS/ICA/DVA/*.csv
do
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ICA/DVA/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
    rm "$file"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

echo "Iniciando Processamento das Informações Contábeis Trimestrais (ITR)."
echo ""

echo "Criando diretório de destino do Balanço Patrimonial Ativo (BPA)..."
mkdir INFORMACOES_CONTABEIS/ITR/BPA > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino do Balanço Patrimonial Passivo (BPP)..."
mkdir INFORMACOES_CONTABEIS/ITR/BPP > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Fluxo de Caixa - Método Direto (DFC_MD)..."
mkdir INFORMACOES_CONTABEIS/ITR/DFC_MD > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações das Mutações do Patrimônio Líquido (DMPL)..."
mkdir INFORMACOES_CONTABEIS/ITR/DMPL > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Resultado (DRE)..."
mkdir INFORMACOES_CONTABEIS/ITR/DRE > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Valor Adicionado (DVA)..."
mkdir INFORMACOES_CONTABEIS/ITR/DVA > /dev/null
echo "Diretório criado."
echo ""

echo "Buscando Informações Contábeis Trimestrais no site da CVM..."
wget -P INFORMACOES_CONTABEIS/ITR/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/DOC/ITR/DADOS/
echo "Informações Trimestrais foram copiadas para o diretório ITR."
echo ""

echo "Extraindo arquivos..."
for file in INFORMACOES_CONTABEIS/ITR/*.zip
do
    unzip -a -C -o -d "INFORMACOES_CONTABEIS/ITR/" "$file" "*con*"
    rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII e os organizando..."
for file in INFORMACOES_CONTABEIS/ITR/*.csv
do
    case $file in
        
        *[Bb][Pp][Aa]*)
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ITR/BPA/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Bb][Pp][Pp]*)
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ITR/BPP/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Ff][Cc]?[Mm][Dd]*)
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ITR/DFC_MD/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Mm][Pp][Ll]*)
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ITR/DMPL/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Rr][Ee]*)
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ITR/DRE/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Vv][Aa]*)
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > INFORMACOES_CONTABEIS/ITR/DVA/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
    esac
    rm "$file"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

echo "Iniciando Processamento das Informações Cadastrais (CAD)."
echo ""

echo "Buscando Informações Cadastrais no site da CVM..."
wget -P INFORMACOES_CONTABEIS/CAD/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/CAD/DADOS/
echo "Informações Cadastrais foram copiadas para o diretório CAD."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in INFORMACOES_CONTABEIS/CAD/*.csv
do
    iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > "INFORMACOES_CONTABEIS/CAD/CAD.csv"
    rm "$file"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""
