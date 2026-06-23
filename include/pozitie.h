    #ifndef POZITIE_H
    #define POZITIE_H
    #include <iostream>

    class Pozitie {
    private:
        int x, y;

    public:
        explicit Pozitie(int _x = 0, int _y = 0);

        [[nodiscard]] int getX() const;
        [[nodiscard]] int getY() const;

        void setX(int val) { x = val; }
        void setY(int val) { y = val; }

        friend std::ostream& operator<<(std::ostream& os, const Pozitie& p);
    };

    #endif