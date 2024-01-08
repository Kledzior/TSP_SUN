#ifndef TSPBRUTE_H
#define TSPBRUTE_H

#include <vector>
#include "nlohmann/json.hpp"

using namespace std;

class TSPbrute 
{
    protected:
    vector<vector<vector<double>>> graph;
    vector<int> cycle;
    int liczba;
    double dlugosc_min = numeric_limits<double>::max();
    int ilosc_cykli=0;
    vector<int> cycle_min;
    //nlohmann::json 
    
    public:
    //void hamilton(vector<vector<vector<double>>>& graph);
    void hamiltonianCycles(const vector<vector<vector<double>>>& graph,int choice2);
    int silnia(int liczba);
    void printHamiltonianCycle(const vector<vector<vector<double>>>& graph, const vector<int>& cycle,int choice2);
    double calculateHamiltonianCycleLength(const vector<vector<vector<double>>>& graph, const vector<int>& cycle);
    bool isEdgeValid(int v, int w, const vector<vector<vector<double>>>& graph, const vector<int>& cycle);
    void generateHamiltonianCycles(const vector<vector<vector<double>>>& graph, vector<int>& cycle, int pos,int choice2);
    //is_Closest(int v, int w, const vector<vector<vector<double>>>& graph, const vector<int>& cycle);


};
   

#endif //TSPBRUTE_H