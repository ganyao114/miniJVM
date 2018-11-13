

#include "../utils/d_type.h"
#include "jvm.h"
#include "java_native_std.h"
#include "jvm_util.h"
#include "garbage.h"


/* ==================================opcode implementation =============================*/



enum {
    /* 0x00 */ op_nop,
    /* 0x01 */ op_aconst_null,
    /* 0x02 */ op_iconst_m1,
    /* 0x03 */ op_iconst_0,
    /* 0x04 */ op_iconst_1,
    /* 0x05 */ op_iconst_2,
    /* 0x06 */ op_iconst_3,
    /* 0x07 */ op_iconst_4,
    /* 0x08 */ op_iconst_5,
    /* 0x09 */ op_lconst_0,
    /* 0x0A */ op_lconst_1,
    /* 0x0B */ op_fconst_0,
    /* 0x0C */ op_fconst_1,
    /* 0x0D */ op_fconst_2,
    /* 0x0E */ op_dconst_0,
    /* 0x0F */ op_dconst_1,
    /* 0x10 */ op_bipush,
    /* 0x11 */ op_sipush,
    /* 0x12 */ op_ldc,
    /* 0x13 */ op_ldc_w,
    /* 0x14 */ op_ldc2_w,
    /* 0x15 */ op_iload,
    /* 0x16 */ op_lload,
    /* 0x17 */ op_fload,
    /* 0x18 */ op_dload,
    /* 0x19 */ op_aload,
    /* 0x1A */ op_iload_0,
    /* 0x1B */ op_iload_1,
    /* 0x1C */ op_iload_2,
    /* 0x1D */ op_iload_3,
    /* 0x1E */ op_lload_0,
    /* 0x1F */ op_lload_1,
    /* 0x20 */ op_lload_2,
    /* 0x21 */ op_lload_3,
    /* 0x22 */ op_fload_0,
    /* 0x23 */ op_fload_1,
    /* 0x24 */ op_fload_2,
    /* 0x25 */ op_fload_3,
    /* 0x26 */ op_dload_0,
    /* 0x27 */ op_dload_1,
    /* 0x28 */ op_dload_2,
    /* 0x29 */ op_dload_3,
    /* 0x2A */ op_aload_0,
    /* 0x2B */ op_aload_1,
    /* 0x2C */ op_aload_2,
    /* 0x2D */ op_aload_3,
    /* 0x2E */ op_iaload,
    /* 0x2F */ op_laload,
    /* 0x30 */ op_faload,
    /* 0x31 */ op_daload,
    /* 0x32 */ op_aaload,
    /* 0x33 */ op_baload,
    /* 0x34 */ op_caload,
    /* 0x35 */ op_saload,
    /* 0x36 */ op_istore,
    /* 0x37 */ op_lstore,
    /* 0x38 */ op_fstore,
    /* 0x39 */ op_dstore,
    /* 0x3A */ op_astore,
    /* 0x3B */ op_istore_0,
    /* 0x3C */ op_istore_1,
    /* 0x3D */ op_istore_2,
    /* 0x3E */ op_istore_3,
    /* 0x3F */ op_lstore_0,
    /* 0x40 */ op_lstore_1,
    /* 0x41 */ op_lstore_2,
    /* 0x42 */ op_lstore_3,
    /* 0x43 */ op_fstore_0,
    /* 0x44 */ op_fstore_1,
    /* 0x45 */ op_fstore_2,
    /* 0x46 */ op_fstore_3,
    /* 0x47 */ op_dstore_0,
    /* 0x48 */ op_dstore_1,
    /* 0x49 */ op_dstore_2,
    /* 0x4A */ op_dstore_3,
    /* 0x4B */ op_astore_0,
    /* 0x4C */ op_astore_1,
    /* 0x4D */ op_astore_2,
    /* 0x4E */ op_astore_3,
    /* 0x4F */ op_iastore,
    /* 0x50 */ op_lastore,
    /* 0x51 */ op_fastore,
    /* 0x52 */ op_dastore,
    /* 0x53 */ op_aastore,
    /* 0x54 */ op_bastore,
    /* 0x55 */ op_castore,
    /* 0x56 */ op_sastore,
    /* 0x57 */ op_pop,
    /* 0x58 */ op_pop2,
    /* 0x59 */ op_dup,
    /* 0x5A */ op_dup_x1,
    /* 0x5B */ op_dup_x2,
    /* 0x5C */ op_dup2,
    /* 0x5D */ op_dup2_x1,
    /* 0x5E */ op_dup2_x2,
    /* 0x5F */ op_swap,
    /* 0x60 */ op_iadd,
    /* 0x61 */ op_ladd,
    /* 0x62 */ op_fadd,
    /* 0x63 */ op_dadd,
    /* 0x64 */ op_isub,
    /* 0x65 */ op_lsub,
    /* 0x66 */ op_fsub,
    /* 0x67 */ op_dsub,
    /* 0x68 */ op_imul,
    /* 0x69 */ op_lmul,
    /* 0x6A */ op_fmul,
    /* 0x6B */ op_dmul,
    /* 0x6C */ op_idiv,
    /* 0x6D */ op_ldiv,
    /* 0x6E */ op_fdiv,
    /* 0x6F */ op_ddiv,
    /* 0x70 */ op_irem,
    /* 0x71 */ op_lrem,
    /* 0x72 */ op_frem,
    /* 0x73 */ op_drem,
    /* 0x74 */ op_ineg,
    /* 0x75 */ op_lneg,
    /* 0x76 */ op_fneg,
    /* 0x77 */ op_dneg,
    /* 0x78 */ op_ishl,
    /* 0x79 */ op_lshl,
    /* 0x7A */ op_ishr,
    /* 0x7B */ op_lshr,
    /* 0x7C */ op_iushr,
    /* 0x7D */ op_lushr,
    /* 0x7E */ op_iand,
    /* 0x7F */ op_land,
    /* 0x80 */ op_ior,
    /* 0x81 */ op_lor,
    /* 0x82 */ op_ixor,
    /* 0x83 */ op_lxor,
    /* 0x84 */ op_iinc,
    /* 0x85 */ op_i2l,
    /* 0x86 */ op_i2f,
    /* 0x87 */ op_i2d,
    /* 0x88 */ op_l2i,
    /* 0x89 */ op_l2f,
    /* 0x8A */ op_l2d,
    /* 0x8B */ op_f2i,
    /* 0x8C */ op_f2l,
    /* 0x8D */ op_f2d,
    /* 0x8E */ op_d2i,
    /* 0x8F */ op_d2l,
    /* 0x90 */ op_d2f,
    /* 0x91 */ op_i2b,
    /* 0x92 */ op_i2c,
    /* 0x93 */ op_i2s,
    /* 0x94 */ op_lcmp,
    /* 0x95 */ op_fcmpl,
    /* 0x96 */ op_fcmpg,
    /* 0x97 */ op_dcmpl,
    /* 0x98 */ op_dcmpg,
    /* 0x99 */ op_ifeq,
    /* 0x9A */ op_ifne,
    /* 0x9B */ op_iflt,
    /* 0x9C */ op_ifge,
    /* 0x9D */ op_ifgt,
    /* 0x9E */ op_ifle,
    /* 0x9F */ op_if_icmpeq,
    /* 0xA0 */ op_if_icmpne,
    /* 0xA1 */ op_if_icmplt,
    /* 0xA2 */ op_if_icmpge,
    /* 0xA3 */ op_if_icmpgt,
    /* 0xA4 */ op_if_icmple,
    /* 0xA5 */ op_if_acmpeq,
    /* 0xA6 */ op_if_acmpne,
    /* 0xA7 */ op_goto,
    /* 0xA8 */ op_jsr,
    /* 0xA9 */ op_ret,
    /* 0xAA */ op_tableswitch,
    /* 0xAB */ op_lookupswitch,
    /* 0xAC */ op_ireturn,
    /* 0xAD */ op_lreturn,
    /* 0xAE */ op_freturn,
    /* 0xAF */ op_dreturn,
    /* 0xB0 */ op_areturn,
    /* 0xB1 */ op_return,
    /* 0xB2 */ op_getstatic,
    /* 0xB3 */ op_putstatic,
    /* 0xB4 */ op_getfield,
    /* 0xB5 */ op_putfield,
    /* 0xB6 */ op_invokevirtual,
    /* 0xB7 */ op_invokespecial,
    /* 0xB8 */ op_invokestatic,
    /* 0xB9 */ op_invokeinterface,
    /* 0xBA */ op_invokedynamic,
    /* 0xBB */ op_new,
    /* 0xBC */ op_newarray,
    /* 0xBD */ op_anewarray,
    /* 0xBE */ op_arraylength,
    /* 0xBF */ op_athrow,
    /* 0xC0 */ op_checkcast,
    /* 0xC1 */ op_instanceof,
    /* 0xC2 */ op_monitorenter,
    /* 0xC3 */ op_monitorexit,
    /* 0xC4 */ op_wide,
    /* 0xC5 */ op_multianewarray,
    /* 0xC6 */ op_ifnull,
    /* 0xC7 */ op_ifnonnull,
    /* 0xC8 */ op_0xc8,
    /* 0xC9 */ op_0xc9,
    /* 0xCA */ op_breakpoint,
};

static inline u8 *_op_aload_n(u8 *opCode, RuntimeStack *stack, LocalVarItem *localvar, Runtime *runtime, s32 i) {
    __refer value = localvar_getRefer(localvar, i);
    push_ref(stack, value);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("aload_%d push localvar [%llx] into stack\n", i, (s64)(intptr_t)value);
#endif
    opCode += 1;
    return opCode;
}


static inline u8 *_op_ifload_n(u8 *opCode, RuntimeStack *stack, LocalVarItem *localvar, Runtime *runtime, s32 i) {
    Int2Float i2f;
    i2f.i = localvar_getInt(localvar, i);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("if_load_%d: push localvar(%d)= [%x]/%d/%f  \n", i, i, i2f.i, i2f.i, i2f.f);
#endif
    push_int(stack, i2f.i);
    opCode += 1;
    return opCode;
}


static inline void _op_ldc_impl(RuntimeStack *stack, JClass *clazz, Runtime *runtime, s32 index) {

    ConstantItem *item = class_get_constant_item(clazz, index);
    switch (item->tag) {
        case CONSTANT_INTEGER:
        case CONSTANT_FLOAT: {
            s32 v = class_get_constant_integer(clazz, index);
            push_int(stack, v);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
            invoke_deepth(runtime);
        jvm_printf("ldc: [%x] \n", v);
#endif
            break;
        }
        case CONSTANT_STRING_REF: {
            ConstantUTF8 *cutf = class_get_constant_utf8(clazz, class_get_constant_stringref(clazz, index)->stringIndex);
            push_ref(stack, (__refer) cutf->jstr);


#if _JVM_DEBUG_BYTECODE_DETAIL > 5
            invoke_deepth(runtime);
        jvm_printf("ldc: [%llx] =\"%s\"\n", (s64)(intptr_t)cutf->jstr, utf8_cstr(cutf->utfstr));
#endif
            break;
        }
        case CONSTANT_CLASS: {
            JClass *cl = classes_load_get(class_get_constant_classref(clazz, index)->name, runtime);
            if (!cl->ins_class) {
                cl->ins_class = insOfJavaLangClass_create_get(runtime, cl);
            }
            push_ref(stack, cl->ins_class);
            break;
        }
        default: {
            push_long(stack, 0);
            jvm_printf("ldc: something not implemention \n");
        }
    }
}

static inline u8 *_op_iconst_n(u8 *opCode, RuntimeStack *stack, Runtime *runtime, s32 i) {
    push_int(stack, i);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("iconst_%d: push %d into stack\n", i, i);
#endif
    opCode += 1;
    return opCode;
}

static inline u8 *_op_dconst_n(u8 *opCode, RuntimeStack *stack, Runtime *runtime, f64 d) {
    push_double(stack, d);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("dconst_%d: push %lf into stack\n", (s32)(d), d);
#endif
    opCode += 1;
    return opCode;
}

static inline u8 *_op_fconst_n(u8 *opCode, RuntimeStack *stack, Runtime *runtime, f32 f) {
    push_float(stack, f);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("fconst_%f: push %f into stack\n", (s32)f, f);
#endif
    opCode += 1;
    return opCode;
}

static inline u8 *_op_lconst_n(u8 *opCode, RuntimeStack *stack, Runtime *runtime, s64 i) {

    push_long(stack, i);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("lconst_%lld: push %lld into stack\n", i, i);
#endif
    opCode += 1;
    return opCode;
}

static inline u8 *_op_ldoad_n(u8 *opCode, RuntimeStack *stack, LocalVarItem *localvar, Runtime *runtime, s32 index) {
    s64 value = localvar_getLong(localvar, index);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("ld_load_%d: push localvar(%d)= [%llx]/%lld/%lf  \n", index, index, value, value, value);
#endif
    push_long(stack, value);
    opCode += 1;
    return opCode;
}

static inline u8 *_op_istore_n(u8 *opCode, RuntimeStack *stack, LocalVarItem *localvar, Runtime *runtime, s32 i) {
    s32 value = pop_int(stack);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("istore_%d: save %x/%d into localvar(%d)\n", i, value, value, i);
#endif
    localvar_setInt(localvar, i, value);
    opCode += 1;
    return opCode;
}

static inline u8 *_op_astore_n(u8 *opCode, RuntimeStack *stack, LocalVarItem *localvar, Runtime *runtime, s32 i) {
    __refer value = pop_ref(stack);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("astore_%d:  [%llx]\n", i, (s64)(intptr_t)value);
#endif
    localvar_setRefer(localvar, i, value);
    opCode += 1;
    return opCode;
}

static inline u8 *_op_ldstore_n(u8 *opCode, RuntimeStack *stack, LocalVarItem *localvar, Runtime *runtime, s32 i) {
    Long2Double l2d;
    l2d.l = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5

    invoke_deepth(runtime);
    jvm_printf("l(d)store_%d: save localvar(%d) [%llx]/%lld/%lf  \n", i, i, l2d.l, l2d.l, l2d.d);
#endif
    localvar_setLong(localvar, i, l2d.l);
    opCode += 1;
    return opCode;
}


static inline u8 *_op_ifstore_impl(u8 *opCode, RuntimeStack *stack, LocalVarItem *localvar, Runtime *runtime) {
    Short2Char s2c;
    if (runtime->wideMode) {
        s2c.c1 = opCode[1];
        s2c.c0 = opCode[2];
        opCode += 3;
        runtime->wideMode = 0;
    } else {
        s2c.s = (u8) opCode[1];
        opCode += 2;
    }

    s32 value = pop_int(stack);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("i(f)store: save  localvar(%d) [%x]/%d \n", s2c.s, value, value);
#endif
    localvar_setInt(localvar, (u16) s2c.s, value);
    return opCode;
}

static inline u8 *_op_ldstore_impl(u8 *opCode, RuntimeStack *stack, LocalVarItem *localvar, Runtime *runtime) {

    Short2Char s2c;
    if (runtime->wideMode) {
        s2c.c1 = opCode[1];
        s2c.c0 = opCode[2];
        opCode += 3;
        runtime->wideMode = 0;
    } else {
        s2c.s = (u8) opCode[1];
        opCode += 2;
    }

    Long2Double l2d;
    l2d.l = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("l(d)store: save localvar(%d) %llx/%lld/%lf  \n", s2c.s, l2d.l, l2d.l, l2d.d);
#endif
    localvar_setLong(localvar, (u16) s2c.s, l2d.l);
    return opCode;
}

void _op_notsupport(u8 *opCode, Runtime *runtime) {
    invoke_deepth(runtime);
    jvm_printf("not support instruct [%x]\n", opCode[0]);
    exit(-3);
}

//----------------------------------  tool func  ------------------------------------------

static ExceptionTable *
_find_exception_handler(Runtime *runtime, Instance *exception, CodeAttribute *ca, s32 offset, __refer exception_ref) {
    Instance *ins = (Instance *) exception_ref;

    s32 i;
    ExceptionTable *e = ca->exception_table;
    for (i = 0; i < ca->exception_table_length; i++) {

        if (offset >= (e + i)->start_pc
            && offset <= (e + i)->end_pc) {
            if (!(e + i)->catch_type) {
                return e + i;
            }
            ConstantClassRef *ccr = class_get_constant_classref(runtime->clazz, (e + i)->catch_type);
            JClass *catchClass = classes_load_get(ccr->name, runtime);
            if (instance_of(catchClass, exception, runtime))
                return e + i;
        }
    }
    return NULL;
}

static s32 filterClassName(Utf8String *clsName) {
    if (utf8_indexof_c(clsName, "com/sun") < 0
        && utf8_indexof_c(clsName, "java/") < 0
        && utf8_indexof_c(clsName, "javax/") < 0) {
        return 1;
    }
    return 0;
}

static void _printCodeAttribute(CodeAttribute *ca, JClass *p) {

    jvm_printf("attribute name : %s\n", utf8_cstr(class_get_utf8_string(p, ca->attribute_name_index)));
    jvm_printf("attribute arr_length: %d\n", ca->attribute_length);

    jvm_printf("max_stack: %d\n", ca->max_stack);
    jvm_printf("max_locals: %d\n", ca->max_locals);
    jvm_printf("code_length: %d\n", ca->code_length);
}

/**
* 从栈中取得实例对象，中间穿插着调用参数
* @param cmr cmr
* @param stack stack
* @return ins
*/
static inline Instance *getInstanceInStack(JClass *clazz, ConstantMethodRef *cmr, RuntimeStack *stack) {

    //    StackEntry entry;
    //    peek_entry(stack, &entry, stack->size - 1 - cmr->methodParaCount);
    //    Instance *ins = (Instance *) entry_2_refer(&entry);
    //    return ins;

    return stack->store[stack->size - 1 - cmr->para_slots].rvalue;
}

/**
* 把堆栈中的方法调用参数存入方法本地变量
* 调用方法前，父程序把函数参数推入堆栈，方法调用时，需要把堆栈中的参数存到本地变量
* @param method  method
* @param father  runtime of father
* @param son     runtime of son
*/


static inline void _stack2localvar(MethodInfo *method, LocalVarItem *localvar, RuntimeStack *stack) {

    s32 i_local = method->para_slots;
    //    memcpy(localvar, &stack->store[stack->size - i_local], i_local * sizeof(StackEntry));
    StackEntry *store = stack->store;
    s32 i;
    for (i = 0; i < i_local; i++) {
        localvar[i].lvalue = store[stack->size - (i_local - i)].lvalue;
        localvar[i].type = store[stack->size - (i_local - i)].type;
    }
    stack->size -= i_local;
}

static inline void _synchronized_lock_method(MethodInfo *method, Runtime *runtime) {
    //synchronized process
    {
        if (method->access_flags & ACC_STATIC) {
            jthread_lock((MemoryBlock *) runtime->clazz, runtime);
        } else {
            jthread_lock(&((Instance *) localvar_getRefer(runtime->localvar, 0))->mb, runtime);
        }
    }
}

static inline void _synchronized_unlock_method(MethodInfo *method, Runtime *runtime) {
    //synchronized process
    {
        if (method->access_flags & ACC_STATIC) {
            jthread_unlock((MemoryBlock *) runtime->clazz, runtime);
        } else {
            jthread_unlock(&((Instance *) localvar_getRefer(runtime->localvar, 0))->mb, runtime);
        }
    }
}


#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__
//#  define STORE_ADDRESS(index,label) __asm lea eax, label __asm mov edx,opcode_addr  __asm mov [edx][index * TYPE opcode_addr],eax
//#  define JUMP_TO_IP(cur_inst) { void* addr = *opcode_addr[cur_inst]; __asm jmp addr }
#  define STORE_ADDRESS(index,label) while(0){}
#  define JUMP_TO_IP(cur_inst) while(0){}
#else
#  define STORE_ADDRESS(index, label) opcode_addr[index] = &&label
#  define JUMP_TO_IP(cur_inst) goto *opcode_addr[cur_inst]
#endif

static void *opcode_addr[0xff];
static s32 init_addr = 0;

s32 execute_method_impl(MethodInfo *method, Runtime *pruntime, JClass *clazz) {

    if (!init_addr) {
        init_addr = 1;
        STORE_ADDRESS(op_nop, label_nop);
        STORE_ADDRESS(op_aconst_null, label_aconst_null);
        STORE_ADDRESS(op_iconst_m1, label_iconst_m1);
        STORE_ADDRESS(op_iconst_0, label_iconst_0);
        STORE_ADDRESS(op_iconst_1, label_iconst_1);
        STORE_ADDRESS(op_iconst_2, label_iconst_2);
        STORE_ADDRESS(op_iconst_3, label_iconst_3);
        STORE_ADDRESS(op_iconst_4, label_iconst_4);
        STORE_ADDRESS(op_iconst_5, label_iconst_5);
        STORE_ADDRESS(op_lconst_0, label_lconst_0);
        STORE_ADDRESS(op_lconst_1, label_lconst_1);
        STORE_ADDRESS(op_fconst_0, label_fconst_0);
        STORE_ADDRESS(op_fconst_1, label_fconst_1);
        STORE_ADDRESS(op_fconst_2, label_fconst_2);
        STORE_ADDRESS(op_dconst_0, label_dconst_0);
        STORE_ADDRESS(op_dconst_1, label_dconst_1);
        STORE_ADDRESS(op_bipush, label_bipush);
        STORE_ADDRESS(op_sipush, label_sipush);
        STORE_ADDRESS(op_ldc, label_ldc);
        STORE_ADDRESS(op_ldc_w, label_ldc_w);
        STORE_ADDRESS(op_ldc2_w, label_ldc2_w);
        STORE_ADDRESS(op_iload, label_iload);
        STORE_ADDRESS(op_lload, label_lload);
        STORE_ADDRESS(op_fload, label_fload);
        STORE_ADDRESS(op_dload, label_dload);
        STORE_ADDRESS(op_aload, label_aload);
        STORE_ADDRESS(op_iload_0, label_iload_0);
        STORE_ADDRESS(op_iload_1, label_iload_1);
        STORE_ADDRESS(op_iload_2, label_iload_2);
        STORE_ADDRESS(op_iload_3, label_iload_3);
        STORE_ADDRESS(op_lload_0, label_lload_0);
        STORE_ADDRESS(op_lload_1, label_lload_1);
        STORE_ADDRESS(op_lload_2, label_lload_2);
        STORE_ADDRESS(op_lload_3, label_lload_3);
        STORE_ADDRESS(op_fload_0, label_fload_0);
        STORE_ADDRESS(op_fload_1, label_fload_1);
        STORE_ADDRESS(op_fload_2, label_fload_2);
        STORE_ADDRESS(op_fload_3, label_fload_3);
        STORE_ADDRESS(op_dload_0, label_dload_0);
        STORE_ADDRESS(op_dload_1, label_dload_1);
        STORE_ADDRESS(op_dload_2, label_dload_2);
        STORE_ADDRESS(op_dload_3, label_dload_3);
        STORE_ADDRESS(op_aload_0, label_aload_0);
        STORE_ADDRESS(op_aload_1, label_aload_1);
        STORE_ADDRESS(op_aload_2, label_aload_2);
        STORE_ADDRESS(op_aload_3, label_aload_3);
        STORE_ADDRESS(op_iaload, label_iaload);
        STORE_ADDRESS(op_laload, label_laload);
        STORE_ADDRESS(op_faload, label_faload);
        STORE_ADDRESS(op_daload, label_daload);
        STORE_ADDRESS(op_aaload, label_aaload);
        STORE_ADDRESS(op_baload, label_baload);
        STORE_ADDRESS(op_caload, label_caload);
        STORE_ADDRESS(op_saload, label_saload);
        STORE_ADDRESS(op_istore, label_istore);
        STORE_ADDRESS(op_lstore, label_lstore);
        STORE_ADDRESS(op_fstore, label_fstore);
        STORE_ADDRESS(op_dstore, label_dstore);
        STORE_ADDRESS(op_astore, label_astore);
        STORE_ADDRESS(op_istore_0, label_istore_0);
        STORE_ADDRESS(op_istore_1, label_istore_1);
        STORE_ADDRESS(op_istore_2, label_istore_2);
        STORE_ADDRESS(op_istore_3, label_istore_3);
        STORE_ADDRESS(op_lstore_0, label_lstore_0);
        STORE_ADDRESS(op_lstore_1, label_lstore_1);
        STORE_ADDRESS(op_lstore_2, label_lstore_2);
        STORE_ADDRESS(op_lstore_3, label_lstore_3);
        STORE_ADDRESS(op_fstore_0, label_fstore_0);
        STORE_ADDRESS(op_fstore_1, label_fstore_1);
        STORE_ADDRESS(op_fstore_2, label_fstore_2);
        STORE_ADDRESS(op_fstore_3, label_fstore_3);
        STORE_ADDRESS(op_dstore_0, label_dstore_0);
        STORE_ADDRESS(op_dstore_1, label_dstore_1);
        STORE_ADDRESS(op_dstore_2, label_dstore_2);
        STORE_ADDRESS(op_dstore_3, label_dstore_3);
        STORE_ADDRESS(op_astore_0, label_astore_0);
        STORE_ADDRESS(op_astore_1, label_astore_1);
        STORE_ADDRESS(op_astore_2, label_astore_2);
        STORE_ADDRESS(op_astore_3, label_astore_3);
        STORE_ADDRESS(op_iastore, label_iastore);
        STORE_ADDRESS(op_lastore, label_lastore);
        STORE_ADDRESS(op_fastore, label_fastore);
        STORE_ADDRESS(op_dastore, label_dastore);
        STORE_ADDRESS(op_aastore, label_aastore);
        STORE_ADDRESS(op_bastore, label_bastore);
        STORE_ADDRESS(op_castore, label_castore);
        STORE_ADDRESS(op_sastore, label_sastore);
        STORE_ADDRESS(op_pop, label_pop);
        STORE_ADDRESS(op_pop2, label_pop2);
        STORE_ADDRESS(op_dup, label_dup);
        STORE_ADDRESS(op_dup_x1, label_dup_x1);
        STORE_ADDRESS(op_dup_x2, label_dup_x2);
        STORE_ADDRESS(op_dup2, label_dup2);
        STORE_ADDRESS(op_dup2_x1, label_dup2_x1);
        STORE_ADDRESS(op_dup2_x2, label_dup2_x2);
        STORE_ADDRESS(op_swap, label_swap);
        STORE_ADDRESS(op_iadd, label_iadd);
        STORE_ADDRESS(op_ladd, label_ladd);
        STORE_ADDRESS(op_fadd, label_fadd);
        STORE_ADDRESS(op_dadd, label_dadd);
        STORE_ADDRESS(op_isub, label_isub);
        STORE_ADDRESS(op_lsub, label_lsub);
        STORE_ADDRESS(op_fsub, label_fsub);
        STORE_ADDRESS(op_dsub, label_dsub);
        STORE_ADDRESS(op_imul, label_imul);
        STORE_ADDRESS(op_lmul, label_lmul);
        STORE_ADDRESS(op_fmul, label_fmul);
        STORE_ADDRESS(op_dmul, label_dmul);
        STORE_ADDRESS(op_idiv, label_idiv);
        STORE_ADDRESS(op_ldiv, label_ldiv);
        STORE_ADDRESS(op_fdiv, label_fdiv);
        STORE_ADDRESS(op_ddiv, label_ddiv);
        STORE_ADDRESS(op_irem, label_irem);
        STORE_ADDRESS(op_lrem, label_lrem);
        STORE_ADDRESS(op_frem, label_frem);
        STORE_ADDRESS(op_drem, label_drem);
        STORE_ADDRESS(op_ineg, label_ineg);
        STORE_ADDRESS(op_lneg, label_lneg);
        STORE_ADDRESS(op_fneg, label_fneg);
        STORE_ADDRESS(op_dneg, label_dneg);
        STORE_ADDRESS(op_ishl, label_ishl);
        STORE_ADDRESS(op_lshl, label_lshl);
        STORE_ADDRESS(op_ishr, label_ishr);
        STORE_ADDRESS(op_lshr, label_lshr);
        STORE_ADDRESS(op_iushr, label_iushr);
        STORE_ADDRESS(op_lushr, label_lushr);
        STORE_ADDRESS(op_iand, label_iand);
        STORE_ADDRESS(op_land, label_land);
        STORE_ADDRESS(op_ior, label_ior);
        STORE_ADDRESS(op_lor, label_lor);
        STORE_ADDRESS(op_ixor, label_ixor);
        STORE_ADDRESS(op_lxor, label_lxor);
        STORE_ADDRESS(op_iinc, label_iinc);
        STORE_ADDRESS(op_i2l, label_i2l);
        STORE_ADDRESS(op_i2f, label_i2f);
        STORE_ADDRESS(op_i2d, label_i2d);
        STORE_ADDRESS(op_l2i, label_l2i);
        STORE_ADDRESS(op_l2f, label_l2f);
        STORE_ADDRESS(op_l2d, label_l2d);
        STORE_ADDRESS(op_f2i, label_f2i);
        STORE_ADDRESS(op_f2l, label_f2l);
        STORE_ADDRESS(op_f2d, label_f2d);
        STORE_ADDRESS(op_d2i, label_d2i);
        STORE_ADDRESS(op_d2l, label_d2l);
        STORE_ADDRESS(op_d2f, label_d2f);
        STORE_ADDRESS(op_i2b, label_i2b);
        STORE_ADDRESS(op_i2c, label_i2c);
        STORE_ADDRESS(op_i2s, label_i2s);
        STORE_ADDRESS(op_lcmp, label_lcmp);
        STORE_ADDRESS(op_fcmpl, label_fcmpl);
        STORE_ADDRESS(op_fcmpg, label_fcmpg);
        STORE_ADDRESS(op_dcmpl, label_dcmpl);
        STORE_ADDRESS(op_dcmpg, label_dcmpg);
        STORE_ADDRESS(op_ifeq, label_ifeq);
        STORE_ADDRESS(op_ifne, label_ifne);
        STORE_ADDRESS(op_iflt, label_iflt);
        STORE_ADDRESS(op_ifge, label_ifge);
        STORE_ADDRESS(op_ifgt, label_ifgt);
        STORE_ADDRESS(op_ifle, label_ifle);
        STORE_ADDRESS(op_if_icmpeq, label_if_icmpeq);
        STORE_ADDRESS(op_if_icmpne, label_if_icmpne);
        STORE_ADDRESS(op_if_icmplt, label_if_icmplt);
        STORE_ADDRESS(op_if_icmpge, label_if_icmpge);
        STORE_ADDRESS(op_if_icmpgt, label_if_icmpgt);
        STORE_ADDRESS(op_if_icmple, label_if_icmple);
        STORE_ADDRESS(op_if_acmpeq, label_if_acmpeq);
        STORE_ADDRESS(op_if_acmpne, label_if_acmpne);
        STORE_ADDRESS(op_goto, label_goto);
        STORE_ADDRESS(op_jsr, label_jsr);
        STORE_ADDRESS(op_ret, label_ret);
        STORE_ADDRESS(op_tableswitch, label_tableswitch);
        STORE_ADDRESS(op_lookupswitch, label_lookupswitch);
        STORE_ADDRESS(op_ireturn, label_ireturn);
        STORE_ADDRESS(op_lreturn, label_lreturn);
        STORE_ADDRESS(op_freturn, label_freturn);
        STORE_ADDRESS(op_dreturn, label_dreturn);
        STORE_ADDRESS(op_areturn, label_areturn);
        STORE_ADDRESS(op_return, label_return);
        STORE_ADDRESS(op_getstatic, label_getstatic);
        STORE_ADDRESS(op_putstatic, label_putstatic);
        STORE_ADDRESS(op_getfield, label_getfield);
        STORE_ADDRESS(op_putfield, label_putfield);
        STORE_ADDRESS(op_invokevirtual, label_invokevirtual);
        STORE_ADDRESS(op_invokespecial, label_invokespecial);
        STORE_ADDRESS(op_invokestatic, label_invokestatic);
        STORE_ADDRESS(op_invokeinterface, label_invokeinterface);
        STORE_ADDRESS(op_invokedynamic, label_invokedynamic);
        STORE_ADDRESS(op_new, label_new);
        STORE_ADDRESS(op_newarray, label_newarray);
        STORE_ADDRESS(op_anewarray, label_anewarray);
        STORE_ADDRESS(op_arraylength, label_arraylength);
        STORE_ADDRESS(op_athrow, label_athrow);
        STORE_ADDRESS(op_checkcast, label_checkcast);
        STORE_ADDRESS(op_instanceof, label_instanceof);
        STORE_ADDRESS(op_monitorenter, label_monitorenter);
        STORE_ADDRESS(op_monitorexit, label_monitorexit);
        STORE_ADDRESS(op_wide, label_wide);
        STORE_ADDRESS(op_multianewarray, label_multianewarray);
        STORE_ADDRESS(op_ifnull, label_ifnull);
        STORE_ADDRESS(op_ifnonnull, label_ifnonnull);
        STORE_ADDRESS(op_0xc8, label_0xc8);
        STORE_ADDRESS(op_0xc9, label_0xc9);
        STORE_ADDRESS(op_breakpoint, label_breakpoint);


    }

    s32 ret = RUNTIME_STATUS_NORMAL;

    //准备方法栈
    Runtime *runtime = runtime_create_inl(pruntime);

    runtime->method = method;
    runtime->clazz = clazz;
    while (clazz->status < CLASS_STATUS_CLINITING) {
        class_clinit(clazz, runtime);
    }
    s32 method_sync = method->access_flags & ACC_SYNCHRONIZED;
    //    if (utf8_equals_c(method->name, "getMethod")) {
    //        s32 debug = 1;
    //    }
    //操作数栈
    RuntimeStack *stack = runtime->stack;

    if (!(method->access_flags & ACC_NATIVE)) {
        //拿出 Code
        CodeAttribute *ca = method->converted_code;
        if (ca) {
            //初始化本地变量
            localvar_init(runtime, ca->max_locals);
            LocalVarItem *localvar = runtime->localvar;
            //方法参数进入本地变量
            _stack2localvar(method, localvar, stack);
            s32 stackSize = stack->size;

            //如果方法是同步的，加锁
            if (method_sync)_synchronized_lock_method(method, runtime);

            u8 *opCode = ca->code;
            runtime->ca = ca;
            JavaThreadInfo *threadInfo = runtime->threadInfo;
            
            //调试相关
            do {
                runtime->pc = opCode;
                u8 cur_inst = *opCode;
                if (java_debug) {
                    //breakpoint
                    if (method->breakpoint) {
                        jdwp_check_breakpoint(runtime);
                    }
                    //debug step
                    if (threadInfo->jdwp_step.active) {//单步状态
                        threadInfo->jdwp_step.bytecode_count++;
                        jdwp_check_debug_step(runtime);

                    }
                }
                //process thread suspend
                if (threadInfo->suspend_count) {
                    if (threadInfo->is_interrupt) {
                        ret = RUNTIME_STATUS_INTERRUPT;
                        break;
                    }
                    check_suspend_and_pause(runtime);
                }

#if _JVM_DEBUG_PROFILE
                u8 instruct_code = runtime->pc[0];
            s64 start_at = nanoTime();
#endif


                /* ==================================opcode start =============================*/
#ifdef __JVM_DEBUG__
                s64 inst_pc = runtime->pc - ca->code;
#endif
                JUMP_TO_IP(cur_inst);
                switch (cur_inst) {
                    label_nop:
                    case op_nop: {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("nop\n");
#endif
                        opCode += 1;

                        break;
                    }
                    label_aconst_null:
                    case op_aconst_null: {
                        push_ref(stack, NULL);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("aconst_null: push %d into stack\n", 0);
#endif
                        opCode += 1;

                        break;
                    }
                    label_iconst_m1:
                    case op_iconst_m1: {
                        opCode = _op_iconst_n(opCode, stack, runtime, -1);
                        break;
                    }


                    label_iconst_0:
                    case op_iconst_0: {
                        opCode = _op_iconst_n(opCode, stack, runtime, 0);
                        break;
                    }

                    label_iconst_1:
                    case op_iconst_1: {
                        opCode = _op_iconst_n(opCode, stack, runtime, 1);
                        break;
                    }

                    label_iconst_2:
                    case op_iconst_2: {
                        opCode = _op_iconst_n(opCode, stack, runtime, 2);
                        break;
                    }

                    label_iconst_3:
                    case op_iconst_3: {
                        opCode = _op_iconst_n(opCode, stack, runtime, 3);
                        break;
                    }

                    label_iconst_4:
                    case op_iconst_4: {
                        opCode = _op_iconst_n(opCode, stack, runtime, 4);
                        break;
                    }

                    label_iconst_5:
                    case op_iconst_5: {
                        opCode = _op_iconst_n(opCode, stack, runtime, 5);
                        break;
                    }

                    label_lconst_0:
                    case op_lconst_0: {
                        opCode = _op_lconst_n(opCode, stack, runtime, 0);
                        break;
                    }

                    label_lconst_1:
                    case op_lconst_1: {
                        opCode = _op_lconst_n(opCode, stack, runtime, 1);
                        break;
                    }


                    label_fconst_0:
                    case op_fconst_0: {
                        opCode = _op_fconst_n(opCode, stack, runtime, 0.0f);
                        break;
                    }

                    label_fconst_1:
                    case op_fconst_1: {
                        opCode = _op_fconst_n(opCode, stack, runtime, 1.0f);
                        break;
                    }

                    label_fconst_2:
                    case op_fconst_2: {
                        opCode = _op_fconst_n(opCode, stack, runtime, 2.0f);
                        break;
                    }


                    label_dconst_0:
                    case op_dconst_0: {
                        opCode = _op_dconst_n(opCode, stack, runtime, 0.0f);
                        break;
                    }

                    label_dconst_1:
                    case op_dconst_1: {
                        opCode = _op_dconst_n(opCode, stack, runtime, 1.0f);
                        break;
                    }

                    label_bipush:
                    case op_bipush: {
                        //此行 code 的第二个元素就是常量操作数
                        s32 value = (s8) opCode[1];
                        //常量入栈
                        push_int(stack, value);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("bipush a byte %d onto the stack \n", value);
#endif
                        opCode += 2;

                        break;
                    }

                    label_sipush:
                    case op_sipush: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("sipush value %d\n", s2c.s);
#endif
                        push_int(stack, s2c.s);
                        opCode += 3;

                        break;
                    }


                    label_ldc:
                    case op_ldc: {
                        s32 index = opCode[1];
                        opCode += 2;
                        _op_ldc_impl(stack, clazz, runtime, index);
                        break;
                    }

                    label_ldc_w:
                    case op_ldc_w: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];
                        s32 index = s2c.s;
                        opCode += 3;
                        _op_ldc_impl(stack, clazz, runtime, index);
                        break;
                    }

                    label_ldc2_w:
                    case op_ldc2_w: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        s32 index = s2c.s;
                        s64 value = class_get_constant_long(clazz, index);//long or double

                        push_long(stack, value);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("ldc2_w: push a constant(%d) [%llx] onto the stack \n", index, value);
#endif
                        opCode += 3;

                        break;
                    }


                    label_iload:
                    label_fload:
                    case op_iload:
                    case op_fload: {
                        Short2Char s2c;
                        if (runtime->wideMode) {
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += 3;
                            runtime->wideMode = 0;
                        } else {
                            s2c.s = (u8) opCode[1];
                            opCode += 2;
                        }

                        s32 value = localvar_getInt(localvar, (u16) s2c.s);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("i(fa)load: push localvar(%d)= [%x]/%d  \n", s2c.s, value, value);
#endif
                        push_int(stack, value);

                        break;
                    }


                    label_lload:
                    label_dload:
                    case op_lload:
                    case op_dload: {
                        Short2Char s2c;
                        if (runtime->wideMode) {
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += 3;
                            runtime->wideMode = 0;
                        } else {
                            s2c.s = (u8) opCode[1];
                            opCode += 2;
                        }

                        Long2Double l2d;
                        l2d.l = localvar_getLong(localvar, (u16) s2c.s);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("l(d)load: push localvar(%d) [%llx]/%lf into stack \n", s2c.s, l2d.l, l2d.d);
#endif
                        push_long(stack, l2d.l);

                        break;
                    }


                    label_aload:
                    case op_aload: {
                        //                        if (utf8_equals_c(method->name, "reshape")) {
                        //                            int debug = 1;
                        //                        }

                        Short2Char s2c;
                        if (runtime->wideMode) {
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += 3;
                            runtime->wideMode = 0;
                        } else {
                            s2c.s = (u8) opCode[1];
                            opCode += 2;
                        }

                        __refer value = localvar_getRefer(localvar, (u16) s2c.s);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("i(fa)load: push localvar(%d)= [%llx]  \n", s2c.s, (s64)(intptr_t)value);
#endif
                        push_ref(stack, value);

                        break;
                    }

                    label_iload_0:
                    case op_iload_0: {
                        opCode = _op_ifload_n(opCode, stack, localvar, runtime, 0);
                        break;
                    }

                    label_iload_1:
                    case op_iload_1: {
                        opCode = _op_ifload_n(opCode, stack, localvar, runtime, 1);
                        break;
                    }

                    label_iload_2:
                    case op_iload_2: {
                        opCode = _op_ifload_n(opCode, stack, localvar, runtime, 2);
                        break;
                    }

                    label_iload_3:
                    case op_iload_3: {
                        opCode = _op_ifload_n(opCode, stack, localvar, runtime, 3);
                        break;
                    }

                    label_lload_0:
                    case op_lload_0: {
                        opCode = _op_ldoad_n(opCode, stack, localvar, runtime, 0);
                        break;
                    }

                    label_lload_1:
                    case op_lload_1: {
                        opCode = _op_ldoad_n(opCode, stack, localvar, runtime, 1);
                        break;
                    }

                    label_lload_2:
                    case op_lload_2: {
                        opCode = _op_ldoad_n(opCode, stack, localvar, runtime, 2);
                        break;
                    }

                    label_lload_3:
                    case op_lload_3: {
                        opCode = _op_ldoad_n(opCode, stack, localvar, runtime, 3);
                        break;
                    }

                    label_fload_0:
                    case op_fload_0: {
                        opCode = _op_ifload_n(opCode, stack, localvar, runtime, 0);
                        break;
                    }

                    label_fload_1:
                    case op_fload_1: {
                        opCode = _op_ifload_n(opCode, stack, localvar, runtime, 1);
                        break;
                    }

                    label_fload_2:
                    case op_fload_2: {
                        opCode = _op_ifload_n(opCode, stack, localvar, runtime, 2);
                        break;
                    }

                    label_fload_3:
                    case op_fload_3: {
                        opCode = _op_ifload_n(opCode, stack, localvar, runtime, 3);
                        break;
                    }


                    label_dload_0:
                    case op_dload_0: {
                        opCode = _op_ldoad_n(opCode, stack, localvar, runtime, 0);
                        break;
                    }

                    label_dload_1:
                    case op_dload_1: {
                        opCode = _op_ldoad_n(opCode, stack, localvar, runtime, 1);
                        break;
                    }

                    label_dload_2:
                    case op_dload_2: {
                        opCode = _op_ldoad_n(opCode, stack, localvar, runtime, 2);
                        break;
                    }

                    label_dload_3:
                    case op_dload_3: {
                        opCode = _op_ldoad_n(opCode, stack, localvar, runtime, 3);
                        break;
                    }

                    label_aload_0:
                    case op_aload_0: {
                        opCode = _op_aload_n(opCode, stack, localvar, runtime, 0);
                        break;
                    }

                    label_aload_1:
                    case op_aload_1: {
                        opCode = _op_aload_n(opCode, stack, localvar, runtime, 1);
                        break;
                    }

                    label_aload_2:
                    case op_aload_2: {
                        opCode = _op_aload_n(opCode, stack, localvar, runtime, 2);
                        break;
                    }

                    label_aload_3:
                    case op_aload_3: {
                        opCode = _op_aload_n(opCode, stack, localvar, runtime, 3);
                        break;
                    }

                    label_faload:
                    label_iaload:
                    case op_iaload:
                    case op_faload: {
                        s32 index = pop_int(stack);
                        Instance *arr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(arr, index, runtime);
                        if (!ret) {
                            s32 s = *((s32 *) (arr->arr_body) + index);
                            push_int(stack, s);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("if_aload push arr[%llx].(%d)=%x:%d:%lf into stack\n", (u64)(intptr_t)arr, index,
                            s, s, *(f32 *)&s);
#endif
                        }
                        opCode += 1;
                        break;
                    }

                    label_laload:
                    label_daload:
                    case op_laload:
                    case op_daload: {
                        s32 index = pop_int(stack);
                        Instance *arr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(arr, index, runtime);
                        if (!ret) {
                            s64 s = *(((s64 *) arr->arr_body) + index);
                            push_long(stack, s);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("ld_aload push arr[%llx].(%d)=%llx:%lld:%lf into stack\n", (u64)(intptr_t)arr, index,
                            s, s, *(f64 *)&s);
#endif
                        }
                        opCode += 1;
                        break;
                    }

                    label_aaload:
                    case op_aaload: {
                        s32 index = pop_int(stack);
                        Instance *arr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(arr, index, runtime);
                        if (!ret) {
                            __refer s = *(((__refer *) arr->arr_body) + index);
                            push_ref(stack, s);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("aaload push arr[%llx].(%d)=%llx:%lld into stack\n", (u64)(intptr_t)arr, index,
                            (s64)(intptr_t)s, (s64)(intptr_t)s);
#endif
                        }
                        opCode += 1;
                        break;
                    }

                    label_baload:
                    case op_baload: {
                        s32 index = pop_int(stack);
                        Instance *arr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(arr, index, runtime);
                        if (!ret) {
                            s32 s = *(((s8 *) arr->arr_body) + index);
                            push_int(stack, s);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("iaload push arr[%llx].(%d)=%x:%d:%lf into stack\n", (u64)(intptr_t)arr, index,
                            s, s, *(f32 *)&s);
#endif
                        }
                        opCode += 1;
                        break;
                    }

                    label_caload:
                    case op_caload: {
                        s32 index = pop_int(stack);
                        Instance *arr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(arr, index, runtime);
                        if (!ret) {
                            s32 s = *(((u16 *) arr->arr_body) + index);
                            push_int(stack, s);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("iaload push arr[%llx].(%d)=%x:%d:%lf into stack\n", (u64)(intptr_t)arr, index,
                            s, s, *(f32 *)&s);
#endif
                        }
                        opCode += 1;
                        break;
                    }

                    label_saload:
                    case op_saload: {
                        s32 index = pop_int(stack);
                        Instance *arr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(arr, index, runtime);
                        if (!ret) {
                            s32 s = *(((s16 *) arr->arr_body) + index);
                            push_int(stack, s);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("iaload push arr[%llx].(%d)=%x:%d:%lf into stack\n", (u64)(intptr_t)arr, index,
                            s, s, *(f32 *)&s);
#endif
                        }
                        opCode += 1;
                        break;
                    }

                    label_istore:
                    case op_istore: {
                        opCode = _op_ifstore_impl(opCode, stack, localvar, runtime);
                        break;
                    }

                    label_lstore:
                    case op_lstore: {
                        opCode = _op_ldstore_impl(opCode, stack, localvar, runtime);
                        break;
                    }

                    label_fstore:
                    case op_fstore: {
                        opCode = _op_ifstore_impl(opCode, stack, localvar, runtime);
                        break;
                    }

                    label_dstore:
                    case op_dstore: {
                        opCode = _op_ldstore_impl(opCode, stack, localvar, runtime);
                        break;
                    }


                    label_astore:
                    case op_astore: {
                        Short2Char s2c;
                        if (runtime->wideMode) {
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += 3;
                            runtime->wideMode = 0;
                        } else {
                            s2c.s = (u8) opCode[1];
                            opCode += 2;
                        }

                        __refer value = pop_ref(stack);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("i(fa)store: save  localvar(%d) [%llx] \n", s2c.s, (s64)(intptr_t)value);
#endif
                        localvar_setRefer(localvar, (u16) s2c.s, value);


                        break;
                    }

                    label_istore_0:
                    case op_istore_0: {
                        opCode = _op_istore_n(opCode, stack, localvar, runtime, 0);
                        break;
                    }

                    label_istore_1:
                    case op_istore_1: {
                        opCode = _op_istore_n(opCode, stack, localvar, runtime, 1);
                        break;
                    }

                    label_istore_2:
                    case op_istore_2: {
                        opCode = _op_istore_n(opCode, stack, localvar, runtime, 2);
                        break;
                    }

                    label_istore_3:
                    case op_istore_3: {
                        opCode = _op_istore_n(opCode, stack, localvar, runtime, 3);
                        break;
                    }

                    label_lstore_0:
                    case op_lstore_0: {
                        opCode = _op_ldstore_n(opCode, stack, localvar, runtime, 0);
                        break;
                    }

                    label_lstore_1:
                    case op_lstore_1: {
                        opCode = _op_ldstore_n(opCode, stack, localvar, runtime, 1);
                        break;
                    }

                    label_lstore_2:
                    case op_lstore_2: {
                        opCode = _op_ldstore_n(opCode, stack, localvar, runtime, 2);
                        break;
                    }

                    label_lstore_3:
                    case op_lstore_3: {
                        opCode = _op_ldstore_n(opCode, stack, localvar, runtime, 3);
                        break;
                    }


                    label_fstore_0:
                    case op_fstore_0: {
                        opCode = _op_istore_n(opCode, stack, localvar, runtime, 0);
                        break;
                    }

                    label_fstore_1:
                    case op_fstore_1: {
                        opCode = _op_istore_n(opCode, stack, localvar, runtime, 1);
                        break;
                    }

                    label_fstore_2:
                    case op_fstore_2: {
                        opCode = _op_istore_n(opCode, stack, localvar, runtime, 2);
                        break;
                    }

                    label_fstore_3:
                    case op_fstore_3: {
                        opCode = _op_istore_n(opCode, stack, localvar, runtime, 3);
                        break;
                    }


                    label_dstore_0:
                    case op_dstore_0: {
                        opCode = _op_ldstore_n(opCode, stack, localvar, runtime, 0);
                        break;
                    }

                    label_dstore_1:
                    case op_dstore_1: {
                        opCode = _op_ldstore_n(opCode, stack, localvar, runtime, 1);
                        break;
                    }

                    case op_dstore_2: {
                        label_dstore_2:
                        opCode = _op_ldstore_n(opCode, stack, localvar, runtime, 2);
                        break;
                    }

                    label_dstore_3:
                    case op_dstore_3: {
                        opCode = _op_ldstore_n(opCode, stack, localvar, runtime, 3);
                        break;
                    }


                    label_astore_0:
                    case op_astore_0: {
                        opCode = _op_astore_n(opCode, stack, localvar, runtime, 0);
                        break;
                    }

                    label_astore_1:
                    case op_astore_1: {
                        opCode = _op_astore_n(opCode, stack, localvar, runtime, 1);
                        break;
                    }

                    label_astore_2:
                    case op_astore_2: {
                        opCode = _op_astore_n(opCode, stack, localvar, runtime, 2);
                        break;
                    }

                    label_astore_3:
                    case op_astore_3: {
                        opCode = _op_astore_n(opCode, stack, localvar, runtime, 3);
                        break;
                    }

                    label_fastore:
                    label_iastore:
                    case op_fastore:
                    case op_iastore: {
                        s32 i = pop_int(stack);
                        s32 index = pop_int(stack);
                        Instance *jarr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(jarr, index, runtime);
                        if (!ret) {
                            *(((s32 *) jarr->arr_body) + index) = i;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("iastore: save array[%llx].(%d)=%d)\n",
                            (s64)(intptr_t)jarr, index, i);
#endif
                        }
                        opCode += 1;
                        break;
                    }

                    label_dastore:
                    label_lastore:
                    case op_dastore:
                    case op_lastore: {
                        s64 j = pop_long(stack);
                        s32 index = pop_int(stack);
                        Instance *jarr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(jarr, index, runtime);
                        if (!ret) {
                            *(((s64 *) jarr->arr_body) + index) = j;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("iastore: save array[%llx].(%d)=%lld)\n",
                            (s64)(intptr_t)jarr, index, j);
#endif
                        }
                        opCode += 1;
                        break;
                    }


                    label_aastore:
                    case op_aastore: {
                        __refer r = pop_ref(stack);
                        s32 index = pop_int(stack);
                        Instance *jarr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(jarr, index, runtime);
                        if (!ret) {
                            *(((__refer *) jarr->arr_body) + index) = r;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("iastore: save array[%llx].(%d)=%llx)\n",
                            (s64)(intptr_t)jarr, index, (s64)(intptr_t)r);
#endif
                        }
                        opCode += 1;
                        break;
                    }

                    label_bastore:
                    case op_bastore: {
                        s32 i = pop_int(stack);
                        s32 index = pop_int(stack);
                        Instance *jarr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(jarr, index, runtime);
                        if (!ret) {
                            *(((s8 *) jarr->arr_body) + index) = (s8) i;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("iastore: save array[%llx].(%d)=%d)\n",
                            (s64)(intptr_t)jarr, index, i);
#endif
                        }
                        opCode += 1;
                        break;
                    }


                    label_castore:
                    case op_castore: {
                        s32 i = pop_int(stack);
                        s32 index = pop_int(stack);
                        Instance *jarr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(jarr, index, runtime);
                        if (!ret) {
                            *(((u16 *) jarr->arr_body) + index) = i;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("iastore: save array[%llx].(%d)=%d)\n",
                            (s64)(intptr_t)jarr, index, i);
#endif
                        }
                        opCode += 1;
                        break;
                    }

                    label_sastore:
                    case op_sastore: {
                        s32 i = pop_int(stack);
                        s32 index = pop_int(stack);
                        Instance *jarr = (Instance *) pop_ref(stack);
                        ret = jarray_check_exception(jarr, index, runtime);
                        if (!ret) {
                            *(((s16 *) jarr->arr_body) + index) = i;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        jvm_printf("iastore: save array[%llx].(%d)=%d)\n",
                            (s64)(intptr_t)jarr, index, i);
#endif
                        }
                        opCode += 1;
                        break;
                    }


                    label_pop:
                    case op_pop: {
                        pop_empty(stack);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("pop\n");
#endif
                        opCode += 1;

                        break;
                    }


                    label_pop2:
                    case op_pop2: {
                        pop_empty(stack);
                        pop_empty(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("pop2\n");
#endif
                        opCode += 1;

                        break;
                    }


                    label_dup:
                    case op_dup: {
                        StackEntry entry;
                        peek_entry(stack, &entry, stack->size - 1);

                        push_entry(stack, &entry);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dup\n");
#endif
                        opCode += 1;

                        break;
                    }

                    label_dup_x1:
                    case op_dup_x1: {
                        StackEntry entry1;
                        pop_entry(stack, &entry1);
                        StackEntry entry2;
                        pop_entry(stack, &entry2);

                        push_entry(stack, &entry1);
                        push_entry(stack, &entry2);
                        push_entry(stack, &entry1);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dup_x1\n");
#endif
                        opCode += 1;

                        break;
                    }

                    label_dup_x2:
                    case op_dup_x2: {
                        StackEntry entry1;
                        pop_entry(stack, &entry1);
                        StackEntry entry2;
                        pop_entry(stack, &entry2);
                        StackEntry entry3;
                        pop_entry(stack, &entry3);

                        push_entry(stack, &entry1);
                        push_entry(stack, &entry3);
                        push_entry(stack, &entry2);
                        push_entry(stack, &entry1);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dup_x2 \n");
#endif
                        opCode += 1;

                        break;
                    }

                    label_dup2:
                    case op_dup2: {
                        StackEntry entry1;
                        peek_entry(stack, &entry1, stack->size - 1);
                        StackEntry entry2;
                        peek_entry(stack, &entry2, stack->size - 2);

                        push_entry(stack, &entry2);
                        push_entry(stack, &entry1);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_dup2\n");
#endif
                        opCode += 1;

                        break;
                    }


                    label_dup2_x1:
                    case op_dup2_x1: {
                        StackEntry entry1;
                        pop_entry(stack, &entry1);
                        StackEntry entry2;
                        pop_entry(stack, &entry2);
                        StackEntry entry3;
                        pop_entry(stack, &entry3);

                        push_entry(stack, &entry2);
                        push_entry(stack, &entry1);
                        push_entry(stack, &entry3);
                        push_entry(stack, &entry2);
                        push_entry(stack, &entry1);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dup2_x1\n");
#endif
                        opCode += 1;

                        break;
                    }

                    label_dup2_x2:
                    case op_dup2_x2: {
                        StackEntry entry1;
                        pop_entry(stack, &entry1);
                        StackEntry entry2;
                        pop_entry(stack, &entry2);
                        StackEntry entry3;
                        pop_entry(stack, &entry3);
                        StackEntry entry4;
                        pop_entry(stack, &entry4);

                        push_entry(stack, &entry2);
                        push_entry(stack, &entry1);
                        push_entry(stack, &entry4);
                        push_entry(stack, &entry3);
                        push_entry(stack, &entry2);
                        push_entry(stack, &entry1);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dup2_x2\n");
#endif
                        opCode += 1;

                        break;
                    }

                    label_swap:
                    case op_swap: {

                        StackEntry entry1;
                        pop_entry(stack, &entry1);
                        StackEntry entry2;
                        pop_entry(stack, &entry2);

                        push_entry(stack, &entry1);
                        push_entry(stack, &entry2);


#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("swap\n");
#endif
                        opCode += 1;

                        break;
                    }


                    label_iadd:
                    case op_iadd: {

                        s32 value1 = pop_int(stack);
                        s32 value2 = pop_int(stack);
                        s32 result = value1 + value2;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("iadd: %d + %d = %d\n", value1, value2, result);
#endif
                        push_int(stack, result);
                        opCode += 1;

                        break;
                    }


                    label_ladd:
                    case op_ladd: {
                        s64 value1 = pop_long(stack);
                        s64 value2 = pop_long(stack);
                        s64 result = value2 + value1;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("ladd: %lld + %lld = %lld\n", value2, value1, result);
#endif
                        push_long(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_fadd:
                    case op_fadd: {
                        f32 value1 = pop_float(stack);
                        f32 value2 = pop_float(stack);
                        f32 result = value2 + value1;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("fadd: %lf + %lf = %lf\n", value2, value1, result);
#endif
                        push_float(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_dadd:
                    case op_dadd: {

                        f64 value1 = pop_double(stack);
                        f64 value2 = pop_double(stack);
                        f64 result = value1 + value2;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dadd: %lf + %lf = %lf\n", value1, value2, result);
#endif
                        push_double(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_isub:
                    case op_isub: {
                        s32 value2 = pop_int(stack);
                        s32 value1 = pop_int(stack);
                        s32 result = value1 - value2;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("isub : %d - %d = %d\n", value1, value2, result);
#endif
                        push_int(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_lsub:
                    case op_lsub: {
                        s64 value1 = pop_long(stack);
                        s64 value2 = pop_long(stack);
                        s64 result = value2 - value1;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("lsub: %lld - %lld = %lld\n", value2, value1, result);
#endif
                        push_long(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_fsub:
                    case op_fsub: {
                        f32 value1 = pop_float(stack);
                        f32 value2 = pop_float(stack);
                        f32 result = value2 - value1;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("fsub: %f - %f = %f\n", value2, value1, result);
#endif
                        push_float(stack, result);
                        opCode += 1;

                        break;
                    }


                    label_dsub:
                    case op_dsub: {
                        f64 value1 = pop_double(stack);
                        f64 value2 = pop_double(stack);
                        f64 result = value2 - value1;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dsub: %lf - %lf = %lf\n", value2, value1, result);
#endif
                        push_double(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_imul:
                    case op_imul: {

                        s32 value1 = pop_int(stack);
                        s32 value2 = pop_int(stack);
                        s32 result = value1 * value2;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("imul: %d * %d = %d\n", value1, value2, result);
#endif
                        push_int(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_lmul:
                    case op_lmul: {
                        s64 value1 = pop_long(stack);
                        s64 value2 = pop_long(stack);
                        s64 result = value2 * value1;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("lmul: %lld * %lld = %lld\n", value2, value1, result);
#endif
                        push_long(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_fmul:
                    case op_fmul: {
                        f32 value1 = pop_float(stack);
                        f32 value2 = pop_float(stack);
                        f32 result = value1 * value2;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("fmul: %f * %f = %f\n", value1, value2, result);
#endif
                        push_float(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_dmul:
                    case op_dmul: {
                        f64 value1 = pop_double(stack);
                        f64 value2 = pop_double(stack);
                        f64 result = value1 * value2;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dmul: %lf * %lf = %lf\n", value1, value2, result);
#endif
                        push_double(stack, result);
                        opCode += 1;

                        break;
                    }


                    label_idiv:
                    case op_idiv: {

                        s32 value2 = pop_int(stack);
                        s32 value1 = pop_int(stack);
                        s32 result = value1 / value2;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("idiv: %d / %d = %d\n", value1, value2, result);
#endif
                        push_int(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_ldiv:
                    case op_ldiv: {
                        s64 value1 = pop_long(stack);
                        s64 value2 = pop_long(stack);
                        s64 result = value2 / value1;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("ldiv: %lld / %lld = %lld\n", value2, value1, result);
#endif
                        push_long(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_fdiv:
                    case op_fdiv: {
                        f32 value1 = pop_float(stack);
                        f32 value2 = pop_float(stack);
                        f32 result = value2 / value1;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("fdiv: %f / %f = %f\n", value2, value1, result);
#endif
                        push_float(stack, result);
                        opCode += 1;

                        break;
                    }
                    label_ddiv:
                    case op_ddiv: {
                        f64 value1 = pop_double(stack);
                        f64 value2 = pop_double(stack);
                        f64 result = value2 / value1;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("ddiv: %f / %f = %f\n", value2, value1, result);
#endif
                        push_double(stack, result);
                        opCode += 1;

                        break;
                    }


                    label_irem:
                    case op_irem: {
                        s32 value1 = pop_int(stack);
                        s32 value2 = pop_int(stack);
                        s32 result = value2 % value1;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("irem: %d % %d = %d\n", value2, value1, result);
#endif
                        push_int(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_lrem:
                    case op_lrem: {
                        s64 value1 = pop_long(stack);
                        s64 value2 = pop_long(stack);
                        s64 result = value2 % value1;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("lrem: %lld mod %lld = %lld\n", value2, value1, result);
#endif
                        push_long(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_frem:
                    case op_frem: {
                        f32 value1 = pop_float(stack);
                        f32 value2 = pop_float(stack);
                        f32 result = value2 - ((int) (value2 / value1) * value1);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("frem: %f % %f = %f\n", value2, value1, result);
#endif
                        push_float(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_drem:
                    case op_drem: {
                        f64 value1 = pop_double(stack);
                        f64 value2 = pop_double(stack);
                        f64 result = value2 - ((s64) (value2 / value1) * value1);;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("drem: %lf mod %lf = %lf\n", value2, value1, result);
#endif
                        push_double(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_ineg:
                    case op_ineg: {
                        s32 value1 = pop_int(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("ineg: -(%d) = %d\n", value1, -value1);
#endif
                        push_int(stack, -value1);
                        opCode += 1;

                        break;
                    }

                    label_lneg:
                    case op_lneg: {
                        s64 value1 = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("lneg: -(%lld) = %lld\n", value1, -value1);
#endif
                        push_long(stack, -value1);
                        opCode += 1;

                        break;
                    }


                    label_fneg:
                    case op_fneg: {
                        f32 value1 = pop_float(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("fneg: -(%f) = %f\n", value1, -value1);
#endif
                        push_float(stack, -value1);
                        opCode += 1;

                        break;
                    }


                    label_dneg:
                    case op_dneg: {
                        f64 value1 = pop_double(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dneg: -(%lf) = %lf\n", value1, -value1);
#endif
                        push_double(stack, -value1);
                        opCode += 1;

                        break;
                    }

                    label_ishl:
                    case op_ishl: {
                        s32 value1 = pop_int(stack);
                        s32 value2 = pop_int(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("ishl: %x << %x =%x \n", value2, value1, value2 << value1);
#endif
                        push_int(stack, value2 << value1);
                        opCode += 1;

                        break;
                    }

                    label_lshl:
                    case op_lshl: {
                        s32 value1 = pop_int(stack);
                        s64 value2 = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("lshl: %llx << %x =%llx \n", value2, value1, (value2 << value1));
#endif
                        push_long(stack, value2 << value1);
                        opCode += 1;

                        break;
                    }

                    label_ishr:
                    case op_ishr: {
                        s32 value1 = pop_int(stack);
                        s32 value2 = pop_int(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("ishr: %x >> %x =%x \n", value2, value1, value2 >> value1);
#endif
                        push_int(stack, value2 >> value1);
                        opCode += 1;

                        break;
                    }

                    label_lshr:
                    case op_lshr: {
                        s32 value1 = pop_int(stack);
                        s64 value2 = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("lshr: %llx >> %x =%llx \n", value2, value1, value2 >> value1);
#endif
                        push_long(stack, value2 >> value1);
                        opCode += 1;

                        break;
                    }

                    label_iushr:
                    case op_iushr: {
                        s32 value1 = pop_int(stack);
                        u32 value2 = pop_int(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("iushr: %x >>> %x =%x \n", value2, value1, value2 >> value1);
#endif
                        push_int(stack, value2 >> value1);
                        opCode += 1;

                        break;
                    }

                    label_lushr:
                    case op_lushr: {
                        s32 value1 = pop_int(stack);
                        u64 value2 = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("lushr: %llx >>> %x =%llx \n", value2, value1, value2 >> value1);
#endif
                        push_long(stack, value2 >> value1);
                        opCode += 1;

                        break;
                    }

                    label_iand:
                    case op_iand: {
                        u32 value1 = pop_int(stack);
                        u32 value2 = pop_int(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("iand: %x & %x =%x \n", value2, value1, value2 & value1);
#endif
                        push_int(stack, value2 & value1);
                        opCode += 1;

                        break;
                    }

                    label_land:
                    case op_land: {
                        u64 value1 = pop_long(stack);
                        u64 value2 = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("land: %llx  &  %llx =%llx \n", value2, value1, value2 & value1);
#endif
                        push_long(stack, value2 & value1);
                        opCode += 1;

                        break;
                    }


                    label_ior:
                    case op_ior: {
                        u32 value1 = pop_int(stack);
                        u32 value2 = pop_int(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("ior: %x & %x =%x \n", value2, value1, value2 | value1);
#endif
                        push_int(stack, value2 | value1);
                        opCode += 1;

                        break;
                    }

                    label_lor:
                    case op_lor: {
                        u64 value1 = pop_long(stack);
                        u64 value2 = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("lor: %llx  |  %llx =%llx \n", value2, value1, value2 | value1);
#endif
                        push_long(stack, value2 | value1);
                        opCode += 1;

                        break;
                    }


                    label_ixor:
                    case op_ixor: {
                        u32 value1 = pop_int(stack);
                        u32 value2 = pop_int(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("ixor: %x ^ %x =%x \n", value2, value1, value2 ^ value1);
#endif
                        push_int(stack, value2 ^ value1);
                        opCode += 1;

                        break;
                    }

                    label_lxor:
                    case op_lxor: {
                        u64 value1 = pop_long(stack);
                        u64 value2 = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("lxor: %llx  ^  %llx =%llx \n", value2, value1, value2 ^ value1);
#endif
                        push_long(stack, value2 ^ value1);
                        opCode += 1;

                        break;
                    }

                    label_iinc:
                    case op_iinc: {
                        Short2Char s2c1, s2c2;

                        if (runtime->wideMode) {
                            s2c1.c1 = opCode[1];
                            s2c1.c0 = opCode[2];
                            s2c2.c1 = opCode[3];
                            s2c2.c0 = opCode[4];
                            opCode += 5;
                            runtime->wideMode = 0;
                        } else {
                            s2c1.s = (u8) opCode[1];
                            s2c2.s = (s8) opCode[2];
                            opCode += 3;
                        }

                        s32 oldv = localvar_getInt(localvar, (u16) s2c1.s);
                        localvar_setInt(localvar, (u16) s2c1.s, oldv + s2c2.s);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("iinc: localvar(%d) = %d + %d\n", s2c1.s, oldv, s2c2.s);
#endif

                        break;
                    }

                    label_i2l:
                    case op_i2l: {
                        s32 value = pop_int(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("i2l: %d --> %lld\n", (s32)value, (s64)value);
#endif
                        push_long(stack, (s64) value);
                        opCode += 1;

                        break;
                    }

                    label_i2f:
                    case op_i2f: {
                        s32 value = pop_int(stack);
                        f32 result = (f32) value;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("i2f: %d --> %f\n", (s32)value, result);
#endif
                        push_float(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_i2d:
                    case op_i2d: {
                        s32 value = pop_int(stack);
                        f64 result = value;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("i2d: %d --> %lf\n", (s32)value, result);
#endif
                        push_double(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_l2i:
                    case op_l2i: {
                        s64 value = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("l2i: %d <-- %lld\n", (s32)value, value);
#endif
                        push_int(stack, (s32) value);
                        opCode += 1;

                        break;
                    }

                    label_l2f:
                    case op_l2f: {
                        s64 value = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("l2f: %f <-- %lld\n", (f32)value, value);
#endif
                        push_float(stack, (f32) value);
                        opCode += 1;

                        break;
                    }

                    label_l2d:
                    case op_l2d: {
                        s64 value = pop_long(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("l2d: %lf <-- %lld\n", (f64)value, value);
#endif
                        push_double(stack, (f64) value);
                        opCode += 1;

                        break;
                    }


                    label_f2i:
                    case op_f2i: {
                        f32 value1 = pop_float(stack);
                        s32 result = (s32) value1;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("f2i: %d <-- %f\n", result, value1);
#endif
                        push_int(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_f2l:
                    case op_f2l: {
                        f32 value1 = pop_float(stack);
                        s64 result = (s64) value1;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("f2l: %lld <-- %f\n", result, value1);
#endif
                        push_long(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_f2d:
                    case op_f2d: {
                        f32 value1 = pop_float(stack);
                        f64 result = value1;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("f2d: %f <-- %f\n", result, value1);
#endif
                        push_double(stack, result);
                        opCode += 1;

                        break;
                    }


                    label_d2i:
                    case op_d2i: {
                        f64 value1 = pop_double(stack);
                        s32 result = (s32) value1;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("d2i: %d <-- %lf\n", result, value1);
#endif
                        push_int(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_d2l:
                    case op_d2l: {
                        f64 value1 = pop_double(stack);
                        s64 result = (s64) value1;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("d2l: %lld <-- %lf\n", result, value1);
#endif
                        push_long(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_d2f:
                    case op_d2f: {
                        f64 value1 = pop_double(stack);
                        f32 result = (f32) value1;
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("d2f: %f <-- %lf\n", result, value1);
#endif
                        push_float(stack, result);
                        opCode += 1;

                        break;
                    }

                    label_i2b:
                    case op_i2b: {
                        s32 value = pop_int(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("i2b: %d --> %d\n", (s8)value, value);
#endif
                        push_int(stack, (s8) value);
                        opCode += 1;

                        break;
                    }


                    label_i2c:
                    label_i2s:
                    case op_i2c:
                    case op_i2s: {
                        s32 value = pop_int(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("i2s(c): %d --> %d\n", (s16)value, value);
#endif
                        push_int(stack, (u16) value);
                        opCode += 1;

                        break;
                    }

                    label_lcmp:
                    case op_lcmp: {
                        s64 value1 = pop_long(stack);
                        s64 value2 = pop_long(stack);
                        s32 result = value2 == value1 ? 0 : (value2 > value1 ? 1 : -1);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("lcmp: %llx cmp %llx = %d\n", value2, value1, result);
#endif
                        push_int(stack, result);

                        opCode += 1;

                        break;
                    }


                    label_fcmpl:
                    case op_fcmpl: {
                        f32 value1 = pop_float(stack);
                        f32 value2 = pop_float(stack);
                        s32 result = value2 == value1 ? 0 : (value2 > value1 ? 1 : -1);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("fcmpl: %f < %f = %d\n", value2, value1, result);
#endif
                        push_int(stack, result);

                        opCode += 1;

                        break;
                    }

                    label_fcmpg:
                    case op_fcmpg: {
                        f32 value1 = pop_float(stack);
                        f32 value2 = pop_float(stack);
                        s32 result = value2 == value1 ? 0 : (value2 > value1 ? 1 : -1);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("fcmpg: %f > %f = %d\n", value2, value1, result);
#endif
                        push_int(stack, result);

                        opCode += 1;

                        break;
                    }

                    label_dcmpl:
                    case op_dcmpl: {
                        f64 value1 = pop_double(stack);
                        f64 value2 = pop_double(stack);
                        s32 result = value2 == value1 ? 0 : (value2 > value1 ? 1 : -1);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dcmpl: %lf < %lf = %d\n", value2, value1, result);
#endif
                        push_int(stack, result);

                        opCode += 1;

                        break;
                    }

                    label_dcmpg:
                    case op_dcmpg: {
                        f64 value1 = pop_double(stack);
                        f64 value2 = pop_double(stack);
                        s32 result = value2 == value1 ? 0 : (value2 > value1 ? 1 : -1);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("dcmpg: %lf > %lf = %d\n", value2, value1, result);
#endif
                        push_int(stack, result);

                        opCode += 1;

                        break;
                    }


                    label_ifeq:
                    case op_ifeq: {
                        s32 val = pop_int(stack);
                        if (val == 0) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_ifeq: %d != 0  then jump \n", val);
#endif


                        break;
                    }

                    label_ifne:
                    case op_ifne: {
                        s32 val = pop_int(stack);
                        if (val != 0) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_ifne: %d != 0  then jump\n", val);
#endif


                        break;
                    }


                    label_iflt:
                    case op_iflt: {
                        s32 val = pop_int(stack);
                        if (val < 0) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_iflt: %d < 0  then jump  \n", val);
#endif


                        break;
                    }

                    label_ifge:
                    case op_ifge: {
                        s32 val = pop_int(stack);
                        if (val >= 0) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_ifge: %d >= 0  then jump \n", val);
#endif


                        break;
                    }

                    label_ifgt:
                    case op_ifgt: {
                        s32 val = pop_int(stack);
                        if (val > 0) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_ifgt: %d > 0  then jump \n", val);
#endif


                        break;
                    }

                    label_ifle:
                    case op_ifle: {
                        s32 val = pop_int(stack);
                        if (val <= 0) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_ifle: %d <= 0  then jump \n", val);
#endif


                        break;
                    }

                    label_if_icmpeq:
                    case op_if_icmpeq: {
                        s32 v2 = pop_int(stack);
                        s32 v1 = pop_int(stack);
                        if (v1 == v2) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_if_icmpeq: %lld == %lld \n", (s64)(intptr_t)v1, (s64)(intptr_t)v2);
#endif

                        break;
                    }

                    label_if_icmpne:
                    case op_if_icmpne: {
                        s32 v2 = pop_int(stack);
                        s32 v1 = pop_int(stack);
                        if (v1 != v2) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_if_icmpne: %lld != %lld \n", (s64)(intptr_t)v1, (s64)(intptr_t)v2);
#endif

                        break;
                    }

                    label_if_icmplt:
                    case op_if_icmplt: {
                        s32 v2 = pop_int(stack);
                        s32 v1 = pop_int(stack);
                        if (v1 < v2) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_if_icmplt: %lld < %lld \n", (s64)(intptr_t)v1, (s64)(intptr_t)v2);
#endif

                        break;
                    }

                    label_if_icmpge:
                    case op_if_icmpge: {
                        s32 v2 = pop_int(stack);
                        s32 v1 = pop_int(stack);
                        if (v1 >= v2) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_if_icmpge: %lld >= %lld \n", (s64)(intptr_t)v1, (s64)(intptr_t)v2);
#endif

                        break;
                    }

                    label_if_icmpgt:
                    case op_if_icmpgt: {
                        s32 v2 = pop_int(stack);
                        s32 v1 = pop_int(stack);
                        if (v1 > v2) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_if_icmpgt: %lld > %lld \n", (s64)(intptr_t)v1, (s64)(intptr_t)v2);
#endif

                        break;
                    }


                    label_if_icmple:
                    case op_if_icmple: {
                        s32 v2 = pop_int(stack);
                        s32 v1 = pop_int(stack);
                        if (v1 <= v2) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_if_icmple: %lld <= %lld \n", (s64)(intptr_t)v1, (s64)(intptr_t)v2);
#endif

                        break;
                    }

                    label_if_acmpeq:
                    case op_if_acmpeq: {
                        __refer v2 = pop_ref(stack);
                        __refer v1 = pop_ref(stack);
                        if (v1 == v2) {
                            //如果相等，从操作数中取出要跳转的地址
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            //opCode + 偏移跳转
                            opCode += s2c.s;
                        } else {
                            //否则往下执行
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_if_acmpeq: %lld == %lld \n", (s64)(intptr_t)v1, (s64)(intptr_t)v2);
#endif

                        break;
                    }

                    label_if_acmpne:
                    case op_if_acmpne: {
                        __refer v2 = pop_ref(stack);
                        __refer v1 = pop_ref(stack);
                        if (v1 != v2) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_if_acmpne: %lld != %lld \n", (s64)(intptr_t)v1, (s64)(intptr_t)v2);
#endif

                        break;
                    }

                    label_goto:
                    case op_goto: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        s32 branchoffset = s2c.s;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("goto: %d\n", branchoffset);
#endif
                        opCode += branchoffset;


                        break;
                    }

                    label_jsr:
                    case op_jsr: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        s32 branchoffset = s2c.s;
                        push_ref(stack, (__refer) (opCode + 3));
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("jsr: %d\n", branchoffset);
#endif
                        opCode += branchoffset;


                        break;
                    }

                    label_ret:
                    case op_ret: {
                        Short2Char s2c;
                        if (runtime->wideMode) {
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            runtime->wideMode = 0;
                        } else {
                            s2c.s = (u8) opCode[1];
                        }

                        __refer addr = localvar_getRefer(localvar, (u16) s2c.s);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("ret: %x\n", (s64)(intptr_t)addr);
#endif
                        opCode = (u8 *) addr;


                        break;
                    }


                    label_tableswitch:
                    case op_tableswitch: {
                        s32 pos = 0;
                        pos = 4 - ((((u64) (intptr_t) opCode) - (u64) (intptr_t) (runtime->ca->code)) % 4);//4 byte对齐

                        Int2Float i2c;
                        i2c.c3 = opCode[pos++];
                        i2c.c2 = opCode[pos++];
                        i2c.c1 = opCode[pos++];
                        i2c.c0 = opCode[pos++];
                        s32 default_offset = i2c.i;
                        i2c.c3 = opCode[pos++];
                        i2c.c2 = opCode[pos++];
                        i2c.c1 = opCode[pos++];
                        i2c.c0 = opCode[pos++];
                        s32 low = i2c.i;
                        i2c.c3 = opCode[pos++];
                        i2c.c2 = opCode[pos++];
                        i2c.c1 = opCode[pos++];
                        i2c.c0 = opCode[pos++];
                        s32 high = i2c.i;

                        int val = pop_int(stack);// pop an int from the stack
                        int offset = 0;
                        if (val < low || val > high) {  // if its less than <low> or greater than <high>,
                            offset = default_offset;              // branch to default
                        } else {                        // otherwise
                            pos += (val - low) * 4;
                            i2c.c3 = opCode[pos++];
                            i2c.c2 = opCode[pos++];
                            i2c.c1 = opCode[pos++];
                            i2c.c0 = opCode[pos++];
                            offset = i2c.i;     // branch to entry in table
                        }

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("tableswitch: val=%d, offset=%d\n", val, offset);
#endif
                        opCode += offset;


                        break;
                    }

                    label_lookupswitch:
                    case op_lookupswitch: {
                        s32 pos = 0;
                        pos = 4 - ((((u64) (intptr_t) opCode) - (u64) (intptr_t) (runtime->ca->code)) % 4);//4 byte对齐
                        Int2Float i2c;
                        i2c.c3 = opCode[pos++];
                        i2c.c2 = opCode[pos++];
                        i2c.c1 = opCode[pos++];
                        i2c.c0 = opCode[pos++];
                        s32 default_offset = i2c.i;
                        i2c.c3 = opCode[pos++];
                        i2c.c2 = opCode[pos++];
                        i2c.c1 = opCode[pos++];
                        i2c.c0 = opCode[pos++];
                        s32 n = i2c.i;
                        s32 i, key;

                        int val = pop_int(stack);// pop an int from the stack
                        int offset = default_offset;
                        for (i = 0; i < n; i++) {
                            i2c.c3 = opCode[pos++];
                            i2c.c2 = opCode[pos++];
                            i2c.c1 = opCode[pos++];
                            i2c.c0 = opCode[pos++];
                            key = i2c.i;
                            if (key == val) {
                                i2c.c3 = opCode[pos++];
                                i2c.c2 = opCode[pos++];
                                i2c.c1 = opCode[pos++];
                                i2c.c0 = opCode[pos++];
                                offset = i2c.i;
                                break;
                            } else {
                                pos += 4;
                            }
                        }

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("tableswitch: val=%d, offset=%d\n", val, offset);
#endif
                        opCode += offset;

                        break;
                    }

                    label_ireturn:
                    label_lreturn:
                    label_freturn:
                    label_dreturn:
                    label_areturn:
                    case op_ireturn:
                    case op_lreturn:
                    case op_freturn:
                    case op_dreturn:
                    case op_areturn: {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5

                        StackEntry entry;
                    peek_entry(stack, &entry, stack->size - 1);
                    invoke_deepth(runtime);
                    jvm_printf("ilfda_return=[%x]/%d/[%llx]\n", entry_2_int(&entry), entry_2_int(&entry), entry_2_long(&entry));
#endif
                        opCode += 1;
                        ret = RUNTIME_STATUS_RETURN;
                        break;
                    }

                    label_return:
                    case op_return: {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("return: \n");
#endif
                        opCode += 1;
                        ret = RUNTIME_STATUS_RETURN;
                        break;
                    }

                    label_getstatic:
                    case op_getstatic: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        FieldInfo *fi = class_get_constant_fieldref(clazz, s2c.s)->fieldInfo;
                        if (!fi) {
                            ConstantFieldRef *cfr = class_get_constant_fieldref(clazz, s2c.s);
                            fi = find_fieldInfo_by_fieldref(clazz, cfr->item.index, runtime);
                            cfr->fieldInfo = fi;
                        }
                        c8 *ptr = getStaticFieldPtr(fi);

                        if (fi->isvolatile) {
                            barrier();
                        }
                        if (fi->isrefer) {
                            push_ref(stack, getFieldRefer(ptr));
                        } else {
                            // check variable type to determine s64/s32/f64/f32
                            s32 data_bytes = fi->datatype_bytes;
                            switch (data_bytes) {
                                case 4: {
                                    push_int(stack, getFieldInt(ptr));
                                    break;
                                }
                                case 1: {
                                    push_int(stack, getFieldByte(ptr));
                                    break;
                                }
                                case 8: {
                                    push_long(stack, getFieldLong(ptr));
                                    break;
                                }
                                case 2: {
                                    if (fi->datatype_idx == DATATYPE_JCHAR)push_int(stack, getFieldChar(ptr));
                                    else push_int(stack, getFieldShort(ptr));
                                    break;
                                }
                            }
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    StackEntry entry;
                    peek_entry(stack, &entry, stack->size - 1);
                    s64 v = entry_2_long(&entry);
                    jvm_printf("%s: push %s.%s[%llx]\n", "getstatic", utf8_cstr(clazz->name), utf8_cstr(fi->name), (s64)(intptr_t)ptr, v);
#endif
                        opCode += 3;
                        break;
                    }

                    label_putstatic:
                    case op_putstatic: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        FieldInfo *fi = class_get_constant_fieldref(clazz, s2c.s)->fieldInfo;
                        if (!fi) {
                            ConstantFieldRef *cfr = class_get_constant_fieldref(clazz, s2c.s);
                            fi = find_fieldInfo_by_fieldref(clazz, cfr->item.index, runtime);
                            cfr->fieldInfo = fi;
                        }

                        c8 *ptr = getStaticFieldPtr(fi);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        StackEntry entry;
                    peek_entry(stack, &entry, stack->size - 1);
                    invoke_deepth(runtime);
                    jvm_printf("%s  save:%s.%s[%llx]=[%llx]  \n", "putstatic", utf8_cstr(clazz->name), utf8_cstr(fi->name), (s64)(intptr_t)ptr, entry_2_long(&entry));
#endif

                        if (fi->isrefer) {//垃圾回收标识
                            setFieldRefer(ptr, pop_ref(stack));
                        } else {
                            // check variable type to determain long/s32/f64/f32
                            s32 data_bytes = fi->datatype_bytes;
                            //非引用类型
                            switch (data_bytes) {
                                case 4: {
                                    setFieldInt(ptr, pop_int(stack));
                                    break;
                                }
                                case 1: {
                                    setFieldByte(ptr, pop_int(stack));
                                    break;
                                }
                                case 8: {
                                    setFieldLong(ptr, pop_long(stack));
                                    break;
                                }
                                case 2: {
                                    setFieldShort(ptr, pop_int(stack));
                                    break;
                                }
                            }
                        }

                        opCode += 3;
                        break;
                    }

                    label_getfield:
                    case op_getfield: {

                        //从 Code 中获取 Field 的 Index
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        //Field 所在的对象
                        Instance *ins = (Instance *) pop_ref(stack);
                        if (!ins) {
                            //如果对象为空，则抛出空指针异常
                            Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        } else {
                            //先从前面加载的缓存中获取目标 Field 的信息
                            FieldInfo *fi = class_get_constant_fieldref(clazz, s2c.s)->fieldInfo;
                            if (!fi) {
                                //如果是空，那么该段应该没有加载过，先获取引用常量，然后通过引用常量找到真正的 Field
                                ConstantFieldRef *cfr = class_get_constant_fieldref(clazz, s2c.s);
                                fi = find_fieldInfo_by_fieldref(clazz, cfr->item.index, runtime);
                                cfr->fieldInfo = fi;
                            }

                            //从目标对象中获取 Field 值的指针
                            c8 *ptr = getInstanceFieldPtr(ins, fi);

                            //如果该 Field 是原子的
                            if (fi->isvolatile) {
                                //那么设置内存屏障，强制从内存中读取
                                barrier();
                            }
                            if (fi->isrefer) {
                                //如果是引用类型
                                push_ref(stack, getFieldRefer(ptr));
                            } else {
                                // check variable type to determine s64/s32/f64/f32
                                s32 data_bytes = fi->datatype_bytes;
                                //基本类型，只要关注大小
                                switch (data_bytes) {
                                    case 4: {
                                        push_int(stack, getFieldInt(ptr));
                                        break;
                                    }
                                    case 1: {
                                        push_int(stack, getFieldByte(ptr));
                                        break;
                                    }
                                    case 8: {
                                        push_long(stack, getFieldLong(ptr));
                                        break;
                                    }
                                    case 2: {
                                        if (fi->datatype_idx == DATATYPE_JCHAR)push_int(stack, getFieldChar(ptr));
                                        else push_int(stack, getFieldShort(ptr));
                                        break;
                                    }
                                }
                            }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            invoke_deepth(runtime);
                        StackEntry entry;
                        peek_entry(stack, &entry, stack->size - 1);
                        s64 v = entry_2_long(&entry);
                        jvm_printf("%s: push %s.%s[%llx]\n", "getfield", utf8_cstr(clazz->name), utf8_cstr(fi->name), (s64)(intptr_t)ptr, v);
#endif
                        }
                        opCode += 3;
                        break;
                    }


                    label_putfield:
                    case op_putfield: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        StackEntry entry;
                        pop_entry(stack, &entry);
                        if (entry.type & (STACK_ENTRY_LONG | STACK_ENTRY_DOUBLE)) {
                            pop_entry(stack, &entry);
                        }

                        Instance *ins = (Instance *) pop_ref(stack);
                        if (!ins) {
                            Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        } else {
                            // check variable type to determain long/s32/f64/f32
                            FieldInfo *fi = class_get_constant_fieldref(clazz, s2c.s)->fieldInfo;
                            if (!fi) {
                                ConstantFieldRef *cfr = class_get_constant_fieldref(clazz, s2c.s);
                                fi = find_fieldInfo_by_fieldref(clazz, cfr->item.index, runtime);
                                cfr->fieldInfo = fi;
                            }
                            c8 *ptr = getInstanceFieldPtr(ins, fi);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                            if (utf8_equals_c(fi->name, "count") && utf8_equals_c(fi->_this_class->name, "java/lang/StringBuilder")) {
                            int debug = 1;
                        }
                        invoke_deepth(runtime);
                        jvm_printf("%s  save:%s.%s[%llx]=[%llx]  \n", "putfield", utf8_cstr(clazz->name), utf8_cstr(fi->name), (s64)(intptr_t)ptr, entry_2_long(&entry));
#endif

                            if (fi->isrefer) {//垃圾回收标识
                                setFieldRefer(ptr, entry_2_refer(&entry));
                            } else {
                                s32 data_bytes = fi->datatype_bytes;
                                //非引用类型
                                switch (data_bytes) {
                                    case 4: {
                                        setFieldInt(ptr, entry_2_int(&entry));
                                        break;
                                    }
                                    case 1: {
                                        setFieldByte(ptr, entry_2_int(&entry));
                                        break;
                                    }
                                    case 8: {
                                        setFieldLong(ptr, entry_2_long(&entry));
                                        break;
                                    }
                                    case 2: {
                                        setFieldShort(ptr, entry_2_int(&entry));
                                        break;
                                    }
                                }
                            }
                        }
                        opCode += 3;
                        break;
                    }

                    label_invokevirtual:
                    case op_invokevirtual: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        //此cmr所描述的方法，对于不同的实例，有不同的method
                        ConstantMethodRef *cmr = class_get_constant_method_ref(clazz, s2c.s);
                        //取得目标实例
                        Instance *ins = getInstanceInStack(clazz, cmr, stack);
                        if (ins == NULL) {
                            Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        } else {
                            MethodInfo *m = NULL;

                            if (ins->mb.type & (MEM_TYPE_CLASS)) {
                                //如果实例是个类，那么就是调用类的静态方法
                                m = cmr->methodInfo;
                            } else {
                                //先从缓存中查找，key 为方法引用和目标实现类型    
                                m = (MethodInfo *) pairlist_get(cmr->virtual_methods, ins->mb.clazz);
                                if (m == NULL) {
                                    //无命中，则开始遍历父类搜索
                                    m = find_instance_methodInfo_by_name(ins, cmr->name, cmr->descriptor, runtime);
                                    pairlist_put(cmr->virtual_methods, ins->mb.clazz, m);//放入缓存，以便下次直接调用
                                }
                            }


#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                            if (utf8_equals_c(cmr->clsName, "java/io/FileInputStream")
                            && utf8_equals_c(cmr->name, "open")
                            //                                && utf8_equals_c(cmr->descriptor, "(Ljava/lang/String;)Ljava/lang/StringBuilder;")
                            ) {
                            int debug = 1;
                        }
                        invoke_deepth(runtime);
                        jvm_printf("invokevirtual    %s.%s%s  {\n", utf8_cstr(m->_this_class->name), utf8_cstr(m->name), utf8_cstr(m->descriptor));
#endif

                            if (m) {
                                //执行匹配到的实现方法
                                ret = execute_method_impl(m, runtime, m->_this_class);
                            } else {
                                //没找到合适的方法，则抛出 NoSuchMethodException
                                Instance *exception = exception_create_str(JVM_EXCEPTION_NOSUCHMETHOD, runtime,
                                                                           utf8_cstr(cmr->name));
                                push_ref(stack, (__refer) exception);
                                ret = RUNTIME_STATUS_EXCEPTION;
                            }
#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                            invoke_deepth(runtime);
                        jvm_printf("}\n");
#endif

                        }
                        opCode += 3;
                        break;
                    }


                    label_invokespecial:
                    case op_invokespecial: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];
                        u16 object_ref = s2c.s;

                        ConstantMethodRef *cmr = class_get_constant_method_ref(clazz, object_ref);
                        MethodInfo *m = cmr->methodInfo;

#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                        if (utf8_equals_c(cmr->clsName, "org/mini/fs/InnerFile")
                        && utf8_equals_c(cmr->name, "<init>")
                        //                                && utf8_equals_c(cmr->descriptor, "(Ljava/lang/String;)Ljava/lang/StringBuilder;")
                        ) {
                        int debug = 1;
                    }
                    invoke_deepth(runtime);
                    jvm_printf("invokespecial    %s.%s%s {\n", utf8_cstr(m->_this_class->name), utf8_cstr(m->name), utf8_cstr(m->descriptor));
#endif
                        if (m) {
                            ret = execute_method_impl(m, runtime, m->_this_class);
                        } else {
                            Instance *exception = exception_create_str(JVM_EXCEPTION_NOSUCHMETHOD, runtime, utf8_cstr(cmr->name));
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                        invoke_deepth(runtime);
                    jvm_printf("}\n");
#endif

                        opCode += 3;
                        break;
                    }


                    label_invokestatic:
                    case op_invokestatic: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];
                        u16 object_ref = s2c.s;
                        ConstantMethodRef *cmr = class_get_constant_method_ref(clazz, object_ref);

                        MethodInfo *m = cmr->methodInfo;
#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                        if (utf8_equals_c(cmr->name, "readbuf") && utf8_equals_c(cmr->clsName, "org/mini/fs/InnerFile")) {
                        int debug = 1;
                    }

                    invoke_deepth(runtime);
                    jvm_printf("invokestatic   | %s.%s%s {\n", utf8_cstr(m->_this_class->name), utf8_cstr(m->name), utf8_cstr(m->descriptor));
#endif
                        if (m) {
                            ret = execute_method_impl(m, runtime, m->_this_class);
                        } else {
                            Instance *exception = exception_create_str(JVM_EXCEPTION_NOSUCHMETHOD, runtime, utf8_cstr(cmr->name));
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                        invoke_deepth(runtime);
                    jvm_printf("}\n");
#endif

                        opCode += 3;
                        break;
                    }


                    label_invokeinterface:
                    case op_invokeinterface: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];
                        u16 object_ref = s2c.s;

                        s32 paraCount = (u8) opCode[3];

                        ConstantMethodRef *cmr = class_get_constant_method_ref(clazz, object_ref);
                        Instance *ins = getInstanceInStack(clazz, cmr, stack);
                        if (ins == NULL) {
                            Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        } else {
                            //                            if (utf8_equals_c(cmr->name, "forEach") && utf8_equals_c(cmr->clsName, "java/util/List")) {
                            //                                int debug = 1;
                            //                            }
                            //                            if (utf8_equals_c(cmr->name, "hasNext") && utf8_equals_c(cmr->clsName, "java/util/Iterator")) {
                            //                                int debug = 1;
                            //                            }
                            MethodInfo *m = NULL;
                            if (ins->mb.type & MEM_TYPE_CLASS) {
                                m = cmr->methodInfo;
                            } else {
                                m = (MethodInfo *) pairlist_get(cmr->virtual_methods, ins->mb.clazz);
                                if (m == NULL) {
                                    m = find_instance_methodInfo_by_name(ins, cmr->name, cmr->descriptor, runtime);
                                    pairlist_put(cmr->virtual_methods, ins->mb.clazz, m);//放入缓存，以便下次直接调用
                                }
                            }

#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                            invoke_deepth(runtime);
                        jvm_printf("invokeinterface   | %s.%s%s {\n", utf8_cstr(m->_this_class->name),
                            utf8_cstr(m->name), utf8_cstr(m->descriptor));
#endif
                            if (m) {
                                ret = execute_method_impl(m, runtime, m->_this_class);
                            } else {
                                Instance *exception = exception_create_str(JVM_EXCEPTION_NOSUCHMETHOD, runtime, utf8_cstr(cmr->name));
                                push_ref(stack, (__refer) exception);
                                ret = RUNTIME_STATUS_EXCEPTION;
                            }
#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                            invoke_deepth(runtime);
                        jvm_printf("}\n");
#endif

                        }
                        opCode += 5;
                        break;
                    }

                    label_invokedynamic:
                    case op_invokedynamic: {

                        //index
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];
                        u16 id_index = s2c.s;

                        //get bootMethod struct

                        //根据 index 得到 ConstantInvokeDynamic 常量
                        ConstantInvokeDynamic *cid = class_get_invoke_dynamic(clazz, id_index);

                        //bootstrap_method_attr_index -> BootstrapMethod
                        BootstrapMethod *bootMethod = &clazz->bootstrapMethodAttr->bootstrap_methods[cid->bootstrap_method_attr_index];//Boot

                        if (bootMethod->make == NULL) {
                            /**
                            * run bootstrap method java.lang.invoke.LambdaMetafactory
                            *
                            * public static CallSite metafactory(MethodHandles.Lookup caller,
                            *           String invokedName,
                            *           MethodType invokedType,
                            *           MethodType samMethodType,
                            *           MethodHandle implMethod,
                            *           MethodType instantiatedMethodType)
                            *
                            *
                            *  to generate Lambda Class implementation specify interface
                            *  and new a callsite
                            */
                            
                            //准备调用 metafactory() 方法的前3个参数，lookup，invokeName，invokeMethodType
                            //parper bootMethod parameter
                            Instance *lookup = method_handles_lookup_create(runtime, clazz);
                            push_ref(stack, lookup); //lookup

                            Utf8String *ustr_invokeName = class_get_constant_utf8(clazz, class_get_constant_name_and_type(clazz, cid->nameAndTypeIndex)->nameIndex)->utfstr;
                            Instance *jstr_invokeName = jstring_create(ustr_invokeName, runtime);
                            push_ref(stack, jstr_invokeName); //invokeName

                            Utf8String *ustr_invokeType = class_get_constant_utf8(clazz, class_get_constant_name_and_type(clazz, cid->nameAndTypeIndex)->typeIndex)->utfstr;
                            Instance *mt_invokeType = method_type_create(runtime, ustr_invokeType);
                            push_ref(stack, mt_invokeType); //invokeMethodType

                            //other bootMethod parameter
                            //根据 BootstrapMethod.num_bootstrap_arguments 遍历取出各个类型的参数
                            s32 i;
                            for (i = 0; i < bootMethod->num_bootstrap_arguments; i++) {
                                ConstantItem *item = class_get_constant_item(clazz, bootMethod->bootstrap_arguments[i]);
                                switch (item->tag) {
                                    case CONSTANT_METHOD_TYPE: {
                                        ConstantMethodType *cmt = (ConstantMethodType *) item;
                                        Utf8String *arg = class_get_constant_utf8(clazz, cmt->descriptor_index)->utfstr;
                                        Instance *mt = method_type_create(runtime, arg);
                                        push_ref(stack, mt);
                                        break;
                                    }
                                    case CONSTANT_STRING_REF: {
                                        ConstantStringRef *csr = (ConstantStringRef *) item;
                                        Utf8String *arg = class_get_constant_utf8(clazz, csr->stringIndex)->utfstr;
                                        Instance *spec = jstring_create(arg, runtime);
                                        push_ref(stack, spec);
                                        break;
                                    }
                                    case CONSTANT_METHOD_HANDLE: {
                                        ConstantMethodHandle *cmh = (ConstantMethodHandle *) item;
                                        MethodInfo *mip = find_methodInfo_by_methodref(clazz, cmh->reference_index, runtime);
                                        Instance *mh = method_handle_create(runtime, mip, cmh->reference_kind);
                                        push_ref(stack, mh);
                                        break;
                                    }
                                    default: {
                                        jvm_printf("invokedynamic para parse error.");
                                    }
                                }

                            }

                            //get bootmethod
                            //s32 reference_kind = class_get_method_handle(clazz, bootMethod->bootstrap_method_ref)->reference_kind;
                            //bootstrap_method_ref -> ConstantMethodHandle -> metafactory() 的 MethodInfo
                            MethodInfo *boot_m = find_methodInfo_by_methodref(clazz, class_get_method_handle(clazz, bootMethod->bootstrap_method_ref)->reference_index, runtime);

                            if (boot_m) {
                                //执行 metafactory() 得到 CallSite
                                ret = execute_method_impl(boot_m, runtime, boot_m->_this_class);
                                if (ret == RUNTIME_STATUS_NORMAL) {
                                    //调用虚拟机内部方法 org/mini/reflect/vm/LambdaUtil.getMethodInfoHandle(CallSite) 得到 finder 方法的地址
                                    MethodInfo *finder = find_methodInfo_by_name_c("org/mini/reflect/vm/LambdaUtil", "getMethodInfoHandle", "(Ljava/lang/invoke/CallSite;)J", runtime);
                                    if (finder) {
                                        //调用 finder 方法将 calsite.target(MethodHandle) 转换成 MethodInfo * pointer，这才是真正要调用方法的指针，并且存到 bootMethod->make 缓存中
                                        //run finder to convert calsite.target(MethodHandle) to MethodInfo * pointer
                                        ret = execute_method_impl(finder, runtime, finder->_this_class);
                                        if (ret == RUNTIME_STATUS_NORMAL) {
                                            MethodInfo *make = (MethodInfo *) (intptr_t) pop_long(stack);
                                            bootMethod->make = make;
                                        }
                                    } else {
                                        Instance *exception = exception_create(JVM_EXCEPTION_NOSUCHMETHOD, runtime);
                                        push_ref(stack, (__refer) exception);
                                        ret = RUNTIME_STATUS_EXCEPTION;
                                    }
                                }
                            } else {
                                Instance *exception = exception_create(JVM_EXCEPTION_NOSUCHMETHOD, runtime);
                                push_ref(stack, (__refer) exception);
                                ret = RUNTIME_STATUS_EXCEPTION;
                            }
                        }
                        MethodInfo *m = bootMethod->make;
#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                        invoke_deepth(runtime);
                    jvm_printf("invokedynamic   | %s.%s%s {\n", utf8_cstr(m->_this_class->name),
                        utf8_cstr(m->name), utf8_cstr(m->descriptor));
#endif

                        if (ret == RUNTIME_STATUS_NORMAL) {
                            if (m) {
                                // run make to generate instance of Lambda Class
                                //真正执行 lambda 表达式所指向的方法
                                ret = execute_method_impl(m, runtime, m->_this_class);
                            } else {
                                Instance *exception = exception_create(JVM_EXCEPTION_NOSUCHMETHOD, runtime);
                                push_ref(stack, (__refer) exception);
                                ret = RUNTIME_STATUS_EXCEPTION;
                            }
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                        invoke_deepth(runtime);
                    jvm_printf("}\n");
#endif

                        opCode += 5;
                        break;
                    }


                    label_new:
                    case op_new: {

                        //Class 引用 index
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        u16 object_ref = s2c.s;

                        //Class 引用常量
                        ConstantClassRef *ccf = class_get_constant_classref(clazz, object_ref);
                        //获取类并加载初始化
                        if (!ccf->clazz) {
                            Utf8String *clsName = class_get_utf8_string(clazz, ccf->stringIndex);
                            ccf->clazz = classes_load_get(clsName, runtime);
                        }
                        JClass *other = ccf->clazz;
                        //创建实例
                        Instance *ins = NULL;
                        if (other) {
                            ins = instance_create(runtime, other);
                        }
                        push_ref(stack, (__refer) ins);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("new %s [%llx]\n", utf8_cstr(ccf->name), (s64)(intptr_t)ins);
#endif
                        opCode += 3;

                        break;
                    }


                    label_newarray:
                    case op_newarray: {
                        s32 typeIdx = opCode[1];

                        s32 count = pop_int(stack);
                        opCode += 2;
                        Instance *arr = jarray_create_by_type_index(runtime, count, typeIdx);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("(a)newarray  [%llx] type:%c , count:%d  \n", (s64)(intptr_t)arr, getDataTypeTag(typeIdx), count);
#endif
                        if (arr) {
                            push_ref(stack, (__refer) arr);
                        } else {
                            Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        }

                        break;
                    }

                    label_anewarray:
                    case op_anewarray: {
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        s32 count = pop_int(stack);
                        opCode += 3;
                        JClass *arr_class = pairlist_get(clazz->arr_class_type, (__refer) (intptr_t) s2c.s);

                        Instance *arr = NULL;
                        if (!arr_class) {//cache to speed
                            arr_class = array_class_get_by_name(runtime, class_get_utf8_string(clazz, s2c.s));
                            pairlist_put(clazz->arr_class_type, (__refer) (intptr_t) s2c.s, arr_class);
                        }
                        arr = jarray_create_by_class(runtime, count, arr_class);


#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("(a)newarray  [%llx] type:%c , count:%d  \n", (s64)(intptr_t)arr, getDataTypeTag(arr_class->arr_type_index), count);
#endif
                        if (arr) {
                            push_ref(stack, (__refer) arr);
                        } else {
                            Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        }

                        break;
                    }

                    label_arraylength:
                    case op_arraylength: {
                        Instance *arr_ref = (Instance *) pop_ref(stack);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("arraylength  [%llx].arr_body[%llx] len:%d  \n",
                        (s64)(intptr_t)arr_ref, (s64)(intptr_t)arr_ref->arr_body, arr_ref->arr_length);
#endif
                        if (arr_ref == NULL) {
                            Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        } else {
                            push_int(stack, arr_ref->arr_length);
                            opCode += 1;
                        }
                        break;
                    }


                    label_athrow:
                    case op_athrow: {
                        Instance *ins = (Instance *) pop_ref(stack);
                        push_ref(stack, (__refer) ins);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("athrow  [%llx].exception throws  \n", (s64)(intptr_t)ins);
#endif
                        //opCode +=  1;
                        ret = RUNTIME_STATUS_EXCEPTION;
                        break;
                    }

                    label_checkcast:
                    case op_checkcast: {
                        Instance *ins = (Instance *) pop_ref(stack);
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        s32 typeIdx = s2c.s;
                        s32 checkok = 0;
                        if (ins != NULL) {
                            if (ins->mb.type == MEM_TYPE_INS) {
                                JClass *cl = getClassByConstantClassRef(clazz, typeIdx, runtime);
                                //                                if (utf8_equals_c(ins->mb.clazz->name, "java/lang/String")&&utf8_equals_c(cl->name,"java/lang/Comparable")) {//
                                //                                    int debug = 1;
                                //                                }
                                if (instance_of(cl, ins, runtime)) {
                                    checkok = 1;
                                }
                            } else if (ins->mb.type == MEM_TYPE_ARR) {
                                Utf8String *utf = class_get_constant_classref(clazz, typeIdx)->name;
                                u8 ch = utf8_char_at(utf, 1);
                                if (getDataTypeIndex(ch) == ins->mb.clazz->mb.arr_type_index) {
                                    checkok = 1;
                                }
                            } else if (ins->mb.type == MEM_TYPE_CLASS) {
                                Utf8String *utf = class_get_constant_classref(clazz, typeIdx)->name;
                                if (utf8_equals_c(utf, STR_CLASS_JAVA_LANG_CLASS)) {
                                    checkok = 1;
                                }
                            }
                        } else {
                            checkok = 1;
                        }
                        if (!checkok) {
                            Instance *exception = exception_create(JVM_EXCEPTION_CLASSCAST, runtime);
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        } else {
                            push_ref(stack, (__refer) ins);
                            ret = RUNTIME_STATUS_NORMAL;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("checkcast  [%llx] instancof %s is:%d \n", (s64)(intptr_t)ins,
                        utf8_cstr(class_get_constant_classref(clazz, typeIdx)->name),
                        checkok);
#endif
                        opCode += 3;
                        break;
                    }


                    label_instanceof:
                    case op_instanceof: {
                        Instance *ins = (Instance *) pop_ref(stack);
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        s32 typeIdx = s2c.s;

                        s32 checkok = 0;
                        if (ins == NULL) {
                        } else if (ins->mb.type & (MEM_TYPE_INS | MEM_TYPE_ARR)) {
                            if (instance_of(getClassByConstantClassRef(clazz, typeIdx, runtime), ins, runtime)) {
                                checkok = 1;
                            }
                        }
                        //                        else {
                        //                            if (utf8_equals(ins->mb.clazz->name, getClassByConstantClassRef(clazz, typeIdx)->name)) {//
                        //                                checkok = 1;
                        //                            }
                        //                        }
                        push_int(stack, checkok);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("instanceof  [%llx] instancof %s  \n", (s64)(intptr_t)ins, utf8_cstr(class_get_constant_classref(clazz, typeIdx)->name));
#endif
                        opCode += 3;

                        break;
                    }

                    label_monitorenter:
                    case op_monitorenter: {
                        Instance *ins = (Instance *) pop_ref(stack);
                        jthread_lock(&ins->mb, runtime);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("monitorenter  [%llx] %s  \n", (s64)(intptr_t)ins, ins ? utf8_cstr(ins->mb.clazz->name) : "null");
#endif
                        opCode += 1;

                        break;
                    }

                    label_monitorexit:
                    case op_monitorexit: {
                        Instance *ins = (Instance *) pop_ref(stack);
                        jthread_unlock(&ins->mb, runtime);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("monitorexit  [%llx] %s  \n", (s64)(intptr_t)ins, ins ? utf8_cstr(ins->mb.clazz->name) : "null");
#endif
                        opCode += 1;

                        break;
                    }

                    case op_wide: {
                        label_wide:
#if _JVM_DEBUG_BYTECODE_DETAIL > 5

                        invoke_deepth(runtime);
                    jvm_printf("wide  \n");
#endif
                        runtime->wideMode = 1;
                        opCode += 1;

                        break;
                    }

                    label_multianewarray:
                    case op_multianewarray: {
                        //data type index
                        Short2Char s2c;
                        s2c.c1 = opCode[1];
                        s2c.c0 = opCode[2];

                        Utf8String *desc = class_get_utf8_string(clazz, s2c.s);
                        //array dim
                        s32 count = (u8) opCode[3];
#ifdef __JVM_OS_VS__
                        s32 dim[32];
#else
                        s32 dim[count];
#endif
                        int i;
                        for (i = 0; i < count; i++)
                            dim[i] = pop_int(stack);

                        Instance *arr = jarray_multi_create(runtime, dim, count, desc, 0);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("multianewarray  [%llx] type:%s , count:%d  \n", (s64)(intptr_t)arr,
                        utf8_cstr(desc), count);
#endif
                        if (arr) {
                            push_ref(stack, (__refer) arr);

                        } else {
                            Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
                            push_ref(stack, (__refer) exception);
                            ret = RUNTIME_STATUS_EXCEPTION;
                        }
                        opCode += 4;
                        break;
                    }


                    label_ifnull:
                    case op_ifnull: {
                        __refer ref = pop_ref(stack);
                        if (!ref) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_ifnonnull: %d/%llx != 0  then jump %d \n", (s32)(intptr_t)ref,
                        (s64)(intptr_t)ref);
#endif


                        break;
                    }

                    label_ifnonnull:
                    case op_ifnonnull: {
                        __refer ref = pop_ref(stack);
                        if (ref) {
                            Short2Char s2c;
                            s2c.c1 = opCode[1];
                            s2c.c0 = opCode[2];
                            opCode += s2c.s;
                        } else {
                            opCode += 3;
                        }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                        invoke_deepth(runtime);
                    jvm_printf("op_ifnonnull: %d/%llx != 0  then \n", (s32)(intptr_t)ref, (s64)(intptr_t)ref);
#endif

                        break;
                    }

                    label_breakpoint:
                    case op_breakpoint: {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5

                        invoke_deepth(runtime);
                    jvm_printf("breakpoint \n");
#endif
                        //opCode +=  1;

                        break;
                    }


                    label_0xc8:
                    label_0xc9:
                    default:
                        jvm_printf("instruct %x not found\n", cur_inst);
                }

                /* ================================== opcode end =============================*/


#if _JVM_DEBUG_PROFILE
                //time
                s64 spent = nanoTime() - start_at;
                profile_put(instruct_code, spent, 1);
#endif
                if (ret == RUNTIME_STATUS_RETURN) {
                    ret = RUNTIME_STATUS_NORMAL;
                    break;
                } else if (ret == RUNTIME_STATUS_INTERRUPT) {
                    break;
                } else if (ret == RUNTIME_STATUS_EXCEPTION) {
                    Instance *ins = pop_ref(stack);
                    //jvm_printf("stack size:%d , enter size:%d\n", stack->size, stackSize);
                    //restore stack enter method size, must pop for garbage
                    while (stack->size > stackSize)pop_empty(stack);
                    push_ref(stack, ins);

                    //                    if (utf8_equals_c(ins->mb.clazz->name, "espresso/util/NotConstant")) {
                    //                        int debug = 1;
                    //                    }

#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                    s32 lineNum = getLineNumByIndex(ca, runtime->pc - ca->code);
                    printf("   at %s.%s(%s.java:%d)\n",
                        utf8_cstr(clazz->name), utf8_cstr(method->name),
                        utf8_cstr(clazz->name),
                        lineNum
                    );
#endif
                    ExceptionTable *et = _find_exception_handler(runtime, ins, ca, (s32) (opCode - ca->code), ins);
                    if (et == NULL) {
                        break;
                    } else {
#if _JVM_DEBUG_BYTECODE_DETAIL > 3
                        jvm_printf("Exception : %s\n", utf8_cstr(ins->mb.clazz->name));
#endif
                        opCode = (ca->code + et->handler_pc);
                        ret = RUNTIME_STATUS_NORMAL;
                    }
                }
            } while (1);
            if (method_sync)_synchronized_unlock_method(method, runtime);
            localvar_dispose(runtime);
#if _JVM_DEBUG_BYTECODE_DETAIL > 3

            Utf8String *ustr = method->descriptor;
            invoke_deepth(runtime);
            jvm_printf("stack size  %s.%s%s in:%d out:%d  \n", utf8_cstr(clazz->name), utf8_cstr(method->name), utf8_cstr(method->descriptor), stackSize, stack->size);
            if (ret != RUNTIME_STATUS_EXCEPTION) {
                if (utf8_indexof_c(ustr, ")V") >= 0) {//无反回值
                    if (stack->size != stackSize) {
                        exit(1);
                    }
                }
                else if (utf8_indexof_c(ustr, ")J") >= 0) {
                    if (stack->size != stackSize + 2) {
                        exit(1);
                    }
                }
                else {
                    if (stack->size != stackSize + 1) {
                        exit(1);
                    }
                }
            }
#endif
        } else {
            jvm_printf("method code attribute is null.");
        }
    } else {//本地方法
        localvar_init(runtime, method->para_slots);//可能有非静态本地方法调用，因此+1
        _stack2localvar(method, runtime->localvar, stack);
        //缓存调用本地方法
        if (!method->native_func) { //把本地方法找出来缓存
            java_native_method *native = find_native_method(utf8_cstr(clazz->name), utf8_cstr(method->name),
                                                            utf8_cstr(method->descriptor));
            if (!native) {
                Instance *exception = exception_create_str(JVM_EXCEPTION_NOSUCHMETHOD, runtime,
                                                           utf8_cstr(method->name));
                push_ref(stack, (__refer) exception);
                ret = RUNTIME_STATUS_EXCEPTION;
            } else {
                method->native_func = native->func_pointer;
            }
        }

        if (method->native_func) {
            if (method_sync)_synchronized_lock_method(method, runtime);
            ret = method->native_func(runtime, clazz);
            if (method_sync)_synchronized_unlock_method(method, runtime);
        }
        //        if (utf8_equals_c(method->name, "nvgTextGlyphPositionsJni")) {
        //            int debug = 1;
        //        }
        localvar_dispose(runtime);

    }
    runtime_destory_inl(runtime);
    pruntime->son = NULL;  //need for getLastSon()
    return ret;
}
