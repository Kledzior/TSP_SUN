#include "Heurystyka.h"
#include "nlohmann/json.hpp"

#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>

using namespace std;



vector<vector<double>> Dane::calculate_Visibility_matrix(const vector<vector<vector<double>>>& graph)
    {
        int C = graph.size();
        vector<vector<double>> visibilityMatrix;//set the values to 0
        visibilityMatrix.resize(graph.size(), vector<double>(graph.size(), 0.0));

        for (int i = 0; i < C; i++)
        {
            for (int j = 0; j < C; j++)
            {
                visibilityMatrix[i][j] = static_cast<double>(C) / graph[i][j][0];
            }
        }
        return visibilityMatrix;
    }
    
HeurystykaTSP::HeurystykaTSP(double pheromoneMultiplier, double distanceMultiplier, double evaporationRate) : pheromoneMultiplier(pheromoneMultiplier), distanceMultiplier(distanceMultiplier), evaporationRate(evaporationRate) 
{
}
Mrowka::Mrowka(double pheromone, double visibility, double evaporationrate,Dane& dane_referencja) : pheromoneMultiplier(pheromone), visibilityMultiplier(visibility), evaporationRate(evaporationrate),  dane(dane_referencja)
{
    //  {
    //     cycle.push_back(0); //zaczynamy od zerowego
    //     for(int i = 1; i < dane.C; i++)
    //     {
	// 		available.insert(i); //ustawiamy i'ty jako dostepne
	// 	}
    // }
}
Mrowka::~Mrowka()
{
   // cout<<"Deleting object mrowka "<<endl;
}
bool Mrowka::Available(vector<int>& cycle, int N_miasta)//jesli znajduje sie w cyklu to nie jest dostepny
{
        for(int miasto : cycle)
            {
                if(N_miasta == miasto)
                {
                    return false;
                }
            }
            return true;
}
void Mrowka::Reset(vector<int>& cycle)//Wczesniej - > resetowało + ustawiało wszystkie procz 0 na dostepne TERAZ -> resetuje , dostepne beda sprawdzane za pomoca CYCLE
{
   // cout<<"reset - Dane.C = "<<dane.C<<endl;
    
    vector<int> temp;
    temp.push_back(0);
    cycle = temp; 
}
double Mrowka::laczny_dystans(vector<int>& cycle,const vector<vector<vector<double>>>& graph)
{
    double dystans = 0.0;
    for(int i=0;i<cycle.size()-1;i++)
    {
      //  cout<<"CYCLE SIZE   "<<cycle.size()<<endl;
      //  cout<<graph[cycle[i]][cycle[i+1]][0]<<" + ";
            dystans += graph[cycle[i]][cycle[i+1]][0];  
    }
    //cout<<graph[cycle[cycle.size()-1]][cycle[0]][0]<<endl;
    //dystans += graph[cycle[cycle.size()-1]][cycle[0]][0]; //dodaje odleglosc miedzy ostatnim a pierwszym
    return dystans;
}
void Mrowka::setPheromones(vector<int>& cycle,vector<vector<double>>& pheromoneMatrix,const vector<vector<vector<double>>>& graph)
{
    double dystans = laczny_dystans(cycle,graph); //po znalezieniu cyklu licze odleglosc
    int depositAmount = 100;
    double pheromoneAmount = depositAmount/dystans; //ilosc feromonow to 100/odleglosc
    

    for(int i=0;i<cycle.size()-1;i++)
    {
        pheromoneMatrix[cycle[i]][cycle[i+1]] += pheromoneAmount; //na dlugosci calego cyklu przypisuje ilosc feromonow (tą sama)
    }
}
vector<int> Mrowka::koniecTrasy(vector<int>& cycle,vector<vector<double>>& pheromoneMatrix,const vector<vector<vector<double>>>& graph)
{
  //  cout<<"Wszedlem do koniec trasy rozmiar cyklu przed dodaniem zerowego == "<<cycle.size()<<endl;
    setPheromones(cycle,pheromoneMatrix,graph);
    //cout<<"Ustawilem feromony"<<endl;
    cycle.push_back(cycle[0]);
    vector<int> tem = cycle;
    Reset(cycle); //resetuje cykl tak aby byly wszystkie wartosci znowu dostepne
    return tem; //zwracam cala trase wraz z (3-0); 0-1-2-3-0
}
void Mrowka::krok(vector<int>& cycle,vector<vector<double>>& pheromoneMatrix,const vector<vector<vector<double>>>& graph)
{
    dane.C = graph.size();

    //cout<<"wejscie do krok"<<endl;
    int aktualne_miasto = cycle[cycle.size()-1]; //przy pierwszym wywolaniu bedzie to 0 zakladajac ze vector cycle[0] = 0 a tak powinno byc 
  //  cout<<"aktualne miasto danej mrowki "<<aktualne_miasto<<endl; //-> DLA KAZDEJ BEDZIE TO 0
       
    double attractivness = 0.0;
    //Dane danee;
    for(int i=0;i<graph.size();i++)
    {
        if(Available(cycle,i)==true)
        {
            if(aktualne_miasto!=i)
            {
                attractivness += pow(pheromoneMatrix[aktualne_miasto][i],pheromoneMultiplier) * pow(dane.visibilityMatrix[aktualne_miasto][i],visibilityMultiplier);
               // cout<<pheromoneMatrix[aktualne_miasto][i]<<"  - feromony  "<<pheromoneMultiplier<<" - mnoznik "<<dane.visibilityMatrix[aktualne_miasto][i]<< " - widocznosc "<<visibilityMultiplier<<" - mnoznik"<<endl;
               // cout<<pow(pheromoneMatrix[aktualne_miasto][i],pheromoneMultiplier) * pow(dane.visibilityMatrix[aktualne_miasto][i],visibilityMultiplier)<<" atrakcyjnosc trasy "<<aktualne_miasto<< " --- "<<i<<endl;
               // cout<<"ŁĄCZNA ATRAKCYJNOSC  "<<attractivness<<endl;
            }
        }
    }
    double prawdopodobienstwo, losPrawd; //prawdop == stosunek atrakcyjnosci danego miasta do sumy atrakcyjnosci 
    bool moved = false; 
    double highestProb = 0; 
    double cityHighest = 0;

    random_device rd; //non deterministic random seed
    mt19937 gen(rd()); //dla tego samego ziarna rd() generuje taka sama liczbe
    uniform_real_distribution<double> dis(0.0, 1.0); //za kazdym wywolaniem zwraca liczbe losowa z przedzialu 0.0 a 1 jest ona inna poniewaz po kazdym wygenerowaniu liczby losowej zmienia sie ziarno
    for(int i = 1;i<graph.size();i++)//zaczynamod zera -> w sumie mozna od jeden jak bedzie dzialac zmienic na jeden (ZEROWY JEST POCZATKOWY)
    {
        if(Available(cycle,i)==true)
        {
           // cout<<"SPRAWDZAM DOSTEPNOSC MIASTA  "<<i<<endl;
           // cout<<"ŁĄCZNA ATRAKCYJNOSC  "<<attractivness<<endl;
            //cout<<"i - "<<i<<endl;
           // cout<<pow(pheromoneMatrix[aktualne_miasto][i], pheromoneMultiplier) * pow(dane.visibilityMatrix[aktualne_miasto][i], visibilityMultiplier)<<" atrakcyjnosc danego miasta"<<endl;;

			prawdopodobienstwo = pow(pheromoneMatrix[aktualne_miasto][i], pheromoneMultiplier) * pow(dane.visibilityMatrix[aktualne_miasto][i], visibilityMultiplier);
            
            prawdopodobienstwo /= attractivness; //stosuken atrakcyjnosci danego miasta do sumy atrakcyjnosci
           /// cout<<"---- STOSUNEK ATRAKCYJNOSCI TEJ TRASY DO WSZYSTKICH MOZLIWYCH    "<<prawdopodobienstwo<<endl;
			//cout<<prawdopodobienstwo<<"---Stosunek prawdopodobienstwa do atrakcyjnosci [z][do]"<<endl;
           // cout<<highestProb<<" Highest prop -> bedzie update'owane jak jest mniejsze niz stosunek prawdopod do atrakcyjn"<<endl;
            if (prawdopodobienstwo > highestProb) //jak jest najbardziej atrakcyjne to 
            {
				cityHighest = i; //przypisujemy wartosc zawarta w available[i] do cityHighest
                //cout<<cityHighest<<" -- Miasto z najwiekszym prawdopodobienstwem -> update highestPob"<<endl;
				highestProb = prawdopodobienstwo;
                //cout<<highestProb<<"---Powinno byc rowne stosunkowi prawdopodobienstwa"<<endl;
			}
            //czynnik losowy -> jesli losowe prawdopodobienstwo jest <= aktualne prawdopodobienstwo
            //to zaznaczamy ze wykonalismy krok, do cyklu wrzucamy indeks miasta , i usuwamy go z dostepnych 
            // po czym wychodzimy z petli 
			losPrawd = dis(gen); //generuje liczbe losowa od 0.0 do 1.0
			if (losPrawd <= prawdopodobienstwo) 
            {
               // cout<<"Umiejscowiony bo przeszedl test losowy tzn ma prawdopodob z zekresu 0-1 i jest wieksze nic losPrawd ktore jest rowne - "<<losPrawd<<endl;
				moved = true;
				cycle.push_back(i);
              //  cout<<"aktualne miasto danej mrowki PO WSTAWIENIU "<<aktualne_miasto<<endl;
				break;//jak umiejscowie nie sprawdzam kolejnych mozliwosci 
			}
        }
    }
    if(!moved&&cycle.size()) //jesli losowo nie przypisalismy to robimy ruch do miasta ktore ma najwieksze prawdopodobienstwo
        //czyli najwieksza atrakcyjnosc
        {
          //  cout<<"Wstawilem miasto z najwiekszym prawdopodobienstwem  - "<<cityHighest<<endl;
			cycle.push_back(cityHighest);
		}
}


void Mrowka::hamiltonianCycles(const vector<vector<vector<double>>>& graph,double pheromoneMultiplier,double distanceMultiplier,double evaporationRate)
{
   
    vector<vector<double>>& pheromoneMaatrix = dane.pheromoneMatrix; //referencja do obiektu 
    if(flaga2==0)
    {
        dane.pheromoneMatrix.assign(graph.size(),vector<double>(graph.size(),graph.size()));
        flaga2 = 1;
    }

    unsigned int C_mrowek = (graph.size()*graph.size());
    unsigned int C_iteracje = graph.size();
    vector<vector<double>> macierz_widocznosc;
    vector<Mrowka> mrowki;
    double min_dlugosc = numeric_limits<double>::max();
    double koszt_aktualnej = 0.0;
    vector<int> min_cycle;
    Dane macierz;
    Mrowka turbo_grosik(pheromoneMultiplier,distanceMultiplier,evaporationRate,macierz);  

    macierz_widocznosc = macierz.calculate_Visibility_matrix(graph); //the values are calculated but where are they stored???? -> quite possibly they're not stored at all
    //cout<<macierz_widocznosc[0][0]<<endl;

    macierz.visibilityMatrix.resize(graph.size(), vector<double>(graph.size(), 0.0));
    for(int i = 0;i<graph.size();i++)
    {
        for(int j = 0;j<graph.size();j++)
        {   
            macierz.visibilityMatrix[i][j] = macierz_widocznosc[i][j];
        }
    }

    
    for(int i = 0; i < C_mrowek; i++) 
    {
           // cout<<"++++++++++NOWA MROWKA++++++++++"<<endl;
			Mrowka Mrowkaa(pheromoneMultiplier, distanceMultiplier, evaporationRate, macierz);
			mrowki.push_back(Mrowkaa);
	}
    chrono::steady_clock::time_point begin = chrono::steady_clock::now(); //zaczynamy odmierzac czas
    
    vector<vector<int>> cycle(C_mrowek);//ZMIANY CYKL -> PIERWSZA ZMIANA DO DWUWYMIAROWEGO CYKLU 

    for(int i = 0;i<C_mrowek;i++)
    {
        cycle[i].push_back(0);//Kazda mrowka zaczyna od zera
    }
    for(int i=0;i<C_iteracje;i++) // C_iteracje = C;
    {
       // cout<<"ITERACJA NR "<<i<<endl;
        for(int j=0;j<graph.size()-1;j++)//C
        {  
        
       // cout<<"Kazda mrowka zrobi teraz krok (iteracja "<<i<<")"<<" krok nr - "<<j<<endl;
            for(int k = 0 ; k<C_mrowek;k++)//C_mrowek == 1 w chwili obecnej (DO zmiany)
            {
               // cout<<C_mrowek<<" C MROWEK"<<endl;
                //cout<<k<<"  k"<<endl;
              //  cout<<"Mrowka nr "<<k<<" robi krok"<<endl;
                mrowki[k].krok(cycle[k],pheromoneMaatrix,graph);
            }  
        }

        for(int l=0;l<C_mrowek;l++)
        {
           
            vector<int> p = mrowki[l].koniecTrasy(cycle[l],pheromoneMaatrix,graph);
            if(!min_cycle.size())//jesli cykl minimalny jest pusty
            {
                min_cycle = p;
                min_dlugosc = laczny_dystans(p,graph);
               // cout<<min_dlugosc<<" DLUGOSC MINIMALNA"<<endl;
                continue;
            }
            koszt_aktualnej = laczny_dystans(p,graph);
           // cout<<koszt_aktualnej<<"      koszt aktualnej trasy"<<endl;
            if(koszt_aktualnej < min_dlugosc)
            {
                //cout<<"UPDATE  "<<endl;
                min_dlugosc = koszt_aktualnej;
                min_cycle = p;
            }
        }

        //turbo_grosik.cycle = min_cycle;
        //turbo_grosik.setPheromones(min_cycle,pheromoneMaatrix,graph);
        for(int m = 0;m<graph.size();m++)
        {

            for(int n=0;n<graph.size();n++)
            {   
                dane.pheromoneMatrix[m][n] *= evaporationRate;
            }
        }

    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    for(int o=0;o<graph.size();o++)
    {
        cout<<min_cycle[o] << "-";
    }
    cout<< min_cycle[graph.size()] << "\nefektywna dlugosc trasy wynosi: "<<min_dlugosc<<endl;
    cout<<"Time spent during the process of finding the smallest cycle = "<<chrono::duration_cast<chrono::microseconds> (end - begin).count() << "[micro s] == " <<chrono::duration_cast<chrono::milliseconds> (end - begin).count()<<" [mili s]"<<endl;
    string filename = "TSP_5_Ant_Colony_Opt_wynik";
    nlohmann::json wynik0;
    nlohmann::json wynik1;
    wynik1["dlugosc"] = min_dlugosc;
    wynik1["cykl"] = min_cycle;
    wynik0.push_back(wynik1);
    string filePath = "./Wyniki_S0/Wyniki_C"+ to_string(graph.size()) + "/" + filename + ".json";
    ofstream file(filePath);
    if(!file.is_open())
    {
        cerr<<"Unable to save the results in JSON"<<endl;
        return;
    }
    file << wynik0.dump(5);
    file.close();
}
