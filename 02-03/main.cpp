#include <iostream>

static void deduce_char(char ch) {
    switch (ch) {
        case 'A' ... 'Z':
            std::cout << "upper case letter\n";
            break;
        case 'a' ... 'z':
            std::cout << "lower case letter\n";
            break;
        case '0' ... '9':
            std::cout << "digit\n";
            break;
        case ' ':
        case '!':
        case '"':
        case '#':
        case '$':
        case '%':
        case '&':
        case '\'':
        case '(':
        case ')':
        case '*':
        case '+':
        case ',':
        case '-':
        case '.':
        case '/':
        case ':':
        case ';':
        case '<':
        case '=':
        case '>':
        case '?':
        case '@':
        case '[':
        case '\\':
        case ']':
        case '^':
        case '_':
        case '`':
        case '{':
        case '|':
        case '}':
        case '~':
            std::cout << "punctuation\n";
            break;
        default:
            std::cout << "other\n";
            break;
    }
}

int main() {
    char ch;
    std::cin >> ch;

    if (ch < 32 || ch > 127) {
        std::cout << "char is out of range [32, 127]\n";
        return EXIT_FAILURE;
    }

    deduce_char(ch);

    return 0;
}