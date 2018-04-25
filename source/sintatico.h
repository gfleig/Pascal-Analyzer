Token currentToken;
unsigned int currentIndex = 0;

string errorMessage;

vector<string> symbolTable;
vector<string> PcT;

struct IdentifierAndType{
    string identifier;
    string type;
};

vector<IdentifierAndType> identifierList;

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

void addIdentifier(){
    IdentifierAndType duo;
    duo.identifier = currentToken.symbol;
    duo.type = "unset";                     //identificador está "marcado"

    identifierList.push_back(duo);
}

void setTypeOfidentifiers(){
    int lastIdentifier = identifierList.size();

    for(int i = lastIdentifier; i >= 0; --i){
        if(identifierList[i].type != "unset"){
            break;
        }
        else{
            if(currentToken.TokenType == "Palavra Chave")
              identifierList[i].type = "program";
            else
              identifierList[i].type = currentToken.symbol;
        }
    }
}

int updatePcTArithmetic(){
    int top = PcT.size() - 1;
    int subtop = top - 1;

    if(PcT[top] == "integer" && PcT[subtop] == "integer"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("integer");
        return 1;
    }
    else if(PcT[top] == "integer" && PcT[subtop] == "real"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("real");
        return 1;
    }
    else if(PcT[top] == "real" && PcT[subtop] == "integer"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("real");
        return 1;
    }
    else if(PcT[top] == "real" && PcT[subtop] == "real"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("real");
        return 1;
    }
    else{
        return 0;
    }
}

int updatePcTBoolean(){
    int top = PcT.size() - 1;
    int subtop = top - 1;

    if(PcT[top] == "Boolean" && PcT[subtop] == "Boolean"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Boolean");
        return 1;
    }
    else{
        return 0;
    }
}

int updatePcTRelational(){
    int top = PcT.size() - 1;
    int subtop = top - 1;

    if(PcT[top] == "integer" && PcT[subtop] == "integer"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Boolean");
        return 1;
    }
    else if(PcT[top] == "integer" && PcT[subtop] == "real"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Boolean");
        return 1;
    }
    else if(PcT[top] == "real" && PcT[subtop] == "integer"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Boolean");
        return 1;
    }
    else if(PcT[top] == "real" && PcT[subtop] == "real"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Boolean");
        return 1;
    }
    else{
        return 0;
    }
}

int updatePcTAtribution(){
    int top = PcT.size() - 1;
    int subtop = top - 1;

    if(PcT[top] == "integer" && PcT[subtop] == "integer"){
        PcT.pop_back();
        PcT.pop_back();
        return 1;
    }
    else if(PcT[top] == "integer" && PcT[subtop] == "real"){
        PcT.pop_back();
        PcT.pop_back();
        return 1;
    }
    else if(PcT[top] == "real" && PcT[subtop] == "real"){
        PcT.pop_back();
        PcT.pop_back();
        return 1;
    }
    else if(PcT[top] == "Boolean" && PcT[subtop] == "Boolean"){
        PcT.pop_back();
        PcT.pop_back();
        return 1;
    }
    else if(PcT[top] == "real1" && PcT[subtop] == "real1"){
        PcT.pop_back();
        PcT.pop_back();
        return 1;
    }
    else{
        cout << "ERROR: Type mismatch\n" << endl;
        return 0;
    }
}

string getIdentifierType(string identifierName){
    for(int i = identifierList.size(); i >= 0; --i){
        if(identifierList[i].identifier == identifierName){
            return identifierList[i].identifier;
        }
    }
    cout << "No identifier with such name found" << endl;
    return NULL;
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
      cout << "Undeclared identifier on line " << currentToken.line << endl;
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
            cout << "Already declared identifier on line " << currentToken.line << endl;
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

int erro(string message){
    static int check = 0;

    if(!check){
        errorMessage = message;
        ++check;
    }

    return 0;
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
        return erro("Expected multiplication operator('*', '/', or 'and')");
    }
}

int opAditivo(){
    getSymbol();
    if(currentToken.TokenType == "Op. Aditivo"){
        return 1;
    }
    else{
        --currentIndex;
        return erro("Expected addition operator('+', '-', or 'or')");
    }
}

int opRelacional(){
    getSymbol();
    if(currentToken.TokenType == "Op. Relacional"){
        return 1;
    }
    else{
        --currentIndex;
        return erro("Expected relational operator('>', '<', '>=', '<=', or '=')");
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
        return erro("Expected '+' or '-'");
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
                    return erro("Expected closing parentheses ')'");
                }
            }
            else{
                return erro("Expected 'listaDeExpressoes'");
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
                return erro("Expected closing parentheses ')'");
            }
        }
        else{
            return erro("Expected 'expressao'");
        }
    }
    else{
        --currentIndex;
        return erro("Expected opening parentheses '('");
    }
}

int termo_(){
    if(opMultiplicativo()){
        if(fator()){
            if(termo_()){
                return 1;
            }
            else{
                return erro("Expected 'termo_'");
            }
        }
        else{
            return erro("Expected 'fator'");
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
                return erro("Expected 'expressaoSimples_'");
            }
        }
        else{
            return erro("Expected termo");
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
        return erro("During 'expressaoSimples': no valid pattern detected");
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
        return erro("Expected expressaoSimples");
    }
}

int listaDeExpressoes_(){
    getSymbol();
    if(currentToken.symbol == ","){
        if(expressao()){
            return listaDeExpressoes_();
        }
        else{
            return erro("Expected 'expressao'");
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
                return erro("Expected closing parentheses ')'");
            }
        }
        else{
            return erro("Expected 'listaDeExpressoes'");
        }
    }
    else{
        return erro("Expected opening parentheses '('");
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
        return erro("Expected identifier token");
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
            return erro("In 'comando': no valid pattern detected");
        }
    }
    else if (comandoComposto()){
        return 1;
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
                    return erro("Expected 'then'");
                }
            }
            else{
                return erro("Expected 'expressao'");
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
                    return erro("Expected 'do'");
                }
            }
            else{
                --currentIndex;
                return erro("Expected 'epressao'");
            }
        }
        else{
            --currentIndex;
            return erro("Expected 'while'");
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
                return erro("Expected 'end'");
            }
        }
        else{
            return erro("Expected 'comandosOpcionais'");
        }
    }
    else{
        --currentIndex;
        return erro("Expected 'begin'");
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
                    return erro("Expected 'tipo'");
                }
            }
            else{
                return erro("Expected ':'");
            }
        }
        else{
            return erro("Expected 'listaDeIdentificadores'");
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
                return erro("Expected 'tipo'");
            }
        }
        else{
            return erro("Expected ':'");
        }
    }
    else{
        return erro("Expected 'listaDeIdentificadores'");
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
                return erro("Expected closing parentheses ')'");
            }
        }
        else{
            return erro("Expected 'listaDeParametros'");
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
            addIdentifier();
            setTypeOfidentifiers();
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
                                return erro("Expected 'comandoComposto'");
                            }
                        }
                        else{
                            return erro("Expected 'declaracoesDeSubprogramas'");
                        }
                    }
                    else{
                        return erro("Expected 'declaracoesVariaveis'");
                    }
                }
                else{
                    --currentIndex;
                    return erro("Expected ;");
                }
            }
            else{
                return erro("Expected 'argumentos'");
            }
        }
        else{
            --currentIndex;
            return erro("Expected identifier token");
        }
    }
    else{
        --currentIndex;
        return erro("Expected keyword 'procedure'");
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
            return erro("Expected ';'");
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
        currentToken.symbol == "boolean"){
        setTypeOfidentifiers();
        return 1;
    }
    else{
        return erro("Expected 'integer', 'real', 'real1', or 'boolean'");
    }
}

int listaDeIdentificadores_(){
    getSymbol();
    if(currentToken.symbol == ","){
        getSymbol();
        if(currentToken.TokenType == "Identificador"){
            declareSymbol();
            addIdentifier();
            return listaDeIdentificadores_();
        }
        else{
            return erro("Expected identifier token");
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
        addIdentifier();
        return listaDeIdentificadores_();
    }
    else{
        --currentIndex;
        return erro("Expected identifier token");
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
                    return erro("Expected ';'");
                }
            }
            else{
                return erro("Expected 'tipo'");
            }
        }
        else{
            --currentIndex;
            return erro("Expected ':'");
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
                    return erro("Expected ';'");
                }
            }
            else{
                return erro("Expected 'tipo'");
            }
        }
        else{
            --currentIndex;
            return erro("Expected ':'");
        }
    }
    else{
        return erro("Expected 'listaDeIdentificadores'");
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

    cout << "Simbolos na tabela: " << tokenList.size() << endl;

    initializeTable();

    getSymbol();
    if( currentToken.symbol == "program"){
        getSymbol();
        if(currentToken.TokenType == "Identificador"){
            declareSymbol();
            addIdentifier();
            setTypeOfidentifiers();
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
                                cout << "ERROR on line " << currentToken.line << endl;
                                return erro("Expected '.' at the end of the program");
                            }
                        }
                        else{
                            cout << "ERROR on line " << currentToken.line << endl;
                            return 0;
                        }
                    }
                    else{
                        cout << "ERROR on line " << currentToken.line << endl;
                        return 0;
                    }
                }
                else{
                    cout << "ERROR on line " << currentToken.line << endl;
                    return 0;
                }
            }
            else{
                cout << "ERROR on line " << currentToken.line << endl;
                return erro("Expected ';' after program identifier");
            }
        }
        else{
            cout << "ERROR on line " << currentToken.line << endl;
            return erro("Expected identifier after keyword 'program'");
        }
    }
    else{
        cout << "ERROR on line " << currentToken.line << endl;
        return erro("Expected keyword 'program' at the start");
    }
}

int checkTable(){
    int returnCode = programa();
    if(!returnCode){
        cout << errorMessage << endl;
    }
    return returnCode;
}
