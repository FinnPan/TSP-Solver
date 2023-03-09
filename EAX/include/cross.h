#ifndef __Cross__
#define __Cross__

#include "indi.h"

class Evaluator;
class Cross {
public:
  Cross( int N );
  ~Cross();
  void SetParents( const Indi& tPa1, const Indi& tPa2,     /* Set information of the parent tours */
		   int flagC[ 10 ], int numOfKids ); 
  void DoIt( Indi& tKid, Indi& tPa2, int numOfKids,        /* Main procedure of EAX */
	     int flagP, int flagC[ 10 ], int** fEdgeFreq ); 
  void SetABcycle( const Indi& parent1, const Indi& parent2, /* Step 2 of EAX */
		   int flagC[ 10 ], int numOfKids );
  void FormABcycle();                                   /* Store an AB-cycle found */
  void Swap(int &a,int &b);                             /* Swap */ 
  void ChangeSol( Indi& tKid, int ABnum, int type );   /* Apply an AB-cycle to an intermediate solution */
  void MakeCompleteSol( Indi& tKid );                  /* Step 5 of EAX */
  void MakeUnit();                                      /* Step 5-1 of EAX */ 
  void BackToPa1( Indi& tKid );                        /* Undo the parent p_A */
  void GoToBest( Indi& tKid );                         /* Modify tKid to the best offspring solution */
  void IncrementEdgeFreq( int **fEdgeFreq );            /* Increment fEdgeFreq[][] */
  int Cal_ADP_Loss( int **fEdgeFreq );                  /* Compute the difference in the averate distance */
  double Cal_ENT_Loss( int **fEdgeFreq );               /* Compute the difference in the edge entropy */

  void SetWeight( const Indi& parent1, const Indi& parent2 ); /* Block2 */
  int Cal_C_Naive();                                            /* Block2 */
  void Search_Eset( int num );                                  /* Block2 */
  void Add_AB( int AB_num );                                    /* Block2 */
  void Delete_AB( int AB_num );                                 /* Block2 */

  void CheckValid( Indi& indi );                               /* For debug */


  int fNumOfGeneratedCh;
  Evaluator* eval;			 
  int fNumOfPop;
  
private:
  int fFlagImp;         
  int fN;
  Indi tBestTmp;
  int r,exam;
  int exam_flag;
  int **near_data;
  int *koritsu, *bunki, *kori_inv, *bun_inv;
  int koritsu_many,bunki_many;
  int st,ci,pr,stock,st_appear;
  int *check_koritsu;
  int *fRoute;
  int flag_st,flag_circle,pr_type;
  int ch_dis;
  int **fABcycle;
  int *fPermu;
  int fEvalType;
  int fEsetType;
  int fNumOfABcycleInESet;
  int fNumOfABcycle;
  int fPosiCurr;
  int fMaxNumOfABcycle;

  int *fC;
  int *fJun; 
  int *fOrd1, *fOrd2; 

  // Speed Up Start
  int *fOrder;    
  int *fInv;      
  int **fSegment; 
  int *fSegUnit;  
  		       
  int fNumOfUnit; 
  int fNumOfSeg;  
  int *fSegPosiList;
  int fNumOfSPL;    
  int *LinkAPosi;   
  int **LinkBPosi;  
  int *fPosiSeg;    
  int *fNumOfElementInUnit; 
  int *fCenterUnit;         
  int fNumOfElementInCU;    
  int *fListOfCenterUnit;   
  int fNumOfSegForCenter;   
  int *fSegForCenter;       

  int *fGainAB;             
  int fGainModi;            
  int fNumOfModiEdge;				 
  int fNumOfBestModiEdge;				 
  int **fModiEdge;				 
  int **fBestModiEdge;			
  int fNumOfAppliedCycle;
  int fNumOfBestAppliedCycle;
  int *fAppliedCylce;
  int *fBestAppliedCylce;
  // Speed Up End

  // Block2
  int *fNumOfElementINAB;    
  int **fInEffectNode; 
  int **fWeight_RR; 
  int *fWeight_SR;  
  int *fWeight_C;   
  int *fUsedAB, fNumOfUsedAB;
  int fNum_C, fNum_E;
  int fTmax, fMaxStag;
  int *fMoved_AB;
  int fNumOfABcycleInEset;
  int *fABcycleInEset;
  int fDis_AB;     
  int fBest_Num_C, fBest_Num_E;
};


#endif
