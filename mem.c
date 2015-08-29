#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main ( int argc, char ** argv )
{
	--argc;
	++argv;

	if ( argc >= 1 && argc <= 2 ) {
		int ps = getpagesize();
		int fd = open ( "/dev/mem", O_RDWR );
                unsigned long addr = strtoul ( argv[0], 0, 0 );
                unsigned long val;
                char * dir;
		unsigned long * mem;

		if ( fd < 0 ) {
			perror ( "open" );
			exit ( 1 );
		}

		mem = mmap ( 0, ps, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr&-ps );
		if ( mem == (void *)-1 ) {
			perror ( "mmap" );
			exit ( 1 );
		}

                if ( argc >= 2 ) {
                        val = strtoul ( argv[1], 0, 0 );
                        dir = "<=";
                        mem[(addr&~-ps)/4] = val;
                } else {
                        val = mem[(addr&~-ps)/4];
                        dir = "=>";
                }

		munmap ( mem, ps );

		close ( fd );

                printf ( "%lu (0x%lx) %s %lu (0x%lx)\n", addr, addr, dir, val, val );
	} else {
		printf ( "usage: mem <addr>               - read 32-bit word from physical address <addr>\n"
			 "       mem <addr> <val>         - write 32-bit word <val> to physical address <addr>\n"
		);
	}

        return 0;
}
