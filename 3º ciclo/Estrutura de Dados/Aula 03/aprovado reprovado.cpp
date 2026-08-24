#include <iostream>

using namespace std;

int main() {
	int numero;
	
	cout << "Digite um numero ";
	cin >> numero;
	
	string paridade = (numero>=6) ? "aprovado" : "reprovado";
	cout << "o numero " << numero << " e " << paridade << endl;
	
	return 0;
}

