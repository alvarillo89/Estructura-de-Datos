#include "clinvar.h"

/** @file clinvar.cpp
  @brief Fichero de implementación de la clase clinvar.
*/

/** @brief Inserta una mutación en la estructura clinvar.
  @param x Mutación a insertar.
*/
void clinvar::insert(const mutacion & x){
  string gen;
  enfermedad e;
  pair<set<mutacion>::iterator,bool> aux;
  aux = mutDB.insert(x);

  if(aux.second){
    IDm_map.insert(pair<IDmut,set<mutacion>::iterator>(x.getID(),aux.first));

    for(int i = 0; i < x.getGenes().size(); ++i){
      gen = x.getGenes().at(i);
      // buscar si el gen ya está en el map:
      map<IDgen,list<set<mutacion>::iterator>>::iterator it = gen_map.find(gen);

      if(it != gen_map.end()) // Si ya está
        (it->second).push_back(aux.first); //Añadir la mutación a la lista.
      else{
        list<set<mutacion>::iterator> lista;
        lista.push_back(aux.first);
        gen_map.insert(pair<IDgen,list<set<mutacion>::iterator>>(gen,lista));
      }

    }

    for(int i = 0; i < x.getEnfermedades().size(); ++i){
      e = x.getEnfermedades().at(i);
      EnfDB.insert(pair<IDenf,enfermedad>(e.getID(),e));
      IDenf_map.insert(pair<IDenf,set<mutacion>::iterator>(e.getID(),aux.first));
    }
  }
}

/** @brief Inserta toda la información del archivo en la estructura Clinvar.
  @param nombreDB Nombre del archivo que contiene la información que se desea insertar.
  @note El archivo debe de tener un formato concreto.
*/
void clinvar::load(string nombreDB){
  ifstream fe;
  string cadena;

  fe.open(nombreDB.c_str(), ifstream::in);
  if (fe.fail())
    cerr << "Error al abrir el fichero " << nombreDB << endl;
  else {

     do{
       getline(fe,cadena,'\n');
     }while (cadena.find("#")==0 && !fe.eof());

     while (!fe.eof()){
         mutacion mut = mutacion(cadena);
         this->insert(mut);
         getline(fe,cadena,'\n');
     }
  }
  fe.close();
}

/** @brief Elimina una mutación de la estructura Clinvar.
  @param ID ID de la mutación que se desea eliminar.
  @return True si el borrado ha tenido éxito. False en cualquier otro caso.
*/
bool clinvar::erase(IDmut ID){
  bool borrado;
  enfermedad e;
  string gen;
  //Buscar la mutacion en el IDm_map
  unordered_map<IDmut,set<mutacion>::iterator>::iterator it = IDm_map.find(ID);

  // Si la encuentra
  if(it != IDm_map.end()){
    mutacion m = *(it->second);
    // Borrar la mutación de la lista de genes
    for(int i = 0; i < m.getGenes().size(); ++i){
        borrado = false;
        gen = m.getGenes().at(i);
        // buscar el gen en el map:
        map<IDgen,list<set<mutacion>::iterator>>::iterator it2 = gen_map.find(gen);
        // Eliminar el iterador de la lista:
        for(auto pos = (it2->second).begin(); pos != (it2->second).end() && !borrado; ++pos){
          if((*(*pos)) == m){
            (it2->second).erase(pos);
            borrado = true;
          }
        }
    }

    //Borrar de IDenf y de EnfBD:
    for(int i = 0; i < m.getEnfermedades().size(); ++i){
      borrado = false;
      e = m.getEnfermedades().at(i);
      //Contamos cuantas mutaciones tienen esa enfermedad:
      int num = IDenf_map.count(e.getID());
      // Si solo la tiene 1:
      if(num == 1){
        IDenf_map.erase(e.getID());
        EnfDB.erase(e.getID()); //Borramos también de la base de enfermedades.
      }
      else{
        //Obtenemos las mutaciones que tienen esa enfermedad asociada:
        auto res = IDenf_map.equal_range(e.getID());
        // Borramos la que tiene nuestro id:
        for(auto it3 = res.first; it3 != res.second && !borrado; ++it3){
          if( (*(it3->second)) == m){
            IDenf_map.erase(it3);
            borrado = true;
          }
        }
      }
    }
    // Borrar del map de mutaciones:
    IDm_map.erase(ID);
    //Borrar del set:
    mutDB.erase(m);
    return true;
  }// else
  return false;
}

/** @brief Busca una mutación en el conjunto Clinvar.
  @param ID ID de la mutación que se desea buscar.
  @return Iterador apuntando a la mutación. Si no se encuentra devuelve end().
  @see end
*/
clinvar::iterator clinvar::find_Mut(IDmut ID){
  clinvar::iterator sal;
  auto aux = IDm_map.find(ID);
  if(aux != IDm_map.end()){
    sal.it = aux->second;
    return sal;
  }
  //else
  return end();
}

/** @brief Busca una enfermedad en el conjunto Clinvar.
@param ID ID de la enfermedad que se desea buscar.
@return Iterador apuntando a la enfermedad. Si no se encuentra devuelve eend().
@see eend
*/
clinvar::enfermedad_iterator clinvar::find_Enf(IDenf ID){
  clinvar::enfermedad_iterator sal = EnfDB.find(ID);

  return sal;
}

/** @brief Obtiene todas las enfermedades asociadas a una mutación.
  @param mut Mutación de la cual se desea obtener sus enfermedades.
  @return Un vector que contiene todas las enfermedades. Si no hay ninguna, devuelve un vector vacío.
*/
vector<enfermedad> clinvar::getEnfermedades(mutacion & mut){
  return mut.getEnfermedades();
}

/** @brief Obtener todas las enfermedades que contienen en su nombre el "keyword".
  @param keyword Subcadena de una enfermedad o enfermedades.
  @return Una lista que contiene los IDs de las enfermedades que contienen el "keyword". Si no hay ninguna, devuelve una lista vacía.
*/
list<IDenf> clinvar::getEnfermedades(const string & keyword){
  list<IDenf> sal;
  clinvar::enfermedad_iterator it;
  for(it = this->ebegin(); it != this->eend(); ++it){
    if((it->second).nameContains(keyword))
      sal.push_back(it->first);
  }

  return sal;
}

/** @brief Obtener todas las mutaciones que contienen una determinada enfermedad.
  @param ID ID de la enfermedad de la cual se quiere consultar sus mutaciones asociadas.
  @return Un conjunto ordenado con los IDs de las mutaciones que tienen la enfermedad asociada. Si no hay ninguna, devuelve un conjunto vacío.
*/
set<IDmut> clinvar::getMutacionesEnf (IDenf ID){
  set<IDmut> sal;
  //Obetener el rango de mutaciones que tienen ese ID de enfermedad:
  auto aux = IDenf_map.equal_range(ID);
  //Añadirlas al set:
  for(auto it = aux.first; it != aux.second; ++it)
    sal.insert(it->second->getID());

  return sal;
}

/** @brief Obtener todas las mutaciones asociadas a un gen ID.
  @param ID ID del gen del cual se desea consultar sus mutaciones asociadas.
  @return Un conjunto ordenado con los IDs de las mutaciones que tienen el gen asociado. Si no hay ninguna, devuelve un conjunto vacío.
*/
set<IDmut> clinvar::getMutacionesGen (IDgen ID){
  set<IDmut> sal;
  auto it = gen_map.find(ID);
  if(it != gen_map.end()){
    for(auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2)
      sal.insert((**it2).getID());
  }

  return sal;
}

/** @brief Busca las k mutaciones más frecuentes asociadas a las enfermedades que contienen el keyword.
  @param k Número de mutaciones a buscar.
  @param keyword Subcadena del nombre de una enfermedad.
  @return Un set de mutaciones ordenado en orden decreciente de probabilidad.
*/
set<mutacion,ProbMutaciones> clinvar::topKMutaciones (int k, string keyword){
  unordered_set<IDmut> aux;
  set<mutacion,ProbMutaciones> sal;
  priority_queue<mutacion,vector<mutacion>,ProbMutaciones> cola;
  ProbMutaciones comp;
  mutacion m;
  int cont = 0;

  //Buscar los id de las enfermedades asociadas:
  list<IDenf> enfermedades = this->getEnfermedades(keyword);
  for(auto it = enfermedades.begin(); it != enfermedades.end(); ++it){
    //Buscar las mutaciones asociadas a ese id de enfermedad:
    auto mutaciones = IDenf_map.equal_range(*it);
    for(auto it2 = mutaciones.first ; it2 != mutaciones.second ; ++it2){
      m = *(it2->second);
      // Insertar las mutaciones en la cola y unordered_set
      if(aux.find(m.getID()) == aux.end()){ //Si no está ya en el cola (aux).
        if(cont < k){
          cola.push(m);
          aux.insert(m.getID());
          cont++;
        }
        else if(comp(m, cola.top())){
          aux.erase(cola.top().getID());
          cola.pop();
          cola.push(m);
          aux.insert(m.getID());
        }
      }
    }
  }

  // Construir el set:
  for(auto it = aux.begin(); it != aux.end(); ++it){
    auto mut = IDm_map.find(*it);
    sal.insert(*(mut->second));
  }
  return sal;
}

//Métodos relacionados con los iteradores:
//-----------------------------------------------------------------------------------------

/** @brief Inicio de la estructura Clinvar.
  @return Iterador apuntando a la primera mutación de la estructura.
*/
clinvar::iterator clinvar::begin(){
  clinvar::iterator it;
  it.it = mutDB.begin();
  return it;
}

/** @brief Final de la estructura Clinvar.
  @return Iterador apuntado a la posición siguiente al último elemento de la estructura.
*/
clinvar::iterator clinvar::end(){
  clinvar::iterator it;
  it.it = mutDB.end();
  return it;
}

/** @brief Devuelve la primera mutación que no es menor estricta que la dada como argumento.
  @return Iterador apuntando a dicho elemento. Si no existe devuelve end().
  @see end
*/
clinvar::iterator clinvar::lower_bound(string cromosoma, unsigned int posicion){
  mutacion m;
  clinvar::iterator sal;

  m.setChr(cromosoma);
  m.setPos(posicion);

  sal.it = mutDB.lower_bound(m);

  return sal;
}

/** @brief Devuelve la primera mutación que es mayor estricta que la dada como argumento.
  @return Iterador apuntando a dicho elemento. Si no existe devuelve end().
  @see end
*/
clinvar::iterator clinvar::upper_bound(string cromosoma, unsigned int posicion){
  mutacion m;
  clinvar::iterator sal;

  m.setChr(cromosoma);
  m.setPos(posicion);

  sal.it = mutDB.upper_bound(m);

  return sal;
}

/** @brief Inicio del conjunto de enfermedades.
  @return Iterador apuntando a la primera enfermedad de la estructura.
*/
clinvar::enfermedad_iterator clinvar::ebegin(){
  return EnfDB.begin();
}

/** @brief Final del conjunto de enfermedades.
  @return Iterador apuntando a la posición siguiente al último elemento de la estructura.
*/
clinvar::enfermedad_iterator clinvar::eend(){
  return EnfDB.end();
}

/** @brief Inicio del conjunto de genes y mutaciones.
  @return Iterador apuntando a la primera posición de la estructura.
*/
clinvar::gen_iterator clinvar::gbegin(){
  clinvar::gen_iterator it;
  it.itmap = gen_map.begin();
  it.itlist = ((gen_map.begin())->second).begin();
  it.ptrclinvar = this;
  return it;
}

/** @brief Final del conjunto de genes y mutaciones.
  @return Iterador apuntando a la posición siguiente al último elemento de la estructura.
*/
clinvar::gen_iterator clinvar::gend(){
  clinvar::gen_iterator it;
  it.itmap = gen_map.end();
  // buscamos la última lista:
  auto aux(it.itmap);
  aux--;
  it.itlist = (aux->second).end();
  it.ptrclinvar = this;
  return it;
}

// Implementación iterator:
//-----------------------------------------------------------------------------------------

/** @brief Constructor por defecto.
*/
clinvar::iterator::iterator(){ }

/** @brief Operador de asiganción.
*/
clinvar::iterator & clinvar::iterator::operator=(const clinvar::iterator & otro){
  it = otro.it;
  return *this;
}

/** @brief Operador de acceso.
  @return Una referencia constante a la mutación.
*/
const mutacion & clinvar::iterator::operator*(){
  return *it;
}

/** @brief Operador de preincremento.
*/
clinvar::iterator & clinvar::iterator::operator++(){
  ++it;
  return *this;
}

/** @brief Operador de postincremento.
*/
clinvar::iterator clinvar::iterator::operator++(int){
  clinvar::iterator aux = *this;
  ++it;
  return aux;
}

/** @brief Operador de predecremento.
*/
clinvar::iterator & clinvar::iterator::operator--(){
  --it;
  return *this;
}

/** @brief Operador de postdecremento.
*/
clinvar::iterator clinvar::iterator::operator--(int){
  clinvar::iterator aux = *this;
  --it;
  return aux;
}

/** @brief Operador de desigualdad.
  @return True si son distintos, false si no lo son.
*/
bool clinvar::iterator::operator!=(const clinvar::iterator & otro) const{
  return (it != otro.it);
}

/** @brief Operador de igualdad.
  @return True si son iguales, false si no lo son.
*/
bool clinvar::iterator::operator==(const clinvar::iterator & otro) const{
  return (it == otro.it);
}

// Implementación gen_iterator:
//------------------------------------------------------------------------------

/** @brief Constructor por defecto.
*/
clinvar::gen_iterator::gen_iterator(){ }

/** @brief Operador de asiganción.
*/
clinvar::gen_iterator & clinvar::gen_iterator::operator=(const clinvar::gen_iterator & otro){
  itmap = otro.itmap;
  itlist = otro.itlist;
  ptrclinvar = otro.ptrclinvar;
  return *this;
}

/** @brief Operador de acceso.
  @return Una referencia constante a la mutación.
*/
const mutacion & clinvar::gen_iterator::operator*(){
  return *(*itlist);
}

/** @brief Operador de preincremento.
*/
clinvar::gen_iterator & clinvar::gen_iterator::operator++(){
  ++itlist;
  if(itlist == (itmap->second).end()){
    ++itmap;
    if(itmap != (ptrclinvar->gen_map).end())
      itlist = (itmap->second).begin();
  }
  return *this;
}

/** @brief Operador de postincremento.
*/
clinvar::gen_iterator clinvar::gen_iterator::operator++(int){
  clinvar::gen_iterator aux = *this;
  ++(*this);
  return aux;
}

/** @brief Operador de desigualdad.
  @return True si son distintos, false si no lo son.
*/
bool clinvar::gen_iterator::operator!=(const clinvar::gen_iterator & otro) const{
  return (itmap != otro.itmap || itlist != otro.itlist || ptrclinvar != otro.ptrclinvar);
}

/** @brief Operador de igualdad.
  @return True si son iguales, false si no lo son.
*/
bool clinvar::gen_iterator::operator==(const clinvar::gen_iterator & otro) const{
  return (itmap == otro.itmap && itlist == otro.itlist && ptrclinvar == otro.ptrclinvar);
}
