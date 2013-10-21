// ============================================================================ 
// main.cpp
// ~~~~~~~~
// Hung Q. ngo
// Description: get a user's command of the form
// - new listname = [list of non-negative integers separated by space]
// - print listname
// - sort listname
// - merge listname1 listname2
// - remdup listname
// - keepcommon listname1 listname2
// - exit
// their meanings are described in the assignment 6 specification
// ****************************************************************************
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>  // for exit()

#include "listmanip.h"
#include "Lexer.h"
#include "error_handling.h"
#include "term_control.h"

using namespace std;

// -----------------------------------------------------------------------------
// prototypes, typedefs, and global variables
// -----------------------------------------------------------------------------
typedef void (*cmd_t)(Lexer);
void new_list_cmd(Lexer);
void print_list_cmd(Lexer);
void sort_list_cmd(Lexer);
void merge_list_cmd(Lexer);
void remdup_cmd(Lexer);
void keep_common_cmd(Lexer);
void bye(Lexer);    // simply quit
void prompt();
map<string, Node*> list_table;
extern const string usage_msg;                                                  

/**
 * -----------------------------------------------------------------------------
 * main body
 * -----------------------------------------------------------------------------
 */
int main() {
    Lexer lexer; string line; Token tok;
    map<string,cmd_t> cmd_map;
    cmd_map["exit"]  = &bye;
    cmd_map["bye"]   = &bye;
    cmd_map["quit"]  = &bye;
    cmd_map["new"]   = &new_list_cmd;
    cmd_map["print"] = &print_list_cmd;
    cmd_map["merge"] = &merge_list_cmd;
    cmd_map["sort"]  = &sort_list_cmd;
    cmd_map["remdup"] = &remdup_cmd;
    cmd_map["keepcommon"] = &keep_common_cmd;

    cout << term_cc(YELLOW) << usage_msg << endl;

    while (cin) {
        prompt(); getline(cin, line); lexer.set_input(line);
        if (!lexer.has_more_token()) continue;
        tok = lexer.next_token();
        if (tok.type != IDENT) { error_return("Syntax error\n"); continue; }

        if (cmd_map.find(tok.value) != cmd_map.end()) {
            try {
                cmd_map[tok.value](lexer);
            } catch (runtime_error &e) {
                error_return(e.what());
            }
        } else {
            error_return("Unknown command");
        }
    }
    return 0;

}

/**
 * -----------------------------------------------------------------------------
 * process command 'remdup listname'
 * -----------------------------------------------------------------------------
 */
void remdup_cmd(Lexer lex) 
{
    Token tok = lex.next_token();
    if (tok.type != IDENT || lex.has_more_token())
        throw runtime_error("SYNTAX: remdup listname");
    if (list_table.find(tok.value) == list_table.end())
        throw runtime_error(tok.value + " not defined or already destroyed");
    list_table[tok.value] = remove_duplicates(list_table[tok.value]);
}


/**
 * -----------------------------------------------------------------------------
 * process command 'print listname'
 * -----------------------------------------------------------------------------
 */
void print_list_cmd(Lexer lex) 
{
    Token tok = lex.next_token();
    if (tok.type != IDENT || lex.has_more_token())
        throw runtime_error("SYNTAX: print listname");
    if (list_table.find(tok.value) == list_table.end())
        throw runtime_error(tok.value + " not defined or already destroyed");
    print_list(list_table[tok.value]);
}


/**
 * -----------------------------------------------------------------------------
 * process command 'keepcommon listname1 listname2'
 * -----------------------------------------------------------------------------
 */
void keep_common_cmd(Lexer lex) 
{
    vector<Token> vec = lex.tokenize();
    if (vec.size() != 2 || vec[0].type != IDENT || vec[1].type != IDENT)
        throw runtime_error("SYNTAX: keepcommon listname1 listname2");

    if (list_table.find(vec[0].value) == list_table.end())
        throw runtime_error(string("The list " + vec[0].value + 
                                   " doesn't exist"));
    if (list_table.find(vec[1].value) == list_table.end())
        throw runtime_error(string("The list " + vec[1].value + 
                                   " doesn't exist"));

    list_table[vec[0].value] = keep_common(list_table[vec[0].value],
                                           list_table[vec[1].value]);
}

/**
 * -----------------------------------------------------------------------------
 * process command 'merge listname1 listname2'
 * -----------------------------------------------------------------------------
 */
void merge_list_cmd(Lexer lex) 
{
    vector<Token> vec = lex.tokenize();
    if (vec.size() != 2 || vec[0].type != IDENT || vec[1].type != IDENT)
        throw runtime_error("SYNTAX: keepcommon listname1 listname2");

    if (list_table.find(vec[0].value) == list_table.end() ||
        list_table.find(vec[1].value) == list_table.end())
        throw runtime_error("Give me existing lists only");

    list_table[vec[0].value] = merge_lists(list_table[vec[0].value],
                                           list_table[vec[1].value]);
    list_table.erase(vec[1].value);
}

/**
 * -----------------------------------------------------------------------------
 * process command 'new listname 123 345 345'
 * -----------------------------------------------------------------------------
 */
void new_list_cmd(Lexer lex) 
{
    Token tok = lex.next_token();
    if (tok.type != IDENT)
        throw runtime_error("new listname [list of int]");

    if (list_table.find(tok.value) != list_table.end())
        throw runtime_error(tok.value + " already exists");

    vector<Token> tok_vec = lex.tokenize();
    if (tok_vec.size() == 0)
        throw runtime_error("Please enter some integers");
    for (size_t i=0; i < tok_vec.size(); i++)
        if (tok_vec[i].type != INTEGER)
            throw runtime_error("Please enter integers only");
    
    list_table[tok.value] = new_list(tok_vec);
}

/**
 * -----------------------------------------------------------------------------
 * process command 'sort listname'
 * -----------------------------------------------------------------------------
 */
void sort_list_cmd(Lexer lexer) 
{
    Token tok = lexer.next_token();

    if (tok.type != IDENT || lexer.has_more_token())
        throw runtime_error("Syntax error: sort <listname>");

    if (list_table.find(tok.value) == list_table.end())
        throw runtime_error("List not found");

    Node* my_list = list_table[tok.value];
    cout << term_cc(CYAN) << "BEFORE: " << term_cc(); 
    print_list(my_list);
    my_list = sort_list(my_list);
    cout << term_cc(CYAN) << "AFTER: " << term_cc(); 
    print_list(my_list);
    list_table[tok.value] = my_list;
}

/**
 * -----------------------------------------------------------------------------
 * terminates the program, ignores all parameters
 * -----------------------------------------------------------------------------
 */
void bye(Lexer lexer) 
{
    if (lexer.has_more_token()) {
        throw runtime_error("Syntax error: use bye/exit/quit");
    } else {
        exit(0);
    }
}

/**                                                                             
 * -----------------------------------------------------------------------------
 * print the list                                                               
 * -----------------------------------------------------------------------------
 */                                                                             
void print_list(Node* ptr) {                                                    
    cout << term_cc(CYAN);                                                      
    while (ptr != NULL) {                                                       
        cout << ptr->key << " ";                                                
        ptr = ptr->next;                                                        
    }                                                                           
    cout << endl << term_cc();                                                  
} 

/**
 * -----------------------------------------------------------------------------
 * just print a prompt.
 * -----------------------------------------------------------------------------
 */
void prompt() 
{
    cout << term_cc(BLUE) << "> " << term_cc() << flush;
}
