#include "tokenizador.h"
#include <fstream>
#include <sys/stat.h>
#include <stdlib.h>



/**
    Recorre la cadena m y establece todos los caracteres como limitadores
    Complejidad en funcion de la longitud de m
        O(n)
*/
void Tokenizador::addDelimiter(const string& m) const{
    char c;
    int v;

    for (int i=m.size()-1;i>=0;i--){
        c = m[i];
        v = (int)c;
        if (v<0){
            v = v *-1;
            v +=512;
        }
        limitadores[v] = true;
    }
};

/**
    Marca todos los caracteres del string como delimitadores
    O(1) Cte
*/
void Tokenizador::setDelimiters(const string& m) const{
    for (int i=0;i<1024;i++) limitadores[i] = false;  //Primero pongo todos los caracteres a false
    addDelimiter(m);                                  //Ahora añado como limitadores aquellos que estan en la cadena
    if (casosEspeciales) addDelimiter(" ");           //Si los casos especiales estan activos me aseguro de que el espacio es delimitador
};

/**
    Este metodo es un Override del metido de la clase base, en su lugar llamo a addDelimiter
*/
void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters){
    addDelimiter(nuevoDelimiters);
};

/**
    Recorre la cadena "m" y establece todos los caracteres de la cadena como NO limitadores
    Complejidad en funcion de la longitud de m
        O(n)
*/
void Tokenizador::delDelimiter(const string& m) const{
    char c;
    int v;

    for (int i=m.size()-1;i>=0;i--){
        c = m[i];
        v = (int)c;
        if (v<0){
            v = v *-1;
            v +=512;
            }
        limitadores[v] = false;
    }
};

/**
    Retorna True si el caracter pasado es un delimitador
    False en caso contrario
    Complejidad
        O(1)
*/
bool Tokenizador::isDelimiter(const char& c) const{
    int v;
    v = (int)c;
    if (v<0){
        v = v *-1;
        v +=512;
    }
    return limitadores[v];
};


/** Destructor */
Tokenizador::~Tokenizador()
{
    delete [] limitadores;
    limitadores = NULL;
}


/**
    Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@"; casosEspeciales a true;
    pasarAminuscSinAcentos a false
*/
Tokenizador::Tokenizador()
{
    deleteBeforeInsert = true;
    limitadores = new bool[1024];
    delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
    setDelimiters(delimiters);

    casosURL       = ":/.?&-=";
    casosDecimales = ".,";
    casosEMAIL     = "@_.";
    casosACRONIMO  = ".";
    casosGUION     = "-";
    simbolosPermitidosNumeros = "0123456789$¤%ºª,.";
}

/**
    Inicializa delimiters a delimitadoresPalabra; casosEspeciales a kcasosEspeciales;
    pasarAminuscSinAcentos a minuscSinAcentos
*/
Tokenizador::Tokenizador (const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos)
{
    deleteBeforeInsert = true;

    limitadores = new bool[1024];
    delimiters = delimitadoresPalabra;
    pasarAminuscSinAcentos = minuscSinAcentos;
    casosEspeciales = kcasosEspeciales;
    setDelimiters(delimiters);
    if (kcasosEspeciales)   {
          addDelimiter(" ");
    }
    casosURL       = ":/.?&-=";
    casosDecimales = ".,";
    casosEMAIL     = "@_.";
    casosACRONIMO  = ".";
    casosGUION     = "-";
    simbolosPermitidosNumeros = "0123456789$¤%ºª,.";
};

/**
    Funcion que recibe una cadena de texto y retorna la misma cadena pasada a minusculas
    y sin acentos
*/
string Tokenizador::pasarAminusSinAcentos(const string& text) const{

    string salida;
    salida = "";
    char caracter;
    for (int i =0; i<text.size();i++){
        caracter = text[i];
        if (caracter >= 'A' && caracter <= 'Z')
            salida += (char)((int)caracter +32 );
        else
        if (caracter == 'á' || caracter == 'à' || caracter == 'ä' || caracter == 'â' || caracter == 'Á' || caracter == 'À' || caracter == 'Ä' || caracter == 'Â')
            salida += 'a';
        else
        if (caracter == 'é' || caracter == 'è' || caracter == 'ë' || caracter == 'ê' || caracter == 'É' || caracter == 'È' || caracter == 'Ë' || caracter == 'Ê')
            salida += 'e';
        else
        if (caracter == 'í' || caracter == 'ì' || caracter == 'ï' || caracter == 'î' || caracter == 'Í' || caracter == 'Ì' || caracter == 'Ï' || caracter == 'Î')
            salida += 'i';
        else
        if (caracter == 'ó' || caracter == 'ò' || caracter == 'ö' || caracter == 'ô' || caracter == 'Ó' || caracter == 'Ò' || caracter == 'Ö' || caracter == 'Ô')
            salida += 'o';
        else
        if (caracter == 'ú' || caracter == 'ù' || caracter == 'ü' || caracter == 'û' || caracter == 'Ú' || caracter == 'Ù' || caracter == 'Ü' || caracter == 'Û')
            salida += 'u';
        else
        if (caracter == 'Ñ')
            salida += 'ñ';
        else
            salida = salida + caracter;
    }

    return salida;
}

/**
    Aunque se modifique el almacenamiento de los delimitadores por temas de eficiencia,
    el campo delimiters se imprimirá con el string con el que se inicializó el tokenizador
*/
ostream& operator<<(ostream& os, const Tokenizador& tknz)
{
    os << "DELIMITADORES: " << tknz.delimiters << " TRATA CASOS ESPECIALES: " << tknz.casosEspeciales << " PASAR A MINUSCULAS Y SIN ACENTOS: " << tknz.pasarAminuscSinAcentos;
    return os;
};

/**
    Operador de asignacion
*/
void Tokenizador::operator=( const Tokenizador& otro){
    deleteBeforeInsert = otro.deleteBeforeInsert;
    casosEspeciales = otro.casosEspeciales;
    pasarAminuscSinAcentos = otro.pasarAminuscSinAcentos;
    delimiters = otro.delimiters;
    setDelimiters(delimiters);
}



/**
    Cambia la variable privada (casosEspeciales)
*/
void Tokenizador::CasosEspeciales (const bool& nuevoCasosEspeciales)
{
    casosEspeciales = nuevoCasosEspeciales;
    if (casosEspeciales)        addDelimiter(" ");
    else                        delDelimiter(" ");
    //cout << "Casos Espaciales "<< casosEspeciales << endl;
};

/**
    Devuelve el contenido de la variable privada (casosEspeciales)
*/
bool Tokenizador::CasosEspeciales () const
{
    return casosEspeciales;
};

/**
    Cambia la variable privada (pasarAminuscSinAcentos). Atención al formato de codificación
    del corpus (comando (file) de Linux). Para la corrección de la práctica se utilizará el
    formato actual (ISO-8859).
*/
void Tokenizador::PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos)
{
    pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;
}

/**
    Retorna el estado de la variable boleana pasarAminusculasYsinAcentos
*/
bool Tokenizador::PasarAminuscSinAcentos () const
{
    return pasarAminuscSinAcentos;
};

void Tokenizador::setDeleteBeforeInsert(const bool& n){
    deleteBeforeInsert = n;
}


/**
    Establece la lista de delimitadores a la aquellos pasados por parametro
*/
void Tokenizador::DelimitadoresPalabra(const string& newdel)
{
    delimiters = newdel;
    setDelimiters(newdel);
    if (CasosEspeciales()) addDelimiter(" ");
};

/**
    Tokeniza una cadena y almacena los valores extraidos en le vector pasado por parametro
    El vector será vaciado
*/
void Tokenizador::Tokenizar (const string& cadena, list<string>& tokens) const
{
    if (deleteBeforeInsert) tokens.clear(); //Se vacia el vector
    string str;

    //Si esta establecida la variable, se pasa el contenido a minusculas sin acentos
    if (pasarAminuscSinAcentos)
        str = pasarAminusSinAcentos(cadena);
    else
        str = cadena;

    //Si no esta establecido los casos especiales, llamo al tokenizador de la clase base
    if (!casosEspeciales){
        string::size_type lastPos = str.find_first_not_of(delimiters,0);
        string::size_type pos = str.find_first_of(delimiters,lastPos);
        while(string::npos != pos || string::npos != lastPos)
        {
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
        }
    }
    else{
            /*Recorrer la cadena a tokenizar de izquierda a derecha hasta encontrar un delimitador (o un blanco).
            ¿El delimitador aparece entre las excepciones de los casos especiales? (los casos especiales se irán comprobando en el orden descrito anteriormente, quedándose con el primer caso especial que así lo cumpla)
            En ese caso, el delimitador no habría de ser tenido en cuenta y se continuaría analizando según ese caso especial.
            En caso contrario, se extraería del token a devolver (siempre que no sea una palabra vacía) sin incluir el delimitador.
            */

        string::size_type puntero = 0;
        string::size_type fin = str.size();

        string texto  = "";         //Variable auxiliar, Tramo de texto desde el ultimo corte hasta la posicion actual de i
        string caracter = "";       //Actual caracter en la posicion i
        string caracterReal = "";   //En alguna ocasion modifico caracter, en esta variable siempre almaceno el valor que hay de verdad

        bool ignorado;
        int flag = 0;

        for (int i = 0; i<fin;i++){
            caracter = str.substr(i,1);
            caracterReal = str.substr(i,1);

            //int found = delimiters.find(caracter,0);
               // if (found>-1 || i==(fin-1)){    //Si encuentro un separador, o si llego al final de la cadena de texto
                if (isDelimiter(str[i]) || i==(fin-1)){    //Si encuentro un separador, o si llego al final de la cadena de texto

                    ignorado = true;

                    /** Cuando llego al ultimo caracter de la cadena, me imagino que he encontrado un separador,
                    si no lo hiciese asi, no se extraeria la ultima palabra*/
                    if (i==fin-1){
                        i++;
                        caracter = " ";
                    }

                    texto = str.substr(puntero, i-puntero); //Obtengo el tramo de texto

                    /** Solo llego al centro del IF anidado si no encuentro razon para ignorar el separador

                    Ese decir, llegare al centro y extraere la palabra si
                        El separador no esta en la lista de separadores a ignorar
                        y
                        Si la cadena de texto no puede ser url ni numero ni email ... etc*/

                    if (casosURL.find(caracter,0)       == -1  || !puedeSerURL(texto)) //Es caso especial de url? y puede ser una url?
                    if (casosDecimales.find(caracter,0) == -1  || !puedeSerDECIMAL(texto, caracterReal))
                    if (casosEMAIL.find(caracter,0)     == -1  || !puedeSerEMAIL(texto, caracterReal))
                    if (casosACRONIMO.find(caracter,0)  == -1  || !puedeSerACRONIMO(texto + caracterReal))
                    if (casosGUION.find(caracter,0)     == -1  || !puedeSerGUION(texto))   {
                        ignorado = false;

                        if ((flag > puntero) && !esNumero(texto) && !esAcronimo(texto)){
                            insertar(tokens, str.substr(puntero, flag-puntero));
                            puntero = flag+1;
                        }

                        if (str.substr(puntero, i-puntero).size() > 0){
                            if (caracter=="@" && esEmail(texto)){
                                /** Verifico que el proximo caracter no sea un separador, en tal caso lo que venga por detras es un mail valido*/
                                //int a = delimiters.find(str.substr(i+1, 1));
                                //if (a == -1){
                                if (!isDelimiter(str[i+1])){
                                    bool encontrado = false;
                                    int pos = -1;
                                    for (int j=puntero;j<i && pos == -1; j++){  /** busco la posicion de la primera arroba*/
                                        if (str.substr(j,1)=="@") pos = j;
                                    }

                                    if (pos != -1){
                                        insertar(tokens, str.substr(puntero, pos-puntero));             /** inserto hasta esa arroba en el vector */
                                        puntero = pos+1;                                                /** situo el puntero en la posicion siguiente a la arroba */
                                    }
                                }
                            }
                            else{
                                insertar(tokens, str.substr(puntero, i-puntero));
                                puntero = i+1;
                            }
                        }else puntero = i+1;
                    }

                    /** Se ignora un separador porque la cadena puede ser un numero decimal, pero no se si más adelante dejara de ser valido
                    por tanto, me guardo la posicion del separador en "flag" por si tengo que cortar en el futuro por aqui*/
                    if ((caracter == "." || caracter == ",") && puedeSerDECIMAL(texto, caracter)){
                        //cout << "He ignorado el separador >"<< str.substr(i, 1)<< "< de la posicion "<< i << " >"<<texto<< "<"<< endl;
                        flag = i;
                    }

                    /** Caso muy particular al encontrar una arroba como separador
                    Si el texto recorrido es un valor numerico valido y el caracter separador es una arroba, se cortara por la arroba
                    ya que el valor numerico es prioritario a los emails
                    */
                    if (caracter == "@" && esNumero(texto)){
                        insertar(tokens, texto);
                        puntero = i+1;
                    }
                }
        }
    }
}

void Tokenizador::insertar(list<string> &tokens, const string &m) const{

    if (isDelimiter(m[m.size()-1])  && m.size() >=2){
            insertar(tokens, m.substr(0, m.size()-1));
    }
    else
    if (esAcronimo(m) && m.substr(m.size()-1, 1) == "."){
            insertar(tokens, m.substr(0, m.size()-1));
    }
    else
    if (esAcronimo(m) && m.substr(0, 1) == ".")  {
            insertar(tokens, m.substr(1, m.size()-1));
    }
    else
    if (esNumero(m)){
    /**
    Si lo que voy a insertar es un numero, verifico si tiene un caracter de simbolo al final
    en tal caso, inserto por un lado el numero y por otro el simbolo
    */
        string numero = "";
        string simbolo = "";

        if (m.substr(0, 1) == "." || m.substr(0, 1) == ",") numero = "0";
        if ((m.substr(m.size()-1,1) > "9" || m.substr(m.size()-1,1) < "0")  &&
            (m.substr(m.size()-1,1) != ".")  &&
            (m.substr(m.size()-1,1) != ",")){
            numero += m.substr(0, m.size()-1);
            simbolo = m.substr(m.size()-1, 1);
        }
        else{
            numero += m;
        }
        if (numero.size()>=1 ) tokens.push_back(numero);

        if (simbolo != "" && simbolo != " " ) {
                tokens.push_back(simbolo);
        }
    }
    else
    {
        /** Si no se ha cumplido ningun caso particular anterior, inserto sin mas lo que tengo en m*/
        if (m.size() >1 || isDelimiter(m[0]) == false)
        {
            tokens.push_back(m);
        }
    }
}


bool Tokenizador::Tokenizar (const string& NomFichEntr, const string& NomFichSal) const
{
    if (!casosEspeciales){
            struct stat dir;
            int err=stat(NomFichEntr.c_str(), &dir);
            if(S_ISDIR(dir.st_mode)) {
                return true;
            }

            ifstream i;
            ofstream f;
            string cadena;
            list<string> tokens;
            i.open(NomFichEntr.c_str());
            if(!i) {
                cerr << "ERROR: No existe el archivo: " << NomFichEntr << endl;
                //cout << "no existe "<< NomFichEntr<<endl;
                return false;
            } else {
                //cout << "Si existe "<< NomFichEntr<<endl;
                while(!i.eof()){
                    cadena="";
                    getline(i, cadena);
                    if(cadena.length()!=0){
                        Tokenizar(cadena, tokens);
                    }
                }
            }
            i.close();
            f.open(NomFichSal.c_str());
            list<string>::iterator itS;
            for(itS= tokens.begin();itS!= tokens.end();itS++){
                f << (*itS) << endl;
            }
            f.close();
            return true;

    }
    else{
        ifstream i;
        ofstream f;
        string cadena;
        list<string> tokens;
        i.open(NomFichEntr.c_str());
        f.open(NomFichSal.c_str());

        if(!i) {
            cerr << "ERROR: No existe el archivo: " << NomFichEntr << endl;
            return false;
        } else {
            while(!i.eof()){
                cadena="";
                getline(i, cadena);
                if(cadena.length()!=0){
                    Tokenizar(cadena, tokens);
                    list<string>::iterator itS;
                    for(itS= tokens.begin();itS!= tokens.end();itS++){
                        f << (*itS) << endl;
                    }
                }
            }
        }
        i.close();
        f.close();
        return true;
    }
}

/**
Toleniza el contenido del fichero pasado por parametro y almacena la salida en un fichero con el mismo nombre acabado en extension .tk
*/
bool Tokenizador::Tokenizar (const string & i) const{

    struct stat dir;
    int err=stat(i.c_str(), &dir);
    if(S_ISDIR(dir.st_mode)) {
        return true;
    }

    if (!casosEspeciales)
        return Tokenizar(i, i+".tk");
    else{
        string fichEntrada = i;
        string fichSalida = i + ".tk";
        return Tokenizar(fichEntrada, fichSalida);
    }
}


bool Tokenizador::estaEnCasoEspecial(char& a, string &caso){
    return false;
}


list<string> Tokenizador::TokenizaFichero(const string& fichero){
    list<string> tokens;
    ifstream f;
    f.open(fichero.c_str());
    if (!f) cerr << "No existe el fichero "<<fichero << endl;
    else{
        string linea;
        while (!f.eof()){
            getline(f, linea);
            //cout << "  "<< linea << endl;
            Tokenizar(linea, tokens);
            deleteBeforeInsert = false;
        }
        deleteBeforeInsert = true;
    }
    f.close();
    return tokens;
}

bool Tokenizador::puedeSerURL(const string &m) const {
/*
Para detectar los límites de las URL se ha de implementar la heurística que detecta una palabra que comienza por espacio en blanco
(o cualquiera de los delimitadores definidos por el usuario) y SOLO los indicadores de URL http: o https: o ftp: (en minúsculas)
seguido por una secuencia de caracteres (incluidos _:/.?&-=#@ aunque estén definidos como delimitadores), sin ningún blanco por medio.
Finalizará cuando se detecte un delimitador (excepto _:/.?&-=#@) o un blanco (aunque no esté entre el conjunto de delimitadores).
Por ejemplo, la siguiente secuencia de comandos (destacar que no se aplica la detección de e-mails u otros casos especiales dentro de
la URL que es el primer caso especial que se comprueba):
*/

bool res = false;
//Primero busco el comienzo de la frase
if (m.size() == 0)
    res = false;
else
{
    if (m.size() >= 6 && m.substr(0, 6) == "https:") res = true;
    if (m.size() >= 5 && m.substr(0, 5) == "http:") res = true;
    if (m.size() >= 4 && m.substr(0, 4) == "ftp:") res = true;
}

//Si se ha cumplido la condicion de inicio, me aseguro de que no tiene espacios
for (int i=m.size()-1;i>=0;i--)
    if (m[i]==' ') res = false;


return res;
}

bool Tokenizador::puedeSerDECIMAL(const string &m, const string &caracter) const{
/*
Igualmente se aplicaría esta última heurística para los números con o sin ,. y con los componentes del término que sean solo
dígitos numéricos (no se reconocerán números en formato científico, p.ej. 1,23E+10).
Varios puntos o comas seguidos no representarán a un número (p.ej. 3..2).
Se detectará el inicio por un blanco; o separador; o ., seguido de números en cuyo caso se le añadirá
el 0 delante del término (p.ej. .35 se almacenaría 0.35, pero ..35 no cumpliría esa condición).
Se detectará el final por la presencia de un espacio en blanco; o punto/coma seguido de blanco (los cuales no pertenecerían al término); o los símbolos
%$¤ºª seguidos de blanco (los cuales se almacenarían en un término posterior); o delimitador; o lo anterior más delimitador.
*/

    if (m.size()==0)
        if (caracter=="." || caracter==",") {
            return true;
        }else{
            return false;
        }

    if ((caracter == "." || caracter == ",") && (m.substr(m.size()-1,1) == "," || m.substr(m.size()-1,1) == ".")) return false;

    char anterior = ' ';
    char actual  = ' ';
    bool res = true;

    for (int i = 0; i<m.size() && res; i++){
        actual = m[i];

        //Si no es un numero punto o coma, entonces no es un valor decimal
        if ((actual < '0' || actual > '9') && (actual != ',') && (actual != '.'))
            res = false;

        //Si se dan dos puntos o comas seguidos, entonces no es un decimal
        if ((actual == ',' || actual == '.') && (anterior == ',' || anterior == '.'))
            res = false;

        anterior = actual;
    }

return res;
}

bool Tokenizador::puedeSerEMAIL(const string &m, const string &siguiente) const{
/*
Para los e-mails, se detectará la presencia de un e-mail en un término que se inicia por un blanco o separador seguido de cualquier carácter,
y que contiene un solo @ por medio.
Además podrá contener por el medio .-_ (aunque estén definidos como delimitadores), sin ningún
blanco por medio. Se detectará el final por la presencia de un espacio en blanco (aunque no esté definido como delimitador) o
delimitador (excepto .-_).
*/

bool res = false;
int cuenta = 0;

if (siguiente == "@" && m.size()==0) return false;

for (int i=m.size()-1;i>=0;i--){
        if (m[i]=='@')    cuenta++;
}
//&& m.substr(0,1)!="@"
if (m.size()>0 && ((cuenta==0 && siguiente == "@")|| (cuenta==1 && siguiente != "@")) ) res = true;

return res;

}

bool Tokenizador::puedeSerACRONIMO(const string &m) const{
/*
Para los acrónimos, se detectarán términos que contengan puntos por medio sin ningún blanco (aunque se haya definido el punto como
separador). Los puntos han de estar separados por caracteres distinto del propio punto (p.ej. U..S no se detectaría como acrónimo).
Se detectará el final por la presencia de un espacio en blanco o delimitador o varios puntos seguidos. Los puntos del principio y
final del término acrónimo se eliminarán: p.ej. U.S.. se almacenaría como U.S; o ..U.S se almacenaría como U.S.
*/
bool res = true;
//cout << "Verifico si >" << m << "< Puede ser acronimo : ";
char actual = ' ';
char anterior = ' ';
//PUEDE SER ACRONIMO SI...
    //NO TIENE SEPARADORES DIFERENTES DE .
    //NO TIENE MAS DE UN . SEGUIDO
    for (int i = 0; i<m.size() && res;i++)
    {
        actual = m[i];

        if (actual=='.' && anterior == '.') res = false;                    //Dos puntos seguidos, no es un acronimo
        //if (actual != '.' && delimiters.find(actual, 0) != -1) res = false;   //Tengo un delimitador, y no es el ., no puede ser acronimo
        if (actual != '.' && isDelimiter(actual)) res = false;   //Tengo un delimitador, y no es el ., no puede ser acronimo
        anterior = actual;
    }
    //cout << res << endl;
return res;
}

//ES ACRONIMO SI...
//MIDE AL MENOS 3 CARACTERES "A.B"
//NO TIENE SEPARADORES DIFERENTES DE .
//NO TIENE MAS DE UN . SEGUIDO
bool Tokenizador::esAcronimo(const string &m) const{

    //Si cumple las restricciones de acronimo y mide al menos 3 caracteres...
    bool res = (m.size()>=3 && puedeSerACRONIMO(m));

    //hay que verificar que existen puntos de por medio, de lo contrario no es un acronimo
    if (res){
        if (m.substr(1, m.size()-2).find(".", 0) != -1) res = true;
        else                                            res = false;
    }

    return res;
}

bool Tokenizador::esEmail(const string &m) const{
    bool res = false;
    int cuenta = 0;
    for (int i=m.size()-1;i>=0;i--){
        if (m[i]=='@') cuenta++;
    }
    if (cuenta==1)  return true;
    else            return false;
}

bool Tokenizador::esNumero(const string &m) const{

    bool res = true;
    string anterior = "";
    string actual = "";

    if (m.size()==0) return false;          //Con tamaño 0 no es numero
    if (m == "." || m== ",") return false;  //un solo punto no es un numero

    for (int i=m.size()-2;i>=0 && res;i--){ //Recorro todos los digitos menos el ultimo que podria ser un simbolo de moneda
        actual = m.substr(i, 1);

        //Un simbolo diferente de un numero, punto o coma hace que no sea un decimal
        if ((actual < "0" || actual >"9") && actual != "." && actual != ",") res = false;

        //Dos puntos o comas seguidos ya no son un decimal
        if ((actual == "." || actual == ",") && (anterior == "," || anterior == ".")) res = false;
        anterior = actual;
    }

    //Si el ultimo simbolo no esta permitido, entonces no es un numero
    if (res) {//   if (simbolosPermitidosNumeros.find(m.substr(m.size()-1, 1)) == -1) {
        string c = m.substr(m.size()-1, 1);
        if ((c >= "a" && c<="z") || (c >= "A" && c<="Z"))
            res = false;
    }

    return res;
}


bool Tokenizador::puedeSerGUION(const string &m) const{
/*
Para las palabras compuestas formadas por guiones, la heurística a implementar es el detectar un término que contenga
por el medio el carácter - sin ningún espacio en blanco, aunque se haya definido - dentro de los delimitadores.
Se detectará el inicio y final de la palabra compuesta cuando aparezca el blanco (aunque no aparezca entre los delimitadores)
o cualquiera de los delimitadores definidos por el usuario (excepto el -). Por ejemplo, la siguiente secuencia de comandos:
*/

///Rechazo la frase si comienza por guion o si tiene espacios
    if (m.size() == 0)
        return false;
    else
        if (m.substr(0,1) == "-")
            return false;
        else
            for (int i=0;i<m.size();i++)
                if (m[i] == ' ') return false;
    return true;
}


bool Tokenizador::TokenizarListaFicheros (const string& i) const{

    ifstream f;
    string fichero;

    f.open(i.c_str());
    if(!f) {
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    } else {
        while(!f.eof()){
            fichero="";
            getline(f, fichero);
            Tokenizar(fichero);
        }
    }
    f.close();
    return true;
}

/* Tokeniza todos los archivos que contenga el directorio i, incluyendo los de los subdirectorios, guardando la salida en ficheros cuyo nombre
 será el de entrada añadiéndole extensión .tk, y que contendrá una palabra en cada línea del fichero.
 Devolverá true si se realiza la tokenización de forma correcta de todos los archivos;
 devolverá false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista el
 directorio i, o los ficheros que no se hayan podido tokenizar) */

bool Tokenizador::TokenizarDirectorio (const string& dirAIndexar) const {
    struct stat dir;
    int err=stat(dirAIndexar.c_str(), &dir);
    if(err==-1 || !S_ISDIR(dir.st_mode)){
        return false;
    } else {
        // Hago una lista en un fichero con find>fich
        string cmd="find "+dirAIndexar+" -follow | sort > .lista_fich";
        system(cmd.c_str());
        return TokenizarListaFicheros(".lista_fich");
    }
}
