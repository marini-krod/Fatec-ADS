#include <iostream>
using namespace std;

int main() {
    int base, dir, esquerda;

    cout << "Digite o lado A";
    cin >> base;
    cout << "Digite o Lado B";
    cin >> dir;
    cout << "Digite o lado C";
    cin >> esquerda;

    string tipo = (base == esquerda && esquerda == dir) 
    ? "equilatero" 
    : (base == esquerda || base == dir || esquerda == dir)
    ? "isoceles" 
    : "escaleno";

    cout << "O triangulo e " << tipo << endl;

    return 0;
}

