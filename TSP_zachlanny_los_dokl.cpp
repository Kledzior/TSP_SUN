#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

#include "TSPbrute.h"
#include "TSP_zachlanny_los_dokl.h"

#include "nlohmann/json.hpp"

using namespace std;

const double dlugosc_min = numeric_limits<double>::max();
const int ilosc_cykli=0;
const vector<int> cycle_min;

//liczy zachlannie cykl wybierajac za kazdym razem najblizsze miasto startujac od 0 
//ELEMENT LOSOWY - parzyste miasta o indeksie wiekszym niz poprzedni maja dodatkowe 27% szans na wybranie


void TSPzachlanny_los_dokl::printHamiltonianCycle(const vector<vector<vector<double>>>& graph, const vector<int>& cycle,int choice2)
{
    TSPbrute::printHamiltonianCycle(graph,cycle,choice2);
}
double TSPzachlanny_los_dokl::calculateHamiltonianCycleLength(const vector<vector<vector<double>>>& graph, const vector<int>& cycle) 
{
     return TSPbrute::calculateHamiltonianCycleLength(graph, cycle);
}
bool TSPzachlanny_los_dokl::isEdgeValid(int v, int w, const vector<vector<vector<double>>>& graph, const vector<int>& cycle) 
{
    return TSPbrute::isEdgeValid(v,w, graph, cycle);
}
bool TSPzachlanny_los_dokl::czynnik_losowy(int v,int w)
{
    if(w%2 == 0 && v < w)//jesli jest to miasto parzyste oraz numer tego miasta jest wiekszy to mamy dodatkowe 27% szans na to ze zostanie dodane
    {
        if(rand() % 100 < 27)
        {
             return true;
        }
       
    }
    return false;
}
bool TSPzachlanny_los_dokl::is_Closest(int v, int w, const vector<vector<vector<double>>>& graph, const vector<int>& cycle)//v - od miasta A , w - do miasta B , z graph , ma byc minimalna odleglosc, cycle - sprawdzamy czy jest zajety
{   
   double min_odleglosc = numeric_limits<double>::max();
   double dystans;
   //zaczynam zawsze od zerowego a teraz sprawdzam ktory jest najblizszy zerowemu stad moge iterowac od 1
   
        for(int j=0;j<graph.size();j++)
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
void TSPzachlanny_los_dokl::generateHamiltonianCycles(const vector<vector<vector<double>>>& graph, vector<int>& cycle, int pos,int liczba,int choice2) 
{
   if (liczba==cycle.size()-1) //if (pos == cycle.size()) 
    {
        ilosc_cykli++;
           double length = calculateHamiltonianCycleLength(graph, cycle);

        if(length < dlugosc_min) //update if smaller
        {
            dlugosc_min = length;

            cycle_min = cycle;
        }
        
       if(ilosc_cykli == graph.size())
        {    
        cout<<"Najkrotszy cykl: "<<dlugosc_min<<endl;
        printHamiltonianCycle(graph, cycle_min,choice2);
        }            
        return;
    }
    for (int v = 0; v < cycle.size(); ++v) 
    {
     //cout<<"wszedlem do petli i sprawdzam "<<cycle[pos-1]<<" - "<<v<<endl;
        if (isEdgeValid(cycle[pos - 1], v, graph, cycle)&&is_Closest(cycle[pos - 1],v,graph,cycle)||isEdgeValid(cycle[pos - 1], v, graph, cycle)&&czynnik_losowy(cycle[pos-1],v)) 
        {
             cycle[pos] = v;//odchaczam ze ejst zajety -> pos to jest przy którym z kolei jestesmy pos = 2 nie znaczy ze jestem w drugim miescie tylko trzeci element cyklu (bo indeks od 0)
           // cout<<"najblizszym sasiadem "<<cycle[pos-1]<<" jest "<<v<<endl;
           // cout<<liczba<<" "<<(cycle.size()-1)<<endl;
            generateHamiltonianCycles(graph, cycle, pos + 1,++liczba,choice2);
            

        }
    }
    //cout<<"------------------------"<<endl;
}
void TSPzachlanny_los_dokl::hamiltonianCycles(const vector<vector<vector<double>>>& graph,int choice2)
{
    vector<int> cycle(graph.size(), -1);
        
    for(int i=0;i<cycle.size();i++)//<= zeby zaczal od kazdego dla 5 miast cycle size jest 5 czyli bedzie 0,1,2,3,4 a musi jeszcze zaczac od 5
    {
        cycle[0] = i; //zaczynamy od i'tego
        generateHamiltonianCycles(graph, cycle, 1, 0,choice2); 
        for(int j=0;j<graph.size();j++)
        {
            cycle[j] = -1;
        }
    }
}
