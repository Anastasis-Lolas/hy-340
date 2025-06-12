#include "t-code.h"

#include <iomanip>

extern unsigned int total_globals;
std::vector<incomplete_jump *> incjumps_vec;
std::vector<std::string> string_vec_consts;
std::vector<double> num_vec_consts;
std::vector<std::string> lig_strvec_consts;
std::vector<userfunc> funcstack;
std::vector<int> labstack;
extern std::vector<quad *> quad_table;
std::vector<unsigned> final_number_vector;

std::vector<instruction *> instruction_table;
unsigned int currInst = 0;

incomplete_jump *ij_head = (incomplete_jump *)0;
unsigned ij_total = 0;


generator_func_t generators[] = {
    generate_ADD,          generate_SUB,          generate_DIV,
    generate_MOD,          generate_ASSIGN,       generate_UMINUS,
    generate_MUL,          generate_AND,          generate_OR,
    generate_NOT,          generate_IF_EQ,        generate_IF_NOTEQ,
    generate_IF_LESSEQ,    generate_IF_GREATEREQ, generate_IF_LESS,
    generate_IF_GREATER,   generate_CALL,         generate_PARAM,
    generate_RETURN,       generate_GETRETVAL,    generate_FUNCSTART,
    generate_FUNCEND,      generate_NEWTABLE,     generate_JUMP,
    generate_TABLEGETELEM, generate_TABLESETELEM};

unsigned consts_newstring(std::string s) {
    for (unsigned int i = 0; i < string_vec_consts.size(); ++i) {
        if (string_vec_consts[i] == s) {
            return i;
        }
    }
    string_vec_consts.push_back(s);
    return string_vec_consts.size() - 1;
}

unsigned consts_newnumber(double n) {
    num_vec_consts.push_back(n);
    return num_vec_consts.size() - 1;
}

unsigned libfuncs_newused(std::string s) {
    for (unsigned int i = 0; i < lig_strvec_consts.size(); ++i) {
        if (lig_strvec_consts[i] == s) {
            return i;
        }
    }
    lig_strvec_consts.push_back(s);
    return lig_strvec_consts.size() - 1;
}

void make_operand(expr *e, vmarg *arg) {
    switch (e->type) {
        case var_e:
        case tableitem_e:
        case arithexpr_e:
        case boolexpr_e:
        case newtable_e: {
            arg->val = e->sym->value.varVal->offset;
            switch (e->sym->type) {
                case GLOBAL:

                    arg->type = global_a;
                    break;
                case LLOCAL:
                    arg->type = local_a;
                    break;
                case FORMAL:
                    arg->type = formal_a;
                    break;
                default:
                    printf("Error1: Invalid symbol type for operand\n");
            }
            break;
        }
        case constbool_e: {
            arg->val = e->boolConst;
            arg->type = bool_a;
            break;
        }
        case conststring_e: {
            arg->val = consts_newstring(e->strConst);
            arg->type = string_a;
            break;
        }
        case constdouble_e: {
            arg->val = consts_newnumber(e->numConst);
            arg->type = number_a;
            break;
        }
        case intnum_e: {
            arg->val = consts_newnumber(e->numInt);
            arg->type = number_a;
            break;
        }
        case nil_e:
            arg->type = nil_a;
            break;
        case programfunc_e: {
            arg->val = userfunc_newfunc(e->sym);
            arg->type = userfunc_a;
            break;
        }
        case libraryfunc_e: {
            arg->val = libfuncs_newused(e->sym->value.funcVal->name);
            arg->type = libfunc_a;
            break;
        }
        default:
            std::cerr << "Error: Invalid symbol type for operand\n";
            assert(0);
    }
}


void vm_emit(instruction *t) {
    instruction *new_inst = new instruction();


    if (!new_inst) {
        std::cerr << "Failed !" << std::endl;
        return;
    }

    new_inst->opcode = t->opcode;
    new_inst->arg1 = t->arg1;
    new_inst->arg2 = t->arg2;
    new_inst->result = t->result;
    new_inst->srcLine = t->srcLine;
    instruction_table.push_back(new_inst);

    currInst++;
}


void add_incomplete_jump(unsigned instrNo, unsigned iaddress) {
    incomplete_jump *new_inc =
        (incomplete_jump *)malloc(sizeof(incomplete_jump));

    new_inc->instrNo = instrNo;
    new_inc->iaddress = iaddress;

    incjumps_vec.push_back(new_inc);
}

void patch_incomplete_jumps() {
    for (unsigned i = 0; i < incjumps_vec.size(); i++) {
        if (incjumps_vec[i]->iaddress == nextquadlabel()) {
            instruction_table[incjumps_vec[i]->instrNo]->result.val =
                nextinstructionlabel();
        } else {
            instruction_table[incjumps_vec[i]->instrNo]->result.val =
                quad_table[incjumps_vec[i]->iaddress]->taddress;
        }
    }
}

void make_numberoperand(vmarg *arg, double val) {
    arg->val = consts_newnumber(val);
    arg->type = number_a;
}

void make_booloperand(vmarg *arg, unsigned val) {
    arg->val = val;
    arg->type = bool_a;
}

void make_retvaloperand(vmarg *arg) { arg->type = retval_a; }

void reset_operand(vmarg *arg) {
    arg->val = 0;
    arg->type = undef_a;
}

unsigned nextinstructionlabel() { return currInst; }

void generate(vmopcode op, quad *quad) {
    instruction *t = new instruction();
    t->opcode = op;


    if (quad->arg1) make_operand(quad->arg1, &t->arg1);

    if (quad->arg2) make_operand(quad->arg2, &t->arg2);

    if (quad->result) make_operand(quad->result, &t->result);

    t->srcLine = quad->line;

    quad->taddress = nextinstructionlabel();

    vm_emit(t);
}

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

void generate_relational(vmopcode op, quad *q) {
    instruction t;
    t.opcode = op;
    t.arg1 = vmarg();
    t.arg2 = vmarg();
    t.result = vmarg();
    t.srcLine = q->line;

    if (q->arg1) make_operand(q->arg1, &t.arg1);
    if (q->arg2) make_operand(q->arg2, &t.arg2);

    // mark as label
    t.result.type = label_a;

    if (q->label < quad_table.size()) {
        if (quad_table[q->label]->taddress != (unsigned)-1) {
            // backward jump: target already has taddress
            t.result.val = quad_table[q->label]->taddress;
        } else {
            // forward jump: patch later
            t.result.val = 0;
            add_incomplete_jump(nextinstructionlabel(), q->label);
        }
    } else {
        std::cerr << "Invalid jump label " << q->label << "\n";
    }

    q->taddress = nextinstructionlabel();
    vm_emit(&t);
}


void generate_ADD(quad *q) { generate(add_v, q); }
void generate_SUB(quad *q) { generate(sub_v, q); }
void generate_MUL(quad *q) { generate(mul_v, q); }
void generate_DIV(quad *q) { generate(div_v, q); }
void generate_MOD(quad *q) { generate(mod_v, q); }
void generate_NEWTABLE(quad *q) { generate(newtable_v, q); }
void generate_TABLEGETELEM(quad *q) { generate(tablegetelem_v, q); }
void generate_TABLESETELEM(quad *q) { generate(tablesetelem_v, q); }
void generate_ASSIGN(quad *q) { generate(assign_v, q); }

void generate_UMINUS(quad *q) {
    q->taddress = nextinstructionlabel();
    instruction *instr = new instruction();
    instr->opcode = mul_v;
    instr->srcLine = q->line;
    if (q->arg1) make_operand(q->arg1, &instr->arg1);

    make_numberoperand(&instr->arg2, -1);

    if (q->result) make_operand(q->result, &instr->result);

    vm_emit(instr);
}

void generate_NOP(quad *) {
    instruction *t = new instruction();
    t->opcode = not_v;
    // ??
    t->arg1.type = undef_a;
    t->arg2.type = undef_a;
    t->result.type = undef_a;

    vm_emit(t);
}

// void generate_JUMP(quad *q) { generate_relational(jump_v, q); }

instruction *new_jump_inst(quad *q, vmopcode op) {
    instruction *instr = new instruction();
    instr->opcode = op;
    instr->srcLine = q->line;

    // record this quad's instruction index
    q->taddress = nextinstructionlabel();

    // initialize label operand to undef, then queue for patch
    instr->result.type = label_a;
    instr->result.val = 0;
    add_incomplete_jump(q->taddress, q->label);

    // unused args
    instr->arg1.type = undef_a;
    instr->arg2.type = undef_a;

    return instr;
}


void generate_JUMP(quad *q) { vm_emit(new_jump_inst(q, jump_v)); }


void generate_IF_EQ(quad *q) { generate_relational(jeq_v, q); }
void generate_IF_NOTEQ(quad *q) { generate_relational(jne_v, q); }
void generate_IF_GREATER(quad *q) { generate_relational(jgt_v, q); }
void generate_IF_GREATEREQ(quad *q) { generate_relational(jge_v, q); }

void generate_IF_LESS(quad *q) { generate_relational(jlt_v, q); }

void generate_IF_LESSEQ(quad *q) { generate_relational(jle_v, q); }


void generate_NOT(quad *q) { return; }


void generate_OR(quad *q) { return; }

void generate_AND(quad *q) { return; }

void generate_PARAM(quad *q) {
    q->taddress = nextinstructionlabel();
    instruction *t = new instruction();
    t->opcode = pusharg_v;


    if (q->arg1) make_operand(q->arg1, &t->arg1);

    vm_emit(t);
}

void generate_CALL(quad *q) {
    q->taddress = nextinstructionlabel();
    instruction *t = new instruction();
    t->opcode = call_v;

    if (q->result) make_operand(q->result, &t->result);

    vm_emit(t);
}

void generate_GETRETVAL(quad *q) {
    assert(q);

    instruction *t = new instruction();
    t->opcode = assign_v;
    q->taddress = nextinstructionlabel();
    t->srcLine = q->line;


    if (q->result) {
        make_operand(q->result, &t->result);
    }

    make_retvaloperand(&t->arg1);

    vm_emit(t);
}

void generate_RETURN(quad *q) {
    assert(q);
    instruction *t = new instruction();
    t->opcode = assign_v;
    q->taddress = nextinstructionlabel();
    t->srcLine = q->line;

    if (q->result) {
        make_operand(q->result, &t->arg1);
    } else {
        t->arg1.type = undef_a;
        t->arg1.val = 0;
    }

    t->arg2.type = undef_a;
    t->arg2.val = 0;
    t->result.type = undef_a;
    t->result.val = 0;

    vm_emit(t);
}


void generate_FUNCEND(quad *q) {
    assert(q);
    q->taddress = nextinstructionlabel();

    instruction *t = new instruction();
    t->opcode = funcexit_v;
    t->srcLine = q->line;

    if (q->result) {
        t->result.val = labstack.back();
        labstack.pop_back();
        t->result.type = userfunc_a;
    }

    t->result.type = label_a;
    t->result.val = labstack.back();
    labstack.pop_back();
    vm_emit(t);
}

void generate_FUNCSTART(quad *q) {
    assert(q);
    SymbolTableEntry_T f = q->result->sym;

    q->taddress = nextinstructionlabel();
    instruction *t = new instruction();
    t->opcode = funcenter_v;
    t->srcLine = q->line;

    if (q->result) make_operand(q->result, &t->result);
    labstack.push_back(funcstack.size() - 1);

    vm_emit(t);
}


unsigned userfunc_newfunc(SymbolTableEntry_T sym) {
    assert(sym);
    assert(sym->type == USERFUNC);
    for (unsigned int i = 0; i < funcstack.size(); ++i) {
        if (funcstack[i].id == sym->value.funcVal->name) {
            return i;
        }
    }
    userfunc *f = new userfunc();
    f->address = sym->value.funcVal->funcIndex;
    f->localSize = sym->value.funcVal->totalLocals;
    f->id = sym->value.funcVal->name;
    funcstack.push_back(*f);
    return funcstack.size() - 1;
}

void generate_instructions() {
    printf("Generating instructions...\n");
    for (auto &q : quad_table) {
        if (q) q->taddress = (unsigned)-1;
    }

    for (auto *q : quad_table) {
        if (!q) {
            continue;
        }

        q->taddress = nextinstructionlabel();


        generators[q->op](q);
        printf("Finished generator for op = %d.\n", q->op);
    }

    patch_incomplete_jumps();
}

void free_instructions() {
    for (auto inst : instruction_table) {
        if (inst) free(inst);
    }
    instruction_table.clear();
    currInst = 0;
}

// prints
void print_instructions() {
    std::string argCodes[] = {"label_a",    "global_a",  "formal_a", "local_a",
                              "number_a",   "string_a",  "bool_a",   "nil_a",
                              "userfunc_a", "libfunc_a", "retval_a", "undef_a"};

    const int col_num_width = 4;
    const int col_opcode_width = 15;
    const int col_arg_width = 20;
    const int col_srcline_width = 10;

    std::cout
        << "\n========= DEBUG PRINT: INSTRUCTIONS (Simple Aligned) =========\n";
    std::cout << std::left << std::setw(col_num_width) << "No." << std::left
              << std::setw(col_opcode_width) << "Opcode" << std::left
              << std::setw(col_arg_width) << "Result" << std::left
              << std::setw(col_arg_width) << "Arg1" << std::left
              << std::setw(col_arg_width) << "Arg2" << std::left
              << std::setw(col_srcline_width) << "SrcLine" << std::endl;

    int total_width_for_separator = col_num_width + col_opcode_width +
                                    (col_arg_width * 3) + col_srcline_width;
    std::cout << std::string(total_width_for_separator, '-') << std::endl;


    for (unsigned int i = 0; i < instruction_table.size(); ++i) {
        instruction *inst = instruction_table[i];
        if (!inst) {
            std::cout << std::left << std::setw(col_num_width) << i
                      << ": null pointer\n";
            continue;
        }

        std::cout << std::left << std::setw(col_num_width) << i << " ";

        std::cout << std::left << std::setw(col_opcode_width)
                  << vmopcode_to_string(inst->opcode);

        std::string result_str = " ";
        if (inst->result.type != undef_a) {
            result_str = argCodes[inst->result.type] + ":" +
                         std::to_string(inst->result.val);
        } else if (inst->result.type == undef_a) {
            result_str = argCodes[inst->result.type];
        } else {
            result_str = "unused_result";
        }
        std::cout << std::left << std::setw(col_arg_width) << result_str;

        std::string arg1_str = " ";
        if (inst->arg1.type != undef_a) {
            arg1_str = argCodes[inst->arg1.type] + ":" +
                       std::to_string(inst->arg1.val);
        } else if (inst->arg1.type == undef_a) {
            arg1_str = argCodes[inst->arg1.type];
        } else {
            arg1_str = "unused_arg1";
        }
        std::cout << std::left << std::setw(col_arg_width) << arg1_str;

        std::string arg2_str = " ";
        if (inst->arg2.type != undef_a) {
            arg2_str = argCodes[inst->arg2.type] + ":" +
                       std::to_string(inst->arg2.val);
        } else if (inst->arg2.type == undef_a) {
            arg2_str = argCodes[inst->arg2.type];
        } else {
            arg2_str = "unused_arg2";
        }
        std::cout << std::left << std::setw(col_arg_width) << arg2_str;

        std::cout << std::left << std::setw(col_srcline_width)
                  << (inst->srcLine > 0 ? std::to_string(inst->srcLine) : "")
                  << "\n";
    }
    std::cout << std::string(total_width_for_separator, '-') << std::endl;
    std::cout << "=======================================\n";
}

void print_const_strings(void) {
    for (unsigned i = 0; i < string_vec_consts.size(); i++) {
        std::cout << i << " : " << string_vec_consts[i] << std::endl;
    }
}
void print_const_nums(void) {
    for (unsigned i = 0; i < num_vec_consts.size(); i++) {
        std::cout << i << " : " << num_vec_consts[i] << std::endl;
    }
}

void print_libfuncs(void) {
    for (unsigned i = 0; i < lig_strvec_consts.size(); i++) {
        std::cout << i << " : " << lig_strvec_consts[i] << std::endl;
    }
}
void print_userfuncs(void) {
    for (unsigned i = 0; i < funcstack.size(); i++) {
        std::cout << i << " : " << funcstack[i].id << ", "
                  << "Address: " << funcstack[i].address
                  << ", Local Size: " << funcstack[i].localSize << std::endl;
    }
}

void generate_binary_readable(const std::string &outname) {
    std::ofstream outfile(outname, std::ios::binary | std::ios::trunc);
    if (!outfile.is_open()) {
        return;
    }

    // write the magic number but where ?
    unsigned int magic_number = 340200501;
    outfile.write(reinterpret_cast<const char *>(&magic_number),
                  sizeof(magic_number));

    // Strings
    // how many strings we have in this array = number of inputs in the string
    // vector write down each input
    unsigned int total_strings = string_vec_consts.size();
    outfile.write(reinterpret_cast<const char *>(&total_strings),
                  sizeof(total_strings));
    for (const std::string &s : string_vec_consts) {
        unsigned int str_len = s.length();
        outfile.write(reinterpret_cast<const char *>(&str_len),
                      sizeof(unsigned int));
        outfile.write(s.c_str(), str_len);
        char null_terminator = '\0';
        outfile.write(&null_terminator, sizeof(char));
    }

    // Double
    // same as ints but for double vector
    unsigned int total_numbers_double = num_vec_consts.size();
    outfile.write(reinterpret_cast<const char *>(&total_numbers_double),
                  sizeof(unsigned int));
    for (double d : num_vec_consts) {
        outfile.write(reinterpret_cast<const char *>(&d), sizeof(double));
    }

    // User functions
    // write how many functions we have = No of inputs in the userfunc vector
    // write down id,iaddress and...
    unsigned int total_user_funcs = funcstack.size();
    outfile.write(reinterpret_cast<const char *>(&total_user_funcs),
                  sizeof(unsigned int));
    for (const userfunc &uf : funcstack) {
        outfile.write(reinterpret_cast<const char *>(&uf.address),
                      sizeof(unsigned int));
        outfile.write(reinterpret_cast<const char *>(&uf.localSize),
                      sizeof(unsigned int));
        unsigned int id_len = uf.id.length();
        outfile.write(reinterpret_cast<const char *>(&id_len),
                      sizeof(unsigned int));
        outfile.write(uf.id.c_str(), id_len);
        char null_terminator_id = '\0';
        outfile.write(&null_terminator_id, sizeof(char));
    }

    // lib funcs
    // find how many lib funcs we use from the lib_str_func vector
    // which functions -> output = strings ( for example : print,cos etc)
    unsigned int total_lib_funcs = lig_strvec_consts.size();
    outfile.write(reinterpret_cast<const char *>(&total_lib_funcs),
                  sizeof(unsigned int));
    for (const std::string &s_lib : lig_strvec_consts) {
        unsigned int lib_str_len = s_lib.length();
        outfile.write(reinterpret_cast<const char *>(&lib_str_len),
                      sizeof(unsigned int));
        outfile.write(s_lib.c_str(), lib_str_len);
        char null_terminator_lib = '\0';
        outfile.write(&null_terminator_lib, sizeof(char));
    }

    // All of the above :
    // We can know the number of inputs for each vector with :
    // <name_of_vector>.size(); we iterate with auto for for the vec and we :
    // <name_of_vector>[i] ----> this is the value


    // We now have the instruction table
    // since the print is working right we have the 4 'byte' code but it is not
    // yet encoded

    unsigned int total_instructions = instruction_table.size();
    outfile.write(reinterpret_cast<const char *>(&total_instructions),
                  sizeof(unsigned int));
    for (const instruction *instr_ptr : instruction_table) {
        if (!instr_ptr) {
            continue;
        }

        uint8_t opcode_byte = static_cast<uint8_t>(instr_ptr->opcode);
        outfile.write(reinterpret_cast<const char *>(&opcode_byte),
                      sizeof(uint8_t));

        vmarg default_undef_vmarg;
        default_undef_vmarg.type = undef_a;
        default_undef_vmarg.val = 0;

        const vmarg *vmarg_operands[3] = {
            &(instr_ptr->result), &(instr_ptr->arg1), &(instr_ptr->arg2)};

        for (int k = 0; k < 3; ++k) {
            const vmarg *current_vmarg = vmarg_operands[k];
            if (current_vmarg && current_vmarg->type != undef_a) {
                uint8_t type_byte = static_cast<uint8_t>(current_vmarg->type);
                outfile.write(reinterpret_cast<const char *>(&type_byte),
                              sizeof(uint8_t));
                outfile.write(
                    reinterpret_cast<const char *>(&current_vmarg->val),
                    sizeof(unsigned int));


            } else {
                uint8_t type_byte =
                    static_cast<uint8_t>(default_undef_vmarg.type);
                outfile.write(reinterpret_cast<const char *>(&type_byte),
                              sizeof(uint8_t));
                outfile.write(
                    reinterpret_cast<const char *>(&default_undef_vmarg.val),
                    sizeof(unsigned int));
            }
        }
    }


    outfile.write(reinterpret_cast<const char *>(&total_globals), sizeof(int));

    std::cout << "Totals : " << total_globals << std::endl;

    outfile.close();
}

void generate_txt_read(const std::string &outname) {
    std::ofstream outfile(outname);
    if (!outfile.is_open()) {
        return;
    }

    std::string argCodes[] = {"label_a",    "global_a",  "formal_a", "local_a",
                              "number_a",   "string_a",  "bool_a",   "nil_a",
                              "userfunc_a", "libfunc_a", "retval_a", "undef_a"};

    outfile
        << "========= DEBUG PRINT: INSTRUCTIONS (Simple Aligned) =========\n";
    const int col_num = 4;
    const int col_opcode = 15;
    const int col_arg = 20;
    const int col_srcline = 10;

    outfile << std::left << std::setw(col_num) << "No." << std::left
            << std::setw(col_opcode) << "Opcode" << std::left
            << std::setw(col_arg) << "Result" << std::left << std::setw(col_arg)
            << "Arg1" << std::left << std::setw(col_arg) << "Arg2" << std::left
            << std::setw(col_srcline) << "SrcLine" << "\n";

    int separator_length = col_num + col_opcode + (col_arg * 3) + col_srcline;
    outfile << std::string(separator_length, '-') << "\n";

    for (unsigned int i = 0; i < instruction_table.size(); ++i) {
        instruction *inst = instruction_table[i];
        if (!inst) {
            outfile << std::left << std::setw(col_num) << i
                    << "null instruction pointer\n";
            continue;
        }

        outfile << std::left << std::setw(col_num) << i << " ";
        outfile << std::left << std::setw(col_opcode)
                << vmopcode_to_string(inst->opcode);

        std::string result_str = " ";
        if (inst->result.type != undef_a) {
            result_str = argCodes[inst->result.type] + ":" +
                         std::to_string(inst->result.val);
        }
        outfile << std::left << std::setw(col_arg) << result_str;

        std::string arg1_str = " ";
        if (inst->arg1.type != undef_a) {
            arg1_str = argCodes[inst->arg1.type] + ":" +
                       std::to_string(inst->arg1.val);
        }
        outfile << std::left << std::setw(col_arg) << arg1_str;

        std::string arg2_str = " ";
        if (inst->arg2.type != undef_a) {
            arg2_str = argCodes[inst->arg2.type] + ":" +
                       std::to_string(inst->arg2.val);
        }
        outfile << std::left << std::setw(col_arg) << arg2_str;

        outfile << std::left << std::setw(col_srcline)
                << (inst->srcLine > 0 ? std::to_string(inst->srcLine) : "")
                << "\n";
    }
    outfile << std::string(separator_length, '-') << "\n";
    outfile
        << "==============================================================\n";

    outfile << "print_const_strings:\n";
    for (size_t i = 0; i < string_vec_consts.size(); ++i) {
        outfile << i << " : \"" << string_vec_consts[i] << "\"\n";
    }

    outfile << "print_const_nums:\n";
    for (size_t i = 0; i < num_vec_consts.size(); ++i) {
        outfile << i << " : " << num_vec_consts[i] << "\n";
    }

    outfile << "print_userfuncs:\n";
    for (size_t i = 0; i < funcstack.size(); ++i) {
        const auto &uf = funcstack[i];
        outfile << i << " : ID=" << uf.id << ", Address=" << uf.address
                << ", Locals=" << uf.localSize << "\n";
    }

    outfile << "print_libfuncs:\n";
    for (size_t i = 0; i < lig_strvec_consts.size(); ++i) {
        outfile << i << " : " << lig_strvec_consts[i] << "\n";
    }

    outfile.close();
}