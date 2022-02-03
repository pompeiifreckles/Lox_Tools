#include <iostream>
#include <memory>

using namespace std;

template<class T> class Beignet;
template<class T> class Cruller;

template<class T>
class PastryVisitor {
	public:
	virtual T visitBeignet(shared_ptr<Beignet<T>> beignet) = 0;
	virtual T visitCruller(shared_ptr<Cruller<T>> cruller) = 0;
	virtual ~PastryVisitor() = default;
};

template<class T>
class Pastry {
	public:
	virtual T accept(shared_ptr<PastryVisitor<T>> visitor) = 0;
	virtual ~Pastry() = default;
};

template<class T>
class Beignet : public Pastry<T>, public enable_shared_from_this<Beignet<T>> {
	
	public:
	T accept(shared_ptr<PastryVisitor<T>> visitor) override {
		return visitor->visitBeignet( this->shared_from_this() );
	}
};

template<class T>
class Cruller : public Pastry<T>, public enable_shared_from_this<Pastry<T>> {
	
	public:
	T accept(shared_ptr<PastryVisitor<T>> visitor) override {
		return visitor->visitCruller( this->shared_from_this() );
	}
};

class PastryPrinter : public PastryVisitor<string>, public enable_shared_from_this<PastryPrinter> {

	public:
	string print(shared_ptr<Pastry<string>> pastry) {
		return pastry->accept( shared_from_this() );
	}

	string visitBeignet(shared_ptr<Beignet<string>> beignet) override {
		return "Beignet";
	}

	string visitCruller(shared_ptr<Cruller<string>> cruller) override {
		return "Cruller";
	}

};

int main() {
	auto pastry = make_shared<PastryPrinter>();
	shared_ptr<Pastry<string>> chocolateBeignet = make_shared<Beignet<string>>();
	cout << pastry->print(chocolateBeignet) << endl;
}
