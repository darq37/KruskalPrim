#include <iostream>
#include <fstream>
#include <chrono>
#include <climits>
#include <random>
#include <algorithm>
#include <sstream>

using namespace std;

int znajdzNajblizszyWierzcholek(int *odleglosc, bool *odwiedzone, int iloscWierzcholkow);

void wypiszWynikPrima(int **graph, int *parent, int n);

int *Prim(int **graph, int iloscWierzcholkow);

int **wygenerujSpojnyGraf(int n, int k);

int **stworzMacierzWypelnionaZerami(int n);

int *wygenerujLosowaPermutacjeIndeksowDoN(int n);

void wstawSymetrycznieDoMacierzy(int **macierz, int i, int j, int wartosc);

int losowaWartoscWZakresie(int min, int max);

vector<pair<int, int>> znajdzMiejscaNaKrawedzie(int n, int *const *macierzSasiedztwa);

void wypiszMacierz(int n, int **macierzSasiedztwa);


int main() {
    int n; // wierzcholki
    int k; // krawedzie
    char tryb;
    cout << "wybierz tryb działania programu:" << '\n';
    cout << "Tryb A - wybierz a" << endl;
    cout << "Tryb B - wybierz b" << endl;
    cin >> tryb;
    switch (tryb) {
        case 'A':
        case 'a': {
            string line;
            ifstream plik;
            cout << "Czytam dane z pliku: " << endl;
            plik.open(R"(C:\Users\wesiv\Desktop\KRUSKAL\dane.txt)");
            getline(plik, line);
            n = stoi(line);


            int **macierzsasiedztwa = stworzMacierzWypelnionaZerami(n);

            while (getline(plik, line)) {
                cout << line << '\n';
                istringstream s(line);
                string v1, v2, waga;
                getline(s, v1, ';');
                getline(s, v2, ';');
                getline(s, waga);
                wstawSymetrycznieDoMacierzy(macierzsasiedztwa, stoi(v1), stoi(v2), stoi(waga));
            }
            wypiszMacierz(n, macierzsasiedztwa);
            int m = 0;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {

                    if (macierzsasiedztwa[i][j] != 0) m++;
                }
            }
            k = m / 2;
            cout << "Ilość krawędzi: " << k;
            Prim(macierzsasiedztwa, n);
            wypiszWynikPrima(macierzsasiedztwa, Prim(macierzsasiedztwa, n), n);
            plik.close();

            break;
        }
        case 'B':
        case 'b': {
            cout << "Wybrano tryb B" << endl;
            cout << "Podaj dolną wartość zakresu wierzcholkow: " << endl;
            int zakresMinimum;
            cin >> zakresMinimum;
            cout << "Podaj górną wartość zakresu wierzcholkow: " << endl;
            int zakresMaksimum;
            cin >> zakresMaksimum;
            cout << "Podaj liczbę wykonań algorytmu (liczbę losowych grafów/ilość wierzchołków na liczbę z zakresu)" << endl;
            int liczbaWykonan;
            cin >> liczbaWykonan;
            double zapelnienie;
            cout << "Podaj zapelnenie grafu" << endl;
            cin >> zapelnienie;
            cout << "Zapełnienie grafu wynosi " << zapelnienie << "%." << endl;
            for (int i = zakresMinimum; i < zakresMaksimum; ++i) {
                int maksymalnaIloscKrawedzi = i * (i - 1) / 2;
                k = static_cast<int>(maksymalnaIloscKrawedzi * zapelnienie / 100);
                cout << '\n' << "Graf [" << i << "] ma teraz " << k << " krawędzi" << endl;
                double czas = 0;
                for (int j = 0; j < liczbaWykonan; j++) {
                    int **macierzSasiedztwa = wygenerujSpojnyGraf(i, k);
                    /*wypiszMacierz(i, macierzSasiedztwa);*/
                    auto started = chrono::steady_clock::now();
                    Prim(macierzSasiedztwa, i);
                    auto done = chrono::steady_clock::now();
                    czas += chrono::duration<double, nano>(done - started).count();
                }
                cout << '\n' << "Sredni czas: " << czas / liczbaWykonan << " nano sekund" << '\n';
            }
            break;
        }
        default: {
            cout << "Nie wybrano odpowiedniego trybu!" << endl;
            break;
        }
    }
    return 0;
}

void wypiszWynikPrima(int **graph, int *parent, int n) {
    cout << '\n' << "Rodzic: \t" << "Cel: \t" << "Waga: " << endl;
    for (int i = 1; i < n; i++) {
        printf("%d\t\t-> \t%d \t\t%d \n", parent[i], i, graph[i][parent[i]]);
    }
}

int *Prim(int **graph, int iloscWierzcholkow) {
    //
    int *parent = new int[iloscWierzcholkow];
    int *odleglosc = new int[iloscWierzcholkow];
    bool *odwiedzone = new bool[iloscWierzcholkow];
    for (int i = 0; i < iloscWierzcholkow; i++) {
        odleglosc[i] = INT_MAX;
        odwiedzone[i] = false;
    }
    odleglosc[0] = 0;
    parent[0] = -1;
    for (int count = 0; count < iloscWierzcholkow - 1; count++) { //idać przez wszystkie wierzchołki (w pierwszym już byłem)
        int najblizszyWiercholek = znajdzNajblizszyWierzcholek(odleglosc, odwiedzone, iloscWierzcholkow); /*przyjmuje index
 * jeszcze nie odwiedzonego wierzcholka o najmniejszej wadze*/
        odwiedzone[najblizszyWiercholek] = true; /*Zapisuje wierzcholek o tym indexie jako odwiedzony*/
        for (int j = 0; j < iloscWierzcholkow; j++) { /*Idąc przez wszystkie wierzchołki*/
            if (graph[najblizszyWiercholek][j]) { /*Jeżeli jestem w najblizszym wierzchołku*/
                if (!odwiedzone[j]) { /*i nie był on odwiedzony*/
                    if (graph[najblizszyWiercholek][j] < odleglosc[j]) { /*Jeżeli wierzcholek jest najblizszy*/
                        parent[j] = najblizszyWiercholek; /*zapisuje jego index jako rodzica*/
                        odleglosc[j] = graph[najblizszyWiercholek][j]; /*nadpisuje odleglosc*/
                    }
                }
            }
        }
    }
    return parent;
}

int znajdzNajblizszyWierzcholek(int *odleglosc, bool *odwiedzone, int iloscWierzcholkow) {
    int min = INT_MAX;
    int min_index;
    for (int i = 0; i < iloscWierzcholkow; i++)
        if (odwiedzone[i] == false && odleglosc[i] < min) //jeżeli wierzchołek nie był odwiedzony, aktualizuje tymczasowa
            // odległość, dążąć do najmniejszej
            min = odleglosc[i], min_index = i;
    return min_index; //zwraca index nie odwiedzonego jeszcze wierzcholka o najmniejszej odległości
}


int **wygenerujSpojnyGraf(int n, int k) {
    int **macierzSasiedztwa = stworzMacierzWypelnionaZerami(n);
    int *permutacjaIndeksow = wygenerujLosowaPermutacjeIndeksowDoN(n);
    for (int i = 0; i < n - 1; i++) {
        wstawSymetrycznieDoMacierzy(macierzSasiedztwa, permutacjaIndeksow[i], permutacjaIndeksow[i + 1], losowaWartoscWZakresie(1, 100));
    }
    int iloscBrakujacychKrawedzi = k - (n - 1);
    vector<pair<int, int>> miejscaNaKrawedzie = znajdzMiejscaNaKrawedzie(n, macierzSasiedztwa);
    shuffle(miejscaNaKrawedzie.begin(), miejscaNaKrawedzie.end(), mt19937(random_device()()));
    for (int i = 0; i < iloscBrakujacychKrawedzi; i++) {
        wstawSymetrycznieDoMacierzy(macierzSasiedztwa, miejscaNaKrawedzie[i].first, miejscaNaKrawedzie[i].second, losowaWartoscWZakresie(1, 100));
    }
    return macierzSasiedztwa;
}

int **stworzMacierzWypelnionaZerami(int n) {
    int **macierzSasiedztwa;
    macierzSasiedztwa = new int *[n];
    for (int i = 0; i < n; i++) {
        macierzSasiedztwa[i] = new int[n];
        for (int j = 0; j < n; j++) {
            macierzSasiedztwa[i][j] = 0;
        }
    }
    return macierzSasiedztwa;
}

int *wygenerujLosowaPermutacjeIndeksowDoN(int n) {
    int *possibleEdges = new int[n];
    for (int i = 0; i < n; i++) {
        possibleEdges[i] = i;
    }
    shuffle(possibleEdges, possibleEdges + n, mt19937(random_device()()));
    return possibleEdges;
}

void wypiszMacierz(int n, int **macierzSasiedztwa) {
    for (int i = 0; i < n; i++) {
        cout << "\n";
        for (int j = 0; j < n; j++) {
            cout << macierzSasiedztwa[i][j] << "\t";
        }
    }
    cout << '\n';
}

void wstawSymetrycznieDoMacierzy(int **macierz, int i, int j, int wartosc) {

    macierz[i][j] = wartosc;
    macierz[j][i] = wartosc;
}

int losowaWartoscWZakresie(int min, int max) {
    random_device rd; // obtain a random number from hardware
    mt19937 rng(rd()); // seed the generator
    uniform_int_distribution<> distr(min, max); // define the range
    return distr(rng);
}

vector<pair<int, int>> znajdzMiejscaNaKrawedzie(int n, int *const *macierzSasiedztwa) {
    vector<pair<int, int>> puste;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (macierzSasiedztwa[i][j] == 0) {
                puste.emplace_back(i, j);
            }
        }
    }
    return puste;
}

int *Kruskal(int **graph,int n,  int k){
    int *parents = new int [n];
    int iloscOdiwedzonychKrawedzi = 0;
    int odleglosci = 0;
    for (int i = 0; i < n; i++) {
        parents[i] = i; // każdy wierzchołek jest teraz rodzicem
    }
    while(iloscOdiwedzonychKrawedzi< n-1){


        iloscOdiwedzonychKrawedzi++;
    }



    return parent;
}

