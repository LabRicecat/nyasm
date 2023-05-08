#ifndef NYASM_HPP
#define NYASM_HPP

#include "catpkgs/kittenlexer/kittenlexer.hpp"
#include "catpkgs/stack-nyachine/stack-nyachine.hpp"

#include <functional>

using return_t = std::vector<stack_nyachine::chuwunk>;
using position_t = stack_nyachine::StackNyachine::size_tywp;
using msize_t = position_t;
inline std::map<std::string, position_t> labels;
inline std::map<std::string,std::vector<position_t>> unresolved_labels;

namespace reserved {
    inline const constexpr static position_t null = 0; 
    inline const constexpr static position_t jmp_buf1 = 1; 
    inline const constexpr static position_t jmp_buf2 = 2; 
    inline const constexpr static position_t jmp_buf3 = 3; 
    inline const constexpr static position_t ret_buf1 = 4;
    inline const constexpr static position_t ret_buf2 = 5;
    inline const constexpr static position_t ret_buf3 = 6;
    inline const constexpr static position_t argtmp_buf1 = 7;
    inline const constexpr static position_t argtmp_buf2 = 8;
    inline const constexpr static position_t argtmp_buf3 = 9;
    inline const constexpr static position_t argtmp_buf4 = 10;
    inline const constexpr static position_t argtmp_buf5 = 11;
    inline const constexpr static position_t argtmp_buf6 = 12;
    inline const constexpr static position_t argtmp_buf7 = 13;
    inline const constexpr static position_t argtmp_buf8 = 14;
    inline const constexpr static position_t argtmp_buf9 = 15;
    inline const constexpr static position_t argtmp_buf10 = 16;
    inline const constexpr static position_t argtmp_buf11 = 17;
    inline const constexpr static position_t argtmp_buf12 = 18;
    inline const constexpr static position_t argtmp_buf13 = 19;
    inline const constexpr static position_t argtmp_buf14 = 20;
    inline const constexpr static position_t ssz = 21;
    inline const constexpr static position_t svl = 22;
    inline const constexpr static position_t spt = 23;
    inline const constexpr static position_t math_buf1 = 24;
    inline const constexpr static position_t math_buf2 = 25;
    inline const constexpr static position_t math_buf3 = 26;
    inline const constexpr static position_t reserved_max = 27;
};

struct SyntaxException {
    std::string message;
    SyntaxException(std::string s) { message = s; }
};

struct ArgumentException {
    std::string instruction;
    int required = 0;
    int got = 0;
};

struct FailedExecutionException {
    std::vector<SyntaxException> syntax;
    std::vector<ArgumentException> argument;
};

long long num(std::string s) {
    try {
        return std::stoll(s);
    }
    catch(...) {
        throw SyntaxException("Unresolvable argument: \"" + s + "\"");
    }
    return 0;
}
void argcheck(std::string name, int fixed, int size) {
    if(size != fixed) throw ArgumentException{name,fixed,size};
}

inline static constexpr const position_t data_begin() {
    return reserved::reserved_max;
}

inline static position_t data_pointer = data_begin();

inline static position_t new_data() {
    return data_pointer++;
}

inline static std::map<std::string,position_t> data_field;

namespace gen {
    #define _gen_N(...) v.push_back(__VA_ARGS__)

    msize_t push_size() { return 3; }
    msize_t push_data() { return 1; }
    msize_t push_pos() { return 2; }
    void push(stack_nyachine::chuwunk data, position_t pos, return_t& v) {
        _gen_N(stack_nyachine::OPT_PUwUSH);
        _gen_N(data);
        _gen_N(pos);
    }

    msize_t spush_size() { return 2; }
    void spush(position_t pos, return_t& v) {
        using namespace stack_nyachine;
        _gen_N(stack_nyachine::OPT_PUwUSHS);
        _gen_N(pos);
    }

    msize_t stop_size() { return 2; }
    void stop(position_t pos, return_t& v) {
        _gen_N(stack_nyachine::OPT_TOwOPS);
        _gen_N(pos);
    }

    msize_t deref_size() { return 3; }
    void deref(position_t s, position_t d, return_t& v) {
        _gen_N(stack_nyachine::OPT_DEREF);
        _gen_N(s);
        _gen_N(d);
    }

    msize_t move_src() { return 1; }
    msize_t move_des() { return 2; }
    msize_t move_size() { return 3; }
    void move(position_t src, position_t des, return_t& v) {
        _gen_N(stack_nyachine::OPT_MOwOV);
        _gen_N(src);
        _gen_N(des);
    }
    
    msize_t add_size() { return 4; }
    void add(position_t lhs, position_t rhs, position_t destination, return_t& v) {
        _gen_N(stack_nyachine::OPT_ADD);
        _gen_N(lhs);
        _gen_N(rhs);
        _gen_N(destination);
    }

    msize_t sub_size() { return 4; }
    void sub(position_t lhs, position_t rhs, position_t destination, return_t& v) {
        _gen_N(stack_nyachine::OPT_SUwUB);
        _gen_N(lhs);
        _gen_N(rhs);
        _gen_N(destination);
    }

    msize_t mul_size() { return 4; }
    void mul(position_t lhs, position_t rhs, position_t destination, return_t& v) {
        _gen_N(stack_nyachine::OPT_MUwUL);
        _gen_N(lhs);
        _gen_N(rhs);
        _gen_N(destination);
    }

    msize_t div_size() { return 5; }
    void div(position_t lhs, position_t rhs, position_t destination, position_t prec, return_t& v) {
        _gen_N(stack_nyachine::OPT_DIWV);
        _gen_N(lhs);
        _gen_N(rhs);
        _gen_N(destination);
        _gen_N(prec);
    }
    
    msize_t jump_size() { return 4; }
    msize_t jump_pos() { return 3; }
    void jump(position_t pos, position_t lhs, position_t rhs, return_t& v) {
        _gen_N(stack_nyachine::OPT_JUwUMP);
        _gen_N(lhs);
        _gen_N(rhs);
        _gen_N(pos);
    }

    msize_t nop_size() { return 1; }
    void nop(return_t& v) {
        _gen_N(stack_nyachine::OPT_NOwOP);
    }
    msize_t spop_size() { return 1; }
    void spop(return_t& v) {
        _gen_N(stack_nyachine::OPT_POwOPS);
    }

    #undef _gen_N
}

inline std::map<std::string, std::string> macros = {
    {"rv1",std::to_string(reserved::ret_buf1)},
    {"rv2",std::to_string(reserved::ret_buf2)},
    {"rv3",std::to_string(reserved::ret_buf3)},
    {"null",std::to_string(reserved::null)}
};

inline std::map<std::string,std::function<return_t(std::vector<std::string>, position_t)>> instructions = {
    {"void",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("void",0,args.size());
        using namespace stack_nyachine;
        return_t s;
        s.push_back(OPT_NOwOP);
        return s;
    }},
    {"jump",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("jump",1,args.size());
        using namespace stack_nyachine;
        return_t s;

        try {
            gen::deref(std::stoll(args[0]),reserved::jmp_buf1,s);
            gen::jump(reserved::jmp_buf1,reserved::null,reserved::null,s);
            return s;
        }
        catch(...) {
            if(labels.count(args[0]) == 0) {
                gen::push(0,reserved::jmp_buf1,s);
                unresolved_labels[args[0]].push_back(p + s.size() - gen::push_size() + gen::push_data());
                gen::jump(reserved::jmp_buf1,reserved::null,reserved::null,s);
                return s;
            }
            gen::push(labels[args[0]],reserved::jmp_buf1,s);
            gen::jump(reserved::jmp_buf1,reserved::null,reserved::null,s);
        }

        return s;
    }},
    {"jumpif",[](std::vector<std::string> args, position_t p)->return_t {
        using namespace stack_nyachine;
        return_t s;
        argcheck("jumpif",3,args.size());
        auto lhs = num(args[0]);
        auto rhs = num(args[1]);
        try {
            gen::deref(std::stoll(args[2]),reserved::jmp_buf1,s);
            gen::jump(reserved::jmp_buf1,lhs,rhs,s);
            return s;
        }
        catch(...) {
            if(labels.count(args[2]) == 0) {
                gen::push(0,reserved::jmp_buf1,s);
                unresolved_labels[args[2]].push_back(p + s.size() - gen::push_size() + gen::push_data());
                gen::jump(reserved::jmp_buf1,lhs,rhs,s);
                return s;
            }
            gen::push(labels[args[2]],reserved::jmp_buf1,s);
            gen::jump(reserved::jmp_buf1,lhs,rhs,s);
        }

        return s;
    }},
    {"move",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("move",2,args.size());
        using namespace stack_nyachine;
        return_t s;
        gen::move(num(args[1]),num(args[0]),s);
        return s;
    }},
    {"call",[](std::vector<std::string> args, position_t p)->return_t {
        return_t s;
        argcheck("call",1,args.size());
        auto after_jmp = p + gen::push_size() + gen::spush_size() + gen::jump_size()
            + gen::push_size();
        gen::push(after_jmp,reserved::jmp_buf1,s);
        gen::spush(reserved::jmp_buf1,s);
        if(labels.count(args[0]) != 0) {
            gen::push(labels[args[0]],reserved::jmp_buf1,s);
        }
        else {
            gen::push(0,reserved::jmp_buf1,s);
            unresolved_labels[args[0]].push_back(p + s.size() - gen::push_size() + gen::push_data());
        }
        gen::jump(reserved::jmp_buf1,reserved::null,reserved::null,s);
        gen::nop(s);
        return s;
    }},
    {"return",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("return",0,args.size());
        return_t s;
        gen::stop(reserved::jmp_buf1,s);
        gen::spop(s);

        gen::jump(reserved::jmp_buf1,reserved::null,reserved::null,s);
        return s;
    }},
    {"data",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("data",1,args.size());
        using namespace stack_nyachine;
        if(data_field.count(args[0]) != 0) return {};
        data_field[args[0]] = new_data();
        return {};
    }},
    {"add",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("add",3,args.size());
        using namespace stack_nyachine;
        position_t lhs = num(args[0]);
        position_t rhs = num(args[1]);
        position_t des = num(args[2]);
        return_t s;
        gen::add(lhs,rhs,des,s);
        return s;
    }},
    {"sub",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("sub",3,args.size());
        using namespace stack_nyachine;
        position_t lhs = num(args[0]);
        position_t rhs = num(args[1]);
        position_t des = num(args[2]);
        return_t s;
        gen::sub(lhs,rhs,des,s);
        return s;
    }},
    {"mul",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("mul",3,args.size());
        using namespace stack_nyachine;
        position_t lhs = num(args[0]);
        position_t rhs = num(args[1]);
        position_t des = num(args[2]);
        return_t s;
        gen::mul(lhs,rhs,des,s);
        return s;
    }},
    {"div",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("div",3,args.size());
        using namespace stack_nyachine;
        return_t s;
        auto a = num(args[0]);
        auto b = num(args[1]);
        auto dest = num(args[2]);
        gen::div(a,b,dest,reserved::math_buf1,s);
        return s;
    }},
    {"deref",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("deref",2,args.size());
        using namespace stack_nyachine;
        return_t s;
        gen::deref(num(args[0]),num(args[1]),s);
        return s;
    }},
    {"push",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("push",1,args.size());
        using namespace stack_nyachine;
        return_t s;
        s.push_back(OPT_PUwUSHS);
        s.push_back(num(args[0]));
        return s;
    }},
    {"pop",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("pop",0,args.size());
        using namespace stack_nyachine;
        return_t s;
        s.push_back(OPT_POwOPS);
        return s;
    }},
    {"halt",[](std::vector<std::string> args, position_t p)->return_t {
        argcheck("halt",0,args.size());
        using namespace stack_nyachine;
        return_t s;
        s.push_back(OPT_AAH_STOPP);
        return s;
    }},

    {"@mac",[](std::vector<std::string> args, position_t p)->return_t {
        using namespace stack_nyachine;
        macros[args[0]] = args[1];
        return return_t();
    }},
};

void replace_macros(lexed_kittens& line) {
    for(auto& j : line)
        if(macros.count(j.src) != 0) j.src = macros[j.src];
}

std::string check_builtin(std::string current, return_t& v, position_t pos) {
    if(current == "this") return std::to_string(pos);
    if(current == "ssz") { 
        v.push_back(stack_nyachine::OPT_SSIZE);
        v.push_back(reserved::ssz);
        return std::to_string(reserved::ssz);
    }
    if(current == "svl") {
        v.push_back(stack_nyachine::OPT_TOwOPS);
        v.push_back(reserved::svl);
        return std::to_string(reserved::svl);
    }
    if(current == "spt") {
        v.push_back(stack_nyachine::OPT_SPTR);
        v.push_back(reserved::spt);
        return std::to_string(reserved::spt);
    }
    if(macros.count(current) != 0) return macros[current];
    if(data_field.count(current) != 0) return std::to_string(data_field[current]);
    return current;
}

void pargs_literal_check(std::string& src, position_t pos, return_t& v, position_t into) {
    src = check_builtin(src,v,pos);
    try {
        gen::push(std::stoll(src),into,v);
        src = std::to_string(into);
    }
    catch(...) {
        if(labels.count(src) != 0) {
            gen::push(labels[src],into,v);
            src = std::to_string(into);
        }
        else {
            gen::push(0,into,v);
            unresolved_labels[src].push_back(pos + v.size() - gen::push_size() + gen::push_data());
            src = std::to_string(into);
        }
    }
}

std::string eval_argument(std::string source, return_t& v, position_t pos, stack_nyachine::chuwunk& counter) {
    source = check_builtin(source,v,pos);
    if(source.front() == '[' || source.front() == '{') {
        bool df = source.front() == '[';
        source.erase(source.begin());
        source.pop_back();

        std::string rhs, lhs, op;
        std::stack<char> brc;
        for(auto i : source) {
            if(i == '[' || i == '{') brc.push(i);
            if(i == ']' || i == '}') brc.pop(); // todo: errorchecks
            if((i == '-' || i == '+' ) && brc.empty()) op = i;
            else if(isspace(i)) continue;
            else if(op == "") lhs += i;
            else rhs += i;
        }
        
        lhs = eval_argument(lhs,v,pos,++counter);
        if(rhs != "") {
            rhs = eval_argument(rhs,v,pos,counter);
            gen::push(counter,++counter,v);
            if(op == "+")
                gen::add(std::stoll(lhs),std::stoll(rhs),counter,v);
            else if(op == "-")
                gen::sub(std::stoll(lhs),std::stoll(rhs),counter,v);
            if(df) {
                ++counter; 
                gen::push(counter,counter,v);
                gen::deref(counter-1,counter,v);
            }
        }
        else {
            if(df) {
                gen::push(counter,counter,v);
                gen::deref(std::stoll(lhs),counter,v);
            }
        }

        return std::to_string(counter);
    }
    else if(source[0] == '%') {
        source.erase(source.begin());
        pargs_literal_check(source,pos,v,++counter);
        return std::to_string(counter);
    }
    else return source;
}

stack_nyachine::StackNyachine compile(std::string source, bool shrink = false, std::string* debuginfo = nullptr, 
    stack_nyachine::StackNyachine::size_tywp memory_size = 100000, 
    stack_nyachine::StackNyachine::size_tywp heap_size = 100000) {
    KittenLexer lexer = KittenLexer()
        .add_capsule('(',')')
        .add_capsule('[',']')
        .add_capsule('{','}')
        .add_ignore(' ')
        .add_ignore('\t')
        .add_linebreak('\n')
        .add_lineskip(';')
        .add_extract(':')
        .erase_empty()
        .ignore_backslash_opts()
        ;
    auto lexed = lexer.lex(source);
    std::vector<lexed_kittens> lines;
    int l = 0;
    for(auto i : lexed) {
        if(l != i.line) {
            l = i.line;
            lines.push_back({});
        }
        lines.back().push_back(i);
    }
    stack_nyachine::StackNyachine m(memory_size,heap_size);
    long long lp = -1;
    FailedExecutionException fail;
    for(auto i : lines) {
        if(debuginfo != nullptr) *debuginfo += std::to_string(i.front().line) + ": " + std::to_string(lp+1) + "-";
        replace_macros(i);
        std::string inst = i.front().src;
        ++lp;
        if(i.size() == 2 && i[1].src == ":") {
            m.memowory[lp] = stack_nyachine::OPT_NOwOP;
            labels[i.front().src] = lp;
            if(debuginfo != nullptr) *debuginfo += std::to_string(lp) + "\n";    
            continue;
        }
        if(instructions.count(inst) == 0) {
            SyntaxException err("Unknown instruction: " + inst);
            std::cout << "SyntaxException in line " << i.front().line << "\n"
            << " | ";
            for(auto k : i) std::cout << k.src << " ";
            std::cout << "\n:: " << err.message << "\n";
            fail.syntax.push_back(err);
            continue;
        }
        std::vector<std::string> args;
        stack_nyachine::chuwunk counter = reserved::argtmp_buf1;
        for(size_t j = 1; j < i.size(); ++j) {
            return_t v;
            args.push_back(eval_argument(i[j].src,v,lp,counter));
            if(!v.empty())
                for(auto j : v)
                    m.memowory[lp++] = j;
        }
        return_t res;
        try {
            res = instructions[inst](args,lp);
        }
        catch(SyntaxException& err) {
            std::cout << "SyntaxException in line " << i.front().line << "\n"
            << " | ";
            for(auto k : i) std::cout << k.src << " ";
            std::cout << "\n:: " << err.message << "\n";
            fail.syntax.push_back(err);
            continue;
        }
        catch(ArgumentException& err) {
            std::cout << "ArgumentException in line " << i.front().line << "\n"
            << " | ";
            for(auto k : i) std::cout << k.src << " ";
            std::cout << "\n:: " << "Required: " << err.required << " | Got: " << err.got << "\n";
            fail.argument.push_back(err);
            continue;
        }

        for(size_t j = 0; j < res.size(); ++j, ++lp)
            m.memowory[lp] = res[j];
        --lp;
        if(debuginfo != nullptr) *debuginfo += std::to_string(lp) + "\n";        
    }
    for(auto i : unresolved_labels) {
        if(labels.count(i.first) == 0) { std::cout << "undefined label: \"" + i.first + "\"\n"; break; }
        for(auto j : i.second) {
            m.memowory[j] = labels[i.first];
        }
    }
    m.memowory[++lp] = stack_nyachine::OPT_AAH_STOPP;
    if(shrink)
        m.memowory_size = lp;
    if(fail.syntax.size() == 0 && fail.argument.size() == 0)
        return m;
    throw fail;
}

#endif