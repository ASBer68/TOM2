#ifndef DBactionH
#define DBactionH
#include "..\MySQL\mysql.h"
//---------------------------------------------------------------------------
class sql
{ //����� ��� �������� � MySQL
  static bool ErrFlag; //������� ����
  MYSQL_RES*RES;
  MYSQL_ROW ROW;
  public:
  bool Error;          //���������� ����
  bool __fastcall Next(void);
  const char* __fastcall operator[](int i){ return ROW[i]; };
  //�����������/����������
  __fastcall sql(const AnsiString&TextSQL,bool&Flag=ErrFlag);
  __fastcall~sql();
};


//---------------------------------------------------------------------------
#endif
