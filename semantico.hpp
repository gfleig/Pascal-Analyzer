
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

Token currentToken;
unsigned int currentIndex = 0;

std::vector<std::string> PcT;

struct IdentifierAndType{
    string identifier;
    string type;
};

vector<IdentifierAndType> symbolTable;

//inicializa a tabela de símbolos. o MARK é $.
void initializeTable(){
    IdentifierAndType duo;
    duo.identifier = "$";
    duo.type = "mark";
    symbolTable.push_back(duo);
}

//checa se existe um ident. com esse nome
void callSymbol(){
    int aux = 0;
    for(int i = symbolTable.size() - 1; i > 0; --i){
        if(symbolTable[i].identifier == currentToken.symbol){
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
    IdentifierAndType duo;
    duo.identifier = currentToken.symbol;
    duo.type = "unset";

    for(int i = symbolTable.size() - 1; i >= 0; --i){
        if(symbolTable[i].identifier == "$"){
            symbolTable.push_back(duo);
            break;
        }
        else if(symbolTable[i].identifier == currentToken.symbol){
            cout << "Already declared identifier on line " << currentToken.line << endl;
            break;
        }
    }
}

void toString(){
    for(int i = symbolTable.size() - 1; i >= 0; --i){
        cout << "Simbolo: " << symbolTable[i].identifier << "Tipo: " << symbolTable[i].type  << endl;
    }
}

void toStringPCT(){
    for(int i = PcT.size() - 1; i >= 0; --i){
        cout << "Tipo: " << PcT[i] << endl;
    }
}

//coloca um MARK pra sinalizar novo escopo
void enterScope(){
    //symbolTable.push_back("$");
    IdentifierAndType duo;
    duo.identifier = "$";
    duo.type = "mark";
    symbolTable.push_back(duo);
}

//para sair do escopo atual, deleta-se tudo até o primeiro MARK, inclusive o próprio MARK.
void exitScope(){
    while(symbolTable.back().identifier != "$"){
        symbolTable.pop_back();
    }
    symbolTable.pop_back();
}

void setTypeOfidentifiers(){
    for(int i = symbolTable.size() - 1; i >= 0; --i){
        if(symbolTable[i].type != "unset"){
            break;
        }
        else{
            if(currentToken.symbol == "integer")
                symbolTable[i].type = "Inteiro";
            else if(currentToken.symbol == "real")
                symbolTable[i].type = "Real";
            else if (currentToken.symbol == "real1")
                symbolTable[i].type == "Real 1";
            else if(currentToken.symbol == "Boolean")
                symbolTable[i].type = "Boolean";
            else
                symbolTable[i].type = "program";
        }
    }
}

string getIdentifierType(){
    for(int i = symbolTable.size() - 1; i >= 0; --i){
        if(symbolTable[i].identifier == currentToken.symbol){
            return symbolTable[i].type;
        }
    }
    cout << "No identifier with such name found" << endl;
    return " ";
}

void updatePcTArithmetic(){
    int top = PcT.size() - 1;
    int subtop = top - 1;

    if(PcT[top] == "Inteiro" && PcT[subtop] == "Inteiro"){
        PcT.pop_back();
    }
    else if(PcT[top] == "Inteiro" && PcT[subtop] == "Real"){
        PcT.pop_back();
    }
    else if(PcT[top] == "Real" && PcT[subtop] == "Inteiro"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Real");
    }
    else if(PcT[top] == "Real" && PcT[subtop] == "Real"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Real");
    }
    else{
        cout << "ERROR: Type mismatch (Arithmetic)\n" << endl;
    }
}

void updatePcTBoolean(){
    int top = PcT.size() - 1;
    int subtop = top - 1;

    if(PcT[top] == "Boolean" && PcT[subtop] == "Boolean"){
        PcT.pop_back();
    }
    else{
        cout << "ERROR: Type mismatch (Boolean)\n" << endl;
    }
}

void updatePcTRelational(){
    int top = PcT.size() - 1;
    int subtop = top - 1;

    if(PcT[top] == "Inteiro" && PcT[subtop] == "Inteiro"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Boolean");
    }
    else if(PcT[top] == "Inteiro" && PcT[subtop] == "Real"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Boolean");
    }
    else if(PcT[top] == "Real" && PcT[subtop] == "Inteiro"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Boolean");
    }
    else if(PcT[top] == "Real" && PcT[subtop] == "Real"){
        PcT.pop_back();
        PcT.pop_back();
        PcT.push_back("Boolean");
    }
    else{
        cout << "ERROR: Type mismatch (Relational)\n" << endl;
    }
}

void updatePcTAtribution(){
    int top = PcT.size() - 1;
    int subtop = top - 1;

    if(PcT[top] == "Inteiro" && PcT[subtop] == "Inteiro"){
        PcT.pop_back();
        PcT.pop_back();
    }
    else if(PcT[top] == "Inteiro" && PcT[subtop] == "Real"){
        PcT.pop_back();
        PcT.pop_back();
    }
    else if(PcT[top] == "Real" && PcT[subtop] == "Real"){
        PcT.pop_back();
        PcT.pop_back();
    }
    else if(PcT[top] == "Boolean" && PcT[subtop] == "Boolean"){
        PcT.pop_back();
        PcT.pop_back();
    }
    else if(PcT[top] == "Real 1" && PcT[subtop] == "Real 1"){
        PcT.pop_back();
        PcT.pop_back();
    }
    else{
        cout << "ERROR: Type mismatch (Atribution)\n" << endl;
    }
}
