#ifndef __MCC_BUILDASM__
#define __MCC_BUILDASM__

#include <iostream>
#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "general.h"
#include "buildasm2.h"
#include <fstream>
#include <map>

// $!?
static size_t MCC__lfx 		= 0;
static size_t MCC__ldx 		= 0;
static size_t MCC__lifx 	= 0;
static size_t MCC__lwhilex 	= 0;
static size_t MCC__lforx	= 0;
#undef  name_flmcc
#define name_flmcc std::string("__flmcc")
#undef  name_dlmcc
#define name_dlmcc std::string("__dlmcc")

class MCCBuild_ASM
{
private:
	std::ofstream ofa;
 	struct SymType {
		enum DataType type;
		size_t offset;
	};
	size_t lc_offset = 0;
	std::map<std::string, SymType> symtable;
	std::string text_sect, data_sect;

	template <class... Type> void addasmc (Type&&... args) noexcept {
		((text_sect += std::string("\t") + args + "\n"), ...);
	}
	template <class... Type> void addasmcd(Type&&... args) noexcept {
		((data_sect += std::string("\t") + args + "\n"), ...);
	}
	inline auto gencode(Node* la) noexcept -> enum DataType
	{
		enum DataType trave;
		if (!la) { return DT_UWHAT; }
		else if (auto* ptr = dynamic_cast<IntNum_Node*>(la))
		{
			auto okok_x6432 = [&](void) -> void {
					addasmc("mov " + std::string(((getarch() == 32) ? "eax" : "rax")) + ", " + ptr->num, 
						"push " + std::string(((getarch() == 32) ? "eax" : "rax"))
					);
			};
			switch (ptr->type_enum)
			{
				// 8bit
				case DT_CHAR:
					okok_x6432();
					return DT_CHAR;
				case DT_BOOL:
					okok_x6432();
					return DT_BOOL;
				// 16bit && 32bit
				case DT_SHORT:
					okok_x6432();
					return DT_SHORT;
				case DT_INT:
					okok_x6432();
					return DT_INT;
				// 64bit
				case DT_LONG:
					okok_x6432();
					return DT_LONG;
				case DT_LLONG:
					long long v = std::stoll(ptr->num);
					if (getarch() == 32)
					{
						uint32_t lo = (uint32_t)(v & 0xFFFFFFFFULL);
						uint32_t hi = (uint32_t)((uint64_t)v >> 32);
						addasmc(
							"mov eax, " + std::to_string(lo),
							"mov edx, " + std::to_string(hi),
							"push eax",
							"push edx"
						);
					}
					else {
						addasmc(
						"mov rax, " + ptr->num,
						"push rax"
						);
					}
					return DT_LLONG;
			}
			return DT_UWHAT;
			/*
			 * [!] Luu y:
			 * 	(*) x86:
			 * 		char | bool | short | int | long | llong
			 * 		eax  ; eax ; eax    ; eax; eax  ; rax (2 lan eax)
			 * 	(*) x86_64:
			 * 		char | bool | short | int | long | llong
			 * 		rax  ; rax ; rax    ; rax; rax  ; rax
			 */
		}
		else if (auto* ptr = dynamic_cast<FloatNum_Node*>(la)) 
		{
			switch(ptr->type_enum)
			{
				// x32 float
				case DT_FLOAT:
					addasmcd(
						name_flmcc + std::to_string(++MCC__lfx) + ":",
						"\tdd " + ptr->num
					);
					addasmc("movss xmm0, [" + name_flmcc + std::to_string(MCC__lfx) +"]");
					if (getarch() == 32) {
						addasmc("sub esp, 4", "movss [esp], xmm0");
					}
					else { addasmc("sub rsp, 4", "movss [rsp], xmm0"); }
					return DT_FLOAT;
				// x64 double
				case DT_DOUBLE:
					addasmcd(
						name_dlmcc + std::to_string(++MCC__ldx) + ":",
						"\tdq " + ptr->num
					);
					addasmc("movsd xmm0, [" + name_dlmcc + std::to_string(MCC__ldx) +"]");
					if (getarch() == 32) {
						addasmc("sub esp, 8", "movsd [esp], xmm0");
					}
					else { addasmc("sub rsp, 8", "movsd [rsp], xmm0"); }
					return DT_DOUBLE;
				// x64 128 [ldouble
				case DT_LDOUBLE:
					return DT_LDOUBLE;
				default:
					return DT_UWHAT;
			}
		}
		else if (auto* ptr = dynamic_cast<VarRef_Node*>(la)) 
		{
			auto repit = symtable.find(ptr->var);
			if (repit == symtable.end()) {
				errprint(1, "use a undeclared var. identifier");
				return DT_UWHAT;
			}
			auto& rep = repit->second;
			auto push_x6432 = [&](void) -> void {
				addasmc("push " + std::string((getarch() == 32) ? "eax" : "rax"));
			};
			auto resbp_x6432 = [&](void) -> std::string { return (getarch() == 32) ? "ebp" : "rbp"; };
			switch (rep.type)
			{
				// 8bit
				case DT_CHAR: // luon la unsigned
					addasmc(
						"movzx eax, byte [" + resbp_x6432() + " - " +
						std::to_string(rep.offset) + "]"
					);
					push_x6432();
					return DT_CHAR;
				case DT_BOOL:
					addasmc(
						"movzx eax, byte [" + resbp_x6432() + " - " +
						std::to_string(rep.offset) + "]"
					);
					push_x6432();
					return DT_BOOL;
				// 16bit
				case DT_SHORT:
					addasmc(
						"movsx eax, word [" + resbp_x6432() + " - " +
						std::to_string(rep.offset) + "]"
					);
					push_x6432();
					return DT_SHORT;
				// 32bit
				case DT_INT:
					addasmc(
						"mov eax, dword [" + resbp_x6432() + " - " +
						std::to_string(rep.offset) + "]"
					);
					push_x6432();
					return DT_INT;
				// 64bit
				case DT_LONG:
					if (getarch() == 32) {
						addasmc(
							"mov eax, dword [" + resbp_x6432() + " - " +
							std::to_string(rep.offset) + "]"
						);
					}
					else {
						addasmc(
							"mov rax, qword [" + resbp_x6432() + " - " +
							std::to_string(rep.offset) + "]"
						);
					}
					push_x6432();
					return DT_LONG;
				case DT_LLONG:
					if (getarch() == 32)
    					{
        					addasmc(
            						"mov eax, dword [ebp - " + std::to_string(rep.offset) + "]",
            						"mov edx, dword [ebp - " + std::to_string(rep.offset - 4) + "]",
							"push eax",
            						"push edx"
        					);
					}	
					else {
        					addasmc(
            						"mov rax, qword [rbp - " + std::to_string(rep.offset) + "]",
            						"push rax"
        					);
    					}
					return DT_LLONG;
				// x32 float
				case DT_FLOAT:
					addasmc(
						"movss xmm0, dword [" + resbp_x6432() + " - " +
						std::to_string(rep.offset) + "]"
					);
					if (getarch() == 32) {
						addasmc("sub esp, 4", "movss [esp], xmm0");
					}
					else { addasmc("sub rsp, 4", "movss [rsp], xmm0"); }
					return DT_FLOAT;
				// x64 double
				case DT_DOUBLE:
					addasmc(
						"movsd xmm0, qword [" + resbp_x6432() + " - " +
						std::to_string(rep.offset) + "]"
					);
					if (getarch() == 32) {
						addasmc("sub esp, 8", "movsd [esp], xmm0");
					}
					else { addasmc("sub rsp, 8", "movsd [rsp], xmm0"); }
					return DT_DOUBLE;
				// x64 [128] ldouble:
				case DT_LDOUBLE:
					addasmc(
						"movdqu xmm0, oword [" + resbp_x6432() + " - " +
						std::to_string(rep.offset) + "]"
					);
					return DT_LDOUBLE;
				default:;
			}
		}
		else if (auto* ptr = dynamic_cast<Assign_Node*>(la))
		{
			// Code ve trai: mau: a = c + 1 * 2 * 3 * 4* 5 + b
			DataType rdt = this->gencode(ptr->expr);
			auto repit = symtable.find(ptr->name);
			if (repit == symtable.end()) {
				errprint(1, "use a undeclared var. identifier");
				return DT_UWHAT;
			}
			auto& rep = repit.operator->()->second;
			std::string a = "[" + std::string(((getarch() == 32) ? "ebp" : "rbp")) +
					" - " + std::to_string(rep.offset) + "]";
			switch(rep.type)
			{
				case DT_BOOL: case DT_CHAR:
					addasmc("pop " + std::string((getarch() == 32) ? "eax" : "rax"));
					addasmc("mov byte " + a + ", al");
					break;
				case DT_SHORT:
					addasmc("pop " + std::string((getarch() == 32) ? "eax" : "rax"));
					addasmc("mov word " + a + ", ax");
					break;
				case DT_INT:
					addasmc("pop " + std::string((getarch() == 32) ? "eax" : "rax"));
					addasmc("mov dword " + a + ", eax");
					break;
				case DT_LONG:
					addasmc("pop " + std::string((getarch() == 32) ? "eax" : "rax"));
    					if (getarch() == 32)
        					addasmc("mov dword " + a + ", eax");
    					else
						addasmc("mov qword " + a + ", rax");
					break;
				case DT_LLONG:
					if (getarch() == 32) {
						addasmc("pop edx", "pop eax");
    						addasmc("mov dword " + a + ", eax",
							"mov dword [ebp - " +
							std::to_string(rep.offset - 4) + "], edx"
						);
					}
					else {
						addasmc("pop " + std::string((getarch() == 32) ? "eax" : "rax"));
    						addasmc("mov qword " + a + ", rax");
					}
    					break;
				case DT_FLOAT:
    					addasmc("movss dword " + a + ", xmm0");
					if (getarch() == 32) {
						addasmc("sub esp, 4", "movss [esp], xmm0");
					}
					else { addasmc("sub rsp, 4", "movss [rsp], xmm0"); }
    					break;
				case DT_DOUBLE:
					addasmc("movsd qword " + a + ", xmm0");
					if (getarch() == 32) {
						addasmc("sub esp, 8", "movsd [esp], xmm0");
					}
					else { addasmc("sub rsp, 8", "movsd [rsp], xmm0"); }
    					break;
				case DT_LDOUBLE:
 					addasmc("movdqu oword " + a + ", xmm0");
					break;
				default:
					return DT_UWHAT;
			}
			return rep.type;
		}
		else if (auto* ptr = dynamic_cast<VarDecl_Node*>(la))
		{
			struct SymType s = { ptr->type_enum };
			lc_offset += getsize_t(ptr->type);
			s.offset   = lc_offset;
			symtable[ptr->name] = s;
			if (ptr->khoitao) {
				Assign_Node gt(ptr->name, ptr->khoitao);
				this->gencode(&gt);
			}
			return ptr->type_enum;
		}
		else if (auto* ptr = dynamic_cast<BinOp2_Node*>(la))
		{
			DataType trai = this->gencode(ptr->trai),
				 phai = this->gencode(ptr->phai);
			if (trai == DT_UWHAT || phai == DT_UWHAT) {
				errprint(1, "unknown typename");
				return DT_UWHAT;
			}
			//--------------------------
			switch(ptr->op2.toktype)
			{
				case TOK_PLUS_O:
					MCCBuild_ASM2_gencode_addop(trai);
					return trai;
				case TOK_MIN_O:
					MCCBuild_ASM2_gencode_subop(trai);
					return trai;
				case TOK_STAR_O:
					MCCBuild_ASM2_gencode_mulop(trai);
					return trai;
				case TOK_SLASH_O:
					MCCBuild_ASM2_gencode_divmodop(trai, 0);
					return trai;
				case TOK_PERCENT_O:
					MCCBuild_ASM2_gencode_divmodop(trai, 1);
					return trai;
				// Compare
				// Helpme (TODO): Add the code if running on x86 computer
				case TOK_EQEQ_O:
					addasmc(
						"pop rcx", "pop rax",
						"cmp rax, rcx",
						"sete al",
						"movzx rax, al",
						"push rax"
					);
					return trai;
				case TOK_NEQ_O:
					addasmc(
						"pop rcx", "pop rax",
						"cmp rax, rcx",
						"setne al",
						"movzx rax, al",
						"push rax"
					);
					return trai;
				case TOK_SMALL_O:
					addasmc(
						"pop rcx", "pop rax",
						"cmp rax, rcx",
						"setl al",
						"movzx rax, al",
						"push rax"
					);
					return trai;
				case TOK_SEQ_O:
					addasmc(
						"pop rcx", "pop rax",
						"cmp rax, rcx",
						"setle al",
						"movzx rax, al",
						"push rax"
					);
					return trai;
				case TOK_LARGE_O:
					addasmc(
						"pop rcx", "pop rax",
						"cmp rax, rcx",
						"setg al",
						"movzx rax, al",
						"push rax"
					);
					return trai;
				case TOK_LEQ_O:
					addasmc(
						"pop rcx", "pop rax",
						"cmp rax, rcx",
						"setge al",
						"movzx rax, al",
						"push rax"
					);
					return trai;
				default:
					errprint(1, "unsupported this operator / invalid operator");
					return DT_UWHAT;
			}
			//--------------------------
		}
		else if (auto* ptr = dynamic_cast<Condition_Node*>(la))
		{
			this->gencode(ptr->dk);
			std::string tghi = (getarch() == 32) ? "eax" : "rax";
			addasmc(
				"pop " + tghi,
				"cmp " + tghi + ", 0",
				"je __iflabmcc" + std::to_string(++MCC__lifx)
			);
			for (Node* (&i) : ptr->lenh->st) { this->gencode(i); }
			addasmc(
				"jmp __ifendmcc" + std::to_string(MCC__lifx), 
				"__iflabmcc" + std::to_string(MCC__lifx) + ":"
			);
			if (ptr->elst) { this->gencode(ptr->elst); }
			addasmc("__ifendmcc" + std::to_string(MCC__lifx) + ":");
			return DT_UWHAT;
		}
		else if (auto* ptr = dynamic_cast<WhileLoop_Node*>(la))
		{
			std::string tghi = (getarch() == 32) ? "eax" : "rax";
			addasmc("__while_beginmcc" + std::to_string(++MCC__lwhilex) + ":");
			this->gencode(ptr->dk);
			addasmc(
				"pop " + tghi,
				"cmp " + tghi + ", 0",
				"je __while_endmcc" + std::to_string(MCC__lwhilex),
				""
			);
			for (Node* (&i) : ptr->lenh->st) { this->gencode(i); }
			addasmc(
				"",
				"jmp __while_beginmcc" + std::to_string(MCC__lwhilex),
				"__while_endmcc" + std::to_string(MCC__lwhilex) + ":"
			);
			return DT_UWHAT;
		}
		else if (auto* ptr = dynamic_cast<ForLoop_Node*>(la))
		{
			std::string tghi = (getarch() == 32) ? "eax" : "rax";
			this->gencode(ptr->khoitao);
			addasmc("__for_beginmcc" + std::to_string(++MCC__lforx) + ":");
			this->gencode(ptr->dk);
			addasmc(
				"pop " + tghi,
				"cmp " + tghi + ", 0",
				"je __for_endmcc" + std::to_string(MCC__lforx),
				""
			);
			for (Node* (&i) : ptr->lenh->st) { this->gencode(i); }
			this->gencode(ptr->update);
			addasmc(
				"",
				"jmp __for_beginmcc" + std::to_string(MCC__lforx),
				"__for_endmcc" + std::to_string(MCC__lforx) + ":"
			);
			return DT_UWHAT;
		}
		else if (auto* ptr = dynamic_cast<Function_Node*>(la))
		{}
		return DT_UWHAT;
	}
public:
	explicit MCCBuild_ASM(const std::string& f) noexcept : ofa(f) {}
	explicit MCCBuild_ASM(std::ofstream&&    f) noexcept : ofa(std::move(f)) {}
		 MCCBuild_ASM(MCCBuild_ASM&& 	 b) noexcept : ofa(std::move(b.ofa)) {}
		 MCCBuild_ASM(const MCCBuild_ASM&)  noexcept = delete;
	inline int build(Barrel_Node* la) noexcept
	{
		if (!la) { return 1; }
		if (ofa.fail() || !ofa.is_open()) {
			errprint(1, "cannot create file object execute (asm)");
			return 1;
		}

		ofa << "section .text\n"
		    << "global _start\n"
		    << "_start:\n";
		for (Node* cay : la->st) { this->gencode(cay); }
		if (getarch() == 32) {
			ofa << "\tpush ebp\n" << "\tmov ebp, esp\n" << "\tsub esp, " + std::to_string(lc_offset) + "\n\n";
		}
		else { ofa << "\tpush rbp\n" << "\tmov rbp, rsp\n" << "\tsub rsp, " + std::to_string(lc_offset) + "\n\n"; }
		ofa << this->text_sect;
		if (!data_sect.empty()) { ofa << "\n;Section: Data/Du lieu\nsection .data\n" << this->data_sect; }
		ofa.flush();
		ofa.close();
		return 0;
	}
};

#endif
