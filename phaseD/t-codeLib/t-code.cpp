#include "t-code.h" 

std::vector<instruction *> instruction_table;

// void make_operand (expr* e, vmarg* arg) {
//     switch (e->type) {



//         case var_e:
//         case tableitem_e:
//         case arithexpr_e:
//         case boolexpr_e:
//         case newtable_e: {


//             assert(e->sym);
//             unsigned final_var;
//             if (e->sym->type == GLOBAL || e->sym->type == LLOCAL || e->sym->type == FORMAL) {
//                 final_var = e->sym->value.varVal->offset;
//             } else {
//                 final_var = e->sym->value.funcVal->offset;
//             }
//             arg->val = final_var;
//             switch (e->sym->type) {
//                 case GLOBAL:     arg->type = global_a; break;
//                 case LLOCAL:     arg->type = local_a;  break;
//                 case FORMAL:     arg->type = formal_a; break;
//                 default: assert(0);
//             }
//             break;
//         }
//         case constbool_e: {
//             arg->val = e->boolConst;
//             arg->type = bool_a;
//             break;
//         }
//         case conststring_e: {
//             arg->val = consts_newstring(e->strConst);
//             arg->type = string_a;
//             break;
//         }
//         case constnum_e: {
//             arg->val = consts_newnumber(e->numConst);
//             arg->type = number_a;
//             break;
//         }
//         case nil_e: arg->type = nil_a; break;
//         case programfunc_e: {
//             arg->val = userfunc_newfunc(e->sym);
//             arg->type = userfunc_a;
//             break;
//         }
//         case libraryfunc_e: {
//             arg->val = libfuncs_newused(e->sym->value.funcVal->name);
//             arg->type = libfunc_a;
//             break;
//         }
//         default: assert(0);
//     }
// }


void vm_emit(vmopcode op , vmarg * arg1 ,vmarg * arg2,vmarg * result , unsigned srcLine){
    instruction * new_inst = (instruction *)malloc(sizeof(instruction));

    if(!new_inst){
        std::cerr<<"Failed !"<<std::endl;
        return;
    }

    new_inst->opcode    = op;
    new_inst->arg1      = arg1;
    new_inst->arg2      = arg2;
    new_inst->result    = result;
    new_inst->srcLine   = srcLine;
    instruction_table.push_back(new_inst);


}


void add_incomplete_jump(unsigned instrNo,unsigned iaddress){
    incomplete_jump * new_inc =(incomplete_jump *)malloc(sizeof(incomplete_jump));

    new_inc->instrNo    = instrNo;
    new_inc->iaddress   = iaddress;

    if(ij_head == NULL){
        new_inc->next = NULL;
    }else{
        new_inc->next = ij_head;
    }

    ij_head = new_inc;
}