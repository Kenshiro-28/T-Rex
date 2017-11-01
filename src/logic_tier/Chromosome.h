/*
 * Chromosome.h
 *
 *  Created on: Sep 7, 2017
 *      Author: kenshiro
 */

#ifndef LOGIC_TIER_CHROMOSOME_H_
#define LOGIC_TIER_CHROMOSOME_H_

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#define CHROMOSOME_MINIMUM_NUMBER_OF_GENES 1

typedef struct chromosome Chromosome;

typedef enum
{
	GENE_STATE_POSITIVE = 1,
	GENE_STATE_NEGATIVE = -1,
} Gene;

typedef enum
{
	CHROMOSOME_RETURN_VALUE_OK = 0,
	CHROMOSOME_NULL_POINTER_ERROR = -1,
	CHROMOSOME_MEMORY_ALLOCATION_ERROR = -2,
	CHROMOSOME_NUMBER_OF_GENES_ERROR = -3
} ChromosomeErrorCode;

ChromosomeErrorCode createChromosome(Chromosome **myChromosome, int numberOfGenes);
ChromosomeErrorCode destroyChromosome(Chromosome **myChromosome);
ChromosomeErrorCode getNumberOfGenes(Chromosome *myChromosome, int *numberOfGenes);
ChromosomeErrorCode getGene(Chromosome *myChromosome, int position, Gene *myGene);
ChromosomeErrorCode setGene(Chromosome *myChromosome, int position, Gene myGene);
ChromosomeErrorCode mutateChromosome(Chromosome *myChromosome);

#endif /* LOGIC_TIER_CHROMOSOME_H_ */
