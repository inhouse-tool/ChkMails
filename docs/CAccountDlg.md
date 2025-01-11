# `CAccountDlg`

## �\��

�\�[�X�R�[�h���,
[`AccountDlg.h`](../ChkMails/ChkMails/AccountDlg.h) ��
[`AccountDlg.cpp`](../ChkMails/ChkMails/AccountDlg.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CAccountDlg`](#caccountdlg-1)

#### Public

[`SetAccounts`](#setaccounts)
[`GetAccounts`](#getaccounts)

#### Override

[`OnInitDialog`](#oninitdialog)
[`DestroyWindow`](#destroywindow)
[`OnOK`](#onok)

#### �W�� Window Message �n���h���[

[`OnGetMinMaxInfo`](#ongetminmaxinfo)
[`OnTimer`](#ontimer)

#### �R�}���h�n���h���[

[`OnHelp`](#onhelp)
[`OnCheckHide`](#oncheckhide)
[`OnButtonAdd`](#onbuttonadd)
[`OnButtonDelete`](#onbuttondelete)
[`OnChangeEdit`](#onchangeedit)
[`OnClickRadio`](#onclickradio)
[`OnChangeList`](#onchangelist)

#### �ŗL�֐�

[`OnSelectItem`](#onselectitem)
[`EnableAdd`](#enableadd)


## �T�v

���[���A�J�E���g�ݒ�̂��߂̉��L�̂悤�ȃ_�C�A���O�𓮂����Ă��� class �ł�.

![](../Examples/Accounts3.png)

�ǂ��Ƃ������Ƃ͂Ȃ�����ӂꂽ�Ɨ� dialog �ł���,
[`CListCtrl`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class)
��
[`CEdit`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cedit-class)
�̃t�H���g��,
Arial �̂悤�ȃv���|�[�V���i���ȃt�H���g���Ɖ����ƊԈႢ�̂��� ( "rn" �� "m" ���������Â炢�Ƃ� ) �Ȃ̂�,
[Main Window](CMainWnd.md) ���w��̓����t�H���g
( [`GetCommonFont()`](CChkMailsApp.md#getcommonfont) ) �ɓ���ւ��Ă��܂�.

����, �X�N���[���̒����ł͂Ȃ�, �^�X�N�o�[ ( ��̃A�C�R�� ) �Ɋ�����[�����ɏo�Ă���̂�,
[Main Window](CMainWnd.md) �̍����� ( [`NotifyPopup()`](CChkMailsApp.md#notifypopup) ) �ł�.

���̃_�C�A���O�� resizable ( �}�E�X�Ń_�C�A���O�̋���������������ƃT�C�Y���ς����� ) �Ȃ̂�,
���[�U�[����v���o�C�_�[���������Ď��܂�Ȃ��ꍇ�͓K���Ɉ������΂��Ă��g����������.

�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.


## `CAccountDlg`

���� class �� constructor �ł�.

�\�[�X�R�[�h�������, ��������Ă��Ȃ��悤�Ɍ����āu�v��́H����B�v�Ƃ��������ł���,
���\�[�X ID ���w�肵�Ċ��N���X�� constructor ���ĂԂƂ����厖�Ȏd�������Ă��܂�.


## `SetAccounts`

�A�J�E���g��ݒ肳��܂�.

[Main Window](CMainWnd.md) �������Ă���A�J�E���g�̔z����󂯎��܂�.
�_�C�A���O���\�������O�Ɏ󂯎��܂�.
�� class ��
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
������n�߂���ł͉��̏o��������ł��Ȃ��̂�, ���O�ɍς܂��Ă��������Ȃ��̂ł�.


## `GetAccounts`

�A�J�E���g���擾����܂�.

�� class ��
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
����I����,
�� class �������Ă���A�J�E���g�̔z��� [Main Window](CMainWnd.md) ���擾���Ă����܂�.
<br>
<sub>
�����z���グ��ꂽ����, �{ class �� [Main Window](CMainWnd.md) �Ɋ��Ă��܂�.
( ���������������������, �Ȃ� Main Window ���q�h���l�݂����ł���. )
</sub>


## `OnInitDialog`

[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
�� override �ł�.

�܂��͕��ʂ�
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
���Ă񂾌�, �ȉ��̓Ǝ����������݂܂�.

1. �_�C�A���O�̃A�C�R�����Z�b�g����.
1. �_�C�A���O�̍ŏ��̃T�C�Y�������Ă���.
1. [`GetCommonFont`](CChkMailsApp.md#getcommonfont) �ŋ��ʂ̓����t�H���g�𓾂�.
1. Edit Box �ɓ����t�H���g���Z�b�g����.
1. List Control ��, �S�J�����őI��\������悤�ɐݒ�.
1. List Control ��, �O���b�h��\������悤�ɐݒ�.
1. �J���� 0 �Ƀ_�~�[�������, �J���� 1 �ɉE�l�߂� `User`, �J���� 2 �ɍ��l�߂� `Host` ��ǉ�.
1. �J���� 0 �̃_�~�[���폜.
1. [`SetAccounts`](#setaccounts) �Őݒ肳��Ă����A�J�E���g�� List Control �ɒǉ�.
1. 1�ł��A�J�E���g���������� List Control �̃J����������������.
1. �_�C�A���O��̃A�C�e�����f�t�H���g�̏�Ԃɂ���.
1. [`NotifyPopup`](CChkMailsApp.md#notifypopup) ���Ă�Ń_�C�A���O������ꂽ���Ƃ�ʒm.
1. `OK` �{�^���Ƀt�H�[�J�X���ڂ�.
1. �t�H�[�J�X��ݒ肵���̂�, `FALSE` ��Ԃ�.

���̂���, 7. �� 8. �ɂ���u�_�~�[�v�͈ꌩ�s�v�����Ȓi���ł���,
[�u�J���� 0 �͍��l�߂ɂ����ł��Ȃ��v](https://learn.microsoft.com/ja-jp/windows/win32/api/commctrl/ns-commctrl-lvcolumnw#remarks)
�Ƃ��� List Control �̏�Ȃ������������邽�߂̂��̂ł�.
<br>
<sup>
����, �u�����v���������񂶂�Ȃ���, �u�C���v���Ă���񂩂�?
</sup>


## `DestroyWindow`

`CDialog::DestroyWindow` �� override �ł�.

[`NotifyPopup`](CChkMailsApp.md#notifypopup) ���Ă�Ń_�C�A���O���������񂾂��Ƃ�ʒm����,
�Ƃ����菇�����ނ��߂����� override ���Ă��܂�.


## `OnOK`

`CDialog::OnOK` �� override �ł�.

���͂��ꂽ�A�J�E���g�̒ǉ��� [`OnButtonAdd`](#onbuttonadd) �ōς܂��Ă���̂�,
���� `OnOK` �ł��邱�Ƃ��Ȃ��̂ł���,

* ���������A�J�E���g������͂����̂� `Add` �{�^���������̂�Y��� `OK` �{�^����������

�Ƃ����������肳�񂪋����̂�, �u`Add` �������Y��� `OK` �������ꂽ�v�Ƃ����C�z���@�m������,
����� `Add` �������Ă����Ƃ����S�����������Ă���܂�.

����,

* �������� `Server`, `User`, `Password` �܂œ��ꂽ�̂� `POP3` �� `POP3S` ��I�Ԃ̂�Y��� `OK` �{�^����������

�Ƃ����������肳��������̂�, �u�ǂ������I�ׂ�v�ƃc�b�R��� `CDialog::OnOK` �͂��Ȃ��Ƃ����S�����������Ă���܂�.


## `OnGetMinMaxInfo`

Window Message [`WM_GETMINMAXINFO`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-getminmaxinfo)
�ɑ΂���n���h���[�ł�.

[`OnInitDialog`](#oninitdialog) �ŉ����Ă������_�C�A���O�̍ŏ��̃T�C�Y��,
�u�ŏ��T�C�Y�v�Ƃ��ĕԂ��܂�.
����ɂ��, �_�C�A���O�̒[�� drag ���Ă����̃T�C�Y��菬�����͂ł��Ȃ��Ȃ�܂�.
���������Ă��Ӗ��͂Ȃ��̂œ���Ă����܂���.
�傫�����镪�ɂ͈Ӗ�������̂ŕs����ł�.

## `OnTimer`

Window Message [`WM_TIMER`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-timer)
�ɑ΂���n���h���[�ł�.

���L�̃^�C�}�[�l����舵���Ă��܂�.

| �^�C�}�[�l | �� | �������e |
| --- | --- | --- |
| `TID_SELCHANGE` | �u�I��ΏەύX�v | [`OnSelectItem`](#onselectitem) �Ăяo��. |


## `OnHelp`

`Help` �{�^���������ꂽ���ɔ��ł��� `IDHELP` �R�}���h�̃n���h���[�ł�.

[`ShowHelp`](CChkMailsApp.md#showhelp) �Ɏd�����ۓ����ł�.


## `OnCheckHide`

`Hide` Check Box �� click ���ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

Check Box ��&#x2611;���ꂽ���ۂ��ɂ����,
`Password` �� Edit Box �̕����^�ʏ�\����؂�ւ��܂�.


## `OnButtonAdd`

`Add` �{�^���������ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

`Server`, `User` �ɓ��͂��ꂽ���ꂼ��̕������,
List Control �ɒǉ����܂�.

������, ���ł� List Control ���ɑS�������g�ݍ��킹���G���g���[�ς݂̏ꍇ��,
�G���g���[�ς݂̕�����U�폜���Ă��� List �̍Ō���ɒǉ����܂�.
( ���̓������g����, �G���g���[�ς݂̍��ڂ��蓮�ŕ��בւ��ł��܂�.
���������ւ����i�̎������T�{�����̂�, �ǁ[���Ă�����ւ������ꍇ��, ���̃p�Y���݂����ȕ��@�ł��肢���܂�. )


## `OnButtonDelete`

`Delete` �{�^���������ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

�� List Control ��őI������Ă���s�̃G���g���[���폜���܂�.

## `OnChangeEdit`

`Server`, `User`, `Password` �̂����ꂩ�ŕ����񂪕ύX���ꂽ���ɔ��ł��� `EN_CHANGE` �R�}���h�̃n���h���[�ł�.

[`EnableAdd`](#enableadd) ���ĂԂ����ł�.


## `OnClickRadio`

`POP3`, `POP3S` �̂����ꂩ���N���b�N���ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

[`EnableAdd`](#enableadd) ���ĂԂ����ł�.


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


## `OnSelectItem`

List Control ��̑I�����ς���ꂽ���ƂɑΉ����܂�.

�����ꂩ�̗L���ȍs���I�����ꂽ�ꍇ�F

* `Server`, `User`, `Password` �̊e Edit Box �ɑI�����ꂽ�A�J�E���g�̓��e���l�߂�.
* `POP3` �� `POP3S` ���̑I����I�����ꂽ�A�J�E���g�̑I��ʂ�ɉ��߂�.
* `Delete` �{�^����L���ɂ���.

�����������ꍇ:

* `Server`, `User`, `Password` �̊e Edit Box ����ɂ���.
* `POP3` �� `POP3S` ���̑I���𗼕������ɂ���.
* `Delete` �{�^���𖳌��ɂ���.


## `EnableAdd`

`Add` �{�^����L���܂��͖����ɂ��܂�.

`Add` �{�^����L���ɂ�������������Ă���ꍇ,
�܂� `Server`, `User`, `Password` �̂�����������񂪓��͂���Ă���,
�Ȃ����� `POP3` �� `POP3S` �̑I�����ς�ł���ꍇ,
`Add` �{�^����L���ɂ��܂�.

�����������Ă��Ȃ��ꍇ�� `Add` �{�^���𖳌��ɂ��܂�.

[`OnChangeEdit`](#onchangeedit) �� [`OnClickRadio`](#onclickradio) ����Ă΂�Ă��܂�.