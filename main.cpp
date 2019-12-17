/* 
 * File:   main.cpp
 * Author: max_l and oswalt_mosby
 *
 * Created on 10 de diciembre de 2019, 08:17 PM
 */

#include "Clases_Interprete.h"
using namespace std;

map<string, string> variables;
map<string, int> var_dty; //map para relacionar las variables con sus tipos de datos
map<string, string>::iterator it;
vector<string> instrucciones_tracker;
bool boleanos = false;
int current_line;
/////////MAIN//////////////////////////////////////////////////////////////////


/////////MAIN//////////////////////////////////////////////////////////////////

vector<vector<TokenConClave>> subMatriz(vector<vector<TokenConClave>> matriz, int linea) {
    vector<vector < TokenConClave>> submatriz;
    vector<TokenConClave> ins_linea;

    /*for(int i=linea+1;i<matriz.size()-linea;i++){
            for(int j=0;j<matriz[i].size())
       }*/

    int cont = 0;
    for (int i = linea + 1; i < matriz.size(); i++) {
        for (int j = 0; j < matriz[i].size(); j++) {

            if (matriz[i][j].getToken() != "}") {
                ins_linea.push_back(matriz[i][j]);
            } else {

                submatriz.push_back(ins_linea);
                current_line = i;
                return submatriz;
            }

        }
        submatriz.push_back(ins_linea);
        ins_linea.clear();
        cont++;
    }
    cout << "Error en una instruccion loop o if/else\n";
    exit(10);

    return submatriz;
}

//--------------------------------------------------------------------------------

float Evaluar_expresion(vector<string> raw_expresion) {
    boleanos = false;
    map<string, float> Jerarquia = Prioridades();
    vector<string> translated = Translate(raw_expresion);
    queue<string> postfijo = post(translated, Jerarquia);
    float res = operar(postfijo);
    return res;
}

map<string, float> Prioridades() { 
    map<string, float> p;
    p["("] = 4;
    p[")"] = 4;

    p["*"] = 3;
    p["/"] = 3;

    p["-"] = 2;
    p["+"] = 2;

    p[">="] = 1;
    p["<="] = 1;
    p[">"] = 1;
    p["<"] = 1;
    p["=="] = 1;

    p["!="] = 1; // is not

    p["!"] = 1.5; // not

    p["or"] = -1;
    p["&&"] = -1;
    p["||"] = -1;


    return p;
}

vector<string> Translate(vector<string> tokens) {
    vector<string> trans;
    map<string, string>::iterator it;
    string item;
	
    for (int i = 0; i < tokens.size(); i++) {
        item = tokens[i];
        try {
            if (item == "0" || item == "not" || item == "is not" || item == "or" || item == "and" || item == "is"
                    || item == "+" || item == "-" || item == "*" || item == "/" || item == "&&" || item == "||" || item == "!=" || item == "!"
                    || item == "==" || item == ">" || item == ">=" || item == "<" || item == "<=" || item == "(" || item == ")" || stof(item)) {
                //cout<<"El item es un numero o un operador\n";
                trans.push_back(item);
            }
        } catch (exception& e) {
            it = variables.find(item);

            if (it != variables.end()) {
                //cout<<"El item es una variable.\n";
                trans.push_back(variables[item]);
            } else {
                cout << "La variable no existe.\n";
                exit(10);

            }
        }

    }
    /*cout<<"Translate: ";
    for(auto i:trans){
        cout<<i;
    }
    */
    return trans;
}

queue<string> post(vector<string> tTokens, map<string, float> prioridades) { 
    stack<string> p;
    queue<string> q;
    //printTokens(tTokens);

    for (int i = 0; i < tTokens.size(); i++) {
        try {
            if (stoi(tTokens[i]) || stof(tTokens[i]) || tTokens[i] == "0")
                q.push(tTokens[i]);

        } catch (exception& e) {

            if (tTokens[i] == "(") {
                p.push(tTokens[i]);

            } else if (tTokens[i] == ")") {
                while (!p.empty() && p.top() != "(") {
                    q.push(p.top());
                    p.pop();
                }
                p.pop();

            } else {
                while (!p.empty() && prioridades[p.top()] >= prioridades[tTokens[i]] && p.top() != "(") {
                    q.push(p.top());
                    p.pop();
                }
                p.push(tTokens[i]);
            }
        }
    } //Fin del for

    while (!p.empty()) {
        q.push(p.top());
        p.pop();
    }
    //printQueue(q);
    return q;
}

float operar(queue<string> exp) {
    stack<string> espera;
    string item = "";
    float cuenta = 0;

    while (!exp.empty()) {
        item = exp.front();
        exp.pop();
        

        try {
            if (item == "0" || stof(item)) {
                espera.push(item);
            }
        } catch (exception& e) {
            //------------------------------------------------------ + ---------------------------------------
            if (item == "+" && espera.size() >= 2) {
                float n2 = stof(espera.top());
                espera.pop();
                float n1 = stof(espera.top());
                espera.pop();
                cuenta = n1 + n2;
                espera.push(to_string(cuenta));
                //------------------------------------------------------ - ---------------------------------------
            } else if (item == "-" && espera.size() >= 2) {
                float n2 = stof(espera.top());
                espera.pop();
                float n1 = stof(espera.top());
                espera.pop();
                cuenta = n1 - n2;
                espera.push(to_string(cuenta));
                //------------------------------------------------------ * ---------------------------------------	
            } else if (item == "*" && espera.size() >= 2) {
                float n2 = stof(espera.top());
                espera.pop();
                float n1 = stof(espera.top());
                espera.pop();
                cuenta = n1*n2;
                espera.push(to_string(cuenta));
                //------------------------------------------------------ / ---------------------------------------
            } else if (item == "/" && espera.size() >= 2) {
                float n2 = stof(espera.top());
                espera.pop();
                float n1 = stof(espera.top());
                espera.pop();
                try { //try por si es n1 / 0;
                    cuenta = n1 / n2;
                    espera.push(to_string(cuenta));
                } catch (exception& e) {
                    cout << "Error en la expresion\n";
                    exit(10);
                    //por devolver algo, pero me baso en la variable errors para saber si estÃ¡ bien.
                }
                //------------------------------------------- && || > >= < <=---------------------------------------		
            } else if ((item == "!" || item == "not") && espera.size() >= 1) {
                boleanos = true;
                float n1;
                if (espera.top() == "true") {
                    espera.pop();
                    espera.push("false");
                } else if (espera.top() == "false") {
                    espera.pop();
                    espera.push("true");
                } else {
                    n1 = stof(espera.top());
                    espera.pop();
                    espera.push(to_string(!n1));
                }



            } else if (((item == "&&") || (item == "||") || (item == ">") || (item == ">=") || (item == "<") || (item == "<=") || (item == "==") || (item == "!=") || item == "and" || item == "or" || item == "not" || item == "is not" || item == "is") && espera.size() >= 2) {
                boleanos = true;
                float n1, n2;
                if (espera.top() == "true") {
                    n2 = 1;
                    espera.pop();
                } else if (espera.top() == "false") {
                    n2 = 0;
                    espera.pop();
                } else {
                    n2 = stof(espera.top());
                    espera.pop();
                }

                if (espera.top() == "true") {
                    n1 = 1;
                    espera.pop();

                } else if (espera.top() == "false") {
                    n1 = 0;
                    espera.pop();
                } else {
                    n1 = stof(espera.top());
                    espera.pop();
                }

                try {
                    if (item == "&&" || item == "and") {
                        if (n1 && n2) {
                            espera.push("true");
                        } else {
                            espera.push("false");
                        }

                    } else if (item == "||" || item == "or") {
                        if (n1 || n2) {
                            espera.push("true");
                        } else {
                            espera.push("false");
                        }

                    } else if (item == ">") {
                        if (n1 > n2) {
                            espera.push("true");
                        } else {
                            espera.push("false");
                        }

                    } else if (item == ">=") {
                        if (n1 >= n2) {
                            espera.push("true");
                        } else {
                            espera.push("false");
                        }

                    } else if (item == "<") {
                        if (n1 < n2) {
                            espera.push("true");
                        } else {
                            espera.push("false");
                        }

                    } else if (item == "<=") {
                        if (n1 <= n2) {
                            espera.push("true");
                        } else {
                            espera.push("false");
                        }
                    } else if (item == "==" || item == "is") {
                        if (n1 == n2) {
                            espera.push("true");
                        } else {
                            espera.push("false");
                        }
                    } else if (item == "!=" || item == "is not") {
                        if (n1 != n2) {
                            espera.push("true");
                        } else {
                            espera.push("false");
                        }
                    }

                } catch (exception& e) {
                    cout << "Error en la expresion\n";
                    exit(10); //por devolver algo, pero me baso en la variable errors para saber si estÃ¡ bien.	
                }

                //--------------------------------------------- Caso invÃ¡lido ---------------------------------------		
            } else {
                cout << "Error en la expresion\n";
                //cout<<"Espera: "<<espera.size();
                exit(10);
            }
        } //Catch
    } //While
    if (espera.size() > 1) {
        cout << "Error en la expresion\n";
        exit(10);
    } else if (espera.top() == "true") {
        return 1;
    } else if (espera.top() == "false") {
        return 0;
    } else {
        return stof(espera.top());
    }

}


//_---------------------------------- Analizador: Lexico, Semantico y Sintactico ----------------------------------
vector<vector<TokenConClave>> analizador_lexico(map<string, int> claves, string filename) {
	
	vector <vector < string>> matrizTokens = getTokens(filename);
    vector< vector < TokenConClave>>tokensConClave = relacionaTokenConClave(matrizTokens, claves);
    return tokensConClave;
}

void analizador_semantico(vector<instruccion*> ins) {
    for (int i = 0; i < ins.size(); i++) {
        ins[i]->ejecutar();
    }

}

vector<instruccion*> analizador_sintactico(vector<vector < TokenConClave >> m_lexico) {
    vector<instruccion*> principal;

    for (int i = 0; i < m_lexico.size(); i++) {
        for (int j = 0; j < m_lexico[i].size(); j++) {
            //DeclaraciÃ³n.
            if ((m_lexico[i][j].getClave() == 2 || m_lexico[i][j].getClave() == 3 ||
                    m_lexico[i][j].getClave() == 4) && (m_lexico[i].size() - j) == 3) {
                declaracion(m_lexico[i], j, i, principal);
                break;
            }//AsignaciÃ³n.
            else if (m_lexico[i][j].getClave() == 1 && (m_lexico[i].size() - j >= 2)) {
                asignacion(m_lexico[i], j, m_lexico[i][j].getToken(), i, principal);
                break;
            }//Show
            else if (m_lexico[i][j].getClave() == 14) {
                show(m_lexico[i], j, i, principal);
                break;
            }//read.
            else if (m_lexico[i][j].getClave() == 15) {
                read(m_lexico[i], j, principal);
                break;
            }//Loop
            else if (m_lexico[i][j].getClave() == 8) {
                if ((m_lexico[i].size() - j > 1) && m_lexico[i][j + 1].getClave() == 9) {
                    loop_while(m_lexico[i], j, principal, i, m_lexico);
                    i = current_line;
                    break;

                } else if ((m_lexico[i].size() - j) > 1 && m_lexico[i][j + 1].getClave() == 10) {
                    loop_for(m_lexico[i], j, principal, i, m_lexico);
                    i = current_line;
                    break;
                }
            }//IF-ELSE
            else if (m_lexico[i][j].getClave() == 5) {
                iff(m_lexico[i], j, principal, i, m_lexico);
                i = current_line;
                break;
            } else {
                cout << "Error en la linea: " << i << endl;
                exit(10);
                break;
            }
        }

    }

    return principal;
}



//--------------------------------------   EJECUCIÓN -------------------------------------------
void Declaracion::ejecutar() {
	
    it = variables.find(this->identificador);
    if (it != variables.end()) {
        cout << "La variable ya fue declarada anteriormente.\n";
        exit(10);
    } else {
        if (this->tipo == 2 || this->tipo == 3) { //Si es un tipo de dato numerico.
            variables[this->identificador] = "0n";
            var_dty[this->identificador]=this->tipo;
            

        } else if (this->tipo == 4) {
        	
            variables[this->identificador] = "t";
            var_dty[this->identificador]=this->tipo;
        }
    }
}

void Asignacion::ejecutar() {
    if (this->exp.size() == 1 && this->tipodato == 4) {
        variables[this->identificador] = this->exp[0];
        
    } else if (this->tipodato == 2 || this->tipodato == 3) {
        float res = Evaluar_expresion(this->exp);
        variables[this->identificador] = to_string(res);
    } else {
       
        cout << "Error al ejecutar la asignacion.\n";
        exit(10);
    }
    
}

void Imprimir::ejecutar() {
	//cout<<"Imprimir ejecutado"<<endl;
	if(this->exp.size()>1){
		float res = Evaluar_expresion(this->exp);
        cout << res << endl;		
	} else if(this->exp.size()==1){
		try{
			if(stof(this->exp[0])){
				float res = Evaluar_expresion(this->exp);
        		cout << res <<endl;
			}
			
			
		} catch(exception& e){
			it = variables.find(this->exp[0]);
			if(it!=variables.end()){
				string valor = variables[this->exp[0]];
				if(valor != "t" || valor != "0n"){
					cout<<valor << endl;
				} else {
					float res = Evaluar_expresion(this->exp);
        			cout << res <<endl;	
				}
			
			} else {
				cout<<this->exp[0] << endl;
			}
		}
	}
}

void Leer::ejecutar() {
	
    it = variables.find(this->identificador);
    if (it != variables.end()) {
    
        if(this->tipo == var_dty[this->identificador]){ //Que coincida con el que le enviamos con el que se le delcaro
        	cin>>this->lectura;
			variables[this->identificador] = this->lectura;
         
		} else {
			cout<<"Error en los tipos de datos de lectura"<<endl;
			exit(10);
		}
        
    } else {
        cout << "La variable no existe.\n";
        exit(10);
    }
}

void LoopFor::ejecutar() {

	for (int i = this->start; i<this->end; i += this->incremento) {
		variables[this->identificador] = to_string(i);
		for(int j=0;j<this->instrucciones.size();j++){
			this->instrucciones[j]->ejecutar();
		}
	
    }
}

void LoopWhile::ejecutar() {
    float res = Evaluar_expresion(this->exp);
        while(res){
        	for (int i = 0; i<this->instrucciones.size(); i++) {
            this->instrucciones[i]->ejecutar();	
            res = Evaluar_expresion(this->exp);
		}
    }
    
}

void If::ejecutar() {
	float res = Evaluar_expresion(this->exp);
    if (res) {
        for (int i = 0; i<this->instruccionesIf.size(); i++) {
            this->instruccionesIf[i]->ejecutar();
        }
    } else {
        if (this->instruccionesElse.size() > 0) {
            for (int i = 0; i<this->instruccionesIf.size(); i++) {
                this->instruccionesElse[i]->ejecutar();
            }
        }
    }
}



//-----------------------------------  VERIFICACION DE LAS ESTRUCTURAS --------------------------
void show(vector<TokenConClave> tokens, int inicio, int linea, vector<instruccion*> &instrucciones) {
    vector<string> exp;
    if (tokens.size() - inicio >= 4) {
    //show(3+5);
        
        if (tokens[inicio + 1].getToken() == "(" && tokens[tokens.size() - 2].getToken() == ")" && tokens[tokens.size() - 1].getToken() == ";") {//verificamos la estructura del show
            if (tokens.size() == 5 && (tokens[inicio + 2].getClave() == 22 || tokens[inicio + 2].getClave() == 4)) {
                exp.push_back(tokens[inicio + 2].getToken());
       
                instrucciones.push_back(new Imprimir(exp));
               	instrucciones_tracker.push_back("Objeto Imprimir");
                
            } else {
                for (int i = 2; i < tokens.size()-2; i++){
                	exp.push_back(tokens[i].getToken());
				}
			
        
                instrucciones.push_back(new Imprimir(exp));
                instrucciones_tracker.push_back("Objeto Imprimir");
            
            }
        } else {
            //cout << "Error de sintaxis en la linea: " << linea << endl;
            exit(10);
        }
    } else {
        //cout << "Error de sintaxis en la linea: " << linea << endl;
        exit(10);
    }
}

void declaracion(vector<TokenConClave> tokens, int inicio, int linea, vector<instruccion*> &instrucciones) {
    //natural hola ;
    if (tokens[inicio + 1].getClave() == 1 && tokens[inicio + 2].getToken() == ";") {
    	
        instrucciones.push_back(new Declaracion(tokens[inicio].getClave(), tokens[inicio + 1].getToken()));
		var_dty[tokens[inicio+1].getToken()] = tokens[inicio].getClave();
		instrucciones_tracker.push_back("Objeto Declaracion");
		
    } else {
        cout << "Error en la declaracion." << endl;
        exit(10);
    }

}

void asignacion(vector<TokenConClave> tokens, int inicio, string ide, int linea, vector<instruccion*> &instrucciones) {

    if (tokens[inicio + 1].getToken() == "=" && tokens[tokens.size() - 1].getToken() == ";") {
        vector<string> expresion;

        for (int i = inicio + 2; i < tokens.size() - 1; i++) {
            expresion.push_back(tokens[i].getToken());
        }
        
        instrucciones.push_back(new Asignacion(expresion, tokens[inicio].getToken(),var_dty[tokens[inicio].getToken()]));
        instrucciones_tracker.push_back("Objeto Asignacion");
        

    } else {
        cout << "Eror en la asignacion. \n";
        exit(10);
    }

}

void read(vector<TokenConClave> tokens, int inicio, vector<instruccion*> &instrucciones) {
    //read(natural,var);
    if (tokens.size() == 7 && tokens[inicio + 1].getToken() == "(" && tokens[inicio + 3].getToken() == "," && tokens[inicio + 5].getToken() == ")" && tokens[inicio + 6].getToken() == ";") {
        if (tokens[inicio + 2].getClave() == 2 || tokens[inicio + 2].getClave() == 3 || tokens[inicio + 2].getClave() == 4) {

            instrucciones.push_back(new Leer(tokens[inicio + 2].getClave(), tokens[inicio + 4].getToken()));
           	instrucciones_tracker.push_back("Objeto Read");

        } else {
            cout << "Tipo de dato incorrecto\n";
            exit(10);
        }

    } else {
        cout << "Error en la declaracion del read. Error de estructuracion\n";
        exit(10);
    }

}

void loop_for(vector<TokenConClave> tokens, int inicio, vector<instruccion*> &ins, int linea, vector<vector<TokenConClave>> matriz_completa) {
    vector<string>expresion;
    if (tokens.size() - inicio >= 11) { //si existe la suficiente cantidad de tokens que conforman al for
        if (tokens[inicio + 2].getClave() == 1 && tokens[inicio + 3].getToken() == "in" && tokens[tokens.size() - 6].getToken() == "ends" &&
                tokens[tokens.size() - 5].getToken() == "in" &&
                tokens[tokens.size() - 3].getToken() == "by"
                && tokens[tokens.size() - 1].getToken() == "{") {

            vector<vector<TokenConClave>> submatriz = subMatriz(matriz_completa, linea);
            //loop start i in _ ends in _ by _ {
            try {
                string identifier = tokens[inicio + 2].getToken();
                float start = stof(tokens[inicio + 4].getToken());
                float end = stof(tokens[inicio + 7].getToken());
                float incremento = stof(tokens[inicio + 9].getToken());


                LoopFor* f = new LoopFor(identifier, start, end, incremento);
                f->instrucciones = analizador_sintactico(submatriz);
                ins.push_back(f);
                instrucciones_tracker.push_back("Objeto For");


            } catch (exception& e) {
                cout << "Error en la linea" << linea + 1 << endl;
                exit(10);
            }


        } else {
            cout << "Error de sintaxis en la linea: " << linea << endl;
            exit(10);
            return;
        }
    } else {
        cout << "Error de sintaxis en la linea:" << linea << endl;
        exit(10);
        return;
    }
}

void loop_while(vector<TokenConClave> tokens, int inicio, vector<instruccion*> &ins,int linea, vector<vector<TokenConClave>> matriz_completa) {
    // loop  while  ( 1 )  :
    //Minimum size: 6
    if (tokens.size() - inicio >= 6) {
        
        if (tokens[inicio + 2].getToken() == "(" && tokens[tokens.size() - 2].getToken() == ")"
                && tokens[tokens.size() - 1].getToken() == "{") {
            if (tokens.size() == 6 && (tokens[inicio + 3].getClave() == 22 || tokens[inicio + 3].getClave() == 4)) {
                cout << "Error de expresion en la linea: " << linea << endl;
                exit(10);
                return;
            }
            //loop while( 1 < 3 ) {
            //vector<vector<TokenConClave>> submatriz = subMatriz(matriz_completa,linea);
            vector<string> expresion;
            
            for (int i = inicio + 3; i < tokens.size() - 2; i++) {//recorro la linea de declaracion del loop
                expresion.push_back(tokens[i].getToken());
            }
            vector<vector < TokenConClave>> submatriz = subMatriz(matriz_completa, linea);
            
            
            LoopWhile* w = new LoopWhile(expresion);
            w->instrucciones = analizador_sintactico(submatriz);

            ins.push_back(w);
            instrucciones_tracker.push_back("Objeto While");


        } else {
            cout << "Error de sintaxis en la linea: jeje " << linea << endl;
            exit(10);
            return;
        }
    } else {
        cout << "Error de sintaxis en la linea: " << linea << endl;
        exit(10);
        return;
    }

}

void iff(vector<TokenConClave> tokens, int inicio, vector<instruccion*> &ins, int linea, vector<vector<TokenConClave>> matriz_completa) {
    if (tokens.size() - inicio >= 5) {
        if (tokens[inicio + 1].getToken() == "(" && tokens[inicio + 2].getClave() != 22 &&
                tokens[tokens.size() - 2].getToken() == ")" && tokens[tokens.size() - 1].getToken() == "{") {

            vector<string> expresion;
            for (int i = inicio + 2; i < tokens.size() - 2; i++) {//recorro la linea de declaracion del loop
                expresion.push_back(tokens[i].getToken());
            }
            
            vector<vector < TokenConClave>> submatriz = subMatriz(matriz_completa, linea);

            If* si = new If(expresion);
            si->instruccionesIf = analizador_sintactico(submatriz);
            

            //current esta en la la llave de cierre if.
            if (current_line < matriz_completa.size() - 1) {
                for (int i = 0; i < matriz_completa[current_line + 1].size(); i++) {
                    if (matriz_completa[current_line + 1][i].getToken() == "else") {
                        vector<vector < TokenConClave>> submatriz_else = subMatriz(matriz_completa, current_line + 1);
                        si->instruccionesElse = analizador_sintactico(submatriz_else);
                        instrucciones_tracker.push_back("Objeto Else");

                    }
                }

            }

			instrucciones_tracker.push_back("Objeto If");
            ins.push_back(si);
           
        }
    } else {
        cout << "Error de sintaxis." << endl;
        exit(10);
        return;
    }

}






map<string, int> generaClaves() {
    map<string, int> mapa;
    mapa.insert(pair<string, int>("identifier", 1));
    mapa.insert(pair<string, int>("natural", 2));
    mapa.insert(pair<string, int>("real", 3));
    mapa.insert(pair<string, int>("text", 4));
    mapa.insert(pair<string, int>("if", 5));
    mapa.insert(pair<string, int>("else", 6));
    mapa.insert(pair<string, int>("else if", 7));
    mapa.insert(pair<string, int>("loop", 8)); //for
    mapa.insert(pair<string, int>("while", 9));
    mapa.insert(pair<string, int>("start", 10));
    mapa.insert(pair<string, int>("in", 11));
    mapa.insert(pair<string, int>("ends", 12));
    mapa.insert(pair<string, int>("by", 13));
    mapa.insert(pair<string, int>("show", 14));
    mapa.insert(pair<string, int>("read", 15));
    mapa.insert(pair<string, int>("entero", 20));
    mapa.insert(pair<string, int>("flotante", 21));
    mapa.insert(pair<string, int>("cadena", 22));
    mapa.insert(pair<string, int>("+", 30));
    mapa.insert(pair<string, int>("-", 31));
    mapa.insert(pair<string, int>("*", 32));
    mapa.insert(pair<string, int>("/", 33));
    mapa.insert(pair<string, int>(">", 34));
    mapa.insert(pair<string, int>("<", 35));
    mapa.insert(pair<string, int>(">=", 36));
    mapa.insert(pair<string, int>("<=", 37));
    mapa.insert(pair<string, int>("is", 38));
    mapa.insert(pair<string, int>("!=", 39));
    mapa.insert(pair<string, int>(";", 40));
    mapa.insert(pair<string, int>(":", 41));
    mapa.insert(pair<string, int>("and", 42));
    mapa.insert(pair<string, int>("or", 43));
    mapa.insert(pair<string, int>("not", 44));
    mapa.insert(pair<string, int>("=", 45));
    mapa.insert(pair<string, int>("->", 46));
    mapa.insert(pair<string, int>("(", 47));
    mapa.insert(pair<string, int>(")", 48));
    mapa.insert(pair<string, int>(",", 49));
    mapa.insert(pair<string, int>("True", 50));
    mapa.insert(pair<string, int>("False", 51));
    mapa.insert(pair<string, int>("\t", 52));
    mapa.insert(pair<string, int>("{", 53));
    mapa.insert(pair<string, int>("}", 54));
    mapa.insert(pair<string, int>("==", 55));
    return mapa;
}
bool is_float(string s) {
    int dot_counter = 0;
    for (auto i : s) {
        if (i == '.') {
            dot_counter++;
        }
        if (dot_counter < 2 && (!isdigit(i) && i != '.')) {
            return false;
        }
    }

    if (dot_counter > 1)return false;
    return true;
}

bool is_int(string s) {
    for (auto i : s) {
        if (!isdigit(i))return false;
    }
    return true;
}

vector <vector <string>> getTokens(string filename) {
    vector<vector < string>>matrizTokens;
    string line;
    ifstream myfile(filename);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            matrizTokens.push_back(separaToken(line));
        }
        myfile.close();
    } else cout << "Unable to open file";
    return matrizTokens;
}

vector<string> separaToken(string s){
    vector<string> tokens;
    bool cadena = 0;
    string aux = "";
    for (int i = 0; i < s.size(); i++) {//recorremos la cadena
		if(s[i]=='=' && s[i+1]=='='){
			if(aux!=""){
				tokens.push_back(aux);
				aux="";
			}
			tokens.push_back("==");	
			i++;
			continue;
		}
		
        if (s[i] == '"') {
            if (!cadena) {
                cadena = 1;
                aux += "'";
                continue;
            } else if (cadena) {
                cadena = 0;
                tokens.push_back(aux);
                aux = "";
                continue;
            }
        }
        if (cadena) {
            aux += s[i];
        } else {

            if (isalpha(s[i]) || isdigit(s[i]) || s[i] == '.') {
                aux += s[i];
            } else {
                if (!aux.empty()) {
                    tokens.push_back(aux);
                }
                aux = "";
                if (s[i] == '!' && s[i + 1] == '=') {
                    tokens.push_back("!=");
                    i++;
                    continue;
                }
                if (s[i] == '&' && s[i + 1] == '&') {
                    tokens.push_back("&&");
                    i++;
                    continue;
                }
                if (s[i] == '|' && s[i + 1] == '|') {
                    tokens.push_back("||");
                    i++;
                    continue;
                }
                if (s[i] == '>' && s[i + 1] == '=') {
                    tokens.push_back(">=");
                    i++;
                    continue;
                } else if (s[i] == '<' && s[i + 1] == '=') {
                    tokens.push_back("<=");
                    i++;
                    continue;
                } else if (s[i] == '-' && s[i + 1] == '>') {
                    tokens.push_back("->");
                    i++;
                    continue;
                } else if (s[i] != ' ') {

                    aux += s[i];
                    tokens.push_back(aux);
                    aux = "";
                } /*else if(s[i] == '=' && s[i+1]=='='){
                	tokens.push_back("==");
                	i++;
                	continue;
				}*/

            }
        }
    }


    return tokens;
}


vector< vector<TokenConClave>> relacionaTokenConClave(vector <vector< string>> matrizTokens,map<string, int> claves) {
    vector<TokenConClave> renglon;
    vector< vector < TokenConClave>>tokensConClave;
    map<string, int>::iterator it;
    for (int i = 0; i < matrizTokens.size(); ++i) {

        for (int j = 0; j < matrizTokens[i].size(); j++) {

            it = claves.find(matrizTokens[i][j]);
            if (it != claves.end()) {
                renglon.push_back(TokenConClave(matrizTokens[i][j],
                        claves[matrizTokens[i][j]]));
            } else {
                if (is_int(matrizTokens[i][j])) {
                    renglon.push_back(TokenConClave(matrizTokens[i][j],
                            claves["entero"]));
                } else if (is_float(matrizTokens[i][j])) {
                    renglon.push_back(TokenConClave(matrizTokens[i][j],
                            claves["flotante"]));
                } else if (matrizTokens[i][j][0] == 39) {
                    matrizTokens[i][j].erase(0, 1);
                    renglon.push_back(TokenConClave(matrizTokens[i][j],
                            claves["cadena"]));
                } else if (isalpha(matrizTokens[i][j][0])) {
                    renglon.push_back(TokenConClave(matrizTokens[i][j],
                            claves["identifier"]));
                } else {
                    cout << "Error en lexico en la linea: " << i 	 << "." <<
                            "En el token: "<<matrizTokens[i][j]<<endl;
                    exit(EXIT_FAILURE);
                }
            }

        }
        tokensConClave.push_back(renglon);
        renglon.clear();
    }

    return tokensConClave;
}

void printTokens(vector<vector < TokenConClave >> tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        for (int j = 0; j < tokens[i].size(); j++) {

            cout << "[ " << tokens[i][j].to_string() << "]";
        }
        cout << endl;
    }
}

int main(int argc, char** argv) {
  
    map<string, int> claves_analizador_lexico = generaClaves();
    string filename = "example.txt";
    vector<vector < TokenConClave>> m_lexico = analizador_lexico(claves_analizador_lexico, filename);
   
	vector<instruccion*>ins = analizador_sintactico(m_lexico);

	cout<<"Instrucciones: "<<endl;
	for(int i=0;i<instrucciones_tracker.size();i++){
		cout<<i+1<<".- "<<instrucciones_tracker[i]<<endl;
	}

	cout<<"Ejecutando......."<<endl;
    analizador_semantico(ins);

    return 0;
}
