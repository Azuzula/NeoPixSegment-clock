#ifndef SEGMENTS_DEFINITION_H
#define SEGMENTS_DEFINITION_H

typedef struct CharTableStruct{
  unsigned char AsciiIdx; /* Ascii index of character */
  unsigned char CharData; /* Representation of Ascii character in segment according to Segment definition below*/
}CharTableStruct;


//---< Segment definition >---
/*******************************
*              222             *
*            1    3            *
*            1    3            *
*            1    3            *
*             777              *
*            6    4            *
*            6    4            *
*            6    4            *
*             555              *
*******************************/

/*
  CharTable holds character that are possible show on display. The table can be user extended with other Ascii characters.
  User can define Ascii representation of an character in a segment by add a row to the table before TableEnd row.
*/
CharTableStruct  CharTable[] = {
                 /* AsciiIdx , CharData*/     /* SEG: 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 */
                  {        '0' ,     0xFC },  /* BIT: 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 */
                  {        '1' ,     0x30 },  /* BIT: 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 */
                  {        '2' ,     0x6E },  /* BIT: 0 , 1 , 1 , 0 , 1 , 1 , 1 , 0 */
                  {        '3' ,     0x7A },  /* BIT: 0 , 1 , 1 , 1 , 1 , 0 , 1 , 0 */
                  {        '4' ,     0xB2 },  /* BIT: 1 , 0 , 1 , 1 , 0 , 0 , 1 , 0 */
                  {        '5' ,     0xDA },  /* BIT: 1 , 1 , 0 , 1 , 1 , 0 , 1 , 0 */
                  {        '6' ,     0xDE },  /* BIT: 1 , 1 , 0 , 1 , 1 , 1 , 1 , 0 */
                  {        '7' ,     0x70 },  /* BIT: 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 */
                  {        '8' ,     0xFE },  /* BIT: 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 */
                  {        '9' ,     0xF2 },  /* BIT: 1 , 1 , 1 , 1 , 0 , 0 , 1 , 0 */
                  {        ' ' ,     0x00 },  /* BIT: 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 */
		  {	   'O' ,     0x4F },  /* BIT: 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 */
                  {        'F' ,     0xC6 },  /* BIT: 1 , 1 , 0 , 0 , 0 , 1 , 1 , 0 */
                  {        'n' ,     0x16 },  /* BIT: 0 , 0 , 0 , 1 , 0 , 1 , 1 , 0 */
                  {        '_' ,     0x08 },  /* BIT: 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 */
                  {        '-' ,     0x02 },  /* BIT: 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 */
                  {        'H' ,     0xB6 },  /* BIT: 1 , 0 , 1 , 1 , 0 , 1 , 1 , 0 */
                  {        'i' ,     0x04 },  /* BIT: 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 */
		  { 	   'C' ,     0x43 },  /* BIT: 1 , 1 , 0 , 0 , 1 , 1 , 0 , 0 */
		  { 	   '°' ,     0xB0 },  /* BIT: 1 , 1 , 1 , 0 , 0 , 0 , 1 , 0 */
    /* TableEnd*/ {       0xFF ,     0xFF }
};


/* Segment Index: 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |   */
/* 88:88    ->    8 | 8 | : | 8 | 8 |   |   |   |   */
/* 88:88:88 ->    8 | 8 | : | 8 | 8 | : | 8 | 8 |   */
/* 888888   ->    8 | 8 | 8 | 8 | 8 | 8 |   |   |   */
/* 888.888  ->    8 | 8 | 8 | . | 8 | 8 | 8 |   |   */

#endif