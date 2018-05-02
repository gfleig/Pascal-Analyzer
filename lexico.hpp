#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int currentLine = 1;
int errorID = 0;
char character;

struct Token{
    string symbol;
    string TokenType;
    int line;
};

vector<Token> tokenList;

void real(Token& token, ifstream& program, char& character){
    token.TokenType = "Real";
    token.symbol.push_back(character);

    character = program.get();

    if(character >= 48 && character <= 57){
        real(token, program, character);     //próximo "estado do autônomo". se não houver próximo estado a partir do atual(no caso números inteiros), volta para o começo do loop no main
    }
    else{
        return;     //volta para o loop do main
    }
}

void real1B(Token& token, ifstream& program, char& character){
  char eChar = character;
  character = program.get();

  if(character == '+' || character == '-'){
    token.symbol.push_back(eChar);
    token.symbol.push_back(character);

    character = program.get();

    if(character >= 48 && character <= 57){
      while(true){
        if(character >= 48 && character <= 57){
          token.symbol.push_back(character);
          character = program.get();
          continue;
        }
        else{
          break;
        }
      }
    }
    else{
      //acessar último elemento da string Symbol
      //token.symbol.at(token.symbol.back());
      //remover último elementro da string symbol
      //token.symbol.pop_back();

      Token auxToken;
      auxToken.TokenType = "Op. Aditivo";
      auxToken.line = currentLine;
      auxToken.symbol = token.symbol.at(0);
      token.symbol.erase(0,1);

      tokenList.push_back(auxToken);

      Token aux2Token;
      aux2Token.TokenType = "Op. Aditivo";
      aux2Token.line = currentLine;
      aux2Token.symbol = token.symbol.back();
      token.symbol.pop_back();

      tokenList.push_back(aux2Token);

      Token aux3Token;
      aux3Token.TokenType = "Identificador";
      aux3Token.line = currentLine;
      aux3Token.symbol = token.symbol.back();
      token.symbol.pop_back();

      tokenList.push_back(aux3Token);

      token.TokenType = "Inteiro";

    }
  }
  else{
    Token auxToken;
    auxToken.TokenType = "Op. Aditivo";
    auxToken.line = currentLine;
    auxToken.symbol = token.symbol.at(0);
    token.symbol.erase(0,1);

    tokenList.push_back(auxToken);

    token.TokenType = "Inteiro";

    program.seekg(-2, ios_base::cur);

    character = program.get();

  }
}

void real1A(Token& token, ifstream& program, char& character){
    token.TokenType = "Real 1";
    token.symbol.push_back(character);

    character = program.get();

    if(character >= 48 && character <= 57){
      real1A(token, program, character);
    }
    else if(character == 'e'){
      real1B(token, program, character);
    }
    else if(character == '.'){
      Token auxToken;
      auxToken.TokenType = "Op. Aditivo";
      auxToken.line = currentLine;
      auxToken.symbol = token.symbol.at(0);
      token.symbol.erase(0,1);

      tokenList.push_back(auxToken);

      real(token, program, character);
    }
    else{
      Token auxToken;
      auxToken.TokenType = "Op. Aditivo";
      auxToken.line = currentLine;
      auxToken.symbol = token.symbol.at(0);
      token.symbol.erase(0,1);

      tokenList.push_back(auxToken);

      token.TokenType = "Inteiro";
    }

}

void boolean(Token& token, ifstream& program, char& character){
    token.TokenType = "Boolean";
    token.symbol.push_back(character);

    character = program.get();

    return;     //volta para o loop do main
}

void inteiro(Token& token, ifstream& program, char& character){
    token.TokenType = "Inteiro";
    token.symbol.push_back(character);

    character = program.get();

    if(character >= 48 && character <= 57){
        inteiro(token, program, character);     //próximo "estado do autônomo". se não houver próximo estado a partir do atual(no caso números inteiros), volta para o começo do loop no main
    }
    else if(character == '.'){
        real(token, program, character);
    }
    else{
        return;     //volta para o loop do main
    }
}

void identificador(Token& token, ifstream& program, char& character){
    token.TokenType = "Identificador";
    token.symbol.push_back(character);

    character = program.get();

    if(     (character >= 48 && character <= 57 )
        ||  (character >= 65 && character <= 90 )
        ||  (character >= 97 && character <= 122)
        ||  (character == 95)){
        identificador(token, program, character);
    }
    else{
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
          ||  (token.symbol == "not")){
        token.TokenType = "Palavra Chave";
      }
      else if(token.symbol == "or"){
        token.TokenType = "Op. Aditivo";
      }
      else if(token.symbol == "and"){
        token.TokenType = "Op. Multiplicativo";
      }
      else if(token.symbol == "true" || token.symbol == "false"){
        token.TokenType = "Boolean";
      }
        return;
    }
}

void atribuicao(Token& token, ifstream& program, char& character){
    token.TokenType = "Atribuição";
    token.symbol.push_back(character);

    character = program.get();

    return;
}

void delimitador(Token& token, ifstream& program, char& character){
    token.TokenType = "Delimitador";
    token.symbol.push_back(character);

    character = program.get();

    if(token.symbol[0] == ':' && character == 61){
        atribuicao(token, program, character);
    }
    else{
        return;
    }
}

void aditivo(Token& token, ifstream& program, char& character){
    token.TokenType = "Op. Aditivo";
    token.symbol.push_back(character);

    character = program.get();

    if (character >= 48 && character <= 57){
      real1A(token, program, character);
    }

    return;
}

void multiplicativo(Token& token, ifstream& program, char& character){
    token.TokenType = "Op. Multiplicativo";
    token.symbol.push_back(character);

    character = program.get();

    return;
}

void relacional(Token& token, ifstream& program, char& character){
    token.TokenType = "Op. Relacional";
    token.symbol.push_back(character);

    char firstCharacter = character;

    character = program.get();

    if(firstCharacter == '='){
        return;
    }
    else if(character == '='){
        token.symbol.push_back(character);
        character = program.get();
        return;
    }
    else if (character == '>' && firstCharacter == '<'){
        token.symbol.push_back(character);
        character = program.get();
        return;
    }
}

void comentario(Token& token, ifstream& program, char& character){
  while(character != '}'){
    character = program.get();

    if(character == '\n'){
        ++currentLine;
        continue;
    }
    else if(program.eof()){
      errorID = 2;
      break;
    }
  }
  character = program.get();
  return;
}

void createTable(ifstream& program){
  character = program.get();          //pega o primeiro caractere; os proximos vão ser pegos dentro das funções

  while(!program.eof()){             //enquanto ainda tiver programa a ser lido
      Token currentToken;
      currentToken.line = currentLine;

      if(character == 32 || character == '\t'){             //ignora espaços
          character = program.get();
          continue;
      }
      else if(character == '\n'){
          ++currentLine;
          character = program.get();
          continue;
      }
      else if(character >= 48 && character <= 57){         //primeiro char é um numero
          inteiro(currentToken, program, character);
      }
      else if(    (character >= 65 && character <= 90 )   //primeiro char é uma letra
              ||  (character >= 97 && character <= 122)){
          identificador(currentToken, program, character);
      }
      else if(character == 59 ||                        //primeiro char é um delimitador
              character == 46 ||
              character == 58 ||
              character == 40 ||
              character == 41 ||
              character == 44 ){
          delimitador(currentToken, program, character);
      }
      else if(character == '+' || character == '-'){         //operadores aditivos
          aditivo(currentToken, program, character);
      }
      else if(character == '*' || character == '/'){         //operadores multiplicativos
          multiplicativo(currentToken, program, character);
      }
      else if (character == '>' || character == '<'|| character == '='){        //operadores relacionais
          relacional(currentToken, program, character);
      }
      else if (character == '{'){
          comentario(currentToken, program, character);
          continue;
      }
      else{
          errorID = 1;
          break;
      }
      tokenList.push_back(currentToken);                  //adiciona token no fim do array de tokens
  }

  if (errorID == 0){
      for(unsigned int i = 0; i < tokenList.size(); ++i){      //imprime tokens, seus tipos
          cout << tokenList[i].line << "\t"<< tokenList[i].symbol << "\t\t" << tokenList[i].TokenType << endl;
      }
      return;
  }
  else if (errorID == 1){
      cout << "ERROR: Unkown Symbol on line " << currentLine << endl;
      return;
  }
  else if (errorID == 2){
      cout << "ERROR: Unclosed Comment" << endl;
      return;
  }
}
