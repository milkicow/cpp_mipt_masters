#include <cstdio>
int main() { const char* s = "#include <cstdio>%cint main() { const char* s = %c%s%c; std::printf(s, 10, 34, s, 34, 10); }%c"; std::printf(s, 10, 34, s, 34, 10); }
