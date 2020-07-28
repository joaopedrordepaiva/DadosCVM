#!/usr/bin/env bash
echo ""

echo "Criando diretório para executáveis..."
rm -r SOURCE/EXEC > /dev/null
mkdir SOURCE/EXEC > /dev/null
echo "Diretório criado."
echo ""

echo "Processando arquivo de dados cadastrais..."
gcc -Wall -o SOURCE/EXEC/PROCESSA_CAD SOURCE/MODULOS/LISTA.C SOURCE/MODULOS/XSV.C SOURCE/CODIGOS/PRINCIPAL_CAD.C
./SOURCE/EXEC/PROCESSA_CAD > PROCESSADO.csv
rm INFORMACOES_CONTABEIS/CAD/CAD.csv
mv PROCESSADO.csv INFORMACOES_CONTABEIS/CAD/CAD.csv
echo "Arquivo processado."
echo ""

echo "Processando arquivos de dados contábeis..."
gcc -Wall -o SOURCE/EXEC/PROCESSA_IC SOURCE/MODULOS/LISTA.C SOURCE/MODULOS/XSV.C SOURCE/CODIGOS/PRINCIPAL_IC.C
for file in $(find INFORMACOES_CONTABEIS/ICA -name '*.csv')
do
    ./SOURCE/EXEC/PROCESSA_IC "$file" > PROCESSADO.csv
    rm "$file"
    mv PROCESSADO.csv "$file"
done > /dev/null
for file in $(find INFORMACOES_CONTABEIS/ITR -name '*.csv')
do
    ./SOURCE/EXEC/PROCESSA_IC "$file" > PROCESSADO.csv
    rm "$file"
    mv PROCESSADO.csv "$file"
done > /dev/null
echo "Arquivos processados."
echo ""

echo "Organizando dados contábeis por setor, empresa, relatório e data..."
gcc -Wall -o SOURCE/EXEC/ORGANIZA_EMPRESAS_SETORES SOURCE/MODULOS/LISTA.C SOURCE/CODIGOS/ORGANIZA_EMPRESAS_SETORES.C
./SOURCE/EXEC/ORGANIZA_EMPRESAS_SETORES
echo "Dados organizados."
echo ""
