# `CWhitePage`


## �\��

�\�[�X�R�[�h���,
[`WhitePage.h`](../ChkMails/ChkMails/WhitePage.h) ��
[`WhitePage.cpp`](../ChkMails/ChkMails/WhitePage.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CWhitePage`](#cwhitepage-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`OnOK`](#onok)

#### �W�� Window Message �n���h���[

[`OnDropFiles`](#ondropfiles)

#### �R�}���h�n���h���[

[`OnButtonDelete`](#onbuttondelete)
[`OnChangeList`](#onchangelist)
[`OnClickHeader`](#onclickheader)
[`OnEditPaste`](#oneditpaste)

#### �ŗL�֐�

[`TakeSender`](#takesender)
[`AddSender`](#addsender)
[`SortSender`](#sortsender)
[`CompareSender`](#comparesender)


## �T�v

[Whitelist](../README.md#whitelist) �̐ݒ���s�����߂́uPage�v�ł�.<br>
[`CProperSheet`](CProperSheet.md) �ɏ��������
[`CFilterSheet`](CFilterSheet.md) ��
���˂Ă��� page �̂����� 1�ł�.

![](../pics/Filter.WhiteList2.png)

�蓮���͂̓T�|�[�g���Ă��܂���.
���l�^�ƂȂ郁�[���� `.eml` ���@Drag & Drop ���Ă��炤��,
���l�^�ƂȂ郁�[���̃w�b�_�[�� copy �������̂� paste ���Ă��炤��,
�̓���ł�.

������̏ꍇ��, ���X�g�ɓ���������̒��o�� [`TakeSender()`](#takesender) �Ƃ�����p�֐����S�����Ă���,
���[���̍��o�l�́u���`�v��`From:` ����,
���[���̖{���̑��M�҂� `Authentication-Results:` ( �� `Received:` ) ����,
���ꂼ�ꊄ��o���ė��҂̃y�A�Ń��X�g���\�����܂�.
����, ���̃y�A���擾���鏈����
[Main Window](CMainWnd.md) �Ɋۓ������Ă��܂�.
������ł������Ɩ�����������Ȃ��Ă���̂�.


## `CWhitePage`

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
1. List Control �ɓ����t�H���g���Z�b�g����.
1. List Control ��S�J�����őI��\������悤�ɐݒ�.
1. List Control ���O���b�h�\������悤�ɐݒ�.
1. �J���� 0 �Ƀ_�~�[�������, �J���� 1 �ɉE�l�߂� `From:`, �J���� 2 �ɉE�l�߂� `Domain` ��ǉ�.
1. �J���� 0 �̃_�~�[���폜.
1. �J���� 2 �̃J����������������.
1. �uWhitelist �z��v�̊e�s�ɑ΂��Ĉȉ������s:<br>
�e�s�̍��ӂ�ǉ�.<br>
�e�s�̉E�ӂ��J���� 1 �ɃZ�b�g.<br>
1. List Control ����łȂ���΃J����������������.
1. List Control �̃w�b�_�[���u�J���� 0 �ŏ����\�[�g�v�Ɛݒ�.
1. Page ���i�e�L�X�g���Ɂu�ǉ��̈ē��v���Z�b�g.
1. �t�H�[�J�X��ݒ肵�Ă��Ȃ��̂�, `TRUE` ��Ԃ�.

���̂���, 5. �� 6. �ɂ���u�_�~�[�v�͈ꌩ�s�v�����Ȓi���ł���,
[�u�J���� 0 �͍��l�߂ɂ����ł��Ȃ��v](https://learn.microsoft.com/ja-jp/windows/win32/api/commctrl/ns-commctrl-lvcolumnw#remarks)
�Ƃ��� List Control �̏�Ȃ������������邽�߂̂��̂ł�.
<br>
<sup>
����, �u�����v���������񂶂�Ȃ���, �u�C���v���Ă���񂩂�?
</sup>


## `OnOK`

[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
�� override �ł�.

��� class �̎����ł���
[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
���Ă΂���, �ȉ��̏��������s���܂�.

* �܂��uWhitelist �z��v����ɂ���.
* List Control ���J���� 0 �ŏ����\�[�g.
* List Control �̃J���� 0 �ƃJ���� 1 �� tab �ŋ�؂���������� LF ���s���āuWhitelist �z��v�ɒǉ�.

���� `public` �Ȕz��� `OnOK` �̌�,
[Main Window](CMainWnd.md) ������ɉ�����Ă����蔤�ɂȂ��Ă��܂�.


## `OnDropFiles`

Window Message [`WM_DROPFILES`](https://learn.microsoft.com/ja-jp/windows/win32/shell/wm-dropfiles)
�ɑ΂���n���h���[�ł�.
<sub>
( ����󂪒������̂�[�p���](https://learn.microsoft.com/en-us/windows/win32/shell/wm-dropfiles)�����Q�Ƃ�������. )
</sub>

Drop ���ꂽ `.eml` �t�@�C����Ђ��[����ǂݎ����,
�ǂݎ�������e�� [`TakeSender`](#takesender) ���܂�.

���̌���,
Drop ���ꂽ `.eml` �t�@�C�����璊�o�����u`From:` �Ƒ��M�ҏ����h���C���̃y�A�v��,
List Control �Ɏ�荞�܂�邱�ƂɂȂ�܂�.

������, ���o���� `From:` �ɋ^�`������ꍇ��, ���̎|��
[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
���Ď�荞�݂����ۂ��܂�.


## `OnButtonDelete`

`Delete` �{�^���������ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

�� List Control ��őI������Ă���s�̃G���g���[���폜���܂�.

�폜���I�������, List Control �̃J�������𒲐����Ă����܂�.


## `OnChangeList`

List Control ��̏�Ԃ��ς���ꂽ���ɔ��ł��� `LVN_ITEMCHANGED` �R�}���h�̃n���h���[�ł�.

�ŏI�I�ɂǂ̍s���I�����ꂽ�̂��m�肽��������
[`CAccountDlg::OnChangeList`](CAccountDlg.md#onchangelist) �ł���Ă��鉡���ȏ����ƈ����,
�����}�W���ɃR�}���h�ɑΉ����Ă��܂�.
�Ȃ��Ȃ�,

* �I����Ԃ����I����ԂɂȂ����Ƃ��� `LVN_ITEMCHANGED`
* ��I����Ԃ���I����ԂɂȂ����Ƃ��� `LVN_ITEMCHANGED`

�̗����ɂ��ꂼ��Ή�����������ł�.
�Ȃ����ꂼ��ɑΉ����������Ƃ�����

* �ǂꂩ�̍s���I������Ă���Ƃ���, `Delete` �{�^���� enable ����, ���i�e�L�X�g���Ɂu�폜�̈ē��v���Z�b�g.
* �ǂ̍s���I������Ă��Ȃ��Ƃ���, `Delete` �{�^���� disable ����, ���i�e�L�X�g���Ɂu�ǉ��̈ē��v���Z�b�g.

�Ƃ������������ނ��߂ł�.

�܂�, �f�t�H���g�� page ( �V�[�g ) �T�C�Y�ł�,
��قǍs�������Ȃ��Ȃ������,
��x�ǂꂩ��I�����Ă��܂���, ��I����Ԃɖ߂��͓̂����������܂���.


## `OnClickHeader`

List Control ��̃w�b�_�[�����ꂽ���ɔ��ł��� `HDN_ITEMCLICK` �R�}���h�̃n���h���[�ł�.

���L�̂悤�ɏ�����i�߂Ă��܂�.

1. �w�b�_�[��̂ǂ̃J���������ꂽ�̂��𓾂�.
1. ���̃J�����̌��݂́u�t�H�[�}�b�g�v�𓾂�.
1. ���݂́u�t�H�[�}�b�g�v���u�����\�[�g�v�Ȃ�u�~���\�[�g�v�ɕύX.
1. ���݂́u�t�H�[�}�b�g�v���u�~���\�[�g�v�Ȃ�u�����\�[�g�v�ɕύX.
1. �ύX�����u�t�H�[�}�b�g�v���w�b�_�[�ɃZ�b�g.
1. ���ꂽ�J�����ȊO�́u�t�H�[�}�b�g�v���u�\�[�g�Ȃ��v�ɕύX���ăw�b�_�[�ɃZ�b�g.
1. [`SortSender`](#sortlist) �ŕ��ג���.

[`CZonePage::OnClickHeader`](CZonePage.md#onclickheader) �ł�,
handler ���Œ��� List Control ���\�[�g���Ă��܂���,
�� class �� handler �ł� [`SortSender`](#sortlist) �Ƃ����S���֐�������ł��܂�.
�����, �� class �ł̓w�b�_�[�̑���ȊO�ɂ��\�[�g���������݂����_�@ ( [`AddSender`](#addsender) ) �����邽�߂ł�.


## `OnEditPaste`

�� page �ɑ΂��� `Ctrl`+`V` ���ꂽ�Ƃ��ɔ��ł��� `ID_EDIT_PASTE` �R�}���h�̃n���h���[�ł�.

�N���b�v�{�[�h�̒��g���e�L�X�g�Ƃ��Ď󂯎��,
���̓��e�� [`TakeSender`](#takesender) ���܂�.

���̌���,
Paste ���ꂽ�e�L�X�g���璊�o�����u`From:` �Ƒ��M�ҏ����h���C���̃y�A�v��,
List Control �Ɏ�荞�܂�邱�ƂɂȂ�܂�.

������, ���o���� `From:` �ɋ^�`������ꍇ��, ���̎|��
[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
���Ď�荞�݂����ۂ��܂�.

�Ȃ�, ���� `ID_EDIT_PASTE` �R�}���h�ł���,
[`CFilterSheet::PreTranslateMessage`](CFilterSheet.md#pretranslatemessage)
�l�̊i�ʂȂ��������Ăɂ��, �� class �܂œ͂��Ă��邱�Ƃ�\���Y���Ă����܂�.


## `TakeSender`

�^����ꂽ���[���̃e�L�X�g����, ���̃��[���̑��M�҂���荞�݂܂�.

���́u���M�ҁv��, ���[���w�b�_�[�ɂ��鉺�L�̏�񂩂�\������Ă��܂�.

* `From:` �ɂ���u���`�v�Ƃ��Ă̍��o�l�̃��[���A�h���X
* `Authentication-Results:` �Ȃǂɂ���u���ہv�̃��[�����M�҂̏����h���C��

���̊֐��ɂ̓��[���w�b�_�[���e�L�X�g�`���ŗ^�����Ă���̂�,
��L�̏���T���΂����̂ł���,
�S�������d����ʏ�c�ƂŌJ��L���Ă��� [Main Window](CMainWnd.md) �Ƃ������݂�����̂�,
������Ɉ˗����邱�Ƃɂ��܂���.
�˗��̕��@�́u`WM_GET_SENDER` ��
[send](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
���邱�Ɓv�ł�.
����� [`CMainWnd::OnGetSender`](CMainWnd.md#ongetsender) ���˗����󂯂Ă���܂�.

[send](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
�Ȃ̂�, �˗������d�����������Ă��炱����ɐ��䂪�߂��Ă��܂�.
���̖߂�l��

* �y�A���L�q���� `\n` ��؂�� 2�s�̕�����ւ̃|�C���^�[

�Ƃ����`���Ŕ[�i����Ă���̂ł���,
���̃|�C���^�[�� `NULL` �������ꍇ��,

�u���̑��M�҂͐M�p�ł��Ȃ��B��荞�ނ̂͂�߂Ă��������������B�v

�Ƃ����Ӗ��Ȃ̂�, �����𒆎~���āu��荞��ł��Ȃ��v���Ӗ����� `false` ��Ԃ��ďI���܂�.

`NULL` �łȂ������ꍇ��,
�����Ƀ|�C���^�[���� `CStringA` ���N����,
�[�i���ꂽ�|�C���^�[�͊J�����܂�.
�����Ă��� `CStringA` �� `From:` �� `���ۂ̑��M��` �ɕ����܂�.

������, `From:` �� `���ۂ̑��M��` �̗������󕶎���łȂ����,
����� `���ۂ̑��M��` ������`�����h���C��`�����ɍ�藎�Ƃ��܂�.
�Ȃ���藎�Ƃ��̂��Ƃ�����,
��Ƃ̃��[���ł͕����̃��[���T�[�o�[���g�������Ă��邱�Ƃ�����,
`���ۂ̑��M��` �̕�����S�Ă��L�[�ɂ���Ǝ�M�̂��тɈقȂ錋�ʂ𓱂��o�����˂Ȃ�����ł�.

��, ���� `From:` �� `�����h���C��` ��
[`AddSender`](#addsender) ���Ă�ł��̌�̎d�����ς˂�,
�u��荞�񂾁v���Ӗ����� `true` ��Ԃ��ďI���܂�.

�Ȃ�, `From:` �� `���ۂ̑��M��` �̂����ꂩ���󕶎���ł������ꍇ��,
�u��荞��ł��Ȃ��v���Ӗ����� `false` ��Ԃ��ďI���܂�.


## `AddSender`

���[���̑��M�҂� List �ɉ����܂�.

�^����ꂽ `From:` �� `�����h���C��` �̃y�A�� List Control �ɉ����܂�.

�܂�, List Control �ɃG���g���[�ς݂̑S�Ă̍s����������,
���łɓ����y�A���G���g���[�ς݂������牽�������ɋA��܂�.

�V�K�G���g���[�Ȃ�, �J���� 0 �� `From:` ��, �J���� 1 �� `�����h���C��` ���Z�b�g�������,
�V�K�ǉ������s��I����Ԃɂ��܂�.

������ List Control �̃J�������𒲐���,
[`SortSender`](#sortsender) ���Ă�� List �̍s���т��\�[�g���܂�.

���Ƃ�, �I�����ꂽ�s���ڗ��悤�� List Control �Ƀt�H�[�J�X������,
�I�����ꂽ�s������̂� `Delete` �{�^���� enable ���Ċ����ł�.


## `SortSender`

List Control �̕\�������ܑI�΂�Ă�������Ń\�[�g���܂�.

�܂��� List Control ���猻���ǂݎ��܂�.
���́u����v�Ƃ�

* ���\�[�g�̃L�[�ƂȂ��Ă���J���� ( �̃C���f�b�N�X )
* �\�[�g�͏������~����

�� 2�̏��ł�.

��, ����� ( �\�[�g����O�� ) �e�s�ɑ΂���

* `�s�̃f�[�^` �C�R�[�� `�s�̔ԍ� ( �Ƃ������C���f�b�N�X )`

�ƂȂ�悤
[`SetItemData`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class#setitemdata)
���Ă����܂�.
����̓R�[���o�b�N�֐��ł��� [`CompareSender`](#comparesender) ��,
�󂯎������������ List Control ��̃A�C�e�����Q�Ƃł���悤�ɂ��邽�߂̎��O�����ł�.

������,
[`CompareSender`](#comparesender) ���r�֐��Ɏw�肵��
[`CListCtrl::SortItems`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class#sortitems)
����, ���ۂ̃\�[�g��Ƃ��s���܂�.

�Ō��, �\�[�g��̏�Ԃ�

* `�s�̃f�[�^` �C�R�[�� `�s�̔ԍ� ( �Ƃ������C���f�b�N�X )`

�ƂȂ�悤���߂�
[`SetItemData`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class#setitemdata)
���Ă����܂�.


## `CompareSender`

List Control �ɂ��鍀�ړ��m���r���܂�.

�^����ꂽ��̍��ڂ̂���, �O�҂���Ɉʒu������̂������� `+1` ��, �O�҂��O�Ɉʒu������̂������� `-1` ��Ԃ��܂�.
�S�������ʒu�������� `0` ��Ԃ��܂�.

[`CListCtrl::SortItems`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class#sortitems)
���o�R���ĊԐړI�ɌĂяo����Ă��܂�.

�ǂ̃J�����̍��ړ��m���r����̂���,
�����Ƃ��ė^����ꂽ�\���̂�������o���܂���,
�ǂ̃J�����̕���������[���A�h���X�̂悤�ȃh���C���\�L
( �Ōオ[�g�b�v���x���h���C��](https://ja.wikipedia.org/wiki/�g�b�v���x���h���C��)�ŏI��� )
�̂�,
�\�L�̌�� ( �E ) ����O��֌W���r���Ă����܂�.
<br>
<sub>
( [`CDomainPage::CompareDomain`](CDomainPage.md#comparedomain) �Ɠ����悤�ɔ�r���܂�.
�Ƃ����� [`CDomainPage::CompareDomain`](CDomainPage.md#comparedomain) �̃\�[�X�� copy & paste ����Ă��܂�.
�������W�b�N�������̉ӏ��ɐA����Ă���̂��C�ɓ���܂���,
�Ăяo�����񐔂̑����֐��Ȃ̂�, ���܂�Â������Ƃ͂���, �����𒼐ړ\��t���܂���. )
</sub>

Class �̃����o�[�̂悤�ȃt�������Ă��܂���, �R�[���o�b�N�֐��Ȃ̂� class �����o�[�ɂ̓A�N�Z�X�ł��܂���.
�Ȃ̂ŕK�v�ȏ��͑S�Ĉ����Ŏ󂯎���Ă��܂�.

���̊֐���,
�{�A�v���́u�O��\�[�g�p��r�֐��v(
[`CDomainPage::CompareDomain`](CDomainPage.md#comparedomain),
[`CZonePage::CompareZone`](CZonePage.md#comparezone),
`CWhitePage::CompareSender` )
�� 1�ł�.