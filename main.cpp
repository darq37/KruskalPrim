#include <iostream>
#include <fstream>
#include <chrono>
#include <climits>
#include <random>
#include <algorithm>
#include <sstream>

using namespace std;

int znajdzNajblizszyWierzcholek(int *odleglosc, bool *odwiedzone, int iloscWierzcholkow);

void wypiszWYnikPrima(int **graph, int *parent, int n);

int *Prim(int **graph, int iloscWierzcholkow);

int *Kruskal(int **graph, int n, int k);

void uniteTrees(int z, int b, int *parents);

int pickTree(int n, int *parents);

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
            n = stoi(line); //string to int(line) -> n
            /*bpobiera pierwszą linijke i zapisuje ją jako liczbe wierzcholków*/


            int **macierzsasiedztwa = stworzMacierzWypelnionaZerami(n); // tworze macierz z zerami i przypisuje ją do wskaźnika na macierz

            while (getline(plik, line)) { // póki mam jeszcze linie w pliku
                cout << line << '\n'; // wypisuje wszystkie stringi na ekran
                istringstream s(line); // klasa input stream do operacji na stringach, tworze obiekt s
                string v1, v2, waga; // deklaruje 3 stringi
                getline(s, v1, ';');
                getline(s, v2, ';');
                getline(s, waga); // przypisuje do każdej zmiennej wartosc stringa z lini pliku ( wartosc przypisana do obiektu s), wartosci oddzielone średnikami
                /*składnia: getline (istream&  is, string& str, char delimeter);*/
                wstawSymetrycznieDoMacierzy(macierzsasiedztwa, stoi(v1), stoi(v2), stoi(waga));
                /*uzywam funkcji z poprzedniego algortmu, która wstawia mi odpowiednie wartosci (v1 i v2 oraz wage) go macierzy w symetryczny sposób
                 * jednoczesnie uzywam funkcji string to int, parsując stringi do wartosci typu int*/
            }
            wypiszMacierz(n, macierzsasiedztwa); // wypisuje na ekran macierz
            int m = 0; // zmienna pomocnicza w wypisaniu krawędzi
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {

                    if (macierzsasiedztwa[i][j] != 0) m++;/* iterujac przez macierz, jezeli nie ma 0 w środku tablicy, zwiększam m*/
                }
            }
            k = m / 2; // poniewaz macierz jest symetryczna, wszystkie niezerowe miejsca w tablicy to krawędzie, a liczba krawędzi jest dwukrotnie mniejsza
            // krawędzie potrzbne do Kruskala
            cout << "Ilość krawędzi: " << k;
            cout << "\nWynik wywolania algorytmu Prima :" <<endl;
            Prim(macierzsasiedztwa, n); // uzywam algorytmu prima
            wypiszWYnikPrima(macierzsasiedztwa, Prim(macierzsasiedztwa, n), n); // wypisuje wynik Prima
            cout << "Wynik wywolania algorytmu Kruskala :" <<endl;
            Kruskal(macierzsasiedztwa, n, k); // uzywam algorytmu Kruskala, który wypisuje wynik w swojej implementacji
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
            cout << "Zapełnienie grafu wynosi " << zapelnienie << "%." << endl; /* kopia z poprzedniego algorytmu*/
            for (int i = zakresMinimum; i < zakresMaksimum; ++i) {
                int maksymalnaIloscKrawedzi = i * (i - 1) / 2;
                k = static_cast<int>(maksymalnaIloscKrawedzi * zapelnienie / 100);
                cout << '\n' << "Graf [" << i << "] ma teraz " << k << " krawędzi" << endl;
                double czas = 0;
                double czas2 = 0;/* dwie zmienne do liczenia czasu prima i kruskala*/
                for (int j = 0; j < liczbaWykonan; j++) {
                    int **macierzSasiedztwa = wygenerujSpojnyGraf(i, k);
                    /*wypiszMacierz(i, macierzSasiedztwa);*/
                    auto started = chrono::steady_clock::now();
                    Prim(macierzSasiedztwa, i);
                    auto done = chrono::steady_clock::now();
                    czas += chrono::duration<double, nano>(done - started).count();
                    auto started2 = chrono::steady_clock::now();
                    Kruskal(macierzSasiedztwa, i, k); // wypisywanie danych nie powinno miec tutaj miejsca, trzeba  by to rozdzielic, jak w przypadku Prima
                    auto done2 = chrono::steady_clock::now();
                    czas2 += chrono::duration<double, nano>(done2 - started2).count();
                }
                cout << '\n' << "Sredni czas dla Prima: " << czas / liczbaWykonan << " nano sekund" << '\n';
                cout << '\n' << "Sredni czas dla Kruskala: " << czas2 / liczbaWykonan << " nano sekund" << '\n';


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

void wypiszWYnikPrima(int **graph, int *parent, int n) {
    cout << '\n' << "Rodzic: \t" << "Cel: \t" << "Waga: " << endl;
    for (int i = 1; i < n; i++) {
        printf("%d\t\t-> \t%d \t\t%d \n", parent[i], i, graph[i][parent[i]]); // wypisuje elementy tablicy rodziców, indeks danego elementu (cel) oraz
        /*wstawiam element rodzica jako indeks do macierzy sasiedztwa, odczytujac przy tym jego wagę*/
    }
}

int *Prim(int **graph, int iloscWierzcholkow) {
    //
    int *parent = new int[iloscWierzcholkow];
    int *odleglosc = new int[iloscWierzcholkow];/*tworze wskaźniki na tablice intów z odleglsociami rodzicami*/
    bool *odwiedzone = new bool[iloscWierzcholkow];/* oraz wskaźnik na tablice booli, sprawdzajacą czy wierzcholek byl juz odwieczony*/
    for (int i = 0; i < iloscWierzcholkow; i++) {
        odleglosc[i] = INT_MAX;
        odwiedzone[i] = false;/*ustawiam wszystkie elementy tablic odleglosci na INT_MAX i tablicy potwierdzającej na false*/
    }
    odleglosc[0] = 0;
    parent[0] = -1;
    for (int count = 0; count < iloscWierzcholkow - 1; count++) { //idać przez wszystkie wierzchołki (w pierwszym już byłem)
        int najblizszyWierzcholek = znajdzNajblizszyWierzcholek(odleglosc, odwiedzone, iloscWierzcholkow); /*przyjmuje index
 * jeszcze nie odwiedzonego wierzcholka o najmniejszej wadze*/
        odwiedzone[najblizszyWierzcholek] = true; /*Zapisuje index wierzcholka o najmniejszej wadze jako odwiedzony*/
        for (int j = 0; j < iloscWierzcholkow; j++) { /*Idąc przez wszystkie wierzchołki*/
            if (graph[najblizszyWierzcholek][j]) { /*będąc w wierzcholku oznacznym jako najblizszy*/
                if (!odwiedzone[j]) { /*i nie był on odwiedzony*/
                    if (graph[najblizszyWierzcholek][j] < odleglosc[j]) { /*Jeżeli waga tego wierzcholka jest nizsza od odleglosci do nastepnego wierzcholka */
                        parent[j] = najblizszyWierzcholek; /*zapisuje jego index jako rodzica*/
                        odleglosc[j] = graph[najblizszyWierzcholek][j]; /*nadpisuje odleglosc*/
                    }
                }
            }
        }
    }
    return parent;/*zwracam rodzica w celu wypisania wyniku wywolania algorytmu na ekran*/
}

int znajdzNajblizszyWierzcholek(int *odleglosc, bool *odwiedzone, int iloscWierzcholkow) {
    int min = INT_MAX; // ustawiam zmienna pomocnicza min na int amx
    int min_index; // deklaruje zmienna pomocnicza oznaczającą index wierzcholka u aktualnie najmniejszej wadze
    for (int i = 0; i < iloscWierzcholkow; i++) // idac przez wszystkie wierzcholki
        if (odwiedzone[i] == false && odleglosc[i] < min) //jeżeli wierzchołek nie był jeszcze odwiedzony, aktualizuje tymczasowa
            // odległość, dążąc do najmniejszej
            min = odleglosc[i], min_index = i; /*w pierwszym kroku ustawiam min na 0, oraz index na 0
 * w nastepnych iteracjach, szukam najmniejszej odleglosci*/
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

int *Kruskal(int **graph, int n, int k) {
    int *parent = new int[n];/*tworze wskaznik na tablice zawierającą wierzcholki z ktorych wyszeldem*/
    int iloscOdwiedzonychKrawedzi = 0;
    int odleglosci = 0;
    for (int i = 0; i < n; i++) {
        parent[i] = i; // ustawiam indeks kazdego wierzcholka do tablicy rodzicow
    }
    cout << '\n' << "Rodzic: \t" << "Cel: \t" << "Waga: " << endl;
    while (iloscOdwiedzonychKrawedzi < n - 1) {/*dopoki nie skoncza mi sie krawędzie*/
        int minimum = INT_MAX;/*ustawiam minimum na nieskoncz*/
        int a1 = -1;
        int a2 = -1;/*ustawiam dwie zmienne pomocnicze na -1, bye nie kolidowac z wierzcholkami*/
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {/*iteruje po macierzy sasiedztwa*/
                if (graph[i][j] != 0 && pickTree(i, parent) != pickTree(j, parent) && graph[i][j] < minimum) {/*jezeli dla 2 wierzcholkow istnieje krawedz ORAZ
 * jezeli ta krawedz nie łaczy wierzcholkow, z ktorych wychodzi ta krawędz, ORAZ jezeli waga krawedzi jest mniejsza od minimum*/
                    minimum = graph[i][j];/*ustawiam wage tej krawędzi na minium, a wiec dodaje ją do MST*/
                    a1 = i;/*oraz ustalam zmienne pomocnicze na odpowiednie wiercholki, tworzące krawędź*/
                    a2 = j;
                }
            }
        }
        uniteTrees(a1, a2, parent);/*lacze powstale ewentualne drzewa, na podstawie rodzica krawędzi */
        printf("%d\t\t-> \t%d \t\t%d \n", a1, a2, minimum); //wypisuje wynik, to powinno isc od innej funkcji
        odleglosci += minimum;/*nadpisuje odleglosci*/
        iloscOdwiedzonychKrawedzi++;/*zwiekszam ilosc odwiedzonych krawędzi*/
    }
    return parent;
}

void uniteTrees(int z, int b, int *parents) {/*funkcja do laczenia powstaluch mniejszych drzew spinajacych*/
    int a1 = pickTree(z, parents);/* ustawiam zmienna a1 jako  jedno drzewo na podstawie pierwszej krawędzie*/
    int a2 = pickTree(b, parents);/*ustawiam drugie drzewo*/
    parents[a1] = a2;/* lacze oba drzewa, na podstawie rodzica*/
}

int pickTree(int n, int *parents) {/*funkcja do wybierania drzewa, przyjmuje krawedz oraz tablice rodzicow*/
    while (parents[n] != n)/*póki wartosc elementu z tablicy rodzicow d nie jest rowna indexowi tego elementu*/
        n = parents[n];/*ustawiam wartosc n-tego elementu tablicy pod zienna n*/
    return n;/*oraz zwracam ta zmienna*/
}


