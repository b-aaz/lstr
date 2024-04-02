#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<limits.h>
#include<assert.h>
enum lstr_flags
{
	LS_1BYT	= 0,
	LS_2BYT	= 1,
	LS_3BYT	= 2,
	LS_4BYT	= 3,
	LS_ALOC	= 4,
	LS_PTR	= 8,
	LS_JOINT= 16,
	LS_FREE	= 32,
};
enum lstr_errors
{
	WRONG_FLAGS=-1,
	WRONG_BMASK=-2,
};
#define PEDANTIC 0

#define BMASK 3
#define USEDFBITS 63
static const unsigned char tpow[]= {0,2,4,8} ;
typedef unsigned char lstr ;
/* Is this C or lisp ? */
#define hddr_size(flags) \
	(1+tpow[flags&BMASK]*((!!(flags&LS_ALOC))+1)+((!!(flags&LS_PTR))*sizeof(char *)) )
#define len_ptr(lstr) \
	(lstr+1)
#define aloc_ptr(flags,lstr) \
	(flags&LS_ALOC?lstr+1+tpow[flags&BMASK]:NULL)
#define ptr_ptr(flags,lstr) \
	((char **)(flags&LS_PTR?lstr+1+tpow[flags&BMASK]*((!!(flags&LS_ALOC))+1):NULL))
#define str_ptr(flags,lstr) \
	((char *)(lstr+hddr_size(flags)))
size_t lstralloc_set (lstr * lstr, size_t aloc)
{
	unsigned char flags=lstr[0]&USEDFBITS;

	switch (flags&BMASK)
	{
		case 0:
			if (aloc<UINT8_MAX)
			{
				* ( (uint8_t *) aloc_ptr (flags,lstr)) = (uint8_t) aloc ;
				return 0;
			}
			else
			{
				return -1;
			}

			break;

		case 1:
			if (aloc<UINT16_MAX)
			{
				* ( (uint16_t *) aloc_ptr (flags,lstr)) = (uint16_t) aloc ;
				return 0;
			}
			else
			{
				return -1;
			}

			break;

		case 2:
			if (aloc<UINT32_MAX)
			{
				* ( (uint32_t *) aloc_ptr (flags,lstr)) = (uint32_t) aloc ;
				return 0;
			}
			else
			{
				return -1;
			}

			break;

		case 3:
			if (aloc<UINT64_MAX)
			{
				* ( (uint64_t *) aloc_ptr (flags,lstr)) = (uint64_t) aloc ;
				return 0;
			}
			else
			{
				return -1;
			}

			break;
	}

	assert (PEDANTIC&&"Wrong BMASK");
	return -2;
}
size_t lstralloc (size_t * size,lstr * lstr)
{
	unsigned char flags=lstr[0]&USEDFBITS;

	if (! (flags&LS_ALOC))
	{
		assert (PEDANTIC&&"string doesn't have the LS_ALOC flag set");
		return -1;
	}

	switch (flags&BMASK)
	{
		case 0:
			*size =* (uint8_t *) aloc_ptr (flags,lstr);
			return 0;
			break;

		case 1:
			*size =* (uint16_t *) aloc_ptr (flags,lstr);
			return 0;
			break;

		case 2:
			*size =* (uint32_t *) aloc_ptr (flags,lstr);
			return 0;
			break;

		case 3:
			*size =* (uint64_t *) aloc_ptr (flags,lstr);
			return 0;
			break;
	}

	assert (PEDANTIC&&"Wrong BMASK");
	return 0;
}
enum lstr_errors lstrlen_set (lstr * lstr, size_t len)
{
	unsigned char flags=lstr[0]&USEDFBITS;

	switch (flags&BMASK)
	{
		case 0:
			if (len<UINT8_MAX)
			{
				* ( (uint8_t *) len_ptr (lstr)) = (uint8_t) len ;
				return 0;
			}
			else
			{
				return -1;
			}

			break;

		case 1:
			if (len<UINT16_MAX)
			{
				* ( (uint16_t *) len_ptr (lstr)) = (uint16_t) len ;
				return 0;
			}
			else
			{
				return -1;
			}

			break;

		case 2:
			if (len<UINT32_MAX)
			{
				* ( (uint32_t *) len_ptr (lstr)) = (uint32_t) len ;
				return 0;
			}
			else
			{
				return -1;
			}

			break;

		case 3:
			if (len<UINT64_MAX)
			{
				* ( (uint64_t *) len_ptr (lstr)) = (uint64_t) len ;
				return 0;
			}
			else
			{
				return -1;
			}

			break;
	}

	assert (PEDANTIC&&"Wrong BMASK");
	return -2;
}
size_t lstrlen (lstr * lstr)
{
	unsigned char flags=lstr[0]&USEDFBITS;

	switch (flags&BMASK)
	{
		case 0:
			return (uint8_t) * len_ptr (lstr);

		case 1:
			return (uint16_t) * len_ptr (lstr);

		case 2:
			return (uint32_t) * len_ptr (lstr);

		case 3:
			return (uint64_t) * len_ptr (lstr);
	}

	assert (PEDANTIC&&"Wrong BMASK");
	return 0;
}
void lstr_info(lstr * lstr){
	unsigned char flags = USEDFBITS&lstr[0];
	unsigned char mag = flags&BMASK;
	unsigned char hddrsize = hddr_size(flags);
		printf("String pointer: %p\n", lstr);
	printf("String header size: %d bytes\n", hddrsize);
	printf("String magnitude: %d\n", mag);
	printf("String byte magnitude: %d bytes\n", tpow[mag]);
	printf("String is LS_ALOC flag set?: %s\n", (flags&LS_ALOC)?"yes":"no");
	printf("String is LS_PTR flag set?: %s\n", (flags&LS_PTR )?"yes":"no");
	printf("String is LS_FREE flag set?: %s\n", (flags&LS_FREE )?"yes":"no");
	printf("String is LS_JOINT flag set?: %s\n", (flags&LS_JOINT )?"yes":"no");
	printf("String length: %zu\n", lstrlen(lstr));
	if (flags&LS_ALOC)
	{
		size_t size ;
		lstralloc(&size,lstr);
		printf("String buffer size: %zu\n", size);
	}
	if (flags&LS_PTR)
	{
		printf("String [inner] pointer: %p\n", *ptr_ptr(flags,lstr));
	}

	puts("String header struct form:");
	puts("struct lstr {");
	printf("\tunsigned char flags = %d;\n",flags);
	printf("\tuint%d_t len = %zu;\n",tpow[mag]*8,lstrlen(lstr));
	if (flags&LS_ALOC) {
	printf("\tuint%d_t aloc = %zu;\n",tpow[mag]*8,lstrlen(lstr));}
	if (flags&LS_PTR) {
	printf("\tchar * ptr = %p;\n",*ptr_ptr(flags,lstr));}
	printf("\tchar str [0]; /* pointer to str is: %p */\n", str_ptr(flags,lstr));
	puts("};");
}

unsigned char size_excides_limit (size_t size, unsigned char flags)
{
	switch (flags&BMASK)
	{
		case 0:
			if (size>UINT8_MAX)
			{
				return 1;
			}

			break;

		case 1:
			if (size>UINT16_MAX)
			{
				return 1;
			}

			break;

		case 2:
			if (size>UINT32_MAX)
			{
				return 1;
			}

			break;

		case 3:
			if (size>UINT64_MAX)
			{
				return 1;
			}

			break;
	}

	assert (PEDANTIC&&"Wrong BMASK");
	return 0;
}
lstr * lstrnew (unsigned char flags, size_t minimum_size)
{
	flags=flags&USEDFBITS;
	unsigned char hddrsize = hddr_size (flags);
	printf ("%d\n",hddrsize);
	lstr * lstr;

	if (flags&LS_ALOC)
	{
		switch (flags&BMASK)
		{
			case 0:
				minimum_size= (minimum_size>UINT8_MAX) ?UINT8_MAX :minimum_size;
				break;

			case 1:
				minimum_size= (minimum_size>UINT16_MAX) ?UINT16_MAX:minimum_size;
				break;

			case 2:
				minimum_size= (minimum_size>UINT32_MAX) ?UINT32_MAX:minimum_size;
				break;

			case 3:
				minimum_size= (minimum_size>UINT64_MAX) ?UINT64_MAX:minimum_size;
				break;
		}
	}

	if (flags&LS_JOINT)
	{
		/* +1 for the null terminator. */
		size_t joint_size = (hddrsize>minimum_size?hddrsize:minimum_size)+1;
		lstr= malloc (joint_size);
		lstr[0]=flags;
		lstralloc_set (lstr,minimum_size);
		lstrlen_set (lstr,0);

		if (flags&LS_PTR)
		{
			*ptr_ptr (flags,lstr) =str_ptr (flags,lstr);
		}
	}
	else
	{
		/* +1 for the null byte . */
		lstr = malloc (hddrsize+1);
		lstr[0]=flags;
		lstralloc_set (lstr,minimum_size);
		lstrlen_set (lstr,0);
		*ptr_ptr (flags,lstr) = malloc (minimum_size);
		lstr[hddrsize]='\0';
	}

	return lstr;
}
int main (int argc,char ** argv)
{
	lstr * lstr;
	puts ("hello world");
	lstr =	lstrnew (LS_ALOC|LS_4BYT|LS_JOINT|LS_PTR,0);
	lstr_info(lstr);
}

