Token currentToken;

void erro()
{
  cout << "ERROR on line " << currentToken.currentLine << endl;
}

void getSymbol()
{
    static int currentIndex = 0;

    currentToken = tokenList[currentIndex];

    ++currentIndex;
}

int opMultiplicativo()
{
    getSymbol();
    if(currentToken.TokenType == "Op. Multiplicativo")
    {
        return 1;
    }
    else
    {
        erro();
    }
}

int opAditivo()
{
    getSymbol();
    if(currentToken.TokenType == "Op. Aditivo")
    {
        return 1;
    }
    else
    {
        erro();
    }
}

int opRelacional()
{
    getSymbol();
    if(currentToken.TokenType == "Op. Relacional")
    {
        return 1;
    }
    else
    {
        erro();
    }
}

int sinal()
{
    getSymbol();
    if(currentToken.TokenType == "Op. Relacional")
    {
        return 1;
    }
    else
    {
        erro();
    }
}

int fator()
{
    getSymbol();
    if( currentToken.TokenType == "Identificador" ||
        currentToken.TokenType == "Inteiro" ||
        currentToken.TokenType == "Real" ||
        currentToken.symbol == "true" ||
        currentToken.symbol == "false" ||
    )
    {
        getSymbol();

        if (currentToken.symbol == "(")
        {
          if(lista_de_expressoes() == 1)
          {
              getSymbol();

              if (currentToken.symbol == ")")
              {
                return 1;
              }
              else
              {
                erro();
              }
          }
          else
          {
            erro();
          }
        }
        else
        {
          return 1;
        }
    }
    else if (currentToken.symbol == "not")
    {
        return fator();
    }
    else if (currentToken.symbol == "(")
    {
        if (expressao() == 1)
        {
          getSymbol();

          if (currenToken.symbol == ")")
          {
            return 1;
          }
          else
          {
            erro();
          }
        }
    }
    else
    {
      erro();
    }
}
