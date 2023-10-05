#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <set>

using namespace std;

int klikaczypodobna;

void WczytajInstancje(string nazwapliku1, string nazwapliku2, vector<string> vect1[5], vector<string> vect2[5])
{
    ifstream plik1(nazwapliku1 + ".fasta");
    string line;
    int i = 0;  //licznik dla vect1
    int liczniklinii = 1;  // nowy licznik linii

    while (getline(plik1, line))  //czyta znak po znaku i zapisuje kazdy jako osobny string w vect1 (nukleotydy)
    {
        if (liczniklinii % 2 == 0)  // sprawdza czy liczniklinii jest parzysty
        {
            for (char c : line)
            {
                vect1[i].push_back(string(1, c));
            }
            i++;  //kolejna linia -> kolejny wektor
        }
        liczniklinii++;  // inkrementuje liczniklinii
    }

    ifstream plik2(nazwapliku2 + ".qual");
    i = 0;  //resetuje licznik dla vect2
    liczniklinii = 1;  // resetuje licznik linii

    while (getline(plik2, line))   //czyta linia po linii
    {
        if (liczniklinii % 2 == 0)  // sprawdza czy liczniklinii jest parzysty
        {
            stringstream ss(line); //zamienia linie na slowa
            string word;
            while (ss >> word)
            {
                vect2[i].push_back(word); //zapisuje kazde slowo w vect2 (wiarygodnosci)
            }
            i++; //kolejna linia -> kolejny wektor
        }
        liczniklinii++;  // inkrementuje liczniklinii
    }
    plik1.close();
    plik2.close();
}

void PokazInstancje(vector<string> vect1[5], vector<string> vect2[5])
{
    for (int i = 0; i < 5; i++)
    {
    cout << "SEKWENCJA " << i + 1 << ":" << endl;
        for (string s : vect1[i])
        {
            cout << s << " ";
        }
        cout << endl;
        cout << endl;
        for (string s : vect2[i])
        {
            cout << s << " ";
        }
        cout << endl;
        cout << endl;
    }
}

void UsunPonizejProgu(vector<string> vect1[5], vector<string> vect2[5], vector<int> pozostale[5], int prog)
{
    int numerusunietego;
    int doporownania;
    for (int i = 0; i < 5; i++)
    {
        for (int q = 1; q <= vect1[i].size(); q++)
        {
            pozostale[i].push_back(q);
        }
        numerusunietego = 0; // licznik, indeksu usunietego elementu
        while(numerusunietego < vect2[i].size())
        {
            doporownania = stoi(vect2[i][numerusunietego]); //zamienia wiarygodnosc na int, poniewaz wektor przechowuje je jako stringi
            if (doporownania < prog) //jesli wiarygodnosc jest mniejsza lub rowna podanej wartosci to zostaje ona usunieta z wektoru
            {
                pozostale[i].erase(pozostale[i].begin() + numerusunietego); //usuwa z wektoru indeksy usuniete
                vect1[i].erase(vect1[i].begin() + numerusunietego);
                vect2[i].erase(vect2[i].begin() + numerusunietego);
            }
            else
            {
                numerusunietego++;
            }
        }
    }
    for (int j = 0; j < 5; j++)
    {
        cout << "W SEKWENCJI " << j + 1 << " POZOSTALY ELEMENTY O INDEKSACH:" << endl;
        for (int k : pozostale[j])
        {
            cout << k << " ";
        }
        cout << endl << endl;
    }
}

void PodzielSekwencje(vector<string> vect1[5],vector<int> pozostale[5], vector<int> dografu[2], int podciag)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < (vect1[i].size()-(podciag-1)); j++)
        {
            dografu[0].push_back(i + 1);
            dografu[1].push_back(j + 1);
        }
    }
    //cout << "NUMERY SEKWENCJI:\n";
    for (int i = 0; i<dografu[0].size();i++)
    {
        {
            cout << dografu[0][i] << " " << dografu[1][i];
            cout << endl;
        }
    }
    cout << endl;

}

void UtworzListeSasiedztwa(vector<vector<int>> &listasasiedztwa, vector<string> vect1[5], int podciag)
{
    string temp1, temp2;
    int iterator_nrpozycji1 = 0;
    int iterator_nrpozycji2;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < vect1[i].size()-(podciag-1); j++)
        {
            temp1 = "";
            for (int m = 0; m < podciag; m++) //stworzenie oligonukleotydu 1 do porownania
            {
                temp1 = temp1 + vect1[i][j + m];
            }
            //cout << endl << endl << temp1 << endl << endl ;
            //listasasiedztwa[j].push_back(iterator_nrpozycji1);
            iterator_nrpozycji2 = 0;
            for (int k = 0; k < 5; k++)
            {
                if (k == i)
                {

                    for (int q = 0; q < (vect1[k].size()-(podciag-1)); q++)
                    {
                        iterator_nrpozycji2++;
                    }
                    continue; // pomin jesli k oraz i sa identyczne
                }
                for (int l = 0; l < vect1[k].size()-(podciag-1); l++)
                {
                    temp2 = "";
                    for (int m = 0; m < podciag; m++) //stworzenie oligonukleotydu 2 do porownania
                    {
                        temp2 = temp2 + vect1[k][l + m];
                    }
                    //cout << endl << (abs(pozostale[i][j] - pozostale[k][l])) << endl;
                    //cout << endl << endl << temp2 << endl << endl;
                    if (temp1 == temp2 && (abs((j - l)) <= (podciag * 10))) // jesli oba oligonukleotydy sa identyczne i dystans miedzy nimi nie jest wiekszy niz 10-krotnosc dlugosci podciagu
                    {
                        if (iterator_nrpozycji2 != iterator_nrpozycji1)
                        {
                            if(find(listasasiedztwa[iterator_nrpozycji1].begin(), listasasiedztwa[iterator_nrpozycji1].end(), iterator_nrpozycji2) == listasasiedztwa[iterator_nrpozycji1].end())
                            listasasiedztwa[iterator_nrpozycji1].push_back(iterator_nrpozycji2);
                           /* if (find(listasasiedztwa[iterator_nrpozycji2].begin(), listasasiedztwa[iterator_nrpozycji2].end(), iterator_nrpozycji1) == listasasiedztwa[iterator_nrpozycji2].end())
                            listasasiedztwa[iterator_nrpozycji2].push_back(iterator_nrpozycji1);*/
                        }
                    }
                    iterator_nrpozycji2++;
                }
            }
            iterator_nrpozycji1++;
        }
    }

    for (int i = 0; i < listasasiedztwa.size(); i++)
    {
        sort(listasasiedztwa[i].begin(), listasasiedztwa[i].end());
    }
}

bool CzyZInnychSekwencji(vector<int> temp, vector<int> dografu[2])
{
    set<int> nrsekwencji;
    bool czyzinnych;
    for (int i = 0; i < temp.size(); i++)
    {
        nrsekwencji.insert(dografu[0][temp[i]]);
    }
    if (nrsekwencji.size() == temp.size())
    {
        czyzinnych = true;
    }
    else
    {
        czyzinnych = false;
    }

    return czyzinnych;
}

bool CzyPoJednymZKazdej(vector<int> temp, vector<int> dografu[2])
{
    vector<int> nrsekwencji;
    vector<int> mozliwosci = { 1, 2, 3, 4, 5 };
    bool czypojednym;
    for (int i = 0; i < temp.size(); i++)
    {
        nrsekwencji.push_back(dografu[0][temp[i]]);
    }
    sort(nrsekwencji.begin(), nrsekwencji.end());
    vector<int> intersection;
    set_intersection(nrsekwencji.begin(), nrsekwencji.end(), mozliwosci.begin(), mozliwosci.end(), back_inserter(intersection));
    if (intersection.size() == mozliwosci.size())
    {
        czypojednym = true;
    }
    else
    {
        czypojednym = false;
    }
    return czypojednym;
}
//vector<vector<int>> ZnajdzPodobneDoKliki(vector<vector<int>>& listasasiedztwa)
//{
//    vector<vector<int>> Pkliki;
//    vector<int> temp;
//    for (int i = 0; i < listasasiedztwa.size(); i++)
//    {
//        if (listasasiedztwa[i].size() >= 4)
//        {
//            temp = listasasiedztwa[i];
//            temp.push_back(i);
//            sort(temp.begin(), temp.end());
//            Pkliki.push_back(temp);
//        }
//    }
//    sort(Pkliki.begin(), Pkliki.end());
//    auto last = unique(Pkliki.begin(), Pkliki.end());
//    Pkliki.erase(last, Pkliki.end());
//    return Pkliki;
//}

vector<vector<int>> ZnajdzKlike(vector<vector<int>> &listasasiedztwa, vector<int>dografu[2])
{
    int punkt_kliki;
    bool czyzawiera;
    vector<vector<int>> kliki;
    vector<int> temp;
    vector<int> temp2;
    vector<int> identyczne;
    for (int i = 0; i < listasasiedztwa.size(); i++)
    {
        punkt_kliki = 0;
        temp = listasasiedztwa[i];
        if(temp.size() < 4)
        {
            continue;
        }
        temp.push_back(i);
        sort(temp.begin(), temp.end());
        for (int j = 0; j < listasasiedztwa.size(); j++)
        {
            if (i != j)
            {
                temp2 = listasasiedztwa[j];
                if (temp2.size() < 4)
                {
                    continue;
                }
                temp2.push_back(j);
                sort(temp2.begin(), temp2.end());
                int licznik = 0;
                for (int x : temp)
                {
                    if (find(temp2.begin(), temp2.end(), x) != temp2.end())
                    {

                        licznik++;
                    }
                }

                if (licznik == 5)
                {
                    punkt_kliki++;
                }

                if ((punkt_kliki == 4))
                {
                    for (int x : temp)
                    {
                        if (find(temp2.begin(), temp2.end(), x) != temp2.end())
                        {
                            identyczne.push_back(x); //dodaje tylko wspolne elementy
                        }
                    }
                    if (CzyZInnychSekwencji(identyczne, dografu) == true) //tylko jesli sa z innych sekwencji
                    {
                        kliki.push_back(identyczne);
                        klikaczypodobna = 1;
                        return kliki;

                    }
                    identyczne.clear();
                }
            }
        }
       /* if (punkt_kliki < 4)
        {
            temp.clear();
            temp = listasasiedztwa[i];
            temp.push_back(i);
            if(CzyPoJednymZKazdej(temp, dografu))
            {
                kliki.push_back(temp);
                klikaczypodobna = 2;
                return kliki;
            }
        }*/
            temp2.clear();
        temp.clear();
    }
    for (int i = 0; i < listasasiedztwa.size(); i++)
    {
        temp = listasasiedztwa[i];
        if (temp.size() > 4)
        {
            temp.push_back(i);
            if (CzyPoJednymZKazdej(temp, dografu))
            {
                kliki.push_back(temp);
                klikaczypodobna = 2;
                return kliki;
            }
        }
    }
   /* sort(kliki.begin(), kliki.end());
    auto last = unique(kliki.begin(), kliki.end());
    kliki.erase(last, kliki.end());
    return kliki;*/
    cout << "NIE ZNALEZIONO WIERZCHOLKA, KTORY MA 4 LUB WIECEJ SASIADOW";
}


void WypiszDaneKliki(vector<vector<int>> kliki, vector<int> pozostale[5], vector<string> vect1[5], vector<int> dografu[2], int podciag)
{
    string temp1;
    if(kliki[0].size()<5)
    {
        cout << "NIE ZNALEZIONO KLIKI LUB STRUKTURY PODOBNEJ DO KLIKI" << endl;
        return;
    }
    if (klikaczypodobna == 1)
    {
        for (int i = 0; i < kliki.size(); i++)
        {
            cout << "KLIKA " << i << ":" << endl;
            for (int j = 0; j < kliki[i].size(); j++)
            {
                int nrsek = dografu[0][kliki[i][j]] - 1;
                int nrpoz = dografu[1][kliki[i][j]] - 1;
                temp1 = "";
                for (int m = 0; m < podciag; m++)
                {
                    temp1 = temp1 + vect1[nrsek][nrpoz + m];
                }
                cout << "Indeks wierzcholka : " << kliki[i][j] << " | NR sekwencji wejsciowej: " << dografu[0][kliki[i][j]] << " | NR pozycji: " << pozostale[nrsek][nrpoz] << " | Podciag: " << temp1 << endl;
            }
            cout << endl;
        }
    }
    if (klikaczypodobna == 2)
    {
        for (int i = 0; i < kliki.size(); i++)
        {
            cout << "STRUKTURA NUMER " << i << ":" << endl;
            for (int j = 0; j < kliki[i].size(); j++)
            {
                int nrsek = dografu[0][kliki[i][j]] - 1;
                int nrpoz = dografu[1][kliki[i][j]] - 1;
                temp1 = "";
                for (int m = 0; m < podciag; m++)
                {
                    temp1 = temp1 + vect1[nrsek][nrpoz + m];
                }
                cout << "Indeks wierzcholka : " << kliki[i][j] << " | NR sekwencji wejsciowej: " << dografu[0][kliki[i][j]] << " | NR pozycji: " << pozostale[nrsek][nrpoz] << " | Podciag: " << temp1 << endl;
            }
            cout << endl;
        }
    }
}
int main()
{
    int wybor, prog, podciag;
    string nazwapliku1, nazwapliku2;
    vector<int> dografu[2];
    vector<int> pozostale[5];
    vector<vector<int>> kliki, Pkliki;
    vector<string> fasta[5];
    vector<string> qual[5];
    cout << "Podaj dlugosc podciagu: ";
    cin >> podciag;
    cout << endl;

    while (1)
    {
        cout << "Wybierz opcje:\n1) Wczytaj plik\n2) Pokaz instancje\n3) Usun ponizej podanego progu\n4) Utworz podciagi\n5) Utworz graf\n6) Znajdz kliki\n";
        cin >> wybor;

        switch (wybor)
        {
        case 1:
            cout << "Podaj nazwę pliku FASTA (bez '.fasta')\n";
            cin >> nazwapliku1;
            cout << "Podaj nazwę pliku qual (bez '.qual')\n";
            cin >> nazwapliku2;
            WczytajInstancje(nazwapliku1, nazwapliku2, fasta, qual);
            break;

        case 2:
            PokazInstancje(fasta, qual);
            break;

        case 3:
            cout << "Podaj prog, ponizej ktorego nukleotydy maja byc usuniete:\n";
            cin >> prog;
            UsunPonizejProgu(fasta, qual, pozostale, prog);
            break;

        case 4:
            PodzielSekwencje(fasta, pozostale, dografu, podciag);
            cout << endl << "ILOSC INDEKSOW W DOGRAFU:" << endl << dografu[1].size() << endl;
            break;

        case 5:
            int wielkoscdografu = dografu[1].size();
            vector<vector<int>> listasasiedztwa(wielkoscdografu);
            UtworzListeSasiedztwa(listasasiedztwa, fasta, podciag);
            for (int i = 0; i < listasasiedztwa.size(); i++)
            {
                cout << i << " -> "; // 0 ->
                for (int j = 0; j < listasasiedztwa[i].size(); j++)
                {
                    cout << listasasiedztwa[i][j] << ", ";
                }
                cout << endl;
            }
            kliki = ZnajdzKlike(listasasiedztwa, dografu);
            /*cout << "ZNALEZIONE KLIKI: " << endl;
            for (int i = 0; i < kliki.size(); i++)
            {
                for (int j = 0; j < kliki[i].size(); j++)
                {
                    cout << kliki[i][j] << " ";
                }
                cout << endl;
            }*/
           /* Pkliki = ZnajdzPodobneDoKliki(listasasiedztwa);
            cout << "ZNALEZIONE PODOBNE DO KLIKI: " << endl;
            for (int i = 0; i < Pkliki.size(); i++)
            {
                for (int j = 0; j < Pkliki[i].size(); j++)
                {
                    cout << Pkliki[i][j] << " ";
                }
                cout << endl;
            }*/

            WypiszDaneKliki(kliki, pozostale, fasta, dografu, podciag);
        }

    }
}

