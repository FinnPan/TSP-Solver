#ifndef __KOPT__
#define __KOPT__

#include "indi.h"

class Evaluator;
class Kopt {
public:
  Kopt( int N );
  ~Kopt();
  void SetInvNearList();
  void TransIndiToTree( Indi& indi );
  void TransTreeToIndi( Indi& indi );

  void DoIt( Indi& Indi );             /* Apply a local search with the 2-opt neighborhood */
  void Sub();
  int GetNext( int t );
  int GetPrev( int t );
  void IncrementImp( int flagRev );
  void CombineSeg( int segL, int segS );

  void CheckDetail();
  void CheckValid();

  void Swap(int &a,int &b);
  int Turn( int &orient );

  void MakeRandSol( Indi& indi );      /* Set a random tour */

  Evaluator* eval;

private:
  int fN;

  int fFixNumOfSeg;
  int fNumOfSeg;   
  int **fLink;     
  int *fSegCity;   
  int *fOrdCity;   

  int *fOrdSeg;    
  int *fOrient;    
  int **fLinkSeg;  
  int *fSizeSeg;   
  int **fCitySeg;  

  int *fT;
  int fFlagRev;  
  int fTourLength;

  int *fActiveV;
  int **fInvNearList; 
  int *fNumOfINL;     
  
  int *fArray;
  int *fCheckN;
  int *fGene;
  int *fB;
};

#endif
