//---------------------------------------------------------------------------
#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <ToolWin.hpp>
#include <ValEdit.hpp>
#include <ActnList.hpp>
#include <ActnCtrls.hpp>
#include <ActnMan.hpp>
//---------------------------------------------------------------------------
#include "..\MySQL\mysql.h"
#include <ActnMenus.hpp>
#include <Dialogs.hpp>
#include <Chart.hpp>
#include <DbChart.hpp>
#include <QRTEE.hpp>
#include <QuickRpt.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ImgList.hpp>
extern MYSQL DB;
extern char*MeaningsTypes[];
//--------------------------
extern bool __fastcall InputStr(AnsiString&Str,char*Cap,char*But);
extern bool __fastcall InputTxt(AnsiString&Txt,char*Cap,char*But);
extern bool __fastcall InputForm(AnsiString&Key, AnsiString&Form,char*But);
extern bool __fastcall InputKeyRange(AnsiString&Key1,AnsiString&Key2,char*But);
//extern bool __fastcall InputAssociation(AnsiString&Obj, AnsiString&Prop);
//---------------------------------------------------------------------------
class TMForm : public TForm
{
__published:	// IDE-managed Components
  TProgressBar *ProgressBar;
  TMemo *Status;
  TMainMenu *MainMenu;
  TMenuItem *N1;
  TMenuItem *N2;
  TMenuItem *N3;
  TMenuItem *N4;
  TMenuItem *N7;
  TMenuItem *N9;
  TMenuItem *N10;
  TMenuItem *N11;
  TMenuItem *N15;
  TMenuItem *N6;
  TPopupMenu *PopupMenu1;
  TMenuItem *N16;
  TMenuItem *N17;
  TMenuItem *N18;
  TMenuItem *N12;
  TMenuItem *N13;
  TMenuItem *N5000lemma1;
  TMenuItem *N14;
  TMenuItem *N19;
  TOpenDialog *OpenDialog;
  TSaveDialog *SaveDialog;
  TPopupMenu *PopupMenu2;
  TMenuItem *N20;
  TMenuItem *N21;
  TMenuItem *WordListRefresh;
  TMenuItem *N24;
  TMenuItem *N25;
  TMenuItem *N26;
  TMenuItem *N27;
  TMenuItem *N22;
  TMenuItem *N29;
  TMenuItem *N30;
  TMenuItem *N31;
  TMenuItem *N32;
  TMenuItem *N33;
  TMenuItem *Compile1;
  TMenuItem *N34;
  TMenuItem *N36;
  TMenuItem *ReNumber;
  TMenuItem *Split;
  TMenuItem *N38;
  TMenuItem *N39;
  TMenuItem *N41;
  TMenuItem *N42;
  TMenuItem *N43;
  TMenuItem *N44;
  TMenuItem *N45;
  TPopupMenu *PopupMenu3;
  TMenuItem *class1;
  TMenuItem *attribute1;
  TMenuItem *synonym1;
  TMenuItem *Action1;
  TImageList *ImageList1;
  TMenuItem *actsyn;
  TMenuItem *Verb0Menu;
  TMenuItem *Verb2Menu;
  TMenuItem *fact1;
  TMenuItem *expression1;
  TMenuItem *hagenmorph1;
  TMenuItem *act1;
  TPageControl *Pages;
  TTabSheet *TabSheet4;
  TLabel *Label8;
  TDBChart *DBChart1;
  TBarSeries *Series1;
  TBarSeries *Series2;
  TBarSeries *Series7;
  TBarSeries *Series3;
  TBarSeries *Series4;
  TBarSeries *Series5;
  TBarSeries *Series6;
  TBarSeries *Series8;
  TComboBox *DBList;
  TTabSheet *WordListTSh;
  TPanel *Panel5;
  TTreeView *WordTree;
  TPanel *WordPanel;
  TToolBar *ToolBar1;
  TToolButton *ToolButton2;
  TToolButton *ToolButton3;
  TToolButton *ToolButton4;
  TToolButton *WordTypeButton;
  TToolButton *StatusButton;
  TToolButton *ToolButton6;
  TMemo *WordCode;
  TPanel *MeaningPanel;
  TTreeView *ClassTree;
  TToolBar *ToolBar2;
  TToolButton *ToolButton7;
  TToolButton *MeaningTypeButton;
  TToolButton *MeaningClassButton;
  TToolButton *MeaningClass2Button;
  TToolButton *MeaningClass3Button;
  TToolButton *ToolButton5;
  TPanel *Panel4;
  TLabel *CodeLabel2;
  TLabel *CodeLabel1;
  TMemo *MeaningCodeMemo;
  TMemo *MeaningDscrMemo;
  TTabSheet *TabSheet1;
  TTreeView *TreeView1;
  TTabSheet *SQLTabSh;
  TSplitter *Splitter2;
  TSplitter *Splitter1;
  TPanel *Panel1;
  TLabel *Label4;
  TMemo *SQLResult;
  TPanel *Panel2;
  TPanel *Panel3;
  TLabel *Label6;
  TListBox *TableListBox;
  TSplitter *Splitter3;
  TSplitter *Splitter4;
  TMenuItem *N5;
  TMenuItem *Addvalue1;
  TMenuItem *Addword1;
  TMenuItem *Addsynonym2;
  TSplitter *Splitter6;
  TPopupMenu *PopupMenu4;
  TMenuItem *Add1;
  TMenuItem *Delete1;
  TMenuItem *N8;
  TMenuItem *prp1;
  TMenuItem *N46;
  TMenuItem *N47;
  TMenuItem *N48;
  TMenuItem *atr1;
  TSplitter *Splitter8;
  TPanel *Panel7;
  TLabel *Label5;
  TMemo *ObjCode;
  TLabel *Label7;
  TSplitter *Splitter9;
  TMemo *GlobCode;
  TMenuItem *factsyn;
  TMenuItem *expsyn;
  TPageControl *QueryPgCntr;
  TTabSheet *TabSheet2;
  TMemo *SQLText;
  TTabSheet *TabSheet3;
  TTabSheet *TabSheet5;
  TMemo *SQLText2;
  TMemo *SQLText3;
  TToolButton *WordClassButton;
  TMenuItem *N49;
  TMenuItem *N50;
  TMenuItem *Delete2;
  TMenuItem *SN1;
  TMenuItem *NVerbEnds;
  TMenuItem *NWordEnds;
  TMenuItem *N51;
  TMenuItem *hesh1;
  TMenuItem *N23;
  TMenuItem *N28;
  TListView *AssocList;
  TSplitter *Splitter5;
  TMenuItem *N35;
  TPanel *Panel6;
  TEdit *FindWord;
  TPanel *Panel8;
  TButton *ExeSQLBtn;
  TMenuItem *N37;
  TMenuItem *Addkeyword;
  TMenuItem *Delkeyword;
  TMemo *TagsMemo;
  TLabel *Label1;
  TCheckBox *M_ChBox;
  TTabSheet *TabSheet6;
  TDBChart *DBChart2;
  THorizBarSeries *SeriesClass;
  THorizBarSeries *SeriesAct;
  THorizBarSeries *SeriesAction;
  THorizBarSeries *SeriesActionSyn;
  THorizBarSeries *SeriesAttribute;
  TToolButton *MeaningClass4Button;
  THorizBarSeries *SeriesAct2;
  THorizBarSeries *SeriesAction2;
  THorizBarSeries *SeriesActionSyn2;
  THorizBarSeries *SeriesClass2;
  THorizBarSeries *SeriesAttribute2;
  THorizBarSeries *SeriesFact;
  THorizBarSeries *SeriesFact2;
  THorizBarSeries *SeriesState;
  THorizBarSeries *SeriesState2;
  TMenuItem *location1;
  TMenuItem *tmp1;
  //события формы
  void __fastcall FormDestroy(TObject*Sender);
  void __fastcall FormCloseQuery(TObject*Sender, bool &CanClose);
  void __fastcall SQLTabShEnter(TObject*Sender);
  //действия с базой
  void __fastcall NewDatabase(TObject*Sender);
  void __fastcall SelectDatabase(TObject*Sender);
  void __fastcall RefreshDBChart(TObject*Sender);
  void __fastcall RefreshHashtagChart(TObject*Sender);
  //действия с глобальным списком
  void __fastcall SetWordList(TObject*Sender);
  void __fastcall SetWordListAsList(TObject*Sender);
  void __fastcall SetWordList4Meanings(TObject*Sender);
  void __fastcall SetWordListAsFind(TObject*Sender);
  void __fastcall WordListRefreshClick(TObject *Sender);
  //действия со словом
  void __fastcall NewWord(TObject*Sender);
  void __fastcall EditWord(TObject*Sender);
  void __fastcall EditKey(TObject*Sender);
  void __fastcall EditDefKey(TObject*Sender);
  void __fastcall SetWordClass(TObject*Sender);
  void __fastcall SetStatus(TObject*Sender);
  void __fastcall NewForm(TObject*Sender);
  void __fastcall EditForm(TObject*Sender);
  void __fastcall DeleteWordForm(TObject*Sender);
  void __fastcall SaveObjCode(TObject*Sender);
  void __fastcall TestTranscription(TObject*Sender);
  void __fastcall SetTranscription(TObject*Sender);
  void __fastcall SetTraditional(TObject*Sender);
  void __fastcall SetWithoutEnd(TObject*Sender);
  void __fastcall DeleteInWordList(TObject*Sender);
  void __fastcall ShowSynonym(TObject*Sender);
  //действия напрямую в SQL ----------------------
  void __fastcall ExecuteSQL(TObject*Sender);
  //загрузчики -----------------------------------
  void __fastcall Load5000lemma(TObject*Sender);
  void __fastcall LoadAAZaliznyak(TObject*Sender);
  void __fastcall LoadHagen(TObject*Sender);
  void __fastcall LoadText(TObject*Sender);
  void __fastcall FindWordChange(TObject*Sender);
  //обработчики ----------------------------------
  void __fastcall Clustering(TObject*Sender); //распределение слов по кластерам
  void __fastcall SetAccentuations(TObject*Sender); //анализ ударений
  //анализ файлов
  void __fastcall AnalysisEndsFile(TObject*Sender);
  //подбор кодов
  void __fastcall WordKeyProcessing(TObject*Sender);
  void __fastcall VerbKeyProcessing(TObject*Sender);
  void __fastcall AddVerbKeys(TObject*Sender);
  //сборка словаря -------------------------------
  void __fastcall MakeAll(TObject*Sender);
  void __fastcall MakeEnds(TObject*Sender);
  void __fastcall MakeDic(TObject*Sender);
  void __fastcall MakeHashTab(TObject*Sender);
  void __fastcall MakeEntries(TObject*Sender);
  void __fastcall MakeAssociation(TObject*Sender);
  void __fastcall ReNumberClick(TObject *Sender);
  void __fastcall SplitClick(TObject *Sender);
  //действия со значениями -----------------------
  void __fastcall NewMeaning(TObject*Sender);
  void __fastcall DeleteMeaning(TObject*Sender);
  void __fastcall SetMeaningClass(TObject*Sender);
  void __fastcall SetMeaningType(TObject*Sender);
//void __fastcall SetStatusM(TObject*Sender);
  void __fastcall ClassTreeClick(TObject*Sender);
  void __fastcall SaveMeaningCode(TObject*Sender);
  void __fastcall AddSynonym(TObject*Sender);
  void __fastcall WordTreeClick(TObject *Sender);
  void __fastcall SynToName(TObject *Sender);
  //действия со свойствами ----------------------
  void __fastcall AssocListSelectItem(TObject*Sender,TListItem*Item,bool Selected);
  void __fastcall AssocDblClick(TObject*Sender);
  void __fastcall AddAssoc(TObject*Sender);
  void __fastcall DelAssoc(TObject*Sender);
  void __fastcall SetAssocType(TObject*Sender);
  void __fastcall AddKeyword(TObject*Sender);
  void __fastcall DeleteKeyword(TObject*Sender);
  void __fastcall TagsMemoDblClick(TObject *Sender);
  void __fastcall M_ChBoxClick(TObject *Sender);

private:	// User declarations
  //---------------------------------------------
  //текущая база, слово, значение, синоним, свойство
  AnsiString  CurrDB,
              CurrWord,
              CurrMeaning,
              CurrSynonym,
              CurrProperty;
  int CurrMeaType;
  AnsiString WordId;
  void __fastcall RefreshCaption(void);
  //---------------------------------------------
  AnsiString WordListSQL; //запрос для списка слов
  bool StopFlag; //для остановки длительных процессов
  //отображение информации
  bool __fastcall RefreshBasesList(void);
  bool __fastcall RefreshWordList(void);
  bool __fastcall RefreshCurrWord(void);
  bool __fastcall RefreshCurrMeaning(void);
  bool __fastcall RefreshPropertyList(void);
  bool __fastcall RefreshClassTree(void);
  void __fastcall AddClassItemsToTree(TTreeNode*ClsNode,const AnsiString&ClsId,int Deep);
  //действия со словом
  bool __fastcall WordToFile(const AnsiString&Id,int File);//пишем слово в файл
  //функции для значений
  //загрузки
  char* __fastcall LoadFile(void);
  //обработчики
  void __fastcall SetBegInForm(void);
  void __fastcall SetEndInForm(void);
  void __fastcall FormWithoutEnds(char*FileName);


public:		// User declarations
  __fastcall TMForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMForm *MForm;
//---------------------------------------------------------------------------
#endif
