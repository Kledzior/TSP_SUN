#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>
#include <fstream>

#include "Gen.h"
#include "nlohmann/json.hpp"

using namespace std;

Gen::Gen(){}; // konstruktor generatora

void Gen::generate(const string& fileName, unsigned int C, double Efficiency)
{
    ///////////////////////////////////////////////////////////////////////////////////////////////
    int tab_C[C][2];//macierz miast
    nlohmann::json lokalizacje;
    for(int i=0;i<C;i++)//miasto nr i tab_C[0][0] -> X miasta 0 , tab_C[0][1] - > Y miasta 0
    {
        for(int j=0;j<2;j++)// j=0 wartosc X , j=1 Wartosc Y
        {
            tab_C[i][j] = rand() % 1000; //random in the range of 0 to 999   
        }
            nlohmann::json miasto;
            miasto["id"] = i;
            miasto["X"] = tab_C[i][0];
            miasto["Y"] = tab_C[i][1];
            
            lokalizacje.push_back(miasto);
    }
    string filePath_lokaliza = "./dane_generator/lokalizacje.json"; //tworzenie ścieżki dostępu

        ofstream file_lokalizacje(filePath_lokaliza);

        if(!file_lokalizacje.is_open())
        {
            cerr<<"Unable to open JSON file containing localizations of cities"<<endl; //troche jak perror w linuxie 
            return;
        }
        file_lokalizacje << lokalizacje.dump(5); //przekazuje dane z dystanse_tab do file
        file_lokalizacje.close();
    double tab_krawedzie[C][C];
    for(int i = 0;i<C;i++)
    {
        for(int j = 0;j<C;j++)
        {
            if(i!=j)
            {
                tab_krawedzie[i][j] = sqrt((tab_C[i][0]-tab_C[j][0])*(tab_C[i][0]-tab_C[j][0]) + (tab_C[i][1]-tab_C[j][1])*(tab_C[i][1]-tab_C[j][1]));
            }
            
        }
    }
    double tab_nasloneczienie[C][C];
    
    for(int i=0;i<C;i++)
    {
        for(int j = 0;j<C;j++)
        {
            if(i<j)
            {
                tab_nasloneczienie[i][j] = rand() % 100;
            }
        }
    }
  
    nlohmann::json naslonecznienie;
    for(int i=0;i<C;i++)
    {
        for(int j = 0;j<C;j++)
        {
            if(i>j)
            {
                tab_nasloneczienie[i][j] = tab_nasloneczienie[j][i];
            }
            if(i!=j)
            {
                nlohmann::json slonce_na_trasie;
                slonce_na_trasie["id_start"] = i;
                slonce_na_trasie["id_koniec"] = j;
                slonce_na_trasie["naslonecznienie"] =  tab_nasloneczienie[i][j];   
                naslonecznienie.push_back(slonce_na_trasie);
            }
   
        }
    }
    string filePath_naslonecznienie = "./dane_generator/naslonecznienie.json";

    ofstream file_naslonecznienie(filePath_naslonecznienie);

    if(!file_naslonecznienie.is_open())
    {
        cerr<<"Unable to open JSON file containing naslonecznienie on the roads"<<endl;
        return;
    }
    file_naslonecznienie<<naslonecznienie.dump(5);
    file_naslonecznienie.close();

    double tab_efektywny_dystans[C][C];
    for(int i=0; i<C;i++)
    {
        for(int j=0;j<C;j++)
        {
            if(i!=j)
            {
                if (Efficiency==0)
                {
                    tab_efektywny_dystans[i][j] = tab_krawedzie[i][j]; //gdy efficiency jest 0 nie moge wklepac do wzoru bo bedzie dziel przez 0
                }
                else
                {
                    tab_efektywny_dystans[i][j] = tab_krawedzie[i][j] * (1 - (tab_nasloneczienie[i][j]*(Efficiency/100))/100);//gdy mam tab_naslonecznienie[i][j]/2 to zaklada ze skutecznosc wynosi 50% tzn o 50% zmniejszamy
                }
            }
            else
            {
                tab_efektywny_dystans[i][j] = 0;
            }
        }
    }
    vector<vector<vector<double>>> graph(C, vector<vector<double>>(C, vector<double>(2, -1)));
    for (int i=0;i<C;i++)
    {
        for(int j=0;j<C;j++)
        {
            if(i!=j)
            {
                graph[i][j][1] = 1; //jest dystans 
            }                  
        }
    }
    for(int i=0;i<C;i++)
    {
        for(int j=0;j<C;j++)
        {
            graph[i][j][0] = tab_efektywny_dystans[i][j]; //wpisana odleglosc
        }

    }
    nlohmann::json dystanse_tab; //obiekt typu json
    for(int i=0;i<C;i++)
    {
        for(int j=0;j<C;j++)
        {
            if(i!=j)
            {
             nlohmann::json dystanse;   //obiekt typu json
             dystanse["id_start"] = i;
             dystanse["id_koniec"] = j;
             dystanse["Dystans"] = graph[i][j][0];

             dystanse_tab.push_back(dystanse);
            }
            
        } 
    }
    string filePath = "./dane_generator/" + fileName + ".json"; //tworzenie ścieżki dostępu

        ofstream file(filePath);

        if(!file.is_open())
        {
            cerr<<"Unable to open JSON file"<<endl; //troche jak perror w linuxie 
            return;
        }
        file << dystanse_tab.dump(5); //przekazuje dane z dystanse_tab do file
        file.close();
}
    



// double dlugosc_min = numeric_limits<double>::max();
// int ilosc_cykli=0;

// vector<int> cycle_min;
// int silnia(int liczba)
// {
//     int wynik=1;
//     while(liczba>1)
//     {
//         wynik = liczba*wynik;
//         liczba--;
//     }
//     return wynik;
// }
// void printHamiltonianCycle(const vector<vector<vector<double>>>& graph, const vector<int>& cycle) {
//     for (int vertex : cycle) {
//         cout << vertex << " ";
//     }
//     cout << cycle[0] << endl;
// }

// double calculateHamiltonianCycleLength(const vector<vector<vector<double>>>& graph, const vector<int>& cycle) {
//     double length = 0.0;

//     for (int i = 0; i < cycle.size() - 1; ++i) {
//         int v = cycle[i];
//         int w = cycle[i + 1];
//         length += graph[v][w][0];  // Sumuj długości krawędzi
//     }
//     length += graph[cycle.back()][cycle[0]][0];

//      return length;
// }

// bool isEdgeValid(int v, int w, const vector<vector<vector<double>>>& graph, const vector<int>& cycle) {
//     // Sprawdź, czy istnieje krawędź między v i w oraz czy w nie został jeszcze odwiedzony
//     return graph[v][w][1] == 1 && find(cycle.begin(), cycle.end(), w) == cycle.end();
// }

// void generateHamiltonianCycles(const vector<vector<vector<double>>>& graph, vector<int>& cycle, int pos) {
//     if (pos == cycle.size()) //jesli wszystkie zostaly miasta odwiedzone
//     {
//         ilosc_cykli++;
//         vector<vector<vector<double>>> graph_max;
//         // Cykl Hamiltona został znaleziony
//         // printHamiltonianCycle(graph, cycle);
//         // double length = calculateHamiltonianCycleLength(graph, cycle);
//         // cout << "Długość cyklu: " << length << endl;
//            double length = calculateHamiltonianCycleLength(graph, cycle);
           
           
//         if(length < dlugosc_min)
//         {
//             dlugosc_min = length;
//             graph_max = graph;
//             cycle_min = cycle;
//         }
//         pos = silnia(pos-1);
//         if(ilosc_cykli == pos)
//         {
//             cout << "Najkrotszy cykl: " << dlugosc_min << endl;
//             printHamiltonianCycle(graph_max, cycle_min);
//             cout << "Długość cyklu: " << dlugosc_min << endl;
            
//         }
       
//         return;
//     }

//     for (int v = 1; v < cycle.size(); ++v) {
//         if (isEdgeValid(cycle[pos - 1], v, graph, cycle)) {
//             cycle[pos] = v;//odchaczam ze ejst zajety
//             generateHamiltonianCycles(graph, cycle, pos + 1);
//             cycle[pos] = -1;  // Reset, aby spróbować inne kombinacje
//         }
//     }
// }

// void hamiltonianCycles(const vector<vector<vector<double>>>& graph) {
//     vector<int> cycle(graph.size(), -1);
//     cycle[0] = 0;  // Rozpocznij od pierwszego wierzchołka

//     generateHamiltonianCycles(graph, cycle, 1);
// }

