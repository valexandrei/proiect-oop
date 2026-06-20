#ifndef STIVA_H
#define STIVA_H

#include <vector>
#include <stdexcept>
#include <iostream>

template<typename T>
class Stiva {
private:
    std::vector<T> elemente;
    size_t capacitate;

public:
    explicit Stiva(size_t cap = 100) : capacitate(cap) {}

    void push(const T& elem) {
        if (elemente.size() >= capacitate)
            throw std::overflow_error("Stiva plina!");
        elemente.push_back(elem);
    }

    T pop() {
        if (elemente.empty())
            throw std::underflow_error("Stiva goala!");
        T top = elemente.back();
        elemente.pop_back();
        return top;
    }

    const T& top() const {
        if (elemente.empty())
            throw std::underflow_error("Stiva goala!");
        return elemente.back();
    }

    bool esteGoala() const { return elemente.empty(); }
    size_t marime() const { return elemente.size(); }

    void afiseaza(std::ostream& os) const {
        os << "Stiva [" << elemente.size() << "/" << capacitate << "]: ";
        for (const auto& e : elemente)
            os << e << " ";
        os << "\n";
    }
};

template<typename T>
T maximDinStiva(Stiva<T> stiva) {
    if (stiva.esteGoala())
        throw std::underflow_error("Stiva goala!");
    T maxVal = stiva.pop();
    while (!stiva.esteGoala()) {
        T val = stiva.pop();
        if (val > maxVal) maxVal = val;
    }
    return maxVal;
}

#endif