// #include "Samochod.h"
// #include <iostream>



#include "Gen.h"
#include "nlohmann/json.hpp"
#include "TSPbrute.h"
#include "TSP_zachlanny.h"
#include "TSP_zachlanny_dokl.h"
#include "TSP_zachlanny_los.h"
#include "TSP_zachlanny_los_dokl.h"
#include "Heurystyka.h"
#include <chrono>

#include <direct.h>
#include <iostream>
#include <fstream>

using namespace std;

int main() 
    {
    if (_chdir("C:/Users/Admin/Documents/Studia/OK/TSP_Panele_sloneczne/") != 0)
     { 
        cerr << "Unable to change workfolder" <<endl;
        return 1;
    }
    ifstream dataFile;
    string name;
    unsigned int C;
     string filename;
    char choice;

    cout<<"Do you want to use generator in order to generate data? (y/n)"<<endl;
    cin>>choice;

    if(choice == 'y'||choice == 'Y')
    {
    //GENERATOOOOOOORR
    cout<<"Generating a data file "<<endl;
   // unsigned int C;
    double Efficiency;
    Gen generator; //obiekt klasy Gen o nazwie generatror
    cout<< "\nPlease input filename and the number of cities and the efficiency of the solar system <0;100>"<<endl; //moge dac 100 ze wzgledu na wzor ktory uwzglednia naslonecznienie danego miejsca
    cin>> filename >> C >> Efficiency;   
    while(100<Efficiency || Efficiency <0 || C <= 1)
    {
        if(100<Efficiency || Efficiency <0)
        {
            cout<<"Efficiency has to be from 0 to 100 range"<<endl;
            cout<<"Input efficiency: ";
            cin>>Efficiency;
        }
        else if(C<=1)
        {
            cout<<"The number of cities ought to be bigger"<<endl;
            cout<<"Input the number of cities: ";
            cin>>C;
        }
    }
    generator.generate(filename,C,Efficiency);
    //GENERATOOOOOOORR
    }
    if(choice == 'n' || choice == 'N')
    {
        double Efficiency2;
        string fileName1,fileName2,filepath1,filepath2;
        cout<<"Taking data from file"<<endl;
        cout<<"Please insert name of to the file in which you stored localisations of cities"<<endl;
        cout<<"Insert the name of the file in which you have the insolation aswell "<<endl;
        cin>>fileName1>>fileName2;
        filepath1 = "./dane_odreczne/" + fileName1 + ".json";
        filepath2 = "./dane_odreczne/" + fileName2 + ".json";
        cout<<"You need to insert the efficiency of the solar system"<<endl;
        cin>>Efficiency2;
        while(100<Efficiency2 || Efficiency2 <0)
        {
            cout<<"Efficiency has to be from 0 to 100 range"<<endl;
            cout<<"Input efficiency: ";
            cin>>Efficiency2;
        }
        
        ifstream file1(filepath1);
        ifstream file2(filepath2);
         if(!file1.is_open())
        {
            cerr<<"Unable to open file containing localisations"<<endl; //troche jak perror w linuxie 
            return 1;
        }
         if(!file2.is_open())
        {
            cerr<<"Unable to open file containing insolation"<<endl; //troche jak perror w linuxie 
            return 1;
        }
        nlohmann::json data_lokaliza;
        nlohmann::json data_naslonecznienie;
        file1 >> data_lokaliza;  //przekazuje dane z pliku lokalizacje.json do obiektu json data_lokaliza
        file2 >> data_naslonecznienie;  //przekazuje dane z pliku naslonecznienie.json do obiektu json data_naslonecznienie
        file1.close();
        file2.close();
        int id,X,Y;
        C = 0;
        for(const auto& lokaliza : data_lokaliza)//ilosc miast
        {
            id = lokaliza["id"];
            if (id >= C)
            {
                C = id; //C bedzie przechowywac ilosc miast
            }
        }
        double tab_C[C][2];
        for(const auto& lokaliza : data_lokaliza)
        {
            id = lokaliza["id"];
            X = lokaliza["X"];
            Y = lokaliza["Y"];
            tab_C[id-1][0] = X; //gdyz id indeksuje od 1 a to ze indeksuje od 1 przydaje sie przy liczeniu ilosci miast - kwestia umowna 
            tab_C[id-1][1] = Y;
            //cout<<id<<endl;
            //cout<<tab_C[id][0]<<" "<<tab_C[id][1]<<endl;//ma dobre wartosci
            //cout<<id<<" "<<X<<" "<<Y<<endl//dobrze wczytuje dane
        }
        double tab_krawedzie[C][C];
        for(int i = 0;i<C;i++)
        {
            for(int j = 0;j<C;j++)
            {
                if(i!=j)
                {
                    tab_krawedzie[i][j] = sqrt((tab_C[i][0]-tab_C[j][0])*(tab_C[i][0]-tab_C[j][0]) + (tab_C[i][1]-tab_C[j][1])*(tab_C[i][1]-tab_C[j][1]));
                    //zawiera poprawne krawedzie
                    //cout<<tab_krawedzie[i][j]<<endl;
                }
                
            }
        }
        double tab_naslonecznienie[C][C];
        int i;
        int j;
        for(const auto& slonce : data_naslonecznienie)
        {
            i = slonce["id_start"];
            j = slonce["id_koniec"];
            tab_naslonecznienie[i-1][j-1] = slonce["naslonecznienie"]; //po raz kolejny - indeksuje krawedzie od 1
        }
        double tab_efektywny_dystans[C][C];
        for(int i=0; i<C;i++) 
        {
            for(int j=0;j<C;j++)
            {
                if(i!=j)
                {
                    if (Efficiency2==0)
                    {
                        tab_efektywny_dystans[i][j] = tab_krawedzie[i][j]; //gdy efficiency jest 0 nie moge wklepac do wzoru bo bedzie dziel przez 0
                        //cout<<tab_krawedzie[i][j]<<endl;
                    }
                    else
                    {
                        tab_efektywny_dystans[i][j] = tab_krawedzie[i][j] * (1 - (tab_naslonecznienie[i][j]*(Efficiency2/100))/100);//gdy mam tab_naslonecznienie[i][j]/2 to zaklada ze skutecznosc wynosi 50% tzn o 50% zmniejszamy
                        //cout<<tab_efektywny_dystans[i][j]<<" "<<tab_krawedzie[i][j]<<" "<<tab_naslonecznienie[i][j]<<" "<<Efficiency2<<endl;
                    }
                }
                else
                {
                    tab_efektywny_dystans[i][j] = 0;
                }
            }
        }
            //cout<<"-----------------------"<<endl;
            for(int k=0;k<C;k++)
            {
                for(int l=0;l<C;l++)
                {
                    if(k!=l)
                    {
                        // cout<<tab_efektywny_dystans[k][l]<<endl;
                    }
                   
                }
            }
            vector<vector<vector<double>>> graph2(C, vector<vector<double>>(C, vector<double>(2, -1)));
            for (int k=0;k<C;k++)
                {
                    for(int l=0;l<C;l++)
                    {
                        if(k!=l)
                        {
                            graph2[k][l][1] = 1; //jest dystans 
                        }                  
                    }
                }
            for(int k=0;k<C;k++)
                {
                    for(int l=0;l<C;l++)
                    {
                        graph2[k][l][0] = tab_efektywny_dystans[k][l]; //wpisana odleglosc
                    }

                }
            nlohmann::json dystanse_tab;
            for(int k=0;k<C;k++)
            {
                for(int l=0;l<C;l++)
                {
                    if(k!=l)
                    {
                    nlohmann::json dystanse;   
                    dystanse["id_start"] = k;
                    dystanse["id_koniec"] = l;
                    dystanse["Dystans"] = graph2[k][l][0];

                    dystanse_tab.push_back(dystanse);
                    }   
                } 
            }
            string filePath = "./dane_odreczne/dystans_efektywny.json"; //tworzenie sciezki dostepu
            ofstream file(filePath);
            if(!file.is_open())
            {
                cerr<<"Unable to open JSON file - the one created from lokalizacje and naslonecznienie"<<endl; //troche jak perror w linuxie 
                return 1;
            }
            file << dystanse_tab.dump(5); //przekazuje dane z dystanse_tab do file
            file.close();
            filename = "dystans_efektywny";
        }
        //teraz te dane przemielone zrobic tak aby byly jak w pliku miasta.json 
        //pozniej jesli choice was 'y' name = filename else name = moj_nowy_plik; 
    //name = filename; //name = "./dane_generator/miasta.json";
    if(choice == 'y'|| choice == 'Y')
    {
        name = "./dane_generator/" + filename + ".json";
    }
    if(choice == 'n'|| choice == 'N')
    {
        name = "./dane_odreczne/" + filename + ".json";
    }
    dataFile.open(name);

    if (!dataFile.is_open())
    {
        cerr << "Unable to open JSON file" <<endl;
        return 1;
    }

    nlohmann::json data;
    
    dataFile >> data; //przekazuje dane z pliku miasta.json do obiektu json data
    dataFile.close();
    int k,l;
    for (const auto& odleglosci : data) 
    {
    k = odleglosci["id_start"];
    l = odleglosci["id_koniec"];

    // Update C based on the current city indices
    C = max(C, static_cast<unsigned int>(max(k,l) + 1));
    }
    //potrzebuje moj vektor ktory miele w kodzie tsp brute tzn 
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
    int i;
    int j;
    for (const auto& odleglosci : data) 
    {
        i = odleglosci["id_start"];
        j = odleglosci["id_koniec"];
        graph[i][j][0] = odleglosci["Dystans"];
    }
    //cout<<n<<endl;
    //mam wzięte z  miasta.json dane do wektora graph teraz musze na nim policzyć cykl hamiltona 
    TSPbrute TspSolver;
    TSPzachlanny TspSolver_zachlanny;
    TSPzachlanny_dokl TspSolver_zachlanny_dokl;
    TSPzachlanny_los TspSolver_zachlanny_los;
    TSPzachlanny_los_dokl TspSolver_zachlanny_los_dokl;
    
    double o = 2.2;
    double p = 2.3;
    double r = 2.4;
    int choice2 = -2;

    do 

    {
        cout<<"wprowadz wybor\n'0' - hamiltonburteforce\n'1' - hamilton_zachlannie\n'2' - hamilton_zachlannie_dokladnie\n'3' - hamilton_zachlannie_losowo\n'4' - hamilton_zachlanny_dokl_los\n'5' - heurystyka\n'-1' - quit"<<endl;
        cin>>choice2;

            if(cin.fail()) 
            {
            cin.clear();  // resetuj stan strumienia //choice2 bedzie rowne 0 po resecie 
            choice2 = -2; //stad powracam jego wartosc do -2 -> moglbym tez zadeklarowac 0 na starcie i wybory iterowac od 1 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // ignoruj błędne dane
            cout << "Nieprawidłowe dane. Wprowadź liczbę.\n";   
            }



            if(choice2==0)
            {
                chrono::steady_clock::time_point begin = chrono::steady_clock::now();
                TspSolver.hamiltonianCycles(graph,choice2);
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                auto elapsed_time = chrono::duration_cast<chrono::microseconds>(end - begin).count();
                
                string filename_time_0 = "TSP_0_brute_Czas";
                nlohmann::json czas0;
                czas0["execution_time"] = elapsed_time;
                 
                string filepath = "./Wyniki_Czas/C"+to_string(graph.size())+"/"+filename_time_0+".json";
                
                ofstream file_time_0(filepath);
                if(!file_time_0.is_open())
                {
                    cerr<<"Unable to save execution time in JSON"<<endl;
                    return 0;
                }
                file_time_0 << czas0.dump(2);
                file_time_0.close();
            }
            if(choice2==1)
            {
                chrono::steady_clock::time_point begin = chrono::steady_clock::now();
                TspSolver_zachlanny.hamiltonianCycles(graph,choice2);
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                auto elapsed_time = chrono::duration_cast<chrono::microseconds>(end - begin).count();
                
                string filename_time_1 = "TSP_1_zachl_Czas";
                nlohmann::json czas0;
                czas0["execution_time"] = elapsed_time;
                 
                string filepath = "./Wyniki_Czas/C"+to_string(graph.size())+"/"+filename_time_1+".json";
                
                ofstream file_time_1(filepath);
                if(!file_time_1.is_open())
                {
                    cerr<<"Unable to save execution time in JSON"<<endl;
                    return 0;
                }
                file_time_1 << czas0.dump(2);
                file_time_1.close();
            }
            if(choice2==2)
            {
                chrono::steady_clock::time_point begin = chrono::steady_clock::now();
                TspSolver_zachlanny_dokl.hamiltonianCycles(graph,choice2);
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                auto elapsed_time = chrono::duration_cast<chrono::microseconds>(end - begin).count();
                
                string filename_time_2 = "TSP_2_zachl_dokl_Czas";//
                nlohmann::json czas0;
                czas0["execution_time"] = elapsed_time;
                 
                string filepath = "./Wyniki_Czas/C"+to_string(graph.size())+"/"+filename_time_2+".json";//
                
                ofstream file_time_2(filepath);//
                if(!file_time_2.is_open())//
                {
                    cerr<<"Unable to save execution time in JSON"<<endl;
                    return 0;
                }
                file_time_2 << czas0.dump(2);//
                file_time_2.close();//
            } 
            if(choice2==3)
            {
                chrono::steady_clock::time_point begin = chrono::steady_clock::now();
                TspSolver_zachlanny_los.hamiltonianCycles(graph,choice2);
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                 auto elapsed_time = chrono::duration_cast<chrono::microseconds>(end - begin).count();
                
                string filename_time_3 = "TSP_3_zachl_los_Czas";
                nlohmann::json czas0;
                czas0["execution_time"] = elapsed_time;
                 
                string filepath = "./Wyniki_Czas/C"+to_string(graph.size())+"/"+filename_time_3+".json";
                
                ofstream file_time_3(filepath);
                if(!file_time_3.is_open())
                {
                    cerr<<"Unable to save execution time in JSON"<<endl;
                    return 0;
                }
                file_time_3 << czas0.dump(2);
                file_time_3.close();
            }
            if(choice2==4)
            {
                chrono::steady_clock::time_point begin = chrono::steady_clock::now();
                TspSolver_zachlanny_los_dokl.hamiltonianCycles(graph,choice2);
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                auto elapsed_time = chrono::duration_cast<chrono::microseconds>(end - begin).count();
                
                string filename_time_4 = "TSP_4_zachl_los_dokl_Czas";//
                nlohmann::json czas0;
                czas0["execution_time"] = elapsed_time;
                 
                string filepath = "./Wyniki_Czas/C"+to_string(graph.size())+"/"+filename_time_4+".json";//
                
                ofstream file_time_4(filepath);//
                if(!file_time_4.is_open())//
                {
                    cerr<<"Unable to save execution time in JSON"<<endl;
                    return 0;
                }
                file_time_4 << czas0.dump(2);//
                file_time_4.close();//
            } 
            if(choice2==5)
            {
                cout<<"\n wprowadz mnoznik feromonow, mnoznik dystansu oraz mnoznik parowania z zakresu od 0 do 1"<<endl;
                cin>>o>>p>>r;
                cout<<endl;
                while(r < 0 || r > 1 || o < 0 || o > 1 || p < 0 || p > 1)
                {
                    if(o < 0 || o > 1)
                    {
                    cout<<"mnoznik feromonow musi byc z zakresu od 0 do 1"<<endl;
                    cout<<"Wprowadz mnoznik feromonow"<<endl;
                    cin>>o; 
                    }      
                    if(p < 0 || p > 1)
                    {
                    cout<<"mnoznik dystansu musi byc z zakresu od 0 do 1"<<endl;
                    cout<<"Wprowadz mnoznik dystansu"<<endl;
                    cin>>p;
                    }          
                    if(r < 0 || r > 1)
                    {
                    cout<<"mnoznik parowania musi byc z zakresu od 0 do 1"<<endl;
                    cout<<"Wprowadz mnoznik parowania"<<endl;
                    cin>>r;
                    }    
                }
                Dane dane;
                Mrowka HeurystykaTSP_Mrowkowy(o,p,r,dane);
                chrono::steady_clock::time_point begin = chrono::steady_clock::now();
                HeurystykaTSP_Mrowkowy.hamiltonianCycles(graph,o,p,r);
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                auto elapsed_time = chrono::duration_cast<chrono::microseconds>(end - begin).count();
                
                string filename_time_5 = "TSP_5_Ant_Colony_Opt_Czas";//
                nlohmann::json czas0;
                czas0["execution_time"] = elapsed_time;
                 
                string filepath = "./Wyniki_Czas/C"+to_string(graph.size())+"/"+filename_time_5+".json";//
                
                ofstream file_time_5(filepath);//
                if(!file_time_5.is_open())//
                {
                    cerr<<"Unable to save execution time in JSON"<<endl;
                    return 0;
                }
                file_time_5 << czas0.dump(2);//
                file_time_5.close();//
            }
            if(choice2 >= 6)
            {
                cout<<"Liczba powinna byc z zakresu 0-5"<<endl;
            }
    }
         
    while(choice2!=-1);
}

