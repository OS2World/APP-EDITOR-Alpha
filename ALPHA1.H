/*********** Alpha C Program Include file (.H) ****************************/
/*                                                                        */
/*  The Alpha V3.0 include file defines the structures used               */
/*  in the Alpha1.C etc. and .RC files.                                   */
/*                                                                        */
/*  Lionel de Lambert - 28 Jul 1994 - IBM, Hursley                        */
/*                                                                        */
/**************************************************************************/


#define Max_record_size 256        /* also in Alpha3.asm                  */
#define PATHSZ          256        /* Maximum path size                   */
#define PREVFILES       16         /* No. of save file names              */
#define Max_elm        1000      /* maximum No. elements in structure blk.*/
#define Str_max_nm      24         /* maximum name length for structure   */
#define Str_nm_entr        2       /* number of entries in name           */
                            /* number of entries in structure continuation*/
#define Str_co_entr (Str_max_nm/4)+ Str_nm_entr  
#define Max_modify_rec 64          /* Maximum No. records in modify block */
#define STRINGSZ       32          /* Size of Highlighted string etc.     */
        
struct Toolbar_data {          
      LONG ID;                     /* Menu id                             */
      LONG Button_ID;              /* Bitmap id                           */
      HWND Button;                 /* Button window handle                */
      HWND Bhwnd;                  /* button bit map handle               */
      char H_data[24];             /* Help text                           */
     };

struct rec_elm {                   /* record element - see ALPHA3.ASM     */
                 char *rec;        /* address of record                   */
                 long length;      /* Length of record                    */
                 unsigned Modified :1; /* pointer to Modification entry   */
                 unsigned Continued :1; /* Record is continued            */
                 unsigned Filea     :1; /* Record is unique to file a     */
                 unsigned Fileb     :1; /* Record is unique to file b     */
                 unsigned strcont   :3; /* 0, 1-4 for string continued    */ 
                                     /* from this record to the following */
               };                  /* the length will be 12 bytes         */

#define records_prefix 12          /* length of prefix part of block      */
struct records {            /* record pointer block - see ALPHA3.ASM      */
                 struct records *next;  /* next record block              */
                 int    Max_elemt; /* Maximum No. elements in block       */
                 long   No_elm;    /* number of elements                  */
                 struct rec_elm record_elm[20000];
               };

struct Recd_elmt {                 /* Update record element               */
                  int length;      /* length of record                    */
                  char rec[Max_record_size]; /* new record                */
                 };

struct Modifyblk {                 /* Update record block                 */
                  struct Modifyblk *nxt; /* next modification block       */
                  int Max_rec;     /* maximum No. records in block        */
                  int No_rec;      /* Actual No. records in block         */
                  struct Recd_elmt entry[Max_modify_rec]; /* record entries*/
                 };

struct Label_flgs {                /* flags for label entry etc.          */
                    unsigned flcn : 1; /* continuation entry              */
                    unsigned flse : 1; /* procedure entry                 */
                    unsigned flsc : 1; /* procedure call                  */
                    unsigned flsd : 1; /* label defined here              */

                  };

struct Label_state {               /* label statement number entry        */
                    short int stmtn;  /* statement number                 */
                    struct Label_flgs flag; /* entry flags                */
                   };

union label_nm {
             char name[Str_max_nm];    /* label name                      */
             struct Label_state stmt[Str_max_nm/4]; /* statement number & flags */
            };

struct Label_ent {                 /* label entry                         */
                 struct Label_con *cont; /* pointer to continuation       */
                 int nxfr;         /* next free index                     */
                 struct Label_flgs flag; /* entry flags                   */
                 short int naml;    /* name length                        */
                 union label_nm statemnt;
                 struct Label_state stmtc[Str_nm_entr]; /* statement number & flags */
                 };

struct Label_con {                 /* label continuation                  */
                 struct Label_con *cont; /* pointer to continuation       */
                 int nxfr;         /* next free index                     */
                 struct Label_flgs flag; /* entry flags                   */
                 short int naml;    /* name length                        */
                 struct Label_state stmt[Str_co_entr]; /* record statement # entries*/
                 };

struct Label_pt {                  /* label block                         */
                 struct Label_pt *nxt; /* next block or 0                 */
                 int Num;        /* number of label entries in this block */
                 struct Label_ent entr[Max_elm]; /* label entries         */
                };

struct Backout {                   /* Structure for backout records       */  
             struct Backout *next; /* pointer to next backout block       */ 
             int start_y;          /* start record #                      */ 
             int start_x;          /* start character position            */ 
             int end_y;            /* end record #                        */ 
             int end_x;            /* end character position              */ 
             char *recds;          /* pointer to records                  */
             int len;              /* Length of storage                   */
             unsigned select_mode : 1;  /* 1 => Box mode select           */
             unsigned insert_mode : 1;  /* 1 => in insert mode            */
             unsigned in_use : 1;  /* 1 => The block is active            */
              };      

struct Bookmark {                  /* Bookmark structure                  */
               int mark;           /* Line number                         */
               char name[16];      /* bookmark name                       */
                };

typedef struct child {        
                 struct child * next; /* Next child block or 0            */
                 HWND hwnd;        /* Client widow handle                 */
                 HWND Fhwnd;       /* frame handle                        */
                 HFILE Fhand;      /* file handle                         */
                 HWND hwndPopMenu; /* Pop-up menu window                  */
                 char *buffer;     /* pointer to file buffer              */
                 int lan;          /* language A37=1.ASM=2,etc.           */
                 long file_size;   /* number of characters in file        */
                 int rec_num;      /* current record number               */
                 int rec_last;     /* last rec. # used for Window         */
                 int Offset;       /* horizontal offset                   */
                 int tot_rec;      /* total number of records in file     */
                 int ySel_s;       /* Rec# start of Select area           */
                 int ySel_e;       /* Rec# end of Select area             */
                 int xSel_s;       /* Chr# start of select area           */
                 int xSel_e;       /* Chr# end of select area             */
                 int xMouse_l;     /* Last Mouse x coord.                 */
                 int yMouse_l;     /* Last Mouse y coord.                 */
                 int xEnd;         /* character end of Mouse button1 area */
                 int Hide_s;       /* Rec# start of hide area             */
                 int Hide_e;       /* Rec# end of hide area               */
                 int Pair_s;       /* start of BEGIN/END Pair area        */
                 int Pair_e;       /* end of Pair area                    */
                 int find_s_x;     /* x coord. of start of BEGIN pair     */
                 int menu_id;      /* window menu id of child if used     */
                 int find_s_y; /* y coord. of start of BEGIN pair - offset*/ 
                 int NoChng;       /* Number of changes to file           */ 
                 int column;       /* Column number for grig line or 0    */ 
                 char *OrigPath;   /* original file path                  */
                 struct Bookmark Mark[10]; /* Bookmark line numbers etc.  */
                 struct records *rec_array; /* pointer to records array   */
                 struct Label_pt *structure; /* pointer to structure entries*/
                 struct Modifyblk *Modblk; /* pointer to modification block */
                 struct Backout * pBackout; /* pointer to backout block   */
                 unsigned Expand_child : 1;  /* set if expand window      */
                 unsigned Select : 1;  /* set if button1 drag area        */
                 unsigned Hide : 1;  /* set if hiding records             */
                 unsigned Pair : 1;  /* set if PAIRing records            */
                 unsigned erase_back :1; /* set if background to be erased */
                 unsigned Button1_down :1; /* Mouse button 1 down         */
                 unsigned Cmt_supr :1; /* Set if comment suppression      */
                 unsigned Save :1; /* Set if Save is required             */
                 unsigned Insert :1; /* Set if Insert mode - edit         */
                 unsigned Composite :1; /* Set if Composite from compare  */
                 unsigned ReadingFile :1; /* Set if still reading data    */    
                 unsigned ParsingFile :1; /* Set if still parsing data    */
                 unsigned InSelect :1; /* Set if button 1 in select area  */ 
               } CHILD;            /* End of child structure              */

#define sizeof_lex 256
struct lex {                       /* language table entry                */
           int len;                /* length of key word                  */
           long col;               /* colour                              */
           long backcol;           /* background colour                   */
           unsigned underline :1;  /* use underline font                  */
           char *kwd;              /* key word                            */
           };


struct stat {
            unsigned tile :1;      /* =1 if tile was last window operation */ 
            unsigned saving :1;    /* =1 if currently saving a file        */ 
            unsigned compiling :1; /* =1 if currently compiling a file     */ 
            };


struct OPR_Flg {                   /* operator table flags                */
                unsigned OPR_F_P :1; /* procedure                         */
                unsigned OPR_F_C :1; /* call                              */
                unsigned OPR_F_L :1; /* scan left                         */
                unsigned OPR_F_R :1; /* scan right                        */
                unsigned OPR_F_Co :1; /* label followed by colon          */
                unsigned OPR_F_2 :1; /* second parameteer                 */
               };

struct LANGUAGE {
            char strs[9];          /* String start pairs - 1st. two comment */
            char stre[9];          /* string end pairs                    */  
            char Esc;              /* Escape character or 0               */
            int cmtpos;            /* 0 or chr. pos. of fixed comment     */
            int Nolex;             /* No. of keyword entries              */
            struct lex * lex_ent;  /* Pointer to keyword  lex entries     */
            unsigned tab[2];       /* Four tab values 1-255               */
            unsigned Case    : 1;  /* case sensitive                      */
            unsigned def     : 1;  /* default language type - Algol       */
            unsigned none    : 1;  /* no comment parsing etc.             */
            unsigned A37     : 1;  /* 370 assembler                       */
            unsigned FORTRAN : 1;  /* FORTRAN                             */
            unsigned C       : 1;  /* C language                          */  
            unsigned ASM     : 1;  /* MASM                                */
                };
                                   /* operator table                      */
struct OPR {
            short int Lang;        /* Language or -1                      */
            struct OPR_Flg OFLAG;  /* Flags                               */
            short int Nm_len;      /* name length                         */
            char  Nm[12];          /* operator name                       */
           };

#define No_Cmd 3                   /* Number of commands                  */
#define Cmd_size 256               /* maximum length of commands          */

struct Command {                   /* Command array for compile           */
             char Cmd[No_Cmd][Cmd_size]; /* command strings               */
           };

struct Interval {                  /* text intervals                      */
           struct Interval *fwd;   /* forward pointer                     */
           int a;                  /* interval start                      */
           int b;                  /* interval end                        */
           COLOR fg;               /* forground colour                    */
           COLOR bg;               /* background colour                   */
           unsigned underline :1;  /* use underline font                  */
           unsigned string :1;     /* string interval                     */
                };
                                                         
struct COLR {
             COLOR fg;             /* forground colour                    */ 
             COLOR bg;             /* background colour                   */ 
             unsigned underline :1; /* use underline font                 */  
            };                                                                
                   
#define Max_trace_lvl 16           /* depth of trace level                */
struct Trace_Ent {                 /* trace entry for debug               */
             int Module;           /* Module number                       */
             int Line;             /* Line number                         */
             unsigned Flags;       /* 0x1 - entry, 0x2 - exit, 0x3 - data */
             unsigned Data;        /* data field                          */
                 };

typedef struct _RCD {              /* Msg. entry for record playback      */ 
             HWND hwnd;
             ULONG msg;
             MPARAM mp1;
             MPARAM mp2;   
            } RCD;    
                                   /* Procedure declarations              */
       
/***********************************************************************/
/*                                                                     */
/* block of main program parameters                                    */
/* the variable that points to this block is usually named pThreadParm */
/***********************************************************************/

typedef  struct _MAIN_PARM {
    HAB        hab;                /* hab obtained on thread 1                */
    HWND       hwndFrame;          /* frame handle                            */
    HWND       hwndClient;         /* client window handle                    */
    HWND       hwndObject;         /* object window handle on thread 2        */
    TID        tidObjectThread;    /* thread id for object window         */
} MAIN_PARM;
typedef MAIN_PARM *PMAIN_PARM;

/* end of file - ALPHA1.H                                                  */

