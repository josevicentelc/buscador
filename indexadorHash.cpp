/**
Clase IndexadoHash
    Crea un indice invertido de todos los terminos para una coleccion de documentos

    Autor: Jose Vicente Lozano Copa
    Universidad de Alicante    IngenierÌa informatica
    Explotacion de la Informacion
    AÒo 2016
*/

#include "indexadorHash.h"
#include <fstream>
#include <sys/stat.h>
#include "stemmer.h"

/**
    Recibe un termino para borrar del indice
    Si el termino existe, se borra y retorna true
    Si no existe, se retorna false
*/
bool IndexadorHash::Borra(const string& word)
{
    unordered_map<string, InformacionTermino>::iterator it;
    it = indice.find(word);
    if (it == indice.end())
        return false;
    else
        indice.erase(it);
        return true;
};


bool IndexadorHash::ListarTerminos(const string& nomDoc) const
{
    bool res = false;
    int idDoc = 0;

        unordered_map<string, InfDoc>::const_iterator it;
        it = indiceDocs.find(nomDoc);

        if (it==indiceDocs.end())
            return false;
        else
            idDoc = (*it).second.getId();

        unordered_map<string, InformacionTermino>::const_iterator it2;
        for (it2=indice.begin();it2!=indice.end(); it2++){
            if ((*it2).second.IndexedAtDocument(idDoc)){
                cout << (*it2).first << "\t"<< (*it2).second << endl;
            }
        }
    return true;
};

/**
Salida estandard
*/
ostream& operator<<(ostream& s, const IndexadorHash& p) {
        s << "Fichero con el listado de palabras de parada: " << p.ficheroStopWords << endl;
        s << "Tokenizador: " << p.tok << endl;
        s << "Directorio donde se almacenara el indice generado: " << p.directorioIndice << endl;
        s << "Stemmer utilizado: " << p.tipoStemmer << endl;
        s << "Informacion de la coleccion indexada: " << p.informacionColeccionDocs << endl;
        s << "Se almacenara parte del indice en disco duro: " << p.almacenarEnDisco << endl;
        s << "Se almacenaran las posiciones de los terminos: " << p.almacenarPosTerm;
        return s;
    }

/**
    Recibe el nombre del fichero de palabras de parada y las carga en la variable stopwords
    Si el fichero no existe, muestra un error y no hace nada
*/
void IndexadorHash::setStopWords(const string &nombreFichero){
    ifstream input;
    ficheroStopWords = nombreFichero;
    input.open(nombreFichero.c_str());
    if (!input)
    {
        cerr << "Error, no existe el fichero " << nombreFichero <<  endl;
    }
    else
    {
        //cout << "Cargado fichero de StopWords" << endl;
        stopWords.clear();
        string palabra;

        while(!input.eof()){
            getline(input, palabra);
            if (palabra.size() > 0){
                stopWords.insert(palabra);
            }
        }
    }
    input.close();
}

/** Constructor con parametros*/
IndexadorHash::IndexadorHash(   const string& fichStopWords,
                                const string& delimitadores,
                                const bool& detectComp,
                                const bool& minuscSinAcentos,
                                const string& dirIndice,
                                const int& tStemmer,
                                const bool& almEnDisco,
                                const bool& almPosTerm)
{
    ficheroStopWords = fichStopWords;
    setStopWords(ficheroStopWords);
    tok = Tokenizador(delimitadores, detectComp, minuscSinAcentos);
    tipoStemmer = tStemmer;
    almacenarEnDisco = almEnDisco;
    almacenarPosTerm = almPosTerm;
    directorioIndice = dirIndice;
    nextDoc = 0;
};

/**
Constructor a partir de un indice anterior almacenado en disco
*/
IndexadorHash::IndexadorHash(const string& directorioIndexacion){
    tok = Tokenizador();
    tipoStemmer = 1;
    almacenarEnDisco = true;
    almacenarPosTerm = true;
    nextDoc = 0;
    directorioIndice = directorioIndexacion;
    RecuperarIndexacion(directorioIndexacion);
};

/**
    Constructor de copia
*/
IndexadorHash::IndexadorHash(const IndexadorHash& otro){
    tok = otro.tok;
    pregunta = otro.pregunta;
    ficheroStopWords = otro.ficheroStopWords;
    directorioIndice = otro.directorioIndice;
    tipoStemmer = otro.tipoStemmer;
    almacenarEnDisco = otro.almacenarEnDisco;
    almacenarPosTerm = otro.almacenarPosTerm;
    indice = otro.indice;
    indiceDocs = otro.indiceDocs;
    indicePregunta = otro.indicePregunta;
    informacionColeccionDocs = otro.informacionColeccionDocs;
    infPregunta = otro.infPregunta;
    nextDoc = otro.nextDoc;
    pregunta = otro.pregunta;
};

/**
    Constructor por defecto
*/
IndexadorHash::IndexadorHash(){
    tok = Tokenizador();
    tipoStemmer = 1;
    almacenarEnDisco = true;
    almacenarPosTerm = true;
    nextDoc = 0;
};

/**
    Destrutor
*/
IndexadorHash::~IndexadorHash(){
    infPregunta.~InformacionPregunta();
    informacionColeccionDocs.~InfColeccionDocs();
    indice.clear();
    indiceDocs.clear();
    indicePregunta.clear();
};

/**
    Operador de asignacion
*/
IndexadorHash& IndexadorHash::operator= (const IndexadorHash& otro){
    tok = otro.tok;
    pregunta = otro.pregunta;
    ficheroStopWords = otro.ficheroStopWords;
    directorioIndice = otro.directorioIndice;
    tipoStemmer = otro.tipoStemmer;
    almacenarEnDisco = otro.almacenarEnDisco;
    almacenarPosTerm = otro.almacenarPosTerm;
    indice = otro.indice;
    indiceDocs = otro.indiceDocs;
    indicePregunta = otro.indicePregunta;
    informacionColeccionDocs = otro.informacionColeccionDocs;
    infPregunta = otro.infPregunta;
    nextDoc = otro.nextDoc;
    return (*this);
};

/**
    Recibe un fichero conteniendo un listado de ficheros de texto y los indexa
*/
bool IndexadorHash::Indexar(const string& ficheroDocumentos)
{
    bool res = true;
    ifstream listaDocs;
    listaDocs.open(ficheroDocumentos.c_str());
    if (!listaDocs)    {
        cerr << "Error, no existe el fichero " << ficheroDocumentos << endl;
        res = false;
    }
    else
    {

        string fichero;
        while(!listaDocs.eof()){
            getline(listaDocs, fichero);
            if ( fichero.size() > 0){
                indexaDoc(fichero);
            }
        }
    }
    listaDocs.close();
    informacionColeccionDocs.setBytes(getTamColeccion());

    return res;
};

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
 bool IndexadorHash::seDebeIndexar(const string &f){

        unordered_map<string, InfDoc>::iterator itdoc;
        itdoc = indiceDocs.find(f);
        if (itdoc == indiceDocs.end())
            return true;
        else{
                /** Aqui debo verificar si el fichero ha sido modificado
                En caso de modificacion, borro el documento y retorno true
                Si no hay modificacion, retorno false
                */
                //TODO VERIFICAR MODIFICACION DEL FICHERO
                const char* file = f.c_str();
                if (getFileCreation(file) != (*itdoc).second.getFechaModificacion())
                    return true;

                //cout << "Fichero indexado y no modificado "<<(*itdoc).second.getFechaModificacion()<<endl;
            return false;
        }
 }



bool IndexadorHash::getDocById(const long int &id, string &nombre, InfDoc &informacionDoc) const{
    bool res = false;
    unordered_map<string, InfDoc>::const_iterator it;

    for(it=indiceDocs.begin(); it != indiceDocs.end() && !res; it++){
        if ( (*it).second.idDoc ==  id ){
            res = true;
            nombre = (*it).first;
            informacionDoc = (*it).second;
        }
    }
    return res;
}


/**
Anyade a la coleccion el fichero pasado por parametro
*/

bool IndexadorHash::indexaDoc(const string& fichero){

    //Primero verifico si el fichero debe ser indexado o si ya esta en la coleccion
    if (!seDebeIndexar(fichero)) return false;

    ifstream documento;

    //OBtengo el tama√±o del fichero en Bytes
    long int tamBytes = 0;

    if (fichero.substr(fichero.size()-4, 4) != ".txt" && fichero.substr(fichero.size()-4, 4) != ".tim"){
        //cerr << "Solo puedo indexa ficheros de Texto .TXT"<<endl;
        return false;
    }

    documento.open(fichero);
    if (!documento.is_open()) return false;

    string auxiliar;
    tamBytes = 0;
    while (!documento.eof()){
        getline(documento, auxiliar);
        if (!documento.eof()) auxiliar += '\n';
        tamBytes += auxiliar.size();
    }
    documento.close();

    //Tokenizo el fichero y obtengo los terminos en TOKENS
    list<string> tokens = tok.TokenizaFichero(fichero);

    //Inserto si procede el documento en el indice de documentos (Clase infDoc en indiceDocs) y obtengo de vuelta el id asignado a dicho documento
    long int idDoc = insertaDocumento(fichero);

    // Conteo de palabras del documento para el correspondiente infDoc
        int numPal, NumPalDif,NumPalSinParada;
        contadorPalabras(tokens, numPal, NumPalDif, NumPalSinParada);
        indiceDocs[fichero].setNumPal(numPal);
        indiceDocs[fichero].setNumPalDif(NumPalDif);
        indiceDocs[fichero].setNumPalSinParada(NumPalSinParada);
        indiceDocs[fichero].setTamBytes(tamBytes);
        indiceDocs[fichero].setFechaModificacion(getFileCreation(fichero));
    // --------------------------------

    // Recorro la lista de palabras y las indexo
        int pos = 0;
        list<string>::iterator itS;
        for(itS= tokens.begin();itS!= tokens.end();itS++){
            pos++;
            string b;
            b = (*itS);
            b = aplicarStemmer(b);
            insertaTermino(b, idDoc, pos); // Indexo el "termino" "documento" "Posicion"
        }
    // -----------------------------------------
    return true;
}

/**
Recorre el indice de documentos y suma los bytes de todos ellos
O(n) con N = numero de documentos indexados
*/
long int IndexadorHash::getTamColeccion() const {
    long int tam = 0;
    unordered_map<string, InfDoc>::const_iterator it;
    for (it=indiceDocs.begin(); it!= indiceDocs.end();it++){
        tam += (*it).second.getBytes();
    }
    return tam;
}

/**
    Inserta un termino en el indice de palabras indexadas
    O(1)
*/
void IndexadorHash::insertaTermino(const string &termino, const long int &idDoc, const int &posicion){
    bool esnueva = false;
    bool esparada = false;
    int tamanyo = 0;

    esparada = esStopWord(termino);

    /**Si es Stopword, no se indexa*/
    if (!esparada){
        unordered_map<string, InformacionTermino>::const_iterator got = indice.find(termino);

        /**Si la palabra no estaba indexada, se a√±ade un nuevo InformacionTermino para ella*/
        if (got==indice.end()){
            InformacionTermino a;
            Inserta(termino, a);
            esnueva = true;
        }else{
            esnueva = false;
        }

        /** Se inserta en el informacionTermino correspondiente la posicion de la palabra en el documento*/
        indice[termino].add(idDoc, posicion);
    }
    /**Se le indica al ibjeto infColeccionDocs que se a√±ade una palabra indicando si es nueva y si es de parada, para que lleve el conteo*/
    informacionColeccionDocs.addPal(esnueva, esparada);
}


/** Me dice si una palabra esta en la lista de palabras de parada*/
bool IndexadorHash::esStopWord(const string& word) const{
    bool res = false;
    unordered_set<string>::const_iterator it;
    it = stopWords.find(word);
    if (it != stopWords.end()){
        res = true;
    }
    return res;
}


/**
A partir de una lista de palabras, hace un recuento de:
    Total de palabras
    Total de palabras excluyendo las que son StopWord
    Numero de palabras diferentes
*/
void IndexadorHash::contadorPalabras(list<string>& lista, int& TotalPal, int& TotalDif, int& totalSinPar){

    unordered_map<string, int> palabras;
    unordered_map<string, int>::iterator itp;
    list<string>::iterator its;

    TotalPal = 0;
    totalSinPar = 0;

    /*recorro la lista de palabras*/
    for (its=lista.begin(); its!=lista.end(); its++){

        TotalPal++;

        if (!esStopWord((*its))){
            totalSinPar++;

            itp = palabras.find((*its));
            if (itp==palabras.end()){ //Es un nuevo termino, lo inserto
                palabras[(*its)] = 1;
            }
            else{   //Termino conocido, le sumo 1
                palabras[(*its)]++;
            }
        }
    }
    TotalDif = palabras.size();
    palabras.clear();
}


/**
    Inserta un documento en la coleccion y retorna el id de documento asignado
    Si el documento ya existia, no lo inserta, en su lugar recupera el id que tenia anteriormente
    O(1)
*/
long int IndexadorHash::insertaDocumento(const string &documento){
    long int res;
    res = 0;
    unordered_map<string, InfDoc>::const_iterator got = indiceDocs.find(documento);
    if (got==indiceDocs.end()){
        res = siguienteIdDoc();
        InfDoc Id(res);
        indiceDocs[documento] = Id;
        informacionColeccionDocs.addDoc();
    }
    else
    {
        res = indiceDocs[documento].getId();
    }

    return res;
}

/**
    Retorna el proximo codigo para un nuevo documento
*/
long int IndexadorHash::siguienteIdDoc(){
    nextDoc++;
    return nextDoc;
}

/**
    Indexa todos los ficheros de un directorio
    O(1)
*/
bool IndexadorHash::IndexarDirectorio(const string& dirAIndexar)
{
    struct stat dir;
    int err=stat(dirAIndexar.c_str(), &dir);
    //int err=stat(dirAIndexar.c_str(), &dir);
    if(err==-1 || !S_ISDIR(dir.st_mode)){
        return false;
    } else {
        // Hago una lista en un fichero con find>fich
        string cmd="find "+dirAIndexar+" -follow | sort > .lista_fich";
        system(cmd.c_str());

        return Indexar(".lista_fich");
    }
};


/**
    Recupera el indice del fichero XML
    O(n) n=Longitud del fichero xml
*/
bool IndexadorHash::RecuperarIndexacion (const string& directorioIndexacion)
{
    ifstream entrada;
    entrada.open(directorioIndexacion.c_str());

    //Si el fichero no existe, salgo y retorno un false
    if (!entrada){        return false;    }

    string linea, busca;

    while (!entrada.eof()){
        getline(entrada, linea);

//**********************************************************************************************************
        if (linea == "<indexadorHash>")  while(linea != "</indexadorHash>"){
            getline(entrada, linea);
            busca = "  <stemmer>"; if (linea.substr(0, busca.size()) == busca)   {   tipoStemmer =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
            busca = "  <nextDoc>"; if (linea.substr(0, busca.size()) == busca)   {   nextDoc =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
            busca = "  <almacenarEnDisco>"; if (linea.substr(0, busca.size()) == busca)   {   almacenarEnDisco =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
            busca = "  <almacenarPosTerm>"; if (linea.substr(0, busca.size()) == busca)   {   almacenarPosTerm =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
            busca = "  <ficheroStopWords>"; if (linea.substr(0, busca.size()) == busca)   {   setStopWords(linea.substr(busca.size(), linea.size() - busca.size()));  }
            busca = "  <pregunta>";         if (linea.substr(0, busca.size()) == busca)   {   pregunta =  linea.substr(busca.size(), linea.size() - busca.size());  }
                }
//**********************************************************************************************************
        if (linea == "<tokenizador>"){
            string delimitadoresPalabra;
            bool pasarAminusculasSinAcentos = true;
            bool casosEspeciales = true;

            while(linea != "</tokenizador>"){
            getline(entrada, linea);
            busca = "  <delimitadoresPalabra>";         if (linea.substr(0, busca.size()) == busca)   {   delimitadoresPalabra =  linea.substr(busca.size(), linea.size() - busca.size());  }
            busca = "  <pasarAminusculasSinAcentos>";   if (linea.substr(0, busca.size()) == busca)   {   pasarAminusculasSinAcentos =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
            busca = "  <casosEspeciales>";              if (linea.substr(0, busca.size()) == busca)   {   casosEspeciales =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
            }

            tok = Tokenizador(delimitadoresPalabra, casosEspeciales, pasarAminusculasSinAcentos);
        }
//**********************************************************************************************************
        if (linea == "<InfDoc>"){
            string delimitadoresPalabra;
            bool pasarAminusculasSinAcentos = true;
            bool casosEspeciales = true;
            while(linea != "</InfDoc>"){
            getline(entrada, linea);
                if (linea.substr(0, 7) == "  <doc>")
                {
                        string nomdoc; long int idDoc; long int numPal; long int numpalSinParada; long int numPalDif; long int tamBytes;
                        nomdoc = linea.substr(7, linea.size() - 7);
                        while (linea != "  </doc>"){
                            getline(entrada, linea);
                            busca = "    <idDoc>";   if (linea.substr(0, busca.size()) == busca)   {   idDoc =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
                            busca = "    <nTP>";     if (linea.substr(0, busca.size()) == busca)   {   numPal =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
                            busca = "    <nTPSP>";   if (linea.substr(0, busca.size()) == busca)   {   numpalSinParada =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
                            busca = "    <nTPD>";    if (linea.substr(0, busca.size()) == busca)   {   numPalDif =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
                            busca = "    <tB>";      if (linea.substr(0, busca.size()) == busca)   {   tamBytes =  stoi(linea.substr(busca.size(), linea.size() - busca.size()));  }
                        }
                        InfDoc doc;
                        doc.setId(idDoc);     doc.setNumPal(numPal);    doc.setNumPalSinParada(numpalSinParada);     doc.setNumPalDif(numPalDif);     doc.setTamBytes(tamBytes);
                        indiceDocs[nomdoc] = doc;
                }
            }
        }
//**********************************************************************************************************
        if (linea == "<InfColeccionDocs>")  while(linea != "</InfColeccionDocs>"){
            getline(entrada, linea);
            busca = "  <numDocs>";              if (linea.substr(0, busca.size()) == busca)   {   informacionColeccionDocs.setNumDocs(stoi(linea.substr(busca.size(), linea.size() - busca.size())));  }
            busca = "  <numTotalPal>";          if (linea.substr(0, busca.size()) == busca)   {   informacionColeccionDocs.setTotalPal(stoi(linea.substr(busca.size(), linea.size() - busca.size())));  }
            busca = "  <numTotalPalSinParada>"; if (linea.substr(0, busca.size()) == busca)   {   informacionColeccionDocs.setTotalPalSinParada(stoi(linea.substr(busca.size(), linea.size() - busca.size())));  }
            busca = "  <numTotalPalDiferentes>";if (linea.substr(0, busca.size()) == busca)   {   informacionColeccionDocs.setTotalPalDiferentes(stoi(linea.substr(busca.size(), linea.size() - busca.size())));  }
            busca = "  <tamBytes>";             if (linea.substr(0, busca.size()) == busca)   {   informacionColeccionDocs.setBytes(stoi(linea.substr(busca.size(), linea.size() - busca.size())));  }
        }
//**********************************************************************************************************
        if (linea == "<InformacionTerminoPregunta>")  while(linea != "</InformacionTerminoPregunta>"){
            getline(entrada, linea);
            busca = "  <termino>";
                if (linea.substr(0, busca.size()) == busca)   {
                string termino = linea.substr(busca.size(), linea.size() - busca.size());
                InformacionTerminoPregunta itp;
                itp.fromFile(entrada);
                indicePregunta[termino] = itp;
            }
        }
//**********************************************************************************************************
        if (linea == "<InformacionTermino>")  while(linea != "</InformacionTermino>"){
            getline(entrada, linea);
            busca = "  <termino>";
                if (linea.substr(0, busca.size()) == busca)   {
                string termino = linea.substr(busca.size(), linea.size() - busca.size());
                //cout << "recuperado el termino -> "<< termino << endl;
                InformacionTermino it;
                it.fromFile(entrada);
                indice[termino] = it;
            }
        }
//**********************************************************************************************************
    }
    return true;
};


/**
    Guarda el indice en un formato parecido a XML
*/
bool IndexadorHash::GuardarIndexacion() const
{
    /** Cossas a guardar*/

    ofstream salida;
    salida.open(directorioIndice);

        salida << "<indexadorHash>"<< endl;
            salida << "  <stemmer>"<< tipoStemmer<<endl;
            salida << "  <nextDoc>"<< nextDoc<<endl;
            salida << "  <almacenarEnDisco>"<< almacenarEnDisco<<endl;
            salida << "  <pregunta>"<< pregunta<<endl;
            salida << "  <almacenarPosTerm>"<< almacenarPosTerm<<endl;
            salida << "  <ficheroStopWords>"<< ficheroStopWords<<endl;
        salida << "</indexadorHash>"<< endl;

        salida << "<tokenizador>"<<endl;
            salida << "  <delimitadoresPalabra>"<<tok.getDelimitadores() <<endl;
            salida << "  <pasarAminusculasSinAcentos>"<< tok.PasarAminuscSinAcentos()<<endl;
            salida << "  <casosEspeciales>"<<tok.CasosEspeciales() <<endl;
        salida << "</tokenizador>"<<endl;

        salida << "<InfDoc>"<<endl;
        unordered_map<string, InfDoc>::const_iterator it_infdoc;
        for (it_infdoc=indiceDocs.begin(); it_infdoc != indiceDocs.end();it_infdoc++){
            salida << "  <doc>"<< (*it_infdoc).first<<endl;
            salida << "    <idDoc>"<< (*it_infdoc).second.getId()<<endl;
            salida << "    <nTP>"<< (*it_infdoc).second.getTotalPal()<<endl;
            salida << "    <nTPSP>"<< (*it_infdoc).second.getTotalPalSinParada()<<endl;
            salida << "    <nTPD>"<< (*it_infdoc).second.getTotalPalDiferentes()<<endl;
            salida << "    <tB>"<< (*it_infdoc).second.getBytes()<<endl;
            salida << "  </doc>"<<endl;
        }
        salida << "</InfDoc>"<<endl;

        salida << "<InfColeccionDocs>"<<endl;
            salida << "  <numDocs>"             <<informacionColeccionDocs.getNumDocs()<<endl;
            salida << "  <numTotalPal>"         <<informacionColeccionDocs.getTotalPal()<<endl;
            salida << "  <numTotalPalSinParada>"<<informacionColeccionDocs.getTotalPalSinParada()<<endl;
            salida << "  <numTotalPalDiferentes>"<<informacionColeccionDocs.getTotalPalDiferentes()<<endl;
            salida << "  <tamBytes>"            <<informacionColeccionDocs.getBytes()<<endl;
        salida << "</InfColeccionDocs>"<<endl;


        salida << "<InfPregunta>"<<endl;
            salida << "   <numTotalPal>" << infPregunta.getNumPal()<<endl;
            salida << "   <numTotalPalSinParada>" << infPregunta.getNumPalSinParada()<<endl;
            salida << "   <numTotalPalDiferentes>" << infPregunta.getNumPalDiferentes()<<endl;
        salida << "</InfPregunta>"<<endl;

    //InformacionTerminoPregunta
    salida << "<InformacionTerminoPregunta>"<<endl;
        unordered_map<string, InformacionTerminoPregunta>::const_iterator it_itp;
        for (it_itp=indicePregunta.begin();it_itp!=indicePregunta.end();it_itp++){
            salida << "  <termino>"<<(*it_itp).first<<endl;
            (*it_itp).second.toFile(salida);
        }
    salida << "</InformacionTerminoPregunta>"<<endl;


    salida << "<InformacionTermino>"<<endl;
    unordered_map<string, InformacionTermino>::const_iterator it_it;
    for (it_it = indice.begin();it_it!=indice.end();it_it++){
        salida << "  <termino>"<<(*it_it).first<<endl;
        (*it_it).second.toFile(salida);
    }
    salida << "</InformacionTermino>"<<endl;

    bool res = salida.good();
    salida.close();
    return res;
};

/**
    Retorna la informacion del termino si es que esta indexado
    En caso de no existir, retorna false
*/
bool IndexadorHash::Devuelve(const string& word, InformacionTermino& inf) const
{
    unordered_map<string, InformacionTermino>::const_iterator it;
    it = indice.find(word);
    if (it != indice.end())
    {
        inf = (*it).second;
        return true;
    }
    else
        return false;
};

int IndexadorHash::NumDocsIndexados() const{
    return indiceDocs.size();
}

/**
    Retorna el objeto InformacionTerminoDocumento a partir del documento y terminos pasados por parametro *
    En caso de no existir alguno de ellos o no tener relacion entre si, se retorna false
*/
bool IndexadorHash::Devuelve(const string& word, const string& nomDoc, InfTermDoc& infdoc) const
{
    bool res = false;
    unordered_map<string, InfDoc>::const_iterator it1;
    unordered_map<string, InformacionTermino>::const_iterator it2;

    //obtengo el id del documento
    it1 = indiceDocs.find(nomDoc);
    if (it1 != indiceDocs.end()){
        int idDoc = (*it1).second.getId();

        //Miro que la palabra exista
        it2 = indice.find(word);
        if (it2 != indice.end())

            //Ahora verifico si la palabra esta indexada en el documentote quiero dar un mordisquito
            if ((*it2).second.IndexedAtDocument(idDoc, infdoc)){
                res = true;
            }
    }
    return res;
};

/** Retorna true si el termino pasado se encuentra indexado en la coleccion*/
bool IndexadorHash::Existe(const string& word) const
{
    unordered_map<string, InformacionTermino>::const_iterator it;
    it = indice.find(word);
    if (it != indice.end())
        return true;
    else
        return false;
};

/** Elimina el documento pasado por parametro de la indexacion*/
bool IndexadorHash::BorraDoc(const string& nomDoc)
{
    //Primero miro si el nombre del documento existe en la coleccion y en tal caso obtengo el iterador
    unordered_map<string, InfDoc>::const_iterator it;
    it = indiceDocs.find(nomDoc);

    //Si existe
    if (it!=indiceDocs.end()){
        //Obtengo el identificador
        int numdoc = (*it).second.getId();

        //Actualizo el tamanyo en bytes de la coleccion total, restandole el tamanyo del fichero que voy a eliminar
        informacionColeccionDocs.delDoc();
        informacionColeccionDocs.setBytes(informacionColeccionDocs.getBytes() - (*it).second.getBytes());
        informacionColeccionDocs.setTotalPal( informacionColeccionDocs.getTotalPal() - (*it).second.getTotalPal());
        informacionColeccionDocs.setTotalPalSinParada( informacionColeccionDocs.getTotalPalSinParada() - (*it).second.getTotalPalSinParada());

        //Recorro todos los "InformacionTermino" y les doy la orden de borrar la informacion sobre el codigo de documento
        unordered_map<string, InformacionTermino>::iterator its;
        for (its=indice.begin();its!=indice.end();its++){
            (*its).second.del(numdoc);
        }

        //Despues elimino el objeto de la coleccion
        indiceDocs.erase(it);

        //Ahora hago un barrido buscando terminos que ya no estan indexados en ningun documento porque eran unicos del fichero que se ha suprimido y los elimino
        borraTerminosConFrecuenciaCero();

        return true;
    }
    else
    {
        return false;
    }
};


bool IndexadorHash::Inserta(const string& word, const InformacionTermino& inf)
{
    bool res = false;
    unordered_map<string, InformacionTermino>::const_iterator iter;
    iter = indice.find(word);

    if (iter == indice.end()){
        InformacionTermino it;
        indice[word] = it;
        res = true;
    }
    return res;
};


void IndexadorHash::ListarPalParada() const{
    unordered_set<string>::const_iterator it;
    for (it=stopWords.begin();it!=stopWords.end();it++){
        cout << (*it) << endl;
    }
};


void IndexadorHash::VaciarIndice(){
    indiceDocs.clear();
    indice.clear();
    informacionColeccionDocs.clear();
};


bool IndexadorHash::Actualiza(const string& word, const InformacionTermino& inf)
{
    if (Existe(word)){
        indice[word] = inf;
        return true;
    }
    else
        return false;
};

string IndexadorHash::DevolverDelimitadores () const{           return tok.getDelimitadores();              };
string IndexadorHash::DevolverFichPalParada () const{           return ficheroStopWords;                    };
string IndexadorHash::DevolverDirIndice () const{               return directorioIndice;                    };
int IndexadorHash::NumPalIndexadas() const{                     return indice.size();                       };
int IndexadorHash::NumPalParada() const{                        return stopWords.size();                    };
int IndexadorHash::DevolverTipoStemming () const{               return tipoStemmer;                         };
bool IndexadorHash::DevolverCasosEspeciales () const{           return tok.CasosEspeciales();               };
bool IndexadorHash::DevolverPasarAminuscSinAcentos () const{    return tok.PasarAminuscSinAcentos();        };
bool IndexadorHash::DevolverAlmacenarPosTerm () const{          return almacenarPosTerm;                    };
bool IndexadorHash::DevolverAlmEnDisco () const{                return almacenarEnDisco;                    };
void IndexadorHash::ListarInfColeccDocs() const{                cout << informacionColeccionDocs << endl;   };




bool IndexadorHash::IndexarPregunta(const string& preg)
{
    if (preg == "") return false;

    //Almaceno la pregunta
    pregunta = preg;

    //Eliminos los terminos indexados anteriormente
    indicePregunta.clear();

    //Tokenizo la cadena de textoo y obtengo los terminos en TOKENS
    list<string> tokens;
    tok.Tokenizar(preg, tokens);
    //cout << "terminos de la pregunta " <<tokens.size()<<endl;

    // Conteo de palabras
        int numPal, NumPalDif,NumPalSinParada;
        contadorPalabras(tokens, numPal, NumPalDif, NumPalSinParada);

        infPregunta.setNumPal(numPal);
        infPregunta.setNumPalDiferentes(NumPalDif);
        infPregunta.setNumPalSinParada(NumPalSinParada);
    // --------------------------------

    // Recorro la lista de palabras y las indexo
        int pos = 0;
        list<string>::iterator itS;
        for(itS= tokens.begin();itS!= tokens.end();itS++){
            string b;
            b = (*itS);
            b = aplicarStemmer(b);

            if (!esStopWord(b))
                insertaTerminoPregunta( b, pos);
            pos++;
        }
    // -----------------------------------------
    return true;
};

/**
Aplica al termino pasado el stemmer correcspondiente y retorna el termino ya procesado
*/
string IndexadorHash::aplicarStemmer(string& termino){
    string salida;
    if (tipoStemmer==0) {//No se aplica stemmer
        salida = termino;
    }
    else
    if (tipoStemmer==1) {//Aplicar Stemmer espaÒol

        stemmerPorter ste;
        ste.stemmer(termino, 1, salida);
    }
    else
    if (tipoStemmer==2) {//Aplicar Stemmer ingles
        stemmerPorter ste;
        ste.stemmer(termino, 2, salida);
    }
    else
    cerr << "Tipo de Stemmer desconocido > "<< tipoStemmer<<endl;

    return salida;
}

/**
Indexa un termino pasado por parametro en la posicion pasada
Si el termino no existia en el indice, lo aÒade
*/
void IndexadorHash::insertaTerminoPregunta(const string& termino, const int& posicion){

    //Si el termino no esta indexado en la coleccion de la pregunta, lo aÒado
    unordered_map<string, InformacionTerminoPregunta>::const_iterator itp;
    itp = indicePregunta.find(termino);
    if (itp == indicePregunta.end()){
        InformacionTerminoPregunta infTerPre;
        indicePregunta[termino] = infTerPre;
    }

    //ahora al objeto indexado le aÒado la posicion
    indicePregunta[termino].add(posicion);
}

/**
Si hay una pregunta indexada, retorna true y devuelve la pregunta como string por referencia
Si no hay pregunta, retorna false
*/
bool IndexadorHash::DevuelvePregunta(string& preg) const
{
    if (pregunta.size() > 0){
        preg = pregunta;
        return true;
    }
    return false;
};

/**
Si el termino word esta indexado en la pregunta, retorna el objeto informacionTerminoPregunta por referencia y true
Si no esta, se retorna false */
bool IndexadorHash::DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const
{
    unordered_map<string, InformacionTerminoPregunta>::const_iterator itp;
    itp = indicePregunta.find(word);
    if (itp == indicePregunta.end()){
        return false;
    }
    else
    {
        inf = (*itp).second;
        return true;
    }
};

/**
Si existe una pregunta, retorna true y devuelve la pregunta por referencia
Si no, retorna false*/
bool IndexadorHash::DevuelvePregunta(InformacionPregunta& inf) const
{
    if (pregunta.size()>0){
        inf = infPregunta;
        return true;
    }
    else{
        return false;
    }
};

/**
Muestra por salida estandard todos los Terminos-InformacionTermino
*/
void IndexadorHash::ListarTerminos() const{
    unordered_map<string, InformacionTermino>::const_iterator it1;
    for (it1=indice.begin();it1!=indice.end();it1++){
        cout << (*it1).first << '\t' << (*it1).second << endl;
    }
};


/** Muestra por pantalla todos los documentos indexados*/
void IndexadorHash::ListarDocs() const{
    unordered_map<string, InfDoc>::const_iterator its;
    for (its=indiceDocs.begin(); its!=indiceDocs.end(); its++){
        string nomDoc = (*its).first;
        cout << nomDoc << '\t' << (*its).second << endl;
    }
};

/** Si el nombre de documento pasado por parametro existe en la coleccion de documentos indexados, muestra su informacion por pantalla*/
bool IndexadorHash::ListarDocs(const string& nomDoc) const
{
    bool res = false;
    unordered_map<string, InfDoc>::const_iterator its;

    its = indiceDocs.find(nomDoc);
    if (its != indiceDocs.end()){
        cout << nomDoc << '\t' << (*its).second << endl;
        res = true;
    }
    return res;
};


void IndexadorHash::ImprimirIndexacion() const {
    cout << "Terminos indexados: " << endl;
    ListarTerminos();

    cout << "Documentos indexados: " << endl;
    ListarDocs();
}


void IndexadorHash::ImprimirIndexacionPregunta() {
    cout << "Pregunta indexada: " << pregunta << endl;
    cout << "Terminos indexados en la pregunta: " << endl;
    cout << "Informacion de la pregunta: " << infPregunta << endl;
}

void IndexadorHash::ImprimirPregunta() {
    cout << "Pregunta indexada: " << pregunta << endl;
    cout << "Informacion de la pregunta: " << infPregunta << endl;
}


void IndexadorHash::borraTerminosConFrecuenciaCero(){

    unordered_map<string, InformacionTermino> new_indice;
    unordered_map<string, InformacionTermino>::const_iterator it;

    for (it=indice.begin(); it!=indice.end(); it++){
        if ((*it).second.getFrecuencia() != 0){

            new_indice[(*it).first] = (*it).second;
        }
    }

    informacionColeccionDocs.decPal(indice.size() - new_indice.size());
    indice.clear();
    indice = new_indice;
}


string IndexadorHash::getFileCreation(const string& f)const {
    const char* path = f.c_str();
    struct stat attr;
    stat(path, &attr);
    string salida = ctime(&attr.st_mtime);
    //cout << "Fecha de modificacion >"<< salida;
    return salida;
}

