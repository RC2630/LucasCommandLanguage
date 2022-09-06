#include "customPrint.h"

// the purpose of this function is to facilitate the chaining of operator <<
ostream& cprint::operator << (ostream& out, const ostream& ignore) {
	return out;
}