object main: Tmain
  Left = 253
  Top = 394
  BorderStyle = bsDialog
  Caption = 'Enter Serial Number'
  ClientHeight = 114
  ClientWidth = 250
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 18
    Top = 11
    Width = 123
    Height = 13
    Caption = 'Serial Number (0 - 65535):'
  end
  object Label2: TLabel
    Left = 14
    Top = 35
    Width = 127
    Height = 13
    Caption = 'Serial Number Location: 0x'
  end
  object number: TEdit
    Left = 144
    Top = 8
    Width = 97
    Height = 21
    MaxLength = 5
    TabOrder = 0
    Text = '65535'
  end
  object bigendian: TRadioButton
    Left = 56
    Top = 56
    Width = 81
    Height = 17
    Caption = 'Big Endian'
    Checked = True
    TabOrder = 1
    TabStop = True
  end
  object littleendian: TRadioButton
    Left = 144
    Top = 56
    Width = 97
    Height = 17
    Caption = 'Little Endian'
    TabOrder = 2
  end
  object okbutton: TButton
    Left = 168
    Top = 80
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 3
    OnClick = okbuttonClick
  end
  object location: TEdit
    Left = 144
    Top = 32
    Width = 97
    Height = 21
    MaxLength = 4
    TabOrder = 4
    Text = 'FFFF'
  end
end
