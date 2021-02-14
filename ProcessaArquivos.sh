#!/usr/bin/env bash
echo ""

echo "Criando diretório para executáveis..."
rm -r bin > /dev/null
mkdir bin > /dev/null
echo "Diretório criado."
echo ""

echo "Processando arquivo de dados cadastrais..."
g++ -Wall -o bin/ProcessaCAD lib/XSV.cpp src/ProcessaCAD.cpp
./bin/ProcessaCAD > PROCESSADO.csv
rm InformacoesContabeis/CAD/CAD.csv
mv PROCESSADO.csv InformacoesContabeis/CAD/CAD.csv
echo "Arquivo processado."
echo ""

echo "Processando arquivos de dados contábeis..."
g++ -Wall -o bin/ProcessaIC lib/XSV.cpp src/ProcessaIC.cpp
for file in $(find InformacoesContabeis/ICA -name '*.csv')
do
    ./bin/ProcessaIC "$file" > PROCESSADO.csv
    rm "$file"
    mv PROCESSADO.csv "$file"
done > /dev/null
for file in $(find InformacoesContabeis/ITR -name '*.csv')
do
    ./bin/ProcessaIC "$file" > PROCESSADO.csv
    rm "$file"
    mv PROCESSADO.csv "$file"
done > /dev/null
echo "Arquivos processados."
echo ""

echo "Organizando dados contábeis por setor, empresa, relatório e data..."
g++ -Wall -o bin/OrganizaEmpresasEmSetores src/OrganizaEmpresasEmSetores.cpp
./bin/OrganizaEmpresasEmSetores
echo "Dados organizados."
echo ""
