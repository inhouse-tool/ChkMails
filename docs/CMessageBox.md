# `CMessageBox`

## �\��

�\�[�X�R�[�h���,
[`MessageBox.h`](../ChkMails/ChkMails/MessageBox.h) ��
[`MessageBox.cpp`](../ChkMails/ChkMails/MessageBox.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor & Destructor

[`MessageBox`](#cmessagebox-1)
[`~MessageBox`](#cmessagebox-2)

#### Public

[`CreateModal`](#createmodal)

#### Override

[`OnInitDialog`](#oninitdialog)
[`DestroyWindow`](#destroywindow)
[`OnCommand`](#oncommand)

#### �W�� Window Message �n���h���[

[`OnShowWindow`](#onshowwindow)

#### �ŗL�֐�

[`FillDialog`](#filldialog)
[`FillIcon`](#fillicon)
[`FillText`](#filltext)
[`FillButtons`](#fillbuttons)
[`SelectButtons`](#selectbuttons)
[`SelectIcon`](#selecticon)
[`Align`](#align)


## �T�v

[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
�ɐ��������
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
��\������ class �ł�.

![](../Examples/Exit.png)

�Ȃ�����Ȃ��̂��킴�킴���肵�Ă���̂��Ƃ�����,
`MessageBox`���^�X�N�o�[ ( ��̃A�C�R�� ) �Ɋ�����[�����ɏo��������������v
�Ƃ������Ƃǂ��ł��ǂ������ȓ��@����ł�.
���̃A�v��, �S�Ẵ��[�U�[���삪�^�X�N�o�[�̒ʒm�̈�ɂ���A�C�R����������Ƃ��납��n�܂�̂�,
�Ȃ�ׂ����̋ߏ��ő�����ς܂�����������ł�.

���̐�����肪�ӊO�Ɩʓ|�Ȃ̂�,

* [`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
�n�̃_�C�A���O��, �Ăяo���ƕK��
[`DoModal()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) �ɓ���.

* [`DoModal()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) �ɓ����,
�O���瓭�������ă_�C�A���O�𓮂�����i���Ȃ�.

* [`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) �ɓ���O��,
�����������삷�ׂ��_�C�A���O�����݂��Ȃ�.

�Ƃ��������, �_�C�A���O�̔h�� class ������ē������瑀�삵�Ȃ���΂Ȃ�Ȃ��Ƃ���ł�.

�����ЂƂʓ|�Ȃ̂�,
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
�ŏo��_�C�A���O�ɂ�[�_�C�A���O���\�[�X](https://learn.microsoft.com/ja-jp/cpp/mfc/creating-the-dialog-resource)���Ȃ�
( ���\�[�X�G�f�B�^�Ń_�C�A���O�̒�`��p�ӂ��Ă����K�v���Ȃ� )
�Ƃ����_�ł�.
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
�n�̃_�C�A���O��,
�Ăяo�����Ɓu�������[��Ƀ_�C�A���O�e���v���[�g���`������v�Ƃ����悭����Ȃ����U���g����,
���\�[�X�ɒ�`����Ă��Ȃ��_�C�A���O������ɕ\�����܂�.

�Ȃ̂ł��� class �̒��ł�, ���c
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
�Ɠ����悤�ȏ�����i�߂�,
���c�Ɠ����悤�Ƀ_�C�A���O�����\�[�X�Ȃ��ŏo���Ă��܂�.

���c�ƈقȂ�_��,

* �_�C�A���O�����������ꂽ�ۂƕ����ۂ�
[Main Window](CMainwnd.md)
�ɂ��̎|��ʒm����.<br>
( ����� [Main Window](CMainwnd.md) �̕���, �\���ʒu���񂹂Ă��ꂽ��, ���̃_�C�A���O�Ƌ������Ȃ��悤�ɂ��Ă���܂�. )

* ���₷���t�H���g�ő傫�߂ɕ\������.<br>
( �Ƃ����Ă������́uArial�v�ł���, Windows10/11 �o���Ŋm���Ɏg����t�H���g�Ƃ�����, �R������. )

* [`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
��[�o���Ă����A�C�R��](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox#remarks)
�ȊO��, �A�v���̃��C���̃A�C�R�� (`IDR_MAINFRAME`) ���o����.<br>
( [�������̂���������](../README.md#�^�p) �ŏo���Ă���Ⴊ�\���ł�. )

�Ƃ����Ƃ���ł�.


�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.


## `CMessageBox`

���� class �� constructor �ł�.

�����o�[�ϐ������������Ă��܂�.
�������Ȃ���
[C26495](https://learn.microsoft.com/en-us/cpp/code-quality/c26495)
���w�E����ăE�U������, �Ƃ������R�ŕt���Ă���ɂ����܂���.

�u��� class �� constructor ���Ăяo���v�ȂǂƂ������厖�Ȏd�����ʂ����Ă���킯�ł�����܂���.
��� class �� constructor �ł���
[`CDialog::CDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#cdialog)
��, �_�C�A���O�{�b�N�X�e���v���[�g���\�[�X���肫�̍\�����Ȃ̂�,
����Ȃ��̂��Ȃ����� 
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
���ǂ��� class ����͌Ăт悤������܂���.
�}�W��
[C26495](https://learn.microsoft.com/en-us/cpp/code-quality/c26495)
�����Ȃ����ł�.

���̒i�K�ł͈Ӗ��̂Ȃ����̏����l�Ń����o�[�ϐ������������邾����,
�Ӗ��̂���l�Ń����o�[�ϐ������߂���̂�,
[`CreateModal`](#createmodal)
�ł̘b�ƂȂ�܂�.


## `~CMessageBox`

���� class �� destructor �ł�.

�ғ����ɍ\�z����
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
�����߂��������[���J�����邾���ł�.


## `CreateModal`

Message Box ���� Modal �ȃ_�C�A���O�� create ���܂�.

���֐��ւ̈�����,
[`AfxMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
��
[`CWinApp::DoMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#domessagebox)
�Ɠ��l�ł���,
�w���v�� ID �͂���܂���.
�e�L�X�g�ƃ^�C�v�̎w��݂̂ł�.
<sub>
( ��������
[`AfxMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
�Ńw���v�� ID ���Ďw�肵�����ƂȂ���ł����. �f�t�H���g�łȂ��Ă��������ƂɂȂ��Ă邵. )
</sub>

�����̈����Ɋ�Â���
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
���\�z��, ����
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
���g���ă_�C�A���O������������Ƃ����̂����֐��̎d���ł�.
���̎d�������L�̃��C���i�b�v�ŉʂ����܂�.

| �S�� | �d�����e |
| --- | --- |
| [`FillDialog`](#filldialog) | �_�C�A���O�� `DLGTEMPLATE` �̍\�z |
| [`FillIcon`](#fillicon) | �A�C�R���� `DLGITEMTEMPLATE` �̍\�z |
| [`FillText`](#filltext) | �e�L�X�g�� `DLGITEMTEMPLATE` �̍\�z |
| [`FillButtons`](#fillbuttons) | �{�^���� `DLGITEMTEMPLATE` �̍\�z |

�܂�
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
���\�z��, ���̉��Ɋe�A�C�e����
[`DLGITEMTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate)
�������Ă����Ƃ����̂������̎d���̒i���ł���,
��L�̃��C���i�b�v�̊֐��͂��ꂼ�� 2�x�Ăяo����܂�.
1�x�ڂ͂��ꂼ��̃e���v���[�g�̃T�C�Y�����߂邽��,
2�x�ڂ͂��ꂼ��̃e���v���[�g�ɒ��g���l�߂Ă������߂ł�.
���� 1�x�ڂ� 2�x�ڂ̊Ԃ�, ���߂�ꂽ�T�C�Y���̃������[���m�ۂ����Ƃ����܂��Ă��܂�.

�Ō��, �������[��Ɋm�ۂ���, ���g���l�߂�ꂽ
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
��������
[`CDialog::InitModalIndirect`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#initmodalindirect)
���Ăяo����, Modal �_�C�A���O�̃I�u�W�F�N�g�����������܂�.

Modal �_�C�A���O�̃I�u�W�F�N�g������������Ă��܂���,
���Ƃ� [`CDialog::DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
���Ăяo���΂���ƂȂ�܂�.


## `OnInitDialog`

[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
�� override �ł�.

�܂��͕��ʂ�
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
���Ă񂾌�, �ȉ��̓Ǝ����������݂܂�.

1. [`NotifyPopup`](CChkMailsApp.md#notifypopup) ���Ă�Ń_�C�A���O������ꂽ���Ƃ�ʒm.
1. �t�H�[�J�X��ݒ肵�Ă��Ȃ��̂�, `TRUE` ��Ԃ�.

�������ꂾ���ł�.
�������ꂾ���̂��Ƃ���������, �킴�킴����� class ��������悤�Ȃ��̂ł�.
�f��
[`CDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class)
��,

* �_�C�A���O�� window �m����� `CWnd*` ( �� `HWND` ) ��ʒm���� ( �܂��̓R�[���o�b�N���Ă� )

�Ƃ����@�\��������t���Ă����, ����ȋ�J�����邱�Ƃ��Ȃ�������ł�.

���̓Ǝ������̂�������,
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
���Ă���_�C�A���O���D���Ȉʒu�Ɉړ��ł���悤�ɂȂ�܂�. <sup>*</sup>
<br>
<sub>
*
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
���ċC�̗����Ȃ��d�l�ł���ˁB�ƌ����Ă���悤�Ȃ���ł���.
</sub>


## `DestroyWindow`

`CDialog::DestroyWindow` �� override �ł�.

���ʂ�
`CDialog::DestroyWindow`
���ĂԑO��, �ȉ��̓Ǝ����������݂܂�.

1. [`NotifyPopup`](CChkMailsApp.md#notifypopup) ���Ă�Ń_�C�A���O�����������Ƃ�ʒm.

�������ꂾ���ł�.

���̓Ǝ������̂�������, �_�C�A���O��
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
���Ă�����Ԃ�c���ł���悤�ɂȂ�܂�. <sup>*</sup>
<br>
<sub>
*
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
���ċC�̗����Ȃ��d�l�ł���ˁB�ƌ����Ă���悤�Ȃ���ł���.
</sub>


## `OnCommand`

`CDialog::OnCommand` �� override �ł�.

[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
�n�̃_�C�A���O��, `CDialog::OnCommand` ���Ăяo�����ǖʂƂ�,
���[�U�[�������ꂩ�̃{�^�������������Ƃ��Ӗ����܂�.
�܂�
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
�n�̃_�C�A���O�����ׂ��Ƃ����Ӗ����܂�.

�����ł�,
���̃R�}���h�� ID ��
[`CDialog::EndDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#enddialog)
���Ăяo��,
[`CDialog::DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
�̖߂�l���R�}���h�� ID �ƂȂ�悤�ɂ��Ă��܂�.
�Ⴆ��, `IDOK` �̃{�^���������ꂽ��, `DoModal` �̖߂�l�� `IDOK` �ƂȂ�, �Ƃ��������Ƃł�.

[`CDialog::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#onok)
��
[`CDialog::OnCancel`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oncancel)
�Ȃǂ�, �ʂł͂Ȃ���ԑŐs�� override ���Ă�����̂Ƃ�������������.
���������Ă����Ȃ��� `Yes` / `No` �Ȃǂ́u`OK` / `Cancel` �ȊO�̃{�^���v�ɐ����������ł��Ȃ���ł�.

## `OnShowWindow`

Window Message [`WM_SHOWWINDOW`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-showwindow)
�ɑ΂���n���h���[�ł�.

�\�����ꂽ�Ƃ���, �f�t�H���g�̃{�^����
[`CDialog::SetDefID`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#setdefid) ��,
���̓t�H�[�J�X��^���܂�.


## `FillDialog`

�_�C�A���O��
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
���\�z���܂�.

�܂�,
�m�ۂ��ׂ��������[�̃T�C�Y�����߂�Ăяo����,
�m�ۂ����������[�Ƀe���v���[�g�̒��g���������ނ��߂̌Ăяo����؂蕪���܂�.
�������[�T�C�Y�����߂�Ăяo���̏ꍇ��,
�A�C�e�� 1���͊m���Ɏ��܂鉼�u���p�̃������[�����O�Ŋm�ۂ��܂�.

�ǂ���̌Ăяo���̏ꍇ��, �ȍ~�̏����͕ς��܂���.
���g���������ޏ�����,
�����Ƃ��ė^����ꂽ�������[�ɍs���̂�,
���O�Ŋm�ۂ������u���p�̃������[�ɍs���̂������̈Ⴂ�ł�.
���̏������ݏ�����, �ȉ��̎菇�ōs���܂�.

1. �^�C�v��������A�C�R���̎�ʂ𒊏o��, [`SelectIcon`](#selecticon) �ŃA�C�R���̗L���ƃ��\�[�X ID �𓾂�.
1. �^�C�v��������{�^���̎�ʂ𒊏o��, [`SelectButtons`](#selectbuttons) �Ń{�^���̌��ƃ��\�[�X ID �𓾂�.
1. �Ƃ肠�����ŏ����̃T�C�Y��, `DLGTEMPLATE` �̒��g���l�߂�.
1. `DLGTEMPLATE` �̒���ɒǉ��̔z���u��.

[�����̔z��](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate#remarks)��
`WORD` �̔z���,
�ȉ��̂悤�ɗv�f�����ׂ��Ă��܂�.

| `0x0000`| ���j���[�z�� (�͖���) |
| --: | :-- |
| `0x0000`| �N���X�z�� (������) |
| `L"ChkMails\0"`| �^�C�g���z�� |
| `9`| �t�H���g�T�C�Y |
| `L"Arial\0"`| �t�H���g�� |

���j���[�z���N���X�z��͎g���Ă��Ȃ��̂Ō��ߑł��� `0` �Ƃ��Ă��܂�.
�^�C�g���z��̓A�v���P�[�V�����̖��O��,
�t�H���g�T�C�Y�͓� class �Œ�`���� `9` �����ߑł��œ���,
�t�H���g���͓� class �Œ�`���� `"Arial"` �̕�����𖄂ߍ���ł��܂�.

�Ō��, ���̔z�� 32bit ( 4byte ) ���E�ɏ��悤�� [`Align`](#align) �Œ������܂�.

�ȏ�̏������݂��I������A�h���X�Ə������݂��n�߂��A�h���X�̍�����,
�߂�l�Ƃ��ĕԂ��o�C�g���Ƃ��܂�.

�����čŌ��, �������[�̃T�C�Y�����߂�Ăяo���̏ꍇ�͉��u���̃������[���J�����܂�.


## `FillIcon`

�A�C�R����
[`DLGITEMTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate)
���\�z���܂�.

�܂�,
�m�ۂ��ׂ��������[�̃T�C�Y�����߂�Ăяo����,
�m�ۂ����������[�Ƀe���v���[�g�̒��g���������ނ��߂̌Ăяo����؂蕪���܂�.
�������[�T�C�Y�����߂�Ăяo���̏ꍇ��,
�A�C�e�� 1���͊m���Ɏ��܂鉼�u���p�̃������[�����O�Ŋm�ۂ��܂�.

�ǂ���̌Ăяo���̏ꍇ��, �ȍ~�̏����͕ς��܂���.
���g���������ޏ�����,
�����Ƃ��ė^����ꂽ�������[�ɍs���̂�,
���O�Ŋm�ۂ������u���p�̃������[�ɍs���̂������̈Ⴂ�ł�.
���̏������ݏ�����, �ȉ��̎菇�ōs���܂�.

1. �^�C�v��������A�C�R���̎�ʂ𒊏o��, [`SelectIcon`](#selecticon) �ŃA�C�R���̗L���ƃ��\�[�X ID �𓾂�.
1. �A�C�R��������ꍇ��, ���̃A�C�R���� `DLGITEMTEMPLATE` �̒��g���l�߂�.
1. �A�C�R��������ꍇ��, `DLGITEMTEMPLATE` �̒���Ɂu�N���X�z��v, �u���\�[�X�z��v, �u�^�C�g���z��v��u��.

[�����̔z��](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate#remarks)��
`WORD` �̔z���,
�A�C�R���̏ꍇ�͈ȉ��̂悤�ɗv�f�����ׂ��Ă��܂�.

| `0xffff`| �N���X�z��: �����\�� |
| --: | :-- |
| `0x0082`| ����: 'Static contol' |
| `0xffff`| ���\�[�X�z��: �����\�� |
| `0x0080`| ����: `IDI_QUESTION` |
| `\0`| �^�C�g���z��: ������\��(�󕶎���) |

* �u�N���X�z��v�͖`���� `0xffff` �ŕ�����ł͂Ȃ������l�ł��邱�Ƃ�����,<br>
���� `0x0082` �� static control �ł��邱�Ƃ����������ƂȂ��Ă��܂� ( �A�C�R���� static �̈�� ).
* �u���\�[�X�z��v�͖`���� `0xffff` �ŕ�����ł͂Ȃ������l�ł��邱�Ƃ�����,<br>
���� Resource ID �� `IDI_QUESTION` �� `IDR_MAINFRAME` �Ȃǂ̃A�C�R���� ID �������������u����Ă��܂�.
* �u�^�C�g���z��v�͖`���� `0xffff` �ł͂Ȃ��̂ŏ����l�ł͂Ȃ�������ł��邱�Ƃ������Ă��܂���,<br>
�`������ `\0` �Ȃ̂ŋ󕶎���ƂȂ��Ă��܂�.

�Ō��, ���̔z�� 32bit ( 4byte ) ���E�ɏ��悤�� [`Align`](#align) �Œ������܂�.

�ȏ�̏������݂��I������A�h���X�Ə������݂��n�߂��A�h���X�̍�����,
�߂�l�Ƃ��ĕԂ��o�C�g���Ƃ��܂�.

�����čŌ��, �������[�̃T�C�Y�����߂�Ăяo���̏ꍇ�͉��u���̃������[���J�����܂�.


### `FillText`

�e�L�X�g��
[`DLGITEMTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate)
���\�z���܂�.

�܂�,
�m�ۂ��ׂ��������[�̃T�C�Y�����߂�Ăяo����,
�m�ۂ����������[�Ƀe���v���[�g�̒��g���������ނ��߂̌Ăяo����؂蕪���܂�.
�������[�T�C�Y�����߂�Ăяo���̏ꍇ��,
�A�C�e�� 1���͊m���Ɏ��܂鉼�u���p�̃������[�����O�Ŋm�ۂ��܂�.

�ǂ���̌Ăяo���̏ꍇ��, �ȍ~�̏����͕ς��܂���.
���g���������ޏ�����,
�����Ƃ��ė^����ꂽ�������[�ɍs���̂�,
���O�Ŋm�ۂ������u���p�̃������[�ɍs���̂������̈Ⴂ�ł�.
���̏������ݏ�����, �ȉ��̎菇�ōs���܂�.

1. [Main Window](CMainwnd.md) ���� Device Context ��q�؂��āu���₷���t�H���g�v�ł� Dialog Unit �����߂�.
1. �^����ꂽ�e�L�X�g��\�����邽�߂ɏc���� Dialog Unit �K�v�����߂�.
1. �A�C�R��������ꍇ��, ���̃A�C�R���̕��������ɉ�����.
1. [`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)�̉����E�c����\���ɕK�v�ȕ��g�傷��.
1. �q�؂��� Device Context ���J������.
1. �e�L�X�g�� `DLGITEMTEMPLATE` �̒��g���l�߂�.
1. `DLGITEMTEMPLATE` �̒���Ƀe�L�X�g�́u�N���X�z��v, �u���\�[�X�z��v, �u�^�C�g���z��v��u��.

[�����̔z��](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate#remarks)��
`WORD` �̔z���,
�e�L�X�g�̏ꍇ�͈ȉ��̂悤�ɗv�f�����ׂ��Ă��܂�.

| `0xffff`| �N���X�z��: �����\�� |
| --: | :-- |
| `0x0082`| ����: 'Static contol' |
| `"Quit?\0"`| ���\�[�X�z��: ������\�� |
| `\0`| �^�C�g���z��: ������\��(�󕶎���) |

* �u�N���X�z��v�͖`���� `0xffff` �ŕ�����ł͂Ȃ������l�ł��邱�Ƃ�����,<br>
���� `0x0082` �� static control �ł��邱�Ƃ����������ƂȂ��Ă��܂�.
* �u���\�[�X�z��v�͖`���� `0xffff` �ł͂Ȃ��̂ŏ����l�ł͂Ȃ�������ł��邱�Ƃ�����,<br>
�����v�f�� `\0` �܂ł����̕�����ł��邱�Ƃ������܂�.
* �u�^�C�g���z��v�͖`���� `0xffff` �ł͂Ȃ��̂ŏ����l�ł͂Ȃ�������ł��邱�Ƃ������Ă��܂���,<br>
�`������ `\0` �Ȃ̂ŋ󕶎���ƂȂ��Ă��܂�.

�Ō��, ���̔z�� 32bit ( 4byte ) ���E�ɏ��悤�� [`Align`](#align) �Œ������܂�.

�ȏ�̏������݂��I������A�h���X�Ə������݂��n�߂��A�h���X�̍�����,
�߂�l�Ƃ��ĕԂ��o�C�g���Ƃ��܂�.

�����čŌ��, �������[�̃T�C�Y�����߂�Ăяo���̏ꍇ�͉��u���̃������[���J�����܂�.

�Ƃ���Łc�c
<br>
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
�Ɏd���ޕ��⍂���̒P�ʂ́uDialog Unit (DLU)�v�Ƃ����u�f�o�C�X�Ɉˑ����Ȃ�(�Ƃ����t���R�~��)�v�P�ʂȂ̂ł���,
[Microsoft&reg; �̐���](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/bb226789(v=vs.85))�ɂ���,
> About DLUs: In unmanaged code, the system measures the placement of controls on a surface in dialog units (DLUs), not pixels. A dialog unit is the device-independent measure to use for layout. One horizontal dialog unit is equal to one-fourth of the average character width for the current system font. One vertical dialog unit is equal to one-eighth of an average character height for the current system font.

�Ƃ������Ƃ�,
�������� 1 [Dialog Unit] ��
> one-fourth of the average character width for the current system font

�܂�u�V�X�e���t�H���g�̕������̕��ϒl�� 1/4�v�ƂȂ�܂�.
�u���ρv�Ƃ������Ă��s���Ƃ��܂���,
( �v���|�[�V���i���ȃt�H���g�ł� )
�啶���� 'W' �Ə������� 'i' �Ƃ��F�X�����قȂ�̂ł����������Ƃɂ����̂ł��傤.

�Ȃ̂�, �K�R�I��

* �S���啶���̕���������Ă����ƁA���̌��ς��肪����Ȃ��Ȃ�

�Ƃ������ƂɂȂ�܂�.
<sub>
( �f��
[`AfxMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
�Ŏ����Ă݂���, �����Ȃ邱�Ƃ��m�F�ł��܂���. )
</sub>

�Ă̒�, [���C�Z���X�錾](../README.md#license-announcement)�̑S���啶���������������������̂�,
���̎����ł� Dialog Unit �ɏ����␳��������
�u���Ȃ��Ƃ����C�Z���X�錾�����܂���x�v�ɂ͍L���Ă��܂�.

### `FillButtons`

�{�^����
[`DLGITEMTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate)
���\�z���܂�.

�܂�,
�m�ۂ��ׂ��������[�̃T�C�Y�����߂�Ăяo����,
�m�ۂ����������[�Ƀe���v���[�g�̒��g���������ނ��߂̌Ăяo����؂蕪���܂�.
�������[�T�C�Y�����߂�Ăяo���̏ꍇ��,
�A�C�e�� 1���͊m���Ɏ��܂鉼�u���p�̃������[�����O�Ŋm�ۂ��܂�.

�ǂ���̌Ăяo���̏ꍇ��, �ȍ~�̏����͕ς��܂���.
���g���������ޏ�����,
�����Ƃ��ė^����ꂽ�������[�ɍs���̂�,
���O�Ŋm�ۂ������u���p�̃������[�ɍs���̂������̈Ⴂ�ł�.

���̏������ݏ�����,
�܂��^�C�v�������� [`SelectButtons`](#selectbuttons) �Ń{�^���̌��Ƃ��̎�ʂ𓾂�,
�{�^���̌������ȉ��̎菇���J��Ԃ��܂�.

1. �{�^���� `DLGITEMTEMPLATE` �̒��g���l�߂�.
1. `DLGITEMTEMPLATE` �̒���Ƀ{�^���́u�N���X�z��v, �u���\�[�X�z��v, �u�^�C�g���z��v��u��.

[�����̔z��](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate#remarks)��
`WORD` �̔z���,
�{�^���̏ꍇ�͈ȉ��̂悤�ɗv�f�����ׂ��Ă��܂�.

| `0xffff`| �N���X�z��: �����\�� |
| --: | :-- |
| `0x0080`| ����: 'Button contol' |
| `"OK\0"`| ���\�[�X�z��: ������\�� |
| `\0`| �^�C�g���z��: ������\��(�󕶎���) |

* �u�N���X�z��v�͖`���� `0xffff` �ŕ�����ł͂Ȃ������l�ł��邱�Ƃ�����,<br>
���� `0x0080` �� button control �ł��邱�Ƃ����������ƂȂ��Ă��܂�.
* �u���\�[�X�z��v�͖`���� `0xffff` �ł͂Ȃ��̂ŏ����l�ł͂Ȃ�������ł��邱�Ƃ�����,<br>
�����v�f�� `\0` �܂ł����̕�����ł��邱�Ƃ������܂�.
* �u�^�C�g���z��v�͖`���� `0xffff` �ł͂Ȃ��̂ŏ����l�ł͂Ȃ�������ł��邱�Ƃ������Ă��܂���,<br>
�`������ `\0` �Ȃ̂ŋ󕶎���ƂȂ��Ă��܂�.

�Ō��, ���̔z�� 32bit ( 4byte ) ���E�ɏ��悤�� [`Align`](#align) �Œ������܂�.

�ȏ�̏������݂��I������A�h���X�Ə������݂��n�߂��A�h���X�̍�����,
�߂�l�Ƃ��ĕԂ��o�C�g���Ƃ��܂�.

�����čŌ��, �������[�̃T�C�Y�����߂�Ăяo���̏ꍇ�͉��u���̃������[���J�����܂�.


## `SelectButtons`

�^�C�v�����ɉ������{�^����I�т܂�.

[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
�n�̃_�C�A���O�ɂ�
[�^�C�v](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox#parameters)
�Ƃ���������������,
���̒��Ɂu�ǂ����� `MessageBox` ���o�����v���w�肷�邠�ꂱ�ꂪ�����Ă���̂ł���,
���̂��ꂱ��ɉ����āu�ǂ������{�^�����o�����v�����߂Ă��܂�.

�{�^���͕��� ( �ő� 3�� ) ����̂�,
���̊֐����Ăяo�����ł���S�������܂�z���p�ӂ��Ă������,
���̊֐��ł͂��̔z��̒��ɓ������������ޕ����ɂ��Ă��܂�.

�������ޓ�����,
�{�^����\�����鏇�Ԓʂ��

* �{�^���� ID
* �{�^���̃L�[�g�b�v�ɕ\�����镶����

�Ƃ��Ă��܂�.

���֐����Ăяo���Ă���͈̂ȉ��݂̂Ȃ���ł�:

| �Ăяo���� | �ړI |
| --- | --- |
| [`FillDialog`](filldialog) | �{�^���̌��� `DLGTEMPLATE` �̃A�C�e�����ɔ��f�����邽�� |
| [`FillButtons`](fillicon) | �{�^���� Resource ID �� `DLGITEMTEMPLATE` �Ɏd���ނ��� |


## `SelectIcon`

�^�C�v�����ɉ������A�C�R����I�т܂�.

[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
�n�̃_�C�A���O�ɂ�
[�^�C�v](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox#parameters)
�Ƃ���������������,
���̒��Ɂu�ǂ����� `MessageBox` ���o�����v���w�肷�邠�ꂱ�ꂪ�����Ă���̂ł���,
���̂��ꂱ��ɉ����āu�ǂ������A�C�R�����o�����v�����߂Ă��܂�.
( �A�C�R�����Ȃ��ꍇ������܂�. )

�߂�l��, �A�C�R���̃��\�[�X ID ( `IDI_QUESTION` �Ƃ� `IDR_MAINFRAME` �Ƃ� ) �ł�.
�߂�l�̌^�� `LPWSTR` �ƂȂ��Ă���, �Ȃɂ��������\�����Ă���悤�Ɍ����܂���,
���ۂɊi�[����Ă���̂̓��\�[�X ID �����������l�ł�.

���֐����Ăяo���Ă���͈̂ȉ��݂̂Ȃ���ł�:

| �Ăяo���� | �ړI |
| --- | --- |
| [`FillDialog`](filldialog) | �A�C�R���̗L���� `DLGTEMPLATE` �̃A�C�e�����ɔ��f�����邽�� |
| [`FillIcon`](fillicon) | �A�C�R���� Resource ID �� `DLGITEMTEMPLATE` �Ɏd���ނ��� |
| [`FillText`](filltext) | �A�C�R���̗L���Ńe�L�X�g�̔z�u���ς�邽�� |

## `Align`

�^����ꂽ�|�C���^�[�� 4�o�C�g�A���C�����g�ɏ�����l�ɂ��ĕԂ��܂�.

�Ȃ�ł���Ȃ��Ƃ�����Ă���̂��Ƃ�����,
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
��
[`DLGITEMTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate)
��, 4�o�C�g���E�ɂ̂����ʒu�ɒu���Ȃ���΂Ȃ�Ȃ�����ł�.