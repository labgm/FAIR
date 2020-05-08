import os
import sys

file = "databases_adapters.txt"
f = open(file, 'r')

def processLine(line):
    linha = line.replace("\n", "")
    linha = linha.split(",")

    print("\n ++ Database: "+linha[0]+" ++\n")

    os.system("mkdir ../data/"+linha[0]+"/output_fair")
    os.system("mkdir ../data/"+linha[0]+"/output_fair/k0")
    os.system("mkdir ../data/"+linha[0]+"/output_fair/k1")
    os.system("mkdir ../data/"+linha[0]+"/output_fair/k2")
    os.system("mkdir ../data/"+linha[0]+"/output_fair/k3")
    os.system("mkdir ../data/"+linha[0]+"/output_fair/k4")
    os.system("mkdir ../data/"+linha[0]+"/output_fair/k5")

    print("\n--- k0 ---\n")
    
    print("./FAIR --only-remove -f ../data/"+linha[0]+"/"+linha[0]+"_1.fastq --forward-adapter "+linha[1]+" -r ../data/"+linha[0]+"/"+linha[0]+"_2.fastq --reverse-adapter "+linha[2]+" -o ../data/"+linha[0]+"/output_fair/k0 -mm 0")

    os.system("./FAIR --only-remove -f ../data/"+linha[0]+"/"+linha[0]+"_1.fastq --forward-adapter "+linha[1]+" -r ../data/"+linha[0]+"/"+linha[0]+"_2.fastq --reverse-adapter "+linha[2]+" -o ../data/"+linha[0]+"/output_fair/k0 -mm 0")
    print("\n--- k1 ---\n")
    os.system("./FAIR --only-remove -f ../data/"+linha[0]+"/"+linha[0]+"_1.fastq --forward-adapter "+linha[1]+" -r ../data/"+linha[0]+"/"+linha[0]+"_2.fastq --reverse-adapter "+linha[2]+" -o ../data/"+linha[0]+"/output_fair/k1 -mm 1")
    print("\n--- k2 ---\n")
    os.system("./FAIR --only-remove -f ../data/"+linha[0]+"/"+linha[0]+"_1.fastq --forward-adapter "+linha[1]+" -r ../data/"+linha[0]+"/"+linha[0]+"_2.fastq --reverse-adapter "+linha[2]+" -o ../data/"+linha[0]+"/output_fair/k2 -mm 2")
    print("\n--- k3 ---\n")
    os.system("./FAIR --only-remove -f ../data/"+linha[0]+"/"+linha[0]+"_1.fastq --forward-adapter "+linha[1]+" -r ../data/"+linha[0]+"/"+linha[0]+"_2.fastq --reverse-adapter "+linha[2]+" -o ../data/"+linha[0]+"/output_fair/k3 -mm 3")
    print("\n--- k4 ---\n")
    os.system("./FAIR --only-remove -f ../data/"+linha[0]+"/"+linha[0]+"_1.fastq --forward-adapter "+linha[1]+" -r ../data/"+linha[0]+"/"+linha[0]+"_2.fastq --reverse-adapter "+linha[2]+" -o ../data/"+linha[0]+"/output_fair/k4 -mm 4")
    print("\n--- k5 ---\n")
    os.system("./FAIR --only-remove -f ../data/"+linha[0]+"/"+linha[0]+"_1.fastq --forward-adapter "+linha[1]+" -r ../data/"+linha[0]+"/"+linha[0]+"_2.fastq --reverse-adapter "+linha[2]+" -o ../data/"+linha[0]+"/output_fair/k5 -mm 5")

    print("\n ** FIM Database: "+linha[0]+" **\n")

for line in f:
	processLine(line)