echo ""

echo "Removendo diretório de destino das Informações Contábeis (IC)..."
rm -r InformacoesContabeis > /dev/null
echo "Diretório removido."
echo ""

echo "Criando diretório de destino das Informações Contábeis (IC)..."
mkdir InformacoesContabeis > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretórios de destino das Informações Contábeis Anuais (ICA), Informações Contábeis Trimestrais (ITR) e Informações Cadastrais (CAD)..."
mkdir InformacoesContabeis/ICA InformacoesContabeis/ITR InformacoesContabeis/CAD > /dev/null
echo "Diretórios criados."
echo ""

echo "Iniciando Processamento das Informações Contábeis Anuais (ICA)."
echo ""

echo "Criando diretório de destino do Balanço Patrimonial Ativo (BPA)..."
mkdir InformacoesContabeis/ICA/BPA > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino do Balanço Patrimonial Passivo (BPP)..."
mkdir InformacoesContabeis/ICA/BPP > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Fluxo de Caixa - Método Direto (DFC_MD)..."
mkdir InformacoesContabeis/ICA/DFC_MD > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Fluxo de Caixa - Método Indireto (DFC_MI)..."
mkdir InformacoesContabeis/ICA/DFC_MI > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações das Mutações do Patrimônio Líquido (DMPL)..."
mkdir InformacoesContabeis/ICA/DMPL > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Resultado (DRE)..."
mkdir InformacoesContabeis/ICA/DRE > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Valor Adicionado (DVA)..."
mkdir InformacoesContabeis/ICA/DVA > /dev/null
echo "Diretório criado."
echo ""

echo "Buscando Informações Contábeis Anuais no site da CVM..."
wget -P InformacoesContabeis/ICA/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/DOC/DFP/DADOS/
echo "Informações Anuais foram copiadas para o diretório ICA."
echo ""

echo "Extraindo arquivos..."
for file in InformacoesContabeis/ICA/*.zip
do
    unzip -a -C -o -d "InformacoesContabeis/ICA/" "$file" "*con*"
    rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo, sanitizando e organizando arquivos..."
for file in InformacoesContabeis/ICA/*.csv
do
    case $file in
        
        *[Bb][Pp][Aa]*)
    mv "$file" InformacoesContabeis/ICA/BPA/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Bb][Pp][Pp]*)
    mv "$file" InformacoesContabeis/ICA/BPP/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Ff][Cc]?[Mm][Dd]*)
    mv "$file" InformacoesContabeis/ICA/DFC_MD/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;

        *[Dd][Ff][Cc]?[Mm][Ii]*)
    mv "$file" InformacoesContabeis/ICA/DFC_MI/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Mm][Pp][Ll]*)
    mv "$file" InformacoesContabeis/ICA/DMPL/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Rr][Ee]*)
    mv "$file" InformacoesContabeis/ICA/DRE/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Vv][Aa]*)
    mv "$file" InformacoesContabeis/ICA/DVA/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
    esac
done > /dev/null
echo "Arquivos foram organizados."
echo ""

echo "Iniciando Processamento das Informações Contábeis Trimestrais (ITR)."
echo ""

echo "Criando diretório de destino do Balanço Patrimonial Ativo (BPA)..."
mkdir InformacoesContabeis/ITR/BPA > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino do Balanço Patrimonial Passivo (BPP)..."
mkdir InformacoesContabeis/ITR/BPP > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Fluxo de Caixa - Método Direto (DFC_MD)..."
mkdir InformacoesContabeis/ITR/DFC_MD > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Fluxo de Caixa - Método Indireto (DFC_MI)..."
mkdir InformacoesContabeis/ITR/DFC_MI > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações das Mutações do Patrimônio Líquido (DMPL)..."
mkdir InformacoesContabeis/ITR/DMPL > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Resultado (DRE)..."
mkdir InformacoesContabeis/ITR/DRE > /dev/null
echo "Diretório criado."
echo ""

echo "Criando diretório de destino das Demonstrações de Valor Adicionado (DVA)..."
mkdir InformacoesContabeis/ITR/DVA > /dev/null
echo "Diretório criado."
echo ""

echo "Buscando Informações Contábeis Trimestrais no site da CVM..."
wget -P InformacoesContabeis/ITR/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/DOC/ITR/DADOS/
echo "Informações Trimestrais foram copiadas para o diretório ITR."
echo ""

echo "Extraindo arquivos..."
for file in InformacoesContabeis/ITR/*.zip
do
    unzip -a -C -o -d "InformacoesContabeis/ITR/" "$file" "*con*"
    rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo, sanitizando e organizando arquivos..."
for file in InformacoesContabeis/ITR/*.csv
do
    case $file in
        
        *[Bb][Pp][Aa]*)
    mv "$file" InformacoesContabeis/ITR/BPA/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Bb][Pp][Pp]*)
    mv "$file" InformacoesContabeis/ITR/BPP/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Ff][Cc]?[Mm][Dd]*)
    mv "$file" InformacoesContabeis/ITR/DFC_MD/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;

        *[Dd][Ff][Cc]?[Mm][Ii]*)
    mv "$file" InformacoesContabeis/ITR/DFC_MI/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Mm][Pp][Ll]*)
    mv "$file" InformacoesContabeis/ITR/DMPL/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Rr][Ee]*)
    mv "$file" InformacoesContabeis/ITR/DRE/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
        
        *[Dd][Vv][Aa]*)
    mv "$file" InformacoesContabeis/ITR/DVA/`expr match "$file" '.*\(2[0-9][0-9][0-9]\)'`.csv
        ;;
    esac
done > /dev/null
echo "Arquivos foram organizados."
echo ""

echo "Iniciando Processamento das Informações Cadastrais (CAD)."
echo ""

echo "Buscando Informações Cadastrais no site da CVM..."
wget -P InformacoesContabeis/CAD/ -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" http://dados.cvm.gov.br/dados/CIA_ABERTA/CAD/DADOS/
echo "Informações Cadastrais foram copiadas para o diretório CAD."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in InformacoesContabeis/CAD/*.csv
do
    mv "$file" "InformacoesContabeis/CAD/CAD.csv"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""
