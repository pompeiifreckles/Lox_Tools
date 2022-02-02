#include <iostream>
#include <memory>
#include <any>
#include <vector>

using namespace std;

class Cat;
class Dog;

class AnimalVisitor {
public:
	AnimalVisitor() = default;
	static any speakDog(shared_ptr<Dog> d);
	static any speakCat(shared_ptr<Cat> c);
};

any AnimalVisitor::speakCat(shared_ptr<Cat> c) {
	cout << "Meow" << endl;
	return 9;
}
any AnimalVisitor::speakDog(shared_ptr<Dog> d) {
	cout << "Woof" << endl;
	return vector<int>{1,2,3};
}

class Animal {
public:
	virtual any speak() = 0;
};

class Cat : public Animal {
public:
	any speak() override {
		return AnimalVisitor::speakCat(make_shared<Cat>(*this));
	}
};

class Dog : public Animal, public enable_shared_from_this<Dog> {
public:
	any speak() override {
		return AnimalVisitor::speakDog(shared_from_this());
	}
};

int main() {
	// Animal *a = new Dog;
	// a->speak();
	shared_ptr<Animal> a = make_shared<Dog>();
	any result = a->speak();
	// cout << (result.type() == typeid(vector<int>) ? "yes" : "no") << endl;
}
