#include "CgaBob.h"



//--------------------------RouletteWheelSelection-----------------
//
//	selects a member of the population by using roulette wheel 
//	selection as described in the text.
//------------------------------------------------------------------
SGenome& CgaBob::RouletteWheelSelection()
{
	double fSlice	= RandFloat() * m_dTotalFitnessScore;
	
	double cfTotal	= 0.0;
	
	int	SelectedGenome = 0;
	
	for (int i=0; i<m_iPopSize; ++i)
	{
		
		cfTotal += m_vecGenomes[i].dFitness;
		
		if (cfTotal > fSlice) 
		{
			SelectedGenome = i;
			break;
		}
	}
	
	return m_vecGenomes[SelectedGenome];
}

//----------------------------Mutate---------------------------------
//	iterates through each genome flipping the bits acording to the
//	mutation rate
//--------------------------------------------------------------------
void CgaBob::Mutate(vector<int> &vecBits)
{
	for (int curBit=0; curBit<vecBits.size(); curBit++)
	{
		//do we flip this bit?
		if (RandFloat() < m_dMutationRate)
		{
			//flip the bit
			vecBits[curBit] = !vecBits[curBit];
		}
	}//next bit
}
void CgaBob::Mutate2(vector<WayPoint> &vecWayPoint)
{
	for (int curBit=0; curBit<vecWayPoint.size(); curBit++)
	{
		//do we flip this bit?
		if (RandFloat() < m_dMutationRate)
		{
			//flip the bit
			vecWayPoint[curBit].relativeXY.y +=RandInt(-100,100)*RandFloat();
			SPoint rand = vecWayPoint[curBit].relativeXY;
			vecWayPoint[curBit].absoluteXY=m_BobsMap.TransRelativePointToAbusolutePoint(rand);
		}
	}//next bit
}

//----------------------------Crossover--------------------------------
//	Takes 2 parent vectors, selects a midpoint and then swaps the ends
//	of each genome creating 2 new genomes which are stored in baby1 and
//	baby2.
//---------------------------------------------------------------------
void CgaBob:: Crossover2(const vector<WayPoint>	&mum,
						 const  vector<WayPoint>	&dad,
								vector<WayPoint>	&baby1,
								vector<WayPoint>	&baby2)
{
	//just return parents as offspring dependent on the rate
	//or if parents are the same
	if ( (RandFloat() > m_dCrossoverRate) ) 
	{
		baby1 = mum;
		baby2 = dad;

		return;
	}
	
	//determine a crossover point
	int cp = RandInt(0, m_iChromoLength - 1);

	//swap the bits
	for (int i=0; i<cp; ++i)
	{
		baby1.push_back(mum[i]);
		baby2.push_back(dad[i]);
	}

	for (int i=cp; i<mum.size(); ++i)
	{
		baby1.push_back(dad[i]);
		baby2.push_back(mum[i]);
	}
}
//-----------------------------------Run----------------------------------
//
//	This is the function that starts everything. It is mainly another 
//	windows message pump using PeekMessage instead of GetMessage so we
//	can easily and dynamically make updates to the window while the GA is 
//	running. Basically, if there is no msg to be processed another Epoch
//	is performed.
//------------------------------------------------------------------------
void CgaBob::Run(HWND hwnd)
{
	//The first thing we have to do is create a random population
	//of genomes
	CreateStartPopulation();
	
	m_bBusy = true;
		
}
//----------------------CreateStartPopulation---------------------------
//
//-----------------------------------------------------------------------
void CgaBob::CreateStartPopulation()
{
	//clear existing population
	m_vecGenomes.clear();
	
	for (int i=0; i<m_iPopSize; i++)
	{
		
		//lhx
		vector<WayPoint> wpoints;
		while(!m_BobsMap.GetOneValidPath(wpoints))
		{
			wpoints.clear();
		}
		m_vecGenomes.push_back(SGenome (wpoints));
		m_iChromoLength=wpoints.size();
	}

	//reset all variables
	m_iGeneration		 = 0;
	m_iFittestGenome	 = 0;
	m_dBestFitnessScore  = 0;
	m_dTotalFitnessScore = 0;
}
//--------------------------------Epoch---------------------------------
//
//	This is the workhorse of the GA. It first updates the fitness
//	scores of the population then creates a new population of
//	genomes using the Selection, Croosover and Mutation operators
//	we have discussed
//----------------------------------------------------------------------
void CgaBob::Epoch()
{
	
	UpdateFitnessScores();

	//Now to create a new population
	int NewBabies = 0;

	//create some storage for the baby genomes 
	vector<SGenome> vecBabyGenomes;

	while (NewBabies < m_iPopSize)
	{
		//select 2 parents
		SGenome mum = RouletteWheelSelection();
		SGenome dad = RouletteWheelSelection();
	

		//operator - crossover
		SGenome baby1, baby2;
		Crossover2(mum.vecWayPoint, dad.vecWayPoint, baby1.vecWayPoint, baby2.vecWayPoint);

		//operator - mutate
		Mutate2(baby1.vecWayPoint);
		Mutate2(baby2.vecWayPoint);

		//add to new population
		vecBabyGenomes.push_back(baby1);
		vecBabyGenomes.push_back(baby2);

		NewBabies += 2;
	}

	//copy babies back into starter population
	m_vecGenomes = vecBabyGenomes;

	//increment the generation counter
	++m_iGeneration;
}

//---------------------------UpdateFitnessScores------------------------
//	updates the genomes fitness with the new fitness scores and calculates
//	the highest fitness and the fittest member of the population.
//	Also sets m_pFittestGenome to point to the fittest. If a solution
//	has been found (fitness == 1 in this example) then the run is halted
//	by setting m_bBusy to false
//-----------------------------------------------------------------------
void CgaBob::UpdateFitnessScores()
{
	m_iFittestGenome		= 0;
	m_dBestFitnessScore		= 0;
	m_dTotalFitnessScore	= 0;

	m_bBusy=true;
	CBobsMap TempMemory;
	 
	//update the fitness scores and keep a check on fittest so far
	for (int i=0; i<m_iPopSize; ++i)
	{
		//lhx
		vector<WayPoint> vecWayPoints=m_vecGenomes[i].vecWayPoint;

		//get it's fixed route
		m_vecGenomes[i].vecFixedPoint =m_BobsMap.FixToBestPath(vecWayPoints);
		//calculate the tourlength for each chromosome
		float TourLength = m_BobsMap.GetPathLength(m_vecGenomes[i].vecFixedPoint);
		// if route has intersection then add punishment
		TourLength +=50* m_BobsMap.CalculateInvalidPointCount(m_vecGenomes[i].vecWayPoint);

		m_vecGenomes[i].dFitness = TourLength;
		
		//keep a track of the shortest route found each generation
		if (TourLength < m_fShortestRoute)
		{
			m_fShortestRoute = TourLength;

			m_iFittestGenome = i;
		}
		
		//keep a track of the worst tour each generation
		if (TourLength > m_fLongestRoute)
		{
			m_fLongestRoute = TourLength;
		}
	
	}//next genome

	
	//Now we have calculated all the tour lengths we can assign
	//the fitness scores
	for (int i=0; i<m_iPopSize; ++i)
	{
		m_vecGenomes[i].dFitness = m_fLongestRoute - m_vecGenomes[i].dFitness;
		m_dTotalFitnessScore +=m_vecGenomes[i].dFitness;
	}
}



//------------------------- Render -------------------------------
//
//	given a surface to render to this function renders the map
//	and the best path if relevant. cxClient/cyClient are the 
//	dimensions of the client window.
//----------------------------------------------------------------
void CgaBob::Render(int cxClient, int cyClient, HDC surface)
{
	//render the map
	m_BobsMap.Render(cxClient, cyClient, surface);

	//render the best route
	if(m_bBusy)
	{
		vector<SGenome>::const_iterator siter =m_vecGenomes.begin();
		while(siter!=m_vecGenomes.end())
		{	
		
			m_BobsBrain.MemoryRender2(cxClient, cyClient, surface,siter->vecWayPoint);
			m_BobsBrain.MemoryRender3(cxClient, cyClient, surface,	m_BobsMap.FixToBestPath(siter->vecWayPoint));
			siter++;
		}
	}

		

	//Render additional information
	string s = "Generation: " + itos(m_iGeneration);
	TextOut(surface, 5, 0, s.c_str(), s.size());
	

	if (!m_bBusy)
	{
		string Start = "Press Return to start a new run";
		
		TextOut(surface, cxClient/2 - (Start.size() * 3), cyClient - 20, Start.c_str(), Start.size());
	}
	
	else
		
	{
		string Start = "Space to stop";
		
		TextOut(surface, cxClient/2 - (Start.size() * 3), cyClient - 20, Start.c_str(), Start.size());
	}

	if(m_iGeneration>=30) Stop();
	
}
