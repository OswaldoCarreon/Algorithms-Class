/* File:   Clases_Interprete.h
 * Author: max_l and oswalt_mosby
 * Created on 10 de diciembre de 2019, 08:18 PM
 */

#include <bits/stdc++.h>
#ifndef CLASES_INTERPRETE_H
#define	CLASES_INTERPRETE_H
using namespace std;

class TokenConClave {
private:
    string token;
    int clave;
public:

    TokenConClave(string token, int clave) {
        this->token = token;
        this->clave = clave;
    }

    TokenConClave(const TokenConClave& original) {
        this->token = original.token;
        this->clave = original.clave;
    }

    string getToken() {
        return this->token;
    }

    int getClave() {
        return this->clave;
    }

    string to_string() {
        return this->token + " " + std::to_string(this->clave);
    }
    //~TokenConClave();


};

vector<vector<TokenConClave>> analizador_lexico(map<string, int> claves, string filename);

map<string, int> generaClaves();

//-------------------------- Separar tokens ------------
vector<string> separaToken(string s);

bool is_float(string s);

bool is_int(string s);

//-------------------------- Lee el archivo y crea la matriz ------------

vector <vector <string>> getTokens(string filename);

//-------------------------- Asocia cada token con su clave ------------

vector< vector<TokenConClave>> relacionaTokenConClave(
        vector <vector< string>> matrizTokens,
        map<string, int> claves);

//------------------------- Métodos evaluacion de expresiones ---------------
map<string, float> Prioridades(); //jerarquía

vector<string> Translate(vector<string> tokens); //Translate

queue<string> post(vector<string> tTokens, map<string, float> prioridades); //Creando la expresion postfijo.

float operar(queue<string> exp); //operando la expresion











//--------------------------Funcion para imprimir la matriz de tokens--------
void printTokens(vector<vector < TokenConClave >> tokens);
/////////////////////////////////////////////////////////////////////////////

class instruccion {
public:
    virtual void ejecutar() = 0;
    virtual void imprimir() = 0;
};


class Declaracion : public instruccion {
public:
    int tipo; //Conforme a la asociaci?n del map
    string identificador; //Nombre de la variable

    Declaracion(int tipo, string identificador) {
        this->tipo = tipo;
        this->identificador = identificador;
    }
    void ejecutar();

    void imprimir() {
        switch (tipo) {
            case 2:
                cout << "natural ";
                break;
            case 3:
                cout << "real ";
                break;
            case 4:
                cout << "text ";
                break;
        }
        cout << this->identificador << endl;
    }
};

class Asignacion : public instruccion {
public:
    vector<string> exp;
    string identificador;
    int tipodato;

    Asignacion(vector<string> e, string id, int tipo) {
        this->exp = e;
        this->identificador = id;
        this->tipodato = tipo;
    }
    void ejecutar();

    void imprimir() {
        cout << this->identificador << " = ";
        for (int i = 0; i<this->exp.size(); i++) {
            cout << this->exp[i] << " ";
        }
        cout << endl;
    }
    ~Asignacion(){
        cout<<"Objeto Asignacion destruido\n";
    }
};

class Imprimir : public instruccion {
public:
    vector<string> exp;
	
    Imprimir(vector<string> e) {
        this->exp = e;
    }
    void ejecutar();

    void imprimir() {
    	cout<< "EL bojeto imprimir tiene: ";
    	for(int i=0;i<this->exp.size();i++){
    		cout <<this->exp[i] <<" ";
		}

    }
};

class Leer : public instruccion {
public:
    int tipo; //Conforme a la asociación del map.
    string identificador; //nombre de la variable donde se guardará.
    string lectura;

    Leer(int t, string id) {
        this->tipo = t;
        this->identificador = id;
        this->lectura = "";
    }
    void ejecutar();

    void imprimir() {
    	cout<<"Imprimir(): ";
    	cout<<this->tipo<<" "<<this->identificador << " " << this->lectura;
    }
};

class LoopFor : public instruccion {
public:
    string identificador; //Nombre de la variable a iterar.
    int start; //Numero de inicio.
    int end; //Numero de final
    int incremento; //Incremento
    vector<instruccion*> instrucciones;

    LoopFor(string id, int i, int en, int inc) {
        this->identificador = id;
        this->start = i;
        this->end = en;
        this->incremento = inc;
    }
    void ejecutar();

    void imprimir() {
    }

    ~LoopFor() {
        for (int i = 0; i<this->instrucciones.size(); i++)delete instrucciones[i];
    }
};

class LoopWhile : public instruccion {
public:
    vector<string> exp;
    vector<instruccion*> instrucciones;

    LoopWhile(vector<string> e) {
        this->exp = e;
    }
    void ejecutar();

    void imprimir() {
    }

    ~LoopWhile() {
        for (int i = 0; i<this->instrucciones.size(); i++)delete instrucciones[i];
    }
};

class If : public instruccion {
public:
    vector<string> exp;
    vector<instruccion*> instruccionesIf;
    vector<instruccion*> instruccionesElse;

    If(vector<string> e) {
        this->exp = e;
    }

    void ejecutar();

    void imprimir() {
    }

    ~If() {
        for (int i = 0; i<this->instruccionesIf.size(); i++)delete instruccionesIf[i];
        for (int i = 0; i<this->instruccionesElse.size(); i++)delete instruccionesElse[i];
    }
};





void analizador_semantico(vector<instruccion*> ins);

vector<instruccion*> analizador_sintactico(vector<vector<TokenConClave>> m_lexico);

void show(vector<TokenConClave> tokens, int inicio, int linea, vector<instruccion*>& ins);

void declaracion(vector<TokenConClave> tokens, int inicio, int linea, vector<instruccion*>& ins);

void asignacion(vector<TokenConClave> tokens, int inicio, string ide, int linea, vector<instruccion*> &ins);

void read(vector<TokenConClave> tokens, int inicio, vector<instruccion*> &ins);

void loop_for(vector<TokenConClave> tokens, int inicio, vector<instruccion*> &ins,int linea,vector<vector<TokenConClave>>);
//loop start i in _ ends in _ by _:

void loop_while(vector<TokenConClave> tokens, int inicio, vector<instruccion*>& ins,int linea,vector<vector<TokenConClave>>);
void iff(vector<TokenConClave> tokens, int inicio, vector<instruccion*>& ins, int linea,vector<vector<TokenConClave>>);
//if(_):

//----------------------------Fin de los metodos de instrucciones-------------------

#endif	/* CLASES_INTERPRETE_H */

