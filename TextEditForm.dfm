object TxtEdForm: TTxtEdForm
  Left = 328
  Top = 155
  Width = 428
  Height = 373
  Caption = #1056#1077#1076#1072#1082#1090#1086#1088' '#1090#1077#1082#1089#1090#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  DesignSize = (
    420
    346)
  PixelsPerInch = 96
  TextHeight = 13
  object Edit: TMemo
    Left = 0
    Top = 0
    Width = 417
    Height = 300
    Anchors = [akLeft, akTop, akRight, akBottom]
    ScrollBars = ssBoth
    TabOrder = 0
    WordWrap = False
  end
  object Button: TButton
    Left = 256
    Top = 312
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Ok'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object Button1: TButton
    Left = 336
    Top = 312
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
end
