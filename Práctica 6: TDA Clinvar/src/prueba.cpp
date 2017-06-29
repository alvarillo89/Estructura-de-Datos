//////////////////////////////////////////////////////////////////////////////
// Álvaro Fernández García
// Fichero de prueba de la clase ClinVar
/////////////////////////////////////////////////////////////////////////////

#include "mutacion.h"
#include "enfermedad.h"
#include "clinvar.h"

using namespace std;

int main(){
	clinvar cv;

	//Prueba load e insert
	cv.load("./datos/clinvar_20160831.vcf");
	cout << "Ya se han cargado las mutaciones..." << endl;
	cout << "-----------------------------------------------------------------" << endl;

	//Prueba iteradores:
	int contm = 0, conte = 0, contg = 0;
	for(clinvar::iterator it = cv.begin(); it != cv.end(); ++it)
		contm++;
	for(clinvar::enfermedad_iterator it = cv.ebegin(); it != cv.eend(); ++it)
		conte++;
	for(clinvar::gen_iterator it = cv.gbegin(); it != cv.gend(); ++it)
		contg++;

	cout << "Se han cargado " << contm << " mutaciones" << endl;
	cout << "Se han cargado " << conte << " enfermedades" << endl;
	cout << "Se han cargado " << contg << " mutaciones con gen" << endl;
	cout << "-----------------------------------------------------------------" << endl;

	//Prueba erase:
	cout << "Borrando la mutación con id rs115173026..." << endl;
	if(cv.erase("rs115173026"))
		cout << "Mutación borrada con éxito" << endl;

	cout << "-----------------------------------------------------------------" << endl;

	// Prueba Métodos find:
	//Mutación
	cout << "Buscar mutación con id rs115173026: " << endl;
	if(cv.find_Mut("rs115173026") == cv.end())
		cout << "La mutación no está en la estructura." << endl;
	cout << "-----------------------------------------------------------------" << endl;
	//Enfermedad
	cout << "Buscar enfermedad con id C4017378: " << endl;
	cout << (cv.find_Enf("C4017378"))->second << endl;
	cout << "-----------------------------------------------------------------" << endl;

	//Metodos get:
	list<IDenf> sal1 = cv.getEnfermedades("cancer");
	cout << "Se han encontrado " << sal1.size() << " enfermedades con la palabra cancer." << endl;
	set<IDmut> sal2 = cv.getMutacionesEnf("CN221808:616126");
	cout << "Se han encontrado " << sal2.size() << " mutaciones asociadas a la enfermedad CN221808:616126." << endl;
	set<IDmut> sal3 = cv.getMutacionesGen("AGRN:375790");
	cout << "Se han encontrado " << sal3.size() << " mutaciones asociadas al gen AGRN:375790." << endl;
	cout << "-----------------------------------------------------------------" << endl;

	//Prueba topKMutaciones:
	auto res = cv.topKMutaciones(10,"ganglia");
	cout << "Las 10 mutaciones más probables que contienen la enfermedad ganglia son :" << endl;
	for(auto it = res.begin(); it != res.end(); ++it)
		cout << *it << endl;
		cout << "-----------------------------------------------------------------" << endl;

	//Prueba upper_bound y lower_bound:
	cout << "Prueba de lower_bound de 2 y 1015000." << endl;
	cout << *(cv.lower_bound("2",1015000)) << endl;
	cout << "Prueba de upper_bound de 1 y 1014000." << endl;
	cout << *(cv.upper_bound("1",1014000)) << endl;

}
