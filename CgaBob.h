#ifndef CGABOB_H
#define CGABOB_H

/////////////////////////////////////////////////////////////////////////
//
//		File: CGABob.h
//
//		Author: Mat Buckland
//
//		Desc: definition of the SGenome class and the genetic algorithm
//			  class CGABob from chapter 3
//
/////////////////////////////////////////////////////////////////////////

#include <vector>
#include <sstream>

#include "defines.h"
#include "CBobsMap.h"
#include "utils.h"

using namespace std;



//--------------------------------------------------------------
//	define the genome structure
//--------------------------------------------------------------
struct SGenome
{

	//lihx
	vector<int> vecYCoordinates;
	double		dFitness;

	SGenome():dFitness(0){}
	
	SGenome(const vector<int> &yCoordinates)
	{
		vecYCoordinates = yCoordinates;
	}
};


//--------------------------------------------------------------
//	define the genetic algorithm class
//---------------------------------------------------------------
class CgaBob
{
private:

	//the population of genomes
	vector<SGenome>	m_vecGenomes;
	
	//size of population
	int             m_iPopSize;

	double          m_dCrossoverRate;
	
	double          m_dMutationRate;
	
	//how many bits per chromosome
	int             m_iChromoLength;

	//how many bits per gene
	int             m_iGeneLength;
	
	int             m_iFittestGenome;
	
	double          m_dBestFitnessScore;
	
	double          m_dTotalFitnessScore;
	//lihx
	float			m_fLongestRoute;
	float			m_fShortestRoute;
	
	int             m_iGeneration;

	//create an instance of the map class
	CBobsMap        m_BobsMap;

	//we use another CBobsMap object to keep a record of 
	//the best route each generation as an array of visited
	//cells. This is only used for display purposes.
	CBobsMap		m_BobsBrain;

	//lets you know if the current run is in progress.
	bool			m_bBusy;
	

	void		Mutate2(vector<int> &vectYCoordinates);

	void        Crossover2(const  vector<int>	&mum,
						 const   vector<int>	&dad,
								 vector<int>	&baby1,
								 vector<int>	&baby2);
	SGenome&		RouletteWheelSelection();
	
	//updates the genomes fitness with the new fitness scores and calculates
  //the highest fitness and the fittest member of the population.
  void			  UpdateFitnessScores();

	//creates a start population of random bit strings
  void			  CreateStartPopulation();

public:
	
	CgaBob(double cross_rat,
         double mut_rat,
         int    pop_size,
         int    num_bits,
         int    gene_len):m_dCrossoverRate(cross_rat),
                          m_dMutationRate(mut_rat),
                          m_iPopSize(pop_size),
                          m_iChromoLength(num_bits),
                          m_dTotalFitnessScore(0.0),
                          m_iGeneration(0),
                          m_iGeneLength(gene_len),
						  m_fLongestRoute(0),
						  m_fShortestRoute(9999.0),
                          m_bBusy(false)
		
	{
		CreateStartPopulation();
	}
	
	void			Run(HWND hwnd);

	void			Render(int cxClient, int cyClient, HDC surface);

  void			Epoch();
	
	//accessor methods
	int				Generation(){return m_iGeneration;}
	int				GetFittest(){return m_iFittestGenome;}
  bool      Started(){return m_bBusy;}
  void			Stop(){m_bBusy = false;}
};



#endif