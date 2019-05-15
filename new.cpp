#include <iostream>
#include <vector>

using namespace std;

void go(vector<int> a) {
	for (auto x: a) {
		for(auto y: a) {
			cout << x << " " << y << endl;
		}
	}
}

int main () {
	vector<int> a; 
	a.push_back(1);
	a.push_back(2);
	go(a);
}


