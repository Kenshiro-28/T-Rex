/*
 * Chromosome.c
 *
 *  Created on: Sep 7, 2017
 *      Author: kenshiro
 */

#include "Chromosome.h"

struct chromosome
{
	int size;
    Gene geneArray[];
};


ChromosomeErrorCode createChromosome(Chromosome **myChromosome, int numberOfGenes)
{
	ChromosomeErrorCode returnValue = CHROMOSOME_RETURN_VALUE_OK;

	if (myChromosome==NULL)
		returnValue = CHROMOSOME_NULL_POINTER_ERROR;

	if ((numberOfGenes<CHROMOSOME_MINIMUM_NUMBER_OF_GENES) || (numberOfGenes>INT_MAX))
		returnValue = CHROMOSOME_NUMBER_OF_GENES_ERROR;

	if (returnValue==CHROMOSOME_RETURN_VALUE_OK)
	{
		int memorySize = sizeof(int) + sizeof(Gene)*numberOfGenes;
		*myChromosome = malloc(memorySize);

		if (*myChromosome==NULL)
		    returnValue = CHROMOSOME_MEMORY_ALLOCATION_ERROR;
	}

	//Initialize
	if (returnValue==CHROMOSOME_RETURN_VALUE_OK)
	{
		(*myChromosome)->size = numberOfGenes;

		for (int i=0; i<numberOfGenes; i++)
		{
			int randomValue = rand() % 2;

			if (randomValue==0)
				(*myChromosome)->geneArray[i] = GENE_STATE_NEGATIVE;
			else
				(*myChromosome)->geneArray[i] = GENE_STATE_POSITIVE;
		}
	}

	return returnValue;
}

ChromosomeErrorCode destroyChromosome(Chromosome **myChromosome)
{
	ChromosomeErrorCode returnValue = CHROMOSOME_RETURN_VALUE_OK;

	if ((myChromosome==NULL) || (*myChromosome==NULL))
		returnValue = CHROMOSOME_NULL_POINTER_ERROR;

	if (returnValue==CHROMOSOME_RETURN_VALUE_OK)
	{
		free(*myChromosome);
		*myChromosome = NULL;
	}

	return returnValue;
}

ChromosomeErrorCode getNumberOfGenes(Chromosome *myChromosome, int *numberOfGenes)
{
	ChromosomeErrorCode returnValue = CHROMOSOME_RETURN_VALUE_OK;

	if ((myChromosome==NULL) || (numberOfGenes==NULL))
		returnValue = CHROMOSOME_NULL_POINTER_ERROR;

	if (returnValue==CHROMOSOME_RETURN_VALUE_OK)
		*numberOfGenes = myChromosome->size;

	return returnValue;
}

ChromosomeErrorCode getGene(Chromosome *myChromosome, int position, Gene *myGene)
{
	ChromosomeErrorCode returnValue = CHROMOSOME_RETURN_VALUE_OK;

	if ((myChromosome==NULL) || (myGene==NULL))
		returnValue = CHROMOSOME_NULL_POINTER_ERROR;

	if ((position<0) || (position>=myChromosome->size))
		returnValue = CHROMOSOME_NUMBER_OF_GENES_ERROR;

	if (returnValue==CHROMOSOME_RETURN_VALUE_OK)
		*myGene = myChromosome->geneArray[position];

	return returnValue;
}

ChromosomeErrorCode setGene(Chromosome *myChromosome, int position, Gene myGene)
{
	ChromosomeErrorCode returnValue = CHROMOSOME_RETURN_VALUE_OK;

	if (myChromosome==NULL)
		returnValue = CHROMOSOME_NULL_POINTER_ERROR;

	if ((position<0) || (position>=myChromosome->size))
		returnValue = CHROMOSOME_NUMBER_OF_GENES_ERROR;

	if (returnValue==CHROMOSOME_RETURN_VALUE_OK)
		myChromosome->geneArray[position] = myGene;

	return returnValue;
}

ChromosomeErrorCode mutateChromosome(Chromosome *myChromosome)
{
	ChromosomeErrorCode returnValue = CHROMOSOME_RETURN_VALUE_OK;

	if (myChromosome==NULL)
		returnValue = CHROMOSOME_NULL_POINTER_ERROR;

	if (returnValue==CHROMOSOME_RETURN_VALUE_OK)
	{
		//Calculate a random number of mutations
		int numberOfMutations = (rand() % myChromosome->size) + 1;

		for (int i=0; i<numberOfMutations; i++)
		{
			int randomPosition = rand() % myChromosome->size;

			if (myChromosome->geneArray[randomPosition]==GENE_STATE_NEGATIVE)
				myChromosome->geneArray[randomPosition]=GENE_STATE_POSITIVE;
			else
				myChromosome->geneArray[randomPosition]=GENE_STATE_NEGATIVE;
		}
	}

	return returnValue;
}

