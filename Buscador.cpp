#include "Buscador.h"
#include <fstream>
#include <sstream>


/** CONSTRUCTORES, DESTRUCTORES Y OPERADORES*/

/** ****************************************************************************************************** */
Buscador::Buscador(const string& directorioIndexacion, const int& f): IndexadorHash(directorioIndexacion){
    formSimilitud = f;
    c = 2; k1 = 1.2; b = 0.75;
    setStopWords("StopWordsEspanyol.txt");
}

/** ****************************************************************************************************** */
Buscador::Buscador(const Buscador & otro): IndexadorHash(otro){
    formSimilitud = otro.formSimilitud;
    c = otro.c; k1 = otro.k1; b = otro.b;
}

/** ****************************************************************************************************** */
Buscador::Buscador(){}

/** ****************************************************************************************************** */
Buscador::~Buscador(){}

/** ****************************************************************************************************** */
string Buscador::getResult(const int& id) const{
    string salida = "";
    priority_queue<ResultadoRI> dO = docsOrdenados;

    for (int i =0;i< id;i++){
        dO.pop();
    }
    InfDoc infoDoc;
    getDocById(dO.top().IdDoc(), salida, infoDoc);

    return salida;
}

/** ****************************************************************************************************** */
Buscador& Buscador::operator= (const Buscador & otro){
    formSimilitud = otro.formSimilitud;
    c = otro.c; k1 = otro.k1; b = otro.b;
    return (*this);
}

/** GETTERS Y SETTERS*/

/** ****************************************************************************************************** */
int Buscador::DevolverFormulaSimilitud() const{    return formSimilitud;}

/** ****************************************************************************************************** */
bool Buscador::CambiarFormulaSimilitud(const int& f){
    if (f == 1 || f == 0){
        formSimilitud = f;
        return true;
    }
    return false;
}

/** ****************************************************************************************************** */
void Buscador::CambiarParametrosDFR(const double& kc){    c = kc;}

/** ****************************************************************************************************** */
double Buscador::DevolverParametrosDFR() const{ return c;}

/** ****************************************************************************************************** */
void Buscador::CambiarParametrosBM25(const double& kk1, const double& kb){
    k1 = kk1;
    b = kb;
}

/** ****************************************************************************************************** */
void Buscador::DevolverParametrosBM25(double& kk1, double& kb) const{
    kk1 = k1;
    kb = b;
}


/** ****************************************************************************************************** */
string Buscador::genResultados(const int& numDocumentos) const{
    string resultado;
    priority_queue<ResultadoRI> dO = docsOrdenados;

    for (int i =0;i< numDocumentos && !dO.empty();i++){
        string salida;
        if (formSimilitud==0)   salida += "DFR ";
        else                    salida += "BM25 ";

        //Obtengo el nombre del fichero a partir del id de documento
        string nombreFichero;
        InfDoc inf;
        getDocById(dO.top().IdDoc(), nombreFichero, inf);

        //Al nombre de fichero le quito la ruta de la carpeta
        int ultimaBarra = -1;
        for (int j=0;j<nombreFichero.size();j++){
            if (nombreFichero[j] == '/') ultimaBarra = j+1;
        }
        if (ultimaBarra != -1)
            nombreFichero = nombreFichero.substr(ultimaBarra, nombreFichero.size()-ultimaBarra);

        //Ahora le quito la extension .tim, quitandole los ultimos 4 caracteres
        nombreFichero = nombreFichero.substr(0, nombreFichero.size()-4 );

        salida += nombreFichero + " ";

        ostringstream conv;
        conv << i << " " << dO.top().VSimilitud() << " " << dO.top().NP() << " ConjuntoDePreguntas";
        salida += conv.str();
        resultado +=  salida + '\n';
        dO.pop();
    }

    return resultado;
}

/** ****************************************************************************************************** */
void Buscador::ImprimirResultadoBusqueda(const int& numDocumentos) const{
    cout << genResultados(numDocumentos);
}

/** ****************************************************************************************************** */
bool Buscador::ImprimirResultadoBusqueda(const int& numDocumentos, const string& nombreFichero) const{

    ofstream salida;
    salida.open(nombreFichero);
    string res = genResultados(numDocumentos);
    if (salida){
        salida <<  res;
        cout << res << endl;
        salida.close();
        return true;
    } else
    {
        cerr << "Error al crear el fichero "<< nombreFichero << endl;
        return false;
    }
}


/** METODOS DE QUESQUEDA*/

/** ****************************************************************************************************** */
bool Buscador::Buscar(const int& numDocumentos){
    //Vacio los resultados anteriores
    while (!docsOrdenados.empty()) docsOrdenados.pop();

    if (formSimilitud == 0){    //Formula DFR
        //Añado a la lista la similitud para cada documento de la coleccion
        unordered_map<string, InfDoc>::const_iterator it;
        for (it=indiceDocs.begin(); it!= indiceDocs.end(); it++){
            docsOrdenados.push(ResultadoRI(sim((*it).second), (*it).second.idDoc, 1));
        }
    }
    else
    {
        //Primero necesito calcular el parametro avgdl
        //que es la media del numero de palabras de la coleccion
        double avgdl = 0;
        unordered_map<string, InfDoc>::const_iterator itd;
        for (itd=indiceDocs.begin(); itd != indiceDocs.end(); itd++){
            avgdl += (*itd).second.getTotalPalSinParada();
        }
        avgdl = 1.0 * avgdl / indiceDocs.size();

        //Formula BM25
        unordered_map<string, InfDoc>::const_iterator it;
        for (it=indiceDocs.begin(); it!= indiceDocs.end(); it++){
            docsOrdenados.push(ResultadoRI(score((*it).second, avgdl), (*it).second.idDoc, 0));
        }
    }
    return true;
}

/** ****************************************************************************************************** */
bool Buscador::Buscar(const string& dirPreguntas, const int& numDocumentos, const int& numPregInicio, const int& numPregFin){

    double avgdl = 0;

    //Vacio los resultados anteriores
    while (!docsOrdenados.empty()) docsOrdenados.pop();

    //Si se utiliza BM25, calculo ya avgdl que se utilizara para todos los calculos posteriores
    if (formSimilitud == 1){
        unordered_map<string, InfDoc>::const_iterator itd;
        for (itd=indiceDocs.begin(); itd != indiceDocs.end(); itd++){
            avgdl += (*itd).second.getTotalPalSinParada();
        }
        avgdl = 1.0 * avgdl / indiceDocs.size();
    }

    for (int actual=numPregInicio;actual<numPregFin || actual==numPregFin; actual++){

        ifstream input;
        string fichero;
        ostringstream convert;
        convert << actual;
        fichero = dirPreguntas+"/"+convert.str()+".txt";

        //Obtengo la pregunta del fichero numero X
        input.open(fichero.c_str());
        if (!input)
        {
            cerr << "Error, no existe el fichero >" << fichero << "<" << endl;
            return false;
        }
        else
        {
            string pregunta;
            getline(input, pregunta);
            IndexarPregunta(pregunta);
            priority_queue<ResultadoRI> temporal;

            //Formula DFR
            if (formSimilitud == 0){
                //Añado a la lista la similitud para cada documento de la coleccion
                unordered_map<string, InfDoc>::const_iterator it;
                for (it=indiceDocs.begin(); it!= indiceDocs.end(); it++){
                    temporal.push(ResultadoRI(sim((*it).second), (*it).second.idDoc, actual));
                }
            }
            else
            {
            //Formula BM25
                unordered_map<string, InfDoc>::const_iterator it;
                for (it=indiceDocs.begin(); it!= indiceDocs.end(); it++){
                    temporal.push(ResultadoRI(score((*it).second, avgdl), (*it).second.idDoc, actual));
                }
            }

            for (int i=0;i< numDocumentos;i++){
              docsOrdenados.push(temporal.top());
              temporal.pop();
            }
        }
        input.close();
    }
    return true;
}


/** Metodos propios*/

/** ****************************************************************************************************** */
/** Retorna un indice de similitus entre la query indexada y el documento pasado*/
double Buscador::sim(const InfDoc &Doc){
    double res = 0;
    unordered_map<string, InformacionTerminoPregunta>::const_iterator it;

    //Recorro todos los terminos de la pregunta
    for (it=indicePregunta.begin(); it!=indicePregunta.end();it++){
        //Obtengo el string del termino
        string strtermino = "";
        strtermino = (*it).first;
        //Localizo el termino en la indeacion
        InformacionTermino termino;
        if (Devuelve(strtermino, termino)){
            res += pesoTerminoQuery((*it).second) * pesoTerminoDocumento(termino, Doc);
        }
    }
    return res;
}


/** ****************************************************************************************************** */
/** Retorna el peso que tiene un termino en la query indexada*/
double Buscador::pesoTerminoQuery(const InformacionTerminoPregunta &termino){
    long int k = infPregunta.getNumPal();
    if (k == 0) return 0;
    double res = 0;
    res = 1.0 * termino.ft / k;
    return res;
}

/** ****************************************************************************************************** */
/** Retorna el peso de un termino en el documento pasado por parametro*/
double Buscador::pesoTerminoDocumento(const InformacionTermino &termino, const InfDoc &Doc){
    double res = 0;
    double lambat = lambda(termino);
    res = log2(1 + lambat);
    res += fPrimaTerminoDocumento(termino, Doc) * log2((1+ lambat) / lambat);
    res = res * (termino.ftc +1) / termino.l_docs.size() * (fPrimaTerminoDocumento(termino, Doc) * log2((1+ lambat) / lambat) +1);
    return res;
}

/** ****************************************************************************************************** */
double Buscador::lambda(const InformacionTermino &termino){
    long int ft = termino.getFrecuencia();

    int N = indiceDocs.size();
    if (N == 0) return 0;

    else {
        return (1.0 * ft/N);
    }
}

/** ****************************************************************************************************** */
double Buscador::fPrimaTerminoDocumento(const InformacionTermino& termino, const InfDoc &Doc) const{
    InfTermDoc infter;
    if (!termino.IndexedAtDocument(Doc.getId(), infter)) return 0;
    double res = infter.posTerm.size() * log2(1 + (c * (informacionColeccionDocs.getBytes() / informacionColeccionDocs.numDocs) / Doc.getBytes()));
    return res;
}


/** ****************************************************************************************************** */
/** Funciones utilizadas en el modelo BM25*/
double Buscador::score(const InfDoc &Doc, double &avgdl){
    double salida = 0;

    unordered_map<string, InformacionTerminoPregunta>::const_iterator it;
    for (it = indicePregunta.begin(); it!= indicePregunta.end(); it++){
        double fqiD = 0;
        //Localizo el termino en el indice
        unordered_map<string, InformacionTermino>::const_iterator ittermino;
        ittermino = indice.find((*it).first);
        if (ittermino!=indice.end()){

            //Obtengo la informacion del Termino/Documento
            InfTermDoc itd;
            if ((*ittermino).second.IndexedAtDocument(Doc.getId(), itd)){
                //Obtengo la frecuencia del termino en el documento
                fqiD = itd.ft;
            }
        }

        //Solo si fqid es distinto de 0 tiene sentido continuar
        if (fqiD != 0){

            double salidaTermporal = 0;
            salidaTermporal = 1.0 * fqiD * (k1 +1);
            salidaTermporal = 1.0 * salidaTermporal / (fqiD + k1 * (1 - b + b * (Doc.getTotalPalSinParada() / avgdl)  ));
            if (salidaTermporal != 0){

                double idf = IDF((*it).first);
                salidaTermporal = salidaTermporal * idf;
                salida += salidaTermporal;
            }
        }
    }
    return salida;
}

/** ****************************************************************************************************** */
double Buscador::IDF(const string &termino){
    double salida = 0;
    //Obtengo (nqi) numero de documentos en los que aparece el termino
    long int nqi = 0;
    unordered_map<string, InformacionTermino>::const_iterator it;
    it = indice.find(termino);
    if (it != indice.end()){
        nqi = (*it).second.l_docs.size();
    }
    salida = indiceDocs.size() - nqi + 0.5;
    salida = 1.0 * salida / (nqi+0.5);
    salida = log(salida);
    return salida ;
}

