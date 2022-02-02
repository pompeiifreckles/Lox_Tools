#include <iostream>
#include <memory>

using namespace std;

class Player {
	struct Concept {
		virtual ~Concept() {}
		virtual void printName() = 0;
	};
	
	template<typename T>
	struct Model : Concept {
		Model(const T& val) 
		: player(val) {}

		void printName() {
			player.printName();
		}

		T player;
	};

	unique_ptr<Concept> m_val;
public:
	template<typename T>
	Player(T&& val) {
		*this = val;
	}

	template<typename T>
	Player& operator=(T&& val) {
		m_val = make_unique<Model<T>>(val);
		return *this;
	}

	void printName() {
		m_val->printName();
	}
};

class Zelda {
public:
	void printName() {
		cout << "Zelda" << endl;
	}
};

class Link {
public:
	void printName() {
		cout << "Link" << endl;
	}
};

int main() {
	Player p {Link()};
	p.printName();
	p = Zelda();
	p.printName();
}
