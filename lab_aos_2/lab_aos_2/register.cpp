#include <iostream>
#include "register.h"
#include "lexer.h"

using namespace std;

void processor::left(int i, int n) {
    st.set_cmd_text(cur_cmd);
    st.left(i,n);
}
void processor::right(int i, int n) {
    st.set_cmd_text(cur_cmd);
    st.right(i,n);
}
void processor::mov(int i, int n) {
    st.set_cmd_text(cur_cmd);
    st.mov(i,n);
}
void Stack::left(int ind, int n){
    cmd_ctr++;
    tick_ctr = 1;
    R(0) = R(ind - 1);
    do_next_tick();

    for(int i = 0; i < n; i++){
            R(0).left();
        status = R(1).last();
    }
    do_next_tick();
}

void Stack::right(int ind, int n){
    cmd_ctr++;
    tick_ctr = 1;
    R(0) = R(ind - 1);
    do_next_tick();

    for(int i = 0; i < n; i++){
            R(0).right();
        status = R(0).last();
    }
    do_next_tick();
}

void Stack::mov(int i, int n){
    cmd_ctr++;
    tick_ctr = 1;
    status = R(i - 1).last();
    do_next_tick();

    R(i - 1) = n;
    status = R(i - 1).last();
    do_next_tick();
}

inline void Stack::set_cmd_text(const std::string &src){
    cur_cmd = src;
}
bool Stack::do_next_tick(){
    print();
    std::string tmp;
    getline(std::cin, tmp);
    ++tick_ctr;
    return true;
}

reg& Stack::R(int n){
    return regist[n];
}
void Stack::print(std::ostream &out){
    for(int i = 0; i < regs; i++){
        cout << "R" << i +1 << " ";
        R(i).print();
        cout << endl;
    }
    cout << endl;

    cout << "Command input  " << cur_cmd << endl;
    cout << "Status         ";
    if(status == 0)
        cout << "+";
    else cout << "-";
    cout << endl;

    cout << "Tick           " << tick_ctr << endl;
    cout << "Command        " << cmd_ctr << endl;

    cout << "==============================|" << endl;
}

const size_t reg::bits = 12;
reg::reg(){
    for(int i = 0; i < bits; i++)
        regist[i] = 0;
}
reg::reg(int n) : reg() {
    bool n_positive = true;
    if(n < 0){
        n = -n;
        n_positive = false;
    }

    for(int i = 0; i < bits - 1; i++)
    {
        regist[i] = n % 2;
        n /= 2;
    }

    if(!n_positive){
        for(int i = 0; i < bits - 1; i++)
            regist[i] = (regist[i] + 1) % 2;

        int mov_num = 1;
        for(int i = 0; i < bits - 1; i++){
            int tmp = regist[i] + mov_num;
            regist[i] = tmp % 2;
            mov_num = tmp / 2;
        }
        last() = 1;
    }
    else
        last() = 0;
}
void reg::operator=(reg &other){
    for(int i = 0; i < bits; i++)
        regist[i] = other.regist[i];
}

void reg::operator=(int n){
    bool n_positive = true;
    if(n < 0){
        n = -n;
        n_positive = false;
    }

    for(int i = 0; i < bits - 1; i++)
    {
        regist[i] = n % 2;
        n /= 2;
    }

    if(!n_positive){
        for(int i = 0; i < bits - 1; i++)
            regist[i] = (regist[i] + 1) % 2;

        int mov_num = 1;
        for(int i = 0; i < bits- 1; i++){
            int tmp = regist[i] + mov_num;
            regist[i] = tmp % 2;
            mov_num = tmp / 2;
        }
        last() = 1;
    }
    else
        last() = 0;
}

short& reg::at(int pos_num){
    if(pos_num >= 0 && pos_num < bits)
        return regist[pos_num];
    else
        throw invalid_argument("short& reg::at(size_t pos_num) got invalid argument.");
}
short& reg::first(){
    return at(0);
}
short& reg::last(){
    return at(bits - 1);
}

void reg::print(){
    for(int i = bits - 1; i >= 0; i--)
        cout << regist[i];
}

short reg::left(short mov_n){
    short tmp = last();
    for(int i = bits - 1; i > 0; i--)
        regist[i] = regist[i - 1];
    first() = mov_n;
    return tmp;
}

short reg::right(short mov_n){
    short tmp = first();
    for(int i = 0; i < bits - 1; i++)
        regist[i] = regist[i + 1];
    last() = mov_n;
    return tmp;
}

void processor::do_left_right(int &cur, int argc, std::vector<std::string> &argv, int left_or_right){
    cur++;
    cur_cmd = "";
    if(!(left_or_right != 0 || left_or_right != 1))
         throw invalid_argument("in processor::do_left_right arguments left_or_right have wrong value\n");
    if(cur >= argc)
    {
        if(left_or_right == 0)
            cout << "left";
        else if(left_or_right == 1)
                cout << "right";
        cout << " : no arguments" << endl;
    }
    else
    {
        if(left_or_right == 0)
            cur_cmd += "left";
        else cur_cmd += "right";

        string arg_1 = argv[cur];
        cur++;
        string arg_2 = argv[cur];
        cur_cmd += " " + arg_1 + " " + arg_2;
        cur++;

        int arg1 = 0, arg2 = 0;
        try{
            arg1 = stoi(arg_1);
            arg2 = stoi(arg_2);
        }
        catch(invalid_argument &ex)
        {
            cout << cur_cmd << " : wrong argument(not a number)." << endl;
            return;
        }
        if(left_or_right == 0)
            left(arg1, arg2);
        else
            right(arg1, arg2);
    }
}

void processor::mov(int &cur, int argc, std::vector<std::string> &argv){
    cur++;
    cur_cmd = "";
    if(cur >= argc)
        cout << "mov : no arguments" << endl;
    else {
        if(cur + 1 >= argc)
            cout << "mov : no much arguments" << endl;
        else{
            cur_cmd += "mov";
            string arg_1 = argv[cur];
            cur++;
            string arg_2 = argv[cur];
            cur_cmd += " " + arg_1 + " " + arg_2;
            cur++;

            int arg1 = 0, arg2 = 0;
            try{
                arg1 = stoi(arg_1);
                arg2 = stoi(arg_2);
            }
            catch(invalid_argument &ex)
            {
                cout << cur_cmd << " : wrong argument." << endl;
                return;
            }
            mov(arg1, arg2);
        }
    }
}

void processor::do_command(int &cur, int argc, std::vector<std::string> &argv){
    if(argv[cur] == "left")
        do_left_right(cur, argc, argv, 0);
    else if(argv[cur] == "right")
        do_left_right(cur, argc, argv, 1);
    else if(argv[cur] == "mov")
        mov(cur, argc, argv);
    else
    {
        cout << argv[cur] << " : ignored." << endl;
        cur++;
    }
}

void processor::execute(const std::string &str)
{
    vector<string> argv;
    string argument = "";
    int argc = 0;
    Lexer lex;
    lex.load(str);
    while(lex.next(argument))
    {
        argv.push_back(argument);
        ++argc;
    }

    if(argc == 0)
        cout << str << " : nothing to do." << endl;
    int cur = 0;
    while(cur < argc)
        do_command(cur, argc, argv);
}
