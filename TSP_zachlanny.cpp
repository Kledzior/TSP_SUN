#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

#include "TSPbrute.h"
#include "TSP_zachlanny.h"

#include "nlohmann/json.hpp"

using namespace std;

const double dlugosc_min = numeric_limits<double>::max();
const int ilosc_cykli=0;
const vector<int> cycle_min;

//liczy zachlannie cykl wybierajac za kazdym razem najblizsze miasto startujac od 0 



void TSPzachlanny::printHamiltonianCycle(const vector<vector<vector<double>>>& graph, const vector<int>& cycle,int choice2)
{
    TSPbrute::printHamiltonianCycle(graph,cycle,choice2);
}
double TSPzachlanny::calculateHamiltonianCycleLength(const vector<vector<vector<double>>>& graph, const vector<int>& cycle) 
{
     return TSPbrute::calculateHamiltonianCycleLength(graph, cycle);
}
bool TSPzachlanny::isEdgeValid(int v, int w, const vector<vector<vector<double>>>& graph, const vector<int>& cycle) 
{
    return TSPbrute::isEdgeValid(v,w, graph, cycle);
}
bool TSPzachlanny::is_Closest(int v, int w, const vector<vector<vector<double>>>& graph, const vector<int>& cycle)//v - od miasta A , w - do miasta B , z graph , ma byc minimalna odleglosc, cycle - sprawdzamy czy jest zajety
{   
   double min_odleglosc = numeric_limits<double>::max();
   double dystans;
   //zaczynam zawsze od zerowego a teraz sprawdzam ktory jest najblizszy zerowemu stad moge iterowac od 1
   
        for(int j=1;j<graph.size();j++)
        {
            if(v!=j && isEdgeValid(v,j,graph,cycle))// uniemozliwiam sprawdzanie czy juz uzyty jest najblizszy gdyz isEdgeValid sprawdza czy ten juz byl wykorzystany
            {
                dystans = graph[v][j][0];
                if(dystans < min_odleglosc)//znajduje najkrotsza odleglosc mozliwa
                {
                    min_odleglosc = dystans;
                }
            }
        }
        if(graph[v][w][0] == min_odleglosc)//jesli odleglosc miedzy v i w jest najkrotksza
        {
            return true;
        }
        return false; 
}
void TSPzachlanny::generateHamiltonianCycles(const vector<vector<vector<double>>>& graph, vector<int>& cycle, int pos,int choice2) 
{
    if (pos==cycle.size()) //if (pos == cycle.size()) 
    {
           double length = calculateHamiltonianCycleLength(graph, cycle);

        if(length < dlugosc_min)
        {
            dlugosc_min = length;

            cycle_min = cycle;
        }
        //cout<<cycle.size();
           // cout<<"graph.size() == "<<graph.size()<<endl;
            cout<<"Najkrotszy cykl: "<<dlugosc_min<<endl;
            //printHamiltonianCycle(graph, cycle_min);
            printHamiltonianCycle(graph, cycle,choice2);
        return;
    }
    for (int v = 1; v < cycle.size(); ++v) 
    {
        if (isEdgeValid(cycle[pos - 1], v, graph, cycle)&&is_Closest(cycle[pos - 1],v,graph,cycle)) 
        {
             cycle[pos] = v;//odchaczam ze ejst zajety
             //cout<<"najblizszym sasiadem "<<cycle[pos-1]<<" jest "<<v<<endl;
            generateHamiltonianCycles(graph, cycle, pos + 1,choice2);
        }
    }

}

void TSPzachlanny::hamiltonianCycles(const vector<vector<vector<double>>>& graph,int choice2)
{
    vector<int> cycle(graph.size(), -1); //wektor o rozmiaze graph.size() wypelniony -1
    cycle[0] = 0;  // Rozpocznij od pierwszego wierzchołka

    generateHamiltonianCycles(graph, cycle, 1,choice2); //1 to pozycja w wektorze na której aktualnie probujemy pojsc
}