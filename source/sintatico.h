Token currentToken;

int erro()
{
  cout << "ERROR on line " << currentToken.currentLine << endl;
  return 0;
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
        return erro();
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
        return erro();
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
        return erro();
    }
}

int sinal()
{
    getSymbol();
    if( currentToken.symbol == "+"  || 
        currentToken.symbol == "-")
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int fator()
{
    getSymbol();
    if( currentToken.TokenType == "Inteiro" ||
        currentToken.TokenType == "Real" ||
        currentToken.symbol == "true" ||
        currentToken.symbol == "false" ||
    )
    {
        return 1;
    }
    else if (currentToken.TokenType == "Identificador")
    {
        getSymbol();

        if (currentToken.symbol == "(")
        {
            if(listaDeExpressoes())
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
        if (expressao())
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

int termo_()
{

}

int termo()
{

}

int expressaoSimples_()
{
    
}

int expressaoSimples()
{

}

int expressao()
{

}

int listaDeExpressoes_()
{

}

int listaDeExpressoes()
{

}

int ativacaoDeProcedimento()
{

}

int variavel()
{
    
}

int parteElse()
{

}

int comando()
{

}

int listaDeComandos_()
{

}

int listaDeComandos()
{

}

int comandosOpcionais()
{

}

int comandoComposto()
{

}

int listaDeParametros_()
{

}

int listaDeParametros()
{

}

int argumentos()
{

}

int declaracaoDeSubprograma()
{

}

int declaracaoDeSubprogramas_()
{

}

int declaracaoDeSubprogramas()
{

}

int tipo()
{

}

int listaDeIdentificadores_()
{

}

int listaDeIdentificadores()
{
    
}

int listaDeclaracaoVariaveis_()
{

}

int listaDeclaracaoVariaveis()
{
    
}

int declaracoesVariaveis()
{

}

int programa()
{
    getSymbol();
    if( currentToken.symbol == "program")
    {
        getSymbol();
        if(currentToken.TokenType == "Identificador")
        {
            getSymbol();
            if(currentToken.symbol == ";")
            {
                declaracoesVariaveis();
                declaracaoDeSubprogramas();
                comandoComposto();

                getSymbol();

                if(currentToken.symbol == ".")
                {
                    return 1;
                }
                else
                {
                    return erro();
                }
            }
            else
            {
                return erro();
            }            
        }
        else
        {
            return erro();
        }
    }
    else 
    {
        return erro();
    }
}