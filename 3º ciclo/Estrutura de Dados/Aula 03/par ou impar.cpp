#include <iostream>

using namespace std;

int main() {
	int numero;
	
	cout << "Digite um numero ";
	cin >> numero;
	
	if(numero % 2 == 0){
		cout<<"Numero e par ";
	}else{
		cout<<"Numero e impar ";
	}
	
	return 0;
}

