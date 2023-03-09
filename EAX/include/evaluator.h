#ifndef __EVALUATOR__
#define __EVALUATOR__

#include "indi.h"

class Evaluator {
 public:
  Evaluator();
  ~Evaluator();
  void SetInstance( const char filename[] );       /* Set the instance */
  void DoIt( Indi& indi );                  /* Set the value of indi.fEvaluationValue */
  void WriteTo( FILE* fp, Indi& indi );     /* Write an tour */
  bool ReadFrom( FILE* fp, Indi& indi );    /* Read an tour */
  bool CheckValid( int* array, int value ); /* Check an tour */ 

  int fNearNumMax;  /* Maximum number of k (see below) */
  int **fNearCity;  /* NearCity[i][k]: k-th nearest city from */
  int  **fEdgeDis;  /* EdgeDis[i][j]: distance between i and j */
  int Ncity;        /* Number of cities */
  double *x;        /* x[i]: x-coordinate of */
  double *y;        /* y[i]: x-coordinate of */
  int* checkedN;
};

#endif
