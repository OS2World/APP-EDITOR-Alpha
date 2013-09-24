/*************** AlphaE.C - Exit procedure for Alpha  *********************/
/*                                                                        */                
/* PROGRAM NAME: ALPHA.EXE                                                */
/* -------------                                                          */                
/*  Presentation Manager  C Program                                       */
/*                                                                        */                
/* REVISION LEVEL: 3.1                                                    */
/* ---------------                                                        */
/*                                                                        */
/* Lionel de Lambert - July 1994 - IBM - Hursley                          */
/*                                                                        */
/* WHAT THIS PROGRAM DOES:                                                */
/* -----------------------                                                */     
/*    Procedures -                                                        */
/*                 Alpha_parse_exit                                       */
/*                                                                        */
/*    This procedure is called before Alpha writes the line of text.      */
/*    It is called from the WM_PAINT section of the child window.         */
/*    return code - 0 - Execute standard Alpha parsing etc.               */
/*                  4 - Don't excute Alpha parsing - this routine has     */
/*                      output the text.                                  */
/*                                                                        */
/* WHAT YOU NEED TO COMPILE THIS PROGRAM:                                 */
/* --------------------------------------                                 */
/*                                                                        */
/*  REQUIRED FILES:                                                       */
/*  ---------------                                                       */     
/*                                                                        */     
/*    ALPHAE.C      - Source code                                         */
/*    ALPHAE.DEF    - Module definition file                              */
/*    ALPHA.H       - Application header file                             */
/*                                                                        */
/*    OS2.H          - Presentation Manager include file                  */     
/*    STRING.H       - String handling function declarations              */     
/*                                                                        */     
/*  REQUIRED LIBRARIES:                                                   */                                                  
/*  -------------------                                                   */     
/*                                                                        */     
/*    OS2386.LIB     - Presentation Manager/OS2 library                   */
/*                                                                        */
/* Use C/SET2 compiler, e.g.                                              */
/* icc /Ge- /Fealphae.dll alphae.c alphae.def                             */
/*                                                                        */
/*  REQUIRED PROGRAMS:                                                    */     
/*  ------------------                                                    */     
/*    IBM C Compiler - icc                                                */                                                                          
/*    IBM Linker                                                          */
/*                                                                        */     
/**************************************************************************/
     
#define INCL_BASE
#define INCL_WIN
#define INCL_GPI
#define INCL_WINWINDOWMGR     

#include <os2.h>                   /* PM header file                      */
#include <stdio.h>                 /* standard io                         */
#include <CTYPE.H>
#include <MALLOC.H>
#include <STDLIB.H>
#include <string.h>

/* External - global varialbles                                           */

#include "ALPHA1.h"                 /* Structure definitions              */


#pragma subtitle("Alpha_parse_exit")
#pragma page(1)

APIRET Alpha_parse_exit( HAB hab, FATTRS fat, FONTMETRICS fm,
                       CHILD *pChild, char *ptr,
                      int l, char *ptr0,
                      int l0, int Cmt_s, int Cmt_e,
                      int Str_s, int Str_e, int recno,
                      POINTL pAPTL[Max_record_size] )
{
/* hab       - PM anchor block handle                                     */
/* fat       - Attributes of font                                         */
/* fm        - Currently selected font metrics                            */
/* pChild    - pointer to child structure                                 */
/* ptr       - pointer to text string including offset                    */
/* l         - length of record less offset                               */
/* ptr0      - pointer to complete record                                 */
/* l0        - length of complete record                                  */
/* Cmt_s     - start position of comment in record or 0                   */
/* Cmt_e     - end position of comment in record or 0 - continued         */
/* Str_s     - Start position of string in record or 0                    */
/* Str_e     - End position of string in record or 0                      */
/* recno     - record number                                              */
/* pAPTL     - pointer to array of character positions                    */
/*                                                                        */
/* return code - 0 - Execute standard Alpha parsing                       */
/*               4 - Don't excute Alpha parsing - this routine has output */
/*                   the text.                                            */


 int e, i, m, p, q, s, w;
 int cmt_start;
 POINTL CurPos;                    /* Current end of line position        */
 USHORT Left, Right = FALSE;       /* State of end of keywords            */
 char Upp_rec[Max_record_size];    /* upper case copy of record           */
 RECTL rc;
 COLOR Text_clr = CLR_DARKBLUE;
 COLOR Back_clr = CLR_WHITE;
 COLOR Select_clr = CLR_RED;
 COLOR Cmmt_Back_clr = CLR_GREEN;
 COLOR Cmmt_clr = CLR_BLACK;
 int Chr_height = 20;
 HPS   Chps;                       /* Presentation Space handle           */
 ERRORID  erridErrorCode;




 Chps = WinGetPS(pChild -> hwnd);

 GpiCreateLogFont( Chps, NULL, 1L, &fat ); /* create font 1               */
 GpiSetCharSet( Chps , 1L );      /* select font                          */

 Chr_height =   fm.lMaxBaselineExt; /* chr. height                        */

 WinQueryWindowRect( pChild -> hwnd, &rc ); /* reset window  rectangle    */


 GpiSetBackMix( Chps, BM_OVERPAINT ); /*                                  */
 GpiSetMix( Chps, FM_OVERPAINT );   /*                                    */
 GpiSetColor( Chps, Text_clr );     /* colour of the text part            */
 GpiSetBackColor( Chps, Back_clr ); /* colour of the text background      */


/*                 |----select------|                                     */
/*     -------------------------------------------------------            */
/*     |  text               |           comment             |            */
/*     -------------------------------------------------------            */
/*     a           b         c      d                        e            */
/*                                                                        */

 cmt_start = Cmt_s;
 if (cmt_start >= 1)               /* =0 if no comment part               */
  if (cmt_start>pChild -> Offset)  /*                                     */
   {                               /* reset comment start from offset     */
    cmt_start = cmt_start - pChild -> Offset;  /*                         */
   }                               /*                                     */
  else cmt_start = 1;              /* offset beyond comment start         */


 if (cmt_start>0)
    {
      GpiCharStringAt( Chps, &pAPTL[0], cmt_start, ptr ); /* write text   */

                                   /* write out the comment part          */
                                   /* colour of the comment background    */
      GpiSetBackColor( Chps, Cmmt_Back_clr ); /*                          */
      GpiSetColor( Chps, Cmmt_clr ); /* colour of the comment text part   */
      GpiCharStringAt( Chps, &pAPTL[cmt_start-1],l-cmt_start+1,
                              &ptr[cmt_start-1] );
    }
 else
    {
     GpiCharStringAt( Chps, &pAPTL[0], l, ptr ); /* write text            */
    };
                                   /* Need to fill rest of line           */
 GpiQueryCurrentPosition( Chps, &CurPos );  /* get end of line            */
 WinQueryWindowRect( pChild -> hwnd, &rc ); /* get window  rectangle      */
 rc.xLeft = CurPos.x;
 rc.yBottom = CurPos.y - fm.lMaxDescender;
 rc.yTop = CurPos.y + Chr_height - fm.lMaxDescender;
 WinFillRect( Chps, &rc, CLR_BACKGROUND ); /* fill rectangle              */

 GpiSetCharSet(Chps, 0L);          /* release the font before deleting    */
 GpiDeleteSetId(Chps, 1L);         /* delete the logical font             */  

 WinReleasePS(Chps);

 return (4);                       /* Don't call Alpha parsing            */
};                                 /* end of Alpha_parse_exit             */

