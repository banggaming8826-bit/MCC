#ifndef __MCC_BUILDASM2__
#define __MCC_BUILDASM2__

#include "ast.h"
#include "parser.h"
#include "lexer.h"

#undef MCCBuild_ASM2_gencode_addop
#undef MCCBuild_ASM2_gencode_subop

#define MCCBuild_ASM2_gencode_addop(d) \
		switch(d)\
		{\
			case DT_CHAR: \
			case DT_BOOL:\
			case DT_SHORT:\
			case DT_INT: \
			case DT_LONG:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"pop ecx",\
						"pop eax",\
						"add eax, ecx",\
						"push eax" \
					); \
				}\
				else\
				{\
					addasmc(\
						"pop rcx",\
						"pop rax",\
						"add rax, rcx",\
						"push rax"\
					);\
				}\
				break;\
			case DT_LLONG:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"pop edx",\
						"pop eax",\
						"pop ecx",\
						"pop ebx",\
						"add eax, ebx",\
						"adc edx, ecx",\
						"push eax",\
						"push edx"\
					);\
				}\
				else\
				{\
					addasmc(\
						"pop rcx",\
						"pop rax",\
						"add rax, rcx",\
						"push rax"\
					);\
				}\
				break;\
			case DT_FLOAT:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"movss xmm1, [esp]",\
						"add esp, 4",\
						"movss xmm0, [esp]",\
						"add esp, 4",\
						\
						"addss xmm0, xmm1",\
						"sub esp, 4",\
						"movss [esp], xmm0"\
					);\
				}\
				else\
				{\
					addasmc(\
						"movss xmm1, [rsp]",\
						"add rsp, 4",\
						"movss xmm0, [rsp]",\
						"add rsp, 4",\
						\
						"addss xmm0, xmm1",\
						"sub rsp, 4",\
						"movss [rsp], xmm0"\
					);\
				}\
				break;\
			case DT_DOUBLE:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"movsd xmm1, [esp]",\
						"add esp, 8",\
						"movsd xmm0, [esp]",\
						"add esp, 8",\
						\
						"addsd xmm0, xmm1",\
						"sub esp, 8",\
						"movsd [esp], xmm0"\
					);\
				}\
				else\
				{\
					addasmc(\
						"movsd xmm1, [rsp]",\
						"add rsp, 8",\
						"movsd xmm0, [rsp]",\
						"add rsp, 8",\
						\
						"addsd xmm0, xmm1",\
						"sub rsp, 8",\
						"movsd [rsp], xmm0"\
					);\
				}\
				break;\
			case DT_LDOUBLE:\
				break;\
			default:\
				errprint(1, "unknown typename");\
				break;\
		}
#define MCCBuild_ASM2_gencode_subop(d) \
		switch(d)\
		{\
			case DT_CHAR: \
			case DT_BOOL:\
			case DT_SHORT:\
			case DT_INT: \
			case DT_LONG:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"pop ecx",\
						"pop eax",\
						"sub eax, ecx",\
						"push eax" \
					); \
				}\
				else\
				{\
					addasmc(\
						"pop rcx",\
						"pop rax",\
						"sub rax, rcx",\
						"push rax"\
					);\
				}\
				break;\
			case DT_LLONG:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"pop edx",\
						"pop eax",\
						"pop ecx",\
						"pop ebx",\
						"sub eax, ebx",\
						"sbb edx, ecx",\
						"push eax",\
						"push edx"\
					);\
				}\
				else\
				{\
					addasmc(\
						"pop rcx",\
						"pop rax",\
						"sub rax, rcx",\
						"push rax"\
					);\
				}\
				break;\
			case DT_FLOAT:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"movss xmm1, [esp]",\
						"add esp, 4",\
						"movss xmm0, [esp]",\
						"add esp, 4",\
						\
						"subss xmm0, xmm1",\
						"sub esp, 4",\
						"movss [esp], xmm0"\
					);\
				}\
				else\
				{\
					addasmc(\
						"movss xmm1, [rsp]",\
						"add rsp, 4",\
						"movss xmm0, [rsp]",\
						"add rsp, 4",\
						\
						"subss xmm0, xmm1",\
						"sub rsp, 4",\
						"movss [rsp], xmm0"\
					);\
				}\
				break;\
			case DT_DOUBLE:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"movsd xmm1, [esp]",\
						"add esp, 8",\
						"movsd xmm0, [esp]",\
						"add esp, 8",\
						\
						"subsd xmm0, xmm1",\
						"sub esp, 8",\
						"movsd [esp], xmm0"\
					);\
				}\
				else\
				{\
					addasmc(\
						"movsd xmm1, [rsp]",\
						"add rsp, 8",\
						"movsd xmm0, [rsp]",\
						"add rsp, 8",\
						\
						"subsd xmm0, xmm1",\
						"sub rsp, 8",\
						"movsd [rsp], xmm0"\
					);\
				}\
				break;\
			case DT_LDOUBLE:\
				break;\
			default:\
				errprint(1, "unknown typename");\
				break;\
		}
#define MCCBuild_ASM2_gencode_mulop(d) \
		switch(d)\
		{\
			case DT_CHAR: \
			case DT_BOOL:\
			case DT_SHORT:\
			case DT_INT: \
			case DT_LONG:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"pop ecx",\
						"pop eax",\
						"imul eax, ecx",\
						"push eax" \
					); \
				}\
				else\
				{\
					addasmc(\
						"pop rcx",\
						"pop rax",\
						"imul rax, rcx",\
						"push rax"\
					);\
				}\
				break;\
			case DT_LLONG:\
				break;\
			case DT_FLOAT:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"movss xmm1, [esp]",\
						"add esp, 4",\
						"movss xmm0, [esp]",\
						"add esp, 4",\
						\
						"mulss xmm0, xmm1",\
						"sub esp, 4",\
						"movss [esp], xmm0"\
					);\
				}\
				else\
				{\
					addasmc(\
						"movss xmm1, [rsp]",\
						"add rsp, 4",\
						"movss xmm0, [rsp]",\
						"add rsp, 4",\
						\
						"mulss xmm0, xmm1",\
						"sub rsp, 4",\
						"movss [rsp], xmm0"\
					);\
				}\
				break;\
			case DT_DOUBLE:\
				if (getarch() == 32)\
				{\
					addasmc(\
						"movsd xmm1, [esp]",\
						"add esp, 8",\
						"movsd xmm0, [esp]",\
						"add esp, 8",\
						\
						"mulsd xmm0, xmm1",\
						"sub esp, 8",\
						"movsd [esp], xmm0"\
					);\
				}\
				else\
				{\
					addasmc(\
						"movsd xmm1, [rsp]",\
						"add rsp, 8",\
						"movsd xmm0, [rsp]",\
						"add rsp, 8",\
						\
						"mulsd xmm0, xmm1",\
						"sub rsp, 8",\
						"movsd [rsp], xmm0"\
					);\
				}\
				break;\
			case DT_LDOUBLE:\
				break;\
			default:\
				errprint(1, "unknown typename");\
				break;\
		}
#define MCCBuild_ASM2_gencode_divmodop(d, mod)                        \
switch(d)                                                            \
{                                                                    \
        case DT_BOOL:                                                \
        case DT_CHAR:                                                \
        case DT_SHORT:                                               \
        case DT_INT:                                                 \
        case DT_LONG:                                                \
                if(getarch() == 32)                                  \
                {                                                    \
                        addasmc(                                     \
                                "pop ecx",                           \
                                "pop eax",                           \
                                "cdq",                               \
                                "idiv ecx"                           \
                        );                                           \
                                                                \
                        if(mod)                                      \
			{ addasmc("push edx");}                 \
                        else                                         \
			{ addasmc("push eax");   }              \
                }                                                    \
                else                                                 \
                {                                                    \
                        addasmc(                                     \
                                "pop rcx",                           \
                                "pop rax",                           \
                                "cqo",                               \
                                "idiv rcx"                           \
                        );                                           \
                                                                \
                        if(mod)                                      \
			{addasmc("push rdx");}                 \
                        else                                         \
			{ addasmc("push rax");}                 \
                }                                                    \
                break;                                               \
        case DT_FLOAT:                                               \
                if(mod)                                               \
		{ errprint(1, "floating modulo is unsupported."); }\
                else                                                 \
                {                                                    \
                	if(getarch() == 32)                                  \
              		{\
				addasmc(\
					"movss xmm1, [esp]",\
    					"add esp, 4",\
    					"movss xmm0, [esp]",\
    					"add esp, 4",\
    					"divss xmm0, xmm1",\
    					"sub esp, 4",\
    					"movss [esp], xmm0"\
				);\
                	}                                                    \
                	else                                                 \
                	{                                                    \
				addasmc(\
					"movss xmm1, [rsp]",\
    					"add rsp, 4",\
    					"movss xmm0, [rsp]",\
    					"add rsp, 4",\
    					"divss xmm0, xmm1",\
    					"sub rsp, 4",\
    					"movss [rsp], xmm0"\
				);\
                	}                                                    \
                }         \
		break; /* arr=[operator:std::stream::set_type{$+;$-;$*;$/;$%;$__open[]__;$__close[]__}] */\
        case DT_DOUBLE:                                              \
                if(mod)                                               \
                {errprint(1, "floating modulo is unsupported.");}\
                else                                                 \
                {                                                    \
                	if(getarch() == 32)                                  \
              		{                                                    \
				addasmc(\
					"movsd xmm1, [esp]",\
    					"add esp, 8",\
    					"movsd xmm0, [esp]",\
    					"add esp, 8",\
    					"divsd xmm0, xmm1",\
    					"sub esp, 8",\
    					"movsd [esp], xmm0"\
				);\
                	}                                                    \
                	else                                                 \
                	{                                                    \
				addasmc(\
					"movsd xmm1, [rsp]",\
    					"add rsp, 8",\
    					"movsd xmm0, [rsp]",\
    					"add rsp, 8",\
    					"divsd xmm0, xmm1",\
    					"sub rsp, 8",\
    					"movsd [rsp], xmm0"\
				);\
                	}                                                    \
                }                                                   \
                break;                                               \
        case DT_LLONG:                                               \
                break;                                               \
        case DT_LDOUBLE:                                             \
                break;                                               \
}
#endif
