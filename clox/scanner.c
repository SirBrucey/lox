//
// Created by Brucey on 01/07/2024.
//

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

#include <stdlib.h>

typedef struct {
    const char *start;
    const char *current;
    int line;
} Scanner;

Scanner scanner;

void initScanner(const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isAlpha(const char c) {
    return
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}


static bool isDigit(const char c) {
    return c >= '0' && c <='9';
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

static Token mkToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static char peek(void) {
    return *scanner.current;
}

static char peekNext(void) {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;
    scanner.current++;
    return true;
}

static Token errorToken(const char *msg) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = msg;
    token.length = (int)strlen(msg);
    token.line = scanner.line;
    return token;
}

static void skipWhitespace(void) {
    for (;;) {
        const char c = peek();
        switch (c) {
            // Inline whitespace
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            // newlines
            case '\n':
                scanner.line++;
                advance();
                break;
            // Comments are not whitespace, but we treat them the same.
            case '/':
                if (peekNext() == '/') {
                    // A comment goes to the end of the line.
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return;
                }
            default:
                return;
        }
    }
}

static TokenType checkKeyword(int start, int length, const char *rest, TokenType type) {
    if (
        scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start , rest , length) == 0
    ) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static TokenType identifierType(void) {
    switch (scanner.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f': {
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        }
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't': {
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        }
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier(void) {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    return mkToken(identifierType());
}

static Token string(void) {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    // Closing quote.
    advance();
    return mkToken(TOKEN_STRING);
}

static Token number(void) {
    while (isDigit(peek()) && !isAtEnd()) advance();

    // Look for fractional
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume `.`
        advance();

        while (isDigit(peek())) advance();
    }

    return mkToken(TOKEN_NUMBER);
};

Token scanToken() {
    skipWhitespace();
    scanner.start = scanner.current;

    if (isAtEnd()) return mkToken(TOKEN_EOF);

    const char c = advance();

    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        case '(': return mkToken(TOKEN_LEFT_PAREN);
        case ')': return mkToken(TOKEN_RIGHT_PAREN);
        case '{': return mkToken(TOKEN_LEFT_BRACE);
        case '}': return mkToken(TOKEN_RIGHT_BRACE);
        case '.': return mkToken(TOKEN_DOT);
        case ',': return mkToken(TOKEN_COMMA);
        case ';': return mkToken(TOKEN_SEMICOLON);
        case '+': return mkToken(TOKEN_PLUS);
        case '-': return mkToken(TOKEN_MINUS);
        case '*': return mkToken(TOKEN_STAR);
        case '/': return mkToken(TOKEN_SLASH);
        case '!':
            return mkToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return mkToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return mkToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return mkToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
        default: {
            return errorToken("Unexpected characters.");
        };
    }

}
