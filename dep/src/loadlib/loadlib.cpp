#define _CRT_SECURE_NO_DEPRECATE

#include "loadlib.h"
#include "mpq_libmpq04.h"

#include <stdio.h>

class MPQFile;

FileLoader::FileLoader()
{
    data = 0;
    data_size = 0;
    version = 0;
}

FileLoader::~FileLoader()
{
    free();
}

bool FileLoader::loadFile(char *filename, bool log)
{
    free();
    MPQFile mf(filename);
    if(mf.isEof())
    {
        if (log)
            printf("No such file %s\n", filename);
        return false;
    }

    data_size = mf.getSize();

    data = new uint8 [data_size];
    if (data)
    {
        mf.read(data, data_size);
        mf.close();
        if (prepareLoadedData())
            return true;
    }
    printf("Error loading %s", filename);
    mf.close();
    free();
    return false;
}

bool FileLoader::prepareLoadedData()
{
    // Check version
    version = (file_MVER *) data;

    if (!version)
        return false;

	if( ( version->fcc_txt[ 3 ] != 'M' ) ||
		( version->fcc_txt[ 2 ] != 'V' ) ||
		( version->fcc_txt[ 1 ] != 'E' ) ||
		( version->fcc_txt[ 0 ] != 'R' ) )
		return false;

    if (version->ver != FILE_FORMAT_VERSION)
        return false;
    return true;
}

void FileLoader::free()
{
    if (data) delete[] data;
    data = 0;
    data_size = 0;
    version = 0;
}