#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Unit {
    std::string name = "Unit";
    int health = 100;
    int attack = 10;
    int defense = 5;

    void display() const {
        std::cout << name << " [HP: " << health << ", ATK: " << attack << ", DEF: " << defense
                  << "]" << std::endl;
    }
};

class UnitBuilder {
   private:
    std::unique_ptr<Unit> m_unit;

   public:
    UnitBuilder() : m_unit(std::make_unique<Unit>()) {}

    UnitBuilder& setName(const std::string& name) {
        m_unit->name = name;
        return *this;
    }

    UnitBuilder& setHealth(int health) {
        m_unit->health = health;
        return *this;
    }

    UnitBuilder& setAttack(int attack) {
        m_unit->attack = attack;
        return *this;
    }

    UnitBuilder& setDefense(int defense) {
        m_unit->defense = defense;
        return *this;
    }

    Unit build() { return *m_unit; }
};

class GameObject {
   public:
    virtual ~GameObject() = default;
    virtual void display() const = 0;
    virtual int getTotalHealth() const = 0;
};

class UnitLeaf : public GameObject {
   private:
    Unit unit;

   public:
    UnitLeaf(const Unit& u) : unit(u) {}

    void display() const override {
        std::cout << "  - ";
        unit.display();
    }

    int getTotalHealth() const override { return unit.health; }

    Unit& getUnit() { return unit; }
};

class Squad : public GameObject {
   private:
    std::string name;
    std::vector<std::shared_ptr<GameObject>> units;

   public:
    Squad(const std::string& n) : name(n) {}
    void add(std::shared_ptr<GameObject> obj) { units.push_back(obj); }

    void display() const override {
        std::cout << "\n[Squad: " << name << "]" << std::endl;
        for (const auto& unit : units) {
            unit->display();
        }
        std::cout << "Total HP: " << getTotalHealth() << std::endl;
    }

    int getTotalHealth() const override {
        int total = 0;
        for (const auto& unit : units) {
            total += unit->getTotalHealth();
        }
        return total;
    }

    std::vector<std::shared_ptr<GameObject>>& getUnits() { return units; }
};

class BattleStrategy {
   public:
    virtual ~BattleStrategy() = default;

    // Template Method - defines battle algorithm
    void executeBattle(Unit& attacker, Unit& defender) {
        std::cout << "\n=== Battle Start ===" << std::endl;

        prepareBattle(attacker, defender);

        int damage = calculateDamage(attacker, defender);
        std::cout << attacker.name << " attacks " << defender.name << " for " << damage
                  << " damage!" << std::endl;

        defender.health -= damage;
        if (defender.health < 0) defender.health = 0;

        finalizeBattle(attacker, defender);

        std::cout << "=== Battle End ===\n" << std::endl;
    }

   protected:
    virtual void prepareBattle(Unit& attacker, Unit& defender) {
        std::cout << attacker.name << " vs " << defender.name << std::endl;
    }

    virtual int calculateDamage(Unit& attacker, Unit& defender) = 0;

    virtual void finalizeBattle(Unit& attacker, Unit& defender) {
        std::cout << "Defender HP remaining: " << defender.health << std::endl;
    }
};

class StandardBattle : public BattleStrategy {
   protected:
    int calculateDamage(Unit& attacker, Unit& defender) override {
        int damage = attacker.attack - defender.defense;
        return damage > 0 ? damage : 1;
    }
};

class CriticalBattle : public BattleStrategy {
   protected:
    void prepareBattle(Unit& attacker, Unit& defender) override {
        std::cout << "CRITICAL STRIKE! ";
        BattleStrategy::prepareBattle(attacker, defender);
    }

    int calculateDamage(Unit& attacker, Unit& defender) override {
        int baseDamage = attacker.attack - defender.defense;
        return (baseDamage > 0 ? baseDamage : 1) * 2;
    }
};

int main() {
    std::cout << "=== STRATEGY GAME FRAMEWORK ===\n" << std::endl;

    std::cout << "--- Creating Units (Builder Pattern) ---" << std::endl;
    Unit warrior =
        UnitBuilder().setName("Warrior").setHealth(150).setAttack(20).setDefense(10).build();

    Unit archer = UnitBuilder().setName("Archer").setHealth(80).setAttack(25).setDefense(5).build();

    Unit knight =
        UnitBuilder().setName("Knight").setHealth(200).setAttack(30).setDefense(15).build();

    warrior.display();
    archer.display();
    knight.display();

    std::cout << "\n--- Organizing Army (Composite Pattern) ---" << std::endl;

    auto squad1 = std::make_shared<Squad>("Alpha Squad");
    squad1->add(std::make_shared<UnitLeaf>(warrior));
    squad1->add(std::make_shared<UnitLeaf>(archer));

    auto squad2 = std::make_shared<Squad>("Beta Squad");
    squad2->add(std::make_shared<UnitLeaf>(knight));

    Squad army("Main Army");
    army.add(squad1);
    army.add(squad2);

    army.display();

    std::cout << "\n--- Battle System (Template Method Pattern) ---" << std::endl;

    Unit attacker = UnitBuilder().setName("Orc").setHealth(120).setAttack(18).setDefense(8).build();

    Unit defender = warrior;

    StandardBattle standardBattle;
    standardBattle.executeBattle(attacker, defender);

    CriticalBattle criticalBattle;
    criticalBattle.executeBattle(attacker, defender);

    std::cout << "Final state:" << std::endl;
    defender.display();

    return 0;
}
