#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {

  // Leer archivo de entrada
  ifstream fin(argv[1]);

  // Abrir archivos de salida
  ofstream fout1("columna1.txt");
  ofstream fout2("columna2.txt");
  ofstream fout3("columna3.txt");

  // Leer filas del archivo de entrada
  string row;
  while (getline(fin, row)) {
    float a, b, c;
    sscanf(row.c_str(), "%f\t%f\t%f", &a, &b, &c);
    // Guardar valores en vectores correspondientes a cada columna
    vector<float> column1, column2, column3;
    column1.push_back(a);
    column2.push_back(b);
    column3.push_back(c);
  }

  // Guardar vectores en archivos de texto
  for (int i = 0; i < column1.size(); i++) {
    fout1 << column1[i] << endl;
    fout2 << column2[i] << endl;
    fout3 << column3[i] << endl;
  }
  

  // Cerrar archivos
  fin.close();
  fout1.close();
  fout2.close();
  fout3.close();

  return 0;
}
  }
