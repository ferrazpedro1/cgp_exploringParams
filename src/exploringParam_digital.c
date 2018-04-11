/*
	This file is part of CGP-Library
	Copyright (c) Andrew James Turner 2014 (andrew.turner@york.ac.uk)

    CGP-Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published 
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CGP-Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CGP-Library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <math.h>
#include "cgp.h"  
#include <stdlib.h>
#include <time.h>

//#define POPULATIONSIZE 5
//#define NUMINPUTS 4
//#define NUMNODES 16
//#define NUMOUTPUTS 1
#define ARITY 2

void openOutputFile(char*);
void writeCircuitInfo();
void closeOutputFile();

FILE * pFile;

int POPULATIONSIZE;
int NUMINPUTS;
int NUMNODES;
int NUMOUTPUTS;
int maxGens;
int generations;
int numActiveConnections;
int numActiveNodes;
int finalFitness;
double MutationRate;
char* gatesToUse;



double elapsed_secs;

unsigned int seed;

//char* inputFile;
//char* outputFile;

int execNumber;

int main(int argc, char* argv[]){


    if (argc != 13) {
		printf("Please, fill all the parameters.\n");
		printf("Usage: \n");
		printf("./CGP_ExploringParam <population_size> <number_inputs> <number_outputs> <matrix_size> <max_generations> <seed> <mutation_rate> <list_of_gates_to_use> <truth_table_input> <output_circuit_draw (.dot)>  <CSV_output_file> <exec_number>\n");
		printf("Please, check the run_CGP.sh file.\n");
        return -1;
    }	


// -----------------------------------------
// Set all input parameters
// -----------------------------------------

	POPULATIONSIZE = atoi(argv[1]);
	NUMINPUTS = atoi(argv[2]);
	NUMNODES = atoi(argv[4])*atoi(argv[4]);
	NUMOUTPUTS = atoi(argv[3]);
	maxGens = atoi(argv[5]);
	seed = atoi(argv[6]); 
	MutationRate = atof(argv[7]);
	gatesToUse = argv[8];  
	char* inputFile = argv[9];
	char* outputFileDot = argv[10];
	char* outputFile = argv[11];
	execNumber = atoi(argv[12]);
	

// -----------------------------------------
// Print back to user all the parameters
// -----------------------------------------

	printf("\n---------- CGP Parameters -------------------\n");
	printf("Inputs: %d \nOutputs: %d \nMatrix Size (NxN): %d \nMax Generations: %d \nMutation Rate: %f \n",NUMINPUTS,NUMOUTPUTS,NUMNODES,maxGens,MutationRate);
	printf("\n-------------- Files ------------------------\n");
	printf("Input File: \t%s \nOutput File: \t%s \nDot File: \t%s \n",inputFile,outputFile,outputFileDot);
	printf("Seed: \t\t%d \n",seed);
	printf("\n---------------------------------------------\n");
	printf(" Portas usadas: %s", gatesToUse);
	printf("\n---------------------------------------------\n");
	clock_t begin = clock();

	int i, gen;
	

	struct parameters *params = NULL;
	struct chromosome *population[POPULATIONSIZE];
	struct chromosome *fittestChromosome = NULL;
	struct dataSet *trainingData = NULL;
		
	double targetFitness = 0;

	setRandomNumberSeed(seed);
				
	params = initialiseParameters(NUMINPUTS, NUMNODES, NUMOUTPUTS, ARITY);
	
	//char* portas = "not,or,nor,and,nand,xor";
	//char* portas = "nand";

	addNodeFunction(params, gatesToUse);
	

	openOutputFile(outputFile);

	//addNodeFunction(params, "and");
	/*setTargetFitness(params, targetFitness);*/
	setMutationType(params, "probabilistic");
	setMutationRate(params, MutationRate);
	
	trainingData = initialiseDataSetFromFile(inputFile);
	
	for(i=0; i<POPULATIONSIZE; i++){
		population[i] = initialiseChromosome(params);
	}
	
	fittestChromosome = initialiseChromosome(params);
	
	/* for the number of allowed generations*/
	for(gen=0; gen<maxGens; gen++){
		
		/* set the fitnesses of the population of chromosomes*/
		for(i=0; i<POPULATIONSIZE; i++){
			setChromosomeFitness(params, population[i], trainingData);
		}
		
		/* copy over the last chromosome to fittestChromosome*/
		copyChromosome(fittestChromosome, population[POPULATIONSIZE - 1]);
		
		/* for all chromosomes except the last*/
		for(i=0; i<POPULATIONSIZE-1; i++){
			
			/* copy ith chromosome to fittestChromosome if fitter*/
			if(getChromosomeFitness(population[i]) < getChromosomeFitness(fittestChromosome)){
				copyChromosome(fittestChromosome, population[i]);
			}
		}
				
		/* termination condition*/
		if(getChromosomeFitness(fittestChromosome) <= targetFitness){
			break;
		}
				
		/* set the first member of the population to be the fittest chromosome*/
		copyChromosome(population[0], fittestChromosome);
		
		/* set remaining member of the population to be mutations of the
		 fittest chromosome*/
		for(i=1; i<POPULATIONSIZE; i++){
			
			copyChromosome(population[i], fittestChromosome);
			mutateChromosome(params, population[i]);
		}
	}
	
	printf("gen\tfitness\n");

	float fitness = getChromosomeFitness(fittestChromosome);
	
	finalFitness = fitness;
	generations = gen;
	numActiveNodes = getNumChromosomeActiveNodes(fittestChromosome);
	numActiveConnections = getNumChromosomeActiveConnections(fittestChromosome);


	clock_t end = clock();

	elapsed_secs = ((double)end - (double)begin) / (double)CLOCKS_PER_SEC;

	writeCircuitInfo();

	printf("%d\t%f\n", gen, fitness);
	
	printChromosome(fittestChromosome, 0);
	
//	if (!fitness) saveChromosomeDot(fittestChromosome, 0, outputFileDot);
	saveChromosomeDot(fittestChromosome, 0, outputFileDot);
	
	for(i=0; i<POPULATIONSIZE; i++){
		freeChromosome(population[i]);
	}
	
	freeChromosome(fittestChromosome);
	freeDataSet(trainingData);
	freeParameters(params);
	closeOutputFile();
	

	printf("---------------------------------------------\n");
	printf(" --> Final Fitness: %f", fitness);
	printf("\n---------------------------------------------\n\n\n");

	return fitness;
}


void openOutputFile (char* filename) {

	pFile = fopen (filename,"aw");
	//fprintf(pFile, "NUM_EXECUCAO;NUM_INPUTS;NUM_OUTPUTS;NUM_NODES;POPULATION_SIZE;MAX_GENS;FINAL_FITNESS;GENERATIONS;NUM_ACTIVE_CONNECTIONS;NUM_ACTIVE_NODES;TIME;MUTATION\n");
}

void writeCircuitInfo() {
	fprintf(pFile,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%lf;%lf\n",execNumber,NUMINPUTS,NUMOUTPUTS,NUMNODES,POPULATIONSIZE,maxGens,finalFitness,generations,numActiveConnections,numActiveNodes,elapsed_secs,MutationRate);
}

void closeOutputFile () {
	fclose(pFile);
}
