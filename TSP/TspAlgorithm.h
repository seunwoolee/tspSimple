/*
 * TspAlgorithm.h
 *
 *  Created on: Jul 4, 2017
 *      Author: root
 */

#include "TspCity.h"

/*
#include "../GeneticLibrary/source/Initialization.h"

#include "../GeneticLibrary/source/ChromosomeOperations.h"
#include "../GeneticLibrary/source/MultiValueChromosome.h"

#include "../GeneticLibrary/source/Algorithm.h"
#include "../GeneticLibrary/source/StopCriterias.h"
#include "../GeneticLibrary/source/IncrementalAlgorithm.h"
*/

#include "./inc/Initialization.h"
#include "./inc/ChromosomeOperations.h"
#include "./inc/MultiValueChromosome.h"
#include "./inc/Algorithm.h"
#include "./inc/StopCriterias.h"
#include "./inc/IncrementalAlgorithm.h"

using namespace std;
using namespace __gnu_cxx;

using namespace Algorithm;
using namespace Algorithm::SimpleAlgorithms;
using namespace Algorithm::StopCriterias;

//using namespace Chromosome::Representation;
//using namespace Chromosome;

//class GA

class TspChromosome : public GaMultiValueChromosome<const TspCity*>
{
public:

	//TspChromosome(GaChromosomeDomainBlock<const TspCity*>* configBlock) : GaMultiValueChromosome(configBlock) { }
	TspChromosome(GaChromosomeDomainBlock<const TspCity*>* configBlock) : GaMultiValueChromosome<const TspCity*>(configBlock) { }

	TspChromosome(const TspChromosome& chromosome, bool setupOnly) : GaMultiValueChromosome<const TspCity*>(chromosome, setupOnly) { }

	virtual GaChromosomePtr GACALL MakeCopy(bool setupOnly) const { return new TspChromosome( *this, setupOnly ); }

	virtual GaChromosomePtr GACALL MakeNewFromPrototype() const;

	int GACALL GetCityPosition(const TspCity* city) const;

};

class TspCrossover : public GaCrossoverOperation
{

public:

	virtual GaChromosomePtr GACALL operator ()(const GaChromosome* parent1, const GaChromosome* parent2) const;

	virtual GaParameters* GACALL MakeParameters() const { return NULL; }

	virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }

private:

	inline void SelectNextCity(const TspCity* previousCity, const TspCity** currentBestNextCity, const TspCity* nextCity) const
	{
		if( !*currentBestNextCity ||  previousCity->GetDistance( **currentBestNextCity ) > previousCity->GetDistance( *nextCity ) )
			*currentBestNextCity = nextCity;
	}
};

class TspFitness : public GaFitnessOperation
{
public:

	virtual float GACALL operator ()(const GaChromosome* chromosome) const;

	virtual GaParameters* GACALL MakeParameters() const { return NULL; }

	virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }
};
class TSP
{
private:

	static TSP _instance;

	GaChromosomeDomainBlock<const TspCity*>* _ccb;

	GaChromosome* _prototype;

	GaPopulationConfiguration* _populationConfiguration;

	GaPopulation* _population;

	GaAlgorithm* _algorithm;

public:


public:

	inline static TSP& GetInstance() { return _instance; }
	TSP();
	~TSP();
	inline GaAlgorithm* GetAlgorithm() { return _algorithm; }

	void smartInit();

};
