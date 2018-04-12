#!/bin/bash

echo '************************************************************'
echo 'Cartesian Genetic Programming - Exploring Parameters        '
echo '  Pedro A. P. Ferraz <ferrazpedro1 [at] gmail.com>          '
echo '  Juliana M. M. Araujo <juliana.m.m.araujo [at] gmail.com>  '  
echo '  Pontifical Catholic University of Minas Gerais, 2018      '
echo '************************************************************'

# ---------------------------
# Execution Parameters
# ---------------------------
date=`date +"%H-%M-%S_%d-%m-%Y"`

outputDir="output/exec_$date"

outputFile="parity6bit.txt"
datasetFile="parity6bit.data"

numExec=100

matrixMin=2
matrixMax=8

generationsMin=1000
generationsInterval=1000
generationsMax=20000

mutationMin=0.0
mutationMax=0.5
mutationInterval=0.05

popMin=5
popInterval=5
popMax=25

dataset=parity6bit.data
numInputs=6
numOutputs=1

# leave the var empty if you want a random seed
seed=

gatesToUse="not,or,nor,and,nand,xor"

# ---------------------------
# Checking if output file exists
# ---------------------------
if [ -d $outputDir ]
then
	rm -r $outputDir
fi
mkdir $outputDir
mkdir $outputDir/log
mkdir $outputDir/dot

# ---------------------------
# creates the CSV header
# ---------------------------

echo "NUM_EXECUCAO;NUM_INPUTS;NUM_OUTPUTS;NUM_NODES;POPULATION_SIZE;MAX_GENS;FINAL_FITNESS;GENERATIONS;NUM_ACTIVE_CONNECTIONS;NUM_ACTIVE_NODES;TIME;MUTATION" > $outputDir/$outputFile


# ---------------------------
# Start CGP Loop
# ---------------------------

for i in `seq 1 $numExec`;
do
	for j in `seq $generationsMin $generationsInterval $generationsMax`;
	do
		if [ -z "$seed" ]; then
			seed=`shuf -i1-4000000000 -n1`
		fi
		for k in `seq $matrixMin $matrixMax`;
		do
			for mut in `seq $mutationMin $mutationInterval $mutationMax`;
			do
				for pop in `seq $popMin $popInterval $popMax`;
				do
					echo "================================================================================"
					echo "Execution #$i - Seed $seed - Mutation Rate: $mut  - Initial Population: $pop"
					echo "$outputDir/dot/output$i-$j-$k-$mut.dot"
					echo "================================================================================"					
					bin/CGP_ExploringParam $pop $numInputs $numOutputs $k $j $seed $mut $gatesToUse datasets/$datasetFile $outputDir/dot/output$i-$j-$k-$mut.dot $outputDir/$outputFile $i
				done
			done
		done
	done
done | tee $outputDir/log/log_$date.txt
