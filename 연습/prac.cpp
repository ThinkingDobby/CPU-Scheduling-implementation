#include <iostream>
#include <iomanip>
using namespace std;

class CoffeeMachine {
	int coffee;
public:
	CoffeeMachine(int coffee) {
		this->coffee = coffee;
	}
	void show();
};

void CoffeeMachine::show() {
	cout << setw(10) << left << "Coffee";
	cout << setw(coffee) << setfill('*') << left << "" << endl;
}

int main() {
	CoffeeMachine c(3);
	c.show();
	
}