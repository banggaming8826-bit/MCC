#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <chrono>
#include "mcc_headers/general.h"
#include "mcc_headers/ast.h"
#include "mcc_headers/lexer.h"
#include "mcc_headers/parser.h"
#include "mcc_headers/buildasm.h"

static const char* mcc_vers = "0.01-preview";
static inline const int show_pb(void) noexcept {
	return std::printf("mcc compiler %s version\n", mcc_vers);
}

int main(int argc, char** argv)
{
	auto ctime = std::chrono::high_resolution_clock::now();
	if (argc <= 1) { errprint(true, "no inputs"); }
	std::string fname, fcname;
	bool skobj = 0, skasm = 0, run = 0, show_info = 0, show_ver = 0;
	for (int i = 1; i < argc; i++)
	{
		std::string a = argv[i];
		if (!a.empty() && a[0] != '-') { fname = a; }
		else if (a == "-o") { 
			if (argv[i + 1] == NULL) { errprint(1, "flag `-o' request name (usage: mcc file.mc -o fname)"); }
			fcname = (argv[++i]); 
		}
		else if (a == "-c" || a == "-co") { skasm = 1; }
		else if (a == "-asm" || a == "-S") { skobj = 1; }
		else if (a == "-ra") { run = 1; }
		else if (a == "-si" || a == "-sinfo") { show_info = 1; }
		else if (a == "-v" || a == "--version") { show_ver = 1; }
		else { errprint(1, "no flag argument name: " + a); }
	}
	if (fname.empty() && !show_ver) {
		errprint(1, "no file input (\"mcc <name1> -o <name2>\")");
		return 1;
	}
	if (fcname.empty() && !show_ver) {
		errprint(1, "no file name output (\"mcc <name1> -o <name2>\")");
		return 1;
	}
	std::stringstream ss;
	std::fstream f(fname);
	if (f.fail() || !f.is_open() && !show_ver) {
		errprint(1, "cannot open file: ", fname);
		return 1;
	}
	ss << f.rdbuf();
	std::string code = ss.str();

	MCCLexer lexer(code);
	auto T = lexer.lexer();
	MCCParser parser(T);
	auto* goc = parser.parse();
	if (!goc) {
		errprint(1, "compile fail (!), unknown error");
		return 1;
	}
	MCCBuild_ASM b("__objmcc_" + fname + ".s");
	b.build(goc);

	if (skasm) { return 0; }
	if (show_ver) {
		show_pb();
		return 0;
	}
	const std::string lenh_toasm = std::string(
		"nasm -f " + std::string(((getarch() == 32) ? "elf32" : "elf64")) + " " +
			std::string("__objmcc_") + fname + ".s" +
		" -o " + fcname + ".o"
	);
	int retsys1 = std::system(lenh_toasm.c_str());

	if (skobj) { return 0; }
	int retsys2 = std::system(std::string(
		"ld " + fcname + ".o" + " -o " + fcname
	).c_str());

	auto ctime2 = std::chrono::high_resolution_clock::now();
	if (retsys1 == 0 && retsys2 == 0) { infprint("compile completed!"); }
	else { errprint(1, "compile fail with error!"); }
	if (show_info) 
	{
		auto fs  =  std::chrono::duration<double>(ctime2 - ctime).count();
		auto fms =  std::chrono::duration<double, std::milli>(ctime2 - ctime).count();
		std::cout << "(i) compile info:\n"
			  << "- [<-] file input name		: " << fname << "\n"
			  << "- [->] file output name		: " << fcname << "\n"
			  << "- [*]  compile time		: " << fs << "s / " << fms << "ms\n"
			  << "- [#]  auto run			: " << std::boolalpha << run << "\n"
			  << "- [=]  architecture (arch)	: " << ((getarch() == 32) ? "elf32 / x32" : "elf64 / x64")<<"\n"
			  << "- [$]  mcc compiler version	: " << mcc_vers << "\n"
			  << "- [v]  compile complete		: " << ((retsys1 == 0 && retsys2 == 0) ? "true" : "false") << "\n"
			  << "- [@]  identifier count (...)	: " << parser.tokvect_size() << "\n"
			  << "- [.]  more infomation:\n"
			  << "-		[$] nasm info (-v):\n"
			  << "		"
			  << std::flush;
		std::system("nasm -v");
	}

	if (run) { std::system(std::string("./" + fcname).c_str()); }
	return 0;
}
