/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <iostream>
#include <string>
#include "Symtable/symtable.h"
#include "Symtable/TableEntry/SymbolTableEntry.h"
#include "Symtable/ScopeList/scopelist.h"

#include "Quads/quad.h"
#include "expr_handler.h"




using namespace         std;
extern FILE*            yyin;
void yyerror            (const char* yaccProvidedMessage);
extern int              yylineno;
extern int              yylex(void);
extern char*            yytext;
extern int              yylex(void);
extern unsigned int     scope;
extern unsigned int     currQuad;
std::vector<void *>     args;


#define DEBUG_REDUCE(msg) std::cout << "Reduced: " << msg << " (line " << yylineno << ")\n"
//#define DEBUG_REDUCE(msg)


#line 101 "parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INTEGER = 3,                    /* INTEGER  */
  YYSYMBOL_DOUBLE = 4,                     /* DOUBLE  */
  YYSYMBOL_REALCONST = 5,                  /* REALCONST  */
  YYSYMBOL_IDENT = 6,                      /* IDENT  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_FUNCTION = 10,                  /* FUNCTION  */
  YYSYMBOL_FOR = 11,                       /* FOR  */
  YYSYMBOL_RETURN = 12,                    /* RETURN  */
  YYSYMBOL_BREAK = 13,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 14,                  /* CONTINUE  */
  YYSYMBOL_AND = 15,                       /* AND  */
  YYSYMBOL_NOT = 16,                       /* NOT  */
  YYSYMBOL_OR = 17,                        /* OR  */
  YYSYMBOL_NIL = 18,                       /* NIL  */
  YYSYMBOL_TRUE = 19,                      /* TRUE  */
  YYSYMBOL_FALSE = 20,                     /* FALSE  */
  YYSYMBOL_LOCAL = 21,                     /* LOCAL  */
  YYSYMBOL_ASSIGN = 22,                    /* ASSIGN  */
  YYSYMBOL_PLUS = 23,                      /* PLUS  */
  YYSYMBOL_MINUS = 24,                     /* MINUS  */
  YYSYMBOL_MULT = 25,                      /* MULT  */
  YYSYMBOL_DIV = 26,                       /* DIV  */
  YYSYMBOL_MOD = 27,                       /* MOD  */
  YYSYMBOL_EQUAL = 28,                     /* EQUAL  */
  YYSYMBOL_NOT_EQUALS = 29,                /* NOT_EQUALS  */
  YYSYMBOL_PLUS_PLUS = 30,                 /* PLUS_PLUS  */
  YYSYMBOL_MINUS_MINUS = 31,               /* MINUS_MINUS  */
  YYSYMBOL_GREATER = 32,                   /* GREATER  */
  YYSYMBOL_LESS = 33,                      /* LESS  */
  YYSYMBOL_GREATER_EQUAL = 34,             /* GREATER_EQUAL  */
  YYSYMBOL_LESS_EQUAL = 35,                /* LESS_EQUAL  */
  YYSYMBOL_LEFT_BRACE = 36,                /* LEFT_BRACE  */
  YYSYMBOL_RIGHT_BRACE = 37,               /* RIGHT_BRACE  */
  YYSYMBOL_LEFT_BRACKET = 38,              /* LEFT_BRACKET  */
  YYSYMBOL_RIGHT_BRACKET = 39,             /* RIGHT_BRACKET  */
  YYSYMBOL_LEFT_PARENTHESIS = 40,          /* LEFT_PARENTHESIS  */
  YYSYMBOL_RIGHT_PARENTHESIS = 41,         /* RIGHT_PARENTHESIS  */
  YYSYMBOL_SEMICOLON = 42,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 43,                     /* COMMA  */
  YYSYMBOL_DOUBLE_DOT = 44,                /* DOUBLE_DOT  */
  YYSYMBOL_DOT = 45,                       /* DOT  */
  YYSYMBOL_NAMESPACE = 46,                 /* NAMESPACE  */
  YYSYMBOL_COLON = 47,                     /* COLON  */
  YYSYMBOL_STRING = 48,                    /* STRING  */
  YYSYMBOL_UNDEFINED = 49,                 /* UNDEFINED  */
  YYSYMBOL_UMINUS = 50,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 51,                  /* $accept  */
  YYSYMBOL_program = 52,                   /* program  */
  YYSYMBOL_stmt_list = 53,                 /* stmt_list  */
  YYSYMBOL_stmt = 54,                      /* stmt  */
  YYSYMBOL_expr = 55,                      /* expr  */
  YYSYMBOL_term = 56,                      /* term  */
  YYSYMBOL_assignexpr = 57,                /* assignexpr  */
  YYSYMBOL_primary = 58,                   /* primary  */
  YYSYMBOL_lvalue = 59,                    /* lvalue  */
  YYSYMBOL_member = 60,                    /* member  */
  YYSYMBOL_call = 61,                      /* call  */
  YYSYMBOL_callsuffix = 62,                /* callsuffix  */
  YYSYMBOL_normcall = 63,                  /* normcall  */
  YYSYMBOL_methodcall = 64,                /* methodcall  */
  YYSYMBOL_elist = 65,                     /* elist  */
  YYSYMBOL_elist_tail = 66,                /* elist_tail  */
  YYSYMBOL_objectdef = 67,                 /* objectdef  */
  YYSYMBOL_indexed = 68,                   /* indexed  */
  YYSYMBOL_indexedelem = 69,               /* indexedelem  */
  YYSYMBOL_block = 70,                     /* block  */
  YYSYMBOL_71_1 = 71,                      /* $@1  */
  YYSYMBOL_funcdef = 72,                   /* funcdef  */
  YYSYMBOL_73_2 = 73,                      /* $@2  */
  YYSYMBOL_74_3 = 74,                      /* $@3  */
  YYSYMBOL_75_4 = 75,                      /* $@4  */
  YYSYMBOL_76_5 = 76,                      /* $@5  */
  YYSYMBOL_const = 77,                     /* const  */
  YYSYMBOL_idlist = 78,                    /* idlist  */
  YYSYMBOL_ifprefix = 79,                  /* ifprefix  */
  YYSYMBOL_elseprefix = 80,                /* elseprefix  */
  YYSYMBOL_ifstmt = 81,                    /* ifstmt  */
  YYSYMBOL_loopstart = 82,                 /* loopstart  */
  YYSYMBOL_loopend = 83,                   /* loopend  */
  YYSYMBOL_loopstmt = 84,                  /* loopstmt  */
  YYSYMBOL_whilestart = 85,                /* whilestart  */
  YYSYMBOL_whilecond = 86,                 /* whilecond  */
  YYSYMBOL_whilestmt = 87,                 /* whilestmt  */
  YYSYMBOL_N = 88,                         /* N  */
  YYSYMBOL_M = 89,                         /* M  */
  YYSYMBOL_forprefix = 90,                 /* forprefix  */
  YYSYMBOL_forstmt = 91,                   /* forstmt  */
  YYSYMBOL_Break = 92,                     /* Break  */
  YYSYMBOL_Continue = 93,                  /* Continue  */
  YYSYMBOL_returnstmt = 94                 /* returnstmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   613

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  191

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   305


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   109,   109,   113,   117,   121,   126,   127,   128,   129,
     130,   131,   132,   133,   138,   145,   146,   147,   148,   149,
     150,   151,   166,   174,   188,   196,   204,   212,   221,   230,
     234,   238,   245,   254,   268,   284,   297,   313,   320,   349,
     350,   351,   352,   354,   358,   359,   360,   361,   365,   366,
     367,   368,   372,   385,   386,   391,   392,   396,   400,   405,
     406,   409,   413,   419,   420,   424,   425,   429,   434,   434,
     439,   440,   439,   443,   445,   443,   452,   453,   454,   455,
     456,   457,   461,   465,   469,   473,   487,   499,   504,   513,
     515,   517,   520,   527,   541,   557,   564,   570,   582,   597,
     613,   627,   635
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INTEGER", "DOUBLE",
  "REALCONST", "IDENT", "IF", "ELSE", "WHILE", "FUNCTION", "FOR", "RETURN",
  "BREAK", "CONTINUE", "AND", "NOT", "OR", "NIL", "TRUE", "FALSE", "LOCAL",
  "ASSIGN", "PLUS", "MINUS", "MULT", "DIV", "MOD", "EQUAL", "NOT_EQUALS",
  "PLUS_PLUS", "MINUS_MINUS", "GREATER", "LESS", "GREATER_EQUAL",
  "LESS_EQUAL", "LEFT_BRACE", "RIGHT_BRACE", "LEFT_BRACKET",
  "RIGHT_BRACKET", "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "SEMICOLON",
  "COMMA", "DOUBLE_DOT", "DOT", "NAMESPACE", "COLON", "STRING",
  "UNDEFINED", "UMINUS", "$accept", "program", "stmt_list", "stmt", "expr",
  "term", "assignexpr", "primary", "lvalue", "member", "call",
  "callsuffix", "normcall", "methodcall", "elist", "elist_tail",
  "objectdef", "indexed", "indexedelem", "block", "$@1", "funcdef", "$@2",
  "$@3", "$@4", "$@5", "const", "idlist", "ifprefix", "elseprefix",
  "ifstmt", "loopstart", "loopend", "loopstmt", "whilestart", "whilecond",
  "whilestmt", "N", "M", "forprefix", "forstmt", "Break", "Continue",
  "returnstmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-162)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -162,    10,   167,  -162,  -162,  -162,  -162,   -15,  -162,    14,
     -10,   205,    12,    20,   282,  -162,  -162,  -162,    27,   282,
      -3,    -3,  -162,   236,   259,  -162,    44,  -162,  -162,   354,
    -162,  -162,  -162,   -17,  -162,    -6,  -162,  -162,  -162,  -162,
     167,  -162,    49,  -162,  -162,  -162,  -162,  -162,  -162,   282,
    -162,    50,   282,  -162,   375,  -162,  -162,    45,  -162,  -162,
      42,    -9,    -6,    -9,  -162,   282,   333,    52,   -32,  -162,
     417,    51,  -162,  -162,  -162,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,  -162,   282,  -162,  -162,
     282,   282,    87,    88,  -162,  -162,  -162,   282,   282,    90,
      89,   282,  -162,   282,   438,    58,    94,    59,  -162,    62,
     116,   308,   282,  -162,  -162,  -162,    69,  -162,    67,   282,
     282,    -8,    -8,    -8,    -8,    -8,   584,   584,    32,    32,
      32,    32,   543,   480,    68,    72,  -162,   501,    73,  -162,
    -162,   167,   459,   167,  -162,    74,  -162,    94,  -162,     1,
    -162,    67,  -162,   282,   333,  -162,   282,   571,   558,  -162,
    -162,   282,  -162,  -162,  -162,  -162,  -162,  -162,     8,  -162,
     107,   282,   522,  -162,    75,    79,  -162,   167,  -162,    95,
    -162,   396,  -162,  -162,  -162,  -162,    95,  -162,  -162,  -162,
    -162
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,    76,    77,    44,     0,    92,    70,
       0,     0,     0,     0,     0,    79,    80,    81,     0,     0,
       0,     0,    68,    59,     0,    14,     0,    78,     3,     0,
      29,    15,    37,    39,    47,    40,    41,    12,    13,    43,
       0,     6,     0,     7,    95,     8,    10,    11,     9,     0,
      73,     0,    59,   101,     0,    99,   100,    32,    45,    31,
       0,    33,     0,    35,     4,     0,    62,     0,     0,    65,
       0,     0,    46,    96,    96,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     0,    34,    36,
       0,    59,     0,     0,    52,    55,    56,     0,    59,     0,
      87,     0,    89,    59,     0,     0,    84,     0,   102,     0,
       0,     0,     0,    60,    63,    64,     0,    30,    42,     0,
       0,    16,    17,    18,    19,    20,    25,    26,    21,    23,
      22,    24,    38,     0,     0,     0,    48,     0,     0,    50,
      86,     0,     0,     0,    94,     0,    85,    84,    82,     0,
      96,     0,    69,     0,    62,    66,    59,    27,    28,    49,
      57,    59,    51,    53,    88,    93,    90,    95,     0,    71,
       0,     0,     0,    61,     0,     0,    91,     0,    74,     0,
      83,     0,    67,    54,    58,    95,     0,    72,    97,    98,
      75
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -162,  -162,    60,   -39,    -2,  -162,  -162,  -162,    25,  -162,
      66,  -162,  -162,  -162,   -50,   -21,  -162,  -162,    26,  -160,
    -162,   -20,  -162,  -162,  -162,  -162,  -162,    -4,  -162,  -162,
    -162,  -162,  -162,  -162,  -162,  -162,  -162,  -161,   -66,  -162,
    -162,  -162,  -162,  -162
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    28,    66,    30,    31,    32,    33,    34,
      35,    94,    95,    96,    67,   113,    36,    68,    69,    37,
      64,    38,    51,   179,   105,   186,    39,   149,    40,   141,
      41,   143,   176,   144,    42,   102,    43,   103,   119,    44,
      45,    46,    47,    48
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      29,   100,   107,     6,    71,    87,   177,   115,   120,    54,
       3,   116,    57,    88,    89,    75,    76,    59,    18,   187,
      50,    90,    70,    91,   189,    49,   190,    92,    93,    90,
      52,    91,    97,    58,    98,    92,    93,    60,    29,    99,
     109,   134,   169,    26,   170,    61,    63,   104,   138,   178,
      72,   170,     9,   145,    55,    75,    76,    77,    78,    79,
      80,    81,    56,   111,    -1,    -1,    -1,    -1,    75,    76,
      77,    78,    79,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   171,   132,    62,    62,   133,   101,
     106,   114,   118,   135,   136,   137,   139,   140,   147,   142,
     148,   150,   164,   151,   166,    65,   174,   156,    29,   160,
     154,   175,   161,   180,   163,   167,   183,   157,   158,     4,
     184,     5,     6,     7,   110,     8,     9,    10,    11,    12,
      13,    22,    14,   173,    15,    16,    17,    18,   185,    29,
      19,    29,   155,   168,     0,     0,    20,    21,     0,     0,
       0,   172,    22,   152,    23,     0,    24,     0,    25,     0,
       0,     0,    26,     0,    27,     0,     0,     0,     0,   181,
       4,     0,     5,     6,     7,    29,     8,     9,    10,    11,
      12,    13,     0,    14,     0,    15,    16,    17,    18,     0,
       0,    19,     0,     0,     0,     0,     0,    20,    21,     0,
       0,     0,     0,    22,     0,    23,     0,    24,     4,    25,
       5,     6,     0,    26,     0,    27,     0,     0,     0,     0,
       0,    14,     0,    15,    16,    17,    18,     0,     0,    19,
       0,     0,     0,     0,     0,    20,    21,     0,     0,     4,
       0,     5,     6,    23,     0,    24,     0,    53,     0,     0,
       0,    26,    14,    27,    15,    16,    17,    18,     0,     0,
      19,     0,     4,     0,     5,     6,    20,    21,     0,     9,
       0,     0,    65,     0,    23,    14,    24,    15,    16,    17,
      18,     0,    26,    19,    27,     4,     0,     5,     6,    20,
      21,     0,     0,     0,     0,     0,     0,    23,    14,    24,
      15,    16,    17,    18,     0,    26,    19,    27,     0,     0,
       0,     0,    20,    21,     0,     0,     0,     0,     0,     0,
      23,     0,    24,    73,     0,    74,     0,     0,    26,     0,
      27,    75,    76,    77,    78,    79,    80,    81,     0,     0,
      82,    83,    84,    85,     0,     0,     0,     0,    73,     0,
      74,     0,     0,     0,     0,   153,    75,    76,    77,    78,
      79,    80,    81,     0,     0,    82,    83,    84,    85,    73,
       0,    74,     0,     0,     0,     0,   112,    75,    76,    77,
      78,    79,    80,    81,     0,     0,    82,    83,    84,    85,
      73,     0,    74,     0,     0,     0,    86,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,    82,    83,    84,
      85,    73,     0,    74,     0,     0,     0,   108,     0,    75,
      76,    77,    78,    79,    80,    81,     0,     0,    82,    83,
      84,    85,    73,     0,    74,     0,     0,     0,   188,     0,
      75,    76,    77,    78,    79,    80,    81,     0,     0,    82,
      83,    84,    85,    73,     0,    74,     0,     0,   117,     0,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
      82,    83,    84,    85,    73,     0,    74,     0,     0,   146,
       0,     0,    75,    76,    77,    78,    79,    80,    81,     0,
       0,    82,    83,    84,    85,    73,     0,    74,     0,     0,
     165,     0,     0,    75,    76,    77,    78,    79,    80,    81,
       0,     0,    82,    83,    84,    85,    73,     0,    74,   159,
       0,     0,     0,     0,    75,    76,    77,    78,    79,    80,
      81,     0,     0,    82,    83,    84,    85,    73,     0,    74,
     162,     0,     0,     0,     0,    75,    76,    77,    78,    79,
      80,    81,     0,     0,    82,    83,    84,    85,    73,   182,
      74,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,    80,    81,    73,     0,    82,    83,    84,    85,     0,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
      82,    83,    84,    85,    75,    76,    77,    78,    79,    80,
      81,     0,     0,    82,    83,    84,    85,    75,    76,    77,
      78,    79,    -1,    -1
};

static const yytype_int16 yycheck[] =
{
       2,    40,    52,     6,    24,    22,   167,    39,    74,    11,
       0,    43,    14,    30,    31,    23,    24,    19,    21,   179,
       6,    38,    24,    40,   185,    40,   186,    44,    45,    38,
      40,    40,    38,     6,    40,    44,    45,    40,    40,    45,
      60,    91,    41,    46,    43,    20,    21,    49,    98,    41,
       6,    43,    10,   103,    42,    23,    24,    25,    26,    27,
      28,    29,    42,    65,    32,    33,    34,    35,    23,    24,
      25,    26,    27,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,   150,    87,    20,    21,    90,    40,
      40,    39,    41,     6,     6,    97,     6,     8,    40,   101,
       6,    42,   141,    41,   143,    36,   156,    40,   110,    41,
     112,   161,    40,     6,    41,    41,    41,   119,   120,     3,
      41,     5,     6,     7,    64,     9,    10,    11,    12,    13,
      14,    36,    16,   154,    18,    19,    20,    21,   177,   141,
      24,   143,   116,   147,    -1,    -1,    30,    31,    -1,    -1,
      -1,   153,    36,    37,    38,    -1,    40,    -1,    42,    -1,
      -1,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,   171,
       3,    -1,     5,     6,     7,   177,     9,    10,    11,    12,
      13,    14,    -1,    16,    -1,    18,    19,    20,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    -1,
      -1,    -1,    -1,    36,    -1,    38,    -1,    40,     3,    42,
       5,     6,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    18,    19,    20,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    -1,     3,
      -1,     5,     6,    38,    -1,    40,    -1,    42,    -1,    -1,
      -1,    46,    16,    48,    18,    19,    20,    21,    -1,    -1,
      24,    -1,     3,    -1,     5,     6,    30,    31,    -1,    10,
      -1,    -1,    36,    -1,    38,    16,    40,    18,    19,    20,
      21,    -1,    46,    24,    48,     3,    -1,     5,     6,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    16,    40,
      18,    19,    20,    21,    -1,    46,    24,    48,    -1,    -1,
      -1,    -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    40,    15,    -1,    17,    -1,    -1,    46,    -1,
      48,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    15,    -1,
      17,    -1,    -1,    -1,    -1,    47,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,    33,    34,    35,    15,
      -1,    17,    -1,    -1,    -1,    -1,    43,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    32,    33,    34,    35,
      15,    -1,    17,    -1,    -1,    -1,    42,    -1,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    32,    33,    34,
      35,    15,    -1,    17,    -1,    -1,    -1,    42,    -1,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,    33,
      34,    35,    15,    -1,    17,    -1,    -1,    -1,    42,    -1,
      23,    24,    25,    26,    27,    28,    29,    -1,    -1,    32,
      33,    34,    35,    15,    -1,    17,    -1,    -1,    41,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,    33,    34,    35,    15,    -1,    17,    -1,    -1,    41,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    -1,
      -1,    32,    33,    34,    35,    15,    -1,    17,    -1,    -1,
      41,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    32,    33,    34,    35,    15,    -1,    17,    39,
      -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    33,    34,    35,    15,    -1,    17,
      39,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    -1,    -1,    32,    33,    34,    35,    15,    37,
      17,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    15,    -1,    32,    33,    34,    35,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,    33,    34,    35,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    33,    34,    35,    23,    24,    25,
      26,    27,    28,    29
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    52,    53,     0,     3,     5,     6,     7,     9,    10,
      11,    12,    13,    14,    16,    18,    19,    20,    21,    24,
      30,    31,    36,    38,    40,    42,    46,    48,    54,    55,
      56,    57,    58,    59,    60,    61,    67,    70,    72,    77,
      79,    81,    85,    87,    90,    91,    92,    93,    94,    40,
       6,    73,    40,    42,    55,    42,    42,    55,     6,    55,
      40,    59,    61,    59,    71,    36,    55,    65,    68,    69,
      55,    72,     6,    15,    17,    23,    24,    25,    26,    27,
      28,    29,    32,    33,    34,    35,    42,    22,    30,    31,
      38,    40,    44,    45,    62,    63,    64,    38,    40,    45,
      54,    40,    86,    88,    55,    75,    40,    65,    42,    72,
      53,    55,    43,    66,    39,    39,    43,    41,    41,    89,
      89,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    65,     6,     6,    55,    65,     6,
       8,    80,    55,    82,    84,    65,    41,    40,     6,    78,
      42,    41,    37,    47,    55,    69,    40,    55,    55,    39,
      41,    40,    39,    41,    54,    41,    54,    41,    78,    41,
      43,    89,    55,    66,    65,    65,    83,    88,    41,    74,
       6,    55,    37,    41,    41,    54,    76,    70,    42,    88,
      70
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    52,    53,    53,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      56,    56,    56,    56,    56,    56,    56,    56,    57,    58,
      58,    58,    58,    58,    59,    59,    59,    59,    60,    60,
      60,    60,    61,    61,    61,    62,    62,    63,    64,    65,
      65,    66,    66,    67,    67,    68,    68,    69,    71,    70,
      73,    74,    72,    75,    76,    72,    77,    77,    77,    77,
      77,    77,    78,    78,    78,    79,    80,    81,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    94
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     1,
       3,     2,     2,     2,     2,     2,     2,     1,     3,     1,
       1,     1,     3,     1,     1,     2,     2,     1,     3,     4,
       3,     4,     2,     4,     6,     1,     1,     3,     5,     0,
       2,     3,     0,     3,     3,     1,     3,     5,     0,     4,
       0,     0,     7,     0,     0,     8,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     4,     1,     2,     4,     0,
       0,     3,     1,     3,     3,     0,     0,     7,     7,     2,
       2,     2,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: stmt_list  */
#line 109 "parser.y"
                    { DEBUG_REDUCE("program -> stmt_list"); }
#line 1404 "parser.cpp"
    break;

  case 3: /* stmt_list: stmt_list stmt  */
#line 113 "parser.y"
                           {
                             (yyval.s) = stmt_list_handler((yyvsp[-1].s),(yyvsp[0].s));
                             DEBUG_REDUCE("stmt -> expr ;"); 
                           }
#line 1413 "parser.cpp"
    break;

  case 4: /* stmt_list: %empty  */
#line 117 "parser.y"
              {(yyval.s) = new stmt_t(); make_stmt((yyval.s));DEBUG_REDUCE("stmt list  -> empty ;"); }
#line 1419 "parser.cpp"
    break;

  case 5: /* stmt: expr SEMICOLON  */
#line 121 "parser.y"
                          {  
                            (yyval.s) = new stmt_t();
                            make_stmt((yyval.s));
                            DEBUG_REDUCE("stmt -> expr ;"); 
                          }
#line 1429 "parser.cpp"
    break;

  case 6: /* stmt: ifstmt  */
#line 126 "parser.y"
                          {(yyval.s) = (yyvsp[0].s); DEBUG_REDUCE("stmt -> ifstmt"); }
#line 1435 "parser.cpp"
    break;

  case 7: /* stmt: whilestmt  */
#line 127 "parser.y"
                          {(yyval.s) = (yyvsp[0].s); DEBUG_REDUCE("stmt -> whilestmt"); }
#line 1441 "parser.cpp"
    break;

  case 8: /* stmt: forstmt  */
#line 128 "parser.y"
                          {(yyval.s) = (yyvsp[0].s); DEBUG_REDUCE("stmt -> forstmt"); }
#line 1447 "parser.cpp"
    break;

  case 9: /* stmt: returnstmt  */
#line 129 "parser.y"
                          {(yyval.s) = (yyvsp[0].s); DEBUG_REDUCE("stmt -> returnstmt"); }
#line 1453 "parser.cpp"
    break;

  case 10: /* stmt: Break  */
#line 130 "parser.y"
                          { DEBUG_REDUCE("stmt -> break ;"); }
#line 1459 "parser.cpp"
    break;

  case 11: /* stmt: Continue  */
#line 131 "parser.y"
                          { DEBUG_REDUCE("stmt -> continue ;"); }
#line 1465 "parser.cpp"
    break;

  case 12: /* stmt: block  */
#line 132 "parser.y"
                          {(yyval.s) = (yyvsp[0].s); DEBUG_REDUCE("stmt -> block"); }
#line 1471 "parser.cpp"
    break;

  case 13: /* stmt: funcdef  */
#line 133 "parser.y"
                          {
                            (yyval.s) = new stmt_t();
                            make_stmt((yyval.s)); 
                            DEBUG_REDUCE("stmt -> funcdef"); 
                          }
#line 1481 "parser.cpp"
    break;

  case 14: /* stmt: SEMICOLON  */
#line 138 "parser.y"
                          {DEBUG_REDUCE("stmt -> ;"); }
#line 1487 "parser.cpp"
    break;

  case 15: /* expr: assignexpr  */
#line 145 "parser.y"
                                              { (yyval.exprVal) = (yyvsp[0].exprVal); DEBUG_REDUCE("expr -> assignexpr"); }
#line 1493 "parser.cpp"
    break;

  case 16: /* expr: expr PLUS expr  */
#line 146 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr + expr");  (yyval.exprVal) = emit_arith_op(add, (yyvsp[-2].exprVal), (yyvsp[0].exprVal)); }
#line 1499 "parser.cpp"
    break;

  case 17: /* expr: expr MINUS expr  */
#line 147 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr - expr");   (yyval.exprVal) = emit_arith_op(sub, (yyvsp[-2].exprVal), (yyvsp[0].exprVal)); }
#line 1505 "parser.cpp"
    break;

  case 18: /* expr: expr MULT expr  */
#line 148 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr * expr");   (yyval.exprVal) = emit_arith_op(mul, (yyvsp[-2].exprVal), (yyvsp[0].exprVal)); }
#line 1511 "parser.cpp"
    break;

  case 19: /* expr: expr DIV expr  */
#line 149 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr / expr");   (yyval.exprVal) = emit_arith_op(divv, (yyvsp[-2].exprVal), (yyvsp[0].exprVal)); }
#line 1517 "parser.cpp"
    break;

  case 20: /* expr: expr MOD expr  */
#line 150 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr % expr");   (yyval.exprVal) = emit_arith_op(mod, (yyvsp[-2].exprVal), (yyvsp[0].exprVal)); }
#line 1523 "parser.cpp"
    break;

  case 21: /* expr: expr GREATER expr  */
#line 151 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr > expr");   
                                                
                                                    expr* left = boolify_expr((yyvsp[-2].exprVal));
                                                    expr* right = boolify_expr((yyvsp[0].exprVal));

                                                    (yyval.exprVal) = newexpr(boolexpr_e);
                                                    (yyval.exprVal)->sym = newtemp();

                                                    (yyval.exprVal)->truelist.push_back(nextquad());
                                                    (yyval.exprVal)->falselist.push_back(nextquad() + 1);

                                                    emit(if_greater, left, right, NULL, 0, yylineno);
                                                    emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 1542 "parser.cpp"
    break;

  case 22: /* expr: expr GREATER_EQUAL expr  */
#line 166 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr >= expr");   
                                                (yyval.exprVal) = newexpr(boolexpr_e);
                                                (yyval.exprVal)->sym = newtemp();
                                                (yyval.exprVal)->truelist.push_back(nextquad());
                                                (yyval.exprVal)->falselist.push_back(nextquad() + 1);
                                                emit(if_greatereq, (yyvsp[-2].exprVal), (yyvsp[0].exprVal), NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }
#line 1554 "parser.cpp"
    break;

  case 23: /* expr: expr LESS expr  */
#line 174 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr < expr");   
		                                            expr* left = boolify_expr((yyvsp[-2].exprVal));
                                                    expr* right = boolify_expr((yyvsp[0].exprVal));

                                                    (yyval.exprVal) = newexpr(boolexpr_e);
                                                    (yyval.exprVal)->sym = newtemp();

                                                    (yyval.exprVal)->truelist.push_back(nextquad());
                                                    (yyval.exprVal)->falselist.push_back(nextquad() + 1);

                                                    emit(if_less, left, right, NULL, 0, yylineno);
                                                    emit(jump, NULL, NULL, NULL, 0, yylineno);
                                                    
                                                }
#line 1573 "parser.cpp"
    break;

  case 24: /* expr: expr LESS_EQUAL expr  */
#line 188 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr <= expr");  
                                                (yyval.exprVal) = newexpr(boolexpr_e);
                                                (yyval.exprVal)->sym = newtemp();
                                                (yyval.exprVal)->truelist.push_back(nextquad());
                                                (yyval.exprVal)->falselist.push_back(nextquad() + 1);
                                                emit(if_lesseq, (yyvsp[-2].exprVal), (yyvsp[0].exprVal), NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }
#line 1585 "parser.cpp"
    break;

  case 25: /* expr: expr EQUAL expr  */
#line 196 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr == expr"); 
                                                (yyval.exprVal) = newexpr(boolexpr_e);
                                                (yyval.exprVal)->sym = newtemp();
                                                (yyval.exprVal)->truelist.push_back(nextquad());
                                                (yyval.exprVal)->falselist.push_back(nextquad() + 1);
                                                emit(if_eq, (yyvsp[-2].exprVal), (yyvsp[0].exprVal), NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }
#line 1597 "parser.cpp"
    break;

  case 26: /* expr: expr NOT_EQUALS expr  */
#line 204 "parser.y"
                                              { DEBUG_REDUCE("expr -> expr != expr");  
                                                (yyval.exprVal) = newexpr(boolexpr_e);
                                                (yyval.exprVal)->sym = newtemp();
                                                (yyval.exprVal)->truelist.push_back(nextquad());
                                                (yyval.exprVal)->falselist.push_back(nextquad() + 1);
                                                emit(if_noteq, (yyvsp[-2].exprVal), (yyvsp[0].exprVal), NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }
#line 1609 "parser.cpp"
    break;

  case 27: /* expr: expr AND M expr  */
#line 212 "parser.y"
                                                { DEBUG_REDUCE("expr -> expr and expr");
                                                    (yyvsp[-3].exprVal) = to_boolexpr((yyvsp[-3].exprVal));  // Ensure expr1 is boolexpr_e
                                                    (yyvsp[0].exprVal) = to_boolexpr((yyvsp[0].exprVal));  // Ensure expr2 is boolexpr_e
                                                    backpatch((yyvsp[-3].exprVal)->truelist, (unsigned)(yyvsp[-1].exprVal)->numConst);
                                                    (yyval.exprVal) = newexpr(boolexpr_e);
                                                    (yyval.exprVal)->truelist = (yyvsp[0].exprVal)->truelist;
                                                    (yyval.exprVal)->falselist = merge((yyvsp[-3].exprVal)->falselist, (yyvsp[0].exprVal)->falselist);
}
#line 1622 "parser.cpp"
    break;

  case 28: /* expr: expr OR M expr  */
#line 221 "parser.y"
                                                {
                                                   DEBUG_REDUCE("expr -> expr or expr");
                                                    (yyvsp[-3].exprVal) = to_boolexpr((yyvsp[-3].exprVal));
                                                    (yyvsp[0].exprVal) = to_boolexpr((yyvsp[0].exprVal));
                                                    backpatch((yyvsp[-3].exprVal)->falselist, (unsigned)(yyvsp[-1].exprVal)->numConst);
                                                    (yyval.exprVal) = newexpr(boolexpr_e);
                                                    (yyval.exprVal)->truelist = merge((yyvsp[-3].exprVal)->truelist, (yyvsp[0].exprVal)->truelist);
                                                    (yyval.exprVal)->falselist = (yyvsp[0].exprVal)->falselist;
}
#line 1636 "parser.cpp"
    break;

  case 29: /* expr: term  */
#line 230 "parser.y"
                                              {  (yyval.exprVal) = (yyvsp[0].exprVal); DEBUG_REDUCE("expr -> term"); (yyval.exprVal) = (yyvsp[0].exprVal); }
#line 1642 "parser.cpp"
    break;

  case 30: /* term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 235 "parser.y"
        { (yyval.exprVal) = (yyvsp[-1].exprVal); 
        // Pass the expr unchanged
        DEBUG_REDUCE("term -> (expr)"); }
#line 1650 "parser.cpp"
    break;

  case 31: /* term: MINUS expr  */
#line 238 "parser.y"
                             {
        DEBUG_REDUCE("term -> -expr");
      
        (yyval.exprVal) = newexpr(arithexpr_e);
        (yyval.exprVal)->sym = newtemp();
        emit(uminus, newexpr_constnum(0), (yyvsp[0].exprVal), (yyval.exprVal), 0, yylineno);}
#line 1661 "parser.cpp"
    break;

  case 32: /* term: NOT expr  */
#line 246 "parser.y"
        { DEBUG_REDUCE("term -> not expr");
        (yyvsp[0].exprVal) = to_boolexpr((yyvsp[0].exprVal));
        (yyval.exprVal) = newexpr(boolexpr_e);
        (yyval.exprVal)->sym = newtemp();
        
        (yyval.exprVal)->truelist = (yyvsp[0].exprVal)->falselist;  // NOT swaps true and false lists
        (yyval.exprVal)->falselist = (yyvsp[0].exprVal)->truelist;
    }
#line 1674 "parser.cpp"
    break;

  case 33: /* term: PLUS_PLUS lvalue  */
#line 255 "parser.y"
        {temrs_error((yyvsp[0].exprVal),"++");
        if((yyvsp[0].exprVal)->type == tableitem_e) {
													(yyval.exprVal) = emit_iftableitem((yyvsp[0].exprVal));
													emit(add, (yyval.exprVal), newexpr_constnum(1), (yyval.exprVal), nextquad(), yylineno);
													emit(add, (yyvsp[0].exprVal), (yyvsp[0].exprVal)->index, (yyval.exprVal), nextquad(), yylineno);
												}
									else {
													emit(add, (yyvsp[0].exprVal), newexpr_constnum(1), (yyvsp[0].exprVal),nextquad(), yylineno);
													(yyval.exprVal) = newexpr(arithexpr_e);
													(yyval.exprVal)->sym = newtemp();
													emit(assign, (yyval.exprVal), (yyvsp[0].exprVal), NULL, nextquad(), yylineno);
												}
                                                 DEBUG_REDUCE("term -> ++lvalue"); }
#line 1692 "parser.cpp"
    break;

  case 34: /* term: lvalue PLUS_PLUS  */
#line 269 "parser.y"
        {temrs_error((yyvsp[-1].exprVal),"++"); 
     
        (yyval.exprVal) = newexpr(arithexpr_e);
        (yyval.exprVal)->sym = newtemp();
        if ((yyvsp[-1].exprVal)->type == tableitem_e) {
            (yyval.exprVal) = emit_iftableitem((yyvsp[-1].exprVal)); // Get current value
            expr* newval = newexpr(arithexpr_e);
            newval->sym = newtemp();
            emit(add, (yyval.exprVal), newexpr_constnum(1), newval, 0, yylineno); // Compute new value
            emit(tablesetelem, (yyvsp[-1].exprVal), (yyvsp[-1].exprVal)->index, newval, 0, yylineno); // Update table
        } else {
            emit(assign, (yyvsp[-1].exprVal), NULL, (yyval.exprVal), 0, yylineno); // Copy current value
            emit(add, (yyvsp[-1].exprVal), newexpr_constnum(1), (yyvsp[-1].exprVal), 0, yylineno); // Update lvalue
        }
        DEBUG_REDUCE("term -> lvalue++"); }
#line 1712 "parser.cpp"
    break;

  case 35: /* term: MINUS_MINUS lvalue  */
#line 285 "parser.y"
        {temrs_error((yyvsp[0].exprVal),"--"); 
        (yyval.exprVal) = newexpr(arithexpr_e);
        (yyval.exprVal)->sym = newtemp();
        if ((yyvsp[0].exprVal)->type == tableitem_e) {
            expr* val = emit_iftableitem((yyvsp[0].exprVal));
            emit(sub, val, newexpr_constnum(1), (yyval.exprVal), 0, yylineno);
            emit(tablesetelem, (yyvsp[0].exprVal), (yyvsp[0].exprVal)->index, (yyval.exprVal), 0, yylineno);
        } else {
            emit(sub, (yyvsp[0].exprVal), newexpr_constnum(1), (yyvsp[0].exprVal), 0, yylineno);
            emit(assign, (yyvsp[0].exprVal), NULL, (yyval.exprVal), 0, yylineno);
        }
        DEBUG_REDUCE("term -> --lvalue"); }
#line 1729 "parser.cpp"
    break;

  case 36: /* term: lvalue MINUS_MINUS  */
#line 298 "parser.y"
        {temrs_error((yyvsp[-1].exprVal),"--"); 
        
        (yyval.exprVal) = newexpr(arithexpr_e);
        (yyval.exprVal)->sym = newtemp();
        if ((yyvsp[-1].exprVal)->type == tableitem_e) {
            (yyval.exprVal) = emit_iftableitem((yyvsp[-1].exprVal));
            expr* newval = newexpr(arithexpr_e);
            newval->sym = newtemp();
            emit(sub, (yyval.exprVal), newexpr_constnum(1), newval, 0, yylineno);
            emit(tablesetelem, (yyvsp[-1].exprVal), (yyvsp[-1].exprVal)->index, newval, 0, yylineno);
        } else {
            emit(assign, (yyvsp[-1].exprVal), NULL, (yyval.exprVal), 0, yylineno);
            emit(sub, (yyvsp[-1].exprVal), newexpr_constnum(1), (yyvsp[-1].exprVal), 0, yylineno);
        }
        DEBUG_REDUCE("term -> lvalue--"); }
#line 1749 "parser.cpp"
    break;

  case 37: /* term: primary  */
#line 314 "parser.y"
        { DEBUG_REDUCE("term -> primary"); 
        (yyval.exprVal) = (yyvsp[0].exprVal);}
#line 1756 "parser.cpp"
    break;

  case 38: /* assignexpr: lvalue ASSIGN expr  */
#line 321 "parser.y"
    {
        assign_error((yyvsp[-2].exprVal));
        DEBUG_REDUCE("assignexpr -> lvalue = expr");

        expr* rval = (yyvsp[0].exprVal);

        // Convert boolean expressions to true/false if needed
        if (rval->type == boolexpr_e) {
            rval = boolify_expr(rval);
        }

        if ((yyvsp[-2].exprVal)->type == tableitem_e) {
            // Handle table elements (unchanged)
            emit(tablesetelem, (yyvsp[-2].exprVal), (yyvsp[-2].exprVal)->index, rval, -1, yylineno);
            (yyval.exprVal) = emit_iftableitem((yyvsp[-2].exprVal));
            (yyval.exprVal)->type = assignexpr_e;
        } else {
            // Assign rval to lvalue
            emit(assign, rval, NULL, (yyvsp[-2].exprVal), 0, yylineno);
            // Create a temporary to hold lvalue's value
            expr* temp = newexpr(var_e);
            temp->sym = newtemp();
            emit(assign, temp, (yyvsp[-2].exprVal), NULL, 0, yylineno);
            (yyval.exprVal) = temp;
        }
    }
#line 1787 "parser.cpp"
    break;

  case 39: /* primary: lvalue  */
#line 349 "parser.y"
                                            { DEBUG_REDUCE("primary -> lvalue");    }
#line 1793 "parser.cpp"
    break;

  case 40: /* primary: call  */
#line 350 "parser.y"
                                            { DEBUG_REDUCE("primary -> call");      }
#line 1799 "parser.cpp"
    break;

  case 41: /* primary: objectdef  */
#line 351 "parser.y"
                                            { DEBUG_REDUCE("primary -> objectdef"); }
#line 1805 "parser.cpp"
    break;

  case 42: /* primary: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS  */
#line 353 "parser.y"
                                            { DEBUG_REDUCE("primary -> (funcdef)"); }
#line 1811 "parser.cpp"
    break;

  case 43: /* primary: const  */
#line 354 "parser.y"
                                            { DEBUG_REDUCE("primary -> const");     }
#line 1817 "parser.cpp"
    break;

  case 44: /* lvalue: IDENT  */
#line 358 "parser.y"
                                            {SymbolTableEntry_T entry = add_ident(*(yyvsp[0].stringValue)); (yyval.exprVal) = symEntr_to_expr(entry); DEBUG_REDUCE("lvalue -> IDENT"); }
#line 1823 "parser.cpp"
    break;

  case 45: /* lvalue: LOCAL IDENT  */
#line 359 "parser.y"
                                            {SymbolTableEntry_T entry = add_local_dent(*(yyvsp[0].stringValue)); (yyval.exprVal) = symEntr_to_expr(entry);  DEBUG_REDUCE("lvalue -> local IDENT"); }
#line 1829 "parser.cpp"
    break;

  case 46: /* lvalue: NAMESPACE IDENT  */
#line 360 "parser.y"
                                            {SymbolTableEntry_T entry = handle_namespace_dent(*(yyvsp[0].stringValue)); (yyval.exprVal) = symEntr_to_expr(entry); DEBUG_REDUCE("lvalue -> ::IDENT"); }
#line 1835 "parser.cpp"
    break;

  case 47: /* lvalue: member  */
#line 361 "parser.y"
                                            {(yyval.exprVal) = (yyvsp[0].exprVal);  DEBUG_REDUCE("lvalue -> member"); }
#line 1841 "parser.cpp"
    break;

  case 48: /* member: lvalue DOT IDENT  */
#line 365 "parser.y"
                                                {(yyval.exprVal) = lvalue_id_handler((yyvsp[-2].exprVal), *(yyvsp[0].stringValue)); DEBUG_REDUCE("member -> lvalue . IDENT"); }
#line 1847 "parser.cpp"
    break;

  case 49: /* member: lvalue LEFT_BRACKET expr RIGHT_BRACKET  */
#line 366 "parser.y"
                                                {(yyval.exprVal) = member_handler((yyvsp[-3].exprVal), (yyvsp[-1].exprVal)); DEBUG_REDUCE("member -> lvalue [expr]");     }
#line 1853 "parser.cpp"
    break;

  case 50: /* member: call DOT IDENT  */
#line 367 "parser.y"
                                                {(yyval.exprVal) = member_item((yyvsp[-2].exprVal), *(yyvsp[0].stringValue)); DEBUG_REDUCE("member -> call . IDENT");         }
#line 1859 "parser.cpp"
    break;

  case 51: /* member: call LEFT_BRACKET expr RIGHT_BRACKET  */
#line 368 "parser.y"
                                                {(yyval.exprVal) = member_handler((yyvsp[-3].exprVal), (yyvsp[-1].exprVal)); DEBUG_REDUCE("member -> call [expr]");       }
#line 1865 "parser.cpp"
    break;

  case 52: /* call: lvalue callsuffix  */
#line 372 "parser.y"
                            {
                                (yyvsp[-1].exprVal) = emit_iftableitem((yyvsp[-1].exprVal));
                                expr* funcToCall = (yyvsp[-1].exprVal);
                                if((yyvsp[0].callVal)->method){
                                    expr* self = newexpr(var_e);
                                    self = member_item((yyvsp[-1].exprVal), (yyvsp[0].callVal)->name);
                                    self->next = (yyvsp[0].callVal)->elist;
                                    (yyvsp[0].callVal)->elist = self;
                                }

                                (yyval.exprVal) = call_handler(funcToCall, (yyvsp[0].callVal)->elist);
                                DEBUG_REDUCE("call -> lvalue callsuffix"); 
                            }
#line 1883 "parser.cpp"
    break;

  case 53: /* call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  */
#line 385 "parser.y"
                                                    {(yyval.exprVal) = call_handler((yyvsp[-3].exprVal), (yyvsp[-1].exprVal)); DEBUG_REDUCE("call -> call(elist)"); }
#line 1889 "parser.cpp"
    break;

  case 54: /* call: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  */
#line 387 "parser.y"
                                           {(yyval.exprVal) = anonym_call((yyvsp[-4].symEntry), (yyvsp[-1].exprVal)); DEBUG_REDUCE("call -> (funcdef)(elist)"); }
#line 1895 "parser.cpp"
    break;

  case 55: /* callsuffix: normcall  */
#line 391 "parser.y"
                                            {(yyval.callVal) = (yyvsp[0].callVal); DEBUG_REDUCE("callsuffix -> normcall"); }
#line 1901 "parser.cpp"
    break;

  case 56: /* callsuffix: methodcall  */
#line 392 "parser.y"
                                            {(yyval.callVal) = (yyvsp[0].callVal); DEBUG_REDUCE("callsuffix -> methodcall"); }
#line 1907 "parser.cpp"
    break;

  case 57: /* normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  */
#line 396 "parser.y"
                                              {(yyval.callVal) = normcall_handler((yyvsp[-1].exprVal));DEBUG_REDUCE("normcall -> (elist)");}
#line 1913 "parser.cpp"
    break;

  case 58: /* methodcall: DOUBLE_DOT IDENT LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  */
#line 401 "parser.y"
                                           {(yyval.callVal) = methodcall_handler((yyvsp[-1].exprVal), *(yyvsp[-3].stringValue)); DEBUG_REDUCE("methodcall -> ::IDENT(elist)"); }
#line 1919 "parser.cpp"
    break;

  case 59: /* elist: %empty  */
#line 405 "parser.y"
                                            {(yyval.exprVal) = nullptr; DEBUG_REDUCE("elist -> empty"); }
#line 1925 "parser.cpp"
    break;

  case 60: /* elist: expr elist_tail  */
#line 406 "parser.y"
                                            {(yyvsp[-1].exprVal)->next = (yyvsp[0].exprVal); (yyval.exprVal) = (yyvsp[-1].exprVal); DEBUG_REDUCE("elist -> expr"); }
#line 1931 "parser.cpp"
    break;

  case 61: /* elist_tail: COMMA expr elist_tail  */
#line 409 "parser.y"
                            {
          (yyvsp[-1].exprVal)->next = (yyvsp[0].exprVal);
          (yyval.exprVal) = (yyvsp[-1].exprVal);
      }
#line 1940 "parser.cpp"
    break;

  case 62: /* elist_tail: %empty  */
#line 413 "parser.y"
                  {
          (yyval.exprVal) = nullptr;
      }
#line 1948 "parser.cpp"
    break;

  case 63: /* objectdef: LEFT_BRACKET elist RIGHT_BRACKET  */
#line 419 "parser.y"
                                               { DEBUG_REDUCE("objectdef -> {elist}"); }
#line 1954 "parser.cpp"
    break;

  case 64: /* objectdef: LEFT_BRACKET indexed RIGHT_BRACKET  */
#line 420 "parser.y"
                                                   { DEBUG_REDUCE("objectdef -> indexed "); }
#line 1960 "parser.cpp"
    break;

  case 65: /* indexed: indexedelem  */
#line 424 "parser.y"
                                            {  DEBUG_REDUCE("indexed -> indexedelem"); }
#line 1966 "parser.cpp"
    break;

  case 66: /* indexed: indexed COMMA indexedelem  */
#line 425 "parser.y"
                                           {  DEBUG_REDUCE("indexed -> indexed , indexedelem"); }
#line 1972 "parser.cpp"
    break;

  case 67: /* indexedelem: LEFT_BRACE expr COLON expr RIGHT_BRACE  */
#line 430 "parser.y"
                                           { DEBUG_REDUCE("indexedelem -> [expr : expr]"); }
#line 1978 "parser.cpp"
    break;

  case 68: /* $@1: %empty  */
#line 434 "parser.y"
                 { scope++; }
#line 1984 "parser.cpp"
    break;

  case 69: /* block: LEFT_BRACE $@1 stmt_list RIGHT_BRACE  */
#line 434 "parser.y"
                                                    {(yyval.s)=(yyvsp[-1].s); exit_block(); DEBUG_REDUCE("block -> {stmt_list}"); }
#line 1990 "parser.cpp"
    break;

  case 70: /* $@2: %empty  */
#line 439 "parser.y"
               {enter_func(0, "");}
#line 1996 "parser.cpp"
    break;

  case 71: /* $@3: %empty  */
#line 440 "parser.y"
                                                      {scope--;add_anon_function(args);}
#line 2002 "parser.cpp"
    break;

  case 72: /* funcdef: FUNCTION $@2 LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS $@3 block  */
#line 441 "parser.y"
                  {exit_func(0, "", (yyvsp[0].s)->returnList); DEBUG_REDUCE("funcdef -> function(idlist) block"); }
#line 2008 "parser.cpp"
    break;

  case 73: /* $@4: %empty  */
#line 443 "parser.y"
                     {enter_func(1, *(yyvsp[0].stringValue));}
#line 2014 "parser.cpp"
    break;

  case 74: /* $@5: %empty  */
#line 445 "parser.y"
            {scope--; add_function(*(yyvsp[-4].stringValue), args);}
#line 2020 "parser.cpp"
    break;

  case 75: /* funcdef: FUNCTION IDENT $@4 LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS $@5 block  */
#line 447 "parser.y"
            {exit_func(1, *(yyvsp[-6].stringValue), (yyvsp[0].s)->returnList); DEBUG_REDUCE("funcdef -> function IDENT(idlist) block"); }
#line 2026 "parser.cpp"
    break;

  case 76: /* const: INTEGER  */
#line 452 "parser.y"
                  { (yyval.exprVal) = newexpr(constnum_e); (yyval.exprVal)->numConst = (yyvsp[0].intValue);  DEBUG_REDUCE("const -> INTEGER"); }
#line 2032 "parser.cpp"
    break;

  case 77: /* const: REALCONST  */
#line 453 "parser.y"
                  { (yyval.exprVal) = newexpr(constnum_e); (yyval.exprVal)->numConst = (yyvsp[0].doubleVal);  DEBUG_REDUCE("const -> REALCONST"); }
#line 2038 "parser.cpp"
    break;

  case 78: /* const: STRING  */
#line 454 "parser.y"
                  { (yyval.exprVal) = newexpr_conststring(*(yyvsp[0].stringValue)); DEBUG_REDUCE("const -> STRING"); }
#line 2044 "parser.cpp"
    break;

  case 79: /* const: NIL  */
#line 455 "parser.y"
                  { (yyval.exprVal) = newexpr(nil_e); DEBUG_REDUCE("const -> NIL"); }
#line 2050 "parser.cpp"
    break;

  case 80: /* const: TRUE  */
#line 456 "parser.y"
                  { (yyval.exprVal) = newexpr(constbool_e); (yyval.exprVal)->boolConst = true; DEBUG_REDUCE("const -> TRUE"); }
#line 2056 "parser.cpp"
    break;

  case 81: /* const: FALSE  */
#line 457 "parser.y"
                  { (yyval.exprVal) = newexpr(constbool_e); (yyval.exprVal)->boolConst = false; DEBUG_REDUCE("const -> FALSE"); }
#line 2062 "parser.cpp"
    break;

  case 82: /* idlist: IDENT  */
#line 461 "parser.y"
          {
        args=handle_func_args(args,*(yyvsp[0].stringValue));
        DEBUG_REDUCE("idlist -> IDENT");
    }
#line 2071 "parser.cpp"
    break;

  case 83: /* idlist: idlist COMMA IDENT  */
#line 465 "parser.y"
                       {
        args = handle_func_args(args,*(yyvsp[0].stringValue));
        DEBUG_REDUCE("idlist -> idlist , IDENT");
    }
#line 2080 "parser.cpp"
    break;

  case 84: /* idlist: %empty  */
#line 469 "parser.y"
    {}
#line 2086 "parser.cpp"
    break;

  case 85: /* ifprefix: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 473 "parser.y"
                                             {
    
    emit(if_eq, newexpr_bool('1'), (yyvsp[-1].exprVal), newexpr_constnum(nextquad() + 2), currQuad, yylineno);

    (yyval.exprVal) = newexpr(constnum_e);
    (yyval.exprVal)->numConst = nextquad();

    emit(jump, NULL, NULL, 0, currQuad, yylineno); 

   
}
#line 2102 "parser.cpp"
    break;

  case 86: /* elseprefix: ELSE  */
#line 487 "parser.y"
       {

    (yyval.exprVal) = newexpr(constnum_e);

    (yyval.exprVal)->numConst = nextquad();

    emit(jump, NULL, NULL, 0, currQuad, yylineno); 

}
#line 2116 "parser.cpp"
    break;

  case 87: /* ifstmt: ifprefix stmt  */
#line 499 "parser.y"
                {

      patchlabel((int)(yyvsp[-1].exprVal)->numConst, nextquad());
     
  }
#line 2126 "parser.cpp"
    break;

  case 88: /* ifstmt: ifprefix stmt elseprefix stmt  */
#line 504 "parser.y"
                                {

      patchlabel((int)(yyvsp[-3].exprVal)->numConst, (int)(yyvsp[-1].exprVal)->numConst + 1);

      patchlabel((int)(yyvsp[-1].exprVal)->numConst, nextquad());
     
  }
#line 2138 "parser.cpp"
    break;

  case 89: /* loopstart: %empty  */
#line 513 "parser.y"
                                   {++loopcounter;}
#line 2144 "parser.cpp"
    break;

  case 90: /* loopend: %empty  */
#line 515 "parser.y"
                                   {--loopcounter;}
#line 2150 "parser.cpp"
    break;

  case 91: /* loopstmt: loopstart stmt loopend  */
#line 517 "parser.y"
                                   {(yyval.s) = (yyvsp[-1].s);}
#line 2156 "parser.cpp"
    break;

  case 92: /* whilestart: WHILE  */
#line 520 "parser.y"
                   {
   
    (yyval.exprVal) = newexpr(constnum_e);
    (yyval.exprVal)->numConst = nextquad();
}
#line 2166 "parser.cpp"
    break;

  case 93: /* whilecond: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 527 "parser.y"
                                                    {
   
    emit(if_eq, (yyvsp[-1].exprVal), newexpr_bool('1'), NULL, nextquad()+2, yylineno); //assign true
    

    (yyval.exprVal) = newexpr(constnum_e);
    (yyval.exprVal)->numConst = nextquad();
  
    emit(jump, NULL, NULL, 0,0,yylineno);
    
}
#line 2182 "parser.cpp"
    break;

  case 94: /* whilestmt: whilestart whilecond loopstmt  */
#line 541 "parser.y"
                                  {

        make_stmt((yyvsp[0].s));

        emit(jump, NULL, NULL, (yyvsp[-2].exprVal),(int)(yyvsp[-2].exprVal)->numConst, yylineno);
        
        patchlabel((int)(yyvsp[-1].exprVal)->numConst, nextquad());

       
        patchlist((yyvsp[0].s)->breakList, nextquad());
        
        patchlist((yyvsp[0].s)->contList, nextquad());
        
    }
#line 2201 "parser.cpp"
    break;

  case 95: /* N: %empty  */
#line 557 "parser.y"
    {
    (yyval.exprVal) = newexpr(constnum_e);
    (yyval.exprVal)->numConst = nextquad();
    emit(jump,NULL,NULL,0,-1,yylineno);
}
#line 2211 "parser.cpp"
    break;

  case 96: /* M: %empty  */
#line 564 "parser.y"
    {
    (yyval.exprVal) = newexpr(constnum_e);
    (yyval.exprVal)->numConst = nextquad();
}
#line 2220 "parser.cpp"
    break;

  case 97: /* forprefix: FOR LEFT_PARENTHESIS elist SEMICOLON M expr SEMICOLON  */
#line 570 "parser.y"
                                                                  {

    make_loop_t((yyval.loop_t));

    (yyval.loop_t)->test = (yyvsp[-2].exprVal)->numConst;
    (yyval.loop_t)->enter = nextquad();

    emit(if_eq,(yyvsp[-1].exprVal),newexpr_bool('1'),0,-1,yylineno);

}
#line 2235 "parser.cpp"
    break;

  case 98: /* forstmt: forprefix N elist RIGHT_PARENTHESIS N stmt N  */
#line 582 "parser.y"
                                                      {

    patchlabel((yyvsp[-6].loop_t)->enter,(int)(yyvsp[-2].exprVal)->numConst+1);
    patchlabel((int)(yyvsp[-5].exprVal)->numConst,nextquad());
    patchlabel((int)(yyvsp[-2].exprVal)->numConst,(yyvsp[-6].loop_t)->test);
    patchlabel((int)(yyvsp[0].exprVal)->numConst,(int)(yyvsp[-5].exprVal)->numConst+1);

    make_stmt((yyvsp[-1].s));

    patchlist((yyvsp[-1].s)->breakList,nextquad());
    patchlist((yyvsp[-1].s)->contList,(int)(yyvsp[-5].exprVal)->numConst + 1);

}
#line 2253 "parser.cpp"
    break;

  case 99: /* Break: BREAK SEMICOLON  */
#line 597 "parser.y"
                        {

    if(loopcounter == 0) {
        yyerror("break statement is outside of the loop ");
        (yyval.s) = new stmt_t();  
        make_stmt((yyval.s));
    } else {
        (yyval.s) = new stmt_t();  
        make_stmt((yyval.s));
        (yyval.s)->breakList = nextquad();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
    }
}
#line 2271 "parser.cpp"
    break;

  case 100: /* Continue: CONTINUE SEMICOLON  */
#line 613 "parser.y"
                              { 
  if(loopcounter == 0) {
        yyerror("continue statement is outside of the loop ");
        (yyval.s) = new stmt_t();
        make_stmt((yyval.s));
    } else {
        (yyval.s) = new stmt_t();  
        make_stmt((yyval.s));
        (yyval.s)->contList = nextquad();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
    }
}
#line 2288 "parser.cpp"
    break;

  case 101: /* returnstmt: RETURN SEMICOLON  */
#line 628 "parser.y"
        {  
            emit(ret, nullptr, nullptr, nullptr, 0, nextquadlabel());
            (yyval.s) = new stmt_t();
            make_stmt((yyval.s));
            (yyval.s)->returnList = nextquadlabel() - 1;
            DEBUG_REDUCE("returnstmt -> return ;");
        }
#line 2300 "parser.cpp"
    break;

  case 102: /* returnstmt: RETURN expr SEMICOLON  */
#line 636 "parser.y"
        {
            emit(ret, (yyvsp[-1].exprVal), nullptr, nullptr, 0, nextquadlabel());
            (yyval.s) = new stmt_t();
            make_stmt((yyval.s));
            (yyval.s)->returnList = nextquadlabel() - 1;
            DEBUG_REDUCE("returnstmt -> return expr ;"); 
        }
#line 2312 "parser.cpp"
    break;


#line 2316 "parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 644 "parser.y"


int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* inputFile = fopen(argv[1], "r");
        if (!inputFile) {
            perror("Failed to open file");
            return 1;
        }
        yyin = inputFile;
    }
    init_tables();
    push_loopcounter();
    yyparse();

    //print_args(args);
   // printFullSymTable(oSymTable); 
    print_quads();
    return 0;
}

void yyerror(const char *s) {
    std::cerr << "Parse error: " << s << " at line : "<<yylineno <<" before token: " <<  yytext << endl;
}
