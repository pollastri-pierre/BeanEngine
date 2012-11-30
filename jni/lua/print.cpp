/*
** $Id: print.c,v 1.55a 2006/05/31 13:30:05 lhf Exp $
** print bytecodes
** See Copyright Notice in lua.h
*/

#include <ctype.h>
#include <stdio.h>

#define luac_c
#define LUA_CORE

#include "ldebug.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lundump.h"

#include <android/log.h>

#define PrintFunction	luaU_print

#define Sizeof(x)	((int)sizeof(x))
#define VOID(p)		((const void*)(p))

static void PrintString(const TString* ts)
{
 const char* s=getstr(ts);
 size_t i,n=ts->tsv.len;
 putchar('"');
 for (i=0; i<n; i++)
 {
  int c=s[i];
  switch (c)
  {
   case '"': __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\\\""); break;
   case '\\': __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\\\\"); break;
   case '\a': __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\\a"); break;
   case '\b': __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\\b"); break;
   case '\f': __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\\f"); break;
   case '\n': __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\\n"); break;
   case '\r': __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\\r"); break;
   case '\t': __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\\t"); break;
   case '\v': __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\\v"); break;
   default:	if (isprint((unsigned char)c))
   			putchar(c);
		else
			__android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\\%03u",(unsigned char)c);
  }
 }
 putchar('"');
}

static void PrintConstant(const Proto* f, int i)
{
 const TValue* o=&f->k[i];
 switch (ttype(o))
 {
  case LUA_TNIL:
	__android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","nil");
	break;
  case LUA_TBOOLEAN:
	__android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine",bvalue(o) ? "true" : "false");
	break;
  case LUA_TNUMBER:
	__android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine",LUA_NUMBER_FMT,nvalue(o));
	break;
  case LUA_TSTRING:
	PrintString(rawtsvalue(o));
	break;
  default:				/* cannot happen */
	__android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","? type=%d",ttype(o));
	break;
 }
}

static void PrintCode(const Proto* f)
{
 const Instruction* code=f->code;
 int pc,n=f->sizecode;
 for (pc=0; pc<n; pc++)
 {
  Instruction i=code[pc];
  OpCode o=GET_OPCODE(i);
  int a=GETARG_A(i);
  int b=GETARG_B(i);
  int c=GETARG_C(i);
  int bx=GETARG_Bx(i);
  int sbx=GETARG_sBx(i);
  int line=getline(f,pc);
  __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t%d\t",pc+1);
  if (line>0) __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","[%d]\t",line); else __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","[-]\t");
  __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","%-9s\t",luaP_opnames[o]);
  switch (getOpMode(o))
  {
   case iABC:
    __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","%d",a);
    if (getBMode(o)!=OpArgN) __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine"," %d",ISK(b) ? (-1-INDEXK(b)) : b);
    if (getCMode(o)!=OpArgN) __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine"," %d",ISK(c) ? (-1-INDEXK(c)) : c);
    break;
   case iABx:
    if (getBMode(o)==OpArgK) __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","%d %d",a,-1-bx); else __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","%d %d",a,bx);
    break;
   case iAsBx:
    if (o==OP_JMP) __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","%d",sbx); else __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","%d %d",a,sbx);
    break;
  }
  switch (o)
  {
   case OP_LOADK:
    __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t; "); PrintConstant(f,bx);
    break;
   case OP_GETUPVAL:
   case OP_SETUPVAL:
    __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t; %s", (f->sizeupvalues>0) ? getstr(f->upvalues[b]) : "-");
    break;
   case OP_GETGLOBAL:
   case OP_SETGLOBAL:
    __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t; %s",svalue(&f->k[bx]));
    break;
   case OP_GETTABLE:
   case OP_SELF:
    if (ISK(c)) { __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t; "); PrintConstant(f,INDEXK(c)); }
    break;
   case OP_SETTABLE:
   case OP_ADD:
   case OP_SUB:
   case OP_MUL:
   case OP_DIV:
   case OP_POW:
   case OP_EQ:
   case OP_LT:
   case OP_LE:
    if (ISK(b) || ISK(c))
    {
     __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t; ");
     if (ISK(b)) PrintConstant(f,INDEXK(b)); else __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","-");
     __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine"," ");
     if (ISK(c)) PrintConstant(f,INDEXK(c)); else __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","-");
    }
    break;
   case OP_JMP:
   case OP_FORLOOP:
   case OP_FORPREP:
    __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t; to %d",sbx+pc+2);
    break;
   case OP_CLOSURE:
    __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t; %p",VOID(f->p[bx]));
    break;
   case OP_SETLIST:
    if (c==0) __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t; %d",(int)code[++pc]);
    else __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t; %d",c);
    break;
   default:
    break;
  }
  __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\n");
 }
}

#define SS(x)	(x==1)?"":"s"
#define S(x)	x,SS(x)

static void PrintHeader(const Proto* f)
{
 const char* s=getstr(f->source);
 if (*s=='@' || *s=='=')
  s++;
 else if (*s==LUA_SIGNATURE[0])
  s="(bstring)";
 else
  s="(string)";
 __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\n%s <%s:%d,%d> (%d instruction%s, %d bytes at %p)\n",
 	(f->linedefined==0)?"main":"function",s,
	f->linedefined,f->lastlinedefined,
	S(f->sizecode),f->sizecode*Sizeof(Instruction),VOID(f));
 __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","%d%s param%s, %d slot%s, %d upvalue%s, ",
	f->numparams,f->is_vararg?"+":"",SS(f->numparams),
	S(f->maxstacksize),S(f->nups));
 __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","%d local%s, %d constant%s, %d function%s\n",
	S(f->sizelocvars),S(f->sizek),S(f->sizep));
}

static void PrintConstants(const Proto* f)
{
 int i,n=f->sizek;
 __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","constants (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t%d\t",i+1);
  PrintConstant(f,i);
  __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\n");
 }
}

static void PrintLocals(const Proto* f)
{
 int i,n=f->sizelocvars;
 __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","locals (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t%d\t%s\t%d\t%d\n",
  i,getstr(f->locvars[i].varname),f->locvars[i].startpc+1,f->locvars[i].endpc+1);
 }
}

static void PrintUpvalues(const Proto* f)
{
 int i,n=f->sizeupvalues;
 __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","upvalues (%d) for %p:\n",n,VOID(f));
 if (f->upvalues==NULL) return;
 for (i=0; i<n; i++)
 {
  __android_log_print(ANDROID_LOG_DEBUG, "NBeanEngine","\t%d\t%s\n",i,getstr(f->upvalues[i]));
 }
}

void PrintFunction(const Proto* f, int full)
{
 int i,n=f->sizep;
 PrintHeader(f);
 PrintCode(f);
 if (full)
 {
  PrintConstants(f);
  PrintLocals(f);
  PrintUpvalues(f);
 }
 for (i=0; i<n; i++) PrintFunction(f->p[i],full);
}
