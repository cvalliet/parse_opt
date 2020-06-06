#include <cstdlib>
#include <iostream>

class arguments
{
private:
    int    argc_;
    char** argv_;

public:
    class iterator: public std::iterator<std::output_iterator_tag,
        char**, char**, const char ***, char**>
    {
    private:
        char** p_;

    public:
        explicit iterator(char** p): p_{p} {}

        iterator& operator++() { ++p_; return *this; }
        bool operator==(iterator other) const { return p_ == other.p_; }
        bool operator!=(iterator other) const { return !(*this == other); }
        reference operator*() const { return p_; }
    };

    explicit arguments(int argc, char** argv):
        argc_{argc}, argv_{argv}
    {}

    iterator begin() { return iterator(argv_); }
    iterator end() { return iterator(argv_ + argc_); }
};

int main(int argc, char* argv[])
{
    arguments args(argc, argv);

    for (auto arg: args) {
        std::cout << "Arg : " << *arg << std::endl; 
    }

    return EXIT_SUCCESS;
}