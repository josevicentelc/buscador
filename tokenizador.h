#ifndef TOKENIZADOR_H
#define TOKENIZADOR_H
#include <iostream>
#include <list>
#include <fstream>

using namespace std;


class Tokenizador
{

    friend ostream& operator<<(ostream&, const Tokenizador&);

    public:
        void operator=( const Tokenizador&);
        Tokenizador (const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos);
        Tokenizador ();
        ~Tokenizador ();

        void AnyadirDelimitadoresPalabra(const string& nuevoDelimiters);

        void CasosEspeciales (const bool& nuevoCasosEspeciales);
        bool CasosEspeciales () const;
        void PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos) ;
        bool PasarAminuscSinAcentos () const;
        void DelimitadoresPalabra(const string&);

        void Tokenizar (const string& str, list<string>& tokens) const;
        bool Tokenizar (const string& i, const string& f) const;
        bool Tokenizar (const string & i) const;

        bool TokenizarListaFicheros (const string& i) const;
        bool TokenizarDirectorio (const string& i) const;

        list<string> TokenizaFichero(const string&);


        void setDeleteBeforeInsert(const bool&);

        string getDelimitadores() const {return delimiters;}

    private:
        bool casosEspeciales;
        bool pasarAminuscSinAcentos;
        bool deleteBeforeInsert;

        string delimiters;

        string casosURL;
        string casosDecimales;
        string casosEMAIL;
        string casosACRONIMO;
        string casosGUION;
        string simbolosPermitidosNumeros;

        string pasarAminusSinAcentos(const string&text) const;

        bool puedeSerURL(const string &m) const;
        bool puedeSerDECIMAL(const string &m, const string &caracter) const;
        bool puedeSerEMAIL(const string &m, const string&) const;
        bool puedeSerACRONIMO(const string &m) const;
        bool puedeSerGUION(const string &m) const;

        bool esAcronimo(const string &m) const;
        bool esEmail(const string &m) const;
        bool esNumero(const string &m) const;

        bool estaEnCasoEspecial(char& a, string &caso);

        void insertar(list<string>&, const string&) const;

        bool* limitadores;
        bool isDelimiter(const char&) const;
        void setDelimiters(const string&) const;
        void addDelimiter(const string&) const;
        void delDelimiter(const string&) const;
};


#endif // TOKENIZADOR_H
