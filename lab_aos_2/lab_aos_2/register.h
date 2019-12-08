#pragma once
#include <vector>

class reg {
public:
    reg();
    reg(int n);
    void operator=(reg &other);
    void operator=(int );

    short& at(int pos_num);
    short& first();
    short& last();
    void print();
    short left(short mov_n = 0);
    short right(short mov_n = 0);

    static const size_t bits;
private:
    short regist[12] = {};
};

class Stack {
public:
    Stack() = default;

    void left(int i, int n);
    void right(int i, int n);
    void mov(int i, int n);
    void print(std::ostream &out = std::cout);
    inline void set_cmd_text(const std::string &);
    bool do_next_tick();
private:
    reg& R(int n);

    static const int regs = 2;
    reg* regist = new reg[regs];

    int cmd_ctr = 0;
    int tick_ctr = 0;
    int status = 0;
    std::string cur_cmd = "";
};

class processor {
public:
    void execute(const std::string &str);
private:
    void left(int i, int n);
    void right(int i, int n);
    void mov(int i, int n);

    void do_left_right(int &cur, int argc, std::vector<std::string> &argv, int left_or_right);
    void do_command(int &cur, int argc, std::vector<std::string> &argv);
    void mov(int &cur, int argc, std::vector<std::string> &argv);

    Stack st;
    std::string cur_cmd = "";
};
