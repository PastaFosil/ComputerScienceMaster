/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 1 C++
 * 
 * Suma o multiplica matrices haciendo uso de multiprocesos
************************************************************/

#include <iostream>
using namespace std;

struct tablero {
    bool a: 1;
    bool b: 1;
    bool c: 1;
    bool d: 1;
    bool e: 1;
    bool f: 1;
    bool g: 1;
    bool h: 1;
};

int main() {
    struct tablero am;
    am.a = am.b = am.c = 1;
    am.d = am.e = am.f = am.g = am.h = 0;

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "   ^" << endl;

    am.d |= am.c;
    am.c = !am.c;

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "  ^" << endl;

    // c <-> e

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "    ^" << endl;

    // e <-> f

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "     ^" << endl;

    am.f |= am.d;
    am.d = !am.d;

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "   ^" << endl;

    am.d |= am.b;
    am.b = !am.b;

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << " ^" << endl;

    am.b |= am.a;
    am.a = !am.a;

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "^" << endl;

    // a <-> c

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "  ^" << endl;

    // c <-> e

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "    ^" << endl;

    // e <-> g

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "      ^" << endl;

    // g <-> h

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "       ^" << endl;

    am.h |= am.f;
    am.f = !am.f;

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "     ^" << endl;

    am.f |= am.d;
    am.d = !am.d;

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "   ^" << endl;

    am.d |= am.b;
    am.b = !am.b;

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << " ^" << endl;

    // b <-> c

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "  ^" << endl;

    // c <-> e

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "    ^" << endl;

    // e <-> g

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "      ^" << endl;

    am.g |= am.f;
    am.f = !am.f;

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "     ^" << endl;

    am.f |= am.d;
    am.d = !am.d;

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "   ^" << endl;

    // d <-> e

    cout << am.a << am.b << am.c << am.d << am.e << am.f << am.g << am.h << endl;
    cout << "    ^" << endl;
}