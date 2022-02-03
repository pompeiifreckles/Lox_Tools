#include <iostream>
#include <memory>

using namespace std;

class Cat;
class Dog;

class AnimalVisitor {
public:
	AnimalVisitor() = default;
	static void speakDog(shared_ptr<Dog> d);
	static void speakCat(shared_ptr<Cat> c);
};

void AnimalVisitor::speakCat(shared_ptr<Cat> c) {
	cout << "Meow" << endl;
}

void AnimalVisitor::speakDog(shared_ptr<Dog> d) {
	cout << "Woof" << endl;
}

class Animal {
public:
	virtual void speak() = 0;
};

class Cat : public Animal {
public:
	void speak() override {
		AnimalVisitor::speakCat(make_shared<Cat>(*this));
	}
};

class Dog : public Animal, public enable_shared_from_this<Dog> {
public:
	void speak() override {
		AnimalVisitor::speakDog(shared_from_this());
	}
};

int main() {
	shared_ptr<Animal> a = make_shared<Dog>();
	a->speak();
	a = make_shared<Cat>();
	a->speak();
}
