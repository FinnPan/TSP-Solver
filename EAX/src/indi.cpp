#include <assert.h>
#include <random>

#include "indi.h"

void Utils::Permutation(int* array, int numOfElement, int numOfSample)
{
    int i, j, k, r;
    int* b;

    if (numOfElement <= 0)
        return;

    b = new int[numOfElement];

    for (j = 0; j < numOfElement; j++) b[j] = 0;
    for (i = 0; i < numOfSample; i++)
    {
        r = rand() % (numOfElement - i);
        k = 0;
        for (j = 0; j <= r; j++)
        {
            while (b[k] == 1)
            {
                k++;
            }
            k++;
        }
        array[i] = k - 1;
        b[k - 1] = 1;
    }
    delete[] b;
}

int Utils::Integer(int minNumber, int maxNumber)
{
    return minNumber + (int)(rand() * (double)(maxNumber - minNumber + 1));
}

void Utils::Index_B(int* Arg, int numOfArg, int* indexOrderd, int numOfOrd)
{
    int indexBest = 0;
    int valueBest;
    int* checked;
    checked = new int[numOfArg];

    assert(Arg[0] > -999999999);

    for (int i = 0; i < numOfArg; ++i)
        checked[i] = 0;

    for (int i = 0; i < numOfOrd; ++i)
    {
        valueBest = -999999999;
        for (int j = 0; j < numOfArg; ++j)
        {
            if ((Arg[j] > valueBest) && checked[j] == 0) {
                valueBest = Arg[j];
                indexBest = j;
            }
        }
        indexOrderd[i] = indexBest;
        checked[indexBest] = 1;
    }

    delete[] checked;
}

Indi::Indi()
{                
  fN = 0;
  fLink = NULL;
  fEvaluationValue = 0;
}
 
Indi::~Indi()
{
  for ( int i = 0; i < fN; ++i ) 
    delete[] fLink[ i ];
  delete[] fLink;
}

void Indi::Define( int N )
{
  fN = N;
  
  fLink = new int* [ fN ];
  for( int i = 0; i < fN; ++i ) 
    fLink[ i ] = new int [ 2 ];
} 

Indi& Indi::operator = ( const Indi& src )
{
  fN = src.fN;

  for ( int i = 0; i < fN; ++i ) 
    for ( int j = 0; j < 2; ++j ) 
      fLink[i][j] = src.fLink[i][j];
  fEvaluationValue = src.fEvaluationValue;

  return *this;
}

bool Indi::operator == ( const Indi& src )
{
  int curr,next,pre;

  if( fN != src.fN )  
    return false;
  if( fEvaluationValue != src.fEvaluationValue )  
    return false;
  
  curr = 0;
  pre = -1;
  for( int i = 0; i < fN; ++i )
  {
    if( fLink[curr][0] == pre ) 
      next = fLink[curr][1];
    else 
      next = fLink[curr][0];
	
    if( src.fLink[curr][0] != next && src.fLink[curr][1] != next ) 
    {
      return false;
    }

    pre = curr;    
    curr = next; 
  }

  return true;
}

