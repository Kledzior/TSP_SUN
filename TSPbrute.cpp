#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>
#include <fstream>
#include <chrono>

#include "TSPbrute.h"
#include "nlohmann/json.hpp"

using namespace std;



const double dlugosc_min = numeric_limits<double>::max();
const int ilosc_cykli=0;
const vector<int> cycle_min;


int TSPbrute::silnia(int liczba)
    {
        int wynik=1;
        while(liczba>1)
        {
            wynik = liczba*wynik;
            liczba--;
        }
        return wynik;
    }
void TSPbrute::printHamiltonianCycle(const vector<vector<vector<double>>>& graph, const vector<int>& cycle, int choice2) 
{
    vector<int> cycle2; //tworze nowy vektor aby zawieral caly cykl wraz z zerowym na koncu
    //jest to po to aby moc po prostu przekazac go do json
    for(int i=0;i<=cycle.size();i++)
    {
        if(i == cycle.size())
        {
            cycle2.push_back(cycle[0]);
        }
        else
        {
            cycle2.push_back(cycle[i]);
        }
    }
    string filename;
    
    if(choice2 == 0)
    {

        filename = "TSP_0_brute_wynik";
    }
    if(choice2 == 1)
    {
        filename = "TSP_1_zachl_wynik";
    }
    if(choice2 == 2)
    {
        filename = "TSP_2_zachl_dokl_wynik";
    }
    if(choice2 == 3)
    {
        filename = "TSP_3_zachl_los_wynik";
    }
    if(choice2 == 4)
    {
        filename =  "TSP_4_zachl_los_dokl_wynik";
    }
    nlohmann::json wynik0;
    nlohmann::json wynik1;
    wynik1["dlugosc"] = dlugosc_min;
    wynik1["cykl"] = cycle2;
    wynik0.push_back(wynik1);
    // string filePath = "./Wyniki_C"+graph.size()+"/" + filename + ".json";
    // cout<<cycle.size()<<endl;
    
    string filePath = "./Wyniki_S0/Wyniki_C" + to_string(cycle.size()) + "/" + filename + ".json"; //tworzenie sciezki dostepu
    cout <<filePath;
    ofstream file(filePath);
    if(!file.is_open())
    {
        cerr<<"Unable to save the results in JSON"<<endl;
        return;
    }
    file << wynik0.dump(5);
    file.close();
    for (int vertex : cycle2) 
    {
        cout << vertex << " ";
    }
    //cout << cycle[0] << endl;
    cout << "\nDługość cyklu: " << dlugosc_min << endl;
    dlugosc_min = numeric_limits<double>::max();
    ilosc_cykli=0;
    cycle_min.clear();
}

double TSPbrute::calculateHamiltonianCycleLength(const vector<vector<vector<double>>>& graph, const vector<int>& cycle) {
    double length = 0.0;

    for (int i = 0; i < cycle.size() - 1; ++i) {
        int v = cycle[i];
        int w = cycle[i + 1];
        length += graph[v][w][0];  // Sumuj długości krawędzi
    }
    length += graph[cycle.back()][cycle[0]][0];

     return length;
}

bool TSPbrute::isEdgeValid(int v, int w, const vector<vector<vector<double>>>& graph, const vector<int>& cycle) {
    // Sprawdź, czy istnieje krawędź między v i w oraz czy w nie został jeszcze odwiedzony
    return graph[v][w][1] == 1 && find(cycle.begin(), cycle.end(), w) == cycle.end();
}

void TSPbrute::generateHamiltonianCycles(const vector<vector<vector<double>>>& graph, vector<int>& cycle, int pos,int choice2) {
    if (pos == cycle.size()) //jesli wszystkie zostaly miasta odwiedzone
    {
        ilosc_cykli++;
        vector<vector<vector<double>>> graph_max;
        // Cykl Hamiltona został znaleziony
        // printHamiltonianCycle(graph, cycle);
        // double length = calculateHamiltonianCycleLength(graph, cycle);
        // cout << "Długość cyklu: " << length << endl;
           double length = calculateHamiltonianCycleLength(graph, cycle);
           
           
        if(length < dlugosc_min)
        {
            dlugosc_min = length;
            graph_max = graph;
            cycle_min = cycle;
        }
        pos = silnia(pos-1);
        if(ilosc_cykli == pos)
        {
            cout << "Najkrotszy cykl: " << dlugosc_min << endl;
            printHamiltonianCycle(graph_max, cycle_min,choice2);
        }
       
        return;
    }

    for (int v = 1; v < cycle.size(); ++v) {
        if (isEdgeValid(cycle[pos - 1], v, graph, cycle)) {
            cycle[pos] = v;//odchaczam ze ejst zajety
            generateHamiltonianCycles(graph, cycle, pos + 1,choice2);
            cycle[pos] = -1;  // Reset, aby spróbować inne kombinacje
        }
    }
}

void TSPbrute::hamiltonianCycles(const vector<vector<vector<double>>>& graph,int choice2)
{
    vector<int> cycle(graph.size(), -1);
    cycle[0] = 0;  // Rozpocznij od pierwszego wierzchołka

    generateHamiltonianCycles(graph, cycle, 1,choice2);
}




