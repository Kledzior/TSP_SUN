#ifndef HEURYSTYKA_H
#define HEURYSTYKA_H

#include <set>
#include <vector>

using namespace std;

class Dane
{
    public:

    int C;
    //double maxFuelConsumption;
    unsigned int pheromoneMultiplier;
    vector<vector<double>> pheromoneMatrix;
    //vector<vector<double>> calculate_pheromone_matrix(const vector<vector<vector<double>>>& graph);
    //vector<vector<double>> fuelConsumptionMatrix;
    vector<vector<double>> visibilityMatrix;

    vector<vector<double>> calculate_Visibility_matrix(const vector<vector<vector<double>>>& graph);
    double& operator()(int i, int j)
    {
        return visibilityMatrix[i][j];
    }

    const double& operator()(int i, int j) const
    {
        return visibilityMatrix[i][j];
    }

   
};

class Mrowka
{
    public:
    Dane& dane; //kazdy obiekt klasy mrowka ma dostep do danych przechowywanych w obiekcie klasy Data, bez konieczności kopiowania tych danych
    vector<int> cycle;
    set<int> available; //dostepne miasta
    double evaporationRate;         // predkosc parowania 
	double pheromoneMultiplier;     // wplyw wspolczynnika feromonow
	double visibilityMultiplier;    // wplyw wspolczynnika widocznosci
    int k;
    // int flaga;
     int flaga2=0;
    //vector<vector<double>> VisibilityMatrix;
    //int anon = 4;
    Mrowka(double pheromone, double visibility, double evaporationrate, Dane& dane);
    //HeurystykaTSP(double pheromoneMultiplier, double distanceMultiplier, double evaporationRate, Dane& daneRef);
    ~Mrowka();
    void Reset(vector<int>& cycle);
    double laczny_dystans(vector<int>& cycle,const vector<vector<vector<double>>>& graph);
    void setPheromones(vector<int>& cycle,vector<vector<double>>& pheromoneMatrix,const vector<vector<vector<double>>>& graph);
    vector<int> koniecTrasy(vector<int>& cycle,vector<vector<double>>& pheromoneMatrix,const vector<vector<vector<double>>>& graph);
    void krok(vector<int>& cycle,vector<vector<double>>& pheromoneMatrix,const vector<vector<vector<double>>>& graph);
    void hamiltonianCycles(const vector<vector<vector<double>>>& graph,double pherom_mult,double distan_mult,double evap_rate);
    bool Available(vector<int>& cycle, int N_miasta);
};

class HeurystykaTSP
{
    public:
    double evaporationRate;         // predkosc parowania 
	double pheromoneMultiplier;     // wplyw wspolczynnika feromonow
	double visibilityMultiplier;    // wplyw wspolczynnika widocznosci
    double distanceMultiplier;
    HeurystykaTSP(double pheromoneMultiplier, double distanceMultiplier, double evaporationRate);
};
class Test
{
    public: 
    int anon = 4;
};
#endif //HEURYSTYKA_H