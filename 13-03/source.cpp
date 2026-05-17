// comment at top of file

/* multi-line
   block comment */

#include <iostream>
#include <string>

// another comment
int main() /* inline block comment */
{
    // comment inside function

    int x = 42; // trailing comment

    auto s1 = "hello // not a comment";
    auto s2 = "world /* also not a comment */";

    // raw string: comment-like content and blank lines must be preserved
    auto raw1 = R"(
line inside raw string
// fake comment inside raw
/* fake block inside raw */

blank line above must be preserved
)";

    // raw string with custom delimiter: ) and " inside are safe
    auto raw2 = R"XY(has ) inside and " quotes safely)XY";



    return 0; // end
}

// trailing comment at end of file

