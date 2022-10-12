/**************************************************************************
    Name: Brennan Basinger
    Programming Environment: VisualStudio Code - Ubuntu 20.04.2 ARM64
    Purpose of File: Productions for Recursive Descent Parser
		Part 2 of writing my own Interpreter
***************************************************************************/

#ifndef PRODUCTIONS_H
#define PRODUCTIONS_H
#include <iostream>

extern set<string> symbolTable;
int indentationLevel = 0;  // used to indent outputLexeme to approximate parse tree
int nextToken = 0;  // token returned from yylex

extern "C"
{
	// Instantiate global variables used by flex
	extern char* yytext;       // text of current lexeme
	extern int   yylex();      // the generated lexical analyzer
}

// Forward declarations of production parsing functions
void program();
bool statement();
bool readStatement();
bool writeStatement();
bool whileStatement();
bool ifStatement();
bool simpleExpression();
bool compound();
bool term();
bool factor();
bool assignment();
bool expression();
bool block();

// Forward declarations of functions that check whether the current token is
// in the first set of a production rule
bool first_of_program();
bool firstFactor();

inline void indent()
{
    for (int x=0; x<indentationLevel; x++)
    {
        cout << ("    ");
    }
}


void printEnterAndType(string type)
{
    indent();
    cout << "enter <" << type << ">"  << "\n";
    ++indentationLevel;
}


void outputLexeme()
{
    indent();
    cout << "-->found " << yytext << "\n";
    nextToken = yylex();
}


void printExitAndType(string type)
{
    --indentationLevel;
    indent();
    cout << "exit <" << type << ">" << "\n";
}


bool firstFactor()
{
    return
        nextToken == TOK_IDENT || nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT ||
        nextToken == TOK_OPENPAREN || nextToken == TOK_NOT || nextToken == TOK_MINUS;
}

bool first_of_program(void)
{
    return nextToken == TOK_PROGRAM;
}


void program()
{
    if (!first_of_program()) // Check for PROGRAM
    {
        throw "3: 'PROGRAM' expected";
    }
    indent();
    cout << "enter <program>" << "\n";
    ++indentationLevel;
    // TODO: finish this parsing function...
    outputLexeme();
    if (nextToken == TOK_IDENT)
    {
        outputLexeme();
        if(nextToken == TOK_SEMICOLON)
        {
            outputLexeme();
            if(!block())
            {
                return;
            }
        }
        else
        {
            throw "14: ';' expected";
        }
    }
    else
    {
        throw "2: identifier expected";
    }
    //Remaining Starting Point Code for program()
    --indentationLevel;
    indent();
    cout << "exit <program>" << "\n";
}


bool statement()
{
    printEnterAndType("statement");
    bool is_correct;
    switch(nextToken)
    {
        case TOK_IDENT:
            is_correct = assignment();
            break;
        case TOK_READ:
            is_correct = readStatement();
            break;
        case TOK_WRITE:
            is_correct = writeStatement();
            break;
        case TOK_WHILE:
            is_correct = whileStatement();
            break;
        case TOK_IF:
            is_correct = ifStatement();
            break;
        case TOK_BEGIN:
            is_correct = compound();
            break;
        default:
            throw "900: illegal type of statement";
            break;
    }
    if(is_correct == false)
    {
        return false;
    }
    printExitAndType("statement");
    return is_correct;
}


bool readStatement()
{
    bool val;
    if(nextToken == TOK_READ)
    {
        printEnterAndType("read");
        outputLexeme();

        if(nextToken == TOK_OPENPAREN)
        {
            outputLexeme();

            if(nextToken == TOK_IDENT)
            {
                outputLexeme();
                if(nextToken == TOK_CLOSEPAREN)
                {
                    outputLexeme();
                    val = true;
                }
                else
                {
                    throw "4: ')' expected";
                }
            }
            else
            {
                throw "2: identifier expected";
            }
        }
        else
        {
            throw "9: '(' expected";
        }
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("read");
    return val;
}


bool writeStatement()
{
    bool val;
    if(nextToken == TOK_WRITE)
    {
        printEnterAndType("write");
        outputLexeme();
        if(nextToken == TOK_OPENPAREN)
        {
            outputLexeme();
            if(nextToken == TOK_STRINGLIT ||nextToken == TOK_IDENT)
            {
                outputLexeme();
                if(nextToken == TOK_CLOSEPAREN)
                {
                    outputLexeme();
                    val = true;
                }
                else
                {
                    throw "4: ')' expected";
                }
            }
            else
            {
                throw "134: illegal type of operand(s)";
            }
        }
        else
        {
            throw "9: '(' expected";
        }
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("write");
    return val;
}


bool whileStatement()
{
    bool val;
    if(nextToken == TOK_WHILE)
    {
        printEnterAndType("while statement");
        outputLexeme();
        if(expression())
        {
            if(statement())
            {
                val = true;
            }
            else
            {
                val = false;
            }
        }
        else
        {
            val = false;
        }
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("while statement");
    return val;
}


bool ifStatement()
{
    bool val;
    if(nextToken == TOK_IF)
    {
        printEnterAndType("if statement");
        outputLexeme();
        if(expression())
        {
            if(nextToken == TOK_THEN)
            {
                outputLexeme();
                if(statement())
                {
                    if(nextToken == TOK_ELSE)
                    {
                        outputLexeme();
                        if(statement())
                        {
                            val = true;
                        }
                    }
                    else
                    {
                        val = true;
                    }
                }
                else
                {
                    val = false;
                }
            }
            else
            {
                throw "52: 'THEN' expected";
            }
        }
        else
        {
            val = false;
        }
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("if statement");
    return val;
}


bool simpleExpression()
{
    bool val;
    printEnterAndType("simple expression");
    if(!firstFactor())
    {
        throw "901: illegal type of simple expression";
    }
    if(term())
    {
        if(nextToken == TOK_OR || nextToken == TOK_PLUS || nextToken == TOK_MINUS)
        {
            outputLexeme();
            if(term())
            {
                val = true;
            }
            else
            {
                val = false;
            }
        }
        else
        {
            val = true;
        }
    }
    else
    {
        val = false;
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("simple expression");
    return val;
}


bool compound()
{
    bool val;
    if(nextToken == TOK_BEGIN)
    {
        printEnterAndType("compound_statement");
        outputLexeme();
        if(statement())
        {
            while(nextToken == TOK_SEMICOLON)
            {
                outputLexeme();
                if(!statement())
                {
                    val = false;
                    break;
                }
            }
            if(nextToken == TOK_END)
            {
                outputLexeme();
                val = true;
            }
            else
            {
                throw "13: 'END' expected";
            }
        }
        else
        {
            val = false;
        }
    }
    else
    {
        throw "17: 'BEGIN' expected";
    }
    if (val == false)
    {
        return false;
    }
    printExitAndType("compound_statement");
    return val;
}


bool term()
{
    bool val;
    printEnterAndType("term");
    if(!firstFactor())
    {
        throw "902: illegal type of term";
    }
    if(factor())
    {
        if(nextToken == TOK_AND || nextToken == TOK_DIVIDE || nextToken == TOK_MULTIPLY)
        {
            do
            {
                outputLexeme();
                if(factor())
                {
                    val = true;
                }
                else
                {
                    val = false;
                }
            }
            while(nextToken == TOK_AND || nextToken == TOK_DIVIDE || nextToken == TOK_MULTIPLY);
        }
        else
        {
            val = true;
        }
    }
    else
    {
        val = false;
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("term");
    return val;
}


bool factor()
{
    symbolTable.find(yytext);
    bool val;
    printEnterAndType("factor");
    if(!firstFactor())
    {
        throw "903: illegal type of factor";
    }
    if(nextToken == TOK_FLOATLIT || nextToken == TOK_INTLIT)
    {
        outputLexeme();
        val = true;
    }
    else if(symbolTable.find(yytext) != symbolTable.end() && nextToken == TOK_IDENT)
    {
        outputLexeme();
        val = true;
    }
    else if(nextToken == TOK_IDENT)
    {
        throw "104: identifier not declared";
    }
    else if(nextToken == TOK_OPENPAREN)
    {
        outputLexeme();
        if (expression())
        {
            if(nextToken == TOK_CLOSEPAREN)
            {
                outputLexeme();
                val = true;
            }
            else
            {
                throw "4: ')' expected";
            }
        }
        else
        {
            val = false;
        }
    }
    else if(nextToken == TOK_MINUS || nextToken == TOK_NOT)
    {
        outputLexeme();
        if(factor())
        {
            val = true;
        }
        else
        {
            val = false;
        }
    }
    else
    {
        val = false;
    }

    if (val == false)
    {
        return false;
    }
    printExitAndType("factor");
    return val;
}


bool assignment()
{
    bool val;
    if(nextToken == TOK_IDENT)
    {
        printEnterAndType("assignment");
        outputLexeme();
        if(nextToken == TOK_ASSIGN)
        {
            outputLexeme();
            if(expression())
            {
                val = true;
            }
            else
            {
                val = false;
            }
        }
        else
        {
            throw "51: ':=' expected";
        }
    }
    else
    {
        throw "2: identifier expected";
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("assignment");
    return val;
}


bool expression()
{
    bool val;
    printEnterAndType("expression");
    if(!firstFactor())
    {
        throw "144: illegal type of expression";
    }
    if(simpleExpression())
    {
        if(nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_EQUALTO || nextToken == TOK_NOTEQUALTO)
        {
            outputLexeme();
            if(simpleExpression())
            {
                val = true;
            }
            else
            {
                val = false;
            }
        }
        else
        {
            val = true;
        }
    }
    else
    {
        val = false;
    }

    if(val == false)
    {
        return false;
    }
    printExitAndType("expression");
    return val;
}


bool block()
{
    bool val;
    printEnterAndType("block");
    if(nextToken != TOK_BEGIN && nextToken != TOK_VAR)
    {
        throw "18: error in declaration part OR 17: 'BEGIN' expected";
    }
    if(nextToken == TOK_VAR)
    {
        outputLexeme();
        if(nextToken == TOK_IDENT)
        {
            do
            {
                if(symbolTable.find(yytext) == symbolTable.end())
                {
                    symbolTable.insert(yytext);
                }
                else
                {
                    throw "101: identifier declared twice";
                }
                outputLexeme();
                if(nextToken == TOK_COLON)
                {
                    outputLexeme();
                    if(nextToken == TOK_REAL || nextToken == TOK_INTEGER)
                    {
                        outputLexeme();
                        if(nextToken == TOK_SEMICOLON)
                        {
                            outputLexeme();
                        }
                        else
                        {
                            throw "14: ';' expected";
                        }
                    }
                    else
                    {
                        throw "10: error in type";
                    }
                }
                else
                {
                    throw "5: ':' expected";
                    break;
                }
                cout << "\n";
            }
            while(nextToken == TOK_IDENT);
        }
        else
        {
            throw "2: identifier expected";
        }
    }
    if(compound())
    {
        val = true;
    }
    else
    {
        val = false;
    }

    if(val == false)
    {
        return false;
    }
    printExitAndType("block");
    return true;
}

#endif
