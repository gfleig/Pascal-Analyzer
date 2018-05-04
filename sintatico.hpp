Token currentToken;
unsigned int currentIndex = 0;

vector<string> symbolTable;

int programa();
int declaracoesVariaveis();
int listaDeclaracaoVariaveis();
int listaDeclaracaoVariaveis_();
int listaDeIdentificadores();
int listaDeIdentificadores_();
int tipo();
int declaracoesDeSubprogramas();
int declaracoesDeSubprogramas_();
int declaracaoDeSubprograma();
int argumentos();
int listaDeParametros();
int listaDeParametros_();
int comandoComposto();
int comandosOpcionais();
int listaDeComandos();
int listaDeComandos_();
int comando();
int parteElse();
int variavel();
int ativacaoDeProcedimento();
int listaDeExpressoes();
int listaDeExpressoes_();
int expressao();
int expressaoSimples();
int expressaoSimples_();
int termo();
int termo_();
int fator();
int sinal();
int opMultiplicativo();
int opAditivo();
int opRelacional();

int erro()
{
    static int counter = 0;
    if(counter == 0)
    {
        cout << "ERROR: At line " << currentToken.line <<  " (" << currentToken.symbol << ")" << endl;
        counter++;
    }

    return 0;
}

//inicializa a tabela de símbolos. o MARK é $.
void initializeTable(){
    symbolTable.push_back("$");
}

//checa se existe um ident. com esse nome
void callSymbol(){
    int aux = 0;
    for(int i = symbolTable.size() - 1; i > 0; --i){
        if(symbolTable[i] == currentToken.symbol){
            aux++;
            break;
        }
    }
    if(!aux){
      cout << "Undeclared identifier on line " << currentToken.line <<  " (" << currentToken.symbol << ")" << endl;
    }
}

//checa até o primeiro MARK se já tem alguma var com o memso nome
void declareSymbol(){
    for(int i = symbolTable.size() - 1; i >= 0; --i){
        if(symbolTable[i] == "$"){
            symbolTable.push_back(currentToken.symbol);
            break;
        }
        else if(symbolTable[i] == currentToken.symbol){
            cout << "Already declared identifier on line " << currentToken.line <<  " (" << currentToken.symbol << ")" << endl;
            break;
        }
    }
}

//coloca um MARK pra sinalizar novo escopo
void enterScope(){
    symbolTable.push_back("$");
}

//para sair do escopo atual, deleta-se tudo até o primeiro MARK, inclusive o próprio MARK.
void exitScope(){
    while(symbolTable.back() != "$"){
        symbolTable.pop_back();
    }
    symbolTable.pop_back();
}

void getSymbol(){
    if(currentIndex != tokenList.size()){
      currentToken = tokenList[currentIndex];
      ++currentIndex;
    }
}

int opMultiplicativo(){
    getSymbol();
    if(currentToken.TokenType == "Op. Multiplicativo"){
        return 1;
    }
    else{
        --currentIndex;
        return 0;
    }
}

int opAditivo(){
    getSymbol();
    if(currentToken.TokenType == "Op. Aditivo"){
        return 1;
    }
    else{
        --currentIndex;
        return 0;
    }
}

int opRelacional(){
    getSymbol();
    if(currentToken.TokenType == "Op. Relacional"){
        return 1;
    }
    else{
        --currentIndex;
        return 0;
    }
}

int sinal(){
    getSymbol();
    if( currentToken.symbol == "+"  ||
        currentToken.symbol == "-"){
        return 1;
    }
    else{
        --currentIndex;
        return erro();
    }
}

int fator(){
    getSymbol();
    if( currentToken.TokenType == "Inteiro" ||
        currentToken.TokenType == "Real" ||
        currentToken.TokenType == "Real 1" ||
        currentToken.TokenType == "Boolean"){
        return 1;
    }
    else if (currentToken.TokenType == "Identificador"){
        callSymbol();
        getSymbol();

        if (currentToken.symbol == "("){
            if(listaDeExpressoes()){
                getSymbol();

                if (currentToken.symbol == ")"){
                    return 1;
                }
                else{
                    --currentIndex;
                    return erro();
                }
            }
            else{
                return erro();
            }
        }
        else{
            --currentIndex;
            return 1;
        }
    }
    else if (currentToken.symbol == "not"){
        return fator();
    }
    else if (currentToken.symbol == "("){
        if (expressao()){
            getSymbol();

            if (currentToken.symbol == ")"){
                return 1;
            }
            else{
                --currentIndex;
                return erro();
            }
        }
        else{
            return erro();
        }
    }
    else{
        --currentIndex;
        return erro();
    }
}

int termo_(){
    if(opMultiplicativo()){
        if(fator()){
            if(termo_()){
                return 1;
            }
            else{
                return erro();
            }
        }
        else{
            return erro();
        }
    }
    else{
        return 1;
    }
}

int termo(){
    return (fator() && termo_());
}

int expressaoSimples_(){
    if(opAditivo()){
        if(termo()){
            if(expressaoSimples_()){
                return 1;
            }
            else{
                return erro();
            }
        }
        else{
            return erro();
        }
    }
    else{
        return 1;
    }
}

int expressaoSimples(){
    if(termo() && expressaoSimples_()){
        return 1;
    }
    else if(sinal() && termo() && expressaoSimples_()){
        return 1;
    }
    else{
        return erro();
    }
}

int expressao(){
    if(expressaoSimples()){
        if(opRelacional() && expressaoSimples()){
            return 1;
        }
        else{
            return 1;
        }
    }
    else{
        return erro();
    }
}

int listaDeExpressoes_(){
    getSymbol();
    if(currentToken.symbol == ","){
        if(expressao()){
            return listaDeExpressoes_();
        }
        else{
            return erro();
        }
    }
    else{
        --currentIndex;
        return 1;
    }
}

int listaDeExpressoes(){
    return (expressao() && listaDeExpressoes_());
}

int ativacaoDeProcedimento(){
    if(currentToken.symbol == "("){
        if(listaDeExpressoes()){
            getSymbol();
            if(currentToken.symbol == ")"){
                return 1;
            }
            else{
                --currentIndex;
                return erro();
            }
        }
        else{
            return erro();
        }
    }
    else{
        return erro();
    }
}

int variavel(){
    getSymbol();
    if(currentToken.TokenType == "Identificador"){
        callSymbol();
        return 1;
    }
    else{
        --currentIndex;
        return 0;
    }
}

int parteElse(){
    getSymbol();
    if(currentToken.symbol == "else"){
        return comando();
    }
    else{
        --currentIndex;
        return 1;
    }
}

int comando(){
    if(variavel()){
        getSymbol();
        if(currentToken.TokenType == "Atribuição"){
            return expressao();
        }
        else if (ativacaoDeProcedimento()){
            return 1;
        }
        else{
            --currentIndex;
            return erro();
        }
    }
    else{
        getSymbol();
        if(currentToken.symbol == "if"){
            if(expressao()){
                getSymbol();
                if(currentToken.symbol == "then"){
                    return comando() && parteElse();
                }
                else{
                    --currentIndex;
                    return erro();
                }
            }
            else{
                return erro();
            }
        }
        else if (currentToken.symbol == "while"){
            if(expressao()){
                getSymbol();
                if(currentToken.symbol == "do"){
                    return comando();
                }
                else{
                    --currentIndex;
                    return erro();
                }
            }
            else{
                --currentIndex;
                return erro();
            }
        }
        else{
            --currentIndex;
            if (comandoComposto()){
                return 1;
            }
            return erro();
        }
    }
  }

int listaDeComandos_(){
    getSymbol();
    if(currentToken.symbol == ";"){
        return comando() && listaDeComandos_();
    }
    else{
        --currentIndex;
        return 1;
    }
}

int listaDeComandos(){
    return comando() && listaDeComandos_();
}

int comandosOpcionais(){
    getSymbol();
    if(currentToken.symbol == "end"){
        --currentIndex;
        return 1;
    }
    --currentIndex;
    return listaDeComandos();
}

int comandoComposto(){
    getSymbol();
    if(currentToken.symbol == "begin"){
        if(comandosOpcionais()){
            getSymbol();
            if(currentToken.symbol == "end"){
                return 1;
            }
            else{
                --currentIndex;
                return erro();
            }
        }
        else{
            return erro();
        }
    }
    else{
        --currentIndex;
        return erro();
    }
}

int listaDeParametros_(){
    getSymbol();
    if(currentToken.symbol == ";"){
        if(listaDeIdentificadores())
        {
            getSymbol();
            if(currentToken.symbol == ":"){
                if(tipo()){
                    return listaDeParametros_();
                }
                else{
                    return erro();
                }
            }
            else{
                return erro();
            }
        }
        else{
            return erro();
        }
    }
    else{
        --currentIndex;
        return 1;
    }
}

int listaDeParametros(){
    if(listaDeIdentificadores()){
        getSymbol();
        if(currentToken.symbol == ":"){
            if(tipo()){
                return listaDeParametros_();
            }
            else{
                return erro();
            }
        }
        else{
            return erro();
        }
    }
    else{
        return erro();
    }
}

int argumentos(){
    getSymbol();
    if(currentToken.symbol == "("){
        if(listaDeParametros()){
            getSymbol();
            if(currentToken.symbol == ")"){
                return 1;
            }
            else{
                --currentIndex;
                return erro();
            }
        }
        else{
            return erro();
        }
    }
    else{
        --currentIndex;
        return 1;
    }
}

int declaracaoDeSubprograma(){
    getSymbol();
    if(currentToken.symbol == "procedure"){
        enterScope();
        getSymbol();
        if(currentToken.TokenType == "Identificador"){
            declareSymbol();
            if(argumentos()){
                getSymbol();
                if(currentToken.symbol == ";"){
                    if(declaracoesVariaveis()){
                        if(declaracoesDeSubprogramas()){
                            if(comandoComposto()){
                                exitScope();
                                return 1;
                            }
                            else{
                                return erro();
                            }
                        }
                        else{
                            return erro();
                        }
                    }
                    else{
                        return erro();
                    }
                }
                else{
                    --currentIndex;
                    return erro();
                }
            }
            else{
                return erro();
            }
        }
        else{
            --currentIndex;
            return erro();
        }
    }
    else{
        --currentIndex;
        return 0;
    }
}

int declaracoesDeSubprogramas_(){
    if(declaracaoDeSubprograma()){
        getSymbol();
        if(currentToken.symbol == ";"){
            return declaracoesDeSubprogramas_();
        }
        else{
            --currentIndex;
            return erro();
        }
    }
    else{
        return 1;
    }
}

int declaracoesDeSubprogramas(){
    return declaracoesDeSubprogramas_();
}

int tipo(){
    getSymbol();
    if( currentToken.symbol == "integer" ||
        currentToken.symbol == "real" ||
        currentToken.symbol == "real1" ||
        currentToken.symbol == "Boolean"){
        return 1;
    }
    else{
        return erro();
    }
}

int listaDeIdentificadores_(){
    getSymbol();
    if(currentToken.symbol == ","){
        getSymbol();
        if(currentToken.TokenType == "Identificador"){
            declareSymbol();
            return listaDeIdentificadores_();
        }
        else{
            return 0;
        }
    }
    else{
        --currentIndex;
        return 1;
    }
}

int listaDeIdentificadores(){
    getSymbol();
    if(currentToken.TokenType == "Identificador"){
        declareSymbol();
        return listaDeIdentificadores_();
    }
    else{
        --currentIndex;
        return 0;
    }
}

int listaDeclaracaoVariaveis_(){
    if(listaDeIdentificadores()){
        getSymbol();
        if(currentToken.symbol == ":"){
            if(tipo()){
                getSymbol();
                if(currentToken.symbol == ";"){
                    return listaDeclaracaoVariaveis_();
                }
                else{
                    return erro();
                }
            }
            else{
                return erro();
            }
        }
        else{
            --currentIndex;
            return erro();
        }
    }
    else{
        return 1;
    }
}

int listaDeclaracaoVariaveis(){
    if(listaDeIdentificadores()){
        getSymbol();
        if(currentToken.symbol == ":"){
            if(tipo()){
                getSymbol();
                if(currentToken.symbol == ";"){
                    return listaDeclaracaoVariaveis_();
                }
                else{
                    --currentIndex;
                    return erro();
                }
            }
            else{
                return erro();
            }
        }
        else{
            --currentIndex;
            return erro();
        }
    }
    else{
        return erro();
    }
}

int declaracoesVariaveis(){
    getSymbol();
    if(currentToken.symbol == "var"){
        return listaDeclaracaoVariaveis();
    }
    else{
        --currentIndex;
        return 1;
    }
}

int programa(){

    initializeTable();

    getSymbol();
    if( currentToken.symbol == "program"){
        getSymbol();
        if(currentToken.TokenType == "Identificador"){
            declareSymbol();
            getSymbol();
            if(currentToken.symbol == ";"){
                if (declaracoesVariaveis()){
                    if (declaracoesDeSubprogramas()){
                        if (comandoComposto()){
                            getSymbol();

                            if(currentToken.symbol == "."){
                                cout << "Programa Sintaticamente Correto! " << endl;
                                return 1;
                            }
                            else{
                                --currentIndex;
                                getSymbol();
                                return erro();
                            }
                        }
                        else{
                            return 0;
                        }
                    }
                    else{
                        return 0;
                    }
                }
                else{
                    return 0;
                }
            }
            else{
                return erro();
            }
        }
        else{
            return erro();
        }
    }
    else{
        return erro();
    }
}

int checkTable(){

    return programa();
}
