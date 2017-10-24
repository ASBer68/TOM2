#pragma hdrstop
#include "MainForm.h"
#include "DBaction.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
AnsiString MeaningId;     //Id �������� ��������
AnsiString MeaningType;   //��� �������� ��������
AnsiString SynonymId;     //Id �������� ��������
AnsiString PropertyId;    //Id �������� ��������
char*MeaningsTypes[]={ "class", "attribute", "variable", "act", "action",
                       "act_syn", "fact", "fact_syn", "expression", "exp_syn",
                       "location", 0 };
//---------------------------------------------------------------------------
//��������� �������� ��������
bool __fastcall TMForm::RefreshCurrMeaning(void)
{ ClassTree->Items->Clear();
  AssocList->Items->Clear();
  MeaningId    = "";
  SynonymId    = "";
  PropertyId   = "";
  CurrProperty = "";

  if(CurrMeaning.IsEmpty() || CurrMeaType<0) return false;
  //--------------------------------------------------
  //��������� ������� ��������
  AnsiString SQL=
  "SELECT meaning.word_id, meaning.id, "               // 0, 1,
  " meaning.type, meaning.description, meaning.code, " // 2, 3, 4
  " Cls1.name, Cls2.name, Cls3.name, Cls4.name, "      // 5, 6, 7, 8
  " meaning.status, Cls1.description, meaning.hashtag "// 9, 10, 11
  "FROM meaning "
  "LEFT JOIN meaning AS Cls1 ON Cls1.id=meaning.class_id "
  "LEFT JOIN meaning AS Cls2 ON Cls2.id=meaning.class2_id "
  "LEFT JOIN meaning AS Cls3 ON Cls3.id=meaning.class3_id "
  "LEFT JOIN meaning AS Cls4 ON Cls4.id=meaning.part_id "
  "WHERE meaning.name = '";
  SQL+=CurrMeaning;
  SQL+="' AND meaning.type = '";
  SQL+=MeaningsTypes[CurrMeaType];
  SQL+="'";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return false;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return false;
  }
  MYSQL_ROW row;
  if(!bool(row=mysql_fetch_row(Result)))
  { Status->Text=Name + " - �������� �� �������!";
    mysql_free_result(Result);
    return false;
  }
  //�������� Id ����� � �������� ��������
  WordId = row[0];
  MeaningId = row[1];
  //������� ��� ��������
  if(row[2]) MeaningType = row[2];
  else       MeaningType = "NULL";
  MeaningTypeButton->Caption = MeaningType;
  //������� 1� �����
  if(row[5]&&row[5][0])
    MeaningClassButton->Caption = row[5];
  else
    MeaningClassButton->Caption = "�����...";
  //������� 2� �����
  if(row[6]&&row[6][0])
    MeaningClass2Button->Caption = row[6];
  else
    MeaningClass2Button->Caption = "����� 2...";
  //������� ������� (3� �����)
  if(row[7]&&row[7][0])
    MeaningClass3Button->Caption = row[7];
  else
    MeaningClass3Button->Caption = "�������...";
  //������� ����� (4� �����)
  if(row[8]&&row[8][0])
    MeaningClass4Button->Caption = row[8];
  else
    MeaningClass4Button->Caption = "�����...";

  //������� ������
  M_ChBox->Checked = row[9];

  //������� ����������� / ��������� �������
  if
  ( strcmp(row[2], "act")!=0 &&
    strstr("action; act_syn; fact; fact_syn; expression; exp_syn;", row[2])
  ) CodeLabel1->Caption = "���������:";
  else
    CodeLabel1->Caption = "�����������:";
  MeaningDscrMemo->Text = row[3];

  //������� ���
  if
  ( strcmp(row[2], "act")!=0 && strcmp(row[2], "fact")!=0 &&
    strstr("act_syn; fact_syn; exp_syn;", row[2])
  )
  { CodeLabel2->Caption = "��������� �������� �������:";
    MeaningCodeMemo->Text = row[10]; //��������� ��������
  }
  else
  { CodeLabel2->Caption = "������:";
    MeaningCodeMemo->Text = row[4]; //���
  }
  //������� ����
  TagsMemo->Text = row[11]; //����

  mysql_free_result(Result);

  //������ id ��������
  if(!CurrSynonym.IsEmpty())
  { SQL =
      "SELECT synonym.id FROM synonym "
      "JOIN word ON word_id = word.id "
      "WHERE object_id = ";
      SQL+=MeaningId;
      SQL+=" AND word.name = '";
      SQL+=CurrSynonym + "'";
    sql Q(SQL);
    if(Q.Next()) SynonymId = Q[0]; //�������� Id ��������
  }
/*
  //--------------------------------------------------
  //������� ������ �������
  ClassTree->Items->BeginUpdate();
  AnsiString OldRootClassId = RootClassId;
  if(GetRootClass())
  { TTreeNode*Root=ClassTree->Items->Add(0,RootClassName);
    AddClassItemsToTree(Root,RootClassId,0);
    Root->AlphaSort(true);
  }
  else ClassTree->Items->Clear(); //��� ������
  ClassTree->Items->EndUpdate();
  //--------------------------------------------------
*/
  RefreshClassTree();
  RefreshPropertyList();
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TMForm::RefreshPropertyList(void)
{ //������� ������ ������� � �������
  AssocList->Items->Clear();
  if(MeaningId.IsEmpty()) return true;
  //������� �������� � ������
  AnsiString SQL =
  "SELECT association.id, association.name, association.type, "
  "LEFT(association.code,15), word.name "
  "FROM association "
  "JOIN meaning ON object_id =  meaning.id "
  "LEFT JOIN word ON association.word_id = word.id "
  "WHERE object_id = ";
  SQL+=MeaningId;
  sql Q(SQL);
  while(Q.Next())
  { TListItem*Str=AssocList->Items->Add();
    Str->Caption = Q[1]; //��� ��������
    for(int i=2; i<=5; i++) Str->SubItems->Add(Q[i]);
  }
  return !Q.Error;
}

//---------------------------------------------------------------------------
AnsiString RootPath; //���� � �����
bool __fastcall TMForm::RefreshClassTree(void)
{ if(MeaningId.IsEmpty()) return false;

  ClassTree->Items->BeginUpdate();
  ClassTree->Items->Clear();

  //������� �������� ����� ��� �������� ��������
  RootPath="/";
  AnsiString RootName, RootId;
  AnsiString SQL, CurrId = MeaningId; //Id �������� ��������
  int Deep=0;
  for(; Deep>-10; Deep--) //������� ������ �� ����� 10
  { SQL="SELECT class_id, class2_id, class3_id, part_id, name " //0, 1, 2, 3, 4
        "FROM meaning "
        "WHERE id = ";
    SQL+= CurrId;
    sql Q(SQL);
    if(Q.Next())
    { RootId = CurrId;
      RootName = Q[4];
      RootPath+=CurrId+"/";
      if     (Q[0]) CurrId = Q[0];
      else if(Q[1]) CurrId = Q[1];
      else if(Q[2]) CurrId = Q[2];
      else if(Q[3]) CurrId = Q[3];
      else break;
    }
    else break;
  }
  //��������� ������ � ��������� � ���� ������
  TTreeNode*Root=ClassTree->Items->Add(0, RootName);
  AddClassItemsToTree(Root, RootId, Deep);
  Root->AlphaSort(true);

  ClassTree->Items->EndUpdate();
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TMForm::AddClassItemsToTree( TTreeNode*ClsNode,
                                             const AnsiString&ClsId, int Deep )
{ if(Deep > 3) return; //������������ ������� ������
  //������� �������� ������ � ������
  AnsiString SQL =
    "SELECT id, name, type, hashtag, class3_id, part_id " // 0, 1, 2, 3, 4, 5
    "FROM meaning "
    "WHERE class_id = ";
    SQL+=ClsId;
    SQL+=" OR class2_id = ";
    SQL+=ClsId;
    SQL+=" OR class3_id = ";
    SQL+=ClsId;
    SQL+=" OR part_id = ";
    SQL+=ClsId;
    SQL+=" ORDER BY name, type";
  sql Q(SQL);
  while(Q.Next())
  {
    if(!RootPath.Pos(AnsiString("/")+Q[0]+"/"))
    { //��� ������� ����� ���������� ������ ���� � �����
      if(Deep<-2) continue;
      //������ �� �������
      if(!FindWord->Text.IsEmpty() && FindWord->Text[1]=='#')
      { AnsiString Tags=Q[3];
        if(!Tags.Pos(FindWord->Text)) continue;
      }
    }

    //������� ����
    AnsiString Name = Q[1];
    if(ClsId==Q[4]) Name+=" (�������)";
    if(ClsId==Q[5]) Name+=" (�����)";
    TTreeNode*Node=ClassTree->Items->AddChild(ClsNode,Name);
    //�������� ������������
    for(int i=0; MeaningsTypes[i]; i++)
      if(strcmp(MeaningsTypes[i],Q[2])==0)
      { Node->ImageIndex = i;
        Node->SelectedIndex = i;
        break;
      }
    //������� �����
    AddClassItemsToTree(Node,Q[0],Deep+1);
    Node->AlphaSort(true);
    //������� ������� ����
    if(MeaningId==Q[0])
    { ClassTree->Select(Node,TShiftState()); //ssShift?
      Node->Expanded = true;
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TMForm::NewMeaning(TObject*Sender)
{ if(WordId.IsEmpty()) return; //����� ������ ���� �������
  //��������� ����� ����� ��������
  AnsiString Name=CurrWord, Type="class";
  Status->Text="������� ����� ��������...";
  if(InputStr(Name,"������� ��� ��������","��������"))
  { //�������� ���
    InputStr(Type,"������� ��� ��������",0);
    //������� ��������
    AnsiString SQL="INSERT INTO meaning SET name = '";
    SQL+=Name + "', type = '";
    SQL+=Type + "', word_id = ";
    SQL+=WordId;

    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    Status->Text="Ok, �������� ���������";
  }
  else Status->Text="";

  RefreshWordList();
}

//---------------------------------------------------------------------------
void __fastcall TMForm::DeleteMeaning(TObject*Sender)
{ //������� ������� ��������
  if(MeaningId.IsEmpty())
  { Status->Text="�������� �������� ��� ��������!";
    return;
  }
  //������ ������
  Status->Text="������ ��������� �������";
  AnsiString SQL ="UPDATE meaning SET class_id = NULL WHERE class_id = ";
  SQL+=MeaningId;
  sql Q1(SQL);
  SQL ="UPDATE meaning SET class2_id = NULL WHERE class2_id = ";
  SQL+=MeaningId;
  sql Q2(SQL);
  SQL ="UPDATE meaning SET class3_id = NULL WHERE class3_id = ";
  SQL+=MeaningId;
  sql Q3(SQL);
  //������� ��������
  SQL ="DELETE FROM association WHERE object_id = ";
  SQL+=MeaningId;
  sql Q4(SQL);
  //������� ��������
  SQL ="DELETE FROM synonym WHERE object_id = ";
  SQL+=MeaningId;
  sql Q5(SQL);
  //������� ��������
  Status->Text="������� ��������...";
  SQL="DELETE FROM meaning WHERE id = ";
  SQL+=MeaningId;
  sql Q(SQL);

  CurrMeaning="";
  MeaningId="";
  if(RefreshWordList())
    Status->Text="�������� �������.";
}

//---------------------------------------------------------------------------
// ��������� ��� ��������
void __fastcall TMForm::SetMeaningType(TObject*Sender)
{ if(MeaningId.IsEmpty()) return; //�������� ������ ���� �������
  TMenuItem*Menu = (TMenuItem*)Sender;

  AnsiString SQL="UPDATE meaning SET type = '";
  SQL+=Menu->Caption;
  SQL+="' WHERE id = " + MeaningId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  if(RefreshPropertyList())
    Status->Text="Ok, ��� ����������.";
}
//---------------------------------------------------------------------------
// ��������� ����� ��������
void __fastcall TMForm::SetMeaningClass(TObject*Sender)
{ if(MeaningId.IsEmpty()) return; //�������� ������ ���� �������
  //������� ��� ������� ������
  TToolButton*Btn = (TToolButton*)Sender;
  AnsiString ClassName, OldClassName;
  if(Btn->Caption.Pos("...")==0) OldClassName = ClassName = Btn->Caption;
  //������� ��� ������ ������
  Status->Text="������� ����� ����� ������...";
  if(InputStr(ClassName,"������� ��� ������",0) && ClassName!=OldClassName)
  { //������ Id ������
    AnsiString ClsId, SQL;
    if(ClassName!="del")
    { SQL =
        "SELECT id, type FROM meaning "
        "WHERE name = '";
      SQL+=ClassName + "'";
      sql Q(SQL);
      int MaxW=0;
      while(Q.Next()) //���� �� ���������� � ������
      { int W = 10;
        char*CurType = MeaningsTypes[CurrMeaType];
        if(strcmp(Q[1],"class")==0) W=100;
        else if(strcmp(Q[1],"act")==0)
        {
          if(strcmp(CurType,"act")==0) W=110;
          else if(strcmp(CurType,"action")==0) W=110;
          else if(strcmp(CurType,"variable")==0) W=90;
        }
        else if(strcmp(Q[1],"action")==0)
        {
          if(strcmp(CurType,"act_syn")==0) W=110;
        }
        else if(strcmp(Q[1],"attribute")==0)
        {
          if(strcmp(CurType,"attribute")==0) W=110;
        }

        if(MaxW<W)
        { ClsId = Q[0];
          MaxW = W;
        }
      }
      if(ClsId.IsEmpty())
      { Status->Text="����� �� ������!";
        return;
      }
    }
    //��������� �����
    SQL="UPDATE meaning SET ";
    if(Sender==MeaningClassButton)       SQL+="class_id = ";
    else if(Sender==MeaningClass2Button) SQL+="class2_id = ";
    else if(Sender==MeaningClass3Button) SQL+="class3_id = ";
    else if(Sender==MeaningClass4Button) SQL+="part_id = ";
    else
    { Status->Text="��� ����� ������!!!";
      return;
    }
    if(ClassName=="del") SQL+="NULL WHERE id = ";
    else                 SQL+=ClsId + " WHERE id = ";
    SQL+=MeaningId;

    sql Q(SQL);
    if(RefreshCurrMeaning())
      Status->Text="Ok, ����� ����������.";
  }
  else Status->Text="����� �� ������...";
}
//---------------------------------------------------------------------------
/*
void __fastcall TMForm::SetStatusM(TObject*Sender)
{ if(MeaningId.IsEmpty()) return;
  AnsiString Stat;
  if(InputStr( Stat,"������� ������","����������"))
  { AnsiString SQL="UPDATE meaning SET status = ";
    SQL+=Stat + " WHERE id = " + MeaningId;
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    Status->Text="Ok, ������ ����������";
    RefreshCurrMeaning(); //������� ���
  }
}
*/

//---------------------------------------------------------------------------
void __fastcall TMForm::ClassTreeClick(TObject*Sender)
{ //��������� �� ��������� ��������
  if
  (!ClassTree->Selected ||
    ClassTree->Selected->Text.IsEmpty()
  ) return; //��� ��������

  AnsiString ItemName = ClassTree->Selected->Text;
  int p=ItemName.Pos(" (�������)");
  if(p) ItemName=ItemName.Delete(p,10);
  p=ItemName.Pos(" (�����)");
  if(p) ItemName=ItemName.Delete(p,8);
  if
  ( CurrMeaning==ItemName &&
    CurrMeaType==ClassTree->Selected->ImageIndex
  ) return; //��� ���������

  //������ ��� ����� �� ����� ��������
  AnsiString SQL =
    "SELECT word.name FROM word "
    "JOIN meaning ON word.id = word_id "
    "WHERE meaning.name = '";
  SQL+=ItemName + "' AND meaning.type = '";
  SQL+=MeaningsTypes[ClassTree->Selected->ImageIndex];
  SQL+="'";
  sql Q(SQL);
  if(Q.Next())
  { CurrWord = Q[0];
    CurrMeaning = ItemName;
    CurrMeaType = ClassTree->Selected->ImageIndex;
    CurrSynonym = "";
  }
  else
  { Status->Text="�������� �� �������!";
    return;
  }
  //������������� ������ ����
  FindWord->Text = CurrWord;
  RefreshWordList();
  //������� ��������
  RefreshCurrMeaning();
  RefreshCaption();
}
//---------------------------------------------------------------------------
void __fastcall TMForm::SaveMeaningCode(TObject*Sender)
{
  if(MeaningId.IsEmpty())
  { Status->Text="�� ������ ������!";
    return;
  }
  AnsiString SQL;
  AnsiString Constr;
  if(MeaningDscrMemo->Text.IsEmpty()) Constr="NULL";
  else Constr=AnsiString("'") + MeaningDscrMemo->Text + "'";
  AnsiString Code;
  if(MeaningCodeMemo->Text.IsEmpty()) Code="NULL";
  else Code=AnsiString("'") + MeaningCodeMemo->Text + "'";
  AnsiString Tags;
  if(TagsMemo->Text.IsEmpty()) Tags="NULL";
  else Tags=AnsiString("'") + TagsMemo->Text + "'";
  //��������� ��� ��������
  SQL="UPDATE meaning SET description = ";
  SQL+=Constr + ", code = " + Code + ", hashtag = " + Tags;
  SQL+=" WHERE id = " + MeaningId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  Status->Text="Ok, ��� ��������";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::AddSynonym(TObject*Sender)
{ if(MeaningId.IsEmpty())
  { Status->Text="�� ������� ��������!";
    return;
  }
  //������� ��� ��������
  Status->Text="������� ����� ��������...";
  AnsiString SynName;
  if(InputStr(SynName,"������� �������",0))
  { //������ Id �����
    AnsiString WrdId;
    AnsiString SQL = "SELECT id FROM word WHERE name = '";
    SQL+=SynName + "'";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    MYSQL_RES*Result=mysql_store_result(&DB);
    if(!Result)
    { Status->Text=mysql_error(&DB);
      return;
    }
    MYSQL_ROW row;
    if(bool(row=mysql_fetch_row(Result))) WrdId = row[0];
    mysql_free_result(Result);

    if(WrdId.IsEmpty())
    { Status->Text="����� �� �������!";
      return;
    }

    //������� ��������
    SQL="INSERT INTO synonym SET add_name = FALSE, object_id = ";
    SQL+=MeaningId + ", word_id = ";
    SQL+=WrdId;
    if(mysql_query(&DB,SQL.c_str()))
      Status->Text=mysql_error(&DB);
    else
    {
      Status->Text="Ok, ������� ��������";
      RefreshWordList();
    }
  }
  else Status->Text="";
}
//==============================================================================
void __fastcall TMForm::AssocListSelectItem(TObject*,TListItem*Item,bool Selected)
{ //������� ������ � ������ ����������
  CurrProperty = Item->Caption;
  //������� id, ��� � ����������� �� ����� ������� � ��������
  AnsiString SQL =
  "SELECT id, code FROM association "
  "WHERE object_id = ";
  SQL+=MeaningId;
  SQL+=" AND name = '";
  SQL+=CurrProperty;
  SQL+="'";
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_ROW row;
  if(!bool(row=mysql_fetch_row(Result)))
  { Status->Text="�������� �� �������!";
    mysql_free_result(Result);
    return;
  }
  PropertyId = row[0];
  Status->Text=CurrMeaning+"."+CurrProperty+": "+row[1];
  mysql_free_result(Result);
}
//---------------------------------------------------------------------------
void __fastcall TMForm::AssocDblClick(TObject*Sender)
{ //����������� �������� �������
  if(CurrMeaning==""||PropertyId==""||CurrProperty=="") return;
  //������� ���
  AnsiString SQL =
  "SELECT code FROM association "
  "WHERE id = ";
  SQL+=PropertyId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_RES*Result=mysql_store_result(&DB);
  if(!Result)
  { Status->Text=mysql_error(&DB);
    return;
  }
  MYSQL_ROW row;
  if(!bool(row=mysql_fetch_row(Result)))
  { Status->Text="�������� �� �������!";
    mysql_free_result(Result);
    return;
  }
  AnsiString Code=row[0];;
  mysql_free_result(Result);

  if(InputTxt(Code, (CurrMeaning+"."+CurrProperty+":").c_str(),"���������"))
  { //��������� ��� ��������
    if(Code.IsEmpty()) Code="NULL";
    else Code=AnsiString("'") + Code + "'";

    AnsiString SQL="UPDATE association SET code = " +
    Code+=" WHERE id = " + PropertyId;

    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //������� ��������
    if(RefreshPropertyList())
      Status->Text="Ok, �������� ���������.";
  }
}
//---------------------------------------------------------------------------
void __fastcall TMForm::AddAssoc(TObject*Sender)
{ //��������� ��������/�����
  if(MeaningId.IsEmpty()) return;
  AnsiString PropertyName;
  if(InputStr(PropertyName,"���������� ������ ��������","Ok"))
  { //����� � ����
    AnsiString SQL = "INSERT INTO association SET object_id = ";
    SQL+= MeaningId + ", type = 'prp', name = '";
    SQL+=PropertyName + "'";
    if(mysql_query(&DB,SQL.c_str()))
    { Status->Text=mysql_error(&DB);
      return;
    }
    //������� ��������
    if(RefreshPropertyList())
      Status->Text="��������� ��������.";
  }
}
//---------------------------------------------------------------------------
void __fastcall TMForm::DelAssoc(TObject*Sender)
{ //������� ����������
  if(PropertyId.IsEmpty())
  { Status->Text="�������� ��������� ��������!";
    return;
  }
  AnsiString SQL = "DELETE FROM association WHERE id = ";
  SQL+= PropertyId;
  sql Q(SQL);
  if(RefreshPropertyList())
    Status->Text="�������� �������.";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::SetAssocType(TObject*Sender)
{ //������������� ��� ����������
  if(PropertyId.IsEmpty())
  { Status->Text="�������� �� �������!";
    return;
  }
  TMenuItem*Menu = (TMenuItem*)Sender;

  AnsiString SQL = "UPDATE association SET type = '";
  SQL+=Menu->Caption.SubString(1,3);
  SQL+= "' WHERE id = ";
  SQL+= PropertyId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }

  RefreshCurrMeaning();
  Status->Text="Ok. ������� ��� ��������.";

}
//---------------------------------------------------------------------------
void __fastcall TMForm::AddKeyword(TObject*Sender)
{ if(PropertyId=="") return;
  //������� ��� ��������� �����
  Status->Text="������� ���� ��������� �����...";
  AnsiString KeyName;
  if(InputStr(KeyName,"������� �������� �����",0))
  { //������ Id �����
    AnsiString WrdId, SQL = "SELECT id FROM word WHERE name = '";
    SQL+=KeyName + "'";
    sql Q(SQL);
    if(Q.Next()) WrdId = Q[0];
    else
    { Status->Text="����� �� �������!";
      return;
    }
    //������� �������� �����
    SQL = "UPDATE association SET word_id = ";
    SQL+= WrdId + " WHERE id = ";
    SQL+= PropertyId;
    sql Q2(SQL);
    if(RefreshPropertyList())
      Status->Text="��������� �������� ����� ��������.";
  }
  else Status->Text="";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::DeleteKeyword(TObject*Sender)
{ if(PropertyId=="") return;
  AnsiString SQL = "UPDATE association SET word_id = NULL WHERE id = ";
  SQL+= PropertyId;
  sql Q(SQL);
  if(RefreshPropertyList())
    Status->Text="������� �������� ����� ��������.";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::DeleteInWordList(TObject*Sender)
{ //������ ��� ��������
  if(SynonymId.IsEmpty())
  { Status->Text="������� �� ������!";
    return;
  }

  AnsiString SQL =
    "DELETE FROM synonym WHERE id = ";
  SQL+= SynonymId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }
  SynonymId="";
  CurrSynonym="";

  RefreshWordList();
  Status->Text="Ok. ������� �����.";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::SynToName(TObject *Sender)
{ //������ ��� ��������
  if(SynonymId.IsEmpty())
  { Status->Text="������� �� ������!";
    return;
  }

  AnsiString SQL =
    "UPDATE synonym SET add_name = IF(add_name,FALSE,TRUE) WHERE id = ";
  SQL+= SynonymId;
  if(mysql_query(&DB,SQL.c_str()))
  { Status->Text=mysql_error(&DB);
    return;
  }

  RefreshWordList();
  Status->Text="Ok. ������� ��� ��������.";
}
//---------------------------------------------------------------------------
void __fastcall TMForm::M_ChBoxClick(TObject*Sender)
{ //������/��������
  if(MeaningId.IsEmpty())
  { Status->Text="�� ������ ������!";
    return;
  }
  AnsiString SQL;
  if(M_ChBox->Checked)
    SQL = "UPDATE meaning SET status = 1 WHERE id = ";
  else
    SQL = "UPDATE meaning SET status = NULL WHERE id = ";
  SQL+= MeaningId;
  sql Q(SQL);
  Status->Text="Ok, ������ ������.";
}
//---------------------------------------------------------------------------

