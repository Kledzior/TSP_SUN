#ifndef TSPZACHLANNYlos_H
#define TSPZACHLANNYlos_H

#include "TSPbrute.h"
#include <vector>

using namespace std;

class TSPzachlanny_los : public TSPbrute
{
    private:
    // vector<vector<vector<double>>> graph;
    // vector<int> cycle;
    // int liczba;
    // double dlugosc_min = numeric_limits<double>::max();
    // int ilosc_cykli=0;
    // vector<int> cycle_min;
    //  vector<vector<vector<double>>> graph;
    // vector<int> cycle;
    // int liczba;
    // double dlugosc_min = numeric_limits<double>::max();
    // int ilosc_cykli=0;
    // vector<int> cycle_min;
    public:
    //void hamilton(vector<vector<vector<double>>>& graph);
    
    bool is_Closest(int v, int w, const vector<vector<vector<double>>>& graph, const vector<int>& cycle);
    void hamiltonianCycles(const vector<vector<vector<double>>>& graph,int choice2);
    void printHamiltonianCycle(const vector<vector<vector<double>>>& graph, const vector<int>& cycle,int choice2);
    double calculateHamiltonianCycleLength(const vector<vector<vector<double>>>& graph, const vector<int>& cycle);
    bool isEdgeValid(int v, int w, const vector<vector<vector<double>>>& graph, const vector<int>& cycle);
    void generateHamiltonianCycles(const vector<vector<vector<double>>>& graph, vector<int>& cycle, int pos,int choice2);
    bool czynnik_losowy(int v,int w);

};

#endif //TSPZACHLANNYdokl_H