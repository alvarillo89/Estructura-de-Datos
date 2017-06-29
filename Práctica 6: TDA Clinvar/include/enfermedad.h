#ifndef __ENFERMEDAD_H
#define __ENFERMEDAD_H

/** @file enfermedad.h
  @brief Fichero de cabeceras de la clase enfermedad
*/

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>

using namespace std;

class enfermedad {
private:
  string  name;       // nombre de la enfermedad. Almacenar completo en minúscula.
  string  ID;         // ID único para la enfermedad
  string  database;   // Base de datos que provee el ID

public:
 enfermedad (); //Constructor de enfermedad por defecto
 enfermedad (const string & name, const string & ID, const string & database);

 void setName(const string & name);
 void setID(const string & ID);
 void setDatabase(const string & database);

 string getName() const;
 string getID() const;
 string getDatabase() const;

 enfermedad & operator=(const enfermedad & e);
 string toString() const;

// Operadores relacionales
 bool operator==(const enfermedad & e) const;
 bool operator!=(const enfermedad & e) const;
 bool operator<(const enfermedad & e) const;	//Orden alfabético por campo name.

 bool nameContains(const string & str) const;   //Devuelve True si str está incluido en el nombre de la enfermedad, aunque no se trate del nombre completo. No debe ser sensible a mayúsculas/minúsculas.

};

 ostream& operator<< ( ostream& os, const enfermedad & e); //imprime enfermedad (con TODOS sus campos)

#endif
