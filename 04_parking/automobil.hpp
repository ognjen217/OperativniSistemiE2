#ifndef AUTOMOBIL_HPP_INCLUDED
#define AUTOMOBIL_HPP_INCLUDED

#include <iostream>
#include <mutex>
#include "parking.hpp"

using namespace std;

class Automobil {
private:
    mutex terminal;
public:
    // rbr - Redni broj automobila
    void ceka(int rbr, int freeSpotCounter) {
        unique_lock<mutex> u(terminal);
        cout << "Automobil " << rbr << " ceka jer je parking zauzet." << "\tfreeSpotCounter: " << freeSpotCounter << endl;
    }

    // rbr - Redni broj automobila
    void parkira(int rbr, int freeSpotCounter) {
        unique_lock<mutex> u(terminal);
        cout << "Automobil " << rbr << " stupa na parking.\t" << "\tfreeSpotCounter: " << freeSpotCounter << endl;
    }

    // rbr - Redni broj automobila
    void napusta(int rbr, int freeSpotCounter) {
        unique_lock<mutex> u(terminal);
        cout << "Automobil " << rbr << " je izasao sa parkinga." << "\tfreeSpotCounter: " << freeSpotCounter << endl;
    }
};

#endif // AUTOMOBIL_HPP_INCLUDED
