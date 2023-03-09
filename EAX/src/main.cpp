#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "env.h"

int main( int argc, char* argv[] )
{
  Environment* gEnv = NULL;
  gEnv = new Environment();

  gEnv->fNumOfPop = 100;
  gEnv->fNumOfKids = 30;
  gEnv->fFileNameTSP = "..\\testcase\\fnl4461.tsp";

  gEnv->Define();
  gEnv->DoIt();

  return 0;
}
