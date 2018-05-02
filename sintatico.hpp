#include "semantico.hpp"

void getSymbol(){
    if(currentIndex != tokenList.size()){
      currentToken = tokenList[currentIndex];
      ++currentIndex;
    }
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
        --currentIndex;
        return 0;
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
        --currentIndex;
        return 0;
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
        --currentIndex;
        return 0;
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
        --currentIndex;
        return 0;
    }
}

int fator()
{
    getSymbol();
    if( currentToken.TokenType == "Inteiro" ||
        currentToken.TokenType == "Real" ||
        currentToken.TokenType == "Real 1" ||
        currentToken.symbol == "true" ||
        currentToken.symbol == "false"
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
                    --currentIndex;
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            --currentIndex;
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
                --currentIndex;
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        --currentIndex;
        return 0;
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
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
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
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
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
        return 0;
    }
}

int expressao()
{
    if(expressaoSimples())
    {
        if(opRelacional() && expressaoSimples())
        {
            return 1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

int listaDeExpressoes_()
{
    getSymbol();
    if(currentToken.symbol == ",")
    {
        if(expressao())
        {
            return listaDeExpressoes_();
        }
        else
        {
            return 0;
        }
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int listaDeExpressoes()
{
    return (expressao() && listaDeExpressoes_());
}

int ativacaoDeProcedimento()
{
    if(currentToken.symbol == "(")
    {
        if(listaDeExpressoes())
        {
            getSymbol();
            if(currentToken.symbol == ")")
            {
                return 1;
            }
            else
            {
                --currentIndex;
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
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
        --currentIndex;
        return 0;
    }
}

int parteElse()
{
    getSymbol();
    if(currentToken.symbol == "else")
    {
        return comando();
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int comando()
{
    if(variavel())
    {
        getSymbol();
        if(currentToken.TokenType == "Atribuição")
        {
            return expressao();
        }
        else if (ativacaoDeProcedimento())
        {
            return 1;
        }
        else
        {
            --currentIndex;
            return 0;
        }
    }
    else if (comandoComposto())
    {
        return 1;
    }
    else
    {
        getSymbol();
        if(currentToken.symbol == "if")
        {
            if(expressao())
            {
                getSymbol();
                if(currentToken.symbol == "then")
                {
                    return comando() && parteElse();
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else if (currentToken.symbol == "while")
        {
            if(expressao())
            {
                getSymbol();
                if(currentToken.symbol == "do")
                {
                    return comando();
                }
                else
                {
                    --currentIndex;
                    return 0;
                }
            }
            else
            {
                --currentIndex;
                return 0;
            }
        }
        else
        {
            --currentIndex;
            return 0;
        }
    }
  }

int listaDeComandos_()
{
    getSymbol();
    if(currentToken.symbol == ";")
    {
        return comando() && listaDeComandos_();
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int listaDeComandos()
{
    return comando() && listaDeComandos_();
}

int comandosOpcionais()
{
    getSymbol();
    if(currentToken.symbol == "end")
    {
        --currentIndex;
        return 1;
    }
    --currentIndex;
    return listaDeComandos();
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
                --currentIndex;
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        --currentIndex;
        return 0;
    }
}

int listaDeParametros_()
{
    getSymbol();
    if(currentToken.symbol == ";")
    {
        if(listaDeIdentificadores())
        {
            getSymbol();
            if(currentToken.symbol == ":")
            {
                if(tipo())
                {
                    return listaDeParametros_();
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int listaDeParametros()
{
    if(listaDeIdentificadores())
    {
        getSymbol();
        if(currentToken.symbol == ":")
        {
            if(tipo())
            {
                return listaDeParametros_();
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
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
                --currentIndex;
                return 0;
            }
        }
        else
        {
            return 0;
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
    getSymbol();
    if(currentToken.symbol == "procedure")
    {
        getSymbol();
        if(currentToken.TokenType == "Identificador")
        {
            if(argumentos())
            {
                getSymbol();
                if(currentToken.symbol == ";")
                {
                    if(declaracoesVariaveis())
                    {
                        if(declaracoesDeSubprogramas())
                        {
                            if(comandoComposto())
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    --currentIndex;
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            --currentIndex;
            return 0;
        }
    }
    else
    {
        --currentIndex;
        return 0;
    }
}

int declaracoesDeSubprogramas_()
{
    if(declaracaoDeSubprograma())
    {
        getSymbol();
        if(currentToken.symbol == ";")
        {
            return declaracoesDeSubprogramas_();
        }
        else
        {
            --currentIndex;
            return 0;
        }
    }
    else
    {
        return 1;
    }
}

int declaracoesDeSubprogramas()
{
    return declaracoesDeSubprogramas_();
}

int tipo()
{
    getSymbol();
    if( currentToken.symbol == "integer" ||
        currentToken.symbol == "real" ||
        currentToken.symbol == "real1" ||
        currentToken.symbol == "boolean")
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int listaDeIdentificadores_()
{
    getSymbol();
    if(currentToken.symbol == ",")
    {
        getSymbol();
        if(currentToken.TokenType == "Identificador")
        {
            return listaDeIdentificadores_();
        }
        else
        {
            return 0;
        }
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int listaDeIdentificadores()
{
    getSymbol();
    if(currentToken.TokenType == "Identificador")
    {
        return listaDeIdentificadores_();
    }
    else
    {
        --currentIndex;
        return 0;
    }
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
                    return listaDeclaracaoVariaveis_();
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            --currentIndex;
            return 0;
        }
    }
    else
    {
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
                    --currentIndex;
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            --currentIndex;
            return 0;
        }
    }
    else
    {
        return 0;
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
        --currentIndex;
        return 1;
    }
}

int programa()
{

    cout << "Simbolos na tabela: " << tokenList.size() << endl;

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
                    if (declaracoesDeSubprogramas())
                    {
                        if (comandoComposto())
                        {
                            getSymbol();

                            if(currentToken.symbol == ".")
                            {
                                cout << "Programa Sintaticamente Correto! " << endl;
                                return 1;
                            }
                            else
                            {
                                --currentIndex;
                                getSymbol();
                                cout << "ERROR on line " << currentToken.line << endl;
                                cout << "Expected '.' before the end of the file" << endl;
                                return 0;
                            }
                        }
                        else
                        {
                            cout << "ERROR on line " << currentToken.line << endl;
                            return 0;
                        }
                    }
                    else
                    {
                        cout << "ERROR on line " << currentToken.line << endl;
                        return 0;
                    }
                }
                else
                {
                    cout << "ERROR on line " << currentToken.line << endl;
                    return 0;
                }
            }
            else
            {
                cout << "ERROR on line " << currentToken.line << endl;
                cout << "Expected ';'" << endl;
                return 0;
            }
        }
        else
        {
            cout << "ERROR on line " << currentToken.line << endl;
            cout << "Expected a program identifier" << endl;
            return 0;
        }
    }
    else
    {
        cout << "ERROR on line " << currentToken.line << endl;
        cout << "Expected keyword 'program'" << endl;
        return 0;
    }
}

int checkTable()
{
    return programa();
}

/*
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
        PcT.push_back(currentToken.TokenType);
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
            updatePcTArithmetic();                              //////////////////////////////////////////////
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
        updatePcTAtribution();                              ///////////////////////////////////////////
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
        PcT.push_back(getIdentifierType());
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
        currentToken.symbol == "Boolean"){
        setTypeOfidentifiers();
        return 1;
    }
    else{
        return erro("Expected 'integer', 'real', 'real1', or 'Boolean'");
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
*/
