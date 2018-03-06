#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int currentLine = 1;
char character;                         //caractere que está sendo lido no momento, deve ser global para ser guardado na mudança de tipo de token

struct Token
{
    string symbol;
    string TokenType;
    int line;
};

/*
    Ranges dos caracteres em ASCII:

    [0..9]: 48-57
    [A..B]: 65-90
    [a..b]: 97-122
    space:  32
    _:      95      >>>>>>>>>>NAO SAI DIREITO<<<<<<<
*/

void inteiro(Token& token, ifstream& program, char& character)
{
    token.TokenType = "inteiro";
    token.symbol.push_back(character);

    character = program.get();

    while(character == '\n' || character == '\t')
    {
        if(character == '\n')
        {
            ++currentLine;
        }
        character = program.get();        
    }  

    if(character >= 48 && character <= 57)
    {
        inteiro(token, program, character);     //próximo "estado do autônomo". se não houver próximo estado a partir do atual(no caso números inteiros), volta para o começo do loop no main
    }
    else
    {
        return;     //volta para o loop do main
    }
}

void identificador(Token& token, ifstream& program, char& character)
{
    token.TokenType = "identificador";
    token.symbol.push_back(character);

    character = program.get();

    while(character == '\n' || character == '\t')
    {
        if(character == '\n')
        {
            ++currentLine;
        }
        character = program.get();        
    }  

    if(     (character >= 48 && character <= 57 ) 
        ||  (character >= 65 && character <= 90 )
        ||  (character >= 97 && character <= 122)
        ||  (character == 95))
    {
        identificador(token, program, character);
    }
    else
    {
        return;
    }
}

void atribuicao(Token& token, ifstream& program, char& character)
{
    token.TokenType = "atribuição";
    token.symbol.push_back(character);

    character = program.get();

    while(character == '\n' || character == '\t')
    {
        if(character == '\n')
        {
            ++currentLine;
        }
        character = program.get();        
    }  
    return;    
}

void delimitador(Token& token, ifstream& program, char& character)
{
    token.TokenType = "delimitador";
    token.symbol.push_back(character);

    character = program.get();

    while(character == '\n' || character == '\t')
    {
        if(character == '\n')
        {
            ++currentLine;
        }
        character = program.get();        
    }  

    if(character == 61)
    {
        atribuicao(token, program, character);
    }
    else
    {
        return;
    }
}

int main()
{   
    ifstream program;
    program.open("teste1");    

    vector<Token> tokenList;

    character = program.get();          //pega o primeiro caractere; os proximos vão ser pegos dentro das funções

    while(!program.eof())               //enquanto ainda tiver programa a ser lido
    {
        Token currentToken;
        currentToken.line = currentLine;        
        
        if(character == 32 || character == '\t')             //ignora espaços
        {
            character = program.get();
            continue;
        }
        else if(character == '\n')
        {
            ++currentLine;
            character = program.get();
        }
        else if(character >= 48 && character <= 57)         //primeiro char é um numero
        {            
            inteiro(currentToken, program, character);
            tokenList.push_back(currentToken);                  //adiciona token no fim do array de tokens            
        }
        else if(    (character >= 65 && character <= 90 )   //primeiro char é uma letra
                ||  (character >= 97 && character <= 122))
        {
            identificador(currentToken, program, character);
            tokenList.push_back(currentToken);                  //adiciona token no fim do array de tokens
        }
        else if(character == 59 || //                       //primeiro char é um delimitador
                character == 46 || //
                character == 58 || //
                character == 40 || //
                character == 41 || //
                character == 44 )  //
        {
            delimitador(currentToken, program, character);
            tokenList.push_back(currentToken);                  //adiciona token no fim do array de tokens
        }        
    }

    for(unsigned int i = 0; i < tokenList.size(); ++i)      //imprime tokens, seus tipos
    {
        cout << tokenList[i].line << "\t"<< tokenList[i].symbol << "\t\t" << tokenList[i].TokenType << endl;
    }

    return 0;
}