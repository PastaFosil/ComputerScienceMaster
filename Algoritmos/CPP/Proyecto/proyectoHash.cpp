/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 8 C++
 * 
 * Maneja tablas hash con bases de datos de libros y realiza
 * operaciones de conjuntos sobre ellas
************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

void print_vector(vector<string> v){
    for (int i=0;i<v.size();i++)
        cout << v[i] << " ";
    cout << endl;
}

class hash_table {
    private:
        unsigned long size = 100;
        string database;

        struct Node { // Estructura de un nodo de la tabla hash
            string title;
            string genre;
        };
        vector<Node> *table;

        struct Genre{ // Estructura de nodo para lista de generos
            string name;
            int count;
        };
        vector<Genre> genres;
        
        unsigned long hash_function(string str, unsigned long size){
            unsigned long hash = 5381;
            for (char c : str)
                hash = ((hash<<5)+hash)+static_cast<unsigned long>(c);
            return hash%size;
        }
    public:

        // Inicializa una tabla hash con un tamaño de 100
        hash_table(){
            table = new vector<Node>[size];
        }

        // Destructor
        ~hash_table(){
            delete[] table;
        }

        // Imprime la tabla hash
        void printTable(){
            int col_size = 80;
            cout << "Tabla de libros" << endl;
            cout << "----------------" << endl;
            cout << setw(6) << "Indice" << setw(col_size) << "Titulo" << setw(col_size/2) << "Genero" << endl << endl;
            for (int i=0;i<size;i++){
                for (int j=0;j<table[i].size();j++){
                    if (j==0)
                        cout << setw(6) << i << setw(col_size) << table[i][j].title << setw(col_size/2) << table[i][j].genre << endl;
                    else cout << setw(6) << " " << setw(col_size) << table[i][j].title << setw(col_size/2) << table[i][j].genre << endl;
                }
            }
        }

        // Inserta un elemento en la tabla hash
        void insert(string title, string genre){
            unsigned long index = hash_function(title, size);
            bool exists = false;
            Node node;
            node.title = title;
            node.genre = genre;
            table[index].push_back(node);
            int num_genres = genres.size();
            for (int i=0;i<num_genres;i++){
                if (genres[i].name==genre){
                    genres[i].count++;
                    exists = true;
                    break;
                }
            }
            if (!exists){
                Genre g;
                g.name = genre;
                g.count = 1;
                genres.push_back(g);
            }
        }

        // LLena la tabla hash con los datos de un archivo
        int fill_hash(string filename){
            ifstream file(filename);
            if (!file.is_open()){
                cout << "ERROR ABRIENDO EL ARCHIVO\n";
                return -1;
            }
            string line;
            while (getline(file, line)){
                int start = 7, end = start;
                while (line[end]!=';')
                    end++;
                string title = line.substr(start, end-start);
                start = end+8;
                string genre = line.substr(start, line.length()-start);
                insert(title, genre);
            }
            file.close();
            database = filename;
            return 0;
        }

        // Calcula el histograma de la tabla hash y lo guarda en un archivo txt
        void histogram(){
            cout << "Archivo guardado en: hist_"+database << endl;
            ofstream file("hist_"+database);
            if (!file.is_open()){
                cout << "ERROR CREANDO EL ARCHIVO\n";
                return;
            }
            int max = 0;
            for (int i=0;i<size;i++)
                if (table[i].size()>max)
                    max = table[i].size();
            file << max << endl;
            for (int i=0;i<size;i++)
                file << i << " " << table[i].size() << endl;
            file.close();
        }

        /* 
            Busca el elemento con los datos especificados, si se encuentra,
            devuelve el indice donde se ubica, de lo contrario, devuelve -1
        */
        int search(string title, string genre){
            unsigned long index = hash_function(title, size);
            for (int i=0;i<table[index].size();i++){
                if (table[index][i].title==title && table[index][i].genre==genre)
                    return index;
            }
            return -1;
        }

        // Imprime la interseccion de dos tablas hash
        void intersection(hash_table &other){
            int col_size = 80;
            cout << setw(col_size) << "Titulo" << setw(col_size/2) << "Genero" << endl << endl;
            for (int i=0;i<size;i++){
                for (int j=0;j<table[i].size();j++){
                    if (other.search(table[i][j].title, table[i][j].genre)!=-1)
                        cout << setw(col_size) << table[i][j].title << setw(col_size/2) << table[i][j].genre << endl;
                }
            }
        }

        // Imprime la union de dos tablas hash
        void union_hash(hash_table &other){
            int col_size = 80;
            hash_table union_table;
            for (int i=0;i<size;i++){
                for (int j=0;j<table[i].size();j++){
                    string title = table[i][j].title;
                    string genre = table[i][j].genre;
                    if (union_table.search(title, genre)==-1)
                        union_table.insert(title, genre);
                }
                for (int j=0;j<other.table[i].size();j++){
                    string title = other.table[i][j].title;
                    string genre = other.table[i][j].genre;
                    if (union_table.search(title, genre)==-1)
                        union_table.insert(title, genre);
                }
            }
            union_table.printTable();
        }

        // Imprime la diferencia entre dos tablas hash
        void difference(hash_table &other){
            int col_size = 80;
            cout << setw(col_size) << "Titulo" << setw(col_size/2) << "Genero" << endl << endl;
            for (int i=0;i<size;i++){
                for (int j=0;j<table[i].size();j++){
                    if (other.search(table[i][j].title, table[i][j].genre)==-1)
                        cout << setw(col_size) << table[i][j].title << setw(col_size/2) << table[i][j].genre << endl;
                }
            }
            cout << endl << endl << "Ocurrencias de generos" << endl;
            cout << setw(col_size/2) << "Genero" << setw(col_size/2) << database.substr(0, database.length()-4) << setw(col_size/2) << other.database.substr(0, other.database.length()-4) << endl << endl;
            int num_genres_this = genres.size(), num_genres_other = other.genres.size();
            int i;
            for (i=0;i<num_genres_this;i++){
                int other_count = -1;
                for (int j=0;j<num_genres_other;j++){
                    if (genres[i].name==other.genres[j].name){
                        other_count = other.genres[j].count;
                        break;
                    }
                }
                cout << setw(col_size/2) << genres[i].name << setw(col_size/2) << genres[i].count;
                if (other_count==-1)
                    cout << setw(col_size/2) << "---" << endl;
                else
                    cout << setw(col_size/2) << other_count << endl;
            }
            if (i<num_genres_other){
                for (i;i<num_genres_other;i++)
                    cout << setw(col_size/2) << other.genres[i].name << setw(col_size/2) << "---" << setw(col_size/2) << other.genres[i].count << endl;
            }
        }

        // Imprime la diferencia simetrica entre dos tablas hash
        void symmetric_difference(hash_table &other){
            int col_size = 80;
            cout << setw(col_size) << "Titulo" << setw(col_size/2) << "Genero" << endl << endl;
            for (int i=0;i<size;i++){
                for (int j=0;j<table[i].size();j++){
                    if (other.search(table[i][j].title, table[i][j].genre)==-1)
                        cout << setw(col_size) << table[i][j].title << setw(col_size/2) << table[i][j].genre << endl;
                }
                for (int j=0;j<other.table[i].size();j++){
                    if (search(other.table[i][j].title, other.table[i][j].genre)==-1)
                        cout << setw(col_size) << other.table[i][j].title << setw(col_size/2) << other.table[i][j].genre << endl;
                }
            }
        }


};

bool is_name(char c, char names[], int num_names){
    for (int i=0;i<num_names;i++)
        if (c==names[i])
            return true;
    return false;
}

bool is_operator(char c){
    return c=='u' || c=='n' || c=='-' || c=='s';
}

vector <string> infija_a_posfija(string expresion, char names[], int num_names){
    vector <string> posfija;
    vector <string> pila;
    int i = 0;
    string str = "";
    while (expresion[i]!='\0'){
        if (is_name(expresion[i],names,num_names)){
            posfija.push_back(str+expresion[i]);
        } else if (expresion[i]=='(' || expresion[i]=='[' || expresion[i]=='{')
            pila.push_back(str+expresion[i]);
        else if (expresion[i]==')' || expresion[i]==']' || expresion[i]=='}'){
            if (expresion[i]==')'){
                while (pila.back()!="(" && !pila.empty()){
                    posfija.push_back(pila.back());
                    pila.pop_back();
                }
                if (pila.empty()){
                    cout << "ERROR: PARENTESIS NO BALANCEADOS\n";
                    posfija.resize(0);
                    return posfija;
                } else pila.pop_back();
            } else if (expresion[i]==']'){
                while (pila.back()!="[" && !pila.empty()){
                    posfija.push_back(pila.back());
                    pila.pop_back();
                }
                if (pila.empty()){
                    cout << "ERROR: PARENTESIS NO BALANCEADOS\n";
                    posfija.resize(0);
                    return posfija;
                } else pila.pop_back();
            } else if (expresion[i]=='}'){
                while (pila.back()!="{" && !pila.empty()){
                    posfija.push_back(pila.back());
                    pila.pop_back();
                }
                if (pila.empty()){
                    cout << "ERROR: PARENTESIS NO BALANCEADOS\n";
                    posfija.resize(0);
                    return posfija;
                } else pila.pop_back();
            }
        } else if (is_operator(expresion[i])){
            while (!pila.empty() && pila.back()!="(" && pila.back()!="[" && pila.back()!="{"){
                posfija.push_back(pila.back());
                pila.pop_back();
            }
            pila.push_back(str+expresion[i]);
        }
        i++;
    }
    while (!pila.empty()){
        posfija.push_back(pila.back());
        pila.pop_back();
    }
    return posfija;
}

vector<string> posfija_a_pila(string expresion, char names[], int num_names){
    vector<string> pila;
    int i = 0;
    string str = "";
    while (expresion[i]!='\0'){
        if (is_name(expresion[i],names,num_names) || is_operator(expresion[i]))
            pila.push_back(str+expresion[i]);
        i++;
    }
    
    return pila;
}

vector<string> prefija_a_posfija(string expresion, char names[], int num_names){
    vector<string> posfija, pila;
    int i = 0, count = 0;
    while (expresion[i]!='\0'){
        if (is_name(expresion[i],names,num_names)){
            posfija.push_back(string(1,expresion[i]));
            count++;
        } else if (is_operator(expresion[i])){
            pila.push_back(string(1,expresion[i]));
        }
        if (count==2 && !pila.empty()){
            posfija.push_back(pila.back());
            pila.pop_back();
            count--;
        }
        i++;
    }
    while (!pila.empty()){
        posfija.push_back(pila.back());
        pila.pop_back();
    }

    return posfija;
}

/* 
    Devuelve el tipo de notacion de la expresion ingresada:
    0: prefija. 1: infija. 2: posfija
*/
int which_fix(string expresion, char names[], int num_names){
    if (is_operator(expresion[0]))
        return 0;
    else if (is_operator(expresion.length()-1))
        return 2;
    else return 1;
}

int main(int argc, char **argv){
    if (argc<3){
        cout << "ERROR: DEBE INGRESAR MAS ARCHIVOS\n";
        return -1;
    }
    vector<hash_table> tables;
    int num_tables = argc-1;
    tables.resize(num_tables);
    char names[num_tables];
    cout << "\n\nLas tablas se identificaran alfabeticamente con una letra mayuscula en el orden en que se ingresaron\n\n";
    for (int i=0;i<num_tables;i++){
        names[i] = 65+i;
        tables[i].fill_hash(argv[i+1]);
    }
    
    string expresion;
    vector<string> posfija;
    while (expresion!="q"){
        cout << "\n\nIngrese la expresion a evaluar ('q' para salir):\n";
        cout << "\nR: ";
        cin >> expresion;
        if (expresion=="q")
            break;
        int notation = which_fix(expresion, names, num_tables);
        posfija.clear();
        if (notation==0)
            posfija = prefija_a_posfija(expresion, names, num_tables);
        else if (notation==1)
            posfija = infija_a_posfija(expresion, names, num_tables);
        else if (notation==2)
            posfija = posfija_a_pila(expresion, names, num_tables);
        if (posfija.size()<2)
            cout << "ERROR: EXPRESION INVALIDA\n";
        else {
            
        }
    }

    return 0;
}