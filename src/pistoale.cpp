#include "pistoale.h"
#include <iostream>

Pistoale::Pistoale(int v, int d, int m) : Arma("Pistoale gemene", v, d), munitie(m) {}

void Pistoale::foloseste() {
    if (munitie > 0) {
        munitie--;
        std::cout << "BANG! Ai tras cu pistoalele. Damage: " << damage
                  << ". Gloante ramase: " << munitie << "\n";
    } else {
        std::cout << "*Click*... Nu mai ai munitie! Trebuie sa reincarci.\n";
    }
}

void Pistoale::afisare() const {
    Arma::afisare();
    std::cout << " [Tip: Arma de foc | Munitie: " << munitie << "]\n";
}

void Pistoale::reincarca() {
    munitie = 6;
    std::cout << "Pistoalele au fost reincarcate!\n";
}