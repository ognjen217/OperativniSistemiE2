#ifndef PARKING_HPP_INCLUDED
#define PARKING_HPP_INCLUDED

#include "automobil.hpp"
#include <condition_variable> 
using namespace std;

class Parking {
private:
    Automobil& automobil;
    enum Stanje{SLOBODAN, ZAUZET};
    //Stanje stanje;
    mutex m;
    condition_variable cv;
public:
    static int freeSpotCounter;

    Parking(Automobil& a) : automobil(a) /*, stanje(SLOBODAN)*/ {
        // Proširiti po potrebi ...
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kako bi automobil pokušao da se parkira.
    // Ako je parking zauzet, izvršenje ove metode će trajati dok se parking ne oslobodi i auto se parkira.
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.ceka kada je parking zauzet i auto mora da čeka.
    // Potrebno je pozvati metodu automobil.parkira kada auto uspe da se parkira.

    void udji(int rbr) {
        unique_lock<mutex> lock(m);

        while (freeSpotCounter == 0){
            if(freeSpotCounter > 0)
                break;
            cv.wait(lock);
            automobil.ceka(rbr, freeSpotCounter);
        }

        freeSpotCounter--;
        //stanje = ZAUZET;
        automobil.parkira(rbr, freeSpotCounter);
        
        // Implementirati ...
        // automobil.ceka(rbr);
        /* unique_lock<mutex> lock(m);
        while(true){
            unique_lock<mutex> lock(m);
            if(stanje == SLOBODAN) {
                stanje = ZAUZET;
                automobil.parkira(rbr);
                break;
            } 
        }
        */
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kada auto izlazi sa parkinga (nakon što je bio parkiran).
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.napusta kada auto napušta parking mesto.
    void izadji(int rbr) {
        // Implementirati ...
        
        unique_lock<mutex> lock(m);
        freeSpotCounter++;
        //stanje = SLOBODAN;
        automobil.napusta(rbr, freeSpotCounter);
        cv.notify_one();
        
        /*unique_lock<mutex> lock(m);
        stanje = SLOBODAN;
        
        */
    }
};

#endif // PARKING_HPP_INCLUDED
