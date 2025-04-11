#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

class QNS {
    int powerLevel;
    int durability;
    int energyStorage;
    int heatLevel;
    bool destroyed;

    public:
    QNS (int p = 1000, int d = 500, int e = 300, int h = 0) {
        if (p > 5000) powerLevel = 5000;
        else powerLevel = p;
        durability = d;
        energyStorage = e;
        if (h < 0) heatLevel = 0;
        else heatLevel = h;
        if (durability < 0) destroyed = true;
        else destroyed = false;
    }

    QNS (const QNS& other) {
        powerLevel = other.powerLevel;
        durability = other.durability;
        energyStorage = other.energyStorage;
        heatLevel = other.heatLevel;
        if (durability < 0) destroyed = true;
        else destroyed = false;
    }

    QNS& operator + (QNS& other) { //upgrade suit
        powerLevel += other.energyStorage;
        if (powerLevel > 5000) powerLevel = 5000;
        durability += other.durability;
        energyStorage += other.powerLevel;
        return *this;
    }

    QNS& operator - (int x) { // simulate damage // if d <= 0 suit is destroyed
        durability -= x;
        energyStorage += x;
        heatLevel += x;
        if (durability < 0) destroyed = true;
        return *this;
    }

    QNS& operator * (int x) { // activates Power Boost mode
        powerLevel += (powerLevel*x)/100;
        if (powerLevel > 5000) powerLevel = 5000;
        energyStorage += 5*x;
        heatLevel += x;
        return *this;
    }

    QNS& operator / (int x) {  // activates the cooling system // if heatLevel <= 0 set it to 0
        durability += x;
        heatLevel -= x;
        if(heatLevel<0)heatLevel=0;
        return *this;
    }

    void boostPower(int factor) { //add power // use * operator
        *this*factor;
    }

    void boostPower(QNS otherSuit) { // transfers energy from another suit to boost durability
        *this+otherSuit;
    }

    bool operator == (QNS& other) {
        if (powerLevel == other.powerLevel && durability == other.durability) {
            return true;
        }
        return false;
    }

    bool operator < (QNS& other) {
        if (powerLevel + durability < other.powerLevel + other.durability) return true;
        return false;
    }

    int getDurability() { // coupled with smart ptr to get durability
        return durability;
    }

    void decDurability(int x) {
        durability -= x;
    }

    int getHeatLevel() { // coupled with smart ptr to get heatLevel
        return heatLevel;
    }

    int getPowerLevel() {
        return powerLevel;
    }

    int getEnergyStorage() {
        return energyStorage;
    }

    void setDestroyed(bool set) {
        destroyed = set;
    }

    bool getDestroyed() {
        return destroyed;
    }
};

class Avenger {
    string name;
    QNS suit;
    int attackStrength;

    public:
    Avenger(string avName, QNS avSuit , int strength) {
        name = avName;
        suit = avSuit;
        attackStrength = strength;
    }

    Avenger(string avName = "", int strength = 0) {
        name = avName;
        attackStrength = strength;
    }

    Avenger (const Avenger& other) {
        name = other.name;
        attackStrength = other.attackStrength;
        suit = other.suit;
    }
    
    void attack(Avenger& enemy) {
        enemy.suit=enemy.suit-(attackStrength);
        if (enemy.suit.getDurability() < 0) enemy.suit.setDestroyed(true);
    }

    void upgradeSuit(QNS& extra) {
        suit + extra;
    }

    void repair(int x) {
        suit/x;
    }

    void printStatus() {
        cout << name << " " << suit.getPowerLevel() << " " << suit.getDurability() << " " << suit.getEnergyStorage() << " " << suit.getHeatLevel() << endl;
    }

    QNS* operator-> () { // could be used to get durabilty/heatLevel of suit
        return &suit;
    }

    string getName() {
        return name;
    }

    int getAttackStrength() {
        return attackStrength;
    }
};

class Battle {
    vector<Avenger> heroes;
    vector<Avenger> enemies;
    vector<string> battleLog;
    queue<QNS> suits;

    public:
    void BattleBegin() {
        string s;
        cin >> s;
        while (s != "End") {
        if (s == "Attack") {
            string a1, a2;
            cin >> a1 >> a2;
            Avenger& avg1 = getAvenger(a1);
            Avenger& avg2 = getAvenger(a2);
            if (avg1->getDurability() > 0 && avg1->getHeatLevel() <= 500 && avg2->getDurability() > 0) {
                string log = avg1.getName() + " attacks " + avg2.getName();
                battleLog.push_back(log);
                avg1.attack(avg2);
                if (avg2->getDurability() <= 0) {
                    log = avg2.getName() + " suit destroyed";
                    battleLog.push_back(log);
                    avg2->setDestroyed(true);
                }
                else if (avg2->getHeatLevel() > 500) {
                    log = avg2.getName() + " suit overheated";
                    battleLog.push_back(log);
                }
            } 
        }

        if (s == "BoostPowerByFactor") {
            string a;
            cin >> a;
            int factor;
            cin >> factor;
            Avenger& avg = getAvenger(a); 
            avg->boostPower(factor);
            string log = a + " boosted";
            battleLog.push_back(log);
            if (avg->getHeatLevel() > 500) {
                log = a + " suit overheated";
                battleLog.push_back(log);
            }
        }

        if (s == "BoostPower") {
            string a;
            cin >> a;
            int p, d, e, h;
            cin >> p >> d >> e >> h;
            QNS powerSuit(p, d, e, h);
            Avenger& avg = getAvenger(a); 
            avg->boostPower(powerSuit);
            string log = a + " boosted";
            battleLog.push_back(log);
            if (avg->getHeatLevel() > 500) {
                log = a + " suit overheated";
                battleLog.push_back(log);
            }
        }

        if (s == "AvengerStatus") {
            string a;
            cin >> a;
            Avenger& avg = getAvenger(a); 
            avg.printStatus();
        }

        if (s == "Upgrade") { 
            string a;
            cin >> a;
            Avenger& avg = getAvenger(a); 
            if (!isEmpty()) {                
                QNS suit = suits.front();
                suits.pop();
                avg.upgradeSuit(suit);   
                string log = avg.getName() + " upgraded";
                battleLog.push_back(log);
            }
            else {
                string log = avg.getName() + " upgrade Fail";
                battleLog.push_back(log);
            }
        }

        if (s == "Repair") {
            string a;
            cin >> a;
            int x;
            cin >> x;
            Avenger& avg = getAvenger(a); 
            avg.repair(x);
            string log = a + " repaired";
            battleLog.push_back(log);
        }

        if (s == "PrintBattleLog") {
            printBattleLog();
        }

        if (s == "BattleStatus") {
            if (Result() == 1) cout << "heroes are winning\n";
            else if (Result() == -1) cout << "enemies are winning\n";
            else cout << "tie\n"; 
        }

        cin >> s;
        }
    }

    void printBattleLog() {
        for (auto log : battleLog) {
            cout << log << endl;
        }
    }

    int Result() {
        int heroesPower = 0, enemiesPower = 0;
        for (auto avg : heroes) {
            if (!avg->getDestroyed()) {
                heroesPower += avg->getPowerLevel() + avg->getDurability();
            }
        }
        for (auto avg : enemies) {
            if (!avg->getDestroyed()) {
                enemiesPower += avg->getPowerLevel() + avg->getDurability();
            }
        }
        if (heroesPower > enemiesPower) {
            return 1;
        }
        else if (heroesPower < enemiesPower) {
            return -1;
        }
        return 0;
    }

    void addHero(Avenger& h) {
        heroes.push_back(h);
    }

    void addEnemy(Avenger& e) {
        enemies.push_back(e);
    }

    void addSuit(QNS& s) {
        suits.push(s);
    }

    bool isEmpty() {
        if (suits.empty()) return true;
        return false;
    }

    QNS& getSuit() {
        QNS& suit = suits.front();
        return suit;
    }

    void removeSuit() {
        suits.pop();
    }

    Avenger& getAvenger(string a) {
            for (auto& avenger : heroes) {
                if (a == avenger.getName()) {
                    return avenger;
                }
            }
            for (auto& avenger : enemies) {
                if (a == avenger.getName()) {
                    return avenger;
                }
            }
            throw runtime_error("Avenger not found");
    }
};

int main() {
    int k, n, m;
    cin >> k >> n >> m;
    Battle battle;
    for (int i = 0; i < k; i++) {
        int p, d, e, h;
        cin >> p >> d >> e >> h;
        QNS suit(p, d, e, h);
        battle.addSuit(suit);
    }

    for (int i = 0; i < n; i++) {
        string n;
        int as; // attackStrength
        cin >> n >> as;
        if (battle.isEmpty()) {
            cout << n << " is out of fight\n";
        }
        else {
            QNS suit = battle.getSuit();
            battle.removeSuit();
            Avenger a(n, suit, as);
            battle.addHero(a);
        }
    }

    for (int i = 0; i < m; i++) {
        string n;
        int as; // attackStrength
        cin >> n >> as;
        if (battle.isEmpty()) {
            cout << n << " is out of fight\n";
        }
        else {
            QNS suit = battle.getSuit();
            battle.removeSuit();
            Avenger a(n, suit, as);
            battle.addEnemy(a);
        }
    }

    string s;
    cin >> s;
    battle.BattleBegin();
}