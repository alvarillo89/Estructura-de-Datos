#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <set>
#include "ktree.h"
#include "Nmer.h"

using namespace std;

//Función auxiliar para leer una cadena desde un fichero sin formato srl:
string leeCadena(const string & fichero){
  fstream f;
  string sal;
  f.open(fichero);
  getline(f,sal,'\n');
  for(unsigned int i = 0; i < sal.size(); ++i)
    sal[i] = toupper(sal[i]);
  return sal;
}

int main(){

  /**********************************************************/
  /******************** PRUEBAS NMER ************************/
  /**************** SELECCIONAR UNA PRUEBA ******************/
  /**********************************************************/

  Nmer prueba, copia;
  /*---------------------------------------------------------* /

  //Operador de asignacion:
  copia.loadSerialized("datos/cadenamusSimple.srl");
  prueba = copia;
  prueba.list_Nmer();

  /*---------------------------------------------------------* /

  //List Nmer:
  prueba.loadSerialized("datos/cadenamusSimple.srl");
  prueba.list_Nmer();
  cout << prueba.size() << " " << prueba.length() << endl;

  /*---------------------------------------------------------* /

  //Sequence ADN:
  prueba.sequenceADN(6, "ATACATCATTGTGG");
  prueba.list_Nmer();

  /*---------------------------------------------------------* /

  //Rare Nmer
  prueba.sequenceADN(6, "ATACATCATTGTGG");
  set<pair<string,int>,OrdenCre> res = prueba.rareNmer(4);
  for(auto it = res.begin(); it != res.end(); ++it)
    cout << "(" << it->first << "," << it->second << ")" << endl;

  /*---------------------------------------------------------* /

  //Prefix
  prueba.sequenceADN(6, "ATACATCATTGTGG");
  copia = prueba.Prefix("CAT");
  copia.list_Nmer();

  /*---------------------------------------------------------* /

  //containsString
  prueba.sequenceADN(6, "ATACATCATTGTGG");
  bool encontrado1 = prueba.containsString("ATACA");
  bool encontrado2 = prueba.containsString("CADENAQUENOESTA");
  if(encontrado1)
    cout << "Primera cadena encontrada" << endl;
  if(encontrado2)
    cout << "Segunda cadena encontrada" << endl;

  /*---------------------------------------------------------* /

  //Prueba included
  prueba.sequenceADN(6, "ATACATCATTGTGG");
  copia.sequenceADN(6, "CATCAT");
  bool res = copia.included(prueba);
  if(res)
    cout << "El arbol está contenido" << endl;

  /*---------------------------------------------------------* /

  //commonNmer
  prueba.sequenceADN(6, "ATACATCATTGTGG");
  set<pair<string,int>,ordenDecre> res = prueba.commonNmer(2);
  for(auto it = res.begin(); it != res.end(); ++it)
    cout << "(" << it->first << "," << it->second << ")" << endl;

  /*---------------------------------------------------------* /

  //level
  prueba.sequenceADN(6, "ATACATCATTGTGG");
  set<pair<string,int>,OrdenCre> res = prueba.level(6);
  for(auto it = res.begin(); it != res.end(); ++it)
    cout << "(" << it->first << "," << it->second << ")" << endl;

  /*---------------------------------------------------------* /

  //union:
  prueba.sequenceADN(6, "CAT");
  copia.sequenceADN(6, "CTAC");
  Nmer sal = prueba.Union(copia);
  prueba.list_Nmer();
  cout << "----------------------------------" << endl;
  sal.list_Nmer();

  /*---------------------------------------------------------* /

  //Prueba distance:
  prueba.sequenceADN(6, "ATACATCATTGTGG");
  copia.sequenceADN(6, "CATCAT");
  cout << prueba.Distance(copia) << endl;


  /*---------------------------------------------------------*/

  /**********************************************************/
  /**************** CÁLCULO DE LAS DISTANCIAS ***************/
  /**********************************************************/

  //Declaramos los Nmers:
  string cad;
  vector<Nmer*> nmers;
  vector<float> distancias;
  Nmer n1, n2, n3, n4, n5, n6, n7, n8, n9, n10;

  cout << "Construyendo los Nmers..." << endl;

  //Construimos los Nmers:
  cad = leeCadena("datos/1.txt");
  n1.sequenceADN(20,cad);
  nmers.push_back(&n1);
  cad = leeCadena("datos/2.txt");
  n2.sequenceADN(20,cad);
  nmers.push_back(&n2);
  cad = leeCadena("datos/3.txt");
  n3.sequenceADN(20,cad);
  nmers.push_back(&n3);
  cad = leeCadena("datos/4.txt");
  n4.sequenceADN(20,cad);
  nmers.push_back(&n4);
  cad = leeCadena("datos/5.txt");
  n5.sequenceADN(20,cad);
  nmers.push_back(&n5);
  cad = leeCadena("datos/6.txt");
  n6.sequenceADN(20,cad);
  nmers.push_back(&n6);
  cad = leeCadena("datos/7.txt");
  n7.sequenceADN(20,cad);
  nmers.push_back(&n7);
  cad = leeCadena("datos/8.txt");
  n8.sequenceADN(20,cad);
  nmers.push_back(&n8);
  cad = leeCadena("datos/9.txt");
  n9.sequenceADN(20,cad);
  nmers.push_back(&n9);
  cad = leeCadena("datos/10.txt");
  n10.sequenceADN(20,cad);
  nmers.push_back(&n10);

  cout << "Calculando distancias..." << endl;

  for(unsigned int i = 0; i < nmers.size(); ++i){
    for(unsigned int j = 0; j < nmers.size(); ++j){
      if(nmers.at(i) != nmers.at(j))
        distancias.push_back((*(nmers.at(i))).Distance(*(nmers.at(j))));
    }
  }

  cout << "Algunas de las distancias son:" << endl;
  for(unsigned int i = 0; i < 15; ++i)
    cout << distancias.at(i) << endl;

  ////////////////////////////////////
  //SALIDA:
  // Construyendo los Nmers...
  // Calculando distancias...
  // Algunas de las distancias son:
  // 0.927586
  // 0.930894
  // 0.937534
  // 0.941825
  // 0.944249
  // 0.934635
  // 0.93253
  // 0.935133
  // 0.941877
  // 0.928297
  // 0.926383
  // 0.928518
  // 0.931883
  // 0.935137
  // 0.933687
  ////////////////////////////////////

  return 0;
}
