#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include "lista.h"
#include "wierzcholek.h"

//HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );

using namespace std;

/**

    Lista poprzednikow/nastepnikow

    Zamiast dodawac sasiadow, dodajemy do listy zwiazanej z danym wierzcholkiem
    informacje o wierzcholkach, do ktorych krawedz "wychodzi" lub z ktorych "wchodzi" z tego danego wierzcholka

**/

// LISTA NASTĘPNIKÓW - dodawane sa wierzcholki do ktorych wychodzi krawedz z wierzcholka

class Lista
{
public:
    int **lista;                // Wskaznik na tablice dwuwymiarowa
    Krawedzz *k;                // wskaznik na poczatek tablicy krawedzi
    int *w;                     // wskaznik na poczatek tablicy wierzcholkow int
    Wierzcholek *w2;            // wskaznik na poczatek tablicy wierzcholkow Wierzcholek

    int rozmiar_k;
    int rozmiar_w;

    Lista() {}

    void stworz(Krawedzz *tab_k, int *tab_w, int rk, int rw)
    {
        // Przekazujemy tablice wierzcholkow oraz tablice krawedzi
        this->rozmiar_k = rk;
        this->rozmiar_w = rw;
        this->k = tab_k;
        this->w = tab_w;

        this->w2 = new Wierzcholek[rozmiar_w];          // Stworzenie tablicy Wierzcholkow

        // Przepisanie wartosci z tablicy intow na Wierzcholek

        for(int i=0; i<rozmiar_w; i++)
        {
            w2[i].numer = w[i];
            w2[i].rozmiar = 0;              // Przypisanie rozmiaru 0 poki co
        }

        // Stworzenie tablicy dwuwymiarowej
        // alokacja pamieci dla kazdego wierzcholka

        lista = new int*[rozmiar_w];

        // Usuniecie z pamieci tablicy intow

        //delete[] w;
    }

    void buildLista()
    {
        for(int i=0; i<rozmiar_w; i++)
        {
            // Petla dla kazdego wierzcholka
            // Sprawdzenie jakich nastepnikow ma kazdy wierzcholek

            int idx = 0;

            for(int j=0; j<rozmiar_k; j++)
            {
                if(k[j].poczatek == w2[i].numer)
                {
                   w2[i].rozmiar++;
                }
            }

            // Zbudowanie listy o takiej wielkosci, ile elementow znaleziono

            // alokacja pamieci dla wierzcholkow wychodzacych

            lista[i] = new int[w2[i].rozmiar];

            // Dodanie elementow do listy

            for(int j=0; j<rozmiar_k; j++)
            {
                if(k[j].poczatek == w2[i].numer)
                {
                    int element = k[j].koniec;
                    lista[i][idx++] = element;
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
                        dispLista();
                        break;
                    }
            }

        }while(wybor!=0);
    }

    void mstPrima()
    {
        // Trzeba rozszerzyc liste - zrobic wierzcholki nieskierowane

        for(int i=0; i<rozmiar_w; i++)
        {
            for(int j=0; j<w2[i].rozmiar; j++)
            {
                // Dodanie elementu do w2

                int wierzcholek = lista[i][j];
                int brak = 1;                       // Brak = 1 mozna dodac, brak = 0 nie mozna

                // Petla po liscie dla wierzcholka wierzcholek

                for(int c=0; c<w2[wierzcholek].rozmiar; c++)
                {
                    // Czy na liscie znajduje sie w2[i].numer?

                    if(lista[wierzcholek][c] == w2[i].numer)
                    {
                        brak = 0;

                    }
                }

                if(brak == 1)
                {
                    // Mozna dodac wierzcholek do tej listy
                    int rozmiar = w2[wierzcholek].rozmiar;
                    int *nowa_tablica = new int[rozmiar + 1];
                    //nowa_tablica->rozmiar = rozmiar;
                    int nowy = w2[i].numer;

                    for(int k=0; k<rozmiar; k++)
                    {
                        nowa_tablica[k] = lista[wierzcholek][k];          //   lista[i][j]
                    }

                    delete[] lista[wierzcholek];
                    lista[wierzcholek] = nowa_tablica;

                    lista[wierzcholek][rozmiar] = nowy;
                    w2[wierzcholek].rozmiar++;

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

            // Sprawdzenie jakie krawedzie naleza z listy

            for(int i=0; i<rozmiar_w; i++)
            {
                for(int j=0; j<w2[i].rozmiar; j++)
                {
                    // element obecny lista[i][j]

                    int mozna = 1;                  // mozna = 1 NIE MOZNA DODAC, mozna = 2 MOZNA DODAC
                    int idxKrawedzi;                // indeks krawedzi w tablicy krawedzi

                    // Sprawdzenie czy start i poczatek sa w dwoch tablicach

                    // Iteracja po liscie krawedzi
                    for(int c=0; c<rozmiar_k; c++)
                    {
                        if((w2[i].numer == k[c].poczatek && lista[i][j] == k[c].koniec) || (w2[i].numer == k[c].koniec && lista[i][j] == k[c].poczatek))
                        {
                            idxKrawedzi = c;
                            break;
                        }
                    }

                    int indeks = rozpatrzone.findElem(k[idxKrawedzi].poczatek);
                    int indeks2 = rozpatrzone.findElem(k[idxKrawedzi].koniec);

                    if(indeks != -1)    // Krawedz znajduje sie dla wierzcholka w  tabeli rozpatrzonych jako POCZATEK
                    {
                        // Srawdzenie czy k[idxKrawedzi].koniec znajduje sie w tabeli nierozpatrzonych

                        int idx = nierozpatrzone.findElem(k[idxKrawedzi].koniec);

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
                        int idx2 = nierozpatrzone.findElem(k[idxKrawedzi].poczatek);

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
                                    if((tablica_mst[c].poczatek == k[idxKrawedzi].poczatek && tablica_mst[c].koniec == k[idxKrawedzi].koniec) || (tablica_mst[c].poczatek == k[idxKrawedzi].koniec && tablica_mst[c].koniec == k[idxKrawedzi].poczatek))
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
                                        if((tablica_krawedzi[c].poczatek == k[idxKrawedzi].koniec && tablica_krawedzi[c].koniec == k[idxKrawedzi].poczatek) || (tablica_krawedzi[c].poczatek == k[idxKrawedzi].poczatek && tablica_krawedzi[c].koniec == k[idxKrawedzi].koniec))      // Jezeli istnieje to nie dodajemy
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
                                        tablica_krawedzi[rozmiar++] = k[idxKrawedzi];
                                    }
                                }
                        }
                }
            }

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

            // przesuniecie wierzcholka z nierozpatrzonych na rozpatrzony

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
        // Trzeba rozszerzyc liste - zrobic wierzcholki nieskierowane
        for(int i=0; i<rozmiar_w; i++)
        {
            for(int j=0; j<w2[i].rozmiar; j++)
            {
                // Dodanie elementu do w2

                int wierzcholek = lista[i][j];
                int brak = 1;                       // Brak = 1 mozna dodac, brak = 0 nie mozna

                // Petla po liscie dla wierzcholka wierzcholek

                for(int c=0; c<w2[wierzcholek].rozmiar; c++)
                {
                    // Czy na liscie znajduje sie w2[i].numer?

                    if(lista[wierzcholek][c] == w2[i].numer)
                    {
                        brak = 0;

                    }
                }

                if(brak == 1)
                {
                    // Mozna dodac wierzcholek do tej listy
                    int rozmiar = w2[wierzcholek].rozmiar;
                    int *nowa_tablica = new int[rozmiar + 1];
                    //nowa_tablica->rozmiar = rozmiar;
                    int nowy = w2[i].numer;

                    for(int k=0; k<rozmiar; k++)
                    {
                        nowa_tablica[k] = lista[wierzcholek][k];
                    }

                    delete[] lista[wierzcholek];
                    lista[wierzcholek] = nowa_tablica;

                    lista[wierzcholek][rozmiar] = nowy;
                    w2[wierzcholek].rozmiar++;

                }
            }
        }

        // Tablica krawedzi
        int rozmiar =0;
        Krawedzz *tablica_krawedzi = new Krawedzz[rozmiar];
        // Zbior krawedzi o najmniejszej wadze MST
        int roz = 0;
        Krawedzz *tablica_mst = new Krawedzz[roz];

        for(int i=0; i<rozmiar_w; i++)
            {
                for(int j=0; j<w2[i].rozmiar; j++)
                {
                    // Sprawdzenie, czy dana krawedz znajduje sie juz w tablicy
                    Krawedzz kra;

                    int jest = 0;

                    for(int c=0; c<rozmiar; c++)
                    {
                        if((tablica_krawedzi[c].poczatek == w2[i].numer && tablica_krawedzi[c].koniec == lista[i][j]) || (tablica_krawedzi[c].poczatek == lista[i][j] && tablica_krawedzi[c].koniec == w2[i].numer))
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

                        // Znalezienie odpowiedniej krawedzi w tablicy krawedzi

                        for(int e=0; e<rozmiar_k; e++)
                        {
                            if((k[e].poczatek == w2[i].numer && k[e].koniec == lista[i][j]) || (k[e].poczatek == lista[i][j] && k[e].koniec == w2[i].numer))
                            {
                                kra = k[e];
                            }

                        }

                        tablica_krawedzi[rozmiar++] = kra;
                    }
                }
            }

        // Posortowanie krawedzi grafu niemalejąco względem wag

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
                            //cout << "Znaleziony indeks " << indeks << endl;
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
                for(int j=0; j<w2[i].rozmiar; j++)
                {
                    // Sprawdzenie czy startem jest nasz rozpatrywany wierzcholek
                    if(w2[i].numer == wierz.numer)
                    {
                        // Znalezienie wierzcholka na liscie

                        for(int d=0; d<rozmiar_w; d++)
                        {
                            if(tablica_wierzcholkow[d].numer == lista[i][j])
                            {
                                // WARUNEK: MOZNA TO ZROBIC TYLKO WTEDY, KIEDY WIERZCHOLEK NIE JEST STARTOWYM

                                if(tablica_wierzcholkow[d].numer != tablica_wierzcholkow[0].numer)
                                {
                                    // Znalezienie przepustowowsci
                                    int waga;

                                   for(int f=0; f<rozmiar_k; f++)
                                   {
                                       if(k[f].poczatek == w2[i].numer && k[f].koniec == lista[i][j])
                                            waga = k[f].przepustowosc;
                                   }

                                    // obliczamy odleglosc
                                    // aktualna odleglosc od wierzcholka startowego
                                    int odleglosc = wierz.d + waga;
                                    // Sprawdzenie, czy jest to wartosc lepsza od starej
                                    if(odleglosc<tablica_wierzcholkow[d].d)
                                    {
                                        // Relaksacja sasiada
                                        tablica_wierzcholkow[d].d = odleglosc;
                                        tablica_wierzcholkow[d].p = w2[i].numer;

                                        // Znalezienie indeksu w tablicy nierozpatrzonych

                                        int idx;

                                        for(int e=0; e<roz; e++)
                                        {
                                            if(nierozpatrzone[e].numer == lista[i][j])
                                                idx = e;
                                        }
                                        nierozpatrzone[idx].d = odleglosc;
                                        nierozpatrzone[idx].p = w2[i].numer;

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

        // Znalezienie tej krawedzi, gdzie wierzcholkiem startowym jest 0
        Krawedzz kra;

        for(int i=0; i<rozmiar_k; i++)
        {
            if(k[i].poczatek == start)
                kra = k[i];
        }

      /*  // Znalezienie tej krawedzi, gdzie wierzcholkiem startowym jest zadany wierzcholek
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
        }*/

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

    void dispLista()
    {
       // Wyswietlenie list

        for(int i=0; i<rozmiar_w; i++)
        {
            cout << w2[i].numer << ": ";
            for(int k=0; k<w2[i].rozmiar; k++)
                cout << lista[i][k] << " ";

            cout << endl;
        }
    }

     ~Lista()
    {
        if(rozmiar_k!= 0 && rozmiar_w !=0)
        {
            // zwolnienie pamieci
            for (int i = 0; i < rozmiar_w; i++) {
                delete[] lista[i];
            }

            delete[] lista;
        }
    }

};

#endif // LISTA_H_INCLUDED
