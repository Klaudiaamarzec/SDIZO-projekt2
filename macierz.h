#ifndef MACIERZ_H_INCLUDED
#define MACIERZ_H_INCLUDED
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include "krawedz.h"
#include "tablica.h"
#include "element.h"
#include "lista2.h"
#include "wierzcholekDijkstry.h"
#include "krawedzBellmana.h"

using namespace std;

HANDLE hOut1 = GetStdHandle( STD_OUTPUT_HANDLE );


 /**

 macierz krawedzi, kazda krawedz ma swoja wage

 1  jezeli a(i) jest lukiem wychodzacym
 -1 jezeli a(i) jest lukiem wchodzacym
 0  jezeli inaczej

**/


class Macierz
{

public:

    Krawedzz *k;        // wskaznik na poczatek tablicy krawedzi
    int *w;             // wskaznik na poczatek tablicy wierzcholkow

    int rozmiar_k;
    int rozmiar_w;
    int **macierz;       // Wskaznik na tablice dwuwymiarowa (macierz)

    // Konstruktor domyslny

    Macierz(): rozmiar_k(0), rozmiar_w(0), macierz(nullptr) {}

    void stworz(Krawedzz *tab_k, int *tab_w, int rk, int rw)
    {
        // Przekazujemy tablice wierzcholkow oraz tablice krawedzi
        this->rozmiar_k = rk;
        this->rozmiar_w = rw;
        this->k = tab_k;
        this->w = tab_w;

        // Stworzenie tablicy dwuwymiarowej

        // alokacja pamieci dla wierszy
        macierz = new int*[rozmiar_w];

        // alokacja pamiêci dla kolumn
        for (int i = 0; i < rozmiar_w; i++)
        {
            macierz[i] = new int[rozmiar_k];
        }
    }

    void buildMacierz()
    {
        // STWORZENIE MACIERZY SASIEDZTWA

        // Uzupelnienie tablicy dwywymiarowej zerami

        for(int i=0; i<rozmiar_w; i++)
        {
            //tab[i] = new int[rozmiar_k];
            for(int j=0; j<rozmiar_k; j++)
            {
                macierz[i][j] = 0;
            }
        }

        // Prawidlowe  uzupelnienie

        for(int i=0; i<rozmiar_w; i++)
        {
            for(int j=0; j<rozmiar_k; j++)
            {
                for(int c=0; c<rozmiar_k; c++)
                {
                    // Sprawdzenie czy element [i][j] jest krawedzia startowa
                    if(k[c].poczatek == w[i])
                        macierz[i][c] = 1;

                    // Sprawdzenie czy element [i][j] jest krawedzia koncowa
                    if(k[c].koniec == w[i])
                        macierz[i][c] = -1;
                }
            }
        }
    }

    void menu()
    {
        cout << "MENU: Wybierz algorytm, ktory chcesz przetestowac" << endl;
        cout << "1. Wyznaczenie minimalnego drzewa rozpinajacego MST - Algorytm Prima\n";
        cout << "2. Wyznaczenie minimalnego drzewa rozpinajacego MST - Algorytm Kruskala\n";
        cout << "3. Wyznaczenie najkrotszej sciezki w grafie - Algorytm Dijkstry\n";
        cout << "4. Wyznaczenie najkrotszej sciezki w grafie - Algorytm Forda-Bellmana\n";
        cout << "5. Wyswietlenie grafu\n";
        cout << "0. EXIT" << endl;
    }

    void choice()
    {
        int wybor;
        do
        {
            cout << endl;
            menu();
            cin >> wybor;
            system("cls");

            switch(wybor)
            {
                case 1:
                    {
                        mstPrima();
                        break;
                    }
                case 2:
                    {
                        mstKruskala();
                        break;
                    }
                case 3:
                    {
                        int start, koniec;
                        do
                        {
                            cout << "Podaj wierzcholek startowy" << endl;
                            cin >> start;

                            if(start>=rozmiar_w || start<0)
                                cout << "Podano nieprawidlowy wierzcholek!" << endl << endl;

                        }
                        while(start>=rozmiar_w || start<0);

                        do
                        {
                            cout << "Podaj wierzcholek koncowy" << endl;
                            cin >> koniec;

                            if(koniec>= rozmiar_w || koniec<0)
                                cout << "Podano nieprawidlowy wierzcholek!" << endl << endl;
                            else if(start == koniec)
                                cout << "Podano ten sam wierzcholek co startowy!" << endl << endl;

                        }while(koniec>= rozmiar_w || koniec<0 || start==koniec);

                        sciezkaDijkstry(start, koniec);
                        break;
                    }
                case 4:
                    {
                        int start, koniec;
                        do
                        {
                            cout << "Podaj wierzcholek startowy" << endl;
                            cin >> start;

                            if(start>=rozmiar_w || start<0)
                                cout << "Podano nieprawidlowy wierzcholek!" << endl << endl;

                        }
                        while(start>=rozmiar_w || start<0);

                        do
                        {
                            cout << "Podaj wierzcholek koncowy" << endl;
                            cin >> koniec;

                            if(koniec>= rozmiar_w || koniec<0)
                                cout << "Podano nieprawidlowy wierzcholek!" << endl << endl;
                            else if(start == koniec)
                                cout << "Podano ten sam wierzcholek co startowy!" << endl << endl;

                        }while(koniec>= rozmiar_w || koniec<0 || start==koniec);

                        sciezkaBellmanaForda(start, koniec);
                        break;
                    }
                case 5:
                    {
                        dispMacierz();
                        break;
                    }
            }

        }while(wybor!=0);
    }

    void mstPrima()
    {
        // Zamiana -1 na 1, bo ma to byc graf nieskierowany
        for(int i=0; i<rozmiar_w; i++)
        {
            for(int j=0; j<rozmiar_k; j++)
            {
                for(int c=0; c<rozmiar_k; c++)
                {
                    // Sprawdzenie czy element [i][j] jest krawedzia startowa
                    if(k[c].poczatek == w[i])
                        macierz[i][c] = 1;

                    // Sprawdzenie czy element [i][j] jest krawedzia koncowa
                    if(k[c].koniec == w[i])
                        macierz[i][c] = 1;
                }
            }
        }

        // Dwa zbiory wierzcholkow:
        // tablica wierzcholkow rozpatrzonych i nierozpatrzonych

        int roz1=0, roz2=0;

        Tablica rozpatrzone(0, hOut1);
        Tablica nierozpatrzone(rozmiar_w, hOut1);

        for(int i=0; i<rozmiar_w; i++)
        {
            nierozpatrzone.complete(i, w[i]);
        }

        // Wybor wierzcholka, z ktorego wychodzi krawedz o najmniejszej wadze

        int minim = k[0].przepustowosc;
        int wmin = k[0].poczatek;            // minimalny wierzcholek

        for(int i=0; i<rozmiar_k; i++)
        {
            if(k[i].przepustowosc < minim)
            {
                minim = k[i].przepustowosc;
                wmin = k[i].poczatek;
            }
        }

        rozpatrzone.addLast(wmin);

        int idx = nierozpatrzone.findElem(wmin);

        nierozpatrzone.deleteElemAny(idx);

        // Zbior krawedzi o najmniejszej wadze MST

        int roz = 0;
        Krawedzz *tablica_mst = new Krawedzz[roz];

        while(rozpatrzone.rozmiar != rozmiar_w)
        {
            // Lista wszystkich krawedzi laczacych zbior wierzcholkow rozpatrzonych ze zbiorem wierzcholkow nierozpatrzonych

            int rozmiar = 0;                // 0 - krawedz nie zostala jeszcze dodana
            Krawedzz *tablica_krawedzi = new Krawedzz[rozmiar];

            // Sprawdzenie jakie krawedzie naleza z macierzy incydencji

            for(int i=0; i<rozmiar_w; i++)
            {
                for(int j=0; j<rozmiar_k; j++)
                {
                    if(macierz[i][j] == 1)
                    {

                        int mozna = 1; // mozna = 1 NIE MOZNA DODAC, mozna = 2 MOZNA DODAC

                        // Sprawdzenie czy start i poczatek sa w dwoch tablicach

                        int indeks = rozpatrzone.findElem(k[j].poczatek);
                        int indeks2 = rozpatrzone.findElem(k[j].koniec);

                        if(indeks != -1)    // Krawedz znajduje sie dla wierzcholka w  tabeli rozpatrzonych jako POCZATEK
                        {
                            // dodanie k[j].koniec

                            // Srawdzenie czy k[j].koniec znajduje sie w tabeli nierozpatrzonych

                            int idx = nierozpatrzone.findElem(k[j].koniec);

                            if(idx != -1)
                            {
                                // TAK, ZNAJDUJE SIE - MOZNA DODAC
                                mozna = 2;
                            }
                            else                    // Jezeli oba sa w tabeli rozpatrzonych, to tez OK
                            {
                                // Sprawdzenie, czy oba sa w rozpatrzonych

                                if(indeks2 != -1)
                                    mozna = 1;
                            }

                        }

                        else if(indeks2 != -1)   // Krawedz znajduje sie dla wierzcholka w  tabeli rozpatrzonych jako KONIEC
                        {
                            // dodanie k[j].poczatek

                            int idx2 = nierozpatrzone.findElem(k[j].poczatek);

                            if(idx2 != -1)
                            {
                                // Dobra krawedz
                                mozna = 2;
                            }
                        }

                        if(mozna == 2)
                        {
                            int dodana = 0;

                            // Sprawdzenie, czy dana krawedz nie zostala juz dodana do MST

                                for(int c=0; c<roz; c++)
                                {
                                    if((tablica_mst[c].poczatek == k[j].poczatek && tablica_mst[c].koniec == k[j].koniec) || (tablica_mst[c].poczatek == k[j].koniec && tablica_mst[c].koniec == k[j].poczatek))
                                    {
                                        dodana = 1;                 // Jezeli istnieje to nie dodajemy
                                        break;
                                    }
                                }

                                if(dodana == 0)
                                {
                                    // Sprawdzenie czy ta krawedz juz znajduje sie w tablicy krawedzi

                                    int warunek = rozmiar;
                                    int jest = 0;           // Jezeli jest = 1 to jest i nie dodawac !!

                                    for(int c=0; c<warunek; c++)
                                    {
                                        if((tablica_krawedzi[c].poczatek == k[j].koniec && tablica_krawedzi[c].koniec == k[j].poczatek) || (tablica_krawedzi[c].poczatek == k[j].poczatek && tablica_krawedzi[c].koniec == k[j].koniec))      // Jezeli istnieje to nie dodajemy
                                        {
                                            jest = 1;
                                            break;
                                        }
                                    }

                                    if(jest == 0)
                                    {
                                        Krawedzz *nowa_tablica = new Krawedzz[rozmiar + 1];

                                        for(int i=0; i<rozmiar; i++)
                                        {
                                            nowa_tablica[i] = tablica_krawedzi[i];
                                        }
                                        delete[] tablica_krawedzi;
                                        tablica_krawedzi = nowa_tablica;
                                        tablica_krawedzi[rozmiar++] = k[j];
                                    }
                                }
                        }
                    }
                }
            }

            // Wybor krawedzi z listy o najmniejsej wadze

            int minimum = tablica_krawedzi[0].przepustowosc;
            Krawedzz kra = tablica_krawedzi[0];

            for(int i=1; i<rozmiar; i++)
            {
                if(tablica_krawedzi[i].przepustowosc < minimum)
                {
                    minimum = tablica_krawedzi[i].przepustowosc;
                    kra = tablica_krawedzi[i];
                }
            }

            Krawedzz *nowa_tablica = new Krawedzz[roz + 1];

            for(int i=0; i<roz; i++)
            {
                nowa_tablica[i] = tablica_mst[i];
            }
            delete[] tablica_mst;
            tablica_mst = nowa_tablica;
            tablica_mst[roz++] = kra;

            // przesuniecie wierzcholka z nierozpatrzonych na rozpatrzonych

            int znaleziono = rozpatrzone.findElem(kra.poczatek);

            if(znaleziono == -1)
                rozpatrzone.addLast(kra.poczatek);

            znaleziono = rozpatrzone.findElem(kra.koniec);

            if(znaleziono == -1)
                rozpatrzone.addLast(kra.koniec);


            int rezultat = nierozpatrzone.findElem(kra.koniec);

            int wynik;

            if(rezultat != -1)
                wynik = nierozpatrzone.deleteElemAny(nierozpatrzone.findElem(kra.koniec));

            rezultat = nierozpatrzone.findElem(kra.poczatek);

            if(rezultat != -1)
                wynik = nierozpatrzone.deleteElemAny(nierozpatrzone.findElem(kra.poczatek));


        }

        cout << "Lista MST: " << endl;
        cout << "[";
        for(int i=0; i<roz; i++)
            cout << " (" << tablica_mst[i].poczatek << ", " << tablica_mst[i].koniec << ") ";;

        cout << " ]" << endl;
    }

    void mstKruskala()
    {
        // Zamiana -1 na 1, bo ma to byc graf nieskierowany
        for(int i=0; i<rozmiar_w; i++)
        {
            for(int j=0; j<rozmiar_k; j++)
            {
                for(int c=0; c<rozmiar_k; c++)
                {
                    // Sprawdzenie czy element [i][j] jest krawedzia startowa
                    if(k[c].poczatek == w[i])
                        macierz[i][c] = 1;

                    // Sprawdzenie czy element [i][j] jest krawedzia koncowa
                    if(k[c].koniec == w[i])
                        macierz[i][c] = 1;
                }
            }
        }

        // Tablica krawedzi
        int rozmiar =0;
        Krawedzz *tablica_krawedzi = new Krawedzz[rozmiar];
        // Zbior krawedzi o najmniejszej wadze MST
        int roz = 0;
        Krawedzz *tablica_mst = new Krawedzz[roz];

        // Stworzenie tablicy krawedzi na podstawie macierzy incydencji

        for(int i=0; i<rozmiar_w; i++)
        {
            for(int j=0; j<rozmiar_k; j++)
            {
                if(macierz[i][j] == 1)
                {
                    // Sprawdzenie, czy dana krawedz znajduje sie juz w tablicy

                    int jest = 0;

                    for(int c=0; c<rozmiar; c++)
                    {
                        if((tablica_krawedzi[c].poczatek == k[j].poczatek && tablica_krawedzi[c].koniec == k[j].koniec) || (tablica_krawedzi[c].poczatek == k[j].koniec && tablica_krawedzi[c].koniec == k[j].poczatek))
                        {
                            jest = 1;
                        }
                    }

                    if(jest == 0)
                    {
                        //Dodanie krawedzi do tablicy krawedzi
                        Krawedzz *nowa_tablica = new Krawedzz[rozmiar + 1];

                        for(int i=0; i<rozmiar; i++)
                        {
                            nowa_tablica[i] = tablica_krawedzi[i];
                        }
                        delete[] tablica_krawedzi;
                        tablica_krawedzi = nowa_tablica;
                        tablica_krawedzi[rozmiar++] = k[j];
                    }
                }
            }
        }

        // Posortowanie krawedzi grafu niemalejąco względem wag

        //int minimum = tablica_krawedzi[0].przepustowosc;

        for(int i=0; i<rozmiar-1; i++)
        {
            for(int j=0; j<rozmiar-i-1; j++)
            {
                if(tablica_krawedzi[j].przepustowosc > tablica_krawedzi[j+1].przepustowosc)
                {
                    // Zamiana
                    Krawedzz temp = tablica_krawedzi[j+1];
                    tablica_krawedzi[j+1] = tablica_krawedzi[j];
                    tablica_krawedzi[j] = temp;

                }
            }
        }

        // Po kolei bierzemy krawedzie z tablicy krawedzi i dodajemy je (lub nie) do zbioru MST
        // Iteracja po tablicy krawedzi

        // TABLICA ZAWIERAJACA PODGRAFY - na poczatku tyle podgrafow, ile jest wierzcholkow

        int roz2 = rozmiar_w;
        Lista2* podgrafy = new Lista2[roz2];

        // Uzupelnienie listy

        for(int i=0; i<roz2; i++)
        {
            podgrafy[w[i]] = Lista2(1, hOut1);  //w[i];
            podgrafy[i].addElements(w[i]);
        }

        int mozna;
        // MOZNA = 1 = NIE MOZNA DODAC
        // MOZNA = 2 = MOZNA DODAC

        int j=0;                        // j -indeks pobranej krawedzi
        while(roz != rozmiar_w-1)
        {
            // Pobranie krawedzi

            // SPRAWDZENIE, CZY WIERZCHOLKI NIE ZNAJDUJA SIE W TYM SAMYM PODGRAFIE

            for(int i=0; i<roz2; i++)
            {
                if(podgrafy[i].find2(tablica_krawedzi[j].poczatek) != -1)   // pierwszy element podgrafu
                {
                    // Sprawdzenie, czy tu znajduje sie koniec

                    if(podgrafy[i].find2(tablica_krawedzi[j].koniec) == -1)
                    {
                        // Sprawdzenie, gdzie znajduje sie koncowy podgraf i usuniecie go
                        int iloscElementow;

                        for(int d=0; d<roz2; d++)
                        {
                            int indeks = podgrafy[d].find2(tablica_krawedzi[j].koniec);
                            if(indeks != -1)
                            {
                                // Sprawdzenie ile elementow znajduje sie w tym podgrafie

                                iloscElementow = podgrafy[d].rozmiar;

                                // Wykonanie dodawnia i usuwania wszystkich elementow w podgrafie

                                while(iloscElementow != 0)
                                {
                                    // POLACZENIE WIERZCHOLKOW W JEDEN PODGRAF

                                    podgrafy[i].addLast(podgrafy[d].findElem(podgrafy[d].rozmiar));
                                    podgrafy[d].deleteLast();
                                    iloscElementow--;
                                }

                                break;
                            }
                        }

                        mozna = 2;
                    }
                    else
                        mozna = 1;

                    break;
                }
            }

            if(mozna == 2)
            {
                // Dodanie krawedzi do zbioru MST

                Krawedzz *nowa_tablica = new Krawedzz[roz + 1];

                for(int i=0; i<roz; i++)
                {
                    nowa_tablica[i] = tablica_mst[i];
                }
                delete[] tablica_mst;
                tablica_mst = nowa_tablica;
                tablica_mst[roz++] = tablica_krawedzi[j];
            }

            j++;
        }

        cout << "Lista MST: " << endl;
        cout << "[";
        for(int i=0; i<roz; i++)
            cout << " (" << tablica_mst[i].poczatek << ", " << tablica_mst[i].koniec << ") ";;

        cout << " ]" << endl;
    }

    void sciezkaDijkstry(int start, int koniec)
    {
        // Wyznaczenie najkrotszej sciezki w grafie - algorytm Dijkstry

        // Dla kazdego wierzcholka pamietamy dwie dane (d,p)
        // d[v] - odleglosc od wierzcholka startowego
        // p[v] - wierzcholek poprzedzajacy

        // tablica wierzcholkow Dijkstry

        WierzcholekDijkstry tablica_wierzcholkow[rozmiar_w];

        // Znalezienie wierzcholka startowego w tabeli wierzcholkow
        int indeks;

        for(int i=0; i<rozmiar_w; i++)
        {
            if(w[i] == start)
                indeks = i;
        }

        tablica_wierzcholkow[0].numer = w[indeks];
        tablica_wierzcholkow[0].d = 0;
        tablica_wierzcholkow[0].p = w[0];

        cout << endl;
        cout << "Wierzcholek startowy: " << tablica_wierzcholkow[0].numer << endl;
        cout << "(d,p)" << endl << endl;

        // Przepisanie tablicy wierzcholkow na tablice wierzcholkow Dijktry

        for(int i=0, j=1; i<indeks; i++, j++)
        {
            tablica_wierzcholkow[j].numer = w[i];
            tablica_wierzcholkow[j].d = 1000000;         // odleglosc od wierzcholka startowego
            tablica_wierzcholkow[j].p = -1;              // wierzcholek, z ktorego osiagnieto wierzcholek
        }

        for(int i=indeks+1; i<rozmiar_w; i++)
        {
            tablica_wierzcholkow[i].numer = w[i];
            tablica_wierzcholkow[i].d = 1000000;         // odleglosc od wierzcholka startowego
            tablica_wierzcholkow[i].p = -1;              // wierzcholek, z ktorego osiagnieto wierzcholek
        }

        Tablica rozpatrzone(0, hOut1);

        int roz = rozmiar_w;
        WierzcholekDijkstry *nierozpatrzone = new WierzcholekDijkstry[roz];

        for(int i=0; i<roz; i++)
        {
            nierozpatrzone[i] = tablica_wierzcholkow[i];
        }

        while(roz!=0)
        {
            // Ze zbioru nieprzebadanych wierzcholkow wybieramy ten o najmniejszej odleglosc
            int minimum = nierozpatrzone[0].d;
            WierzcholekDijkstry wierz = nierozpatrzone[0];

            for(int i=0; i<roz; i++)
            {
                if(nierozpatrzone[i].d < minimum)
                {
                    minimum = nierozpatrzone[i].d;
                    wierz = nierozpatrzone[i];
                }
            }

            // Usuniecie wierzcholka ze zbioru nieprzebadanych

            WierzcholekDijkstry *nowa_tablica = new WierzcholekDijkstry[roz-1];

            // Znalezienie indeksu wierz.numer
            int indeks;

            for(int i=0; i<roz; i++)
            {
                if(nierozpatrzone[i].numer == wierz.numer)
                    indeks = i;
            }

            for(int i=0; i<indeks; i++)
            {
                 nowa_tablica[i] = nierozpatrzone[i];
            }

            for(int i=indeks; i<roz-1; i++)
            {
                nowa_tablica[i] = nierozpatrzone[i+1];
            }

            delete[] nierozpatrzone;
            nierozpatrzone = nowa_tablica;
            roz--;

            // Sprawdzenie, gdzie wychodzi krawedz - sasiedzi

            for(int i=0; i<rozmiar_w; i++)
            {
                for(int j=0; j<rozmiar_k; j++)
                {
                    if(macierz[i][j] == 1)
                    {
                        // Sprawdzenie czy startem jest nasz rozpatrywany wierzcholek
                        if(k[j].poczatek == wierz.numer)
                        {
                            // Znalezienie wierzcholka na liscie

                            for(int d=0; d<rozmiar_w; d++)
                            {
                                if(tablica_wierzcholkow[d].numer == k[j].koniec)
                                {
                                    // WARUNEK: MOZNA TO ZROBIC TYLKO WTEDY, KIEDY WIERZCHOLEK NIE JEST STARTOWYM

                                    if(tablica_wierzcholkow[d].numer != tablica_wierzcholkow[0].numer)
                                    {
                                        int odleglosc = wierz.d + k[j].przepustowosc;
                                        if(odleglosc<tablica_wierzcholkow[d].d)
                                        {
                                            // Relaksacja sasiada
                                            tablica_wierzcholkow[d].d = odleglosc;
                                            tablica_wierzcholkow[d].p = k[j].poczatek;

                                            // Znalezienie indeksu w tablicy nierozpatrzonych

                                            int idx;

                                            for(int i=0; i<roz; i++)
                                            {
                                                if(nierozpatrzone[i].numer == k[j].koniec)
                                                    idx = i;
                                            }
                                            nierozpatrzone[idx].d = odleglosc;
                                            nierozpatrzone[idx].p = k[j].poczatek;

                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            }

            rozpatrzone.addLast(wierz.numer);

        }

        /*cout << "Tablica Dijkstry" << endl;

        for(int i=0; i<rozmiar_w; i++)
        {
            cout << tablica_wierzcholkow[i].numer << ": (" << tablica_wierzcholkow[i].d << ", " << tablica_wierzcholkow[i].p << ")" << endl;
        }*/

        //Wyswietlenie szukanej sciezki wraz z jej waga

        cout << endl <<  "SCIEZKA" << endl;

        for(int i=0; i<rozmiar_w; i++)
        {
            // Szukanie w tablicy Dijkstry wierzcholka koncowego
            if(tablica_wierzcholkow[i].numer == koniec)
            {
                if(tablica_wierzcholkow[i].d == 1000000)
                {
                    cout << "Brak sciezki do tego wierzcholka koncowego!" << endl;
                    break;
                }

                cout << "Koszt sciezki: " << tablica_wierzcholkow[i].d << endl;
                cout << tablica_wierzcholkow[i]. numer;

                do
                {
                    cout << " <- " << tablica_wierzcholkow[i].p;

                    // Znalezienie indeksu rodzica

                    for(int j=0; j<rozmiar_w; j++)
                    {
                        if(tablica_wierzcholkow[j].numer == tablica_wierzcholkow[i].p)
                        {
                            i=j;
                            break;
                        }
                    }

                }while(i != 0);

                break;
            }
        }
        cout << endl;

    }

    bool sciezkaBellmanaForda(int start, int koniec)
    {
        bool result;
        // Wyznaczenie najkrotszej sciezki w grafie - algorytm Bellmana-Forda

        // Dla kazdego wierzcholka pamietamy dwie dane (d,p)
        // d[v] - odleglosc od wierzcholka startowego
        // p[v] - wierzcholek poprzedzajacy

        // Znalezienie tej krawedzi, gdzie wierzcholkiem startowym jest zadany wierzcholek
        Krawedzz kra;

        for(int i=0; i<rozmiar_w; i++)
        {
            for(int j=0; j<rozmiar_k; j++)
            {
                if(macierz[i][j] == 1 && i == start)
                {
                    // Sprawdzenie czy znaleziony wierzcholek poczatkowy to startowy
                    kra = k[j];
                    break;
                }
            }
        }

        Krawedzz tablica_krawedzi[rozmiar_k];

        for(int i=0; i<rozmiar_k; i++)
        {
            tablica_krawedzi[i] = k[i];
        }

        // Wierzcholek startowy - przymujemy jako pierwszy wierzcholek w tabeli wierzcholkow
        // Wierzcholek startowy przyjmujemy jako ten do ktorego nie wchodzi zadna krawedz


        cout << "Wierzcholek startowy: " << kra.poczatek << endl;
        cout << "(d,p)" << endl << endl;

         // tablica wierzcholkow

        WierzcholekDijkstry tablica_wierzcholkow[rozmiar_w];

        // Znalezienie wierzcholka startowego w tabeli wierzcholkow
        int indeks;

        for(int i=0; i<rozmiar_w; i++)
        {
            if(w[i] == start)
                indeks = i;
        }

        tablica_wierzcholkow[0].numer = w[indeks];
        tablica_wierzcholkow[0].d = 0;
        tablica_wierzcholkow[0].p = w[0];

        // Przepisanie tablicy wierzcholkow na tablice wierzcholkow Dijktry

        for(int i=0, j=1; i<indeks; i++, j++)
        {
            tablica_wierzcholkow[j].numer = w[i];
            tablica_wierzcholkow[j].d = 1000000;         // odleglosc od wierzcholka startowego
            tablica_wierzcholkow[j].p = -1;              // wierzcholek, z ktorego osiagnieto wierzcholek
        }

        for(int i=indeks+1; i<rozmiar_w; i++)
        {
            tablica_wierzcholkow[i].numer = w[i];
            tablica_wierzcholkow[i].d = 1000000;         // odleglosc od wierzcholka startowego
            tablica_wierzcholkow[i].p = -1;              // wierzcholek, z ktorego osiagnieto wierzcholek
        }

        for(int k=1; k<rozmiar_w; k++)                      // petla relaksacji
        {
            result = true;
            for(int i=0; i<rozmiar_w; i++)                  // petla po wierzcholkach
            {
                WierzcholekDijkstry wierz = tablica_wierzcholkow[i];

                for(int j=0; j<rozmiar_k; j++)              // petla po krawedziach
                {
                    // Sprawdzenie, czy natrafiono na krawedz, dla ktorej poczatek lub koniec to wierzcholek
                    if(wierz.numer == tablica_krawedzi[j].koniec)
                    {
                        int koszt = tablica_wierzcholkow[i].d;          // aktualny koszt dojscia do rozpatrywanego wierzcholka
                        int odleglosc = tablica_wierzcholkow[tablica_krawedzi[j].poczatek].d;
                        int nowa_odleglosc = odleglosc + tablica_krawedzi[j].przepustowosc;

                        // Sprawdzenie, czy nowa odleglosc jest lepsza od aktualnej

                        if(nowa_odleglosc < koszt)
                        {
                            // Przypisanie nowych danych w tablicy wierzcholkow
                            result = false;
                            tablica_wierzcholkow[i].d = nowa_odleglosc;
                            tablica_wierzcholkow[i].p = tablica_wierzcholkow[tablica_krawedzi[j].poczatek].numer;
                        }
                    }
                }
            }
        }

        // Sprawdzenie istnienia ujemnego cyklu

        for(int k=0; k<rozmiar_w; k++)
        {
            for(int i=0; i<rozmiar_w; i++)
            {
                WierzcholekDijkstry wierz = tablica_wierzcholkow[i];

                for(int j=0; j<rozmiar_k; j++)
                {
                    // Sprawdzenie, czy natrafiono na krawedz, dla ktorej poczatek lub koniec to wierzcholek
                    if(wierz.numer == tablica_krawedzi[j].koniec)
                    {
                        int koszt = tablica_wierzcholkow[i].d;          // aktualny koszt dojscia do rozpatrywanego wierzcholka
                        int odleglosc = tablica_wierzcholkow[tablica_krawedzi[j].poczatek].d;
                        int nowa_odleglosc = odleglosc + tablica_krawedzi[j].przepustowosc;

                        // Sprawdzenie, czy nowa odleglosc dalej jest lepsza od aktualnej

                        if(nowa_odleglosc < koszt)
                            return false;
                    }
                }
            }
        }

        /*cout << "Tablica Bellmana- Forda" << endl;
        for(int i=0; i<rozmiar_w; i++)
        {
            cout << tablica_wierzcholkow[i].numer << ": (" << tablica_wierzcholkow[i].d << ", " << tablica_wierzcholkow[i].p << ")" << endl;
        }*/

        cout << endl <<  "SCIEZKA" << endl;

        for(int i=0; i<rozmiar_w; i++)
        {
            // Szukanie w tablicy Dijkstry wierzcholka koncowego
            if(tablica_wierzcholkow[i].numer == koniec)
            {
                if(tablica_wierzcholkow[i].d == 1000000)
                {
                    cout << "Brak sciezki do tego wierzcholka koncowego!" << endl;
                    break;
                }

                cout << "Koszt sciezki: " << tablica_wierzcholkow[i].d << endl;
                cout << tablica_wierzcholkow[i]. numer;

                do
                {
                    cout << " <- " << tablica_wierzcholkow[i].p;

                    // Znalezienie indeksu rodzica

                    for(int j=0; j<rozmiar_w; j++)
                    {
                        if(tablica_wierzcholkow[j].numer == tablica_wierzcholkow[i].p)
                        {
                            i=j;
                            break;
                        }
                    }

                }while(i != 0);

                break;
            }
        }
        cout << endl;

        return true;
    }

    // REPREZENTACJA MACIERZOWA - wyswietlenie grafu

    void dispMacierz()
    {

        cout << "   |";

        for(int i=0; i<rozmiar_k; i++)
            cout << " (" << setw(1) << k[i].poczatek << "," << setw(1) << k[i].koniec << ") " << "|";
            // cout << "  " << setw(2) << k[i].przepustowosc << " |";

        cout << endl;

        cout << "----";

        for(int i=0; i<rozmiar_k*8; i++)
            cout << "-";

        cout << endl;

        for(int i=0; i<rozmiar_w; i++)
        {
            cout << setw(2) << w[i] << " |";
            for(int j=0; j<rozmiar_k; j++)
            {
                cout << setw(4) << macierz[i][j] << "   |";
            }
            cout << endl;
        }
    }

    ~Macierz()
    {
        // zwolnienie pamiêci dla kolumn
        for (int i = 0; i < rozmiar_w; i++) {
            delete[] macierz[i];
        }

        // zwolnienie pamiêci dla wierszy
        delete[] macierz;
    }

};

#endif // MACIERZ_H_INCLUDED
