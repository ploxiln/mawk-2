
/********************************************
jmp.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


#ifndef   JMP_H
#define   JMP_H

void   BC_new(void) ;
void   BC_insert(int, INST*) ;
void   BC_clear(INST *, INST *) ;
void   code_push(INST *, unsigned, int, FBLOCK*) ;
unsigned   code_pop(INST *) ;
void   code_jmp(int, INST *) ;
void   patch_jmp(INST *) ;

extern int code_move_level ;
   /* used to as one part of unique identification of context when
      moving code.  Global for communication with parser.
   */

#endif  /* JMP_H  */

