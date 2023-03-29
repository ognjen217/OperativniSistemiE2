/*
Proširiti zadatak najbilzi_nuli_vreme ispisom u sekundama i minutima.
*/

/*
Napraviti konkurentni program koji pronalazi element najbliži broju 0 u zadatom vektoru brojeva.

Pretragu podeliti u 3 programske niti. Ulazni vektor brojeva treba da sadrži 900.000 slučajno generisanih brojeva.

Za svaku nit, evidentirati trenutke kada je izvršavanje počelo i kada se završilo. Na kraju programa, ispisati koliko je trajalo izvršavanje svake niti u milisekundama.
*/

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <cmath>
#include <chrono>

using namespace std;
using namespace chrono;

typedef vector<double>::const_iterator cit;

struct vreme
{
    system_clock::time_point pocetak;
    system_clock::time_point kraj;
};

void f(cit pocetak, cit kraj, double &min, struct vreme *v)
{
    v->pocetak = system_clock::now();
    min = *pocetak;
    for (cit it = pocetak + 1; it != kraj; ++it)
    {
        if (abs(*it) < abs(min))
            min = *it;
    }
    v->kraj = system_clock::now();
}

const int BROJ_NITI = 3;
const int DUZINA = 900000;

int main()
{
    int SEGMENT = DUZINA / BROJ_NITI;
    default_random_engine gen;

    gen.seed(duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count()); // Inicijalizacija generatora slučajnih brojeva da pri svakom pokretanju daje različite brojeve
    uniform_real_distribution<double> real_dist(-1e5, 1e5);

    vector<double> v(DUZINA);

    // Puni se vektor pseudo-slučajnim brojevima
    for (int i = 0; i < DUZINA; i++)
        v[i] = real_dist(gen);


    double minimumi[BROJ_NITI];      // Niz elemenata najbližih nuli - svaka programska nit će dati svoj međurezltat
    struct vreme vremena[BROJ_NITI]; // Niz struktura u koji će biti upisani podaci o trajanju izvršavanja svake niti

    thread niti[BROJ_NITI];

    cit pocetak = v.begin();
    cit kraj = pocetak + SEGMENT;

    for (int i = 0; i < BROJ_NITI; i++)
    {
        
        niti[i] = thread(f, pocetak, kraj, ref(minimumi[i]), &vremena[i]);
        pocetak = kraj;
        kraj += SEGMENT;
    
    }

    for (int i = 0; i < BROJ_NITI; i++)
    {
        niti[i].join();

        // auto trajanje = duration_cast<milliseconds>(vremena[i].kraj - vremena[i].pocetak).count();
        // auto trajanje_sec = duration_cast<seconds>(vremena[i].kraj - vremena[i].pocetak).count();
        // auto trajanje_min = duration_cast<minutes>(vremena[i].kraj - vremena[i].pocetak).count();

        duration<double, milli> trajanje_mili = vremena[i].kraj - vremena[i].pocetak;
        duration<double, ratio<60>> trajanje_min = vremena[i].kraj - vremena[i].pocetak;
        duration<double> trajanje_sec = vremena[i].kraj - vremena[i].pocetak;

        cout << "Nit " << i << " trajanje: " << trajanje_mili.count() << " ms" << endl;
        cout << "Nit " << i << " trajanje: " << trajanje_min.count() << " min i " << trajanje_sec.count() << " sec" << endl;
    }

    double najblizi = minimumi[0];
    
    for (int i = 1; i < BROJ_NITI; i++)
    {
        if (abs(minimumi[i]) < abs(najblizi))
            najblizi = minimumi[i];
    }

    cout << "Najblizi nuli je: " << najblizi << " ms" << endl;

    return 0;
}
