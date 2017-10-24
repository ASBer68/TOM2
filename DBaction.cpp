#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#pragma hdrstop
#include "MainForm.h"
#include "DBaction.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
void __fastcall TMForm::NewDatabase(TObject*Sender)
{ //������ ����� ���� ������
  AnsiString DbName;
  if(InputStr(DbName,"������� ��� ���� ������","�������"))
  {
    AnsiString SQL="create database ";
    SQL+=DbName;
    SQL+=" character set = cp1251";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    DBList->Items->Add(DbName);
    //���������� ��������� ����
    SQL="use ";
    SQL+=DbName;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //�������� ��� ������� ����
    CurrDB=DbName;

    //������ ����������� �������
    //������ ����
    if(mysql_query(&DB,
      "CREATE TABLE word("
      "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
      "name VARCHAR(30) NOT NULL UNIQUE KEY," //��� ����� (����������)
      "main_form VARCHAR(30) ,"//�������� �����
      "fkey VARCHAR(12),"      //���� �����
      "defkey VARCHAR(12),"    //���� ����� �� ���������
      "status INT,"      //��������� (���������, �����������, � ������ � �.�)
      "type VARCHAR(6)," //����� ���� - ���������������, ������ � �.�.
      "class_id INT,"    //����� ����� - ���������������, ������ � �.�.
      "cluster INT,"     //����� ��������
      "code TEXT,"       //��� �����
      "obj_code TEXT,"   //��� ������������ �����
      "glb_code TEXT,"   //���������� ��� �����
      "KEY(main_form),"
      "KEY(cluster)"
      ") default charset=cp1251" //��������� �� ���������
    ))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //������ ���������
    if(mysql_query(&DB,
      "CREATE TABLE form("
      "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY," //id ����������
      "word_id INT NOT NULL,"     //id �����
      "form VARCHAR(30) NOT NULL,"//����� �����
      "search_form VARCHAR(30) NOT NULL,"//��������������� ��� ������ ����� �����
      "fkey VARCHAR(12) NOT NULL,"//���� ����� �����
      "word_cnt SMALLINT,"        //���������� ���� ��� ���� �����
      "accentuation BOOLEAN,"     //=true - ��������� ��������
      "UNIQUE KEY(word_id,fkey)," //���������� ������ �� id � ����� ����������
      "KEY(search_form),"         //�� ���������� ������ �� ����������
      "KEY(beg),"
      "KEY(end)"
      ") default charset=cp1251"  //��������� �� ���������
    ))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //������ �������� �����
    if(mysql_query(&DB,
      "CREATE TABLE meaning("
      "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
      "word_id INT NOT NULL,"      //id �����
      "name VARCHAR(50) NOT NULL," //��� ��������
      "type VARCHAR(30) NOT NULL," //��� ��������
      "class_id INT,"              //����� ��������
      "class2_id INT,"             //����� ��������
      "class3_id INT,"             //����� ��������
      "description TEXT,"          //��������
      "code TEXT,"                 //��� �������
      "hashtag VARCHAR(20),"       //��������� ����
      "UNIQUE KEY(word_id, name)"  //���������� ����
      ") default charset=cp1251"   //��������� �� ���������
    ))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //��������
    if(mysql_query(&DB,
      "CREATE TABLE synonym("
      "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
      "object_id INT NOT NULL,"
      "word_id INT NOT NULL,"
      "add_name BOOLEAN NOT NULL," //�������������� ���
      "UNIQUE KEY(object_id, word_id),"
      "KEY(object_id),"
      "KEY(word_id)"
      ") default charset=cp1251"
    ))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //����������
    if(mysql_query(&DB,
      "CREATE TABLE association("
      "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
      "object_id INT NOT NULL,"
      "type VARCHAR(3) NOT NULL," //��� ��������
      "name VARCHAR(50) NOT NULL,"//��� ��������
      "word_id INT,"              //���������� �� ������
      "code TEXT,"                //��� ��������
      "comment TEXT,"             //�����������
      "UNIQUE KEY(object_id, name),"
      "KEY(object_id),"
      "KEY(word_id)"
      ") default charset=cp1251"
    ))
    { Status->Text=mysql_error(&DB);
      return;
    }

    Status->Text="Ok, ���� ������ �������.";
  }
}

//---------------------------------------------------------------------------
void __fastcall TMForm::SelectDatabase(TObject*Sender)
{ //����� ����
  if(DBList->ItemIndex==-1) return;
  Status->Text=DBList->Items->operator[](DBList->ItemIndex);
  AnsiString SQL="use ";
  SQL+=DBList->Items->operator[](DBList->ItemIndex);
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  //�������� ���� �� ������� ����������� ������
  if(mysql_query(&DB,"show tables"))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_use_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_ROW row;
  AnsiString Word="word";
  bool Ok=false;
  for(int r=0; bool(row=mysql_fetch_row(Result)); r++)
    if(Word==row[0])
    { Ok=true; break; }
  mysql_free_result(Result);

  if(!Ok)
  { Status->Text="���� �� �������� ����������� ������!";
    //��������� ����
    if(!CurrDB.IsEmpty())
    { SQL="use ";
      SQL+=CurrDB;
      mysql_query(&DB,SQL.c_str());
//    DBList->Text=CurrDB;
    }
    return;
  }
  //��������� ������� ���� ������
  CurrDB=DBList->Items->operator[](DBList->ItemIndex);

  //������� ������ ����
  if(RefreshWordList()) Status->Text="Ok!";
}
//---------------------------------------------------------------------------
//����� ��� �������� � MySQL
bool sql::ErrFlag=false;
__fastcall sql::sql(const AnsiString&TextSQL,bool&Flag)
{ //������ � ���� ������
  RES=0;
  Error=false;
  if(mysql_query(&DB, TextSQL.c_str()))
  { MForm->Status->Text=mysql_error(&DB);
    Flag=Error=true;
    return;
  }
  RES=mysql_store_result(&DB);
  if(!RES)
  { MForm->Status->Text=mysql_error(&DB);
    Flag=Error=true;
  }
}

__fastcall sql::~sql()
{ //����������� ��������� �������
  mysql_free_result(RES);
}

bool __fastcall sql::Next(void)
{ //�������� ��������� ������
  if(Error) return false;
  return (bool)(ROW=mysql_fetch_row(RES));
}

//---------------------------------------------------------------------------

