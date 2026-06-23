#ifndef INAMICFACTORY_H
#define INAMICFACTORY_H

#include <memory>
#include <string>
#include "entitate.h"
#include "pozitie.h"

class InamicCreator {
public:
    virtual ~InamicCreator() = default;
    virtual std::unique_ptr<Entitate> creeaza(const std::string& nume,
                                               Pozitie pos, int hp, int dmg) const = 0;
    virtual std::string numeImplicit() const = 0;
};

class GoblinCreator : public InamicCreator {
public:
    std::unique_ptr<Entitate> creeaza(const std::string& nume,
                                       Pozitie pos, int hp, int dmg) const override;
    std::string numeImplicit() const override { return "Goblin"; }
};

class OrcCreator : public InamicCreator {
public:
    std::unique_ptr<Entitate> creeaza(const std::string& nume,
                                       Pozitie pos, int hp, int dmg) const override;
    std::string numeImplicit() const override { return "Orc"; }
};

class FantomaCreator : public InamicCreator {
public:
    std::unique_ptr<Entitate> creeaza(const std::string& nume,
                                       Pozitie pos, int hp, int dmg) const override;
    std::string numeImplicit() const override { return "Fantoma"; }
};

class VrajitorCreator : public InamicCreator {
public:
    std::unique_ptr<Entitate> creeaza(const std::string& nume,
                                       Pozitie pos, int hp, int dmg) const override;
    std::string numeImplicit() const override { return "Vrajitor"; }
};

class ScheletCreator : public InamicCreator {
public:
    std::unique_ptr<Entitate> creeaza(const std::string& nume,
                                       Pozitie pos, int hp, int dmg) const override;
    std::string numeImplicit() const override { return "Schelet"; }
};

class BossCreator : public InamicCreator {
public:
    std::unique_ptr<Entitate> creeaza(const std::string& nume,
                                       Pozitie pos, int hp, int dmg) const override;
    std::string numeImplicit() const override { return "Boss"; }
};

class InamicFactory {
public:
    enum class Tip { Goblin, Orc, Fantoma, Vrajitor, Schelet, Boss };

    static std::unique_ptr<Entitate> creeaza(Tip tip, const std::string& nume,
                                              Pozitie pos, int hp, int dmg = 0);
    static std::string numeImplicit(Tip tip);

private:
    static const InamicCreator& getCreator(Tip tip);
};

#endif