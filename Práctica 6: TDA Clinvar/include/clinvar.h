#ifndef _CLINVAR_H
#define _CLINVAR_H

/** @file clinvar.h
  @brief Fichero de cabeceras de la clase clinvar
*/

#include <iostream>
#include <set>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include <map>
#include <list>
#include <vector>
#include "mutacion.h"
#include "enfermedad.h"

typedef string IDgen;
typedef string IDmut;
typedef string IDenf;

using namespace std;

/** @class ProbMutaciones
  @brief Functor Probabilidad Mutaciones. Ordena las mutaciones en orden decreciente de probabilidad.
*/
class ProbMutaciones{
  public:
    bool operator()(const mutacion & m1, const mutacion & m2){
      vector<float> cafnull = {1.0};
      mutacion a(m1);
      mutacion b(m2);
      //Comprobar si tiene caf:
      if(m1.getCaf().empty())
        a.setCaf(cafnull);
      if(m2.getCaf().empty())
        b.setCaf(cafnull);

      //Calcular probabilidades:
      float p1 = 1.0 - a.getCaf().at(0);
      float p2 = 1.0 - b.getCaf().at(0);
      //Si tienen la misma probabilidad se usa el ID:
      if(p1 == p2)
        return (m1.getID() != m2.getID());

      //Orden decreciente:
      return (p1 > p2);
    }
};

class clinvar{

  private:
    set<mutacion> mutDB;                                    //base de datos que contiene toda la información asociada a una mutacion
    unordered_map<IDmut,set<mutacion>::iterator> IDm_map;   // Asocia IDmutacion con mutación
    map<IDgen, list< set<mutacion>::iterator> > gen_map;    // Asocia genes con mutaciones
    map<IDenf,enfermedad> EnfDB;                            // Base de datos de enfermedades
    multimap<IDenf,set<mutacion>::iterator> IDenf_map;      // Asocia enfermedad con mutaciones
    friend class iterator;
    friend class gen_iterator;

  public:

    // Iteradores:
    /** @typedef enfermedad_iterator
      @brief Iterador sobre el conjunto de enfermedades en orden creciente de ID.
    */
    typedef map<IDenf, enfermedad>::iterator enfermedad_iterator;
    class iterator;
    class gen_iterator;

    // Métodos del conjunto:
    void load (string nombreDB);
    void insert (const mutacion & x);
    bool erase (IDmut ID);
    iterator find_Mut (IDmut ID);
    enfermedad_iterator find_Enf(IDenf ID);
    vector<enfermedad> getEnfermedades(mutacion & mut);
    list<IDenf> getEnfermedades(const string & keyword);
    set<IDmut> getMutacionesEnf (IDenf ID);
    set<IDmut> getMutacionesGen (IDgen ID);
    set<mutacion,ProbMutaciones> topKMutaciones (int k, string keyword);

    // Métodos relacionados con los iteradores
    iterator begin();
    iterator end();
    iterator lower_bound(string cromosoma, unsigned int posicion);
    iterator upper_bound(string cromosoma, unsigned int posicion);
    enfermedad_iterator ebegin();
    enfermedad_iterator eend();
    gen_iterator gbegin();
    gen_iterator gend();


  /*------------------------------------------------------------------------------------*/
  // Iteradores del conjunto:

  /** @class iterator
    @brief Iterador sobre el conjunto de mutaciones en orden creciente de char/pos.
  */
  class iterator {

  public:
      set<mutacion>::iterator it;
      friend class clinvar;

    public:
      iterator();
      iterator & operator=(const iterator & otro);
      const mutacion & operator*();
      iterator & operator++();
      iterator operator++(int);
      iterator & operator--();
      iterator operator--(int);
      bool operator!=(const iterator & otro) const;
      bool operator==(const iterator & otro) const;
  };


  /** @class gen_iterator
    @brief Iterador sobre el conjunto de mutaciones en orden creciente de gen ID.
  */
  class gen_iterator {

    private:
      map<IDgen, list< set<mutacion>::iterator>>::iterator itmap;
      list<set<mutacion>::iterator>::iterator itlist;
      clinvar *ptrclinvar;
      friend class clinvar;

    public:
      gen_iterator();
      gen_iterator & operator=(const gen_iterator & otro);
      const mutacion & operator*();
      gen_iterator & operator++();
      gen_iterator operator++(int);
      bool operator!=(const gen_iterator & otro) const;
      bool operator==(const gen_iterator & otro) const;
  };

};

#endif
