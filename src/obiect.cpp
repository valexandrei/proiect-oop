#include "obiect.h"

Obiect::Obiect(std::string n, int v) : nume(std::move(n)), valoare(v) {}

void Obiect::afisare() const {
    std::cout << "Obiect: " << nume << " (Valoare: " << valoare << ")";
}