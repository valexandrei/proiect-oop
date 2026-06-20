#include "gamedata.h"

std::string GameData::getPovesteFundal() {
    return "In vremurile de demult, acest dungeon era inima regatului pierdut. "
           "Legendele spun ca regele blestemat si-a ascuns comorile in cele mai adanci cotloane, "
           "pazite de creaturi nascute din umbra si disperare. Tu, un simplu calator, "
           "ai fost atras de promisiunea bogatiei, dar acum lupti doar pentru supravietuire. "
           "Fiecare pas in labirint te aduce mai aproape de adevarul intunecat al acestui loc.";
}

std::string GameData::getDescriereInamic(const std::string& nume) {
    if (nume == "Goblin")
        return "O creatura mica, verde si vicleana, care prefera atacurile prin surprindere.";
    if (nume == "Orc")
        return "Un razboinic masiv, condus de o furie oarba si o forta bruta devastatoare.";
    if (nume == "Schelet")
        return "Ramasitele animate ale unui fost soldat, tinute impreuna de magie neagra.";
    if (nume == "Spectra")
        return "O entitate eterica ce ignora partial armura si loveste direct in spiritul victimei.";
    return "O creatura necunoscuta care emana o aura de pericol.";
}

std::string GameData::getMesajLevelUp(int nivel) {
    switch (nivel) {
        case 2: return "Simti cum puterea iti curge prin vene. Esti mai rapid si mai agil.";
        case 3: return "Experienta dobandita te face sa vezi punctele slabe ale inamicilor.";
        case 4: return "Corpul tau s-a calit in lupte. Loviturile dor mai putin acum.";
        case 5: return "Ai devenit un maestru al supravietuirii in conditii extreme.";
        default: return "Calea spre glorie continua. Devii din ce in ce mai puternic.";
    }
}

std::vector<std::string> GameData::getTipuriInamici() {
    return {
        "Goblin Infatometat",
        "Orc Distrugator",
        "Schelet Arcas",
        "Spectra Blestemata",
        "Garda Regala Corupta",
        "Paianjen de Pestera",
        "Lich Stravechi"
    };
}