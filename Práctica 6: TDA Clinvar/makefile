CXX=g++
CXXFLAGS=-I$(INC) -g -std=c++11
INC=include/
SRC=src/
OBJ=obj/
BIN=bin/
DOC=doc/

$(BIN)prueba: $(OBJ)enfermedad.o $(OBJ)mutacion.o $(OBJ)clinvar.o $(OBJ)prueba.o
	$(CXX) -o $(BIN)prueba $(OBJ)enfermedad.o $(OBJ)mutacion.o $(OBJ)clinvar.o $(OBJ)prueba.o

$(OBJ)enfermedad.o: $(INC)enfermedad.h $(SRC)enfermedad.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJ)enfermedad.o $(SRC)enfermedad.cpp

$(OBJ)mutacion.o: $(INC)mutacion.h $(INC)enfermedad.h $(SRC)mutacion.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJ)mutacion.o $(SRC)mutacion.cpp

$(OBJ)clinvar.o: $(INC)enfermedad.h $(INC)mutacion.h $(INC)clinvar.h $(SRC)clinvar.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJ)clinvar.o $(SRC)clinvar.cpp

$(OBJ)prueba.o: $(INC)enfermedad.h $(INC)mutacion.h $(INC)clinvar.h $(SRC)prueba.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJ)prueba.o $(SRC)prueba.cpp

documentacion:
	doxygen $(DOC)doxyTDAClinvar

clean:
	echo "Limpiando ..."
	rm $(OBJ)/*.o
	rm $(BIN)/prueba

clean_doc : clean
	-rm doc/html/* doc/latex/*

