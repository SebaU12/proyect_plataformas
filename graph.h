#ifndef GRAPH_H 
#define GRAPH_H

#include <iostream> 
#include "vertex.h"
#include "double.h"
#include "doublesimp.h"
#include "pvector.h" 
#include "node.h" 

using namespace std; 

class Graph;

//////////////////////////////////////////////////////


class Graph{
private:    
    List<Vertex*> *vertexes= new DoubleList<Vertex*>(); 
    List<Vertex*> *estadoInicial= new DoubleList<Vertex*>(); 
    List<Vertex*> *estadoFinal= new DoubleList<Vertex*>(); 

    
public:
    void insertVertex(int id, string vertex);  
    void insertSState(int vertex);  
    void insertFState(int vertex);  
    bool compare_data_string(string data, string compare);
    void generateGraphOfString( pvector *alfabeto, List<pvector*> *substrings);
    void generateNewGraph(Graph &generateGraph);
    void createEdge(int id1, int id2, char w); 
    bool deleteEdge(Graph modify, int id1, int id2);
    Vertex* getVertexByData(string searchData);
    bool empty();
    void display(bool flag);
    bool execBFS(List<pvector*> *substrings);
    bool execBFS(Vertex *vert_actual, Graph& dfsgraph, 
        int index, pvector *substrings);
    Graph generateAFD(pvector *alfabeto); 
};

void Graph::insertVertex(int id, string vertex){
  Vertex *newVertex = new Vertex(vertex, id); 
  this->vertexes->push_back(newVertex);
}

void Graph::insertSState(int vertex){
  auto VertexStart = this->vertexes->find(vertex);
  this->estadoInicial->push_back(VertexStart);
}

void Graph::insertFState(int vertex){
  auto VertexEnd = this->vertexes->find(vertex);
  this->estadoFinal->push_back(VertexEnd);
}

Vertex* Graph::getVertexByData(string searchData){
  for (auto vit = this->vertexes->begin(); vit != nullptr ;vit = this->vertexes->aumentar(vit)) {
      if(vit->data->data == searchData) return vit->data;
  }
  return this->vertexes->begin()->data; 
}

void Graph::generateGraphOfString(pvector *alfabeto, List<pvector *> *substrings){

  this->insertVertex(1, "1");
  int cantVertex = 1; 
  //DEFINIENDO ESTADO INICIAL 
  this->insertSState(1);
  string nombreState; 
  
  for(int i = 0; i < substrings->size(); i++){
    for(int j = 0; j < (*substrings)[1+i]->size(); j++){
      cantVertex += 1; 
      nombreState = to_string(cantVertex); 
      //GENERANDO VERTICE POR CADA CARACTER DEL SUBSTRING 
      this->insertVertex(cantVertex, nombreState);
      //CREANDO EDGE DEL VERTICE 
      if(j == 0){
        this->createEdge(1, cantVertex, (*substrings)[1+i]->get_data(j));
      }else {
        this->createEdge(cantVertex-1, cantVertex, (*substrings)[1+i]->get_data(j));
      }
    }
    //DEFINIENDO ESTADO FINAL  
    this->insertFState(cantVertex);
  }


  //AGREGAR EL ALFABETO AL ESTADO INICIAL 
  for (int i = 0; i < alfabeto->size(); i++){
   this->createEdge(1, 1, alfabeto->get_data(i));
   //AGREGAR EL ALFABETO AL ESTADO FINAL  
   for(auto fit= this->estadoFinal->begin(); fit != nullptr; fit= this->estadoFinal->aumentar(fit)){
     this->createEdge(fit->data->id, fit->data->id, alfabeto->get_data(i));
   }
  }
}

void Graph::createEdge(int id1, int id2, char w){
  auto Vertex1 = this->vertexes->find(id1);
  auto Vertex2 = this->vertexes->find(id2);
  Edge *newEdge1 = new Edge(Vertex1, Vertex2, w); 
  Vertex1->edges->push_back(newEdge1);
}

bool Graph::deleteEdge(Graph modify, int id1, int id2){
  auto Vertex1 = modify.vertexes->find(id1);
  auto Vertex2 = modify.vertexes->find(id2);
  Vertex1->edges->pop_back();  
  return true; 
}

void Graph::display(bool flag = false){
  cout << endl; 
  cout << endl << "DISPLAY" << endl;
  cout << "ESTADO INICIAL: "; 
  for(auto vit = this->estadoInicial->begin(); vit != nullptr; vit = this->estadoInicial->aumentar(vit)){
    cout << vit->data->data; 
  }
  cout << endl << "ESTADO FINAL: "; 
  for(auto vit = this->estadoFinal->begin(); vit != nullptr; vit = this->estadoFinal->aumentar(vit)){
    cout << vit->data->data << ", "; 
  }
  cout << endl << endl; 
  int pasos = 0; 
  for (auto vit = this->vertexes->begin(); vit != nullptr ;vit = this->vertexes->aumentar(vit)) {
      cout << vit->data->data << ":  ";
      for (auto eit = vit->data->edges->begin(); eit != nullptr; eit = vit->data->edges->aumentar(eit)){
        pasos +=1; 
        if(flag == true)
        cout << "Paso [" << pasos<< "] " <<  eit->data->vertexes[1]->data << "(" << eit->data->transition << "),  "; 
        else 
        cout << eit->data->vertexes[1]->data << "(" << eit->data->transition << "),  "; 

      }
      cout << endl << endl; 
  } 
}

bool Graph::empty(){
  return this->vertexes->is_empty(); 
}

void Graph::generateNewGraph(Graph &generateGraph){
  Graph newGraph; 
  for (auto vit = this->vertexes->begin(); vit != nullptr ;vit = this->vertexes->aumentar(vit)) {
      // CREO UN BFSGRAPH CON LA DATA DEL GRADO SELECCIONADO 
      newGraph.insertVertex(vit->data->id, vit->data->data); 
  }
  newGraph.estadoInicial = this->estadoInicial; 
  newGraph.estadoFinal = this->estadoFinal; 
  generateGraph = newGraph; 
}

bool Graph::execBFS(List<pvector*> *substrings){
  cout << endl << "INICIANDO EL BFS" << endl; 
  Graph bfsgraph;

  for (int i = 0; i < substrings->size(); i++){

    this->generateNewGraph(bfsgraph); 
    pvector *cadena = (*substrings)[1+i];
    bool result = this->execBFS(this->estadoInicial->begin()->data, bfsgraph, 0, cadena); 
    cout << endl << "RESULTADO DE: '"; 
    cadena->display(); 
    cout << "' es: " << boolalpha <<  result << endl; 
    //if(result == true) bfsgraph.display(); 
  //  cout << endl << "CAMBIANDO DE SUBSTRING";
  }

  return true;
}


bool Graph::execBFS(Vertex *vert_actual, Graph& bfsgraph, 
    int index, pvector *substrings){
  bool resultado = false; 
  int sizeSubstring = substrings->size(); 

        if(index == sizeSubstring){
          // RECORRER LOS ESTADOS FINALES PARA COMPROBAR QUE ESTOY EN UN ESTADO FINAL 
          for(auto fit= this->estadoFinal->begin(); fit != nullptr; fit= this->estadoFinal->aumentar(fit)){
            // SI ENCUENTRO QUE ESTOY EN ESTADO FINAL RETORNAR TRUE 
            if(fit->data->data == vert_actual->data){
              return true; 
            }
          }
            // SI NO RETORNE ANTES ES PORQUE NO ESTOY EN UN ESTADO FINAL POR LO QUE EL CAMINO ESTA MAL 
            return false;  
        }

  //SI EL INDEX ES MAYOR AL SUBSTRING ESTA MAL, IGUAL SI EL INDEX ES NEGATIVO 
  if(index > sizeSubstring || index < 0) { 
    return  false;
  }

  //Se guarda el vector para cambiarlo cuando no se cumple la recursividad 
  auto securitycopy = vert_actual; 

  for(auto edge_vert = vert_actual->edges->begin(); edge_vert != nullptr; 
      edge_vert = vert_actual->edges->aumentar(edge_vert)){

    //RECORRER TODOS LOS EDGE POR VERTICE 

    //SI LA TRANSICION ESTA EN EL CAMINO DEL GRAFO 
      if(edge_vert->data->transition == substrings->get_data(index)){
      //SI VOY A UN VERTICE ESTE TIENE QUE TENER TRANSICIONES Y MI INDEX DEBE SER DISTINTO AL SIZE DEL SUBSTRING
      //O 
      //SI VOY A UN VERTICE QUE NO TIENE TRANSICIONES, MI INDEX SIGUIENTE DEBE SER EL ULTIMO 
        if((edge_vert->data->vertexes[1]->edges->begin() != nullptr && index != sizeSubstring)
            || 
            (edge_vert->data->vertexes[1]->edges->begin() == nullptr && index+1 == sizeSubstring)){

        // ME POSICIONO EN MI SIGUIENTE ESTADO 
        vert_actual = edge_vert->data->vertexes[1]; 

  

        //CREO UN EDGE DE MI ULTIMO ESTADO VISITADO Y EL ACTUAL 
        bfsgraph.createEdge(securitycopy->id, vert_actual->id, substrings->get_data(index));
        //AUMENTO EL INDEX PORQUE YA ESTOY EN EL ESTADO SIGUIENTE 
        index += 1;  


        // SI MI INDEX ES IGUAL A LA CANTIDAD DE SUBSTRING SIGNIFICA QUE ACABE O QUE EL CAMINO ESTA MAL 



        // GENERAR RECURSION, EN EL CASO QUE SE CUMPLA SEGUIR RETORNANDO 
        resultado = this->execBFS(vert_actual, bfsgraph, index, substrings);
        // SI NO SE CUMPLE LA RECURSION, RETROCEDER EL INDEX, BORRAR EL EDGE CREADO Y VOLVER AL VECTOR DE
        // SEGURIDAD 
        if(resultado == false) {
          index -= 1;  
          bfsgraph.deleteEdge(bfsgraph, securitycopy->id, vert_actual->id);
          vert_actual = securitycopy; 
        }
        if(resultado == true){
          return true; 
        }
      }
        }
  }
  if(resultado == false){
    return false; 
  }
  if(resultado == true){
    return true; 
  }
  return false; 
}

Graph Graph::generateAFD(pvector *alfabeto){
  Graph grafoAFD;

  List<Vertex*> *vert_visited= new DoubleList<Vertex*>(); 
  List<Vertex*> *vert_visited2 = new DoubleList<Vertex*>(); 
  List<Vertex*> *vert_compare = new DoubleList<Vertex*>(); 

  List<Vertex*> *vert_search = new DoubleList<Vertex*>(); 

  bool flag = false; 

  grafoAFD.insertVertex(1, "{1}");
  vert_visited->push_back(grafoAFD.vertexes->back());
  int cantVertex = 1; 
  cout << endl << "TRANSFORMANDO AFN A AFD" << endl; 
  //Tomando el primer vertice del AFN 
  Vertex *vertex_act = this->vertexes->begin()->data;

  bool visitadoAlfabeto = false; 
  bool visitadoVector = false; 

  string nameVertex = "";
  string nameVertex2 = "";
  string checkState = "";
  char dataAlfabeto;
 
  //CREANDO EL PRIMER VECTOR 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  for (int i = 0; i < alfabeto->size(); i++){
    visitadoAlfabeto = false; 
    dataAlfabeto = alfabeto->get_data(i);
    for (auto eit = vertex_act->edges->begin(); eit != nullptr; eit = vertex_act->edges->aumentar(eit)){
      flag = false; 
      if(dataAlfabeto == eit->data->transition){
        visitadoAlfabeto = true; 
        if(nameVertex.size() != 0) nameVertex += ","; 
        if(nameVertex == "") nameVertex += "{"; 
        nameVertex += eit->data->vertexes[1]->data;
        checkState = eit->data->vertexes[1]->data; 
      }
    }
    nameVertex += "}"; 
    for(auto vite = vert_visited->begin(); vite != nullptr ;vite = vert_visited->aumentar(vite)){
      if(vite->data->data == nameVertex){
        if(nameVertex == vert_visited->front()->data){
          grafoAFD.createEdge(1, 1, dataAlfabeto);
          for(auto vit = this->estadoFinal->begin(); vit != nullptr; vit = this->estadoFinal->aumentar(vit)){
            if(vit->data->data == checkState){
              grafoAFD.insertFState(1); 
            } 
          }
        }
        flag = true; 
      }
    }
    if(flag == false){
      cantVertex += 1; 
      grafoAFD.insertVertex(cantVertex, nameVertex); 
      grafoAFD.createEdge(1, cantVertex, dataAlfabeto);
      vert_visited->push_back(grafoAFD.vertexes->back());
      for(auto vit = this->estadoFinal->begin(); vit != nullptr; vit = this->estadoFinal->aumentar(vit)){
        if(vit->data->data == checkState){
          grafoAFD.insertFState(cantVertex); 
        } 
      }
    }
    //SI NO ENCONTRE NI UN ESTADO AGREGAR VACIO 
    if(visitadoAlfabeto == false){
    }
    nameVertex = "";
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Agregando estado inicial 
  grafoAFD.insertSState(1); 
  bool stateEnd = false; 
  
  //vert_visited->clear(); 

  if(grafoAFD.vertexes->begin()->next != nullptr){
    for(auto itsetcompare = grafoAFD.vertexes->begin(); itsetcompare != nullptr; 
        itsetcompare = grafoAFD.vertexes->aumentar(itsetcompare)){
        if(itsetcompare->data->data != grafoAFD.vertexes->begin()->data->data){
          vert_compare->push_back(itsetcompare->data);
        } 
    }
    //Agregando estados restantes 
    while(vert_compare->size() != 0){
      stateEnd = false; 
      Vertex *vert_actual = vert_compare->begin()->data; 
      for(int j = 0; j < vert_actual->data.size(); ++j){
        if(vert_actual->data[j] == '{' || vert_actual->data[j] == '}' || vert_actual->data[j] == ','){
          if(nameVertex != ""){
            //Determinar si se volvera un estado final


            /*

            */

            //Buscar los edges del vertice seleccionado 
            Vertex *vertex_add =  this->getVertexByData(nameVertex); 
            vert_search->push_back(vertex_add); 
            
            //Buscar los edges que tienen el alfabeto 

          }
          nameVertex = ""; 
        } else {
          nameVertex += vert_actual->data[j]; 
        }
      }
      for (int i = 0; i < alfabeto->size(); i++){
        flag = false; 
        nameVertex2 = ""; 
        dataAlfabeto = alfabeto->get_data(i);
        vert_visited2->clear();
        visitadoAlfabeto = false; 
        for (auto itsearch = vert_search->begin(); itsearch != nullptr ; itsearch = vert_search->aumentar(itsearch)) {
          for (auto eit = itsearch->data->edges->begin(); eit != nullptr; eit = itsearch->data->edges->aumentar(eit)){
            if(eit->data->transition == dataAlfabeto){
              //CHEQUEAR SI YA ESTA VISITADO 
              for(auto vite = vert_visited2->begin(); vite != nullptr ;vite = vert_visited2->aumentar(vite)){
                if(eit->data->vertexes[1]->data == vite->data->data){
                  flag = true; 
                }
              }
              //NO VISITADOS, VISITAR Y AGREGAR AL ESTADO 
              if(flag == false){
                if(nameVertex2.size() != 0) nameVertex2 += ","; 
                if(nameVertex2.size() == 0){
                  nameVertex2 += "{"; 
                }
                nameVertex2 += eit->data->vertexes[1]->data;
                vert_visited2->push_back(eit->data->vertexes[1]);
                visitadoAlfabeto = true; 

                for(auto vitend = this->estadoFinal->begin(); vitend != nullptr; 
                  vitend = this->estadoFinal->aumentar(vitend)){
                  if(eit->data->vertexes[1]->data == vitend->data->data){
                    stateEnd = true; 
                  }
                }

              }
            }
          }
        }
        nameVertex2 += "}"; 
        if(visitadoAlfabeto == false){
        }else{
          for(auto vite1 = vert_visited->begin(); vite1 != nullptr ;vite1 = vert_visited->aumentar(vite1)){
            visitadoVector = false; 
            if(vite1->data->data == nameVertex2){
              visitadoVector = true; 
              grafoAFD.createEdge(vert_compare->begin()->data->id, vite1->data->id, dataAlfabeto); 
              break; 
            }
            if(nameVertex2.size() == vite1->data->data.size()){
              this->compare_data_string(vite1->data->data, nameVertex2);
              if(this->compare_data_string(vite1->data->data, nameVertex2) == true){
                visitadoVector = true; 
                grafoAFD.createEdge(vert_compare->begin()->data->id, vite1->data->id, dataAlfabeto); 
                break;
              }
            }
          }
          if(visitadoVector == false){
            cantVertex += 1; 
            grafoAFD.insertVertex(cantVertex, nameVertex2);
            grafoAFD.createEdge(vert_compare->begin()->data->id, cantVertex, dataAlfabeto); 
            vert_visited->push_back(grafoAFD.vertexes->back());
            vert_compare->push_back(grafoAFD.vertexes->back());
            if(stateEnd == true){
              grafoAFD.insertFState(cantVertex);
            }
          }

        }
      }
      vert_compare->pop_front(); 
      vert_search->clear(); 
    }
  }
  return grafoAFD; 
} 

bool Graph::compare_data_string(string data, string compare){
  int size1 = 0, size2 = 0, size3 = data.size(); 
  int igual = 0; 
  bool flag = false;
  
  string n_numero1 = "";
  string n_numero2 = "";

  List<string> *numData1 = new DoubleSimp<string>(); 
  List<string> *numData2 = new DoubleSimp<string>(); 
  for(int i = 1; i < size3; i++){
    if(data[i] == ','){
      size1 += 1;
      numData1->push_back(n_numero1);
      n_numero1 = "";
    } else if(data[i] == '}'){
      numData1->push_back(n_numero1);
      size1 += 1;
    }else{
      n_numero1 += data[i];
    }

    if(compare[i] == ','){
      size2 += 1;
      numData2->push_back(n_numero2);
      n_numero2 = "";
    } else if(compare[i] == '}'){
      numData2->push_back(n_numero2);
      size2 += 1;
    }else{
      n_numero2 += compare[i];
    }
  }
  if(size2 == size1){
    for(auto it1 = numData1->begin(); it1 != nullptr ; it1= numData1->aumentar(it1)){
      for(auto it2 = numData2->begin(); it2 != nullptr ; it2 = numData2->aumentar(it2)){
        if(it1->data == it2->data){
          igual += 1; 
          if(igual == size2){
            return true; 
          }
        }
      }
    }
  }
  
  return false; 
}

#endif
