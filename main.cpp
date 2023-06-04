#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <windows.h>
#include "macierz.h"
#include "graf.h"
#include "tablica.h"

using namespace std;

HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );

void menu1();

Graf stworzGraf(int w, float g);

int main()
{
    int wybor1;
    srand(time(NULL));

    do
    {
        //system("cls");
        cout << "W jaki sposob chcesz uzupelnic strukture grafu?\n" << endl;
        menu1();
        cin >> wybor1;
        system("cls");

         // Wczytanie danych: 1 - z pliku, 2 - wygenerowanie losowo

        switch(wybor1)
        {
            case 1:
                {
                    string nazwa; // = "graf2.txt";
                    cout << "Podaj nazwe pliku, z ktorego chcesz wczytac dane: ";
                    cin >> nazwa;
                    int blad= 0;

                    ifstream plik(nazwa);

                    if(plik.is_open())
                    {
                       string linia;
                       int krawedzie, wierzcholki, indeks=0, idx=0;

                       plik >> krawedzie;
                       plik >> wierzcholki;

                       if(wierzcholki>krawedzie)
                       {
                           cout << "Ilosc wierzcholkow w grafie jest wieksza niz krawedzi! Zmien strukture grafu" << endl << endl;
                           break;
                       }

                       Graf graf(krawedzie, wierzcholki);

                       // Uzupelnienie wierzcholkow z pierwszej linijki

                       int a, b, c;

                       plik >> a;
                       plik >> b;
                       plik >> c;

                       if(c < 0)
                       {
                           blad = 1;
                           cout << "Graf zawiera ujemne krawedzie! Zmien strukture grafu" << endl << endl;
                           break;
                       }

                       graf.completeWierzcholki(indeks++, a);
                       graf.completeWierzcholki(indeks++, b);

                       Krawedzz kra;
                       kra.poczatek =a;
                       kra.koniec = b;
                       kra.przepustowosc = c;

                       graf.completeKrawedzie(idx++, kra);

                       while(getline(plik, linia))
                        {
                            int x, y, z, rezultat;              // rezultat: 1 - jest w tablicy, 2- nie ma w tablicy

                            plik >> x;
                            plik >> y;
                            plik >> z;

                            if(z < 0)
                            {
                                blad = 1;
                                break;
                            }

                            // x - pierwsza krawedz
                            // y - druga krawedz
                            // z - przepustowowsc krawedzi

                            // DODAWANIE WIERZCHO£KÓW DO TABLICY

                            // Sprawdzenie, czy w tabeli wierzcho³ków znajduje siê ju¿ dany wierzcho³ek

                               for(int j=0; j<graf.rozmiarw; j++)
                               {
                                   if(x != graf.wierzcholki[j])
                                   {
                                       rezultat = 2;
                                       continue;
                                   }
                                   else
                                   {
                                       rezultat = 1;
                                       break;
                                   }
                               }

                               if(rezultat == 2)           // Jezeli nie ma w tablicy, to dodajemy
                                   graf.completeWierzcholki(indeks++, x);

                               for(int j=0; j<graf.rozmiarw; j++)
                               {
                                   if(y != graf.wierzcholki[j])
                                   {
                                       rezultat = 2;
                                       continue;
                                   }
                                   else
                                   {
                                       rezultat = 1;
                                       break;
                                   }
                               }

                               if(rezultat == 2)
                                   graf.completeWierzcholki(indeks++, y);

                            // DODAWANIE KRAWEDZI

                            Krawedzz k;
                            k.poczatek =x;
                            k.koniec = y;
                            k.przepustowosc = z;

                            graf.completeKrawedzie(idx++, k);

                        }

                        if(blad != 1)
                        {
                            // Posortowanie wierzcholkow
                            graf.sortWierzcholki();

                            cout << "STRUKTURA GRAFU" << endl;
                            graf.displayKrawedzie();
                            cout << endl;

                            plik.close();
                            graf.choice();
                        }
                        else
                            cout << "Graf zawiera ujemne krawedzie! Zmien strukture grafu" << endl << endl;
                    }
                    else
                        cout << "Nie udalo sie otworzyc pliku" << endl;

                    break;
                }
            case 2:
                {
                    // LOSOWO
                    int wierzcholki;
                    float gestosc;

                    cout << "Podaj liczbe wierzcholkow\n";
                    cin >> wierzcholki;
                    cout << "Podaj gestosc [%]\n";
                    cin >> gestosc;

                    // LOSOWO

                    Graf graf = stworzGraf(wierzcholki, gestosc);

                    if(graf.roz_krawedzi == 0 && graf.roz_wierzcholkow == 0)
                        break;

                    cout << "STRUKTURA GRAFU" << endl;
                    graf.displayKrawedzie();
                    cout << endl;
                    graf.choice();


                    break;
                }

            case 3:
                {
                    // LOSOWO

                    int wierzcholki=70;
                    float gestosc = 99;

                    // Seria losowych instancji - 50

                    for(int i=0; i<50; i++)
                    {
                        Graf graf = stworzGraf(wierzcholki, gestosc);
                        graf.build();

                        graf.pomiaryMacierzowo(wierzcholki);
                        graf.pomiaryListowo(wierzcholki);
                    }

                    break;
                }
        }
    }while(wybor1!=0);

    return 0;
}

void menu1()
{
    cout << "1. Wczytanie danych z pliku\n";
    cout << "2. Losowe wygenerowanie grafu\n";
    cout << "3. POMIARY\n";
    cout << "0. EXIT" << endl;
}

Graf stworzGraf(int w, float g)
{
    // LOSOWO
    int maxi, k;

    // Zalozenie - graf skierowany

    maxi = w*(w-1);

    system("cls");
    // Gestosc grafu

    // Stosunek ilsoci krawedzi do maksymalnej ilosci krawedzi

    // d = (2*k)/(w*(w-1))

    k = ((g/100)*w*(w-1))/2;

    //cout << "Ilosc krawedzi: " << k << endl;        // zaokraglanie w dol

    if(g > 100)
    {
        cout << "Podano nieprawidlowe parametry! Nie da sie zbudowac takiego grafu\n\n";
        Graf graf(0,0);
        return graf;
    }
    else if(k<w)
    {
        cout << "Podaj inne parametry! Ilosc krawedzi jest mniejsza od ilosci wierzcholkow\n\n";
        Graf graf(0,0);
        return graf;
    }
    else
    {
        Graf graf(k, w);
        int rezultat, zawiera, zawiera2, los_start, los_koniec, start, koniec, cykl;
        unsigned int przepustowosc;

        // Uzupelnienie tablicy wierzcholkow
        // Przyjmujemy ze wierzcholki numerowane sa od 0 po kolei

        for(int i=0; i<w; i++)
        {
            graf.completeWierzcholki(i, i);
        }

        // UZUPELNIANIE STRUKTURY GRAFU

        // tablica indeksow, ktore nie zostaly jeszcze wylosowane

        Tablica tab(w, hOut);           // Ka¿dy wierzcho³ek musi miec wejscie
        Tablica tab2(w, hOut);          // Ka¿dy wierzcho³ek musi miec wyjscie

        for(int i=0; i<w; i++)
        {
            tab.complete(i, graf.wierzcholki[i]);
        }

        for(int i=0; i<w; i++)
        {
            tab2.complete(i, graf.wierzcholki[i]);
        }

        // NAJPIERW UZUPELNIANIE STRUKTURY TAK, ABY KAZDY WIERZCHOLEK ZOSTAL UZYTY

        for(int i=0; i<w; i++)
        {
            do
            {
                // LOSOWANIE WIERZCHO£KA STARTOWEGO

                cykl = 0;

                do
                {
                    los_start = rand() % (w);              // losowanie indeksu
                    zawiera = tab.findElem(los_start);
                }
                while(zawiera == -1);

                start = graf.wierzcholki[los_start];

                // LOSOWANIE WIERZCHO£KA KOÑCOWEGO TAK, ABY BY£Y PO£¥CZENIA MIÊDZY WSZYSTKIMI GRAFAMI

                do
                {
                    los_koniec = rand() % (w);
                    // Sprawdzenie, czy nie zostal wylosowany ten sam indeks - nie moze tak byc

                }
                while(los_koniec == los_start);

                koniec = graf.wierzcholki[los_koniec];

                przepustowosc = rand() % 15 + 1;

                // Petla przeszukujaca tablice krawedzi grafu

                for(int j=0; j<graf.rozmiark; j++)
                {
                    if(graf.krawedzie[j].poczatek == koniec && graf.krawedzie[j].koniec == start)
                        cykl = 2;
                }

            }while(cykl==2);

            // usuwanie go z tablicy
            rezultat = tab.deleteElemAny(zawiera);

            // Dodanie do tabeli krawedzi

            Krawedzz k;
            k.poczatek =start;
            k.koniec = koniec;
            k.przepustowosc = przepustowosc;

            graf.completeKrawedzie(i, k);
        }

        // LOSOWANIE POZOSTALYCH POLACZEN - BEZ Z SIEBIE DO SIEBIE

        for(int i=w; i<k; i++)
        {
            int blad;
            do
            {
                blad=0;
                // Losowanie wierzcholkow start i poczatek na zasadzie losowania indeksu z tablicy wierzcholkow
                los_start = rand() % (w);
                los_koniec = rand() % (w);
                start = graf.wierzcholki[los_start];
                koniec = graf.wierzcholki[los_koniec];
                przepustowosc = rand() % 15 + 1;                 // Losowanie przepustowosci krawedzi z zakresu [0,15]

                for(int j=0; j<graf.rozmiark; j++)
                {
                    // Sprawdzenie, czy taka krawedz juz przypadkiem nie istnieje
                    if(((graf.krawedzie[j].poczatek == start) && (graf.krawedzie[j].koniec == koniec)) || ((graf.krawedzie[j].poczatek == koniec) && (graf.krawedzie[j].koniec == start)) || (start== koniec))
                        blad=2;

                }

            }while(blad == 2);

            // Dodanie do tabeli wylosowanego grafu
            // Dodanie do tabeli krawedzi

            Krawedzz k;
            k.poczatek =start;
            k.koniec = koniec;
            k.przepustowosc = przepustowosc;

            graf.completeKrawedzie(i, k);
        }

        return graf;

    }

}
