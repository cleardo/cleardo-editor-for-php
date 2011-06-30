#define PrimeTableSize 10000

typedef int PrimeTable[PrimeTableSize];
extern PrimeTable PTbl;
extern FILE* primeDatFile;

int NextPrime(int);
int getPrime();
int initPrimeTable();
