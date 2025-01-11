# `CSetupDlg`

## �\��

�\�[�X�R�[�h���,
[`CSetupDlg.h`](../ChkMails/ChkMails/SetupDlg.h) ��
[`CSetupDlg.cpp`](../ChkMails/ChkMails/SetupDlg.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CSetupDlg`](#csetupdlg-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`DestroyWindow`](#destroywindow)
[`OnOK`](#onok)

#### �W�� Window Message �n���h���[

[`OnNcActivate`](#onncactivate)
[`OnGetMinMaxInfo`](#ongetminmaxinfo)

#### �R�}���h�n���h���[

[`OnHelp`](#onhelp)
[`OnButtonRefTone`](#onbuttonreftone)
[`OnButtonRefLog`](#onbuttonreflog)
[`OnChangeEdit`](#onchangeedit)

#### �ŗL�֐�

[`IsInStartup`](#isinstartup)
[`PutInStartup`](#putinstartup)
[`GetLinkPath`](#getlinkpath)
[`CreateLink`](#createlink)
[`IsExisting`](#isexisting)


## �T�v

���̑��̐ݒ�̂��߂̉��L�̂悤�ȃ_�C�A���O�𓮂����Ă��� class �ł�.

![](../Examples/Setup.png)

�ǂ��Ƃ������Ƃ͂Ȃ�����ӂꂽ�Ɨ��_�C�A���O�ł���,
[`CListCtrl`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class)
��
[`CEdit`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cedit-class)
�̃t�H���g��,
Arial �̂悤�ȃv���|�[�V���i���ȃt�H���g���Ɖ����ƊԈႢ�̂��� ( "rn" �� "m" ���������Â炢�Ƃ� ) �Ȃ̂�,
[Main Window](CMainWnd.md) ���w��̓����t�H���g ( [`GetCommonFont()`](CChkMailsApp.md) ) �ɓ���ւ��Ă��܂�.

����, �X�N���[���̒����ł͂Ȃ�, �^�X�N�o�[ ( ��̃A�C�R�� ) �Ɋ�����[�����ɏo�Ă���̂�,
[Main Window](CMainWnd.md) �̍����� ( [`NotifyPopup()`](CChkMailsApp.md#notifypopup) ) �ł�.

���̃_�C�A���O�� resizable ( �}�E�X�Ń_�C�A���O�̋���������������ƃT�C�Y���ς����� ) �Ȃ̂�,
�t�@�C������t�H���_�[���������Ď��܂�Ȃ��ꍇ�͓K���Ɉ������΂��Ă��g����������.

�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.

## `CSetupDlg`

���� class �� constructor �ł�.

���\�[�X ID ���w�肵�Ċ��N���X�� constructor ���ĂԂƂ����厖�Ȏd�����ʂ�������,
�����o�[�ϐ������������Ă��܂�.
<sub>
�������Ă����Ȃ���[C26495](https://learn.microsoft.com/ja-jp/cpp/code-quality/c26495)�����邳���̂�.
</sub>


## `OnInitDialog`

[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
�� override �ł�.

�܂��͕��ʂ�
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
���Ă񂾌�, �ȉ��̓Ǝ����������݂܂�.

1. �_�C�A���O�̃A�C�R�����Z�b�g����.
1. �_�C�A���O�̍ŏ��̃T�C�Y�������Ă���.
1. [`GetCommonFont`](#CChkMailsApp.md#getcommonfont) �ŋ��ʂ̓����t�H���g�𓾂�.
1. Edit Box �ɓ����t�H���g���Z�b�g����.
1. �_�C�A���O��̃A�C�e���ɐݒ�l�𔽉f������.
1. [`NotifyPopup`](CChkMailsApp.md#notifypopup) ���Ă�Ń_�C�A���O������ꂽ���Ƃ�ʒm.
1. `OK` �{�^���Ƀt�H�[�J�X���ڂ�.
1. �t�H�[�J�X��ݒ肵���̂�, `FALSE` ��Ԃ�.

���̂���, 5. �Ŕ��f�����Ă���ݒ�l��, ���O��
( ���� class �� [`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) �ɓ���O�� )
[Main Window](CMainWnd.md) ����ݒ肳�ꂽ���̂ł�.
�ݒ�l�͂��� class �� ( ���̉B�����ی���Ȃ� ) `public` �ϐ��Ƃ��Đ錾����Ă���̂�,
[Main Window](CMainWnd.md) ���珑������ł�.

������, 1�����u�ォ��̎w���v�ł͂Ȃ����� class �ŏ���Ɍ��߂Ă���̂�,
`Launch at startup` �� check box �ł�.
���̐ݒ�l������ Windows&reg; �� Startup �ɓo�^����Ă��邩�� class ��
[`IsInStartup`](#isinstartup) �œƎ��ɓǂݎ��,
���̌��ʂ� ON/OFF ���Ă��܂�.


## `DestroyWindow`

`CDialog::DestroyWindow` �� override �ł�.

[`NotifyPopup`](CChkMailsApp.md#notifypopup) ���Ă�Ń_�C�A���O���������񂾂��Ƃ�ʒm����,
�Ƃ����菇�����ނ��߂����� override ���Ă��܂�.


## `OnOK`

`CDialog::OnOK` �� override �ł�.

�Ȃ�₩��� �_�C�A���O ( ��̊e control ) �ɓ��͂��ꂽ�ݒ�������W�߂�,
���� class �� ( ���̉B�����ی���Ȃ� ) `public` �ϐ��Ɋi�[���܂�.
���Ƃ͂��� class ��
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) ������
[Main Window](CMainWnd.md) ��, ����Ɏ����čs���ł��傤.
<br>
<sub>
�����z���グ��ꂽ����, �{ class �� [Main Window](CMainWnd.md) �Ɋ��Ă��܂�.
( ���������������������, �Ȃ� Main Window ���q�h���l�݂����ł���. )
</sub>

������, 1�����u��ւ̕񍐁v�Ȃ��ɂ��� class �ŏ���ɂ���Ă���̂�,
`Launch at startup` �� check box �ł�.
���̐ݒ�l������ Windows&reg; �� Startup �̓o�^�^������ class ��
[`PutInStartup`](#putinstartup) �œƎ��ɂ���Ă��܂�.


## `OnNcActivate`

Window Message [`WM_NCACTIVATE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-ncactivate)
�ɑ΂���n���h���[�ł�.

[Main Window](CMainWnd.md) �̂悤�� modeless �� top level window �̎P���� dailog �ł��铖 class �ł�,
`>>` �{�^���������ĕ\�������鑷�����_�C�A���O��
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
���ł�, �� window ������ł��Ă��܂��܂�.

�Ȃ̂�, `>>` �{�^���� handler �łǂ������^�C�g���̃_�C�A���O�� `DoModal` ���������������Ă���,
�e window �̑���̋N�_�ƂȂ� `WM_NCACTIVATE` ���L���b�`����,

* �������_�C�A���O�� `DoModal` ���Ȃ�, ���_�C�A���O�ւ̑���͎Ւf��, �������_�C�A���O�ɒ��ӊ��N.

�Ƃ����i��������ł���܂�.
[Main Window](CMainWnd.md) �� [`OnIconNotify`](CMainWnd.md#oniconnotify)
������Ă���̂Ɠ��l�ȁumodal���v�ƌ����܂�.

�Ȃ�, �Ȃ��������_�C�A���O�ւ̃|�C���^�[��n���h���ł͂Ȃ�, �^�C�g�� ( caption ) �ŉ����Ă����̂��Ƃ�����,
�������_�C�A���O�� `DoModal` ����܂ł̓|�C���^�[�̓A�e�ɂȂ炸, �n���h���͑��݂��Ȃ�����ł�.
`DoModal` �ɓ����Ă��܂���, �����O����͉���ł��܂���.
�^�C�g���ŉ����Ă�����, `WM_NCACTIVATE` ���������ɂ�
�����Ă������^�C�g��
( �Ƃ��߂Ă���[���X�N](https://www.codeproject.com/Articles/538/Avoiding-Multiple-Instances-of-an-Application)����� �_�C�A���O�� window class �ł���u#32770�v���t���� ) ��
[`FindWindow`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-findwindoww)
���Č����� window �ɒ��ӂ𑣂��Ă��܂�. 
<br>
<sub>
����, ���Ȃ��Ɂu�o�Ă����_�C�A���O�𑀍삷��v�Ƃ��Ă���������΂���Ȃ��Ƃ��Ȃ��Ă��ςނ�ł���,
�o�Ă����_�C�A���O�ȊO�Ɂu���C�v�����, �u����������Ȃ��B���������B�v�ƒ��ӂ����킯�ł�.
</sub>

## `OnGetMinMaxInfo`

Window Message [`WM_GETMINMAXINFO`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-getminmaxinfo)
�ɑ΂���n���h���[�ł�.

[`OnInitDialog`](oninitdialog) �ŉ����Ă������_�C�A���O�̍ŏ��̃T�C�Y��,
�u�ŏ��T�C�Y�v�Ƃ��ĕԂ��܂�.
����ɂ��, �_�C�A���O�̒[�� drag ���Ă����̃T�C�Y��菬�����͂ł��Ȃ��Ȃ�܂�.
���������Ă��Ӗ��͂Ȃ��̂œ���Ă����܂���.
�傫�����镪�ɂ͈Ӗ�������̂ŕs����ł�.


## `OnHelp`

`Help` �{�^���������ꂽ���ɔ��ł��� `IDHELP` �R�}���h�̃n���h���[�ł�.

[`ShowHelp`](CChkMailsApp.md#showhelp) �Ɏd�����ۓ����ł�.


## `OnButtonRefTone`

`Ring` �̉E�� `>>` �{�^���������ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

���݂̐ݒ�Ɋ�Â��Ă��̐e�t�H���_�[���J�����Ƃ��납��n�߂܂�.

�Ȃɂ������� `.wav` �t�@�C�����I�΂�, `OK` �{�^������������,
���� `.wav` �t�@�C���̉���炷�Ƃ����f�����X�g���[�V���������݂܂���,
�{�i�I�ɐݒ�l�ɔ��f�����̂�, �{�_�C�A���O�� `OK` �{�^���������ꂽ�Ƃ��ł�.
<br>
<sub>
����, �e�t�H���_�[���J�����ۂɃt�@�C���̕\�����݂��Ƃ��Ȃ� ( ���r���[�ȕ������I������Ă��Ȃ� ) �̂�,
[�O�q](../README.md#notification)�̒ʂ�,
[Windows&reg; �̃|�J](https://stackoverflow.com/questions/64424601/cmfceditbrowsectrl-is-cropping-the-file-name)
�ł�.
</sub>

�����ŊJ���Ă���
[`CFileDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cfiledialog-class) ��
�u������ dialog�v�ɑ�������̂�, ����
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
���Ɂu���C�v����Ȃ��悤
[`OnNcActivate`](#onncactivate)
�Ō������Ă��܂�.


## `OnButtonRefLog`

`Folder:` �̉E�� `>>` �{�^���������ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

���݂̐ݒ�t�H���_�[���J�����Ƃ��납��n�߂܂�.

�Ȃɂ������̃t�H���_�[���I�΂�, `Select Folder` �{�^������������,
�b��l�Ƃ��Ď�荞�܂�܂���,
�{�i�I�ɐݒ�l�ɔ��f�����̂�, �{�_�C�A���O�� `OK` �{�^���������ꂽ�Ƃ��ł�.
<br>
<sub>
[`CFolderPickerDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cfolderpickerdialog-class)
�Ƃ����t�H���_�[���̃_�C�A���O���g���Ă���̂�,
[`OnButtonRefTone`](#onbuttonreftone) �̂悤��
[Windows&reg; �̃|�J](https://stackoverflow.com/questions/64424601/cmfceditbrowsectrl-is-cropping-the-file-name)
���I�����܂���.
</sub>

�����ŊJ���Ă���
[`CFolderPickerDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cfolderpickerdialog-class) ��
�u������ dialog�v�ɑ�������̂�, ����
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
���Ɂu���C�v����Ȃ��悤
[`OnNcActivate`](#onncactivate)
�Ō������Ă��܂�.


## `OnChangeEdit`

Edit Box �̒��g��ҏW����Ɣ��ł��� `EN_CHANGE` �R�}���h�̃n���h���[�ł�.

�S�Ă� Edit Box �ɉ��炩�̕����񂪓��͂��ꂽ���_��,
�܂����ۂ̗����Ȃ��Ȃ������_��,
`OK` �{�^�����C�l�[�u�����Ă��܂�.


## `IsInStartup`

�{�A�v���� Windows&reg; �� Startup �ɓo�^����Ă��邩�ۂ���Ԃ��܂�.

1. [`GetLinkPath`](#getlinkpash) �� Startup �̃t�H���_�[�𓾂�.
1. [`IsExisting`](#isexisting) �ŏ�L�t�H���_�[�����݂��邩�𓾂�.

�Ƃ��ăt�H���_�[�̗L����Ԃ������ł�.


## `PutInStartup`

�{�A�v���� Windows&reg; �� Startup �œo�^�^�o�^�������܂�.

�����菇�͉��L�̒ʂ�:


1. [`GetLinkPath`](#getlinkpash) �� Startup �̃t�H���_�[�𓾂�.
1. [`IsExisting`](#isexisting) �ŏ�L�t�H���_�[�����݂��邩�𓾂�.
1. �u�o�^�˗��v�Łu�o�^�ς݁v�Ȃ�I��.
1. �u�o�^�����˗��v�Łu���o�^�v�Ȃ�I��.
1. �u�o�^�˗��v�Ȃ�, �{�A�v���� `.exe` �ւ̃p�X�� Startup �̃t�H���_�[�� [`CreateLink`](#createlink).
1. �u�o�^�����˗��v�Ȃ�,  Startup �̃t�H���_�[����{�A�v���ւ̃����N�� [`DeleteFile`](https://learn.microsoft.com/ja-jp/windows/win32/api/winbase/nf-winbase-deletefile).
<br>
<sub>
�Ƃ����,
��L 5. �ł���Ă���u�{�A�v���� `.exe` �ւ̃p�X�v�̋��ߕ����ӊO�Ɋ�����������������邩������܂���.
�u���A���������̂�
[`GetModuleFileName`](https://learn.microsoft.com/ja-jp/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew)
�ł����񂶂�Ȃ���?�v��.
�ł�, ����ŏ\���Ȃ�ł�. [���ۂɊm���߂�������������Ⴂ�܂�](https://hirokio.jp/visualcpp/getmodulefilename/)��.
</sub>


## `GetLinkPath`

�^����ꂽ GUID �ɂ�����A�v���̃����N�̃p�X��Ԃ��܂�.

�����ň����Ƃ��ė^������ [GUID](https://ja.wikipedia.org/wiki/GUID) ��,
Windows&reg; �́u�W���t�H���_�[�v�����ʂ���
[`KNOWNFOLDERID`](https://learn.microsoft.com/ja-jp/windows/win32/shell/knownfolderid)
�Ƃ������̂�, GUID �������Ƃ��ēn���ƊY������t�H���_�[�̕����p�X���Ԃ��Ă��܂�.

GUID �� `FOLDERID_Programs` ( `Start Menu` �� `Programs` �ɑ��� ) �̏ꍇ��,
����ɃT�u�t�H���_�[��t���w��ł��܂�.

�����菇�Ƃ��Ă�:

1. [`SHGetKnownFolderPath`](https://learn.microsoft.com/ja-jp/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath) �� GUID ����t�H���_�[�p�X�𓾂�.
1. `FOLDERID_Programs` �ɑ΂��ăT�u�t�H���_�[�̎w�肪�����, ��L�œ����p�X�ɂ����ǉ�.
1. �A�v������ `".lnk"` �Ɗg���q���������t�@�C�����𐶐�.
1. �p�X�{�t�@�C������Ԃ�.

�ƂȂ��Ă��܂�.
�܂�, 

* GUID �Ŏw�肳�ꂽ�t�H���_�[�ɃA�v���ւ̃����N�����Ƃ�����, ���̃����N�͂��������t���p�X�ɂȂ�

�Ƃ��������񂪕Ԃ�܂�.


## `CreateLink`

�{�A�v���� `.exe` �ւ̃����N���쐬���܂�.

�����菇�͉��L�̒ʂ�:

1. [`GetVersionInfo`](CChkMailsApp.md#getversioninfo)
�Ŏw�� `.exe` �̃o�[�W��������́u���� ( File description )�v�𓾂�.
1. ���ꂩ��
[COM](https://learn.microsoft.com/ja-jp/windows/win32/com/component-object-model--com--portal)
���g���\��Ȃ̂�, ��V�Ƃ��� ( thread ���̂ǂ����ōς܂��Ă���Ώȗ�����, �ꉞ�O�̂��� )
[`CoInitialize`](https://learn.microsoft.com/ja-jp/windows/win32/api/objbase/nf-objbase-coinitialize) ���Ă���.
1. [`IShellLink`](https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nn-shobjidl_core-ishelllinkw)
�C���^�[�t�F�C�X�� 1��
[`CoCreateInstance`](https://learn.microsoft.com/ja-jp/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstance).
1. �w�� `.exe` �ɑ΂���
[`SHGetFileInfo`](https://learn.microsoft.com/ja-jp/windows/win32/api/shellapi/nf-shellapi-shgetfileinfow)
���� `.exe` �̃A�C�R���̏��𓾂�.
1. `IShellLink` �C���^�[�t�F�C�X����� `.exe` �̃A�C�R���̏���
[`SetIconLocation`](https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nf-shobjidl_core-ishelllinkw-seticonlocation).
1. `IShellLink` �C���^�[�t�F�C�X����� `.exe` �́u���� ( File description )�v��
[`SetDescription`](https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nf-shobjidl_core-ishelllinkw-setdescription).
1. `IShellLink` �C���^�[�t�F�C�X����� `.exe` �̃t���p�X��
[`SetPath`](https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nf-shobjidl_core-ishelllinkw-setpath).
1. `IShellLink` �C���^�[�t�F�C�X����� `.exe` �̒u���ꂽ�p�X���u��ƃf�B���N�g�� ( working directory )�v �Ƃ���
[`SetWorkingDirectory`](https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nf-shobjidl_core-ishelllinkw-setworkingdirectory).
1. `IShellLink` �C���^�[�t�F�C�X����� [`IPersistFile`](https://learn.microsoft.com/ja-jp/windows/win32/api/objidl/nn-objidl-ipersistfile) �C���^�[�t�F�C�X��
[`QueryInterface`](https://learn.microsoft.com/ja-jp/cpp/atl/queryinterface).
1. `IPersistFile` �C���^�[�t�F�C�X�����, �����N�̃t���p�X��
[`Save`](https://learn.microsoft.com/ja-jp/windows/win32/api/objidl/nf-objidl-ipersistfile-save).
1. `IPersistFile` �C���^�[�t�F�C�X��
[`Release`](https://learn.microsoft.com/ja-jp/windows/win32/api/unknwn/nf-unknwn-iunknown-release).
1. `IShellLink` �C���^�[�t�F�C�X��
[`Release`](https://learn.microsoft.com/ja-jp/windows/win32/api/unknwn/nf-unknwn-iunknown-release).
1. [COM](https://learn.microsoft.com/ja-jp/windows/win32/com/component-object-model--com--portal)
���g���I������̂�, ���[���Ƃ���
[`CoUninitialize`](https://learn.microsoft.com/ja-jp/windows/win32/api/combaseapi/nf-combaseapi-couninitialize).

[COM](https://learn.microsoft.com/ja-jp/windows/win32/com/the-component-object-model)
���ē���ł���ˁ[.
�����������̗ǂ������l������ł��傤�ˁ[.
<br>
<sub>
�Ō�̔������܎^�Ȃ̂�����Ȃ̂��Ɋւ��Ă�, �񓚂������T�������Ă��������܂�.
</sub>

## `IsExisting`

�^����ꂽ�t�@�C�����̃t�@�C�������݂��邩�ۂ���Ԃ��܂�.

�P�� [`CFile::GetStatus`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cfile-class?#getstatus)
�������������ۂ��Ńt�@�C���̗L���𓚂��邾���ł�.
