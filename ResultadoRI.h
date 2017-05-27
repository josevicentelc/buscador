#ifndef RESULTADORI_H
#define RESULTADORI_H


#include <iostream>
#include <queue>

using namespace std;
class ResultadoRI {
    friend ostream& operator<<(ostream&, const ResultadoRI&);
public:
    ResultadoRI(const double& kvSimilitud, const long int& kidDoc, const int& np);
    double VSimilitud() const;
    long int IdDoc() const;
    int NP() const {return numPregunta;}
    bool operator< (const ResultadoRI& lhs) const;
private:
    double vSimilitud;
    long int idDoc;
    int numPregunta;
};

#endif // RESULTADORI_H
