#ifndef GEN_H
#define GEN_H

#include <string>

using namespace std;

class Gen 
{
    public:
    Gen();

    void generate(const string& fileName, unsigned int ilosc_miast,double Efficiency);
};



#endif //Gen_h