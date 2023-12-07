#pragma once
#ifndef _KEYIN_H_
#define _KEYIN_H_

#define N_KEY		15  
#define K_ARROW		224
#define K_UP		72
#define K_DOWN		80
#define K_LEFT		75
#define K_RIGHT		77
#define K_SPACE		32
#define K_CONFIRM	'c'
#define K_STEAL		'a'
#define K_CONCIL	's'
#define K_LPULL		'z'
#define K_RPULL		'/'
#define K_LLD		'x'
#define K_RLD		'.'
#define K_CHOICE	' '
#define K_QUIT		'q'
#define K_UNDEFINED	'\0'

typedef int key_t;

key_t get_key(void);

#endif
