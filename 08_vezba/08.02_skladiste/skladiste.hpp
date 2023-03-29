#ifndef SKLADISTE_HPP_INCLUDED
#define SKLADISTE_HPP_INCLUDED

#include "kamion.hpp"
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

class Skladiste {
private:
    Kamion& kamion;
    mutex m;
    condition_variable normal;
    condition_variable boom;
    int brboom = 0;
    int rampa = 2;

    
public:
    Skladiste(Kamion& k) : kamion(k) {
        // Prosiriti po potrebi ...
    }

    // Metoda koju poziva nit koja simulira kretanje kamiona kada on pokusava da istovari robu.
    // Metoda je blokirajuca - ako su sve rampe za istovar zauzete, izvrsenje ce blokirati dok se neka ne oslobodi.
    //
    // rbr       - Redni broj kamiona
    // kolicina  - Kolicina robe koja se prevozi (u tonama)
    // zapaljivo - Ukazuje na to da li je roba koja se prevozi zapaljiva (takva roba ima prioritet pri istovaru!)
    //
    // Potrebno je pozvati metodu kamion.ceka kada su rampe zauzete i kamion mora da ceka.
    // Potrebno je pozvati metodu kamion.istovara kada zapocne istovar robe iz kamiona (primetiti da ta metoda zahteva da joj se prosledi indeks rampe na kojoj se obavlja istovar!)
    // Potrebno je pozvati metodu kamion.odlazi kada je kamion zavrsio istovar i odlazi.
    void istovari(int rbr, int kolicina, bool zapaljivo) {
        {
            //if(zapaljivo) brboom++;
            unique_lock<mutex> l(m);
            
                if(zapaljivo){
                    while(true){
                        if(rampa >=1){
                            kamion.istovara(rbr,kolicina,zapaljivo,rampa);
                            brboom++;
                            rampa--;
                            break;
                        } else {
                            kamion.ceka(rbr,kolicina,zapaljivo);
                            boom.wait(l);
                        }
                    }
                }else{
                    normal.wait(l);
                    kamion.ceka(rbr,kolicina,zapaljivo);
                    while(true){
                        if(rampa >= 1){
                            kamion.istovara(rbr,kolicina,zapaljivo,rampa);
                            rampa--;
                            break;
                        } else {
                            kamion.ceka(rbr,kolicina,zapaljivo);
                            normal.wait(l);
                        }
                    }
                }
            }
        
        this_thread::sleep_for(chrono::milliseconds(kolicina * 1000));
        {
            unique_lock<mutex> l(m);
            if(zapaljivo){
                kamion.odlazi(rbr);
                rampa++;
                brboom--;
                boom.notify_one();
            }
            if(brboom == 0){
                normal.notify_one();
            }
            if(!zapaljivo){
                kamion.odlazi(rbr);
                rampa++;
                normal.notify_one();
            }
        }
    }
};

#endif // SKLADISTE_HPP_INCLUDED