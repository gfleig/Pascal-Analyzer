Token currentToken;
int currentIndex = 0;

int erro()
{
  cout << "ERROR on line " << currentToken.currentLine << endl;
  return 0;
}

void getSymbol()
{
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

            if (currentToken.symbol == ")")
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

int termo_()
{
    if(opMultiplicativo())
    {
        if(fator())
        {
            if(termo_())
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
        --currentIndex;
        return 1;
    }
}

int termo()
{
    return (fator() && termo_());
}

int expressaoSimples_()
{
    if(opAditivo())
    {
        if(termo())
        {
            if(expressaoSimples_())
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
        --currentIndex;
        return 1;
    }
}

int expressaoSimples()
{
    if(termo() && expressaoSimples_())
    {
        return 1;
    }
    else if(sinal() && termo() && expressaoSimples_())
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int expressao()
{
    if(expressao())
    {
        return 1;
    }
    else if(expressaoSimples() && opRelacional() && expressaoSimples())
    {
        return 1;
    }
    else
    {
        return erro();
    }
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
    getSymbol();
    if(currentToken.TokenType == "Identificador")
    {
        return 1;
    }
    else
    {
        return erro();
    }
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
    getSymbol();
    if(currentToken.symbol == "begin")
    {
        if(comandosOpcionais())
        {
            getSymbol();
            if(currentToken.symbol == "end")
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
            erro();
        }
    }
    else
    {
        return erro();
    }
}

int listaDeParametros_()
{

}

int listaDeParametros()
{

}

int argumentos()
{
    getSymbol();
    if(currentToken.symbol == "(")
    {
        if(listaDeParametros())
        {
            getSymbol();
            if(currentToken.symbol == ")")
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
        --currentIndex;
        return 1;
    }
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
    getSymbol();
    if( currentToken.TokenType == "integer" ||
        currentToken.TokenType == "real" ||
        currentToken.TokenType == "boolean")
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int listaDeIdentificadores_()
{

}

int listaDeIdentificadores()
{

}

int listaDeclaracaoVariaveis_()
{
    if(listaDeIdentificadores())
    {
        getSymbol();
        if(currentToken.symbol == ":")
        {
            if(tipo())
            {
                getSymbol();
                if(currentToken.symbol == ";")
                {
                    return listaDeIdentificadores_();
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
        --currentIndex;
        return 1;
    }
}

int listaDeclaracaoVariaveis()
{
    if(listaDeIdentificadores())
    {
        getSymbol();
        if(currentToken.symbol == ":")
        {
            if(tipo())
            {
                getSymbol();
                if(currentToken.symbol == ";")
                {
                    return listaDeclaracaoVariaveis_();
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

int declaracoesVariaveis()
{
    getSymbol();
    if(currentToken.symbol == "var")
    {
        return listaDeclaracaoVariaveis();
    }
    else
    {
        return 1;
    }
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
                if (declaracoesVariaveis())
                {
                    if (declaracaoDeSubprogramas())
                    {
                        if (comandoComposto())
                        {
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
                    }
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
