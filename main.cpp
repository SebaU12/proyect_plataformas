#include <iostream> 

//Include files.h 
#include "./node.h"
#include "./pvector.h"
#include "vertex.h"
#include "graph.h"


using namespace std; 


int main(){
  //cout << endl << "INICIANDO PROGRAMA" << endl;

  Graph graph; 
  Graph afd; 
  List<pvector*> *vector_substrings = new DoubleList<pvector*>(); 
  List<pvector*> *vector_test_substrings = new DoubleList<pvector*>(); 
  string getInputAlfabeto = ""; 
  string getInputSubString= ""; 
  string getInputTestSubString= ""; 

  int size_input = 0, size_substring = 0; 
  //cout << "Ingrese el tamaño que tendra el alfabeto: "; 
  cin >> getInputAlfabeto; 
  pvector *input_alfabeto = new pvector(getInputAlfabeto); 
  //cout << endl << "El input ingresado es el siguiente: "; 
  input_alfabeto->display(); 

  //cout << endl << "Ingresar la cantidad de substrings: "; 
  cin >> size_input; 
  for(int i = 0; i < size_input; i++){
    //cout << endl << "Ingresar el tamaño del substrings [" << i+1 << "]: "; 
    cin >> getInputSubString; 
    pvector *input_substring = new pvector(getInputSubString); 
    vector_substrings->push_back(input_substring); 
  }

 // cout << endl << "INICIO DE PROCESAR LOS STRINGS" << endl;
  //cout << endl; 
  for (int i = 0; i < vector_substrings->size(); i++){
    (*vector_substrings)[i+1]->display();
    cout << endl; 
  }
  //cout << endl << "TERMINO DE PROCESAR LOS STRINGS" << endl;


  //cout << endl << "INICIO DE PROCESAR DE CREACION GRAFO " << endl;
  graph.generateGraphOfString(input_alfabeto, vector_substrings);

  //graph.display(false);

  //cout << endl << "Ingrese la cantidad de substrings para testear: " ;
  cin >> size_input; 
  for(int i = 0; i < size_input; i++){
   // cout << endl << "Ingresar el tamaño del substrings [" << i+1 << "]: "; 
    cin >> getInputTestSubString; 
    pvector *input_substring = new pvector(getInputTestSubString); 
    vector_test_substrings->push_back(input_substring); 
  }
  //cout << endl; 
  for (int i = 0; i < vector_test_substrings->size(); i++){
    (*vector_test_substrings)[i+1]->display();
    cout << endl; 
  }

  graph.execBFS(vector_test_substrings);

 // cout << endl; 
  afd = graph.generateAFD(input_alfabeto);
 // cout << endl << endl << "ACABO EL AFD";
  afd.execBFS(vector_test_substrings);

 // cout << endl << endl << "ACABO EL PROGRAMA";
  cout << '\n'; 
  return 0; 
}




