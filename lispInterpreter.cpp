//
//  lispInterpreter.cpp
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#include <iostream>
#include <fstream>

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable.h"
#include "EvaluationVisitor.h"
#include "PrintVisitor.h"

int main(int argc, char* argv[])
{
    // Check if there is at least one input argument
    // The first input argument (argv[0]) is always the name of the program
    if (argc < 2) {
        std::cerr << "No input file!" << std::endl;
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " <filename> " << std::endl;
        return EXIT_FAILURE;
    }

    // Try to open the file to be interpreted
    std::ifstream inputFile;
    try {
        inputFile.open(argv[1]);
        if (!inputFile.is_open()) {
            std::cerr << "Cannot open " << argv[1] << std::endl;
            return EXIT_FAILURE;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Cannot open " << argv[1] << " got: " << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Lexical analysis
    Lexer tokenize;
    std::vector<Token> inputTokens;
    // Extract a token stream from the input stream
    try {
        // Avoid copying the stream of tokens
        inputTokens = std::move(tokenize(inputFile));
    }
    catch (LexicalError& e) {
        std::cerr << "(ERROR in tokenizer: " << e.what() << " )" << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& e) {
        std::cerr << "Cannot read from " << argv[1] << " got: " << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Syntactical analysis
    Parser parse;
    Program* program = nullptr;
    try {
        program = parse(inputTokens);
    }
    catch (SyntaxError& e) {
        std::cerr << "(ERROR in parser: " << e.what() << " )" << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& e) {
        std::cerr << "Something odd happened during parsing, got: " << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Semantical analysis (evaluation)
    SymbolTable symbolTable;
    EvaluationVisitor evaluator{ symbolTable, std::cin, std::cout };
    try {
        if (program != nullptr) {
            program->accept(evaluator);
        }
    }
    catch (EvaluationError& e) {
        std::cerr << "(ERROR in evaluator: " << e.what() << " )" << std::endl;
        
        return EXIT_FAILURE;
    }
    catch (std::exception& e) {
        std::cerr << "Something odd happened during evaluation, got: " << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
