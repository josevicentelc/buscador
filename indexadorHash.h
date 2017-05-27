#ifndef INDEXADORHASH_H
#define INDEXADORHASH_H

#include <iostream>
#include <unordered_set>
#include "indexadorInformacion.h"
#include "tokenizador.h"

using namespace std;

/**
Clase IndexadoHash
    Crea un indice invertido de todos los terminos para una coleccion de documentos

    Autor: Jose Vicente Lozano Copa
    Universidad de Alicante    Ingenier�a informatica
    Explotacion de la Informacion
    A�o 2016
*/

class Buscador;

class IndexadorHash
{
        friend ostream& operator<<(ostream& s, const IndexadorHash& p);

    public:

        friend class Buscador;

        IndexadorHash();
        IndexadorHash(const string& fichStopWords, const string& delimitadores, const bool& detectComp, const bool& minuscSinAcentos, const string& dirIndice, const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm);
        IndexadorHash(const string& directorioIndexacion);

        IndexadorHash(const IndexadorHash&);
        ~IndexadorHash();
        IndexadorHash& operator= (const IndexadorHash&);

        bool Indexar(const string& ficheroDocumentos);
        bool IndexarDirectorio(const string& dirAIndexar);
        bool GuardarIndexacion() const;
        bool RecuperarIndexacion (const string& directorioIndexacion);


        bool IndexarPregunta(const string& preg);

        bool DevuelvePregunta(string& preg) const;
        bool DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const;
        bool DevuelvePregunta(InformacionPregunta& inf) const;
        bool Devuelve(const string& word, InformacionTermino& inf) const;
        bool Devuelve(const string& word, const string& nomDoc, InfTermDoc& InfDoc) const;

        void ImprimirIndexacion() const ;
        void ImprimirIndexacionPregunta() ;
        void ImprimirPregunta();



        // Devuelve true si word aparece como t�rmino indexado
        bool Existe(const string& word) const;

        // Devuelve true si se realiza el borrado (p.ej. si word aparece como t�rmino indexado)
        bool Borra(const string& word);

        /* Devuelve true si nomDoc est� indexado y se realiza el borrado de
        todos los t�rminos del documento y del documento en los campos privados
        "indiceDocs" e "informacionColeccionDocs" */
        bool BorraDoc(const string& nomDoc);

        bool indexaDoc(const string& nomdoc);

        // Borra todos los t�rminos de todos los �ndices
        void VaciarIndice();

        // Ser� true si word est� indexado, sustituyendo la informaci�n  almacenada por "inf"
        bool Actualiza(const string& word, const InformacionTermino& inf);

        // Ser� true si se realiza la inserci�n (p.ej. si word no estaba previamente indexado)
        //No entiendo la utilidad de esto
        bool Inserta(const string& word, const InformacionTermino& inf);

        // Devolver� el n�mero de t�rminos diferentes indexados (cardinalidad de campo privado "�ndice")
        int NumPalIndexadas() const;

        // Devolver� el n�mero de t�rminos diferentes indexados (cardinalidad de campo privado "�ndice")
        int NumDocsIndexados() const;

        // Devuelve el contenido del campo privado "ficheroStopWords"
        string DevolverFichPalParada () const;

        // Mostrar� por pantalla las palabras de parada almacenadas: una palabra por l�nea (salto de l�nea al final de cada palabra)
        void ListarPalParada() const;

        // Devolver� el n�mero de palabras de parada almacenadas
        int NumPalParada() const;

        // Devuelve los delimitadores utilizados por el tokenizador
        string DevolverDelimitadores () const;

        // Devuelve si el tokenizador analiza los casos especiales
        bool DevolverCasosEspeciales () const;

        // Devuelve si el tokenizador pasa a min�sculas y sin acentos
        bool DevolverPasarAminuscSinAcentos () const;

        // Devuelve el valor de almacenarPosTerm
        bool DevolverAlmacenarPosTerm () const;

        // Devuelve "directorioIndice" (el directorio del disco duro donde se almacenar� el �ndice)
        string DevolverDirIndice () const;

        // Devolver� el tipo de stemming realizado en la indexaci�n de acuerdo con el valor indicado en la variable privada "tipoStemmer"
        int DevolverTipoStemming () const;

        // Devolver� el valor indicado en la variable privada "almEnDisco"
        bool DevolverAlmEnDisco () const;

        // Mostrar por pantalla: cout << informacionColeccionDocs << endl;
        void ListarInfColeccDocs() const;

        // Mostrar por pantalla el contenido el contenido del campo privado "�ndice": cout << termino << '\t' << InformacionTermino << endl;
        void ListarTerminos() const;

        /* Devuelve true si nomDoc existe en la colecci�n y muestra por pantalla
        todos los t�rminos indexados del documento con nombre "nomDoc": cout <<
        termino << '\t' << InformacionTermino << endl; . Si no existe no se
        muestra nada*/
        bool ListarTerminos(const string& nomDoc) const;


        /* Mostrar por pantalla el contenido el contenido del campo privado
        "indiceDocs": cout << nomDoc << '\t' << InfDoc << endl;*/
        void ListarDocs() const;

        /* Devuelve true si nomDoc existe en la colecci�n y muestra por pantalla
        el contenido del campo privado "indiceDocs" para el documento con nombre
        "nomDoc": cout*/
        bool ListarDocs(const string& nomDoc) const;

        //InformacionPregunta& getInfPregunta(){return infPregunta;}

        //unordered_map<string, InfDoc>& getIndiceDocs() const {return indiceDocs;}

        //A partir de un ID de documento, obtiene el nombre del fichero y el objeto infdoc
        //Si la funcion retorna false, indica que el id no esta en la coleccion
        bool getDocById(const long int &, string &, InfDoc &) const;

        void setStopWords(const string &nombreFichero);


    private:

        /** Obtiene el proximo valor para Id de Documento*/
        long int siguienteIdDoc();

        /** Almacena el ultimo id de documento*/
        long int nextDoc;

        // �ndice de t�rminos indexados accesible por el t�rmino
        unordered_map<string, InformacionTermino> indice;

        // �ndice de documentos indexados accesible por el nombre del documento
        unordered_map<string, InfDoc> indiceDocs;

        // Informaci�n recogida de la colecci�n de documentos indexada*/
        InfColeccionDocs informacionColeccionDocs;

        // Pregunta indexada actualmente. Si no hay ninguna indexada, contendr�a el valor ""
        string pregunta;

        // �ndice de t�rminos indexados en una pregunta. Se almacenar� en memoria principal
        unordered_map<string, InformacionTerminoPregunta> indicePregunta;

        InformacionPregunta infPregunta;

        // Palabras de parada. Se almacenar� en memoria principal
        unordered_set<string> stopWords;

        // Nombre del fichero que contiene las palabras de parada
        string ficheroStopWords;

        // Tokenizador que se usar� en la indexaci�n
        Tokenizador tok;

        /* "directorioIndice" ser� el directorio del disco duro donde se
        almacenar� el �ndice. En caso que contenga la cadena vac�a se crear� en
        el directorio donde se ejecute el indexador*/
        string directorioIndice;

        // 0 = no se aplica stemmer: se indexa el t�rmino tal y como aparece         tokenizado
        // 1 = stemmer de Porter para espa�ol
        // 2 = stemmer de Porter para ingl�s
        // Para el stemmer de Porter se utilizar�n los archivos
        //stemmer.cpp y stemmer.h, concretamente las funciones de nombre "stemmer"
        int tipoStemmer;

        /* Si es true se almacenar� la m�nima parte de los �ndices de los
        documentos en memoria principal (solo parte de "indice", el resto en
        memoria secundaria, creando las estructuras para acceder a esta memoria
        secundaria de la forma m�s eficiente posible). En caso contrario se
        almacenar� todo el �ndice en memoria principal*/
        bool almacenarEnDisco;

        // Si es true se almacenar� la posici�n en la que aparecen los t�rminos dentro del documento en la clase InfTermDoc
        bool almacenarPosTerm;


        //Indexa el termino en la coleccion
        void insertaTermino(const string &termino, const long int &idDoc, const int &posicion);
        void insertaTerminoPregunta(const string &termino, const int &posicion);

        //Verifica si el nombre del documento pasado por parametro existe en la coleccion, en tal caso retorna su id
        //Si no existe, inserta un nuevo objeto infDoc y se lo asigna, retornando el id asignado al nuevo objeto
        long int insertaDocumento(const string &documento);

        //Recibe una lista de palabras y retorna por medio de los valores por referencia:
        //  Conteo total de palabras
        //  Conteo de palabras diferentes
        //  Conteo de palabras que no son de parada
        //  Tamanyo en Bytes
        void contadorPalabras(list<string>&, int& TotalPal, int& TotalDif, int& totalSinPar);

        /**
        Elimina del indice todos aquellos terminos que ya no estan indexados en ningun documento
        */
        void borraTerminosConFrecuenciaCero();

        /**
        Retorna ture si el termino pasado por parametro esta en la lista de stopwords
        */
        bool esStopWord(const string&) const;

        /**
        Obtiene el tamanyo en Bytes de la coleccion de documentos
        */
        long int getTamColeccion() const;

        /**
        Recibe el nombre de un fichero
            Si el fichero no existe en la coleccion,
                retorna true
            Si el fichero existe y ha sido modificado desde la ultima vez que se indexo,
                Elimina la indexacion anterior
                retorna true
            Si el fichero ya esta indexado y no ha sido modificado desde la ultima vez,
            retorna false
        */
        bool seDebeIndexar(const string &f);


        string aplicarStemmer(string& termino);

        /** Obtiene la fecha de creacion de un fichero*/
        string getFileCreation(const string&) const;

};

#endif // INDEXADORHASH_H


/**
    POR HACER
    ---------

    Almacenar en InfDoc la fecha e modificacion o Hash MD5 del fichero
    Verificar la fecha de modificacion para saber si el fichero debe ser reindexado

    Alm en disco?
    Alm Por Term?

*/
