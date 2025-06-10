#include "avm_execute.h"

#include <iomanip>

#include "../t-codeLib/t-code.h"

#define AVM_ENDING_PC codeSize

std::vector<instruction> exec_instructions;
std::vector<std::string> string_consts;
std::vector<double> nums_consts;
std::vector<std::string> libfuncs;
std::vector<userfunc> userfuncs;
extern avm_memcell stack[AVM_STACKSIZE];

std::string typeStrings[] = {"number",   "string",  "bool", "table",
                             "userfunc", "libfunc", "nil",  "undef"};

std::string vmopcode_to_string(vmopcode op) {
    switch (op) {
        case assign_v:
            return "assign";
        case add_v:
            return "add";
        case sub_v:
            return "sub";
        case mul_v:
            return "mul";
        case div_v:
            return "div";
        case mod_v:
            return "mod";
        case uminus_v:
            return "uminus";
        case and_v:
            return "and";
        case or_v:
            return "or";
        case not_v:
            return "not";
        case jeq_v:
            return "jeq";
        case jne_v:
            return "jne";
        case jle_v:
            return "jle";
        case jge_v:
            return "jge";
        case jlt_v:
            return "jlt";
        case jgt_v:
            return "jgt";
        case call_v:
            return "call";
        case pusharg_v:
            return "pusharg";
        case funcenter_v:
            return "funcenter";
        case funcexit_v:
            return "funcexit";
        case newtable_v:
            return "newtable";
        case tablegetelem_v:
            return "tablegetelem";
        case tablesetelem_v:
            return "tablesetelem";
        case jump_v:
            return "jump";
        case ret_v:
            return "ret";
        case nop_v:
            return "nop";
        default:
            return "unknown_op";
    }
}


execute_func_t executeFuncs[] = {
    execute_assign,       execute_add,          execute_sub,
    execute_mul,          execute_div,          execute_mod,
    execute_uminus,       execute_and,          execute_or,
    execute_not,          execute_jeq,          execute_jne,
    execute_jle,          execute_jge,          execute_jlt,
    execute_jgt,          execute_call,         execute_pusharg,
    execute_funcenter,    execute_funcexit,     execute_newtable,
    execute_tablegetelem, execute_tablesetelem, execute_nop,
    execute_jump,         };


void execute_and(instruction *) {}
void execute_or(instruction *) {}

void execute_uminus(instruction *) {}
void execute_not(instruction *) {}


void execute_cycle(void) {
    
    while(1){
        if (executionFinished)
            break;
        else if (pc == exec_instructions.size()) {
           
            executionFinished = 1;
            break;
        } else {
           
            assert(pc < exec_instructions.size());
            instruction *instr = &exec_instructions[pc];
            assert(instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);

            if (instr->srcLine) currLine = instr->srcLine;
            unsigned oldPC = pc;
            executeFuncs[instr->opcode](instr);
            if (pc == oldPC) ++pc;
        }
    }
}

void read_and_print_avm_binary(const std::string &filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile.is_open()) return;

    unsigned int magic_number_read;
    infile.read(reinterpret_cast<char *>(&magic_number_read), sizeof(unsigned int));
    if (!infile || magic_number_read != 340200501) return;

    unsigned int total_strings;
    infile.read(reinterpret_cast<char *>(&total_strings), sizeof(unsigned int));
    for (unsigned int i = 0; i < total_strings; ++i) {
        unsigned int str_len;
        infile.read(reinterpret_cast<char *>(&str_len), sizeof(unsigned int));
        std::string s(str_len, '\0');
        infile.read(&s[0], str_len);
        char null_terminator;
        infile.read(&null_terminator, sizeof(char));
        string_consts.push_back(s);
    }

    unsigned int total_numbers;
    infile.read(reinterpret_cast<char *>(&total_numbers), sizeof(unsigned int));
    for (unsigned int i = 0; i < total_numbers; ++i) {
        double d;
        infile.read(reinterpret_cast<char *>(&d), sizeof(double));
        nums_consts.push_back(d);
    }

    unsigned int total_user_funcs;
    infile.read(reinterpret_cast<char *>(&total_user_funcs), sizeof(unsigned int));
    for (unsigned int i = 0; i < total_user_funcs; ++i) {
        userfunc uf;
        infile.read(reinterpret_cast<char *>(&uf.address), sizeof(unsigned int));
        infile.read(reinterpret_cast<char *>(&uf.localSize), sizeof(unsigned int));
        unsigned int id_len;
        infile.read(reinterpret_cast<char *>(&id_len), sizeof(unsigned int));
        uf.id.resize(id_len);
        infile.read(&uf.id[0], id_len);
        char null_terminator_id;
        infile.read(&null_terminator_id, sizeof(char));
        userfuncs.push_back(uf);
    }

    unsigned int total_lib_funcs;
    infile.read(reinterpret_cast<char *>(&total_lib_funcs), sizeof(unsigned int));
    for (unsigned int i = 0; i < total_lib_funcs; ++i) {
        unsigned int lib_str_len;
        infile.read(reinterpret_cast<char *>(&lib_str_len), sizeof(unsigned int));
        std::string s_lib(lib_str_len, '\0');
        infile.read(&s_lib[0], lib_str_len);
        char null_terminator_lib;
        infile.read(&null_terminator_lib, sizeof(char));
        libfuncs.push_back(s_lib);
    }

    unsigned int total_instructions;
    infile.read(reinterpret_cast<char *>(&total_instructions), sizeof(unsigned int));
    for (unsigned int i = 0; i < total_instructions; ++i) {
        if (!infile.good()) break;
        uint8_t opcode_byte_read;
        infile.read(reinterpret_cast<char *>(&opcode_byte_read), sizeof(uint8_t));
        vmopcode current_opcode = static_cast<vmopcode>(opcode_byte_read);

        vmarg result_read, arg1_read, arg2_read;
        vmarg *vmarg_ptrs_read[3] = {&result_read, &arg1_read, &arg2_read};

        for (int k = 0; k < 3; ++k) {
            uint8_t type_byte_read;
            infile.read(reinterpret_cast<char *>(&type_byte_read), sizeof(uint8_t));
            vmarg_ptrs_read[k]->type = static_cast<vmarg_t>(type_byte_read);
            infile.read(reinterpret_cast<char *>(&vmarg_ptrs_read[k]->val), sizeof(unsigned int));
        }

          instruction inst = {
            current_opcode,
            result_read,
            arg1_read,
            arg2_read,
            0,
        };

        exec_instructions.push_back(inst);
    }

    infile.close();
}



