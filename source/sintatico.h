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