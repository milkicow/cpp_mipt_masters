#include <iostream>
#include <string>

struct Person {
    std::string name = "";
    int age = 0;
    int grade = 0;
};

class Builder {
   private:
    std::unique_ptr<Person> m_person;

   public:
    Builder() : m_person(std::make_unique<Person>()) {}

    Builder& nameBuilder(const std::string& name) {
        m_person->name = name;
        return *this;
    }

    Builder& age(int age) {
        m_person->age = age;
        return *this;
    }

    Builder& grade(int grade) {
        m_person->grade = grade;
        return *this;
    }

    Person get() const { return *m_person; }
};

int main() {
    auto person = Builder().nameBuilder("Ivan").age(25).grade(10).get();

    std::cout << "Name: " << person.name << ", Age: " << person.age << ", Grade: " << person.grade
              << '\n';

    return 0;
}