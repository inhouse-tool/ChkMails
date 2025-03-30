# `CNamePage`

## �\��

�\�[�X�R�[�h���,
[`NamePage.h`](../ChkMails/ChkMails/NamePage.h) ��
[`NamePage.cpp`](../ChkMails/ChkMails/NamePage.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CNamePage`](#cdomainpage-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`OnOK`](#onok)
[`PreTranslateMessage`](#pretranslatemessage)

#### �W�� Window Message �n���h���[

[`OnDropFiles`](#ondropfiles)
[`OnTimer`](#ontimer)

#### �R�}���h�n���h���[

[`OnButtonAdd`](#onbuttonadd)
[`OnButtonDelete`](#onbuttondelete)
[`OnChangeEdit`](#onchangeedit)
[`OnChangeList`](#onchangelist)
[`OnEditPaste`](#oneditpaste)

#### �ŗL�֐�

[`OnSelectItem`](#onselectitem)
[`SortName`](#sortname)
[`AddNames`](#addnames)
[`LoadText`](#loadtext)


## �T�v

[Name](../README.md#name) �̐ݒ���s�����߂́uPage�v�ł�.<br>
[`CProperSheet`](CProperSheet.md) �ɏ��������
[`CFilterSheet`](CFilterSheet.md) ��
���˂Ă��� page �̂����� 1�ł�.

![](../pics/Filter.Name4.png)

���[���� `From:` ���ɂ�������ȕʖ� ( Alias ) �ƃ��[���A�h���X�̑g�ݍ��킹��o�^���邽��,
�ʖ��ƃ��[���A�h���X�̕�������͂��󂯕t��,
`Add` �{�^���Ń��X�g�ɓo�^���܂�.

���炩���ߍ���Ă������g�ݍ��킹�̃e�L�X�g�t�@�C���� Drag & Drop �Ŏ󂯕t������,
�g�ݍ��킹���L�q�����e�L�X�g�̒��g�� Copy & Paste �Ŏ�荞�ދ@�\�������Ă��܂�.

�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.


## `CNamePage`

���� class �� constructor �ł�.

�\�[�X�R�[�h�������, ��������Ă��Ȃ��悤�Ɍ����āu�v��́H����B�v�Ƃ��������ł���,
���\�[�X ID ���w�肵�Ċ��N���X�� constructor ���ĂԂƂ����厖�Ȏd�������Ă��܂�.


## `OnInitDialog`

[`CPropertyPage::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
�� override �ł�.

�܂��͕��ʂ�
`CPropertyPage::OnInitDialog` ���Ă񂾌�,
( `CPropertyPage` �� `OnInitDialog` �� override ���Ă��Ȃ��悤�Ȃ̂�,
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
�ɃX���[�p�X���ꂽ��, )
 �ȉ��̓Ǝ����������݂܂�.

1. [`GetCommonFont`](#CChkMailsApp.md#getcommonfont) �ŋ��ʂ̓����t�H���g�𓾂�.
1. Edit Box �ɓ����t�H���g���Z�b�g����.
1. List Control �ɓ����t�H���g���Z�b�g����.
1. List Control ��S�J�����őI��\������悤�ɐݒ�.
1. List Control ���O���b�h�\������悤�ɐݒ�.
1. List Control �� 2�J�����ǉ�.
1. [`SortName`](#sortname) �Ŗ��`�z����\�[�g.
1. �u���`�z��v��List Control �ɒǉ�.
1. List Control �̃J����������������.
1. �t�H�[�J�X��ݒ肵�Ă��Ȃ��̂�, `TRUE` ��Ԃ�.


## `OnOK`

[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
�� override �ł�.

��� class �̎����ł���
[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
���Ă΂���, �ȉ��̏��������s���܂�.

* List Control �ɂ���e�y�A���u���`�z��v�Ɋi�[.

���� `public` �Ȕz��� `OnOK` �̌�,
[Main Window](CMainWnd.md) ������ɉ�����Ă����蔤�ɂȂ��Ă��܂�.


## `PreTranslateMessage`

`CProperSheet::PreTranslateMessage` �� override �ł�.

`WM_KEYDOWN` �������Ƃ�, `Domain:` �� Edit Box �ɓ��̓t�H�[�J�X�������ē��̓L�[�� `Enter` ��������,
���̎��ۂ��u`Add` �{�^���������ꂽ�v�ɂ���ւ���
[`OnButtonAdd`](#onbuttonadd) ���Ă΂��悤�Ɏd�����Ă��܂�.

�������� Edit Box �ɉ������ꂽ�̂�,
`Add` �{�^�����������ɃC�L�I�C�� `Enter` �L�[��@�����Ⴄ�������肳�����, ���\����������悤�ł�.
������ page ��� `Enter` �L�[��@���� `OK` �{�^���������ꂽ���ƂɂȂ����Ⴄ�̂�,
���������̓��͂��p�[�ł�.
�Ƃ������Ƃ�, ���̋~�ύ�Ƃ��ċ���ł���S�����ł�.


## `OnDropFiles`

Window Message [`WM_DROPFILES`](https://learn.microsoft.com/ja-jp/windows/win32/shell/wm-dropfiles)
�ɑ΂���n���h���[�ł�.
<sub>
( ����󂪒������̂�[�p���](https://learn.microsoft.com/en-us/windows/win32/shell/wm-dropfiles)�����Q�Ƃ�������. )
</sub>

Drop ���ꂽ�t�@�C���̒��g�� [LoadText](#loadtext) �œǂݎ����,
�ǂݎ�������e�� [`AddNames`](#addnames) ���܂�.

���̌���,
Drop ���ꂽ�t�@�C������ǂݎ�����uAlias �ƃ��[���A�h���X�̃y�A�v��,
List Control �Ɏ�荞�܂�邱�ƂɂȂ�܂�.


## `OnTimer`

Window Message [`WM_TIMER`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-timer)
�ɑ΂���n���h���[�ł�.

���L�̃^�C�}�[�l����舵���Ă��܂�.

| �^�C�}�[�l | �� | �������e |
| --- | --- | --- |
| `TID_SELCHANGE` | �u�I��ΏەύX�v | [`OnSelectItem`](#onselectitem) �Ăяo��. |


## `OnButtonAdd`

`Add` �{�^���������ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

`Alias:` �� `Domain:` �� Edit Box �ɓ��͂��ꂽ�y�A�� List Control �ɒǉ����܂�.

������, ���ł� List Control ���ɑS�������y�A���G���g���[�ς݂̏ꍇ��,
�ǉ��͂��܂���.
�������̂𕡐��o�^���Ă��Ӗ����Ȃ��̂�.

`Add` ����̃y�A��, List Control ��ł́u�I������Ă���v��ԂɂȂ��Ă��܂�.
�u�ԈႦ���v�Ǝv�����炷������ `Delete` �{�^���������܂��傤.
�� `Add` �����΂���̃y�A���o�^��������܂�.

`Domain:` �� Edit Box �ւ̓��͂��I�������_��, ���C�L�I�C�� `Enter` �L�[��@���Ă��܂����Ƃ���,
[`PreTranslateMessage`](#pretranslate) �̐S�����œ��֐����Ă΂�Ă��܂�.


## `OnButtonDelete`

`Delete` �{�^���������ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

�� List Control ��őI������Ă���s�̃G���g���[���폜���܂�.

���́u�I������Ă���v����ɕ\�����邽��,
List Control �̌��l�^�ɂȂ��Ă��郊�\�[�X��ł�,
`Always Show Selection` �� `True` �ɐݒ肳��Ă��܂�.
���̂��A�� List Control �ɓ��̓t�H�[�J�X���������Ă��Ȃ��󋵂ł�,
����Ȃ�Ɂu�I������Ă���v�s���A�s�[������܂�.


## `OnChangeEdit`

Edit Box �ŕ����񂪕ύX���ꂽ���ɔ��ł��� `EN_CHANGE` �R�}���h�̃n���h���[�ł�.

* `Alias:` �� `Domain:` �����ꂩ�̕����񂪋�Ȃ� `Add` �{�^���� disable.
* `Alias:` �� `Domain:` �����̕����񂪋�łȂ���� `Add` �{�^���� enable.

�Ƃ��Ă��܂�.


## `OnChangeList`

List Control ��̑I�����ς���ꂽ���ɔ��ł��� `LVN_ITEMCHANGED` �R�}���h�̃n���h���[�ł�.

�҂����ԃ[���� `TID_SELCHANGE` �̃^�C�}�[���������Ă��̏�͂��߂����܂�.

�Ȃ�����Ȃ��Ƃ����Ă��邩�Ƃ�����,
A ���� B �ɑI��������������

* A ���I����Ԃ����I����ԂɂȂ����Ƃ��� `LVN_ITEMCHANGED`
* B ����I����Ԃ���I����ԂɂȂ����Ƃ��� `LVN_ITEMCHANGED`

�������������ł���̂ł߂�ǂ�����������ł�.
���߂����Ă���u���ǁA�����I�΂ꂽ��?�v�ƍ\����������b���Ȃ������̂�.
( �߂�ǂ������炸�Ƀ}�W���ɑΉ�������� [`CDomainPage::OnChangeList`](CDomainPage.md#onchangelist) �ɂ���܂�. )


## `OnEditPaste`

�� page �ɑ΂��� `Ctrl`+`V` ���ꂽ�Ƃ��ɔ��ł��� `ID_EDIT_PASTE` �R�}���h�̃n���h���[�ł�.

�N���b�v�{�[�h�̒��g���e�L�X�g�Ƃ��Ď󂯎��,
���̓��e�� [`AddNames`](#addnames) ���܂�.

���̌���,
Drop ���ꂽ�t�@�C������ǂݎ�����uAlias �ƃ��[���A�h���X�̃y�A�v��,
List Control �Ɏ�荞�܂�邱�ƂɂȂ�܂�.

�Ȃ�, ���� `ID_EDIT_PASTE` �R�}���h�ł���,
[`CFilterSheet::PreTranslateMessage`](CFilterSheet.md#pretranslatemessage)
�l�̊i�ʂȂ��������Ăɂ��, �� class �܂œ͂��Ă��邱�Ƃ�\���Y���Ă����܂�.


## `OnSelectItem`

List Control ��̑I�����ς���ꂽ���ƂɑΉ����܂�.

�����ꂩ�̗L���ȍs���I�����ꂽ�ꍇ�� `Delete` �{�^����L���ɂ�,
�����������ꍇ�� `Delete` �{�^���𖳌��ɂ��邾���ł�.


## `SortName`

��������\�[�g���� `CStringArray` �Ɋi�[���܂�.

`\n` ��؂�� `CString` �Ɏ��߂�ꂽ�h���C����������\�[�g���� `CStringArray` �Ɋi�[���܂�.

�ݒ�l�� registry �Ɋi�[����ۂ� 1�{�̕����񂾂Ɠs�����ǂ��̂ł���,
List �ɕ��ׂĈ������͔z��̕��������₷���̂�,
�������āu�����񁨔z��v�̏������\���Ă��܂�.


## `AddNames`

�u���`�v���L�q�����e�L�X�g�̒��g����荞�݂܂�.

���̃e�L�X�g�̋L�q��,
�uAlias �ƃ��[���A�h���X�̃y�A�v���^�u (`\t`) �ŋ�؂��Ă��邱�Ƃ��O��ɂȂ��Ă���,
�^�u�̑O�̕������ Alias �Ƃ���, �^�u�̌��̕���������[���A�h���X�Ƃ��Ď�荞�݂܂�.

[OnDropFiles](#ondropfiles) �� [OnEditPaste](#oneditpaste) ����Ă΂�Ă��܂�.


## `LoadText`

�^����ꂽ�e�L�X�g�t�@�C�����J��, ���̓��e��ǂݎ���� `CString` ( UTF16LE) �ŕԂ��܂�.

�e�L�X�g�t�@�C���̃G���R�[�h�`���� ASCII, Shift JIS, UTF8, UTF16LE �ɑΉ����Ă���,
���̂�����̌`���Ȃ̂���, ���̊֐������Ŏ������肵�Ă��܂�.

[OnDropFiles](#ondropfiles) ����Ă΂�Ă��܂�.
