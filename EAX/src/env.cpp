#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "Evaluator.h"
#include "Cross.h"
#include "Kopt.h"
#include "env.h"
     
void MakeRandSol( Evaluator* eval , Indi& indi );
void Make2optSol( Evaluator* eval , Indi& indi );

Environment::Environment()
{
  fEvaluator = new Evaluator();
}


Environment::~Environment()
{
  delete [] fIndexForMating;
  delete [] tCurPop;
  delete fEvaluator;
  delete fCross;

  int N = fEvaluator->Ncity;
  for( int i = 0; i < N; ++i ) 
    delete [] fEdgeFreq[ i ];
  delete [] fEdgeFreq;
}


void Environment::Define()
{
  srand(1111);
  fEvaluator->SetInstance( fFileNameTSP );
  int N = fEvaluator->Ncity;

  fIndexForMating = new int [ fNumOfPop + 1 ];  

  tCurPop = new Indi [ fNumOfPop ];
  for ( int i = 0; i < fNumOfPop; ++i )
    tCurPop[i].Define( N );

  tBest.Define( N );

  fCross = new Cross( N );
  fCross->eval = fEvaluator;                 
  fCross->fNumOfPop = fNumOfPop;             

  fKopt = new Kopt( N );
  fKopt->eval = fEvaluator;
  fKopt->SetInvNearList();

  fEdgeFreq = new int* [ N ]; 
  for( int i = 0; i < N; ++i ) 
    fEdgeFreq[ i ] = new int [ N ]; 
}


void Environment::DoIt()
{
  this->fTimeStart = clock();   

  this->InitPop();                       

  this->fTimeInit = clock();    

  this->Init();
  this->GetEdgeFreq();

  while( 1 )
  {
    this->SetAverageBest();
    printf( "%d: %d %lf\n", fCurNumOfGen, fBestValue, fAverageValue );

    if( this->TerminationCondition() ) break;

    this->SelectForMating();

    for( int s =0; s < fNumOfPop; ++s )
    {
      this->GenerateKids( s );     
      this->SelectForSurvival( s ); 
    }
    ++fCurNumOfGen;
  }

  this->fTimeEnd = clock();   
}
 

void Environment::Init()
{
  fAccumurateNumCh = 0;
  fCurNumOfGen = 0;
  fStagBest = 0;
  fMaxStagBest = 0;
  fStage = 1;          /* Stage I */
  fFlagC[ 0 ] = 4;     /* Diversity preservation: 1:Greedy, 2:--- , 3:Distance, 4:Entropy (see Section 4) */
  fFlagC[ 1 ] = 1;     /* Eset Type: 1:Single-AB, 2:Block2 (see Section 3) */ 
} 


bool Environment::TerminationCondition()
{
  if ( fAverageValue - fBestValue < 0.001 )  
    return true;

  if( fStage == 1 ) /* Stage I */      
  {
    if( fStagBest == int(1500/fNumOfKids) && fMaxStagBest == 0 ){ /* 1500/N_ch (See Section 2.2) */
      fMaxStagBest =int( fCurNumOfGen / 10 );                 /* fMaxStagBest = G/10 (See Section 2.2) */   
    } 
    else if( fMaxStagBest != 0 && fMaxStagBest <= fStagBest ){ /* Terminate Stage I (proceed to Stage II) */
      fStagBest = 0;
      fMaxStagBest = 0;
      fCurNumOfGen1 = fCurNumOfGen;
      fFlagC[ 1 ] = 2; 
      fStage = 2;      
    }
    return false;
  }

  if( fStage == 2 ){ /* Stage II */
    if( fStagBest == int(1500/fNumOfKids) && fMaxStagBest == 0 ){ /* 1500/N_ch */
      fMaxStagBest = int( (fCurNumOfGen - fCurNumOfGen1) / 10 ); /* fMaxStagBest = G/10 (See Section 2.2) */
    } 
    else if( fMaxStagBest != 0 && fMaxStagBest <= fStagBest ){ /* Terminate Stage II and GA */
      return true;
    }

    return false;
  }
  return false;
}


void Environment::SetAverageBest() 
{
  int stockBest = tBest.fEvaluationValue;
  
  fAverageValue = 0.0;
  fBestIndex = 0;
  fBestValue = tCurPop[0].fEvaluationValue;
  
  for(int i = 0; i < fNumOfPop; ++i ){
    fAverageValue += tCurPop[i].fEvaluationValue;
    if( tCurPop[i].fEvaluationValue < fBestValue ){
      fBestIndex = i;
      fBestValue = tCurPop[i].fEvaluationValue;
    }
  }
  
  tBest = tCurPop[ fBestIndex ];
  fAverageValue /= (double)fNumOfPop;

  if( tBest.fEvaluationValue < stockBest ){
    fStagBest = 0;
    fBestNumOfGen = fCurNumOfGen;
    fBestAccumeratedNumCh = fAccumurateNumCh;
  }
  else ++fStagBest;
}


void Environment::InitPop()
{
  for ( int i = 0; i < fNumOfPop; ++i ){ 
    fKopt->MakeRandSol( tCurPop[ i ] );    /* Make a random tour */
    fKopt->DoIt( tCurPop[ i ] );           /* Apply the local search with the 2-opt neighborhood */ 
  }
}


void Environment::SelectForMating()
{
  /* fIndexForMating[] <-- a random permutation of 0, ..., fNumOfPop-1 */
  Utils::Permutation( fIndexForMating, fNumOfPop, fNumOfPop ); 
  fIndexForMating[ fNumOfPop ] = fIndexForMating[ 0 ];
}

void Environment::SelectForSurvival( int s )
{
}


void Environment::GenerateKids( int s )
{
  fCross->SetParents( tCurPop[fIndexForMating[s]], tCurPop[fIndexForMating[s+1]], fFlagC, fNumOfKids );  
  
  /* Note: tCurPop[fIndexForMating[s]] is replaced with a best offspring solutions in tCorss->DoIt(). 
     fEegeFreq[][] is also updated there. */
  fCross->DoIt( tCurPop[fIndexForMating[s]], tCurPop[fIndexForMating[s+1]], fNumOfKids, 1, fFlagC, fEdgeFreq );

  fAccumurateNumCh += fCross->fNumOfGeneratedCh;
}


void Environment::GetEdgeFreq()
{
  int N = fEvaluator->Ncity;
  int k0, k1;
  
  for( int j1 = 0; j1 < N; ++j1 )
    for( int j2 = 0; j2 < N; ++j2 ) 
      fEdgeFreq[ j1 ][ j2 ] = 0;

  
  for( int i = 0; i < fNumOfPop; ++i )
  {
    for(int j = 0; j < N; ++j )
    {
      k0 = tCurPop[ i ].fLink[ j ][ 0 ];
      k1 = tCurPop[ i ].fLink[ j ][ 1 ];
      ++fEdgeFreq[ j ][ k0 ];
      ++fEdgeFreq[ j ][ k1 ];
    }
  }
}


void Environment::PrintOn( int n, char* dstFile ) 
{
  printf( "n = %d val = %d Gen = %d Time = %d %d\n" , 
	  n, 
	  tBest.fEvaluationValue, 
	  fCurNumOfGen, 
	  (int)((double)(this->fTimeInit - this->fTimeStart)/(double)CLOCKS_PER_SEC), 
	  (int)((double)(this->fTimeEnd - this->fTimeStart)/(double)CLOCKS_PER_SEC) );
  fflush(stdout);

  FILE *fp;
  char filename[ 80 ];
  sprintf( filename, "%s_Result", dstFile );
  fp = fopen( filename, "a");
  
  fprintf( fp, "%d %d %d %d %d\n" , 
	   n, 
	   tBest.fEvaluationValue, 
	   fCurNumOfGen, 
	   (int)((double)(this->fTimeInit - this->fTimeStart)/(double)CLOCKS_PER_SEC), 
	   (int)((double)(this->fTimeEnd - this->fTimeStart)/(double)CLOCKS_PER_SEC) );
  
  fclose( fp );
}


void Environment::WriteBest( char* dstFile ) 
{
  FILE *fp;
  char filename[ 80 ];
  sprintf( filename, "%s_BestSol", dstFile );
  fp = fopen( filename, "a");
  
  fEvaluator->WriteTo( fp, tBest );

  fclose( fp );
}


void Environment::WritePop( int n, char* dstFile ) 
{
  FILE *fp;
  char filename[ 80 ];
  sprintf( filename, "%s_POP_%d", dstFile, n );
  fp = fopen( filename, "w");

  for( int s = 0; s < fNumOfPop; ++s )
    fEvaluator->WriteTo( fp, tCurPop[ s ] );

  fclose( fp );
}
