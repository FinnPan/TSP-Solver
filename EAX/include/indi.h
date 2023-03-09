#ifndef __INDI__
#define __INDI__

class Utils {
public:
	static void Permutation(int* array, int numOfelement, int numOfSample);
	static int Integer(int minNumber, int maxNumber);
	static void Index_B(int* Arg, int numOfArg, int* indexOrderd, int numOfOrd);
};

class Indi {
public:
  Indi();
  ~Indi();
  void Define( int N );
  Indi& operator = ( const Indi& src );   /* Copy */
  bool operator == (  const Indi& indi2 ); /* Return true if two tours are the same, false otherwise */

  int fN;                 /* Number of cities */
  int** fLink;            /* fLink[i][]: two vertices adjacent to i */
  int fEvaluationValue;   /* Tour length of */
};

#endif
