#include <iostream>

using namespace std;

int main() {
	int diasemana;
	
	cout << " Digite o numero do dia da semana (1-7): ";
	cin >> diasemana;
	
	switch(diasemana){
		case 1: 
		cout << "Domingo" << endl;
		break;
		case 2: 
		cout << "Segunda" << endl;
		break;
		case 3: 
		cout << "terça" << endl;
		break;
		case 4: 
		cout << "quarta" << endl;
		break;
		case 5: 
		cout << "quinta" << endl;
		break;
		case 6: 
		cout << "sexta" << endl;
		break;
		case 7: 
		cout << "Sabado" << endl;
		break;
		default:
		cout << "Dia invalido" << endl;
	}
	return 0;
	
}

