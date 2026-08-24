#include <iostream>
using namespace std;

int main() {
    float salario, imposto;

    cout << "Digite o salario anual de uma pessoa: ";
    cin >> salario;

    imposto = (salario <= 20000)
    ? 0
    : (salario <= 50000)
    ? salario * 0.10
    : salario * 0.20;

    cout << "O imposto a pagar e: R$ " << imposto << endl;

    return 0;
}

