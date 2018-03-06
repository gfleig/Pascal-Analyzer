#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int currentLine = 1;
int errorID = 0;
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

void real(Token& token, ifstream& program, char& character)
{
    token.TokenType = "Real";
    token.symbol.push_back(character);

    character = program.get();

    if(character >= 48 && character <= 57)
    {
        real(token, program, character);     //próximo "estado do autônomo". se não houver próximo estado a partir do atual(no caso números inteiros), volta para o começo do loop no main
    }
    else
    {
        return;     //volta para o loop do main
    }
}

void inteiro(Token& token, ifstream& program, char& character)
{
    token.TokenType = "Inteiro";
    token.symbol.push_back(character);

    character = program.get();

    if(character >= 48 && character <= 57)
    {
        inteiro(token, program, character);     //próximo "estado do autônomo". se não houver próximo estado a partir do atual(no caso números inteiros), volta para o começo do loop no main
    }
    else if(character == '.')
    {
        real(token, program, character);
    }
    else
    {
        return;     //volta para o loop do main
    }
}

void identificador(Token& token, ifstream& program, char& character)
{
    token.TokenType = "Identificador";
    token.symbol.push_back(character);

    character = program.get();

    if(     (character >= 48 && character <= 57 )
        ||  (character >= 65 && character <= 90 )
        ||  (character >= 97 && character <= 122)
        ||  (character == 95))
    {
        identificador(token, program, character);
    }
    else
    {
      if(     (token.symbol == "program" )
          ||  (token.symbol == "var")
          ||  (token.symbol == "integer")
          ||  (token.symbol == "real")
          ||  (token.symbol == "boolean")
          ||  (token.symbol == "procedure")
          ||  (token.symbol == "begin")
          ||  (token.symbol == "end")
          ||  (token.symbol == "if")
          ||  (token.symbol == "then")
          ||  (token.symbol == "else")
          ||  (token.symbol == "while")
          ||  (token.symbol == "do")
          ||  (token.symbol == "not"))
      {
        token.TokenType = "Palavra Chave";
      }
      else if(token.symbol == "or")
      {
        token.TokenType = "Op. Aditivo";
      }
      else if(token.symbol == "and")
      {
        token.TokenType = "Op. Multiplicativo";
      }
        return;
    }
}

void atribuicao(Token& token, ifstream& program, char& character)
{
    token.TokenType = "Atribuição";
    token.symbol.push_back(character);

    character = program.get();

    return;
}

void delimitador(Token& token, ifstream& program, char& character)
{
    token.TokenType = "Delimitador";
    token.symbol.push_back(character);

    character = program.get();

    if(token.symbol[0] == ':' && character == 61)
    {
        atribuicao(token, program, character);
    }
    else
    {
        return;
    }
}

void aditivo(Token& token, ifstream& program, char& character)
{
    token.TokenType = "Op. Aditivo";
    token.symbol.push_back(character);

    character = program.get();

    return;
}

void multiplicativo(Token& token, ifstream& program, char& character)
{
    token.TokenType = "Op. Multiplicativo";
    token.symbol.push_back(character);

    character = program.get();

    return;
}

void relacional(Token& token, ifstream& program, char& character)
{
    token.TokenType = "Op. Relacional";
    token.symbol.push_back(character);

    char firstCharacter = character;

    character = program.get();

    if(firstCharacter == '=')
    {
        return;
    }
    else if(character == '=')
    {
        token.symbol.push_back(character);
        character = program.get();
        return;
    }
    else if (character == '>' && firstCharacter == '<')
    {
        token.symbol.push_back(character);
        character = program.get();
        return;
    }
}

void comentario(Token& token, ifstream& program, char& character)
{
  while(character != '}')
  {
    character = program.get();

    if(character == '\n')
    {
        ++currentLine;
        character = program.get();
        continue;
    }
    else if(program.eof())
    {
      errorID = 2;
      break;
    }
  }
  character = program.get();
  return;
}

int main()
{
    ifstream program;
    program.open("teste");

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
            continue;
        }
        else if(character >= 48 && character <= 57)         //primeiro char é um numero
        {
            inteiro(currentToken, program, character);
        }
        else if(    (character >= 65 && character <= 90 )   //primeiro char é uma letra
                ||  (character >= 97 && character <= 122))
        {
            identificador(currentToken, program, character);
        }
        else if(character == 59 || //                       //primeiro char é um delimitador
                character == 46 || //
                character == 58 || //
                character == 40 || //
                character == 41 || //
                character == 44 )  //
        {
            delimitador(currentToken, program, character);
        }
        else if(character == '+' || character == '-')         //operadores aditivos
        {
            aditivo(currentToken, program, character);
        }
        else if(character == '*' || character == '/')         //operadores multiplicativos
        {
            multiplicativo(currentToken, program, character);
        }
        else if (character == '>' || character == '<'|| character == '=')        //operadores relacionais
        {
            relacional(currentToken, program, character);
        }
        else if (character == '{')
        {
            comentario(currentToken, program, character);
            continue;
        }
        else
        {
            errorID = 1;
            break;
        }
        tokenList.push_back(currentToken);                  //adiciona token no fim do array de tokens
    }

    if (errorID == 0)
    {
        for(unsigned int i = 0; i < tokenList.size(); ++i)      //imprime tokens, seus tipos
        {
            cout << tokenList[i].line << "\t"<< tokenList[i].symbol << "\t\t" << tokenList[i].TokenType << endl;
        }
    }
    else if (errorID == 1)
    {
        cout << "ERROR: Unkown Symbol on line " << currentLine << endl;
        return 1;
    }
    else if (errorID == 2)
    {
        cout << "ERROR: Unclosed Comment" << endl;
        return 1;
    }

    return 0;
}
