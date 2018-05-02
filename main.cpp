#include "lexico.hpp"
#include "sintatico.hpp"

int main(){
    ifstream program;
    program.open("teste");

    createTable(program);
    checkTable();

    return 0;
}
