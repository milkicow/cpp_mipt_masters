#include <iostream>
#include <source_location>

class Tracer {
   private:
    std::source_location location;

   public:
    explicit Tracer(const std::source_location& loc = std::source_location::current())
        : location(loc) {
        std::cout << "Entering function: " << location.function_name() << " at "
                  << location.file_name() << ":" << location.line() << std::endl;
    }

    ~Tracer() {
        std::cout << "Exiting function:  " << location.function_name() << " at "
                  << location.file_name() << ":" << location.line() << std::endl;
    }

    Tracer(const Tracer&) = delete;
    Tracer& operator=(const Tracer&) = delete;

    Tracer(Tracer&&) = default;
    Tracer& operator=(Tracer&&) = default;
};

#ifdef NDEBUG
#define trace ((void)0)
#else
#define trace Tracer __tracer_##__LINE__
#endif

void recursive_function(int depth) {
    trace;
    if (depth > 0) {
        std::cout << "  Depth: " << depth << std::endl;
        recursive_function(depth - 1);
    }
}

int main() {
    trace;
    recursive_function(3);
    return 0;
}