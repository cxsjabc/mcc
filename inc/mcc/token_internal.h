// ** Never add #ifndef/#ifdef, because it will be included more than once. **
#ifndef DEF_TOK
#define DEF_TOK(tok, str)
#endif

DEF_TOK(TOK_RESERVE, "") // this placeholder is a must, let keywords TOK_xxx > 0
// keywords
DEF_TOK(TOK_AUTO, "auto")
DEF_TOK(TOK_BREAK, "break")
DEF_TOK(TOK_CASE, "case")
DEF_TOK(TOK_CHAR, "char")
DEF_TOK(TOK_CONST, "const")
DEF_TOK(TOK_CONTINUE, "continue")
DEF_TOK(TOK_DEFAULT, "default")
DEF_TOK(TOK_DO, "do")
DEF_TOK(TOK_DOUBLE, "double")
DEF_TOK(TOK_ELSE, "else")
DEF_TOK(TOK_ENUM, "enum")
DEF_TOK(TOK_EXTERN, "extern")
DEF_TOK(TOK_FLOAT, "float")
DEF_TOK(TOK_FOR, "for")
DEF_TOK(TOK_GOTO, "goto")
DEF_TOK(TOK_IF, "if")
DEF_TOK(TOK_INT, "int")
DEF_TOK(TOK_LONG, "long")
DEF_TOK(TOK_REGISTER, "register")
DEF_TOK(TOK_RETURN, "return")
DEF_TOK(TOK_SHORT, "short")
DEF_TOK(TOK_SIGNED, "signed")
DEF_TOK(TOK_SIZEOF, "sizeof")
DEF_TOK(TOK_STATIC, "static")
DEF_TOK(TOK_STRUCT, "struct")
DEF_TOK(TOK_SWITCH, "switch")
DEF_TOK(TOK_TYPEDEF, "typedef")
DEF_TOK(TOK_UNION, "union")
DEF_TOK(TOK_UNSIGNED, "unsigned")
DEF_TOK(TOK_VOID, "void")
DEF_TOK(TOK_VOLATILE, "volatile")
DEF_TOK(TOK_WHILE, "while")

// literal number or string
DEF_TOK(TOK_LITERAL, "")

// identifiers
DEF_TOK(TOK_IDENTIFIER, "identifier")

// preprocess token
DEF_TOK(TOK_PREP, "#")

DEF_TOK(TOK_NOT, "!")
DEF_TOK(TOK_QUOTE, "\"")
DEF_TOK(TOK_MOD, "%%")
DEF_TOK(TOK_AND, "&")
DEF_TOK(TOK_SQUOTE, "\'")
DEF_TOK(TOK_LPAREN, "(")
DEF_TOK(TOK_RPAREN, ")")
DEF_TOK(TOK_STAR, "*")
DEF_TOK(TOK_PLUS, "+")
DEF_TOK(TOK_COMMA, ",")
DEF_TOK(TOK_MINUS, "-")
DEF_TOK(TOK_DOT, ".")
DEF_TOK(TOK_DIV, "/")
DEF_TOK(TOK_COLON, ":")
DEF_TOK(TOK_SEMICOLON, ";")
DEF_TOK(TOK_LESS, "<")
DEF_TOK(TOK_ASSIGN, "=")
DEF_TOK(TOK_GREATER, ">")
DEF_TOK(TOK_QUESTION, "?")
DEF_TOK(TOK_LBRACKET, "[")
DEF_TOK(TOK_BACKSLASH, "\\") 
DEF_TOK(TOK_RBRACKET, "]")
DEF_TOK(TOK_XOR, "^")
DEF_TOK(TOK_LBRACE, "{")
DEF_TOK(TOK_OR, "|")
DEF_TOK(TOK_RBRACE, "}")
DEF_TOK(TOK_TILDE, "~")

// End of File
DEF_TOK(TOK_EOF, "EOF")

// Error token, this is not identifier "ERROR", it means wrong token!
DEF_TOK(TOK_ERROR, "ERROR")
