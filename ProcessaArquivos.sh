#!/usr/bin/env bash
echo ""

echo "Criando diretório para executáveis..."
rm -r bin > /dev/null
mkdir bin > /dev/null
echo "Diretório criado."
echo ""

echo "Processando arquivo de dados cadastrais..."
gcc -Wall -o bin/ProcessaCAD lib/LISTA.c lib/XSV.c src/ProcessaCAD.c
./bin/ProcessaCAD > PROCESSADO.csv
rm InformacoesContabeis/CAD/CAD.csv
mv PROCESSADO.csv InformacoesContabeis/CAD/CAD.csv
echo "Arquivo processado."
echo ""

echo "Processando arquivos de dados contábeis..."
gcc -Wall -o bin/ProcessaIC lib/LISTA.c lib/XSV.c src/ProcessaIC.c
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
gcc -Wall -o bin/OrganizaEmpresasEmSetores lib/LISTA.c src/OrganizaEmpresasEmSetores.c
./bin/OrganizaEmpresasEmSetores
echo "Dados organizados."
echo ""
