#ifndef INDEXADORINFORMACION_H
#define INDEXADORINFORMACION_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>

using namespace std;

class Buscador;

class InfColeccionDocs;
class InfDoc;
class InformacionPregunta;
class InformacionTermino;
class InformacionTerminoPregunta;
class InfTermDoc;



class InfColeccionDocs
{
    friend ostream& operator<<(ostream& s, const InfColeccionDocs& p);
    public:
        friend class Buscador;
        InfColeccionDocs (const InfColeccionDocs &);
        InfColeccionDocs ();
        ~InfColeccionDocs ();
        InfColeccionDocs & operator= (const InfColeccionDocs &);
        void addDoc();
        void delDoc();
        void addPal(const bool& nueva, const bool& parada);
        void decPal(const int&);
        void clear();

        long int getNumDocs() const                     {return numDocs;}
        long int getTotalPal() const                    {return numTotalPal;}
        long int getTotalPalSinParada() const           {return numTotalPalSinParada;}
        long int getTotalPalDiferentes() const          {return numTotalPalDiferentes;}
        long int getBytes() const                       {return tamBytes;};

        void setNumDocs(const int& n)                   {numDocs = n;};
        void setTotalPal(const long int& n)             {numTotalPal = n;};
        void setTotalPalSinParada(const long int& n)    {numTotalPalSinParada= n;};
        void setTotalPalDiferentes(const long int& n)   {numTotalPalDiferentes = n;};
        void setBytes(const int& n)                     {tamBytes = n;};

    protected:
   private:
        long int numDocs; // Nº total de documentos en la colección
        long int numTotalPal;// Nº total de palabras en la colección
        long int numTotalPalSinParada;// Nº total de palabras sin stop-words en la colección
        long int numTotalPalDiferentes;// Nº total de palabras diferentes en la colección que no sean stopwords     (sin acumular la frecuencia de cada una de ellas)
        long int tamBytes; // Tamaño total en bytes de la colección

};
/**************************************************************************************************/
/**************************************************************************************************/

class InfTermDoc
{

    friend ostream& operator<<(ostream& s, const InfTermDoc& p);
public:
    friend class Buscador;
    InfTermDoc (); // Inicializa ft = 0
    ~InfTermDoc (); // Pone ft = 0
    InfTermDoc & operator= (const InfTermDoc &);
    void add(const int &posicion);
    int getFt() const {return ft;}
    void toFile(ostream &) const;
    void fromFile(istream&);
    // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
private:
    int ft; // Frecuencia del término en el documento
    list<int> posTerm;
    // Solo se almacenará esta información si el campo privado del indexador    almacenarPosTerm == true
    /** Lista de números de palabra en los que aparece el término en el
    documento. Los números de palabra comenzarán desde cero (la primera
    palabra del documento). Se numerarán las palabras de parada. Estará
    ordenada de menor a mayor posición. */
};
/**************************************************************************************************/
/**************************************************************************************************/

class InfDoc {
    friend ostream& operator<<(ostream& s, const InfDoc& p);
public:
    friend class Buscador;
    friend class IndexadorHash;

    InfDoc ();
    InfDoc (const long int&);
    InfDoc (const InfDoc &);
    InfDoc & operator= (const InfDoc &);
    ~InfDoc ();

    long int getId() const {return idDoc;};
    long int getTotalPal() const{return numPal;};
    long int getTotalPalSinParada() const {return numPalSinParada;};
    long int getTotalPalDiferentes() const {return numPalDiferentes;};
    long int getBytes() const { return tamBytes;};
    string getFechaModificacion() const {return modificado;}

    void setId(const int &n){ idDoc = n;};
    void setNumPal(const int &n);
    void setNumPalDif(const int &n);
    void setNumPalSinParada(const int &n);
    void setTamBytes(const int &n);
    void setFechaModificacion(const string &g){modificado = g;}


    //void add(const bool &nueva,const bool& parada,const int& tam) ;
// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
private:

    long int idDoc;         // Identificador del documento. El primer documento indexado en la colección será el identificador 1
    int numPal;             // Nº total de palabras del documento
    int numPalSinParada;    // Nº total de palabras sin stop-words del documento
    int numPalDiferentes;   // Nº total de palabras diferentes que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
    int tamBytes;           // Tamaño en bytes del documento Fecha fechaModificacion;
    string modificado;      // Atributo correspondiente a la fecha y hora de modificación del documento. El tipo “Fecha/hora” lo elegirá/implementará el alumno

};

/**************************************************************************************************/
/**************************************************************************************************/

class InformacionPregunta {
    friend ostream& operator<<(ostream& s, const InformacionPregunta& p);
public:
    friend class Buscador;
    InformacionPregunta (const InformacionPregunta &);
    InformacionPregunta ();
    ~InformacionPregunta ();
    InformacionPregunta & operator= (const InformacionPregunta &);

    void setNumPal(const long int& n){ numTotalPal = n;}
    void setNumPalSinParada(const long int& n){ numTotalPalSinParada = n;}
    void setNumPalDiferentes(const long int& n){ numTotalPalDiferentes = n;}
    long int getNumPal() const {return numTotalPal;}
    long int getNumPalSinParada() const {return numTotalPalSinParada;}
    long int getNumPalDiferentes() const {return numTotalPalDiferentes;}


private:
    long int numTotalPal;
    // Nº total de palabras en la pregunta
    long int numTotalPalSinParada;
    // Nº total de palabras sin stop-words en la pregunta
    long int numTotalPalDiferentes;
    // Nº total de palabras diferentes en la pregunta que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
};

/**************************************************************************************************/
/**************************************************************************************************/

class InformacionTermino {

    friend ostream& operator<<(ostream& s, const InformacionTermino& p);

public:
    friend class Buscador;
    InformacionTermino (const InformacionTermino &);
    InformacionTermino (); // Inicializa ftc = 0
    ~InformacionTermino (); // Pone ftc = 0 y vacía l_docs
    InformacionTermino & operator= (const InformacionTermino &);
    // Añadir cuantos metodos se consideren necesarios para manejar la parte privada de la clase
    void add(const long int& idDoc, const int& posicion);
    void del(const long int& idDoc);
    void print() const;
    int getFrecuencia() const {return ftc;}
    bool IndexedAtDocument(const int& d, InfTermDoc& ) const;
    bool IndexedAtDocument(const int& d) const;
    void toFile(ostream &) const;
    void fromFile(istream &salida);


private:
    int ftc; // Frecuencia total del término en la colección
    unordered_map<long int, InfTermDoc> l_docs;
    /** Tabla Hash que se accederá por el id del documento, devolviendo un
    objeto de la clase InfTermDoc que contiene toda la información de
    aparición del término en el documento */
};

/**************************************************************************************************/
/**************************************************************************************************/

class InformacionTerminoPregunta
{
    friend ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p);

    public:

        friend class Buscador;

        InformacionTerminoPregunta (const InformacionTerminoPregunta &);
        InformacionTerminoPregunta ();
        ~InformacionTerminoPregunta ();
        InformacionTerminoPregunta & operator= (const InformacionTerminoPregunta&);
        void toFile(ostream &salida) const;
        void fromFile(istream &);

        //A�ade la ocurrencia de una palabra, pasando la posicion que ocupa
        void add(const int&);
       // int getF() const {return ft;};

    private:
        int ft; // Frecuencia total del término en la pregunta

        list<int> posTerm;
        // Solo se almacenará esta información si el campo privado del indexador     almacenarPosTerm == true
        /** Lista de números de palabra en los que aparece el término en la
        pregunta. Los números de palabra comenzarán desde cero (la primera
        palabra de la pregunta). Se numerarán las palabras de parada. Estará
        ordenada de menor a mayor posición.         */

};



#endif // INDEXADORINFORMACION_H
