/*
 * Napisati C++ program koji cita podatke o uniformnim distribucijama iz ulazne datoteke i potom za
 * svaku distribuciju generise po 10 brojeva, racuna njihov prosek, i nalazi najmanji i najveci broj. 
 * 
 * U ulaznoj datoteci "distribucije.txt" se u svakom redu nalaze informacije o donjoj i gornjoj granici
 * intervala u kojem treba generisati brojeve. Informacije o gornjoj i donjoj granici su odvojene dvotackom.
 * U pitanju su razlomljeni brojevi.
 * 
 * U izlaznoj datoteci "brojevi.csv" treba da se nalaze u jednom redu odvojeni zarezom prvo 10 
 * izgenerisanih brojeva a potom i prosek, najmanji element i najveci element.
 * 
 * Treba napraviti jednu nit koja ce samo citati ulazne podatke, jednu nit koja ce samo pisati gotove
 * brojeve u datoteku i 6 niti radnika koji ce na osnovu podataka iz ulazne datoteke generisati sve
 * neophodno za ispis u izlaznu datoteku.
*/


/* Klasa koja modeluje "postansko sanduce" izmedju citaca i radnika.
*/

#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <queue>
#include <condition_variable>
#include <algorithm>

using namespace std;
using namespace chrono;

template<typename T>

class UlazniPodaci {
private:
    mutex podaci_mx;   // propusnica za sinhronizaciju nad svim poljima klase
    int br_radnika;
    bool kraj;
    queue<T> kolekcija;
    condition_variable cv;

public:

    UlazniPodaci(): kraj(false), br_radnika(0) {}  // na pocetku nije kraj i nema radnika
    void dodaj(T red) {
        unique_lock<mutex> lock(podaci_mx);
        kolekcija.push(red);
        cv.notify_one();
    }

    bool skloni(T &red) {

        unique_lock<mutex> lock(podaci_mx);
        
        while (kolekcija.empty() && !kraj){
            cv.wait(lock);
        }

        if(kolekcija.empty()){
            red = kolekcija.front();
            kolekcija.pop();
            return true;
        }

        return false;
        
    }

    /**
     * Provera da li smo zavrsili sa citanjem podataka. Vraca istinu samo onda kada nema vise podataka
     * u kolekciji i sve niti pisaci u kolekciju su se odjavili.
    */

    bool jeLiKraj() {
        return kraj;
    }

    void prijaviSe(){
        unique_lock<mutex> lock(podaci_mx);
        br_radnika++;
    }

    void odjaviSe(){
        unique_lock<mutex> lock(podaci_mx);
        br_radnika--;

        if(br_radnika == 0) {
            kraj = true;
            cv.notify_all();
        }
    }
};


/** Klasa koja modeluje "postansko sanduce" izmedju radnika i pisaca.
*/
template<typename T>

class IzlazniPodaci {

private:
    mutex podaci_mx;         // propusnica za sinhronizaciju nad svim poljima klase
    int br_radnika;
    bool kraj;
    queue<T> kolekcija;
    condition_variable cv;

public:

    IzlazniPodaci(): kraj(false), br_radnika(0) {}  // na pocetku nije kraj i nema radnika
    
    void dodaj(T red) {
        unique_lock<mutex> lock(podaci_mx);
        kolekcija.push(red);
        cv.notify_one();
    }

    bool skloni(T &red) {
        unique_lock<mutex> lock(podaci_mx);
        
        while (kolekcija.empty() && !kraj){
            cv.wait(lock);
        }

        if(kolekcija.empty()){
            red = kolekcija.front();
            kolekcija.pop();
            return true;
        }
        return false;
    }

    /*
     * Provera da li smo zavrsili sa citanjem podataka. Vraca istinu samo onda kada nema vise podataka
     * u kolekciji i sve niti pisaci u kolekciju su se odjavili.
    */

    bool jeLiKraj() {
        return kraj;
    }

    void prijaviSe(){
        unique_lock<mutex> lock(podaci_mx);
        br_radnika++;
    }

    void odjaviSe(){
        unique_lock<mutex> lock(podaci_mx);
        br_radnika--;

        if(br_radnika == 0) {
            kraj = true;
            cv.notify_all();
        }
    }
};


/**
 * Logika radnika - niti koje vrse transformaciju ulaznih podataka u izlazne podatke spremne za ispis.
*/

void radnik(UlazniPodaci<pair<double, double>> &ulaz, IzlazniPodaci<vector<double>> &izlaz) {
    izlaz.prijaviSe();

    default_random_engine gen;
    gen.seed(duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count());

    pair<double, double> parametri;
    double mean, max, summ, minimum;

    while(ulaz.skloni(parametri)){
        uniform_real_distribution <double> distribucija(parametri.first, parametri.second);
        vector<double> brojevi;
        summ = minimum = max = distribucija(gen);
        brojevi.push_back(max);

        for(int i = 10; i > 0; i--){
            double broj = distribucija(gen);
            brojevi.push_back(broj);

            if(minimum > broj)
                minimum = broj;
            if(max < broj)
                max = broj;
            summ += broj;
        }
        //
            brojevi.push_back(summ/10);
            brojevi.push_back(minimum);
            brojevi.push_back(max);
        //
            izlaz.dodaj(brojevi);
    }
    izlaz.odjaviSe();
}


/**
 * Logika citaca_iz_datoteke - nit koja radi citanje iz ulazne datoteke i salje u ulaznu kolekciju za radnike
*/

void citacf(string ime_ulazne_dat, UlazniPodaci<pair<double, double>> &ulaz) {

    ulaz.prijaviSe();

    ifstream inFile;
    inFile.open(ime_ulazne_dat);

    double donja_granica, gornja_granica;
    while(!inFile.eof()){
        inFile >> donja_granica >> gornja_granica;
        ulaz.dodaj({donja_granica, gornja_granica});
    }

    inFile.close();
    ulaz.odjaviSe();
}

/*
 * Logika pisaca_u_datoteku - nit koja radi pisanje u izlaznu datoteku podataka dobijenih od radnika
*/

void pisacf(IzlazniPodaci<vector<double>> &izlaz, string ime_izlazne_dat) {
    
    ofstream outFile;
    outFile.open(ime_izlazne_dat);

    vector <double> v;
    while(izlaz.skloni(v)){
        for(double val : v ) {
           outFile << val << ","; 
        }
        outFile << endl;
        v.resize(0);
    }
    outFile.close();
}

int main() {
    UlazniPodaci<pair<double, double>> ulazni_podaci;  // bafer podataka koje salje citac_iz_datoteke a obradjuju radnici
    IzlazniPodaci<vector<double>> izlazni_podaci;  // bafer podataka koje pripremaju radnici a ispisuju se u datoteku u pisacu_u_datoteku

    thread citac{citacf, "distribucije.txt", ref(ulazni_podaci)},  // stvaranje niti citaca_iz_datoteke
           pisac{pisacf, ref(izlazni_podaci), "brojevi.csv"},      // stvaranje niti pisaca_u_datoteku
           radnici[6];

    for (thread &nit: radnici)
        nit = thread(radnik, ref(ulazni_podaci), ref(izlazni_podaci));  // stvaranje niti radnika

    for (thread &nit: radnici)
        nit.join();
    
    citac.join();
    pisac.join();

    return 0;
    
}
