echo ""

echo "Criando diretório para executáveis..."
rm -r SOURCE/EXEC > /dev/null
mkdir SOURCE/EXEC > /dev/null
echo "Diretório criado."
echo ""

echo "Processando arquivo de dados cadastrais..."
gcc -Wall -o SOURCE/EXEC/PROCESSA_CAD SOURCE/MODULOS/LISTA.C SOURCE/MODULOS/XSV.C SOURCE/CODIGOS/PRINCIPAL_CAD.C
./SOURCE/EXEC/PROCESSA_CAD > PROCESSADO.CSV
rm INFORMACOES_CONTABEIS/CAD/CAD_CIA_ABERTA.CSV
mv PROCESSADO.CSV INFORMACOES_CONTABEIS/CAD/CAD_CIA_ABERTA.CSV
echo "Arquivo processado."
echo ""

echo "Processando arquivos de dados contábeis..."
gcc -Wall -o SOURCE/EXEC/PROCESSA_IC SOURCE/MODULOS/LISTA.C SOURCE/MODULOS/XSV.C SOURCE/CODIGOS/PRINCIPAL_IC.C
for file in $(find INFORMACOES_CONTABEIS/ICA -name '*.CSV'); do
./SOURCE/EXEC/PROCESSA_IC "$file" > PROCESSADO.CSV
rm "$file"
mv PROCESSADO.CSV "$file"
done > /dev/null
for file in $(find INFORMACOES_CONTABEIS/ITR -name '*.CSV'); do
./SOURCE/EXEC/PROCESSA_IC "$file" > PROCESSADO.CSV
rm "$file"
mv PROCESSADO.CSV "$file"
done > /dev/null
echo "Arquivos processados."
echo ""