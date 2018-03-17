#include "lexico.h"

int main()
{
    ifstream program;
    program.open("teste");

    createTable(program);

    return 0;
}
