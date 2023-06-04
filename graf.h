#ifndef GRAF_H_INCLUDED
#define GRAF_H_INCLUDED
#include "macierz.h"
#include "krawedz.h"
#include "lista.h"
#include <iomanip>
#include <ctime>
#include <cstdlib>

using namespace std;

/**

Za³o¿enie:
Je¿eli graf jest nieskierowany to w pliku tekstowym zapisujemy dan¹ krawedz 2 razy ! Z obu stron

**/

class Graf
{

public:
    int roz_krawedzi;                  // liczba krawedzi
    int roz_wierzcholkow;              // liczba wierzcholkow
    int rozmiark=0, rozmiarw=0;
    Krawedzz *krawedzie;                 // wskaznik na poczatek tablicy krawedzi
    int *wierzcholki;                   // wskaznik na poczatek tablicy wierzcholkow
    int **tab;                          // wskaznik na tablice dwuwymiarowa
    Macierz macierz;
    Lista lista;


    // konstruktor grafu

    Graf(int k, int w)
    {
        this->roz_krawedzi = k;
        this->roz_wierzcholkow = w;

        // Stworzenie tablic dynamicznych dla krawedzi i wierzcholków
        this->krawedzie = new Krawedzz[k];
        this->wierzcholki = new int[w];

        //macierz(krawedzie, wierzcholki, rozmiark, rozmiarw);
    }

    // Uzupelnienie tablic

    void completeWierzcholki(int index, int wartosc)
    {
        wierzcholki[index] = wartosc;
        rozmiarw++;
    }

    void completeKrawedzie(int indeks, Krawedzz k)
    {
        krawedzie[indeks] = k;
        rozmiark++;
    }

    // Wyswietlenie tablicy wierzcholkow

    void displayWierzcholki()
    {
        for(int i=0; i<rozmiarw; i++)
            cout << wierzcholki[i] << " ";

        cout << endl;
    }

    void displayKrawedzie()
    {
        for(int i=0; i<rozmiark; i++)
        {
            cout << "(" << krawedzie[i].poczatek << ", " << krawedzie[i].koniec << "): " << krawedzie[i].przepustowosc << endl;
        }
    }

    // Funkcja sortujaca tablice wierzcholkow

    void sortWierzcholki()
    {
        int temp;
        for(int i=0; i<rozmiarw-1; i++)
        {
            for(int j= i+1; j<rozmiarw; j++)
            {
                if(wierzcholki[i]>wierzcholki[j])
                {
                    temp = wierzcholki[i];
                    wierzcholki[i] = wierzcholki[j];
                    wierzcholki[j] = temp;
                }
            }
        }
    }

    // Wyswietlenie wierzcholkow w kolejnosci uporzadkowanej

    void dispWierzcholki()
    {
        sortWierzcholki();
        displayWierzcholki();
    }

    void choice()
    {
        build();
        int wybor;
        do
        {
            cout << endl;
            //menu2();
            menu1();
            cin >> wybor;
            system("cls");

            switch(wybor)
            {
                case 1:
                    {
                        macierz.choice();
                        break;
                    }
                case 2:
                    {
                        lista.choice();
                        break;
                    }
            }

        }while(wybor!=0);
    }

    void menu1()
    {
        cout << "1. Reprezentacja macierzowa\n";
        cout << "2. Reprezentacja listowa\n";
        cout << "0. EXIT\n";
    }

    void build()
    {
        macierz.stworz(krawedzie, wierzcholki, rozmiark, rozmiarw);
        macierz.buildMacierz();
        lista.stworz(krawedzie, wierzcholki, rozmiark, rozmiarw);
        lista.buildLista();
    }

    void pomiaryMacierzowo(int rozmiar)
    {
        ofstream plik("pomiaryMacierz.txt", std::ios::app);
        srand(time(NULL));

        if(plik.is_open())
        {
                //plik << "Pomiary dla macierzy" << endl << endl;

                // 1. Algorytm Prima

                //pomiar czasu przed wykonaniem operacji
                LARGE_INTEGER start;
                QueryPerformanceCounter(&start);

                macierz.mstPrima();

                //pomiar czasu po wykonaniu operacji
                LARGE_INTEGER koniec;
                QueryPerformanceCounter(&koniec);

                //obliczenie czasu
                LARGE_INTEGER frequency;
                QueryPerformanceFrequency(&frequency);
                double czas = (koniec.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

                plik << czas << "   ";

                // 2. Algorytm Kruskala

                QueryPerformanceCounter(&start);

                macierz.mstKruskala();

                QueryPerformanceCounter(&koniec);

                //obliczenie czasu
                QueryPerformanceFrequency(&frequency);
                czas = (koniec.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

                plik << czas << "   ";

                // 3. Algorytm Dijkstry

                // Wylosowanie wierzcholkow do znalezienia

                int pocz, kon;

                pocz = rand() % rozmiar;
                kon = rand() % rozmiar;

                QueryPerformanceCounter(&start);

                macierz.sciezkaDijkstry(pocz, kon);

                QueryPerformanceCounter(&koniec);

                //obliczenie czasu
                QueryPerformanceFrequency(&frequency);
                czas = (koniec.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

                plik << czas << "   ";

                // 4. Algorytm Bellmana- Forda

                // Wylosowanie wierzcholkow do znalezienia

                pocz = rand() % rozmiar;
                kon = rand() % rozmiar;

                QueryPerformanceCounter(&start);

                macierz.sciezkaBellmanaForda(pocz,kon);

                QueryPerformanceCounter(&koniec);

                //obliczenie czasu
                QueryPerformanceFrequency(&frequency);
                czas = (koniec.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

                plik << czas << "   ";

                plik << endl;
        }
        else
            cout << "Nie udalo sie otworzyc pliku" << endl;

        plik.close();
    }

    void pomiaryListowo(int rozmiar)
    {
        ofstream plik("pomiaryLista.txt", std::ios::app);
        srand(time(NULL));

        if(plik.is_open())
        {
                //plik << "Pomiary dla listy" << endl << endl;

                // 1. Algorytm Prima

                //pomiar czasu przed wykonaniem operacji
                LARGE_INTEGER start;
                QueryPerformanceCounter(&start);

                lista.mstPrima();

                //pomiar czasu po wykonaniu operacji
                LARGE_INTEGER koniec;
                QueryPerformanceCounter(&koniec);

                //obliczenie czasu
                LARGE_INTEGER frequency;
                QueryPerformanceFrequency(&frequency);
                double czas = (koniec.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

                plik << czas << "   ";

                // 2. Algorytm Kruskala

                QueryPerformanceCounter(&start);

                lista.mstKruskala();

                QueryPerformanceCounter(&koniec);

                //obliczenie czasu
                QueryPerformanceFrequency(&frequency);
                czas = (koniec.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

                plik << czas << "   ";

                // 3. Algorytm Dijkstry

                // Wylosowanie wierzcholkow do znalezienia

                int pocz, kon;

                pocz = rand() % rozmiar;
                kon = rand() % rozmiar;

                QueryPerformanceCounter(&start);

                lista.sciezkaDijkstry(pocz,kon);

                QueryPerformanceCounter(&koniec);

                //obliczenie czasu
                QueryPerformanceFrequency(&frequency);
                czas = (koniec.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

                plik << czas << "   ";

                // 4. Algorytm Bellmana- Forda

                pocz = rand() % rozmiar;
                kon = rand() % rozmiar;

                QueryPerformanceCounter(&start);

                lista.sciezkaBellmanaForda(pocz,kon);

                QueryPerformanceCounter(&koniec);

                //obliczenie czasu
                QueryPerformanceFrequency(&frequency);
                czas = (koniec.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

                plik << czas << "   ";

                plik << endl;
        }
        else
            cout << "Nie udalo sie otworzyc pliku" << endl;

        plik.close();
    }
};

#endif // GRAF_H_INCLUDED
