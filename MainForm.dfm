object MForm: TMForm
  Left = 203
  Top = 105
  Width = 1019
  Height = 639
  Caption = #1057#1083#1086#1074#1072#1088#1085#1072#1103' '#1073#1072#1079#1072' '#1058#1054#1052' '#1085#1072' MySQL '
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poScreenCenter
  ShowHint = True
  WindowState = wsMaximized
  OnCloseQuery = FormCloseQuery
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object ProgressBar: TProgressBar
    Left = 0
    Top = 565
    Width = 1003
    Height = 16
    Align = alBottom
    Min = 0
    Max = 100
    TabOrder = 0
  end
  object Status: TMemo
    Left = 0
    Top = 538
    Width = 1003
    Height = 27
    Align = alBottom
    BorderStyle = bsNone
    Color = cl3DLight
    Lines.Strings = (
      #1057#1090#1088#1086#1082#1072' '#1089#1086#1089#1090#1086#1103#1085#1080#1103)
    ReadOnly = True
    TabOrder = 1
  end
  object Pages: TPageControl
    Left = 0
    Top = 0
    Width = 1003
    Height = 538
    ActivePage = WordListTSh
    Align = alClient
    Style = tsFlatButtons
    TabIndex = 1
    TabOrder = 2
    object TabSheet4: TTabSheet
      Caption = 'Base'
      ImageIndex = 5
      OnShow = RefreshDBChart
      object Label8: TLabel
        Left = 0
        Top = 0
        Width = 995
        Height = 13
        Align = alTop
        Caption = #1057#1087#1080#1089#1086#1082' '#1073#1072#1079':'
      end
      object DBChart1: TDBChart
        Left = 0
        Top = 13
        Width = 995
        Height = 494
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Gradient.EndColor = clGray
        Gradient.StartColor = clSilver
        Title.AdjustFrame = False
        Title.Text.Strings = (
          'TDBChart')
        Title.Visible = False
        LeftAxis.ExactDateTime = False
        LeftAxis.Increment = 1
        LeftAxis.Logarithmic = True
        LeftAxis.MinorTickCount = 0
        LeftAxis.MinorTickLength = 0
        Legend.Alignment = laLeft
        View3DOptions.Elevation = 315
        View3DOptions.Perspective = 48
        View3DOptions.Zoom = 105
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        AutoSize = True
        object Series1: TBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = True
          SeriesColor = clRed
          Title = #1057#1091#1097#1077#1089#1090#1074#1080#1090#1077#1083#1100#1085#1099#1077
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series2: TBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = True
          SeriesColor = clGreen
          Title = #1043#1083#1072#1075#1086#1083#1099
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series7: TBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = True
          SeriesColor = clLime
          Title = #1055#1088#1080#1095#1072#1089#1090#1080#1103
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series3: TBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = True
          SeriesColor = clYellow
          Title = #1055#1088#1080#1083#1072#1075#1072#1090#1077#1083#1100#1085#1099#1077
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series4: TBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = True
          SeriesColor = clBlue
          Title = #1053#1072#1088#1077#1095#1080#1103
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series5: TBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = True
          SeriesColor = clWhite
          Title = #1055#1088#1077#1076#1083#1086#1075#1080
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series6: TBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = True
          SeriesColor = clGray
          Title = #1052#1077#1089#1090#1086#1080#1084#1077#1085#1080#1103
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series8: TBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = True
          SeriesColor = 8421440
          Title = #1087#1088#1086#1095#1077#1077
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object DBList: TComboBox
        Left = 2
        Top = 16
        Width = 151
        Height = 21
        Hint = #1041#1072#1079#1072' '#1076#1072#1085#1085#1099#1093
        ItemHeight = 13
        TabOrder = 1
        OnClick = SelectDatabase
      end
    end
    object WordListTSh: TTabSheet
      Caption = #1057#1083#1086#1074#1072#1088#1100' '#1080' '#1089#1090'.'#1073#1080#1073#1083#1080#1086#1090#1077#1082#1072
      ImageIndex = 1
      object Splitter3: TSplitter
        Left = 749
        Top = 0
        Width = 3
        Height = 507
        Cursor = crHSplit
        Align = alRight
      end
      object Splitter5: TSplitter
        Left = 137
        Top = 0
        Width = 3
        Height = 507
        Cursor = crHSplit
      end
      object Panel5: TPanel
        Left = 0
        Top = 0
        Width = 137
        Height = 507
        Align = alLeft
        TabOrder = 0
        object WordTree: TTreeView
          Left = 1
          Top = 30
          Width = 135
          Height = 476
          Align = alClient
          Images = ImageList1
          Indent = 19
          PopupMenu = PopupMenu2
          TabOrder = 0
          OnClick = WordTreeClick
          Items.Data = {
            020000001D0000000800000008000000FFFFFFFFFFFFFFFF0000000000000000
            04746573741D0000000800000008000000FFFFFFFFFFFFFFFF00000000010000
            0004746573741D0000000000000000000000FFFFFFFFFFFFFFFF000000000100
            000004746573741D0000000900000009000000FFFFFFFFFFFFFFFF0000000000
            0000000474657374}
        end
        object Panel6: TPanel
          Left = 1
          Top = 1
          Width = 135
          Height = 29
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          DesignSize = (
            135
            29)
          object FindWord: TEdit
            Left = 8
            Top = 4
            Width = 121
            Height = 21
            Anchors = [akLeft, akTop, akRight]
            TabOrder = 0
            OnChange = FindWordChange
          end
        end
      end
      object WordPanel: TPanel
        Left = 144
        Top = 0
        Width = 593
        Height = 209
        TabOrder = 1
        object Splitter8: TSplitter
          Left = 250
          Top = 24
          Width = 3
          Height = 184
          Cursor = crHSplit
        end
        object ToolBar1: TToolBar
          Left = 1
          Top = 1
          Width = 591
          Height = 23
          AutoSize = True
          ButtonHeight = 21
          ButtonWidth = 83
          Caption = 'ToolBar1'
          Flat = True
          ShowCaptions = True
          TabOrder = 0
          object ToolButton2: TToolButton
            Left = 0
            Top = 0
            Caption = 'Key...'
            ImageIndex = 1
            OnClick = EditKey
          end
          object ToolButton3: TToolButton
            Left = 83
            Top = 0
            Caption = 'Default key...'
            ImageIndex = 2
            OnClick = EditDefKey
          end
          object ToolButton4: TToolButton
            Left = 166
            Top = 0
            Caption = #1057#1083#1086#1074#1086#1092#1086#1088#1084#1099'...'
            DropdownMenu = PopupMenu1
            ImageIndex = 3
          end
          object WordTypeButton: TToolButton
            Left = 249
            Top = 0
            Caption = #1063#1072#1089#1090#1100' '#1088#1077#1095#1080'?'
            ImageIndex = 5
          end
          object WordClassButton: TToolButton
            Left = 332
            Top = 0
            Caption = #1050#1083#1072#1089#1089'...'
            ImageIndex = 6
            OnClick = SetWordClass
          end
          object StatusButton: TToolButton
            Left = 415
            Top = 0
            Caption = #1057#1090#1072#1090#1091#1089
            ImageIndex = 4
            OnClick = SetStatus
          end
          object ToolButton6: TToolButton
            Left = 498
            Top = 0
            Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1082#1086#1076
            ImageIndex = 5
            OnClick = SaveObjCode
          end
        end
        object WordCode: TMemo
          Left = 1
          Top = 24
          Width = 249
          Height = 184
          Align = alLeft
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = cl3DLight
          Lines.Strings = (
            'WordCode')
          ReadOnly = True
          TabOrder = 1
        end
        object Panel7: TPanel
          Left = 253
          Top = 24
          Width = 339
          Height = 184
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 2
          object Label5: TLabel
            Left = 0
            Top = 0
            Width = 339
            Height = 13
            Align = alTop
            Caption = #1050#1086#1076' '#1082#1086#1085#1089#1090#1088#1091#1082#1090#1086#1088#1072':'
          end
          object Label7: TLabel
            Left = 0
            Top = 148
            Width = 339
            Height = 13
            Align = alTop
            Caption = #1043#1083#1086#1073#1072#1083#1100#1085#1099#1081' '#1082#1086#1076':'
          end
          object Splitter9: TSplitter
            Left = 0
            Top = 145
            Width = 339
            Height = 3
            Cursor = crVSplit
            Align = alTop
          end
          object ObjCode: TMemo
            Left = 0
            Top = 13
            Width = 339
            Height = 132
            Align = alTop
            ScrollBars = ssVertical
            TabOrder = 0
            WordWrap = False
          end
          object GlobCode: TMemo
            Left = 0
            Top = 161
            Width = 339
            Height = 23
            Align = alClient
            ScrollBars = ssVertical
            TabOrder = 1
            WordWrap = False
          end
        end
      end
      object MeaningPanel: TPanel
        Left = 144
        Top = 216
        Width = 593
        Height = 289
        TabOrder = 2
        object Splitter4: TSplitter
          Left = 185
          Top = 24
          Width = 3
          Height = 264
          Cursor = crHSplit
        end
        object ClassTree: TTreeView
          Left = 1
          Top = 24
          Width = 184
          Height = 264
          Align = alLeft
          HideSelection = False
          Images = ImageList1
          Indent = 19
          MultiSelect = True
          SortType = stText
          TabOrder = 0
          OnClick = ClassTreeClick
          Items.Data = {
            01000000220000000000000000000000FFFFFFFFFFFFFFFF0000000001000000
            09D1FAE5E4EEE1EDEEE51E0000000100000001000000FFFFFFFFFFFFFFFF0000
            00000100000005C1F3EBEAE8250000000000000000000000FFFFFFFFFFFFFFFF
            00000000000000000CC1F3EBEAE83AF1ECE0EAEEEC}
        end
        object ToolBar2: TToolBar
          Left = 1
          Top = 1
          Width = 591
          Height = 23
          AutoSize = True
          ButtonHeight = 21
          ButtonWidth = 60
          Caption = 'ToolBar2'
          Flat = True
          ShowCaptions = True
          TabOrder = 1
          object ToolButton7: TToolButton
            Left = 0
            Top = 0
            Caption = #1059#1076#1072#1083#1080#1090#1100
            ImageIndex = 1
            OnClick = DeleteMeaning
          end
          object MeaningTypeButton: TToolButton
            Left = 60
            Top = 0
            Caption = #1058#1080#1087'...'
            DropdownMenu = PopupMenu3
            ImageIndex = 1
          end
          object MeaningClassButton: TToolButton
            Left = 120
            Top = 0
            Hint = #1050#1083#1072#1089#1089' 1'
            Caption = #1050#1083#1072#1089#1089'...'
            ImageIndex = 1
            OnClick = SetMeaningClass
          end
          object MeaningClass2Button: TToolButton
            Left = 180
            Top = 0
            Hint = #1050#1083#1072#1089#1089' 2'
            Caption = #1050#1083#1072#1089#1089' 2...'
            ImageIndex = 1
            OnClick = SetMeaningClass
          end
          object MeaningClass3Button: TToolButton
            Left = 240
            Top = 0
            Hint = #1055#1086#1076#1086#1073#1080#1077
            Caption = #1055#1086#1076#1086#1073#1080#1077'...'
            ImageIndex = 1
            OnClick = SetMeaningClass
          end
          object MeaningClass4Button: TToolButton
            Left = 300
            Top = 0
            Caption = #1063#1072#1089#1090#1100'...'
            ImageIndex = 1
            OnClick = SetMeaningClass
          end
          object ToolButton5: TToolButton
            Left = 360
            Top = 0
            Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
            ImageIndex = 0
            OnClick = SaveMeaningCode
          end
          object M_ChBox: TCheckBox
            Left = 420
            Top = 0
            Width = 81
            Height = 21
            Alignment = taLeftJustify
            Caption = '  '#1043#1086#1090#1086#1074#1086
            TabOrder = 0
            OnClick = M_ChBoxClick
          end
        end
        object Panel4: TPanel
          Left = 188
          Top = 24
          Width = 404
          Height = 264
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 2
          object CodeLabel2: TLabel
            Left = 0
            Top = 172
            Width = 404
            Height = 13
            Align = alTop
            Caption = #1050#1086#1076':'
          end
          object CodeLabel1: TLabel
            Left = 0
            Top = 0
            Width = 404
            Height = 13
            Align = alTop
            Caption = #1054#1087#1080#1089#1072#1085#1080#1077' / '#1040#1088#1075#1091#1084#1077#1085#1090#1099':'
          end
          object Splitter6: TSplitter
            Left = 0
            Top = 169
            Width = 404
            Height = 3
            Cursor = crVSplit
            Align = alTop
          end
          object Label1: TLabel
            Left = 0
            Top = 219
            Width = 404
            Height = 13
            Align = alBottom
            Caption = #1055#1086#1080#1089#1082#1086#1074#1099#1077' '#1090#1101#1075#1080':'
          end
          object MeaningCodeMemo: TMemo
            Left = 0
            Top = 185
            Width = 404
            Height = 34
            Align = alClient
            TabOrder = 0
          end
          object MeaningDscrMemo: TMemo
            Left = 0
            Top = 13
            Width = 404
            Height = 156
            Align = alTop
            TabOrder = 1
          end
          object TagsMemo: TMemo
            Left = 0
            Top = 232
            Width = 404
            Height = 32
            Align = alBottom
            TabOrder = 2
            OnDblClick = TagsMemoDblClick
          end
        end
      end
      object AssocList: TListView
        Left = 752
        Top = 0
        Width = 243
        Height = 507
        Align = alRight
        Columns = <
          item
            Caption = 'C'#1074#1086#1081#1089#1090#1074#1086
            Width = 100
          end
          item
            Caption = #1058#1080#1087
            Width = 35
          end
          item
            Caption = #1047#1085#1072#1095#1077#1085#1080#1077
            Width = 100
          end
          item
            Caption = 'Keyword'
            Width = 100
          end>
        GridLines = True
        Items.Data = {
          3200000001000000FFFFFFFFFFFFFFFFFFFFFFFF020000000000000008D1E2EE
          E9F1F2E2EE037072700474727565FFFFFFFF}
        RowSelect = True
        PopupMenu = PopupMenu4
        ShowWorkAreas = True
        SortType = stText
        TabOrder = 3
        ViewStyle = vsReport
        OnDblClick = AssocDblClick
        OnSelectItem = AssocListSelectItem
      end
    end
    object TabSheet1: TTabSheet
      Caption = #1050#1083#1102#1095#1080
      ImageIndex = 1
      object TreeView1: TTreeView
        Left = 0
        Top = 0
        Width = 313
        Height = 507
        Align = alLeft
        BevelInner = bvNone
        BevelOuter = bvNone
        BevelKind = bkFlat
        BorderStyle = bsNone
        Indent = 19
        ParentColor = True
        ReadOnly = True
        TabOrder = 0
        Items.Data = {
          0E0000001C0000000000000000000000FFFFFFFFFFFFFFFF0000000003000000
          03D0EEE4290000000000000000000000FFFFFFFFFFFFFFFF0000000000000000
          10CCF0202D20ECF3E6F1EAEEE920F0EEE4290000000000000000000000FFFFFF
          FFFFFFFFFF000000000000000010D1F0202D20F1F0E5E4EDE8E920F0EEE42900
          00000000000000000000FFFFFFFFFFFFFFFF000000000000000010C6F0202D20
          E6E5EDF1EAE8E920F0EEE41E0000000000000000000000FFFFFFFFFFFFFFFF00
          0000000200000005D7E8F1EBEE300000000000000000000000FFFFFFFFFFFFFF
          FF000000000000000017C5F7202D20E5E4E8EDF1F2E2E5EDEDEEE520F7E8F1EB
          EE310000000000000000000000FFFFFFFFFFFFFFFF000000000000000018CCF7
          202D20ECEDEEE6E5F1F2E2E5EDEDEEE520F7E8F1EBEE1E000000000000000000
          0000FFFFFFFFFFFFFFFF000000000A00000005CFE0E4E5E63000000000000000
          00000000FFFFFFFFFFFFFFFF000000000000000017C8EF202D20E8ECE5EDE8F2
          E5EBFCEDFBE920EFE0E4E5E62F0000000000000000000000FFFFFFFFFFFFFFFF
          000000000200000016D0EF202D20F0EEE4E8F2E5EBFCEDFBE920EFE0E4E5E632
          0000000000000000000000FFFFFFFFFFFFFFFF000000000000000019CFF3202D
          20EFE0F0F2E0F2E8E22028F3F2EEF7EDE5EDE8E5293700000000000000000000
          00FFFFFFFFFFFFFFFF00000000000000001ED1F3202D20F1F7B8F2EDFBE920EF
          E0E4E5E62028F3F2EEF7EDE5EDE8E5292D0000000000000000000000FFFFFFFF
          FFFFFFFF000000000000000014C4EF202D20E4E0F2E5EBFCEDFBE920EFE0E4E5
          E62F0000000000000000000000FFFFFFFFFFFFFFFF000000000000000016C2EF
          202D20E2E8EDE8F2E5EBFCEDFBE920EFE0E4E5E6300000000000000000000000
          FFFFFFFFFFFFFFFF000000000000000017D2EF202D20F2E2EEF0E8F2E5EBFCED
          FBE920EFE0E4E5E62E0000000000000000000000FFFFFFFFFFFFFFFF00000000
          0100000015CFEF202D20EFF0E5E4EBEEE6EDFBE920EFE0E4E5E62D0000000000
          000000000000FFFFFFFFFFFFFFFF000000000000000014CCF3202D20F3F2EEF7
          EDE5EDE8E520ECE5F1F2E03E0000000000000000000000FFFFFFFFFFFFFFFF00
          0000000000000025C0EF202D20EFE0E4E5E620EAEEEDE5F7EDEEE3EE20EFF3ED
          EAF2E02028C0EBEBE0F2E8E229360000000000000000000000FFFFFFFFFFFFFF
          FF00000000000000001DC1EF202D20E8F1F5EEE4EDFBE920EFE0E4E5E62028C0
          E1EBE0F2E8E229350000000000000000000000FFFFFFFFFFFFFFFF0000000000
          0000001CCBEF202D20ECE5F1F2EDFBE920EFE0E4E5E62028CBEEEAE0F2E8E229
          390000000000000000000000FFFFFFFFFFFFFFFF000000000000000020D1EF20
          2D2022F1EAE2EEE7EDEEE92220EFE0E4E5E62028CFF0EEEBE0F2E8E229270000
          000000000000000000FFFFFFFFFFFFFFFF00000000020000000ECEE4F3F8E5E2
          EBE5EDEDEEF1F2FC2A0000000000000000000000FFFFFFFFFFFFFFFF00000000
          0000000011CEE4202D20EEE4F3F8E5E2EBB8EDEDEEE52C000000000000000000
          0000FFFFFFFFFFFFFFFF000000000000000013CDE4202D20EDE5EEE4F3F8E5E2
          EBB8EDEDEEE5220000000000000000000000FFFFFFFFFFFFFFFF000000000100
          000009CEE1EEE1F9E5EDE8E5270000000000000000000000FFFFFFFFFFFFFFFF
          00000000000000000ECEE1202D20EEE1EEE1F9E5EDE8E51E0000000000000000
          000000FFFFFFFFFFFFFFFF000000000400000005C2F0E5ECFF27000000000000
          0000000000FFFFFFFFFFFFFFFF00000000000000000EC8E2202D20E8EDF4E8ED
          E8F2E8E22D0000000000000000000000FFFFFFFFFFFFFFFF0000000000000000
          14CFE2202D20EFF0EEF8E5E4F8E5E520E2F0E5ECFF2D00000000000000000000
          00FFFFFFFFFFFFFFFF000000000000000014CDE2202D20EDE0F1F2EEFFF9E5E5
          20E2F0E5ECFF2B0000000000000000000000FFFFFFFFFFFFFFFF000000000000
          000012C1E2202D20E1F3E4F3F9E5E520E2F0E5ECFF1D00000000000000000000
          00FFFFFFFFFFFFFFFF000000000300000004CBE8F6EE29000000000000000000
          0000FFFFFFFFFFFFFFFF00000000000000001031EB202D20EFE5F0E2EEE520EB
          E8F6EE290000000000000000000000FFFFFFFFFFFFFFFF000000000000000010
          32EB202D20E2F2EEF0EEE520EBE8F6EE290000000000000000000000FFFFFFFF
          FFFFFFFF00000000000000001033EB202D20F2F0E5F2FCE520EBE8F6EE220000
          000000000000000000FFFFFFFFFFFFFFFF000000000300000009CEE1F0E0F9E5
          EDE8E52D0000000000000000000000FFFFFFFFFFFFFFFF000000000000000014
          D2FB202D20EEE1F0E0F9E5EDE8E520EDE020F2FB2D0000000000000000000000
          FFFFFFFFFFFFFFFF000000000000000014C2FB202D20EEE1F0E0F9E5EDE8E520
          EDE020C2FB3C0000000000000000000000FFFFFFFFFFFFFFFF00000000000000
          0023CCFB202D20EEE1F0E0F9E5EDE8E520EDE020CCFB2028E4EBFF20ECEEEDE0
          F0F5EEE2291E0000000000000000000000FFFFFFFFFFFFFFFF00000000020000
          0005D1F2E8EBFC2F0000000000000000000000FFFFFFFFFFFFFFFF0000000000
          00000016CFF1202D20EFEEFDF2E8F7E5F1EAE8E920F1F2E8EBFC280000000000
          000000000000FFFFFFFFFFFFFFFF00000000000000000FD3F1202D20F3F1F2E0
          F0E5E2F8E5E51E0000000000000000000000FFFFFFFFFFFFFFFF000000000200
          000005D4EEF0ECE02B0000000000000000000000FFFFFFFFFFFFFFFF00000000
          0000000012CAF4202D20EAF0E0F2EAE0FF20F4EEF0ECE0360000000000000000
          000000FFFFFFFFFFFFFFFF00000000000000001DCFF4202D20EFEEE2E5EBE8F2
          E5EBFCEDEEE520EDE0EAEBEEEDE5EDE8E5210000000000000000000000FFFFFF
          FFFFFFFFFF00000000010000000832FF20F4EEF0ECE026000000000000000000
          0000FFFFFFFFFFFFFFFF00000000000000000DD432202D2032FF20F4EEF0ECE0
          1E0000000000000000000000FFFFFFFFFFFFFFFF000000000200000005C0EAF2
          B8F0270000000000000000000000FFFFFFFFFFFFFFFF00000000000000000EDD
          E0202D20FDF2EE20E0EAF2E5F0260000000000000000000000FFFFFFFFFFFFFF
          FF00000000000000000DCDE0202D20EDE520E0EAF2E5F0240000000000000000
          000000FFFFFFFFFFFFFFFF00000000020000000BCDE0EFF0E0E2EBE5EDE8E533
          0000000000000000000000FFFFFFFFFFFFFFFF00000000000000001AC2ED202D
          20EDE0EFF0E0E2EBE5EDE8E520E2EE20E2EDF3F2F0FC30000000000000000000
          0000FFFFFFFFFFFFFFFF000000000000000017CDED202D20EDE0EFF0E0E2EBE5
          EDE8E520EDE0F0F3E6F32A0000000000000000000000FFFFFFFFFFFFFFFF0000
          00000300000011CCE5F1F2EEF0E0F1EFEEEBEEE6E5EDE8E53400000000000000
          00000000FFFFFFFFFFFFFFFF00000000000000001BC2EC202D20ECE5F1F2EE20
          E220E7EEEDE520E2E8E4E8ECEEF1F2E82D0000000000000000000000FFFFFFFF
          FFFFFFFF000000000000000014C4EC202D20E4EEF1F2F3EFEDEEE520ECE5F1F2
          EE3B0000000000000000000000FFFFFFFFFFFFFFFF00000000000000002221EC
          202D20EEF2F1F3F2F1F2E2F3E5F220E220F2E5EAF3F9E5E920EBEEEAE0F6E8E8}
      end
    end
    object SQLTabSh: TTabSheet
      Hint = #1055#1088#1086#1080#1079#1074#1086#1083#1100#1085#1099#1081' SQL-'#1079#1072#1087#1088#1086#1089
      Caption = 'SQL'
      ImageIndex = 2
      OnEnter = SQLTabShEnter
      object Splitter2: TSplitter
        Left = 145
        Top = 0
        Width = 3
        Height = 507
        Cursor = crHSplit
        AutoSnap = False
        MinSize = 50
      end
      object Splitter1: TSplitter
        Left = 413
        Top = 0
        Width = 3
        Height = 507
        Cursor = crHSplit
        AutoSnap = False
        MinSize = 120
      end
      object Panel1: TPanel
        Left = 416
        Top = 0
        Width = 579
        Height = 507
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        object Label4: TLabel
          Left = 0
          Top = 0
          Width = 55
          Height = 13
          Align = alTop
          Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090':'
        end
        object SQLResult: TMemo
          Left = 0
          Top = 13
          Width = 579
          Height = 494
          Align = alClient
          ScrollBars = ssBoth
          TabOrder = 0
          WordWrap = False
        end
      end
      object Panel2: TPanel
        Left = 148
        Top = 0
        Width = 265
        Height = 507
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 1
        object QueryPgCntr: TPageControl
          Left = 0
          Top = 33
          Width = 265
          Height = 474
          ActivePage = TabSheet2
          Align = alClient
          TabIndex = 0
          TabOrder = 0
          object TabSheet2: TTabSheet
            Caption = 'Q1'
            object SQLText: TMemo
              Left = 0
              Top = 0
              Width = 257
              Height = 446
              Align = alClient
              TabOrder = 0
            end
          end
          object TabSheet3: TTabSheet
            Caption = 'Q2'
            ImageIndex = 1
            object SQLText2: TMemo
              Left = 0
              Top = 0
              Width = 257
              Height = 428
              Align = alClient
              TabOrder = 0
            end
          end
          object TabSheet5: TTabSheet
            Caption = 'Q3'
            ImageIndex = 2
            object SQLText3: TMemo
              Left = 0
              Top = 0
              Width = 257
              Height = 428
              Align = alClient
              TabOrder = 0
            end
          end
        end
        object Panel8: TPanel
          Left = 0
          Top = 0
          Width = 265
          Height = 33
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          DesignSize = (
            265
            33)
          object ExeSQLBtn: TButton
            Left = 182
            Top = 8
            Width = 75
            Height = 25
            Anchors = [akTop, akRight]
            Caption = #1042#1099#1087#1086#1083#1085#1080#1090#1100
            TabOrder = 0
            OnClick = ExecuteSQL
          end
        end
      end
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 145
        Height = 507
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 2
        object Label6: TLabel
          Left = 0
          Top = 0
          Width = 145
          Height = 13
          Align = alTop
          Caption = #1058#1072#1073#1083#1080#1094#1099':'
        end
        object TableListBox: TListBox
          Left = 0
          Top = 13
          Width = 145
          Height = 494
          Align = alClient
          ItemHeight = 13
          TabOrder = 0
          OnClick = ExecuteSQL
        end
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'Hashtags'
      ImageIndex = 4
      OnShow = RefreshHashtagChart
      object DBChart2: TDBChart
        Left = 0
        Top = 0
        Width = 1003
        Height = 519
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Gradient.EndColor = clGray
        Gradient.StartColor = clSilver
        Title.AdjustFrame = False
        Title.Text.Strings = (
          'TDBChart')
        Title.Visible = False
        LeftAxis.ExactDateTime = False
        LeftAxis.Increment = 1
        LeftAxis.Inverted = True
        LeftAxis.MinorTickCount = 0
        LeftAxis.MinorTickLength = 0
        Legend.Alignment = laLeft
        View3D = False
        View3DOptions.Elevation = 315
        View3DOptions.Perspective = 48
        View3DOptions.Zoom = 105
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        AutoSize = True
        object SeriesAct: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = False
          SeriesColor = 16711808
          Title = #1044#1077#1081#1089#1090#1074#1080#1103
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loAscending
        end
        object SeriesAct2: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = 16711808
          Title = #1044#1077#1081#1089#1090#1074#1080#1103' ('#1085#1077#1075#1086#1090#1086#1074#1086')'
          BarBrush.Color = 16711808
          BarBrush.Style = bsFDiagonal
          BarStyle = bsRectGradient
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesAction: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = False
          SeriesColor = clLime
          Title = #1060#1085'. '#1076#1077#1081#1089#1090#1074#1080#1081
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loAscending
        end
        object SeriesAction2: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = clLime
          Title = #1060#1085'. '#1076#1077#1081#1089#1090#1074#1080#1081' ('#1085#1077#1075#1086#1090#1086#1074#1086')'
          BarBrush.Color = clLime
          BarBrush.Style = bsFDiagonal
          BarStyle = bsRectGradient
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesActionSyn: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = False
          SeriesColor = clYellow
          Title = #1057#1080#1085#1086#1085#1080#1084#1099' '#1092#1091#1085#1082#1094#1080#1081
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loAscending
        end
        object SeriesActionSyn2: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = clYellow
          Title = #1057#1080#1085#1086#1085#1080#1084#1099' '#1092#1085'. ('#1085#1077#1075#1086#1090#1086#1074#1086')'
          BarBrush.Color = clYellow
          BarBrush.Style = bsFDiagonal
          BarStyle = bsRectGradient
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesState: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = clAqua
          Title = #1060#1085'. '#1074#1099#1088#1072#1078#1077#1085#1080#1081
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesState2: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = clAqua
          Title = #1060#1085'. '#1074#1099#1088#1072#1078#1077#1085#1080#1081' ('#1085#1077' '#1075#1086#1090#1086#1074#1086')'
          BarBrush.Color = clAqua
          BarBrush.Style = bsFDiagonal
          BarStyle = bsRectGradient
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFact: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = 16744448
          Title = #1060#1085'. '#1092#1072#1082#1090#1086#1074
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFact2: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = 16744448
          Title = #1060#1085'. '#1092#1072#1082#1090#1086#1074' ('#1085#1077' '#1075#1086#1090#1086#1074#1086')'
          BarBrush.Color = 16744448
          BarBrush.Style = bsFDiagonal
          BarStyle = bsRectGradient
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesClass: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Style = smsValue
          Marks.Visible = False
          SeriesColor = clNavy
          Title = #1050#1083#1072#1089#1089#1099
          BarBrush.Color = clWhite
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loAscending
        end
        object SeriesClass2: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = clNavy
          Title = #1050#1083#1072#1089#1089#1099' ('#1085#1077#1075#1086#1090#1086#1074#1086')'
          BarBrush.Color = clNavy
          BarBrush.Style = bsFDiagonal
          BarStyle = bsRectGradient
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesAttribute: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = clFuchsia
          Title = #1055#1088#1080#1079#1085#1072#1082#1080
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesAttribute2: THorizBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = clFuchsia
          Title = #1055#1088#1080#1079#1085#1072#1082#1080' ('#1085#1077#1075#1086#1090#1086#1074#1086')'
          BarBrush.Color = clFuchsia
          BarBrush.Style = bsFDiagonal
          BarStyle = bsRectGradient
          MultiBar = mbStacked
          XValues.DateTime = False
          XValues.Name = 'Bar'
          XValues.Multiplier = 1
          XValues.Order = loNone
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
    end
  end
  object MainMenu: TMainMenu
    Left = 392
    object N1: TMenuItem
      Caption = 'File'
      object N2: TMenuItem
        Caption = #1057#1086#1079#1076#1072#1090#1100' '#1073#1072#1079#1091
        OnClick = NewDatabase
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object N26: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1084#1072#1090#1088#1080#1094#1091' '#1074' '#1092#1072#1081#1083
      end
    end
    object N9: TMenuItem
      Caption = 'Edit'
      object N11: TMenuItem
        Caption = #1048#1079#1084#1077#1085#1080#1090#1100' '#1080#1076#1077#1085#1090#1080#1092#1080#1082#1072#1090#1086#1088' '#1089#1083#1086#1074#1072
        OnClick = EditWord
      end
      object N10: TMenuItem
        Caption = #1047#1072#1087#1086#1083#1085#1080#1090#1100' '#1087#1086' '#1076#1088#1091#1075#1086#1084#1091' '#1089#1083#1086#1074#1091
      end
      object N15: TMenuItem
        Caption = '-'
      end
      object N4: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074' '#1092#1072#1081#1083
      end
    end
    object N6: TMenuItem
      Caption = 'View'
      object N7: TMenuItem
        Caption = #1058#1077#1082#1091#1097#1080#1081' '#1089#1087#1080#1089#1086#1082' '#1089#1083#1086#1074
        OnClick = SetWordList
      end
      object NVerbEnds: TMenuItem
        Caption = #1060#1072#1081#1083' '#1086#1082#1086#1085#1095#1072#1085#1080#1081' '#1075#1083#1072#1075#1086#1083#1086#1074
        OnClick = AnalysisEndsFile
      end
      object NWordEnds: TMenuItem
        Caption = #1060#1072#1081#1083' '#1086#1082#1086#1085#1095#1072#1085#1080#1081
        OnClick = AnalysisEndsFile
      end
    end
    object N13: TMenuItem
      Caption = 'Load'
      object N5000lemma1: TMenuItem
        Caption = '5000 lemma'
        OnClick = Load5000lemma
      end
      object N14: TMenuItem
        Caption = #1057#1083#1086#1074#1072#1088#1100' '#1040'.'#1040'.'#1047#1072#1083#1080#1079#1085#1103#1082#1072
        OnClick = LoadAAZaliznyak
      end
      object hagenmorph1: TMenuItem
        Caption = #1057#1083#1086#1074#1072#1088#1100' hagen-morph'
        OnClick = LoadHagen
      end
    end
    object N12: TMenuItem
      Caption = 'Tools'
      object N21: TMenuItem
        Caption = #1055#1086#1076#1086#1073#1088#1072#1090#1100' '#1082#1083#1102#1095#1080' '#1082' '#1089#1083#1086#1074#1072#1084
        OnClick = WordKeyProcessing
      end
      object N22: TMenuItem
        Caption = #1055#1086#1076#1086#1073#1088#1072#1090#1100' '#1082#1083#1102#1095#1080' '#1082' '#1075#1083#1072#1075#1086#1083#1072#1084
        OnClick = VerbKeyProcessing
      end
      object Verb0Menu: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1092#1086#1088#1084#1099' '#1085#1077#1089#1086#1074#1077#1088#1096#1105#1085#1085#1086#1075#1086' '#1075#1083#1072#1075#1086#1083#1072
        OnClick = AddVerbKeys
      end
      object Verb2Menu: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1092#1086#1088#1084#1099' '#1089#1086#1074#1077#1088#1096#1105#1085#1085#1086#1075#1086' '#1075#1083#1072#1075#1086#1083#1072
        OnClick = AddVerbKeys
      end
      object N29: TMenuItem
        Caption = '-'
      end
      object N32: TMenuItem
        Caption = #1058#1077#1089#1090' '#1090#1088#1072#1085#1089#1082#1088#1080#1087#1094#1080#1080
        OnClick = TestTranscription
      end
      object N30: TMenuItem
        Caption = #1058#1088#1072#1085#1089#1082#1088#1080#1087#1094#1080#1103' '#1087#1086#1080#1089#1082#1086#1074#1099#1093' '#1092#1086#1088#1084
        OnClick = SetTranscription
      end
      object N33: TMenuItem
        Caption = #1058#1088#1072#1076#1080#1094#1080#1086#1085#1085#1072#1103' '#1079#1072#1087#1080#1089#1100' '#1087#1086#1080#1089#1082#1086#1074#1099#1093' '#1092#1086#1088#1084
        OnClick = SetTraditional
      end
      object N31: TMenuItem
        Caption = '-'
      end
      object N24: TMenuItem
        Caption = #1057#1086#1073#1088#1072#1090#1100' '#1084#1072#1090#1088#1080#1094#1091
      end
      object N27: TMenuItem
        Caption = #1054#1087#1090#1080#1084#1080#1079#1080#1088#1086#1074#1072#1090#1100' '#1084#1072#1090#1088#1080#1094#1091
      end
      object N38: TMenuItem
        Caption = #1040#1083#1100#1090#1077#1088#1085#1072#1090#1080#1074#1085#1072#1103' '#1086#1087#1090#1080#1084#1080#1079#1072#1094#1080#1103
      end
      object N25: TMenuItem
        Caption = #1055#1086#1082#1072#1079#1072#1090#1100' '#1089#1090#1072#1090#1080#1089#1090#1080#1082#1091' '#1087#1086' '#1084#1072#1090#1088#1080#1094#1077
      end
    end
    object Compile1: TMenuItem
      Caption = 'Compile'
      object N19: TMenuItem
        Caption = #1054#1073#1085#1086#1074#1080#1090#1100' '#1089#1083#1086#1074#1072#1088#1085#1099#1077' '#1082#1083#1072#1089#1090#1077#1088#1099
        OnClick = Clustering
      end
      object N35: TMenuItem
        Caption = #1056#1072#1089#1089#1090#1072#1074#1080#1090#1100' '#1091#1076#1072#1088#1077#1085#1080#1103
        OnClick = SetAccentuations
      end
      object N51: TMenuItem
        Caption = #1059#1087#1072#1082#1086#1074#1072#1090#1100' '#1092#1072#1081#1083#1099' '#1086#1082#1086#1085#1095#1072#1085#1080#1081
        OnClick = MakeEnds
      end
      object N41: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' hash-'#1082#1086#1076#1099
        OnClick = SetWithoutEnd
      end
      object N39: TMenuItem
        Caption = '-'
      end
      object N49: TMenuItem
        Caption = #1057#1086#1073#1088#1072#1090#1100' '#1072#1089#1089#1086#1094#1080#1072#1094#1080#1080
        OnClick = MakeAssociation
      end
      object N36: TMenuItem
        Caption = #1057#1086#1073#1088#1072#1090#1100' '#1089#1090#1072#1090#1100#1080
        OnClick = MakeEntries
      end
      object hesh1: TMenuItem
        Caption = #1057#1086#1073#1088#1072#1090#1100' hash-'#1090#1072#1073#1083#1080#1094#1091
        OnClick = MakeHashTab
      end
      object N34: TMenuItem
        Caption = #1057#1086#1073#1088#1072#1090#1100' '#1089#1083#1086#1074#1072#1088#1100
        OnClick = MakeDic
      end
      object N42: TMenuItem
        Caption = '-'
      end
      object N43: TMenuItem
        Caption = #1055#1086#1083#1085#1072#1103' '#1089#1073#1086#1088#1082#1072
        OnClick = MakeAll
      end
    end
  end
  object PopupMenu1: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    Left = 512
    object N16: TMenuItem
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100
      OnClick = NewForm
    end
    object N17: TMenuItem
      Caption = #1048#1079#1084#1077#1085#1080#1090#1100
      OnClick = EditForm
    end
    object N18: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100
      OnClick = DeleteWordForm
    end
    object ReNumber: TMenuItem
      Caption = #1055#1077#1088#1077#1085#1091#1084#1077#1088#1086#1074#1072#1090#1100
      OnClick = ReNumberClick
    end
    object Split: TMenuItem
      Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074' '#1086#1090#1076#1077#1083#1100#1085#1086#1077' '#1089#1083#1086#1074#1086
      OnClick = SplitClick
    end
  end
  object OpenDialog: TOpenDialog
    Left = 424
  end
  object SaveDialog: TSaveDialog
    Left = 456
  end
  object PopupMenu2: TPopupMenu
    Left = 16
    Top = 192
    object N44: TMenuItem
      Caption = #1057#1087#1080#1089#1086#1082' '#1089#1083#1086#1074
      OnClick = SetWordListAsList
    end
    object N28: TMenuItem
      Caption = #1057#1087#1080#1089#1086#1082' '#1079#1085#1072#1095#1077#1085#1080#1081
      OnClick = SetWordList4Meanings
    end
    object N45: TMenuItem
      Caption = #1055#1086#1080#1089#1082' '#1087#1086' '#1082#1086#1076#1091
      OnClick = SetWordListAsFind
    end
    object N20: TMenuItem
      Caption = #1048#1079#1084#1077#1085#1080#1090#1100' '#1079#1072#1087#1088#1086#1089
      OnClick = SetWordList
    end
    object WordListRefresh: TMenuItem
      Caption = #1054#1073#1085#1086#1074#1080#1090#1100
      OnClick = WordListRefreshClick
    end
    object N5: TMenuItem
      Caption = '-'
    end
    object Addword1: TMenuItem
      Caption = 'Add word'
      OnClick = NewWord
    end
    object Addvalue1: TMenuItem
      Caption = 'Add value'
      OnClick = NewMeaning
    end
    object Addsynonym2: TMenuItem
      Caption = 'Add synonym'
      OnClick = AddSynonym
    end
    object N50: TMenuItem
      Caption = '-'
    end
    object N23: TMenuItem
      Caption = #1053#1072#1081#1090#1080' '#1089#1080#1085#1086#1085#1080#1084
      OnClick = ShowSynonym
    end
    object SN1: TMenuItem
      Caption = 'S <=> N'
      OnClick = SynToName
    end
    object Delete2: TMenuItem
      Caption = 'Delete'
      OnClick = DeleteInWordList
    end
  end
  object PopupMenu3: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    Left = 224
    Top = 216
    object class1: TMenuItem
      Caption = 'class'
      OnClick = SetMeaningType
    end
    object attribute1: TMenuItem
      Caption = 'attribute'
      OnClick = SetMeaningType
    end
    object synonym1: TMenuItem
      Caption = 'variable'
      OnClick = SetMeaningType
    end
    object act1: TMenuItem
      Caption = 'act'
      OnClick = SetMeaningType
    end
    object Action1: TMenuItem
      Caption = 'action'
      OnClick = SetMeaningType
    end
    object actsyn: TMenuItem
      Caption = 'act_syn'
      OnClick = SetMeaningType
    end
    object fact1: TMenuItem
      Caption = 'fact'
      OnClick = SetMeaningType
    end
    object factsyn: TMenuItem
      Caption = 'fact_syn'
      OnClick = SetMeaningType
    end
    object expression1: TMenuItem
      Caption = 'expression'
      OnClick = SetMeaningType
    end
    object expsyn: TMenuItem
      Caption = 'exp_syn'
      OnClick = SetMeaningType
    end
    object location1: TMenuItem
      Caption = 'location'
      OnClick = SetMeaningType
    end
  end
  object ImageList1: TImageList
    Left = 576
    Bitmap = {
      494C01010E001300040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000005000000001002000000000000050
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FF800000FF800000FF800000FF800000FF800000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000000000000000000000000000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000FF800000FF800000FF800000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000000000000000000000000000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000000000000000000000000000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000000000000000000000000000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF8000000000000000000000FF800000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FF800000FF800000FF800000FF800000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF80000000000000FF800000FF800000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000FF800000FF800000FF800000FF800000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000FF800000FF800000FF800000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF80000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000FF800000FF80000000000000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000000000000000000000000000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000FF8000000000000000000000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000FF800000FF800000FF800000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000000000000000000000000000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FF800000FF800000FF800000FF800000FF800000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF800000FF800000000000000000000000000000FF800000FF8000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000004040
      0000404000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FF00000000000000FF00000000
      000000FF00000000000000FF000000FF000000FF000000FF0000404000004040
      0000404000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FF0000000000000000
      000000FF000000FF000000FF000000FF000000FF000000FF0000404000004040
      0000404000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FF000000FF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000080FFFF0080FFFF0000000000000000000000
      00000000000000000000000000000000000000FF00000000000000FF000000FF
      000000FF0000000000000000000000FF00004040000040400000404000004040
      000040400000000000000000000000000000000000000000000000000000FF80
      0000FF800000FF800000000000000000000000000000FF800000FF800000FF80
      00000000000000000000000000000000000000000000000000000000000000FF
      000000000000000000000000000000FF000000FF000000000000000000000000
      000000FF000000000000000000000000000000000000000000000000000080FF
      FF0000000000000000000000000080FFFF0080FFFF0000000000000000000000
      000080FFFF000000000000000000000000000000000000FF0000000000000000
      0000000000000000000000000000000000004040000040400000404000004040
      0000404000004040000000FF000000000000000000000000000000000000FF80
      0000FF800000FF800000000000000000000000000000FF800000FF800000FF80
      000000000000000000000000000000000000000000000000000000FF000000FF
      000000FF0000000000000000000000FF000000FF0000000000000000000000FF
      000000FF000000FF00000000000000000000000000000000000080FFFF0080FF
      FF0080FFFF00000000000000000080FFFF0080FFFF00000000000000000080FF
      FF0080FFFF0080FFFF00000000000000000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF00004040000040400000404000004040
      00004040000000FF000000FF000040400000000000000000000000000000FF80
      0000FF800000FF800000000000000000000000000000FF800000FF800000FF80
      00000000000000000000000000000000000000000000000000000000000000FF
      000000FF000000FF00000000000000FF000000FF00000000000000FF000000FF
      000000FF000000000000000000000000000000000000000000000000000080FF
      FF0080FFFF0080FFFF000000000080FFFF0080FFFF000000000080FFFF0080FF
      FF0080FFFF00000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000040400000404000004040
      0000404000004040000040400000FFFF00000000000000000000FF800000FF80
      000000000000FF800000FF80000000000000FF800000FF80000000000000FF80
      0000FF8000000000000000000000000000000000000000000000000000000000
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FF
      FF0000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000404000004040000040400000FFFF00004040
      0000404000004040000040400000FFFF00000000000000000000FF800000FF80
      000000000000FF800000FF80000000000000FF800000FF80000000000000FF80
      0000FF8000000000000000000000000000000000000000000000000000000000
      00000000000000FF000000FF000000FF000000FF000000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF000000
      000000000000000000000000000000000000FFFF0000FFFF000000FFFF00FFFF
      0000FFFF0000FFFF0000FFFF0000404000004040000040400000404000004040
      00004040000040400000FFFF0000FFFF00000000000000000000FF800000FF80
      000000000000FF800000FF80000000000000FF800000FF80000000000000FF80
      0000FF8000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FF000000FF000000FF000000FF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000080FFFF0080FFFF0080FFFF0080FFFF00000000000000
      000000000000000000000000000000000000FFFF0000FFFF000000FFFF00FFFF
      0000FFFF0000FFFF0000FFFF000000FFFF00FFFF000040400000404000004040
      00004040000040400000FFFF0000FFFF00000000000000000000FF800000FF80
      000000000000FF800000FF80000000000000FF800000FF80000000000000FF80
      0000FF8000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FF000000FF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000080FFFF0080FFFF0000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF000000FFFF00FFFF
      0000FFFF0000FFFF000000FFFF0000FFFF00FFFF0000FFFF0000404000004040
      0000404000004040000040400000404000000000000000000000FF800000FF80
      000000000000FF800000FF80000000000000FF800000FF80000000000000FF80
      0000FF8000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FF000000FF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000080FFFF0080FFFF0000000000000000000000
      000000000000000000000000000000000000FFFF000000FFFF00FFFF0000FFFF
      0000FFFF000000FFFF0000FFFF00FFFF0000FFFF0000FFFF0000404000004040
      0000FFFF000040400000FFFF00004040000000000000FF800000FF8000000000
      00000000000000000000FF800000FF800000FF80000000000000000000000000
      0000FF800000FF80000000000000000000000000000000000000000000000000
      000000000000000000000000000000FF000000FF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000080FFFF0080FFFF0000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF000000FF
      FF0000FFFF0000FFFF00FFFF0000FFFF0000FFFF000040400000404000004040
      00004040000040400000FFFF0000FFFF000000000000FF800000FF8000000000
      00000000000000000000FF800000FF800000FF80000000000000000000000000
      0000FF800000FF80000000000000000000000000000000000000000000000000
      000000000000000000000000000000FF000000FF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000080FFFF0080FFFF0000000000000000000000
      00000000000000000000000000000000000000FFFF0000FFFF00FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00004040
      000040400000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0000FFFF0000FFFF00FFFF
      0000FFFF000000FFFF0000FFFF0000FFFF00FFFF0000FFFF0000FFFF00004040
      000040400000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF0000FFFF0000FFFF00FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00004040
      0000FFFF0000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FF000000FF000000FF000000FF000000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FF
      FF00000000000000000000000000000000000000000000000000000000000000
      000000FF000000FF000000FF0000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF0080FFFF0080FFFF00000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      000000FF0000000000000000000000FF000000FF0000000000000000000000FF
      000000FF000000000000000000000000000000000000000000000000000080FF
      FF0080FFFF00000000000000000080FFFF0080FFFF00000000000000000080FF
      FF0080FFFF000000000000000000000000000000000000000000000000000000
      000000FF000000FF000000FF000000FF000000FF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000FF000000FF
      000000000000000000000000000000FF000000FF000000000000000000000000
      000000FF000000FF00000000000000000000000000000000000080FFFF0080FF
      FF0000000000000000000000000080FFFF0080FFFF0000000000000000000000
      000080FFFF0080FFFF0000000000000000000000000000000000000000000000
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF000000
      000000000000000000000000000000000000000000000000000000FF000000FF
      000000000000000000000000000000FF000000FF000000000000000000000000
      000000FF000000FF00000000000000000000000000000000000080FFFF0080FF
      FF0000000000000000000000000080FFFF0080FFFF0000000000000000000000
      000080FFFF0080FFFF0000000000000000000000000000000000000000000000
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000000000000000000000000000000000000000000000000000000
      000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FF
      FF0080FFFF00000000000000000000000000000000000000000000FF000000FF
      000000000000000000000000000000FF000000FF000000000000000000000000
      000000FF000000FF00000000000000000000000000000000000080FFFF0080FF
      FF0000000000000000000000000080FFFF0080FFFF0000000000000000000000
      000080FFFF0080FFFF0000000000000000000000000000000000000000000000
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF000000
      000000000000000000000000000000000000000000000000000000FF000000FF
      000000000000000000000000000000FF000000FF000000000000000000000000
      000000FF000000FF00000000000000000000000000000000000080FFFF0080FF
      FF0000000000000000000000000080FFFF0080FFFF0000000000000000000000
      000080FFFF0080FFFF0000000000000000000000000000000000000000000000
      000000FF000000FF000000FF000000FF000000FF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000FF000000FF
      000000000000000000000000000000FF000000FF000000000000000000000000
      000000FF000000FF00000000000000000000000000000000000080FFFF0080FF
      FF0000000000000000000000000080FFFF0080FFFF0000000000000000000000
      000080FFFF0080FFFF0000000000000000000000000000000000000000000000
      000000FF000000FF000000FF0000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF0080FFFF0080FFFF00000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      000000FF0000000000000000000000FF000000FF0000000000000000000000FF
      000000FF000000000000000000000000000000000000000000000000000080FF
      FF0080FFFF00000000000000000080FFFF0080FFFF00000000000000000080FF
      FF0080FFFF000000000000000000000000000000000000000000000000000000
      000000FF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      0000000000000000000000000000000000000000000000000000000000000000
      000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FF000000FF000000FF000000FF000000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000080FFFF0080FFFF0080FFFF0080FFFF0080FFFF0080FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF00FF00FF00
      FF00000000000000000000000000000000000000000000000000000000000000
      0000FF00FF00FF00FF000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF00FF00FF00
      FF00000000000000000000000000000000000000000000000000000000000000
      0000FF00FF00FF00FF000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000FF00FF00000000000000000000000000000000000000000000000000FF00
      FF0000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      000000000000FF00FF0000000000FF00FF00FF00FF0000000000FF00FF000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF00FF00FF00FF00FF00FF00FF00FF00000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000FF00FF00FF00
      FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
      FF00FF00FF00FF00FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000FF00FF000000000000000000FF00FF00FF00FF0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF00FF00FF00
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF00FF00FF00
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000500000000100010000000000800200000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FFFFFFFF00000000FFFFFFFF00000000
      FFFFFFFF00000000F83FF39F00000000F01FF39F00000000F39FF39F00000000
      FF9FF31F00000000F81FF21F00000000F03FF01F00000000F3FFF09F00000000
      F39FF19F00000000F01FF39F00000000F83FF39F00000000FFFFFFFF00000000
      FFFFFFFF00000000FFFFFFFF00000000FFFFFFFFFFE7FFFFFFFFFFFF5407FFFF
      FC3FFC3FB007FFFFEC37EC374607E38FC423C423BF01E38F800180010000E38F
      C003C0030000C927E007E0070000C927F00FF00F0000C927F81FF81F0000C927
      FC3FFC3F0000C927FC3FFC3F00009C73FC3FFC3F00009C73FC3FFC3F0000FFFF
      FC3FFC3F0000FFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFF81FF81FE7FFE7FFF00FF00FE1FFE1FFE007E007E07FE07FC003C003
      E01FE01F84218421E007E00784218421E001E00184218421E007E00784218421
      E01FE01F84218421E07FE07FC003C003E1FFE1FFE007E007E7FFE7FFF00FF00F
      FFFFFFFFF81FF81FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      C003FFFFFFFFCFF3C003FFFFFFFF87E1C003FFFFC00387E1C0038001C003C243
      C0038001C003F00FC0038001C003F81FC0038001FFFFF00FC0038001C003F00F
      C0038001C003F01FC003FFFFC003C01FC003FFFFC003867FC003FFFFFFFF87FF
      FFFFFFFFFFFFCFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000
      000000000000}
  end
  object PopupMenu4: TPopupMenu
    AutoHotkeys = maManual
    Left = 776
    Top = 72
    object Add1: TMenuItem
      Caption = 'Add'
      OnClick = AddAssoc
    end
    object Delete1: TMenuItem
      Caption = 'Delete'
      OnClick = DelAssoc
    end
    object N8: TMenuItem
      Caption = '-'
    end
    object atr1: TMenuItem
      Caption = 'atr'
      OnClick = SetAssocType
    end
    object prp1: TMenuItem
      Caption = 'prp'
      OnClick = SetAssocType
    end
    object tmp1: TMenuItem
      Caption = 'tmp'
      OnClick = SetAssocType
    end
    object N46: TMenuItem
      Caption = '--'
      OnClick = SetAssocType
    end
    object N47: TMenuItem
      Caption = '=='
      OnClick = SetAssocType
    end
    object N48: TMenuItem
      Caption = '**'
      OnClick = SetAssocType
    end
    object N37: TMenuItem
      Caption = '-'
    end
    object Addkeyword: TMenuItem
      Caption = 'Add keyword'
      OnClick = AddKeyword
    end
    object Delkeyword: TMenuItem
      Caption = 'Delete keyword'
      OnClick = DeleteKeyword
    end
  end
end
