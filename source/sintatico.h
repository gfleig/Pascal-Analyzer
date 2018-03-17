Token currentToken;

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
}

int opAditivo()
{
    getSymbol();
    if(currentToken.TokenType == "Op. Aditivo")
    {
        return 1;
    }
}

int opRelacional()
{
    getSymbol();
    if(currentToken.TokenType == "Op. Relacional")
    {
        return 1;
    }
}

int sinal()
{
    getSymbol();
    if(currentToken.TokenType == "Op. Relacional")
    {
        return 1;
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
        return 1;
    }
    else if (currentToken.symbol == "not")
    {
        return expressao();
    }
    else if
}
