echo ""

echo "Removendo diretório de destino das Informações Contábeis Trimestrais (ITR) e Cadastrais (CAD)..."
rm -r ITR CAD > /dev/null
echo "Diretório removido."
echo ""

echo "Criando diretório de destino das Informações Contábeis Trimestrais (ITR) e Cadastrais (CAD)..."
mkdir ITR CAD > /dev/null
echo "Diretório criado."
echo ""

cd ITR > /dev/null

echo "Buscando Informações Contábeis Trimestrais no site da CVM..."
wget -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" localhost/DOC/ITR/DADOS/
echo "Informações Trimestrais foram copiadas para o diretório ITR."
echo ""

echo "Extraindo Informações Contábeis Trimestrais para o diretório ITR..."
for file in *.zip; do unzip "$file" '*con*' ; done > /dev/null
echo "Informações Trimestrais foram extraidas para o diretório ITR."
echo ""

for file in * ; do
iconv  -f   "ISO 8859-1"  -t   "UTF-8" "$file" > "convertido.tmp"
rm "$file"
mv -- "convertido.tmp" "$(tr [:lower:] [:upper:] <<< "$file")"
done > /dev/null

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

cd CAD > /dev/null

echo "Buscando Informações Cadastrais no site da CVM..."
wget -q -r -nH --cut-dirs=6 --no-parent -e robots=off --reject="index.html*" localhost/CAD/DADOS/
echo "Informações Cadastrais foram copiadas para o diretório CAD."
echo ""

for file in * ; do
iconv  -f   "ISO 8859-1"  -t   "UTF-8" "$file" > "convertido.tmp"
rm "$file"
mv -- "convertido.tmp" "$(tr [:lower:] [:upper:] <<< "$file")"
done > /dev/null

cd .. > /dev/null
