#ifndef DBactionH
#define DBactionH
#include "..\MySQL\mysql.h"
//---------------------------------------------------------------------------
class sql
{ //класс для запросов к MySQL
  static bool ErrFlag; //внешний флаг
  MYSQL_RES*RES;
  MYSQL_ROW ROW;
  public:
  bool Error;          //внутренний флаг
  bool __fastcall Next(void);
  const char* __fastcall operator[](int i){ return ROW[i]; };
  //конструктор/деструктор
  __fastcall sql(const AnsiString&TextSQL,bool&Flag=ErrFlag);
  __fastcall~sql();
};


//---------------------------------------------------------------------------
#endif
