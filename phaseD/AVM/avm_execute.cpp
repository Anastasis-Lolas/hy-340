#include "avm_execute.h"

#include <iomanip>

#define AVM_ENDING_PC codeSize

std::vector<std::string> string_consts;
extern std::vector<double> nums_consts;
std::vector<std::string> libfuncs;
std::vector<userfunc> userfuncs;
extern avm_memcell stack[AVM_STACKSIZE];



execute_func_t executeFuncs[] = {
    execute_assign,       execute_add,          execute_sub,
    execute_mul,          execute_div,          execute_mod,
    execute_uminus,       execute_and,          execute_or,
    execute_not,          execute_jeq,          execute_jne,
    execute_jle,          execute_jge,          execute_jlt,
    execute_jgt,          execute_call,         execute_pusharg,
    execute_funcenter,    execute_funcexit,     execute_newtable,
    execute_tablegetelem, execute_tablesetelem, execute_jump,
    execute_nop};


void execute_cycle(void) {
    if (executionFinished)
        return;
    else if (pc == AVM_ENDING_PC) {
        executionFinished = 1;
        return;
    } else {
        assert(pc < AVM_ENDING_PC);
        instruction *instr = code + pc;
        assert(instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);

        if (instr->srcLine) currLine = instr->srcLine;
        unsigned oldPC = pc;
        executeFuncs[instr->opcode](instr);
        if (pc == oldPC) ++pc;
    }
}

void read_and_print_avm_binary(const std::string &filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile.is_open()) {
        return;
    }

    unsigned int magic_number_read;
    infile.read(reinterpret_cast<char *>(&magic_number_read),
                sizeof(unsigned int));
    if (!infile || magic_number_read != 340200501) {
        return;
    }
    std::cout << "Magic Number: " << magic_number_read << " (OK)\n";

    unsigned int total_strings;
    infile.read(reinterpret_cast<char *>(&total_strings), sizeof(unsigned int));
    std::cout << "\n--- String Constants (Read from file: " << total_strings
              << ") ---" << std::endl;
    for (unsigned int i = 0; i < total_strings; ++i) {
        unsigned int str_len;
        infile.read(reinterpret_cast<char *>(&str_len), sizeof(unsigned int));
        std::string s(str_len, '\0');
        infile.read(&s[0], str_len);
        char null_terminator;
        infile.read(&null_terminator, sizeof(char));
        string_consts.push_back(s);
        std::cout << i << ": \"" << s << "\"\n";
    }

    unsigned int total_numbers;
    infile.read(reinterpret_cast<char *>(&total_numbers), sizeof(unsigned int));
    std::cout << "\n--- Double Constants (Read from file: " << total_numbers
              << ") ---" << std::endl;
    for (unsigned int i = 0; i < total_numbers; ++i) {
        double d;
        infile.read(reinterpret_cast<char *>(&d), sizeof(double));
        nums_consts.push_back(d);
        std::cout << i << ": " << d << "\n";
    }

    unsigned int total_user_funcs;
    infile.read(reinterpret_cast<char *>(&total_user_funcs),
                sizeof(unsigned int));
    std::cout << "\n--- User Functions (Read from file: " << total_user_funcs
              << ") ---" << std::endl;
    for (unsigned int i = 0; i < total_user_funcs; ++i) {
        userfunc uf;
        infile.read(reinterpret_cast<char *>(&uf.address),
                    sizeof(unsigned int));
        infile.read(reinterpret_cast<char *>(&uf.localSize),
                    sizeof(unsigned int));
        unsigned int id_len;
        infile.read(reinterpret_cast<char *>(&id_len), sizeof(unsigned int));
        uf.id.resize(id_len);
        infile.read(&uf.id[0], id_len);
        char null_terminator_id;
        infile.read(&null_terminator_id, sizeof(char));
        userfuncs.push_back(uf);
        std::cout << i << ": ID=" << uf.id << ", Address=" << uf.address
                  << ", Locals=" << uf.localSize << "\n";
    }

    unsigned int total_lib_funcs;
    infile.read(reinterpret_cast<char *>(&total_lib_funcs),
                sizeof(unsigned int));
    std::cout << "\n--- Library Functions (Read from file: " << total_lib_funcs
              << ") ---" << std::endl;
    for (unsigned int i = 0; i < total_lib_funcs; ++i) {
        unsigned int lib_str_len;
        infile.read(reinterpret_cast<char *>(&lib_str_len),
                    sizeof(unsigned int));
        std::string s_lib(lib_str_len, '\0');
        infile.read(&s_lib[0], lib_str_len);
        char null_terminator_lib;
        infile.read(&null_terminator_lib, sizeof(char));
        libfuncs.push_back(s_lib);
        std::cout << i << ": " << s_lib << "\n";
    }

    std::cout << "\n--- Target Code (AVM Instructions Read from file) ---"
              << std::endl;
    unsigned int total_instructions;
    infile.read(reinterpret_cast<char *>(&total_instructions),
                sizeof(unsigned int));

    const int col_num_width_r = 5;
    const int col_opcode_width_r = 15;
    const int col_arg_width_r = 35;

    std::cout << std::left << std::setw(col_num_width_r) << "No." << std::left
              << std::setw(col_opcode_width_r) << "Opcode" << std::left
              << std::setw(col_arg_width_r) << "Result" << std::left
              << std::setw(col_arg_width_r) << "Arg1" << std::left
              << std::setw(col_arg_width_r) << "Arg2" << std::endl;
    int total_width_for_separator_r =
        col_num_width_r + col_opcode_width_r + (col_arg_width_r * 3);
    std::cout << std::string(total_width_for_separator_r, '-') << std::endl;

    for (unsigned int i = 0; i < total_instructions; ++i) {
        if (!infile.good()) break;
        uint8_t opcode_byte_read;
        infile.read(reinterpret_cast<char *>(&opcode_byte_read),
                    sizeof(uint8_t));
        vmopcode current_opcode = static_cast<vmopcode>(opcode_byte_read);

        vmarg result_read, arg1_read, arg2_read;
        vmarg *vmarg_ptrs_read[3] = {&result_read, &arg1_read, &arg2_read};

        for (int k = 0; k < 3; ++k) {
            uint8_t type_byte_read;
            infile.read(reinterpret_cast<char *>(&type_byte_read),
                        sizeof(uint8_t));
            vmarg_ptrs_read[k]->type = static_cast<vmarg_t>(type_byte_read);
            infile.read(reinterpret_cast<char *>(&vmarg_ptrs_read[k]->val),
                        sizeof(unsigned int));
        }

        std::cout << std::left << std::setw(col_num_width_r) << i;
        std::cout << std::left << std::setw(col_opcode_width_r)
                  << vmopcode_to_string(current_opcode);

        auto vmarg_to_display_string =
            [&](const vmarg *arg_to_print) -> std::string {
            if (!arg_to_print || arg_to_print->type == undef_a) return " ";

            std::string argTypeNames[] = {"label_a",   "global_a", "formal_a",
                                          "local_a",   "number_a", "string_a",
                                          "bool_a",    "nil_a",    "userfunc_a",
                                          "libfunc_a", "retval_a", "undef_a"};

            std::string type_str = "??(unknown)";
            if (arg_to_print->type >= label_a &&
                arg_to_print->type <= undef_a) {
                type_str = argTypeNames[arg_to_print->type];
            }

            std::string val_str = std::to_string(arg_to_print->val);
            std::string extra_info = "";

            switch (arg_to_print->type) {
                case number_a:
                    if (arg_to_print->val < nums_consts.size()) {
                        extra_info =
                            " (" +
                            std::to_string(nums_consts[arg_to_print->val]) +
                            ")";
                    }
                    break;
                case string_a:
                    if (arg_to_print->val < string_consts.size())
                        extra_info =
                            " (\"" + string_consts[arg_to_print->val] + "\")";
                    break;
                case userfunc_a:
                    if (arg_to_print->val < userfuncs.size())
                        extra_info =
                            " (" + userfuncs[arg_to_print->val].id + ")";
                    break;
                case libfunc_a:
                    if (arg_to_print->val < libfuncs.size())
                        extra_info = " (" + libfuncs[arg_to_print->val] + ")";
                    break;
                case bool_a:
                    extra_info =
                        (arg_to_print->val != 0 ? " (true)" : " (false)");
                    break;
                default:
                    break;
            }
            return type_str + ":" + val_str + extra_info;
        };

        std::cout << std::left << std::setw(col_arg_width_r)
                  << vmarg_to_display_string(&result_read);
        std::cout << std::left << std::setw(col_arg_width_r)
                  << vmarg_to_display_string(&arg1_read);
        std::cout << std::left << std::setw(col_arg_width_r)
                  << vmarg_to_display_string(&arg2_read);
        std::cout << "\n";
    }
    std::cout << std::string(total_width_for_separator_r, '-') << std::endl;
    std::cout
        << "===========================================================\n";

    infile.close();
}