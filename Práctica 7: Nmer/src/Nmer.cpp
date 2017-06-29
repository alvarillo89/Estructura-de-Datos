#include "Nmer.h"

#include <fstream>

template<typename T, int K>
extern void recorrido_preorden(typename ktree<T,K>::const_node n);


Nmer::Nmer() {
  max_long = 0;
  el_Nmer = ktree<pair<char,int>,4>(pair<char,int>('-',0));

}

Nmer::Nmer(const Nmer & a){
  max_long = a.max_long;
  el_Nmer = a.el_Nmer;
}

bool Nmer::loadSerialized(const string & fichero) {
 string cadena;
 ifstream fe;
 pair<char,int> nulo('x',-1);
 Nmer::String2Base stb;

 fe.open(fichero.c_str(), ifstream::in);
 if (fe.fail()){
    cerr << "Error al abrir el fichero " << fichero << endl;
 }
 else {
    //leo la cabecera del fichero (líneas que comienzan con #)
    do{
      getline(fe,cadena,'\n');
    } while (cadena.find("#")==0 && !fe.eof());
    // leemos Nmer_length
     max_long = std::stoi(cadena);
     // leemos cadena serializada
    getline(fe,cadena,'\n');
    el_Nmer.deserialize(cadena,nulo,';',stb);
    fe.close();
    return true;
 } // else
 fe.close();
 return false;
}

string Nmer::construyeString(ktree<pair<char,int>,4>::const_node n) const{
  string sal = "";

  if(!n.null()){
    while(n != el_Nmer.root()){
      sal += (*n).first;
      n = n.parent();
    }
    reverse(sal.begin(), sal.end());
  }

  return sal;
}

void Nmer::recorrido_preorden(ktree<pair<char,int>,4>::const_node n) const{
  if(!n.null()){
    if(n != el_Nmer.root())
      cout << "(" << construyeString(n) << "," << (*n).second << ")" << endl;

    for ( ktree<pair<char,int>,4>::const_node hijo : n)
      recorrido_preorden(hijo);
  }
}

void Nmer::list_Nmer() const {
  recorrido_preorden(el_Nmer.root());
}

unsigned int Nmer::length() const {
   return max_long;
}

Nmer::size_type Nmer::size() const{
  return el_Nmer.size()-1;
}

Nmer &  Nmer::operator=(const Nmer & a){
  el_Nmer = a.el_Nmer;
  max_long = a.max_long;

  return *this;
}

Nmer Nmer::Prefix(string adn){
  Nmer sal(*this);
  int siguiente_hijo;
  Nmer descarte;
  ktree<pair<char,int>,4>::node n = sal.el_Nmer.root();
  for(int i = 0; i < adn.size(); ++i){
    for(int k = 0; k < 4; ++k){
      if(!n.k_child(k).null())
        if((*(n.k_child(k))).first != adn[i])
          sal.el_Nmer.prune_k_child(n, k, descarte.el_Nmer);
      else
        siguiente_hijo = k;
    }
    n = n.k_child(siguiente_hijo);
  }
  return sal;
}

void Nmer::Union(ktree<pair<char,int>,4>::node  n, ktree<pair<char,int>,4>::node  n_ref, Nmer & ref){
  //Si ninguno de los nodos son nulos, sumamos las referencias:
  Nmer aux;

  if(!n.null() && !n_ref.null())
    (*n).second += (*n_ref).second;

  //Lanzamos la recursividad para los hijos:
  for(int k = 0; k < 4; ++k){
    if(!n.k_child(k).null() && !n_ref.k_child(k).null())
      Union(n.k_child(k), n_ref.k_child(k), ref);
    else if(n.k_child(k).null() && !n_ref.k_child(k).null()){
      ref.el_Nmer.prune_k_child(n_ref, k, aux.el_Nmer);
      el_Nmer.insert_k_child(n, k, aux.el_Nmer);
    }
  }
}

Nmer Nmer::Union(const Nmer reference){
  Nmer aux(reference);
  Nmer copia(*this);
  ktree<pair<char,int>,4>::node raiz = copia.el_Nmer.root();
  ktree<pair<char,int>,4>::node raiz_ref = aux.el_Nmer.root();
  copia.Union(raiz, raiz_ref, aux);
  return copia;
}

bool Nmer::containsString(ktree<pair<char,int>,4>::const_node n_act, const string adn) const{
  bool sal;

  if(!n_act.null()){
    if(construyeString(n_act) == adn)
      return true;
    else{
      sal = false;
      for(auto ini = n_act.begin(); ini != n_act.end() && !sal; ++ini)
        sal = containsString(*ini, adn);
      return sal;
    }
  }
  else
    return false;

}

bool Nmer::containsString(const string adn) const {
  el_Nmer.root();
  bool res = containsString(el_Nmer.root(), adn);
  return res;
}

bool Nmer::included(ktree<pair<char,int>,4>::const_node n_act, const Nmer reference) const{
  bool continuar;
  if(!n_act.null()){
    //buscamos si en referencia está la cadena que determina el nodo n_act:
    bool res = reference.containsString(reference.el_Nmer.root(), construyeString(n_act));
    // Si no está podemos afirmar que el arbol no está incluido:
    if(!res)
      return false;
    else{  //Si sí está, podemos continuar comprobando:
      continuar = true;
      for(auto ini = n_act.begin(); ini != n_act.end() && continuar; ++ini)
        continuar = included(*ini, reference);
      return continuar;
    }
  }
  else //El nodo es null
    return true;
}

bool Nmer::included(const Nmer reference) const{
  bool sal = included(el_Nmer.root(), reference);
  return sal;
}

void Nmer::insertar_cadena(const string & cadena){
  ktree<pair<char,int>,4>::node n_act = el_Nmer.root();
  int hijo;

  for(unsigned int i = 0; i<cadena.size(); ++i){
    switch (cadena[i]) {
      case 'A': hijo = 0; break;
      case 'G': hijo = 1; break;
      case 'C': hijo = 2; break;
      case 'T': hijo = 3; break;
    }
    if(!n_act.k_child(hijo).null())
      (*(n_act.k_child(hijo))).second++;
    else
      el_Nmer.insert_k_child(n_act, hijo, pair<char,int>(cadena[i],1));

    n_act = n_act.k_child(hijo);
  }
}

void Nmer::sequenceADN(unsigned int tama, const string & adn){
  string subcadena;
  el_Nmer.assing(pair<char,int>('-',0));
  for(unsigned int i=0; i<=adn.size(); ++i){
    subcadena = adn.substr(i,tama);
    insertar_cadena(subcadena);
  }
}

void Nmer::rareNmer(ktree<pair<char,int>,4>::node & nodo, int threshold, set<pair<string,int>, OrdenCre> & conjunto){
  bool hoja;

  if(!nodo.null()){
    hoja = true;
    ktree<pair<char,int>,4>::node::child_iterator ini = nodo.begin();
    ktree<pair<char,int>,4>::node::child_iterator fin = nodo.end();
    while(ini != fin){
      rareNmer(*ini, threshold, conjunto);
      ini++;
      //Si se entra aquí es porque tiene hijos, luego no es hoja.
      hoja = false;
    }
    //Si es hoja, entonces no es prefijo, luego insertamos:
    if(hoja){
        if((*nodo).second <= threshold)
          conjunto.insert(pair<string,int>(construyeString(nodo), (*nodo).second));
    }
  }
}

set<pair<string,int>,OrdenCre> Nmer::rareNmer(int threshold){
  set<pair<string,int>,OrdenCre> sal;
  ktree<pair<char,int>,4>::node n = el_Nmer.root();
  rareNmer(n, threshold, sal);
  return sal;
}

void Nmer::commonNmer(ktree<pair<char,int>,4>::node & nodo, int threshold, set<pair<string,int>, ordenDecre> & conjunto){
  bool limite;

  if(!nodo.null()){
    limite = true;
    ktree<pair<char,int>,4>::node::child_iterator ini = nodo.begin();
    ktree<pair<char,int>,4>::node::child_iterator fin = nodo.end();
    while(ini != fin){
      if((*(*ini)).second > threshold){
        commonNmer(*ini, threshold, conjunto);
        //Si se entra aquí es porque tiene hijos con una frecuencia mayor o igual luego es prefijo.
        limite = false;
      }
      ini++;
    }
    //Insertamos:
    if(limite){
        if((*nodo).second > threshold)
          conjunto.insert(pair<string,int>(construyeString(nodo), (*nodo).second));
    }
  }
}

set<pair<string,int>,ordenDecre> Nmer::commonNmer(int threshold){
  set<pair<string,int>,ordenDecre> sal;
  ktree<pair<char,int>,4>::node n = el_Nmer.root();
  commonNmer(n, threshold, sal);
  return sal;
}

void Nmer::level(ktree<pair<char,int>,4>::node & nodo, int l, set<pair<string,int>, OrdenCre> & conjunto){
  bool continua = true;
  if(!nodo.null()){
    if(construyeString(nodo).size() == l){
      conjunto.insert(pair<string,int>(construyeString(nodo), (*nodo).second));
      //Una vez introducidos los del nivel ya no se sigue con los hijos, que tendrán un nivel más:
      continua = false;
    }
    if(continua)
      for ( ktree<pair<char,int>,4>::node hijo : nodo)
        level(hijo, l, conjunto);

  }
}

set<pair<string,int>, OrdenCre > Nmer::level(int l){
  set<pair<string,int>,OrdenCre> sal;
  ktree<pair<char,int>,4>::node n = el_Nmer.root();
  level(n, l, sal);
  return sal;
}

void Nmer::construyeSet(ktree<pair<char,int>,4>::const_node nodo, set<pair<string,int>, ordenDecre> & conjunto) const{
  if(!nodo.null()){
    if(nodo != el_Nmer.root())
      conjunto.insert(pair<string,int>(construyeString(nodo), (*nodo).second));

    for ( ktree<pair<char,int>,4>::const_node hijo : nodo)
      construyeSet(hijo, conjunto);
  }
}

float Nmer::Distance(const Nmer & x){
  //Construir los sets:
  set<pair<string,int>, ordenDecre> setReceptor;
  construyeSet(el_Nmer.root(), setReceptor);
  set<pair<string,int>, ordenDecre> setX;
  x.construyeSet(x.el_Nmer.root(), setX);

  //Sustituir las frecuencias por el ranking.
  //Para ello utilizaremos algo más eficiente que el set, que almancene el Nmer y la posición.
  unordered_map<string,int> rankingX;
  unordered_map<string,int> rankingY;
  int k = 1;
  for(auto it = setReceptor.begin(); it != setReceptor.end(); ++it){
    auto aux = *it;
    aux.second = k;
    k++;
    rankingX.insert(aux);
  }

  k = 1;
  for(auto it = setX.begin(); it != setX.end(); ++it){
    auto aux = *it;
    aux.second = k;
    k++;
    rankingY.insert(aux);
  }

  //Elaborar la distancia:
  int max, posX, posY;
  float valor, dist = 0.0;
  (rankingX.size() > rankingY.size())? max = rankingX.size() : max = rankingY.size();

  //Para cada Nmer de rankingX
  for(auto it = rankingX.begin(); it != rankingX.end(); ++it){
    auto aux = rankingY.find(it->first);
    //Si el Nmer de X está en Y:
    if(aux != rankingY.end()){
      posX = it->second;
      posY = aux->second;
      valor = abs(posX - posY) / max;
    }
    else
      valor = 1;
    dist += valor;
  }
  return dist/rankingX.size();
}
