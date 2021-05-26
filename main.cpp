
#include<stdio.h>
#include<stdlib.h>
#include <vector>

void sub(std::vector<int>& v){
	v.push_back(3);
	for (int i = 0;i<v.size();i++) {
		printf("\n%d[%d],",v[i],i);
  	}
	v.push_back(33);
	for (int i = 0;i<v.size();i++) {
		printf("\n%d[%d],",v[i],i);
  	}
}

int main() {
	std::vector<int> v;
	v.push_back(5);
	v.push_back(4);
	sub(v);
	for (int i = 0;i<v.size();i++) {
		printf("%d,",v[i]);
  	}
	return 0;
}