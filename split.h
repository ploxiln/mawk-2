
/********************************************
split.h
copyright 2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/

#ifndef SPLIT_H
#define SPLIT_H

/* 
 
   Splitting is a two step process:
   There is a call to a split function which loads the pieces
   into a private buffer and returns a count.

   The caller next imediately calls
        transfer_to_fields(size_t)   or
	transfer_to_array(CELL[],size_t)

   to get the pieces out of the anonymous private buffer

   We can split one of three ways:
     (1) By space:
	 space_split()
     (2) By regular expression:
	 re_split()  
     (3) By "" (null -- split into characters)
	 null_split() 
*/

size_t space_split(const char *str, size_t slen);
size_t null_split(const char *str, size_t len);
size_t re_split(const char *str, size_t len, PTR re);

/* the two transfer functions  */
void transfer_to_fields(size_t cnt);
void transfer_to_array(CELL[], size_t cnt);

#endif /* SPLIT_H */
