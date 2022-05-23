#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	vector<int> v;

	while (true) {
		cout << "정수를 입력하세요(0을 입력하면 종료)>>";
		int input;
		cin >> input;
		if (input == 0) break;
		v.push_back(input);
		for_each(v.begin(), v.end(), [](int n) {cout << n << " "; });
		cout << endl;
		int sum = 0;
		for (auto it = v.begin(); it != v.end(); it++) {
			sum += *it;
		}
		cout << "평균 = " << sum / v.size() << endl;
	}
}