#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	vector<int> v;

	while (true) {
		cout << "������ �Է��ϼ���(0�� �Է��ϸ� ����)>>";
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
		cout << "��� = " << sum / v.size() << endl;
	}
}