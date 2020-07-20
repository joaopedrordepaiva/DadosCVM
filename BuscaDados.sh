echo ""

echo "Removendo diretório de destino das Informações Contábeis (IC)..."
rm -r INFORMACOES_CONTABEIS > /dev/null
echo "Diretório removido."
echo ""

echo "Criando diretório de destino das Informações Contábeis (IC)..."
mkdir INFORMACOES_CONTABEIS > /dev/null
echo "Diretório criado."
echo ""

cd INFORMACOES_CONTABEIS > /dev/null

echo "Criando diretórios de destino das Informações Contábeis Anuais (ICA), Informações Contábeis Trimestrais (ITR) e Informações Cadastrais (CAD)..."
mkdir ICA ITR CAD > /dev/null
echo "Diretórios criados."
echo ""

echo "Iniciando Processamento das Informações Contábeis Anuais (ICA)."
echo ""

cd ICA > /dev/null

echo "Criando diretório de destino do Balanço Patrimonial Ativo (BPA)..."
mkdir BPA > /dev/null
echo "Diretório criado."
echo ""

cd BPA > /dev/null

echo "Buscando Informações do Balanço Patrimonial Ativo no site da CVM..."
wget -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" localhost/DOC/DFP/BPA/DADOS/
echo "Informações do Balanço Patrimonial Ativo foram copiadas para o diretório BPA."
echo ""

echo "Extraindo arquivos..."
for file in *.zip; do
unzip "$file" '*con*'
rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in * ; do
iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > "convertido.tmp"
rm "$file"
mv -- "convertido.tmp" "$(tr [:lower:] [:upper:] <<< "$file")"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

cd .. > /dev/null

echo "Criando diretório de destino do Balanço Patrimonial Passivo (BPP)..."
mkdir BPP > /dev/null
echo "Diretório criado."
echo ""

cd BPP > /dev/null

echo "Buscando Informações do Balanço Patrimonial Passivo no site da CVM..."
wget -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" localhost/DOC/DFP/BPP/DADOS/
echo "Informações do Balanço Patrimonial Passivo foram copiadas para o diretório BPP."
echo ""

echo "Extraindo arquivos..."
for file in *.zip; do
unzip "$file" '*con*'
rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in * ; do
iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > "convertido.tmp"
rm "$file"
mv -- "convertido.tmp" "$(tr [:lower:] [:upper:] <<< "$file")"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

cd .. > /dev/null

echo "Criando diretório de destino das Demonstrações de Fluxo de Caixa - Método Direto (DFC_MD)..."
mkdir DFC_MD > /dev/null
echo "Diretório criado."
echo ""

cd DFC_MD > /dev/null

echo "Buscando Informações das Demonstrações de Fluxo de Caixa - Método Direto no site da CVM..."
wget -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" localhost/DOC/DFP/DFC_MD/DADOS/
echo "Informações das Demonstrações de Fluxo de Caixa - Método Direto foram copiadas para o diretório DFC_MD."
echo ""

echo "Extraindo arquivos..."
for file in *.zip; do
unzip "$file" '*con*'
rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in * ; do
iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > "convertido.tmp"
rm "$file"
mv -- "convertido.tmp" "$(tr [:lower:] [:upper:] <<< "$file")"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

cd .. > /dev/null

echo "Criando diretório de destino da Demonstração das Mutações do Patrimônio Líquido (DMPL)..."
mkdir DMPL > /dev/null
echo "Diretório criado."
echo ""

cd DMPL > /dev/null

echo "Buscando Informações da Demonstração das Mutações do Patrimônio Líquido no site da CVM..."
wget -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" localhost/DOC/DFP/DMPL/DADOS/
echo "Informações da Demonstração das Mutações do Patrimônio Líquido foram copiadas para o diretório DMPL."
echo ""

echo "Extraindo arquivos..."
for file in *.zip; do
unzip "$file" '*con*'
rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in * ; do
iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > "convertido.tmp"
rm "$file"
mv -- "convertido.tmp" "$(tr [:lower:] [:upper:] <<< "$file")"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

cd .. > /dev/null

echo "Criando diretório de destino do Demonstrativo de Resultados do Exercício (DRE)..."
mkdir DRE > /dev/null
echo "Diretório criado."
echo ""

cd DRE > /dev/null

echo "Buscando Informações do Demonstrativo de Resultados do Exercício no site da CVM..."
wget -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" localhost/DOC/DFP/DRE/DADOS/
echo "Informações do Demonstrativo de Resultados do Exercício foram copiadas para o diretório DRE."
echo ""

echo "Extraindo arquivos..."
for file in *.zip; do
unzip "$file" '*con*'
rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in * ; do
iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > "convertido.tmp"
rm "$file"
mv -- "convertido.tmp" "$(tr [:lower:] [:upper:] <<< "$file")"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

cd .. > /dev/null

echo "Criando diretório de destino da Demonstração do Valor Adicionado (DVA)..."
mkdir DVA > /dev/null
echo "Diretório criado."
echo ""

cd DVA > /dev/null

echo "Buscando Informações da Demonstração do Valor Adicionado no site da CVM..."
wget -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" localhost/DOC/DFP/DVA/DADOS/
echo "Informações da Demonstração do Valor Adicionado foram copiadas para o diretório DVA."
echo ""

echo "Extraindo arquivos..."
for file in *.zip; do
unzip "$file" '*con*'
rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in * ; do
iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > "convertido.tmp"
rm "$file"
mv -- "convertido.tmp" "$(tr [:lower:] [:upper:] <<< "$file")"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

cd ../.. > /dev/null

echo "Iniciando Processamento das Informações Contábeis Trimestrais (ITR)."
echo ""
cd ITR > /dev/null

echo "Buscando Informações Contábeis Trimestrais no site da CVM..."
wget -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" localhost/DOC/ITR/DADOS/
echo "Informações Trimestrais foram copiadas para o diretório ITR."
echo ""

echo "Extraindo arquivos..."
for file in *.zip; do
unzip "$file" '*con*'
rm "$file"
done > /dev/null
echo "Arquivos foram extraídos."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in * ; do
iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > "convertido.tmp"
rm "$file"
mv -- "convertido.tmp" "$(tr [:lower:] [:upper:] <<< "$file")"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

echo "Criando diretório de destino do Balanço Patrimonial Ativo (BPA)..."
mkdir BPA > /dev/null
echo "Diretório criado."
echo ""

echo "Movendo arquivos pertinentes para o diretório BPA..."
find . -type f -name "*BPA*" -exec mv -t BPA {} +
echo "Arquivos pertinentes foram movidos."
echo ""

echo "Criando diretório de destino do Balanço Patrimonial Passivo (BPP)..."
mkdir BPP > /dev/null
echo "Diretório criado."
echo ""

echo "Movendo arquivos pertinentes para o diretório BPP..."
find . -type f -name "*BPP*" -exec mv -t BPP {} +
echo "Arquivos pertinentes foram movidos."
echo ""

echo "Criando diretório de destino das Demonstrações de Fluxo de Caixa - Método Direto (DFC_MD)..."
mkdir DFC_MD > /dev/null
echo "Diretório criado."
echo ""

echo "Movendo arquivos pertinentes para o diretório DFC_MD..."
find . -type f -name "*DFC_MD*" -exec mv -t DFC_MD {} +
echo "Arquivos pertinentes foram movidos."
echo ""

echo "Criando diretório de destino das Demonstrações das Mutações do Patrimônio Líquido (DMPL)..."
mkdir DMPL > /dev/null
echo "Diretório criado."
echo ""

echo "Movendo arquivos pertinentes para o diretório DMPL..."
find . -type f -name "*DMPL*" -exec mv -t DMPL {} +
echo "Arquivos pertinentes foram movidos."
echo ""

echo "Criando diretório de destino das Demonstrações de Resultado (DRE)..."
mkdir DRE > /dev/null
echo "Diretório criado."
echo ""

echo "Movendo arquivos pertinentes para o diretório DRE..."
find . -type f -name "*DRE*" -exec mv -t DRE {} +
echo "Arquivos pertinentes foram movidos."
echo ""

echo "Criando diretório de destino das Demonstrações de Valor Adicionado (DVA)..."
mkdir DVA > /dev/null
echo "Diretório criado."
echo ""

echo "Movendo arquivos pertinentes para o diretório DVA..."
find . -type f -name "*DVA*" -exec mv -t DVA {} +
echo "Arquivos pertinentes foram movidos."
echo ""

echo "Removendo arquivos restantes..."
rm * 2> /dev/null
echo "Arquivos restantes foram removidos."
echo ""

cd .. > /dev/null

echo "Iniciando Processamento das Informações Cadastrais (CAD)."
echo ""
cd CAD > /dev/null

echo "Buscando Informações Cadastrais no site da CVM..."
wget -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" localhost/CAD/DADOS/
echo "Informações Cadastrais foram copiadas para o diretório CAD."
echo ""

echo "Convertendo arquivos para ASCII..."
for file in * ; do
iconv -f ISO-8859-1 -t ASCII//TRANSLIT "$file" > "convertido.tmp"
rm "$file"
mv -- "convertido.tmp" "$(tr [:lower:] [:upper:] <<< "$file")"
done > /dev/null
echo "Arquivos foram convertidos."
echo ""

cd .. > /dev/null
