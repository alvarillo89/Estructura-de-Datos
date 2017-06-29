
/**
 * @file Nmer.h
 * @brief TDA Nmer
 *   Representa un conjunto de Nmer subsecuencias de tamaño 1 hasta N que se pueden obtener a partir de una cadena de ADN
 * @author alumno
 * @bug Por espeficicar
 */
#ifndef __NMER_H
#define __NMER_H

#include "ktree.h"
#include <string>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <cassert>

using namespace std;


/** @brief Functor para ordenar el set de Nmer en orden creciente:
  */
class OrdenCre{
  public:
    bool operator()(const pair<string,int> & a, const pair<string,int> & b){
      if(a.second == b.second)
        return a.first < b.first;

      return a.second < b.second;
    }
};

class ordenDecre {
  public:
    bool operator()(const pair<string,int> & a, const pair<string,int> & b){
      if(a.second == b.second)
        return a.first > b.first;

      return a.second > b.second;
    }
};

class Nmer {
public:
  typedef unsigned int size_type;

  /** @brief Constructor primitivo .
      Crea un Nmer de longitud maxima 0, con el valor ('-',0) en la raíz
  */
  Nmer();

  /** @brief Constructor de copia
    */
  Nmer(const Nmer & a);

  /** @brief lectura fichero serializado
   * @param nombre_fichero fichero serializado con extension .srl
   *
   * Genera a partir del fichero el ktree que contiene todos los kmers asociados a una cadena de ADN
   *
   * La cadena original viene descrita en el fichero serializado
   */
  bool loadSerialized(const string & fichero);


  /** @brief Imprime los Nmers
   * @todo Debemos de implementar este método de forma que nos garantizemos que se imprimen todos los Nmers.
   */
   void list_Nmer() const;


  /** @brief Máxima longitud de los Nmers almacenados
   */
   unsigned int length()const;


  /** @brief Número de Nmers almacenados
   */
   size_type size() const;

   //Operador de asignacion:
   Nmer &  operator=(const Nmer & a);

   // Devuelve el Nmer (subarbol) asociado a un prefijo. Por ejemplo, si adn
   //es "ACT", devuelve el Nmer que representa todas las subcadenas que
   //empiezan por "ACT" (ACT*)
   Nmer Prefix(string adn);

   // Se devuelve un Nmer donde para cada nodo (representa una secuencia) se
   //computa la suma de las frecuencias en *this y en referencia,
   Nmer Union(const Nmer reference);

   // Devuelve true si la cadena adn está representada en el árbol.
   bool containsString(const string adn) const;

   // Devuelve true si cada nodo de *this está también representado en
   //reference, es decir, si todas las secuencias representadas en el árbol
   //de *this están también incluidas en reference. False en otro caso.
   bool included(const Nmer reference) const;

  /** @brief Construye un Nmer a partir de una cadena de ADN.
   */
   void sequenceADN(unsigned int tama, const string & adn);


  /** @brief Devuelve la lista de todas las subcadenas (no prefijo)
      que aparecen menos de threshold veces en el Nmer ordenadas en
      orden creciente de frecuencia.
    */
   set<pair<string,int>,OrdenCre > rareNmer(int threshold);


  /** @brief Devuelve la lista de las cadenas de longitud mayor posible
      (no prefijo) que aparecen más de threshold veces en el Nmer, ordenadas
      en orden decreciente de frecuencia.
    */
   set<pair<string,int>,ordenDecre > commonNmer(int threshold);


  /** @brief Devuelve el conjunto de Nmers de longitud exacta l.
    */
   set<pair<string,int>, OrdenCre > level(int l);


  /** @brief Distancia entre dos Nmer
    */
  float Distance(const Nmer & x);

private:
  // Atributos
  ktree<pair<char,int>,4> el_Nmer; // subsecuencias
  unsigned int max_long; // Mayor longitud de la cadena representada, esto es, el nivel máximo del árbol

  //Metodos privados auxiliares:
  /** @brief Método para dado un nodo, extraer su Nmer (cadena)
    */
  string construyeString(ktree<pair<char,int>,4>::const_node n) const;

  /** @brief Inserta una subcadena en el Nmer.
    */
  void insertar_cadena(const string & cadena);

  /** @brief Método auxiliar para ListNmer
    */
  void recorrido_preorden(ktree<pair<char,int>,4>::const_node n) const;

  /**@brief Metodo recursivo para rareNmer
    */
  void rareNmer(ktree<pair<char,int>,4>::node & nodo, int threshold, set<pair<string,int>, OrdenCre> & conjunto);

  /** @brief Metodo recursivo para buscar una cadena
    */
  bool containsString(ktree<pair<char,int>,4>::const_node n_act, const string adn) const;

  /** @brief Metodo recursivo para comprobar si un Nmer esta incluido en otro:
    */
  bool included(ktree<pair<char,int>,4>::const_node n_act, const Nmer reference) const;

  /** @brief Metodo recursivo para commonNmer
    */
  void commonNmer(ktree<pair<char,int>,4>::node & nodo, int threshold, set<pair<string,int>, ordenDecre> & conjunto);

  /** @brief Metodo recursivo para level
    */
  void level(ktree<pair<char,int>,4>::node & nodo, int l, set<pair<string,int>, OrdenCre> & conjunto);

  /** @brief Metodo recursivo para Union
    */
  void Union(ktree<pair<char,int>,4>::node n, ktree<pair<char,int>,4>::node n_ref, Nmer & ref);

  /** @brief metodo para construir un set ordenado:
    */
  void construyeSet(ktree<pair<char,int>,4>::const_node nodo, set<pair<string,int>, ordenDecre> & conjunto) const;

  /** @brief Functor para convertir un string en un pair<char,int>
   * se utiliza en loadSerialized
   */
  class String2Base {
   public:
    pair<char,int> operator()( const string & cad) {
	    pair<char,int> salida;
	    salida.first = cad[0];
	    salida.second = std::stoi(cad.substr(1));
	    return salida;
    }
  };

   /** @brief Functor para convertir un pair<char,int> en un string
    * Necesario para serializar un Nmer.
   */
  class Base2String {
   public:
     string operator()( const pair<char,int> & x) {
       string salida = string(1,x.first) + " " +std::to_string(x.second);
       return salida;
     }
  };
};

#endif
