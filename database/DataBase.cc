#include "DataBase.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdarg>
#include <errno.h>
#include <sys/uio.h>

#define IDXLEN_SZ  4
#define SEP       ':'
#define SPACE     ' '
#define NEWLINE   '\n'

#define PTR_SZ    7
#define PTR_MAX   999999
#define NHASH_DEF 137
#define FREE_OFF  0
#define HASH_OFF  PTR_SZ

typedef unsigned long DBHASH;
typedef unsigned long COUNT;

typedef struct {
    int      idxfd;
    int      datfd;
    char     *idxbuf;
    char     *datbuf;
    char     *name;
    off_t    idxoff;
    size_t   idxlen;
    off_t    datoff;
    size_t   datlen;
    off_t    ptrval;
    off_t    ptroff;
    off_t    chainoff;
    off_t    hashoff;
    DBHASH   nhash;
    COUNT    cnt_delok;
    COUNT    cnt_delerr;
    COUNT    cnt_fetchok;
    COUNT    cnt_fetcherr;
    COUNT    cnt_nextrec;
    COUNT    cnt_store1;
    COUNT    cnt_store2;
    COUNT    cnt_store3;
    COUNT    cnt_store4;
    COUNT    cnt_storerr;
} DB;

static DB *_db_alloc(int);




DBHANDLE db_open(const char *pathname, int oflag, ...)
{
    DB      *db;
    int     len, mode;
    size_t  i;
    char    asciiptr[PTR_SZ + 1],
            hash[(NHASH_DEF + 1) * PTR_SZ + 2];

    struct stat statbuff;

    len = strlen(pathname);
    if((db = _db_alloc(len)) == NULL)
    {

    }
}

