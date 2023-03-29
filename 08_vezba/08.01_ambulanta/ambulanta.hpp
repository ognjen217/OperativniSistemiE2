#ifndef AMBULANTA_HPP_INCLUDED
#define AMBULANTA_HPP_INCLUDED

#include "student.hpp"
#include <mutex>
#include <condition_variable>

using namespace std;

class Studentska_ambulanta {
private:
    Student& student;
    mutex m;
    int brmuskih;
    int brzenskih;
    condition_variable c;
public:
    Studentska_ambulanta(Student& st, int muskih, int zenskih) : student(st),brmuskih(muskih),brzenskih(zenskih) {
        // Prosiriti po potrebi ...

    }

    // Metoda koju poziva nit koja simulira studenta u ambulanti kada student pokusava da udje u nju.
    // Ukoliko je ambulanta zauzeta, izvrsenje ove metode ce cekati da se ona oslobodi.
    //
    // rbr - Redni broj studenta
    // p   - Pol studenta
    //
    // Potrebno je pozvati metodu student.ceka kada student ne moze da stupi u ambulantu.
    // Potrebno je pozvati metodu student.ulazi kada student udje u ambulantu.
    void udji(int rbr, Pol p) {
        // Implementirati ...
        unique_lock<mutex> l(m);

        while(true){
            if(p == MUSKI) {
                if(brmuskih > 0)
                    break;
            } else if (p == ZENSKI){
                if(brzenskih > 0)
                    break;
            }
            student.ceka(rbr,p);
            c.wait(l);
        }

        if(p == MUSKI){
            while(true){
            if(brmuskih <= 0){
                student.ceka(rbr,p);
                c.wait(l);
            }else{
                student.ulazi(rbr,p);
                brmuskih--;
                break;
            }
            }
        }else{
            while(true){
            if(brzenskih <= 0){
                student.ceka(rbr,p);
                c.wait(l);
            }else{
                student.ulazi(rbr,p);
                brzenskih--;
                break;
            }
            }
        }
    }

    // Metoda koju poziva nit koja simulira studenta u ambulanti nakon sto je student pregledan i izlazi iz nje.
    //
    // rbr - Redni broj studenta
    // p   - Pol studenta
    void izadji(int rbr, Pol p) {
        // Implementirati ...
        unique_lock<mutex> l(m);
        if(p == MUSKI){
            brmuskih++;
            c.notify_one();
        }else{
            brzenskih++;
            c.notify_one();
        }
    }
};

#endif // AMBULANTA_HPP_INCLUDED
