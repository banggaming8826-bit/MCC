#ifndef __MCC_GENERAL__
#define __MCC_GENERAL__

#include <iostream>
#include <cstdio>
#include <stdint.h>

typedef const size_t MCCArch_t;
template <class PtrType = int*> inline MCCArch_t getarch(void) {
	return (MCCArch_t)(sizeof(PtrType) * 8);
}

typedef const char MCCStatus; 
template <class... Type> inline void errprint(const bool e, Type&&... args) noexcept 
{
	std::cerr << "mcc: \033[31merror\033[0m: ";
	((std::cerr << args), ...);
	std::cerr << "\n";
	if (e) { exit(1); }
}
template <class... Type> inline void infprint(Type&&... args) noexcept 
{
	std::cout << "mcc: \033[34minfo\033[0m: ";
	((std::cout << args), ...);
	std::cout << "\n" << std::flush;
}

inline size_t getsize_t(const std::string& s) noexcept
{
	size_t strsz = sizeof(char*);
	if	(s == "char")		{ return 1;		}
	else if (s == "bool")	 	{ return 1; 		}
	else if (s == "short")	 	{ return 2; 		}
	else if (s == "int")	 	{ return 4; 		}
	else if (s == "long")	 	{ return 8;  		}
	else if (s == "llong")	 	{ return 8; 		}
	else if (s == "float")	 	{ return 4;	 	}
	else if (s == "double")	 	{ return 8; 		}
	else if (s == "ldouble")	{ return 16; 		}
	else if (s == "string_t")	{ return strsz; 	}
	else			 	{ return 0;		}
}

#endif // __MCC_GENERAL__ //
