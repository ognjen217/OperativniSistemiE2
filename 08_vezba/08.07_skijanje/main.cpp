/**
 * Napisati C++ program koji iz ulazne datoteke cita podatke o temperaturi u toku vikenda sa ski 
 * staza na Kopaoniku, Zlatiboru i Jahorini i odredjuje koji dani i na kojoj planini su idealni 
 * za skijanje a koji nisu. Neki idealni opseg temperature za skijanje je od -5 do 3 stepena.
 * 
 * Za svaku od planina postoji posebna datoteka cije ime se sastoji od imena planine i prosirenja
 * (ekstenzije) ".txt". U svakoj pojedinacnoj datoteci se u jednom redu nalaze podaci za jedan dan. 
 * Jedan red sadrzi redom ime_dana, datum, i potom izmerene temperature. Temperatura se meri na 
 * svakih sat vremena, pocevsi od 8 ujutru, do 8 uvece. Svi podaci su odvojeni razmakom.
 * 
 * Izgled jednog reda iz ulaznih datoteka "Kopaonik.txt" "Zlatibor.txt" "Jahorina.txt"
 * 
 *     subota 01.02.  -15 -13 -10 -8 -3 0 -2 -3 2 2 -5 -7 -3
 * 
 * Treba za svaki dan pronaci najnizu i najvisu dnevnu temperaturu. Ukoliko minimalna i maksimalna
 * temperatura upadaju u navedeni opseg, treba informacije za taj dan ispisati u datoteku 
 * "idealno.txt", u suprotnom u datoteku "lose.txt".
 *
 * Ispis u izlaznu datoteku treba da prati format:
 *     <ime_planine> [<ime_dana> <datum>] <min. temp.> >> <maks. temp.>
 * 
 * Primer ispisa u bilo kojoj od izlaznih datoteka "idealno.txt", "lose.txt":
 * 
 *     Kopaonik [subota 01.02.] -15 >> 2
 *
 * Treba napraviti jednu nit koja ce samo citati podatke iz ulaznih datoteka, jednu nit koja ce 
 * samo pisati spremljene podatke u izlazne datoteke i 4 niti radnika koji ce na osnovu podataka iz
 * ulaznih datoteka generisati sve neophodno za ispis u izlazne datoteke.
*/

using namespace std;

class Data {
private:
public:
};

template<typename T>
class InputData {
private:
    mutex data_mtx;
public:
    Data(): end(false), workers_num(0) {}
    void add_data(T data_element) {
    }
    bool remove_data(T &data_element) {
        return false;
    }

    bool the_end() {
        return data.empty() && end;
    }

};

template<typename T>
class OutputData {
private:
    mutex data_mtx;
public:
    Data(): end(false), workers_num(0) {}
    void add_data(T data_element) {
    }
    bool remove_data(T &data_element) {
        return false;
    }

    bool the_end() {
        return data.empty() && end;
    }

};

void reader(vector<string> input_filenames, InputData<string>& raw_data) {
}

void proccessing_data(InputData<string>& raw_data, OutputData<Data>& proccessed_data){
}

void writer(OutputData<Student>& proccessed_data) {
}


int main() {
    InputData<string> raw_data;
    OutputData<Student> proccessed_data;
    vector<string> input_filenames = {"Kopaonik.txt", "Zlatibor.txt", "Jahorina.txt"};

    thread th_reader(reader, input_filenames, ref(raw_data));
    thread th_writer(writer, ref(proccessed_data));
    thread th_workers[4];

    for(auto &th: th_workers){
        th = thread(proccessing_data, ref(raw_data), ref(proccessed_data));
    }

    th_reader.join();
    for(auto &th: th_workers) {
        th.join();
    }
    th_writer.join();
    
    return 0;
}