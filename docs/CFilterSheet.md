# `CFilterSheet`

## �\��

�\�[�X�R�[�h���,
[`FilterSheet.h`](../ChkMails/ChkMails/FilterSheet.h) ��
[`FilterSheet.cpp`](../ChkMails/ChkMails/FilterSheet.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CFilterSheet`](#cfiltersheet-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`DestroyWindow`](#destroywindow)
[`PreTranslateMessage`](#pretranslatemessage)
[`OnCommand`](#oncommand)

#### �ŗL�֐�

[`OnHelp`](#onhelp)
[`AddPages`](#addpages)


## �T�v

[Filter](../README.md#filter) �z���̐ݒ荀�ڂ𑩂˂Ă���u�V�[�g�v�ł�.

��ʓI�� [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�̏�ɏ�������������̂悤�Ɍ����܂���,
�\�[�X���悭����� `CPropertySheet` �ł͂Ȃ� [`CProperSheet`](CProperSheet.md) �Ƃ���,
�����p�`���������� class �ɏ���Ă��܂�.
���́u�p�`�����v�ɂ��Ă�[������](CProperSheet.md)�ɏ���Ƃ���,
���� class ������Ă���Ǝ��v�f�Ƃ��Ă�:

* `Help` �{�^����ǉ����Ă���.
* `Help` �{�^������ʂ̍��[�Ɋ񂹂Ă���.
* `OK` �{�^������ʂ̉E�[�Ɋ񂹂Ă���.
* `Cancel` �{�^���͏ȗ����Ă���.

�Ƃ������Ƃ���ł�.

����, ���̃V�[�g��� `Ctrl` + `V` �Ɖ�����,
��ʓI�ȃ��j���[�� `Edit` (�ҏW) �J�e�S���[�� `Paste` (�y�[�X�g) �R�}���h�ɒu��������,
���I�΂�Ă��� page �Ɂu�����APaste �����Ă�B�v�Ɠ`���܂�.
�����Ƃ��uPaste �ł��ˁB�������܂����[!�v�Ƃ��������������Ă����̂�,
[Whitelist](../README.md#whitelist) ��
[page](CWhitePage.md)������, ���� page �ł͖�������Ă��܂���.

���̃V�[�g�� `OK` �{�^���ŕ����,
�I�΂�Ă��� page ���L�����Ă����܂�.
���� page �̐ݒ�����s����ŌJ��Ԃ����Ƃ������̂�.

�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.


## CFilterSheet

���� class �� constructor �ł�.

��� class [`CProperSheet`](CProperSheet.md) �� constructor ���Ă񂾌�,
[`AddPages`](#addpages) ���Ă�ł��邾���ł�.


## OnInitDialog

[`CProperSheet::OnInitDialog`](CProperSheet.md#oninitdialog)
�� override �ł�.

�������e�͈ȉ��̒ʂ�ł�.

1. ��� class �� [`CProperSheet::OnInitDialog`](CProperSheet.md#oninitdialog) ���Ă�.
1. �O�� `OK` �{�^���������ꂽ page ��
[`SetActivePage`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#setactivepage).
1. `OK` �{�^���� `Help` �{�^���̈ʒu�Ɉړ�.
1. `Help` �{�^�����V�[�g�̍����Ɉړ�.
1. `Help` �{�^���� ID �� `IDHELP` ���� `ID_HELP` �ɏ��������ăC�l�[�u��.
1. `Cancel` �{�^���͉B���Ă���.
1. [`NotifyPopup`](CChkMailsApp.md#notifypopup) ���Ă�ŃV�[�g������ꂽ���Ƃ�ʒm.
1. [`CProperSheet::OnInitDialog`](CProperSheet.md#oninitdialog) �̖߂�l��Ԃ�.

�Ƃ��Ă��܂�.

[`CProperSheet::OnInitDialog`](CProperSheet.md#oninitdialog) �̖߂�l�ɂ�����炸,
��L 2.�`6. �̏�����i�߂Ă��܂��Ă���Ƃ��낪,
�ς��ƌ��u��� class �̖߂�l�𖳎����Đi�߂Ă��܂��Ă����̂�?�v�Ƃ�����ۂ�^���܂���,
���̖߂�l��

* `TRUE`: ���̓t�H�[�J�X���ǂ��ɐݒ肷�邩�́u���܂����v��.
* `FALSE`: ���̓t�H�[�J�X�͂������Őݒ肵������, ����ɕς��Ȃ��ł�.

�Ƃ������x�̂��̂Ȃ̂�,
`OnInitDialog` �����s�����ɂ�������炸�˂������Ă���킯�ł͂���܂���.

����, 5. �ł���Ă��� ID �̏��������ł���,
`IDHELP` �̂܂܂ɂ��Ă�����
[���̂������ʓ|�Ȃ�������Ɋ������܂��](https://jeffpar.github.io/kbarchive/kb/132/Q132715/)����ł�.
�� class �ł� `ID_HELP` �ɏ���������, ���̎������瓦��Ă��܂�.
�������ł��Ȃ��ȏ����ɂȂ�܂���.
<br>
<sub>
( [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�̐݌v�ɂ͔[���������܂���.
�����ł��낢�돈�����Ă����̂͂��肪�����ł���,
���ȃ��[���������t���Ȃ��ł�����������. )
</sub>

## `DestroyWindow`

`CProperSheet::DestroyWindow` �� override �ł�.

[`NotifyPopup`](CChkMailsApp.md#notifypopup) ���Ă�ŃV�[�g���������񂾂��Ƃ�ʒm����,
�Ƃ����菇�����ނ��߂����� override ���Ă��܂�.

���Ȃ݂�,
[`CProperSheet`](CProperSheet.md) �� `DestroyWindow` �� override ���Ă��炸,
`CPropertySheet::DestroyWindow` �ɃX���[�p�X�ł�.
( ��, [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class) ��
`DestroyWindow` �� override ���Ă��Ȃ��悤�Ȃ̂�,
`CWnd::DestroyWindow` �ɃX���[�p�X���Ă���悤�ł�. )


## `PreTranslateMessage`

`CProperSheet::PreTranslateMessage` �� override �ł�.

`WM_KEYDOWN` �������Ƃ�,
`Ctrl` + `V` ��������u���ܑI�΂�Ă��� page �ւ� `ID_EDIT_PASTE` �R�}���h�̔��s�v�ɂ���ւ��Ă��܂�.

���̃R�}���h���󂯂邩�������邩��,
���ܑI�΂�Ă��� page ����ł���.


## `OnCommand`

`CProperSheet::OnCommand` �� override �ł�.

* `IDOK` �R�}���h�������ꍇ��, ���ܑI�΂�Ă��� page �������Ă���.
* `ID_HELP` �R�}���h�������ꍇ��, [`OnHelp`](#onhelp) �̌Ăяo���ɑウ�����Ă�������.

�� 2�_�ȊO��, ��� class �Ɋۓ����ł�.


## `OnHelp`

`Help` �{�^���������ꂽ���ɔ��ł��� `ID_HELP` �R�}���h�̃n���h���[�ł�.
<br>
( ���m�ɂ̓n���h���[�ł͂Ȃ� [`OnCommand`](#oncommand) �ɒ��ڌĂяo����Ă��邽���̊֐��ł���. )

���ܑI�΂�Ă��� page �� window handle ������o����, ���Ƃ�
[`ShowHelp`](CChkMailsApp.md#showhelp) �Ɏd�����ۓ����ł�.


## `AddPages`

�V�[�g�� page �������܂�.

�܂�, �V�[�g�̐U�镑����:

* `Help` �{�^���t��
* `Apply` �{�^���Ȃ�
* �A�C�R���� ![](../pics/Icon.White.png)

�Ƃ��܂�.

���Ƃ͎P���� page �S�Ă�
[`AddPage`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#addpage) ���ďI���ł�.

