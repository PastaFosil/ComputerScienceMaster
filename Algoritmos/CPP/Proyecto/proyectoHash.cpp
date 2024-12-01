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

using namespace std;

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

vector <string> infija_a_posfija(string expresion){
    vector <string> posfija;
    vector <string> pila;
    int len = expresion.length();
    int i = 0;
    while (expresion[i]!='\0'){
        string num = "";
        if (expresion[i]=='(' || expresion[i]=='[' || expresion[i]=='{'){
            if (expresion[i]=='(')
                pila.push_back("(");
            else if (expresion[i]=='[')
                pila.push_back("[");
            else if (expresion[i]=='{')
                pila.push_back("{");
        } else if (expresion[i]==')' || expresion[i]==']' || expresion[i]=='}'){
            if (expresion[i]==')'){
                while (pila.back()!="("){
                    posfija.push_back(pila.back());
                    pila.pop_back();
                }
            } else if (expresion[i]==']'){
                while (pila.back()!="["){
                    posfija.push_back(pila.back());
                    pila.pop_back();
                }
            } else if (expresion[i]=='}'){
                while (pila.back()!="{"){
                    posfija.push_back(pila.back());
                    pila.pop_back();
                }
            }
        } else if (expresion[i]=='u' || expresion[i]=='n' || expresion[i]=='-' || expresion[i]=='s'){
            if (expresion[i]=='u')
                pila.push_back("u");
            else if (expresion[i]=='n')
                pila.push_back("n");
            else if (expresion[i]=='-')
                pila.push_back("-");
            else if (expresion[i]=='s')
                pila.push_back("s");
        }
    }
}

int main(int argc, char **argv){
    if (argc!=3){
        cout << "USO: " << argv[0] << " <archivo 1> <archivo 2>\n";
        return -1;
    }
    hash_table h1, h2;
    h1.fill_hash(argv[1]);
    h2.fill_hash(argv[2]);
    
    int choice = -1;
    while (choice != 8){
        cout << "\n\nSeleccione operacion:\n";
        cout << "1. Imprimir tabla 1\n";
        cout << "2. Imprimir tabla 2\n";
        cout << "3. Imprimir interseccion\n";
        cout << "4. Imprimir union\n";
        cout << "5. Imprimir diferencia\n";
        cout << "6. Imprimir diferencia simetrica\n";
        cout << "7. Guardar archivos con factores de carga de ambas tablas\n";
        cout << "8. Salir.\n";
        cout << "\nR: ";
        cin >> choice;

        switch (choice){
            case 1:
                h1.printTable();
                break;
            case 2:
                h2.printTable();
                break;
            case 3:
                h2.intersection(h2);
                break;
            case 4:
                h1.union_hash(h2);
                break;
            case 5:
                h1.difference(h2);
                break;
            case 6:
                h1.symmetric_difference(h2);
                break;
            case 7:
                h1.histogram();
                h2.histogram();
                break;
            case 8:
                break;
            default:
                cout << "\n\nIngrese una opcion valida\n\n" << endl;
                break;

        }
    }
    
    return 0;
}