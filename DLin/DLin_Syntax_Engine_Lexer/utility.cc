#include "common.h"
#include "utility.h"

PrimeTable PTbl;
FILE* primeDatFile;

int getPrime()
{
    int p = 0;
    int c;
    while ((c = fgetc(primeDatFile)) == ' ' || c == '\t' || c == '\r' || c == '\n')
        ;
    if (isdigit(c))
    {
        do {
            p = p * 10 + (c - '0');
            c = fgetc(primeDatFile);
        } while (isdigit(c));
        return p;
    } 
    else if (c == EOF)
    {
        return -1;
    }
}

int initPrimeTable()
{
    int prime;
    int* pi = PTbl;
    primeDatFile = fopen("prime.dat", "r");
    while((prime = getPrime()) != -1)
        *pi++ = prime;
        //cout<<prime<<" ";
    *pi = -1;
	return OK;
}

int NextPrime(int k)
{
    int i = 0;
    while (PTbl[i] < k)
        i++;
    return PTbl[i];
}
