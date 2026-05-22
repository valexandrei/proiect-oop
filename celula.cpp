#include "celula.h"
#include <stdexcept>

// Returneaza caracterul vizual al celulei: '#' pentru perete, '.' pentru coridor
char Celula::getSimbol() const {
    return perete ? '#' : '.';
}

// Inverseaza tipul celulei (perete <-> coridor)
void Celula::toggle() {
    perete = !perete;
}

// Verifica daca doua celule sunt de acelasi tip
bool Celula::operator==(const Celula& other) const {
    return perete == other.perete;
}

bool Celula::operator!=(const Celula& other) const {
    return !(*this == other);
}