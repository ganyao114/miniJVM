
#ifndef G_JVM_H
#define G_JVM_H



//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../utils/tinycthread.h"

#include "../utils/d_type.h"
#include "../utils/hashtable.h"
#include "../utils/utf8_string.h"
#include "../utils/arraylist.h"
#include "../utils/pairlist.h"
#include "../utils/bytebuf.h"
#include "../utils/hashset.h"

#ifdef __cplusplus
extern "C" {
#endif


//=======================  micro define  =============================
//_JVM_DEBUG   06 print all bytecode
#define _JVM_DEBUG_BYTECODE_DETAIL 0
#define _JVM_DEBUG_PRINT_FILE 0
#define _JVM_DEBUG_GARBAGE_DUMP 0
#define _JVM_DEBUG_PROFILE 0

#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__
#define barrier() MemoryBarrier()
#else
#define barrier() __asm__ __volatile__("": : :"memory")
#endif


/*
 *  TAG
 *  1 UTF-8 String
 *  3 Integer
 *  4 Float
 *  5 Long
 *  6 Double
 *  7 Class reference
 *  8 String reference
 *  9 Field reference
 *  10 Method reference
 *  11 Interface methodRef garbage_refer
 *  12 Name and type descriptor
 * */
//常量类型
enum {
    CONSTANT_UTF8 = 1,
    CONSTANT_INTEGER = 3,
    CONSTANT_FLOAT = 4,
    CONSTANT_LONG = 5,
    CONSTANT_DOUBLE = 6,
    CONSTANT_CLASS = 7,
    CONSTANT_STRING_REF = 8,
    CONSTANT_FIELD_REF = 9,
    CONSTANT_METHOD_REF = 10,
    CONSTANT_INTERFACE_METHOD_REF = 11,
    CONSTANT_NAME_AND_TYPE = 12,
    CONSTANT_METHOD_HANDLE = 15,
    CONSTANT_METHOD_TYPE = 16,
    CONSTANT_INVOKE_DYNAMIC = 18,
};
//=======================  typedef  =============================
//大小端 byte -> 目标类型
#if __JVM_LITTLE_ENDIAN__
typedef union _Short2Char {
    s16 s;
    struct {
        c8 c0;
        c8 c1;
    };
} Short2Char;

typedef union _Int2Float {
    s32 i;
    f32 f;
    struct {
        c8 c0;
        c8 c1;
        c8 c2;
        c8 c3;
    };
} Int2Float;

typedef union _Long2Double {
    f64 d;
    __refer r;
    union {
        s64 l;
        struct {
            s32 i0;
            s32 i1;
        } i2l;
    };
    struct {
        c8 c0;
        c8 c1;
        c8 c2;
        c8 c3;
        c8 c4;
        c8 c5;
        c8 c6;
        c8 c7;
    };
} Long2Double;
#elif __JVM_BIG_ENDIAN__
typedef union _Short2Char {
    s16 s;
    struct {
        c8 c1;
        c8 c0;
    };
} Short2Char;

typedef union _Int2Float {
    s32 i;
    f32 f;
    struct {
        c8 c3;
        c8 c2;
        c8 c1;
        c8 c0;
    };
} Int2Float;

typedef union _Long2Double {
    f64 d;
    __refer r;
    union {
        s64 l;
        struct {
            s32 i1;
            s32 i0;
        } i2l;
    };
    struct {
        c8 c7;
        c8 c6;
        c8 c5;
        c8 c4;
        c8 c3;
        c8 c2;
        c8 c1;
        c8 c0;
    };
} Long2Double;
#endif

typedef struct _ClassLoader ClassLoader;
typedef struct _ClassType JClass;
typedef struct _InstanceType Instance;
typedef struct _FieldInfo FieldInfo;
typedef struct _MethodInfo MethodInfo;
typedef struct _Instruction Instruction;
typedef struct _ThreadLock ThreadLock;
typedef struct _JavaThreadInfo JavaThreadInfo;
typedef struct _Runtime Runtime;
typedef struct _CodeAttribute CodeAttribute;
typedef struct _JNIENV JniEnv;
typedef struct _ReferArr CStringArr;
typedef struct _ReferArr ReferArr;
//线程运行栈
typedef struct _StackFrame RuntimeStack;


typedef s32 (*java_native_fun)(Runtime *runtime, JClass *p);

typedef void (*StaticLibRegFunc)(JniEnv *env);


enum {
    JVM_ERROR_OUTOFMEMORY,
    JVM_ERROR_VIRTUALMACHINE,
    JVM_ERROR_NOCLASSDEFFOUND,
    JVM_EXCEPTION_EOF,
    JVM_EXCEPTION_IO,
    JVM_EXCEPTION_FILENOTFOUND,
    JVM_EXCEPTION_ARRITHMETIC,
    JVM_EXCEPTION_CLASSNOTFOUND,
    JVM_EXCEPTION_NULLPOINTER,
    JVM_EXCEPTION_NOSUCHMETHOD,
    JVM_EXCEPTION_ILLEGALARGUMENT,
    JVM_EXCEPTION_CLASSCAST,
    JVM_EXCEPTION_ARRAYINDEXOUTOFBOUNDS,
    JVM_EXCEPTION_INSTANTIATION,
};

extern char *STRS_CLASS_EXCEPTION[];

extern c8 *STR_CLASS_JAVA_LANG_STRING;
extern c8 *STR_CLASS_JAVA_LANG_STRINGBUILDER;
extern c8 *STR_CLASS_JAVA_LANG_OBJECT;
extern c8 *STR_CLASS_JAVA_LANG_THREAD;
extern c8 *STR_CLASS_JAVA_LANG_CLASS;
extern c8 *STR_CLASS_JAVA_LANG_STACKTRACE;
extern c8 *STR_CLASS_JAVA_LANG_THROWABLE;
extern c8 *STR_CLASS_JAVA_LANG_INVOKE_METHODTYPE;
extern c8 *STR_CLASS_JAVA_LANG_INVOKE_METHODHANDLE;
extern c8 *STR_CLASS_JAVA_LANG_INVOKE_METHODHANDLES_LOOKUP;

extern c8 *STR_FIELD_STACKFRAME;
extern c8 *STR_FIELD_NAME;
extern c8 *STR_FIELD_VALUE;
extern c8 *STR_FIELD_COUNT;
extern c8 *STR_FIELD_OFFSET;

extern c8 *STR_FIELD_CLASSHANDLE;

extern c8 *STR_METHOD_CLINIT;
extern c8 *STR_METHOD_FINALIZE;

extern c8 *STR_INS_JAVA_LANG_STRING;
extern c8 *STR_INS_JAVA_LANG_THREAD;
extern c8 *STR_INS_JAVA_LANG_CLASS;
extern c8 *STR_INS_JAVA_LANG_OBJECT;
extern c8 *STR_INS_JAVA_LANG_STACKTRACEELEMENT;

enum {
    METHOD_INVOKE_DYNAMIC,
    METHOD_INVOKE_INTERFACE,
    METHOD_INVOKE_STATIC,
    METHOD_INVOKE_VIRTUAL,
    METHOD_INVOKE_SPECIAL
};
/**
 * 内存中几个主要对象的类型，他们是不同的数据结构，但是每种类型的第一个字节都是用来标识此内存对象的类型
 */
enum {
    MEM_TYPE_NODEF = 0, //0
    MEM_TYPE_CLASS = 1, //1
    MEM_TYPE_INS = 2,   //2
    MEM_TYPE_ARR = 4   //3
};


/*
boolean   4
char  5
float  6
double 7
byte 8
short   9
int  10
long  11
  reference 12
 */
#define DATATYPE_COUNT 14
extern c8 *data_type_str;

extern s32 data_type_bytes[DATATYPE_COUNT];
//基本类型 + 引用类型
enum {
    DATATYPE_BOOLEAN = 4,
    DATATYPE_JCHAR = 5,
    DATATYPE_FLOAT = 6,
    DATATYPE_DOUBLE = 7,
    DATATYPE_BYTE = 8,
    DATATYPE_SHORT = 9,
    DATATYPE_INT = 10,
    DATATYPE_LONG = 11,
    DATATYPE_REFERENCE = 12,
    DATATYPE_ARRAY = 13,
};
//访问标志
enum {
    ACC_PUBLIC = 0x0001,
    ACC_PRIVATE = 0x0002,
    ACC_PROTECTED = 0x0004,
    ACC_STATIC = 0x0008,
    ACC_FINAL = 0x0010,
    ACC_SYNCHRONIZED = 0x0020,
    ACC_VOLATILE = 0x0040,
    ACC_TRANSIENT = 0x0080,
    ACC_NATIVE = 0x0100,
    ACC_INTERFACE = 0x0200,
    ACC_ABSTRACT = 0x0400,
    ACC_STRICT = 0x0800,
};
//类状态
enum {
    CLASS_STATUS_RAW,
    CLASS_STATUS_LOADED,
    CLASS_STATUS_PREPARING,
    CLASS_STATUS_PREPARED,
    CLASS_STATUS_CLINITING,
    CLASS_STATUS_CLINITED,
};
//线程
enum {
    THREAD_STATUS_ZOMBIE,
    THREAD_STATUS_RUNNING,
    THREAD_STATUS_SLEEPING,
    THREAD_STATUS_MONITOR,
    THREAD_STATUS_WAIT,
};

//指令指行返回状态
enum {
    RUNTIME_STATUS_NORMAL,
    RUNTIME_STATUS_RETURN,
    RUNTIME_STATUS_EXCEPTION,
    RUNTIME_STATUS_ERROR,
    RUNTIME_STATUS_INTERRUPT,
};

//======================= global var =============================
extern s32 jvm_init_flag;

extern ClassLoader *sys_classloader;

extern u8 java_debug;
extern JniEnv jnienv;

extern ArrayList *thread_list;

extern ArrayList *native_libs;
extern Hashtable *sys_prop;

extern s32 STACK_LENGHT;


//==============profile============
#if _JVM_DEBUG_PROFILE
extern Hashtable *profile_instructs;
extern c8 *inst_name[];
typedef struct _ProfileDetail {
    s64 cost;
    s32 count;
} ProfileDetail;

void profile_init();

void profile_put(u8 instruct_code, s64 cost_add, s64 count_add);

void profile_print();

#endif

//======================= MEM_OBJ =============================

typedef struct _MemoryBlock {

    JClass *clazz;
    struct _MemoryBlock *next;
    ThreadLock *volatile thread_lock;

    u8 type;//type of array or object runtime,class
    u8 garbage_mark;
    u8 garbage_reg;
    u8 arr_type_index;
} MemoryBlock;

struct _ClassLoader {
    ArrayList *classpath;
    Hashtable *classes;

    Hashtable *table_jstring_const;

    //
    spinlock_t lock;
};

void memoryblock_destory(__refer ref);

void classloader_release_classs_static_field(ClassLoader *class_loader);

void classloader_destory(ClassLoader *class_loader);


//======================= class file =============================

//定义 class 文件头结构
/**
类型	名称	数量
u4	magic(魔术)	1
u2	minor_version(次版本号)	1
u2	major_version(主版本号)	1
u2	constant_pool_count(常量个数)	1 
cp_info	constant_pool(常量池表)	constant_pool_count-1
u2	access_flags(类的访问控制权限)	1
u2	this_class(类名)	1
u2	super_class(父类名)	1
u2	interfaces_count(接口个数)	1
u2	interfaces(接口名)	interfaces_count
u2	fields_count(域个数)	1
field_info	fields(域的表)	fields_count
u2 	methods_count(方法的个数)	1
method_info	methods(方法表)	methods_count
u2	attributes_count(附加属性的个数)	1
attribute_info	attributes(附加属性的表)	attributes_count
 **/
/* Java Class File */
typedef struct _ClassFileFormat {
    //魔数
    u8 magic_number[4];
    u16 minor_version;
    u16 major_version;
    //常量数量
    u16 constant_pool_count;
    /* constant pool */
    //访问标志 private 这些
    u16 access_flags;
    //本类类名 index
    u16 this_class;
    //父类类名 index
    u16 super_class;
    //接口数量
    u16 interface_count;
    /* interfaceRef pool */
    //类成员变量数量
    u16 fields_count;
    /* obj_fields pool */
    //方法数量
    u16 methods_count;
    /* methodRef pool */
    //字段附加属性的个数
    u16 attributes_count;
    /* attributes pool */
} ClassFileFormat;


typedef struct _ConstantType {
    s16 index;
    s16 tag;
} ConstantItem;

//====
typedef struct _ConstantNameAndType {
    ConstantItem item;
    u16 nameIndex;
    u16 typeIndex;
} ConstantNameAndType;

//====

typedef struct _ConstantUTF8 {
    ConstantItem item;
    u16 string_size;
    //
    Utf8String *utfstr;
    Instance *jstr;
} ConstantUTF8;

typedef struct _ConstantInteger {
    ConstantItem item;
    s32 value;
} ConstantInteger;

typedef struct _ConstantFloat {
    ConstantItem item;
    f32 value;

} ConstantFloat;

typedef struct _ConstantLong {
    ConstantItem item;
    s64 value;
} ConstantLong;

typedef struct _ConstantDouble {
    ConstantItem item;
    f64 value;
} ConstantDouble;

typedef struct _ConstantClassRef {
    ConstantItem item;
    u16 stringIndex;

    //
    Utf8String *name;
    JClass *clazz;

} ConstantClassRef;

typedef struct _ConstantStringRef {
    ConstantItem item;
    u16 stringIndex;
} ConstantStringRef;

typedef struct _ConstantFieldRef {
    ConstantItem item;
    u16 classIndex;
    u16 nameAndTypeIndex;
    //
    FieldInfo *fieldInfo;
} ConstantFieldRef;

typedef struct _ConstantMethodRef {
    ConstantItem item;
    u16 classIndex;
    u16 nameAndTypeIndex;
    //
    MethodInfo *methodInfo;
    s32 para_slots;
    ConstantNameAndType *nameAndType;
    Utf8String *name;
    Utf8String *descriptor;
    Utf8String *clsName;
    Pairlist *virtual_methods;
} ConstantMethodRef, ConstantInterfaceMethodRef;

//typedef struct _ConstantInterfaceMethodRef {
//    ConstantItem item;
//    u16 classIndex;
//    u16 nameAndTypeIndex;
//
//    //
//    Utf8String *name;
//
//} ConstantInterfaceMethodRef;

typedef struct _ConstantMethodHandle {
    ConstantItem item;
    u8 reference_kind;
    u16 reference_index;
} ConstantMethodHandle;

typedef struct _ConstantMethodType {
    ConstantItem item;
    u16 descriptor_index;
} ConstantMethodType;

typedef struct _ConstantInvokeDynamic {
    ConstantItem item;
    u16 bootstrap_method_attr_index;
    u16 nameAndTypeIndex;
} ConstantInvokeDynamic;


//============================================

typedef struct _ConstantPool {
    ArrayList *utf8CP;
    ArrayList *classRef;
    ArrayList *stringRef;
    ArrayList *fieldRef;
    ArrayList *methodRef;
    ArrayList *interfaceMethodRef;
} ConstantPool;
//============================================

typedef struct _InterfacePool {
    ConstantClassRef *clasz;
    s32 clasz_used;
} InterfacePool;
//============================================

typedef struct _AttributeInfo {
    u16 attribute_name_index;
    s32 attribute_length;
    u8 *info;

} AttributeInfo;

//============================================

typedef struct _line_number {
    u16 start_pc;
    u16 line_number;
} LineNumberTable;

typedef struct _ExceptionTable {
    u16 start_pc;
    u16 end_pc;
    u16 handler_pc;
    u16 catch_type;
} ExceptionTable;

typedef struct _LocalVarTable {
    u16 start_pc;
    u16 length;
    u16 name_index;
    u16 descriptor_index;
    u16 index;
} LocalVarTable;

struct _CodeAttribute {
    u16 attribute_name_index;
    s32 attribute_length;
    u16 max_stack;
    u16 max_locals;
    s32 code_length;
    u8 *code; // [code_length];
    u16 exception_table_length;
    ExceptionTable *exception_table; //[exception_table_length];
    u16 line_number_table_length;
    LineNumberTable *line_number_table;
    u16 local_var_table_length;
    LocalVarTable *local_var_table;

};

//============================================

typedef struct _BootstrapMethod {
    u16 bootstrap_method_ref;
    u16 num_bootstrap_arguments;
    u16 *bootstrap_arguments;

    //cache
    MethodInfo *make;
} BootstrapMethod;

typedef struct BootstrapMethods_attribute {
    u16 num_bootstrap_methods;
    BootstrapMethod *bootstrap_methods;
} BootstrapMethodsAttr;
//============================================

struct _FieldInfo {
    u16 access_flags;
    u16 name_index;
    u16 descriptor_index;
    u16 attributes_count;
    AttributeInfo *attributes;
    //link
    Utf8String *name;
    Utf8String *descriptor;
    JClass *_this_class;
    u16 offset;//字段的偏移地址，静态字段存放在class中
    u16 offset_instance;
    //
    u8 datatype_idx;
    u8 isrefer;
    u8 datatype_bytes;
    u8 isvolatile;
};

typedef struct _FieldPool {
    FieldInfo *field;
    s32 field_used;
} FieldPool;
//============================================

typedef struct _MethodParaOffset {
    s32 stackOffset;
    s16 localOffset;
    s16 byteCount;
} MethodParaOffset;

struct _MethodInfo {
    u16 access_flags;
    u16 name_index;
    u16 descriptor_index;
    u16 attributes_count;
    AttributeInfo *attributes;
    //
    CodeAttribute *converted_code;

    MethodParaOffset *paraOffset;

    //link
    Utf8String *name;
    Utf8String *descriptor;
    Utf8String *paraType;
    Utf8String *returnType;
    JClass *_this_class;
    java_native_fun native_func;
    Pairlist *breakpoint;
    s16 para_slots;
    s16 para_count_with_this;
};
//============================================

typedef struct _MethodPool {
    MethodInfo *method;
    s32 method_used;
} MethodPool;
//============================================

typedef struct _AttributePool {
    AttributeInfo *attribute;
    s32 attribute_used;

} AttributePool;


//======================= class =============================

/*
 Gust 20170719 add Class define
 */
struct _ClassType {
    MemoryBlock mb;
    JClass *superclass;
    __refer *constant_item_ptr;//存放常量池项目地址
    s32 constant_item_count;//总数

    //类变量及实例变量的参数
    s32 field_instance_start;//实例变量模板起始起址，继承自父类的变量放在前面
    s32 field_instance_len; //非静态变量长度
    s32 field_static_len; //静态变量内存长度
    c8 *field_static; //静态变量内存地址

    //
    Instance *ins_class; //object of java.lang.Class
    Instance *jClassLoader;// java classloader

    //public:
    s32 (*_load_class_from_bytes)(struct _ClassType *_this, ByteBuf *buf);

    //
    Utf8String *source;
    BootstrapMethodsAttr *bootstrapMethodAttr;

    //
    Utf8String *name;
    MethodInfo *finalizeMethod;
    ClassFileFormat cff;
    ConstantPool constantPool;
    InterfacePool interfacePool;
    FieldPool fieldPool;
    MethodPool methodPool;
    AttributePool attributePool;

    //for array class
    Pairlist *arr_class_type;//for object array create speedup,left is utf8 index of class, right is arr class
    ArrayList *insFieldPtrIndex;//for optmize , save object pointer field index
    ArrayList *staticFieldPtrIndex; //save static field index

    //
    s8 status;
    u8 primitive;//primitive data type int/long/short/char/short/byte/float/double
};


s32 _DESTORY_CLASS(JClass *clazz);

JClass *class_create(Runtime *runtime);

JClass *getSuperClass(JClass *clazz);

void constant_list_create(JClass *clazz);

void constant_list_destory(JClass *clazz);

s32 class_destory(JClass *clazz);

JClass *resole_class(ByteBuf *bytebuf, Runtime *runtime);

s32 load_class(ClassLoader *loader, Utf8String *pClassName, Runtime *runtime);

//s32 load_related_class(ClassLoader *loader, Class *clazz, Runtime *runtime);

s32 _LOAD_CLASS_FROM_BYTES(JClass *_this, ByteBuf *buf);

s32 class_prepar(JClass *clazz, Runtime *runtime);

void _class_optimize(JClass *clazz);

void class_clinit(JClass *clazz, Runtime *runtime);

void printClassFileFormat(ClassFileFormat *cff);

s32 _class_method_info_destory(JClass *clazz);

s32 _class_attribute_info_destory(JClass *clazz);

s32 _class_interface_pool_destory(JClass *clazz);

s32 _class_constant_pool_destory(JClass *clazz);

s32 _class_field_info_destory(JClass *clazz);

u8 instance_of(JClass *clazz, Instance *ins, Runtime *runtime);

u8 isSonOfInterface(JClass *clazz, JClass *son, Runtime *runtime);

u8 assignable_from(JClass *clazzSon, JClass *clazzSuper);

void class_clear_refer(JClass *clazz);


//======================= instance =============================


struct _InstanceType {
    MemoryBlock mb;
    //
    union {
        c8 *obj_fields; //object fieldRef body
        c8 *arr_body;//array body
    };
    s32 arr_length;

};


Instance *instance_create(Runtime *runtime, JClass *clazz);

void instance_init(Instance *ins, Runtime *runtime);

void instance_init_methodtype(Instance *ins, Runtime *runtime, c8 *methodtype, RuntimeStack *para);

void instance_finalize(Instance *ins, Runtime *runtime);

s32 instance_destory(Instance *instance);

Instance *instance_copy(Runtime *runtime, Instance *src, s32 deep_copy);

//======================= bytecode =============================


//ConstantUTF8 *class_get_constant_utf8(Class *clazz, s32 index);
//
//ConstantStringRef *class_get_constant_stringref(Class *clazz, s32 index);
//
//ConstantClassRef *class_get_constant_classref(Class *clazz, s32 index);
//
//ConstantMethodRef *class_get_constant_method_ref(Class *clazz, s32 index);
//
//ConstantInterfaceMethodRef *class_get_constant_interface_method_ref(Class *clazz, s32 index);
//
//ConstantNameAndType *class_get_constant_name_and_type(Class *clazz, s32 index);
//
//ConstantItem *class_get_constant_item(Class *clazz, s32 index);
//
//ConstantFieldRef *class_get_constant_fieldref(Class *clazz, s32 field_ref);
//
//s32 class_get_constant_integer(Class *clazz, s32 index);
//
//s64 class_get_constant_long(Class *clazz, s32 index);
//
//Utf8String *class_get_utf8_string(Class *clazz, s32 index);
//
//f32 class_get_constant_float(Class *clazz, s32 index);
//
//f64 class_get_double_from_constant_pool(Class *clazz, s32 index);


/* find UTF8 */
static inline ConstantUTF8 *class_get_constant_utf8(JClass *clazz, s32 index) {
    return (ConstantUTF8 *) (clazz->constant_item_ptr[index]);
}

/* Find Class Reference */
static inline ConstantStringRef *class_get_constant_stringref(JClass *clazz, s32 index) {
    return (ConstantStringRef *) (clazz->constant_item_ptr[index]);
}


/* Find Class Reference */
static inline ConstantClassRef *class_get_constant_classref(JClass *clazz, s32 index) {
    return (ConstantClassRef *) (clazz->constant_item_ptr[index]);
}

static inline ConstantFieldRef *class_get_constant_fieldref(JClass *clazz, s32 index) {
    return (ConstantFieldRef *) (clazz->constant_item_ptr[index]);
}

static inline ConstantItem *class_get_constant_item(JClass *clazz, s32 index) {
    return (ConstantItem *) (clazz->constant_item_ptr[index]);
}

/* Find Method Reference */
static inline ConstantMethodRef *class_get_constant_method_ref(JClass *clazz, s32 index) {
    return (ConstantMethodRef *) (clazz->constant_item_ptr[index]);
}

static inline ConstantInterfaceMethodRef *
class_get_constant_interface_method_ref(JClass *clazz, s32 index) {
    return (ConstantInterfaceMethodRef *) (clazz->constant_item_ptr[index]);
}

/* Find Name and Type Reference */
static inline ConstantNameAndType *class_get_constant_name_and_type(JClass *clazz, s32 index) {
    return (ConstantNameAndType *) (clazz->constant_item_ptr[index]);
}

/* get integer from constant pool */
static inline s32 class_get_constant_integer(JClass *clazz, s32 index) {
    return ((ConstantInteger *) (clazz->constant_item_ptr[index]))->value;
}

/* get long from constant pool */
static inline s64 class_get_constant_long(JClass *clazz, s32 index) {
    return ((ConstantLong *) (clazz->constant_item_ptr[index]))->value;
}

/* get f32 from constant pool */
static inline f32 class_get_constant_float(JClass *clazz, s32 index) {
    return ((ConstantFloat *) (clazz->constant_item_ptr[index]))->value;
}

/* get f64 from constant pool */
static inline f64 class_get_double_from_constant_pool(JClass *clazz, s32 index) {
    return ((ConstantDouble *) (clazz->constant_item_ptr[index]))->value;
}

static inline Utf8String *class_get_utf8_string(JClass *clazz, s32 index) {
    return ((ConstantUTF8 *) (clazz->constant_item_ptr[index]))->utfstr;
}

static inline ConstantMethodHandle *class_get_method_handle(JClass *clazz, s32 index) {
    return (ConstantMethodHandle *) (clazz->constant_item_ptr[index]);
}

static inline ConstantMethodType *class_get_method_type(JClass *clazz, s32 index) {
    return (ConstantMethodType *) (clazz->constant_item_ptr[index]);
}

static inline ConstantInvokeDynamic *class_get_invoke_dynamic(JClass *clazz, s32 index) {
    return (ConstantInvokeDynamic *) (clazz->constant_item_ptr[index]);
}

MethodInfo *find_instance_methodInfo_by_name(Instance *ins, Utf8String *methodName, Utf8String *methodType, Runtime *runtime);

MethodInfo *find_methodInfo_by_methodref(JClass *clazz, s32 method_ref, Runtime *runtime);

MethodInfo *find_methodInfo_by_name(Utf8String *clsName, Utf8String *methodName, Utf8String *methodType, Runtime *runtime);

MethodInfo *find_methodInfo_by_name_c(c8 *pclsName, c8 *pmethodName, c8 *pmethodType, Runtime *runtime);

FieldInfo *find_fieldInfo_by_fieldref(JClass *clazz, s32 field_ref, Runtime *runtime);

FieldInfo *find_fieldInfo_by_name_c(c8 *pclsName, c8 *pfieldName, c8 *pfieldType, Runtime *runtime);

FieldInfo *find_fieldInfo_by_name(Utf8String *clsName, Utf8String *fieldName, Utf8String *fieldType, Runtime *runtime);

//


JClass *getClassByConstantClassRef(JClass *clazz, s32 index, Runtime *runtime);

//======================= runtime =============================

/* Stack Frame */
#define STACK_ENTRY_NONE        0
#define STACK_ENTRY_INT         1
#define STACK_ENTRY_FLOAT       2
#define STACK_ENTRY_LONG        4
#define STACK_ENTRY_DOUBLE      8
#define STACK_ENTRY_REF         16

typedef struct _StackEntry {
    union {
        s64 lvalue;
        f64 dvalue;
        f32 fvalue;
        s32 ivalue;
        __refer rvalue;
        Instance *ins;
    };
    s32 type;
} StackEntry, LocalVarItem;

struct _StackFrame {
    StackEntry *store;
    s32 size;
    s32 max_size;
};

//解决引用类型可能为4字节或8字节的不同情况
//typedef struct _LocalVarItem {
//    __refer refer;
//    s32 integer;
//} LocalVarItem;


struct _Runtime {

    //方法结构体
    MethodInfo *method;
    //类结构体
    JClass *clazz;
    //pc 指针
    u8 *pc;
    //方法字节码
    CodeAttribute *ca;//method bytecode
    //当前线程信息
    JavaThreadInfo *threadInfo;
    //子方法 runtime，类似栈
    Runtime *son;//sub method's runtime
    //父方法 runtime
    Runtime *parent;//father method's runtime
    //JVM 运行栈，用于基于栈实现的解释器
    RuntimeStack *stack;
    //方法本地变量
    LocalVarItem *localvar;
    //Runtime 缓存
    union {
        Runtime *runtime_pool_header;// cache runtimes for performance
        Runtime *next;  //for runtime pools linklist
    };
    //JNI 结构体
    JniEnv *jnienv;
    s16 localvar_count;
    s16 localvar_max;
    u8 wideMode;
};

//======================= stack =============================
RuntimeStack *stack_create(s32 entry_size);

void stack_destory(RuntimeStack *stack);

//======================= native stack =============================
void push_entry_jni(RuntimeStack *stack, StackEntry *entry);

void push_int_jni(RuntimeStack *stack, s32 value);

void push_long_jni(RuntimeStack *stack, s64 value);

void push_double_jni(RuntimeStack *stack, f64 value);

void push_float_jni(RuntimeStack *stack, f32 value);

void push_ref_jni(RuntimeStack *stack, __refer value);

__refer pop_ref_jni(RuntimeStack *stack);

s32 pop_int_jni(RuntimeStack *stack);

s64 pop_long_jni(RuntimeStack *stack);

f64 pop_double_jni(RuntimeStack *stack);

f32 pop_float_jni(RuntimeStack *stack);

void pop_entry_jni(RuntimeStack *stack, StackEntry *entry);

void pop_empty_jni(RuntimeStack *stack);

s32 entry_2_int_jni(StackEntry *entry);

void peek_entry_jni(RuntimeStack *stack, StackEntry *entry, int index);

s64 entry_2_long_jni(StackEntry *entry);

__refer entry_2_refer_jni(StackEntry *entry);

//======================= native stack =============================


//======================= java stack =============================
/* push Integer */
static inline void push_int(RuntimeStack *stack, s32 value) {
    StackEntry *ptr = &stack->store[stack->size++];
    ptr->ivalue = value;//clear 64bit
    ptr->type = STACK_ENTRY_INT;
}


/* pop Integer */
static inline s32 pop_int(RuntimeStack *stack) {
    return stack->store[--stack->size].ivalue;
}

/* push Double */
static inline void push_double(RuntimeStack *stack, f64 value) {
    StackEntry *store = stack->store;
    StackEntry *ptr = &store[stack->size];
    ptr->dvalue = value;
    ptr->type = STACK_ENTRY_DOUBLE;
    ptr = &store[stack->size + 1];
    ptr->type = STACK_ENTRY_DOUBLE;
//    ptr->dvalue = value;
    stack->size += 2;
}

/* pop Double */
static inline f64 pop_double(RuntimeStack *stack) {
    stack->size -= 2;
    return stack->store[stack->size].dvalue;
}

/* push Float */
static inline void push_float(RuntimeStack *stack, f32 value) {
    StackEntry *ptr = &stack->store[stack->size++];
    //ptr->lvalue = 0;//clear 64bit
    ptr->fvalue = value;
    ptr->type = STACK_ENTRY_FLOAT;
}

/* pop Float */
static inline f32 pop_float(RuntimeStack *stack) {
    return stack->store[--stack->size].fvalue;
}


/* push Long */
static inline void push_long(RuntimeStack *stack, s64 value) {
    StackEntry *store = stack->store;
    StackEntry *ptr = &store[stack->size];
    ptr->lvalue = value;
    ptr->type = STACK_ENTRY_LONG;
    ptr = &store[stack->size + 1];
    ptr->type = STACK_ENTRY_LONG;
//    ptr->lvalue = value;
    stack->size += 2;
}

/* pop Long */
static inline s64 pop_long(RuntimeStack *stack) {
    stack->size -= 2;
    return stack->store[stack->size].lvalue;
}

/* push Ref */
static inline void push_ref(RuntimeStack *stack, __refer value) {
    StackEntry *ptr = &stack->store[stack->size++];
    //ptr->lvalue = 0;//clear 64bit
    ptr->type = STACK_ENTRY_REF;
    ptr->rvalue = value;
}

static inline __refer pop_ref(RuntimeStack *stack) {
    return stack->store[--stack->size].rvalue;
}


static inline void push_entry(RuntimeStack *stack, StackEntry *entry) {
    StackEntry *ptr = &stack->store[stack->size++];
    ptr->type = entry->type;
    ptr->lvalue = entry->lvalue;
}

/* Pop Stack Entry */
static inline void pop_entry(RuntimeStack *stack, StackEntry *entry) {
    StackEntry *ptr = &stack->store[--stack->size];
    entry->type = ptr->type;
    entry->lvalue = ptr->lvalue;

}

static inline void pop_empty(RuntimeStack *stack) {
    stack->size--;
}


static inline void peek_entry(RuntimeStack *stack, StackEntry *entry, int index) {
    //memcpy(entry, &stack->store[index].lvalue, sizeof(StackEntry));
    StackEntry *ptr = &stack->store[index];
    entry->lvalue = ptr->lvalue;
    entry->type = ptr->type;
}


/* Entry to Int */
static inline s32 entry_2_int(StackEntry *entry) {
    return entry->ivalue;
}

static inline s64 entry_2_long(StackEntry *entry) {
    return entry->lvalue;
}

static inline __refer entry_2_refer(StackEntry *entry) {
    return entry->rvalue;
}

//s32 is_cat2(StackEntry *entry);
//
//s32 is_cat1(StackEntry *entry);

static inline s32 is_cat1(StackEntry *entry) {
    if (entry->type & (STACK_ENTRY_INT | STACK_ENTRY_FLOAT | STACK_ENTRY_REF)) {
        return 1;
    }
    return 0;
}

static inline s32 is_cat2(StackEntry *entry) {
    if (entry->type & (STACK_ENTRY_LONG | STACK_ENTRY_DOUBLE)) {
        return 1;
    }
    return 0;
}

s32 is_ref(StackEntry *entry);

//======================= java stack =============================


//======================= localvar =============================
enum {
    RUNTIME_LOCALVAR_SIZE = 10,
};

Runtime *runtime_create(Runtime *parent);

void runtime_destory(Runtime *runtime);

Runtime *getLastSon(Runtime *top);

Runtime *getTopRuntime(Runtime *runtime);

s64 getInstructPointer(Runtime *runtime);

void getRuntimeStack(Runtime *runtime, Utf8String *ustr);

s32 getRuntimeDepth(Runtime *top);

static inline s32 localvar_init(Runtime *runtime, s32 count) {
    if (count > runtime->localvar_max) {
        jvm_free(runtime->localvar);
        runtime->localvar = jvm_calloc(sizeof(LocalVarItem) * count);
        runtime->localvar_max = count;
    } else {
        memset(runtime->localvar, 0, count * sizeof(LocalVarItem));
    }
    runtime->localvar_count = count;
    return 0;
}

static inline s32 localvar_dispose(Runtime *runtime) {
    runtime->localvar_count = 0;
    return 0;
}


static inline void localvar_setInt(LocalVarItem *localvar, s32 index, s32 val) {
    localvar[index].ivalue = val;
    localvar[index].type = STACK_ENTRY_INT;
}

static inline s32 localvar_getInt(LocalVarItem *localvar, s32 index) {
    return localvar[index].ivalue;
}

static inline void localvar_setLong(LocalVarItem *localvar, s32 index, s64 val) {
    localvar[index].lvalue = val;
    localvar[index].type = STACK_ENTRY_LONG;
}

static inline s64 localvar_getLong(LocalVarItem *localvar, s32 index) {
    return localvar[index].lvalue;
}

static inline void localvar_setRefer(LocalVarItem *localvar, s32 index, __refer val) {
    localvar[index].rvalue = val;
    localvar[index].type = STACK_ENTRY_REF;
}

static inline __refer localvar_getRefer(LocalVarItem *localvar, s32 index) {
    return localvar[index].rvalue;
}


void localvar_setInt_jni(LocalVarItem *localvar, s32 index, s32 val);

void localvar_setRefer_jni(LocalVarItem *localvar, s32 index, __refer val);

s32 localvar_getInt_jni(LocalVarItem *localvar, s32 index);

__refer localvar_getRefer_jni(LocalVarItem *localvar, s32 index);

s64 localvar_getLong_2slot_jni(LocalVarItem *localvar, s32 index);

void localvar_setLong_2slot_jni(LocalVarItem *localvar, s32 index, s64 val);

//======================= other =============================
//======================= execute =============================
//
void open_log(void);

void close_log(void);

c8 *getMajorVersionString(u16 major_number);


void jvm_init(c8 *p_classpath, StaticLibRegFunc regFunc);

void jvm_destroy(StaticLibRegFunc unRegFunc);

void thread_boundle(Runtime *runtime);

void thread_unboundle(Runtime *runtime);

void print_exception(Runtime *runtime);

s32 execute_jvm(c8 *p_classpath, c8 *mainclass, ArrayList *java_para);

s32 call_method_main(c8 *p_mainclass, c8 *p_methodname, c8 *p_methodtype, ArrayList *java_para);

s32 call_method_c(c8 *p_mainclass, c8 *p_methodname, c8 *p_methodtype, Runtime *runtime);

s32 execute_method_impl(MethodInfo *method, Runtime *runtime, JClass *clazz);

s32 execute_method(MethodInfo *method, Runtime *runtime, JClass *clazz);

//======================= jni =============================
typedef struct _java_native_method {
    c8 *clzname;
    c8 *methodname;
    c8 *methodtype;
    java_native_fun func_pointer;
} java_native_method;

java_native_method *find_native_method(c8 *cls_name, c8 *method_name, c8 *method_type);

s32 invoke_native_method(Runtime *runtime, JClass *p,
                         c8 *cls_name, c8 *method_name, c8 *type);


typedef struct _JavaNativeLib {
    java_native_method *methods;
    s32 methods_count;
} JavaNativeLib;


struct _ReferArr {
    __refer *arr_body;
    s32 arr_length;
};

s32 native_reg_lib(java_native_method *methods, s32 method_size);

s32 native_remove_lib(JavaNativeLib *lib);

s32 native_lib_destory(void);

void reg_std_native_lib(void);

void reg_net_native_lib(void);

void reg_jdwp_native_lib(void);

void init_jni_func_table();

struct _JNIENV {
    s32 *data_type_bytes;

    s32 (*native_reg_lib)(java_native_method *methods, s32 method_size);

    s32 (*native_remove_lib)(JavaNativeLib *lib);

    void (*push_entry)(RuntimeStack *stack, StackEntry *entry);

    void (*push_int)(RuntimeStack *stack, s32 value);

    void (*push_long)(RuntimeStack *stack, s64 value);

    void (*push_double)(RuntimeStack *stack, f64 value);

    void (*push_float)(RuntimeStack *stack, f32 value);

    void (*push_ref)(RuntimeStack *stack, __refer value);

    __refer (*pop_ref)(RuntimeStack *stack);

    s32 (*pop_int)(RuntimeStack *stack);

    s64 (*pop_long)(RuntimeStack *stack);

    f64 (*pop_double)(RuntimeStack *stack);

    f32 (*pop_float)(RuntimeStack *stack);

    void (*pop_entry)(RuntimeStack *stack, StackEntry *entry);

    void (*pop_empty)(RuntimeStack *stack);

    s32 (*entry_2_int)(StackEntry *entry);

    void (*peek_entry)(RuntimeStack *stack, StackEntry *entry, int index);

    s64 (*entry_2_long)(StackEntry *entry);

    __refer (*entry_2_refer)(StackEntry *entry);

    void (*localvar_setRefer)(LocalVarItem *localvar, s32 index, __refer val);

    void (*localvar_setInt)(LocalVarItem *localvar, s32 index, s32 val);

    __refer (*localvar_getRefer)(LocalVarItem *localvar, s32 index);

    s32 (*localvar_getInt)(LocalVarItem *localvar, s32 index);

    s64 (*localvar_getLong_2slot)(LocalVarItem *localvar, s32 index);

    void (*localvar_setLong_2slot)(LocalVarItem *localvar, s32 index, s64 val);

    void (*jthread_block_enter)(Runtime *runtime);

    void (*jthread_block_exit)(Runtime *runtime);

    Utf8String *(*utf8_create)();

    Utf8String *(*utf8_create_part_c)(char *str, int start, int len);

    char *(*utf8_cstr)(Utf8String *a1);

    void (*utf8_destory)(Utf8String *);

    Instance *(*jstring_create)(Utf8String *src, Runtime *runtime);

    Instance *(*jstring_create_cstr)(c8 *cstr, Runtime *runtime);

    s32 (*jstring_2_utf8)(Instance *jstr, Utf8String *utf8);

    CStringArr *(*cstringarr_create)(Instance *jstr_arr);

    void (*cstringarr_destory)(CStringArr *cstr_arr);

    ReferArr *(*referarr_create)(Instance *jobj_arr);

    void (*referarr_destory)(CStringArr *ref_arr);

    void (*referarr_2_jlongarr)(ReferArr *ref_arr, Instance *jlong_arr);

    Instance *(*jarray_create_by_type_name)(Runtime *runtime, s32 count, Utf8String *type);

    Instance *(*jarray_create_by_type_index)(Runtime *runtime, s32 count, s32 typeIdx);

    void (*jarray_set_field)(Instance *arr, s32 index, s64 val);

    s64 (*jarray_get_field)(Instance *arr, s32 index);

    void *(*jvm_calloc)(u32 size);

    void *(*jvm_malloc)(u32 size);

    void (*jvm_free)(void *ptr);

    void *(*jvm_realloc)(void *pPtr, u32 size);

    void (*instance_release_from_thread)(Instance *ref, Runtime *runtime);

    void (*instance_hold_to_thread)(Instance *ref, Runtime *runtime);

    s32 (*execute_method)(MethodInfo *method, Runtime *runtime, JClass *clazz);

    MethodInfo *(*find_methodInfo_by_name)(Utf8String *clsName, Utf8String *methodName, Utf8String *methodType, Runtime *runtime);

    void (*print_exception)(Runtime *runtime);

    void (*garbage_refer_hold)(__refer ref);

    void (*garbage_refer_release)(__refer ref);
};



//=======================   =============================


#ifdef __cplusplus
}
#endif


#endif
