#include <cstdlib>
#include <iostream>

class arguments
{
private:
    int    argc_;
    char** argv_;

public:
    class parameter
    {
    private:
        char** p_;
        const std::string* opt_;

    public:
        parameter(char** p, const std::string* opt = nullptr):
            p_{p}, opt_{opt}
        {}

        const char* content() const
        {
            if (is_short_opt())
                return *p_ + 1;

            if (is_long_opt())
                return *p_ + 2;
                
            return *p_;
        }

        const char* option() const
        {
            if (not opt_)
                return "";

            return opt_->c_str();
        }

        bool is_opt() const
        {
            return ((*p_)[0] == '-');
        }

        bool is_short_opt() const
        {
            return (((*p_)[0] == '-') && ((*p_)[1] != '-'));
        }

        bool is_long_opt() const
        {
            return (((*p_)[0] == '-') && ((*p_)[1] == '-'));
        }
    };

    class iterator: public std::iterator<std::forward_iterator_tag,
        parameter, parameter, const parameter*, parameter>
    {
    private:
        std::string opt_;
        char**      p_;

        void parse_option()
        {
            if (not *p_)
                return;

            char *p = *p_;

            if (*p == '-') {
                ++p;
                if (*p == '-')
                    ++p;
                opt_ = p;
            }
        }

    public:
        explicit iterator(char** p): opt_(), p_{p} {}

        iterator& operator++() { ++p_; parse_option(); return *this; }
        bool operator==(iterator other) const { return p_ == other.p_; }
        bool operator!=(iterator other) const { return !(*this == other); }
        reference operator*() const { return parameter(p_, &opt_); }
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
        std::string prefix = "arg : ";
        if (arg.is_short_opt())
            prefix = "opt : ";

        if (arg.is_long_opt())
            prefix = "opt : ";

        std::cout << prefix << arg.content() << " " << arg.option() << "\n"; 
    }

    return EXIT_SUCCESS;
}

#if defined(DEPRECATED)
class cstring
{
private:
    char* c_str_;

public:
    class iterator: public std::iterator<std::output_iterator_tag,
        char*, char*, const char **, char*>
    {
    private:
        char* p_;

    public:
        explicit iterator(char** p): p_{p} {}

        iterator& operator++() { ++p_; return *this; }
        bool operator==(iterator other) const { return p_ == other.p_; }
        bool operator!=(iterator other) const { return !(*this == other); }
        reference operator*() const { return p_; }
    };

    explicit cstring(char* c_str):
        c_str_{c_str}
    {}

    iterator begin() { return iterator(c_str_); }
    iterator end() { return iterator(c_str_ + strlen(c_str_)); }
};

#endif