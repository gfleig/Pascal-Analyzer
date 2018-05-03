#include "lexico.hpp"
#include "sintatico.hpp"

int main(){
    ifstream program;
    program.open("teste");

    int lex = createTable(program);

    if (lex)
      checkTable();

    return 0;
}
