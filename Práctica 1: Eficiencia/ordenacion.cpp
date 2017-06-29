#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <vector> 
#include <string>
#include <chrono>
#include <algorithm>


using namespace std;
using namespace std::chrono;

void burbuja(vector<string> & T, int inicial, int final)
{
 int i, j;
 string aux;
 for (i = inicial; i < final - 1; i++)
   for (j = final - 1; j > i; j--)
       if (T[j] < T[j-1])
         {
           aux = T[j];
           T[j] = T[j-1];
           T[j-1] = aux;
         }
}

void insercion(vector<string> & T, int inicial, int final)
{
	int i,j;
	string valor;
	for (i = inicial+1; i < final ; i++){
		valor = T[i];
		j=i;
		while((j>inicial)&&(T[j-1]>valor)){
			T[j] = T[j-1];
			j--;
		}
		T[j] = valor;
	}
}

void seleccion(vector<string> & T, int inicial, int final)
{
	int i, j, min;
	string t;
	for (i=inicial; i<final-1;i++){
		min = i;
		for(j = i+1; j<final; j++)
			if(T[j]<T[min])
				min = j;
		t = T[min];
		T[min] = T[i];
		T[i] = t;
	}
}

void lee_fichero( const char * nf, vector<string> & V) {
 ifstream fe;
  string s;
 fe.open(nf, ifstream::in);
 while ( !fe.eof() )
   { fe >> s;
     if (!fe.eof())
       V.push_back(s);
   }
 fe.close();
}

int main(int argc, char * argv[]) {

 vector<string> Dicc;
 vector<string> aux;
 int tmin = std::atoi(argv[1]);
 int tmax = std::atoi(argv[2]);
 int inc = std::atoi(argv[3]);
 
 
high_resolution_clock::time_point start,end;
duration<double> tiempo_transcurrido;

 int contador =0;
 
 lee_fichero("quijote.txt", Dicc); // Ojo, el vector no esta ordenado!!!
 
 int i;
 for (i = tmin; i < tmax ; i+=inc){
 	aux = Dicc;
 start = high_resolution_clock::now();
 burbuja(aux,0,i);
 end= high_resolution_clock::now(); 
 
tiempo_transcurrido  = duration_cast<duration<double> >(end - start);
cout << i << " " << tiempo_transcurrido.count() << endl;

}

}