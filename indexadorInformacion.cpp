#include "indexadorInformacion.h"

#include <iostream>
using namespace std;

/** ############################################################################################################# */
/** ####################################                         ################################################ */
/** ####################################    INFCOLECCIONDOCS     ################################################ */
/** ####################################                         ################################################ */
/** ############################################################################################################# */

/**
    Salida estandar

    Complejidad O(1)
*/
ostream& operator<<(ostream& s, const InfColeccionDocs& p){
s   << "numDocs: " << p.numDocs << '\t'
    << "numTotalPal: " << p.numTotalPal << '\t'
    << "numTotalPalSinParada: " << p.numTotalPalSinParada << '\t'
    << "numTotalPalDiferentes: " << p.numTotalPalDiferentes << '\t'
    << "tamBytes: " << p.tamBytes;
return s;
}

/**
    Operador de asignacion
*/
InfColeccionDocs & InfColeccionDocs::operator= (const InfColeccionDocs &otro){
    numDocs = otro.numDocs;
    numTotalPal = otro.numTotalPal;
    numTotalPalDiferentes = otro.numTotalPalDiferentes;
    numTotalPalSinParada = otro.numTotalPalSinParada;
    tamBytes = otro.tamBytes;
    return (*this);
}

/**
    Contructor por defecto
*/
InfColeccionDocs::InfColeccionDocs()
{
    numDocs = 0;
    numTotalPal = 0;
    numTotalPalSinParada = 0;
    numTotalPalDiferentes = 0;
    tamBytes = 0;
}

/**
    Destructor
*/
InfColeccionDocs::~InfColeccionDocs()
{
    numDocs = 0;
    numTotalPal = 0;
    numTotalPalSinParada = 0;
    numTotalPalDiferentes = 0;
    tamBytes = 0;
}

/**
    Pone todos los contadores a 0
*/
 void InfColeccionDocs::clear(){
    numDocs = 0;
    numTotalPal = 0;
    numTotalPalSinParada = 0;
    numTotalPalDiferentes = 0;
 };

/**
    Suma 1 documento a la coleccion
*/
void InfColeccionDocs::addDoc(){    numDocs++;}

/**
    Resta un documento a la coleccion
*/
void InfColeccionDocs::delDoc(){    numDocs--;}

/**
    Contea una palabra en la coleccion
    Se indica si es nueva y si es de parada como parametros

    Complejidad O(1)
*/
void InfColeccionDocs::addPal(const bool& nueva, const bool& parada){
    numTotalPal++;
    if (nueva)              numTotalPalDiferentes++;
    if (!parada)            numTotalPalSinParada++;
}

/**
    Resta el numero de palabras diferentes indicadas de la coleccion
    Complejidad O(1)
*/
void InfColeccionDocs::decPal(const int& numPalAQuitar){    numTotalPalDiferentes -= numPalAQuitar;}

/** ############################################################################################################# */
/** ####################################                         ################################################ */
/** ####################################          INFDOC         ################################################ */
/** ####################################                         ################################################ */
/** ############################################################################################################# */

/**
    Salida standar

    Complejidad O(1)
*/
ostream& operator<<(ostream& s, const InfDoc& p) {
    s << "idDoc: "              << p.idDoc << '\t';
    s << "numPal: "             << p.numPal << '\t';
    s << "numPalSinParada: "    << p.numPalSinParada << '\t';
    s << "numPalDiferentes: "   << p.numPalDiferentes << '\t';
    s << "tamBytes: "           << p.tamBytes;
return s;
}

/**
    Contructor de copia
*/
InfDoc::InfDoc(const InfDoc& otro){
    idDoc = otro.idDoc;
    numPal = otro.numPal;
    numPalSinParada = otro.numPalSinParada;
    numPalDiferentes = otro.numPalDiferentes;
    tamBytes = otro.tamBytes;
}

/**
    Contructor, asigna el codigo de documento pasado por parametro

    Complejidad O(1)
*/
InfDoc::InfDoc(const long int& id)
{
    idDoc = id;
    numPal = 0; // Nº total de palabras del documento
    numPalSinParada = 0; // Nº total de palabras sin stop-words del documento
    numPalDiferentes = 0;
    tamBytes = 0; // Tamaño en bytes del documento Fecha fechaModificacion;
}

/**
    Constructor por defecto
*/
InfDoc::InfDoc()
{
    idDoc = 0;
    numPal = 0; // Nº total de palabras del documento
    numPalSinParada = 0; // Nº total de palabras sin stop-words del documento
    numPalDiferentes = 0;
    tamBytes = 0; // Tamaño en bytes del documento Fecha fechaModificacion;
}

/**
    Destructor
*/
InfDoc::~InfDoc()
{
    idDoc = 0;
    numPal = 0; // Nº total de palabras del documento
    numPalSinParada = 0; // Nº total de palabras sin stop-words del documento
    numPalDiferentes = 0;
    tamBytes = 0; // Tamaño en bytes del documento Fecha fechaModificacion;
}

/**
    Operador de asignacion
*/
InfDoc& InfDoc::operator= (const InfDoc &otro){
    idDoc = otro.idDoc;
    numPal = otro.numPal;
    numPalSinParada = otro.numPalSinParada;
    numPalDiferentes = otro.numPalDiferentes;
    tamBytes = otro.tamBytes;
    return *this;
}

/**
    setters

    Complejidad O(1)
*/
void InfDoc::setNumPal(const int &n)            {    numPal = n;            }
void InfDoc::setNumPalDif(const int &n)         {    numPalDiferentes = n;  }
void InfDoc::setNumPalSinParada(const int &n)   {    numPalSinParada = n;   }
void InfDoc::setTamBytes(const int &n)          {    tamBytes = n;          }


/** ############################################################################################################# */
/** ##################################                             ############################################## */
/** ##################################     INFORMACIONPREGUNTA     ############################################## */
/** ##################################                             ############################################## */
/** ############################################################################################################# */

/**
    Salida standar

    Complejidad O(1)
*/
ostream& operator<<(ostream& s, const InformacionPregunta& p){
    s << "numTotalPal: " << p.numTotalPal;
    s << "\tnumTotalPalSinParada: "<< p.numTotalPalSinParada;
    s << "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes;
return s;
}

/**
    Operador de asignacion
*/
InformacionPregunta & InformacionPregunta::operator= (const InformacionPregunta &otro)
{
    numTotalPal = otro.numTotalPal;
    numTotalPalSinParada = otro.numTotalPalSinParada;
    numTotalPalDiferentes = otro.numTotalPalDiferentes;
    return (*this);
}

/**
    Constructor por defecto
*/
InformacionPregunta::InformacionPregunta(){
    numTotalPal = 0;
    numTotalPalSinParada = 0;
    numTotalPalDiferentes = 0;
}

/**
    Destructor por defecto
*/
InformacionPregunta::~InformacionPregunta(){
    numTotalPal = 0;
    numTotalPalSinParada = 0;
    numTotalPalDiferentes = 0;
}


/** ############################################################################################################# */
/** ##################################                             ############################################## */
/** ################################## INFORMACIONTERMINOPREGUNTA  ############################################## */
/** ##################################                             ############################################## */
/** ############################################################################################################# */

/**
    Constructor por defecto
*/
InformacionTerminoPregunta::InformacionTerminoPregunta(){
    ft = 0;
}

/**
    Constructor de copia
*/
InformacionTerminoPregunta::InformacionTerminoPregunta(const InformacionTerminoPregunta& otro){
    ft = otro.ft;
    posTerm = otro.posTerm;
}

/**
    destructor
*/
 InformacionTerminoPregunta::~InformacionTerminoPregunta (){
    posTerm.clear();
    ft = 0;
 }

/**
    Operador de asignacion
*/
InformacionTerminoPregunta& InformacionTerminoPregunta::operator= (const InformacionTerminoPregunta& otro){
    ft = otro.ft;
    posTerm = otro.posTerm;
    return (*this);
}

/**
    Salida standar

    Complejidad O(n) con n=numero de ocurrencias del termino
*/
ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) {
    s << "ft: " << p.ft;

    list<int>::const_iterator it;
    for (it=p.posTerm.begin();it!= p.posTerm.end();it++){
        cout << '\t' << (*it);
    }
    return s;
}

/**
    Añade una ocurrencia en la posicion pasada del termino/documento actual

    Complejidad O(1)
*/
void InformacionTerminoPregunta::add(const int& pos){
    ft++;
    posTerm.push_back(pos);
}

/**
    Escribe el actual objeto en el stream pasado por parametro en formato xml

    Complejidad O(n) con n = numero de ocurrencias del termino en el documento
*/
void InformacionTerminoPregunta::toFile(ostream &salida) const{
    salida << "  <InfTerminoPregunta>"<<endl;
    salida << "    <ft>"<<ft<<endl;
    list<int>::const_iterator it;
    for (it=posTerm.begin();it!=posTerm.end();it++){
        salida << "      <posicion>"<<(*it)<<endl;
    }
    salida << "  </InfTerminoPregunta>"<<endl;
}

/**
    Carga un objeto desde el fichero XML pasado por parametro

    Complejidad O(n) con n = Numero de lineas hasta fin de objeto .xml
*/
void InformacionTerminoPregunta::fromFile(istream &entrada) {
    string busca, linea;
        if (linea == "  <InfTerminoPregunta>")  while(linea != "  <InfTerminoPregunta>"){
            getline(entrada, linea);
            busca = "    <ft>";                   if (linea.substr(0, busca.size()) == busca)   {   ft = stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
            busca = "      <posicion>";           if (linea.substr(0, busca.size()) == busca)   {   posTerm.push_back(stoi(linea.substr(busca.size(), linea.size() - busca.size())));  }
        }
}



/** ############################################################################################################# */
/** ##################################                             ############################################## */
/** ##################################      INFORMACIONTERMINO     ############################################## */
/** ##################################                             ############################################## */
/** ############################################################################################################# */

/**
    Escribe el objeto en el ostream en formato texto

    Complejidad O(n) con n = tamaño del vector l_docs
*/
ostream& operator<<(ostream& s, const InformacionTermino& p) {
    s << "Frecuencia total: " << p.ftc << '\t';
    s << "fd: " << p.l_docs.size();

    unordered_map<long int, InfTermDoc>::const_iterator its;
    for (its=p.l_docs.begin(); its != p.l_docs.end(); its++){
        s << '\t' << "Id.Doc: " << (*its).first << '\t' << (*its).second;
    }
return s;
}

/**
    Retorna True si el termino esta indexado en el documento con id pasado
    En caso afirmativo, se utiliza el infdoc por parametro para retornar la referencia al objeto InformacionTerminoDocumento

    Complejidad O(1)
*/
bool InformacionTermino::IndexedAtDocument(const int& d, InfTermDoc& infdoc) const
{
    bool res = false;
    unordered_map<long int, InfTermDoc>::const_iterator it1;
    it1 = l_docs.find(d);
    if (it1 != l_docs.end()){
        res = true;
        infdoc = (*it1).second;
    }
    return res;
}

/**
    Retorna True si el termino esta indexado en el documento con id pasado
    En caso afirmativo, se utiliza el infdoc por parametro para retornar la referencia al objeto InformacionTerminoDocumento

    Complejidad O(1)
*/
bool InformacionTermino::IndexedAtDocument(const int& d) const
{
    bool res = false;
    unordered_map<long int, InfTermDoc>::const_iterator it1;
    it1 = l_docs.find(d);
    if (it1 != l_docs.end()){
        res = true;
    }
    return res;
}


/**
    Operador de asignacion
*/
InformacionTermino& InformacionTermino::operator= (const InformacionTermino &otro){
    ftc = otro.ftc;
    l_docs = otro.l_docs;
    return (*this);
}

/**
    Escribe el actual objeto InfTerm en el stream pasado por parametro
    en formato xml

    Complejidad O(n) con n = tamaño del vector l_docs
*/
void InformacionTermino::toFile(ostream &salida) const{
    salida << "  <InfTerm>"<<endl;
    salida << "   <ftc>"<<ftc<<endl;
    unordered_map<long int, InfTermDoc>::const_iterator it;
    for (it=l_docs.begin();it!=l_docs.end();it++){
        salida << "   <doc>"<<(*it).first<<endl;
        (*it).second.toFile(salida);
    }
    salida << "  </InfTerm>"<<endl;
};

/**
    Carga un objeto InformacionTermino desde el fichero XML pasado por parametro

    Complejidad O(n) con n = Numero de lineas hasta fin de objeto .xml
*/
void InformacionTermino::fromFile(istream &entrada){
    string busca, linea;
    long int idDoc;
        getline(entrada, linea);
        if (linea == "  <InfTerm>")  while(linea != "  </InfTerm>"){
            getline(entrada, linea);
            busca = "   <ftc>";          if (linea.substr(0, busca.size()) == busca)   {   ftc = stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
            busca = "   <doc>";          if (linea.substr(0, busca.size()) == busca)   {
              idDoc =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));
              InfTermDoc itd;
              itd.fromFile(entrada);
              l_docs[idDoc] = itd;
            }
        }
}


/**
    Constructor de copia
*/
InformacionTermino::InformacionTermino (const InformacionTermino &otro){
    ftc = otro.ftc;
    l_docs = otro.l_docs;
};

/**
    Constructor por defecto
*/
InformacionTermino::InformacionTermino(){
    ftc = 0;
}

/**
    Destructor
*/
InformacionTermino::~InformacionTermino()
{
    ftc = 0;
    l_docs.clear();
}

/**
    Añade al actual termino una referencia al documento/posicion pasados

    Complejidad O(1)
*/
void InformacionTermino::add(const long int& idDoc, const int& posicion){
    ftc++;
    unordered_map<long int, InfTermDoc>::const_iterator pos = l_docs.find(idDoc);
    if (pos==l_docs.end()){
        InfTermDoc doc;
        l_docs[idDoc] = doc;
    }

    l_docs[idDoc].add(posicion);
}

/**
    Elimina del actual termino todas las refencias a un documento
*/
void InformacionTermino::del(const long int& idDoc) {
    //Se elimina la referencia al id del documento de la coleccion
    unordered_map<long int, InfTermDoc>::const_iterator pos = l_docs.find(idDoc);
    if (pos!=l_docs.end()){
        ftc = ftc - (*pos).second.getFt();
        l_docs.erase(pos);
    }
}


/** ############################################################################################################## */
/** ##################################                              ############################################## */
/** ################################## INFORMACIONTERMINODOCUMENTO  ############################################## */
/** ##################################                              ############################################## */
/** ############################################################################################################## */

/**
    Salida estandar

    Complejidad O(n) con n= Numero de ocurrencias del termino dentro del documento
*/
ostream& operator<<(ostream& s, const InfTermDoc& p) {
    s << "ft: " << p.ft;
    list<int>::const_iterator its;
    for (its = p.posTerm.begin(); its != p.posTerm.end(); its++){
        s <<    '\t' << ((*its)-1);
    }
    return s;
}

/**
    Constructor por defecto
*/
InfTermDoc::InfTermDoc(){
    ft = 0;
    posTerm.clear();
}

/**
    Destructor
*/
InfTermDoc::~InfTermDoc(){
    ft = 0;
    posTerm.clear();
}

/**
    Escribe el actual objeto en el stream pasado por parametro en formato xml

    Complejidad O(n) con n = numero de ocurrencias del termino en el documento
*/

void InfTermDoc::toFile(ostream &salida) const{
    salida << "   <InfTermDoc>"<<endl;
    salida << "    <ft>"<<ft<<endl;
    list<int>::const_iterator it;
    for (it=posTerm.begin();it!=posTerm.end();it++){
        salida << "     <p>"<<(*it)<<endl;
    }
    salida << "   </InfTermDoc>"<<endl;
}

/**
    Carga un objeto desde el fichero XML pasado por parametro

    Complejidad O(n) con n = Numero de lineas hasta fin de objeto .xml
*/
void InfTermDoc::fromFile(istream& entrada){
    string busca, linea;
    getline(entrada, linea);
        if (linea == "   <InfTermDoc>")  while(linea != "   </InfTermDoc>"){
            getline(entrada, linea);
            busca = "    <ft>";                   if (linea.substr(0, busca.size()) == busca)   {   ft = stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
            busca = "     <p>";           if (linea.substr(0, busca.size()) == busca)   {
                posTerm.push_back(stoi(linea.substr(busca.size(), linea.size() - busca.size())));
            }
        }
}


void InfTermDoc::add(const int &posicion){
    ft++;
    posTerm.push_back(posicion);
}


InfTermDoc& InfTermDoc::operator= (const InfTermDoc &otro){
    ft = otro.ft;
    posTerm = otro.posTerm;
    return *this;
}

