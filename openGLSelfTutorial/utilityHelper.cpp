#include "utilityHelper.h"

namespace uHelper {

	// loadFile - loads text file from file fname as a char* 
	// Allocates memory - so remember to delete after use
	// size of file returned in fSize
	GLchar* loadFile(const char * fname, GLint &fSize)
	{
		int size;
		char * memblock;

		// file read based on example in cplusplus.com tutorial
		// ios::ate opens file at the end
		ifstream file(fname, ios::in | ios::binary | ios::ate);
		if (file.is_open()) {
			size = (int)file.tellg(); // get location of file pointer i.e. file size
			fSize = (GLint)size;
			memblock = new char[size];
			file.seekg(0, ios::beg);
			file.read(memblock, size);
			file.close();
			cout << "file " << fname << " loaded" << endl;
		}
		else {
			cout << "Unable to open file " << fname << endl;
			fSize = 0;
			// should ideally set a flag or use exception handling
			// so that calling function can decide what to do now
			return nullptr;
		}
		return memblock;
	}
	

}

