# `CChkMailsApp`

## �\��

�\�[�X�R�[�h���,
[`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) ��
[`ChkMailsApp.cpp`](../ChkMails/ChkMails/ChkMails.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Macros

[`NotifyPopup`](#notifypopup)
[`GetCommonFont`](#getcommonfont)
[`ShowHelp`](#showhelp)

#### Public

[`DeleteProfileValue`](#deleteprofilevalue)
[`GetVersionInfo`](#getversioninfo)

#### Override

[`InitInstance`](#initinstance)
[`PreTranslateMessage`](#pretranslatemessage)
[`DoMessageBox`](#domessagebox)

## �T�v

[MFC](https://en.wikipedia.org/wiki/Microsoft_Foundation_Class_Library)
�x�[�X�Ȃ̂�,
�v���O�����̎n�܂��
[`CWinApp`](https://learn.microsoft.com/en-us/cpp/mfc/reference/cwinapp-class)
�h�� class ����ł�.

���� `CChkMailsApp` class �̓��F�Ƃ��Ă�.

1. MFC �I�X�X���� *Document & View Architecture* �Ƃ��_�C�A���O�x�[�X�Ȃǂ̍\���ł͂Ȃ�,
�Ǝ��� [`CWnd` �h�� class](CMainWnd.md) �������,
��������C���E�C���h�E�Ƃ��Ă���.
1. [`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox) �̍s��������ł���
[`DoMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#domessagebox)
 �ɉ����Ǝ����������Ă����Ă���.

�Ƃ����Ƃ���ł��傤��.

1. ��, �E�C���h�E�Ƃ��Č�����K�v���Ȃ��풓�A�v���Ȃ̂�, 
[`CView`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cview-class)
��
[`CDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class)
���v��Ȃ�����,<br>
2. ��, �A�v��������Ă΂ꂽ
[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
�̕\���ʒu���^�X�N�o�[�Ɋ񂹂�������,<br>

�Ƃ������Ƃǂ��ł��������@�ōs���Ă���J�X�^�}�C�Y�ł�.

����, ���F�Ƃ��Ă�, �����A�v����2��ȏ�N�������,
��y����y�������ނ��ėB��̃v���Z�X�̍��ɒ����Ƃ����Ƃ���ł�����.
�����N�����Ă��Ӗ��̂Ȃ��A�v���Ȃ̂�.

[`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) ��,
class ���̒�`�݂̂Ȃ炸, �{�A�v�����\������S�Ẵ\�[�X�ɋ��ʂ̒�`��񋟂����ڂ𕉂��Ă��܂�.
<p>
�ȉ��̓Ǝ� Window Message ����`����Ă��܂�.

| Message | �p�r |
| --- | --- |
| `WM_USER_TRAY_NOTIFY`	| �^�X�N�o�[�̒ʒm�̈�ɏ풓�������A�C�R������̒ʒm<br>[`CMainWnd`](CMainWnd.md) �̒��ł����g���Ă��Ȃ� Window Message �Ȃ̂� [`MainWnd.h`](../ChkMails/ChkMails/MainWnd.h) �Œ�`����Ηǂ������Ȃ��̂���, `WM_APP`+x ���ꂩ���ɕ��ׂ����Ƃ������R�����ł����Œ�`. |
| `WM_POPUP_NOTIFY`	| �e�_�C�A���O���\�������Ƃ��ɂ��ꂼ�ꂩ�瑗���Ă��� Window Message.<br>[`CMainWnd`](CMainWnd.md) �͂�����󂯂�, ���茳�� window �̕\���ʒu���A�C�R���Ɋ񂹂�. |
| `WM_GET_COMMONFONT`	| ���ʂ̓����t�H���g�𓾂邽�߂Ɋe�_�C�A���O���瑗���Ă��� Window Message.<br>[`CMainWnd`](CMainWnd.md) �͂�����󂯂�, ���炩���ߍ���Ă����������t�H���g��Ԃ�. |
| `WM_GET_SENDER`	| ���[���̑��M�҂𓾂邽�߂�[`CWhitePage`](#cwhitepage)���瑗���Ă��� Window Message.<br>[`CMainWnd`](CMainWnd.md) �͂�����󂯂�, ���炩���ߎ擾���Ă�������������������. |
| `WM_SHOW_HELP`	| Help ��\�����邽�߂ɔz���̃_�C�A���O���瑗���Ă��� Window Message.<br>[`CMainWnd`](CMainWnd.md) �͂�����󂯂�, ���M�� window �ɑΉ����� help �\�����s��. |

����, ��L�S�Ă� Window Message �� [`CMainWnd`](CMainWnd.md) ����݂Ȃ̂�,
����ς� [`MainWnd.h`](../ChkMails/ChkMails/MainWnd.h) �Œ�`����Ηǂ������Ȃ���ł���,
[`MainWnd.h`](../ChkMails/ChkMails/MainWnd.h) ���Ə��̊J���ʂ��������ĉ������L���Ă���̂��t�H�[�J�X���ڂ₯��̂�,
������ [`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) �Œ�`���āu���ꂾ���݂͂�ȋ��ʂ̒�`�v�Ƃ����������o���Ă��܂�.

�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.


## `NotifyPopup`

Dialog ���|�b�v�A�b�v�������Ƃ�m�点��}�N��.

���̂��������֐��ł͂Ȃ�, [`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) �ɒ�`���ꂽ�}�N���ł�.
( �Ȃ̂Ŏ��ۂɂ� class �Ƃ����֌W�ł���. )

���[�U�[��`�� Window Message �ł��� `WM_POPUP_NOTIFY` ��
[`CMainWnd`](CMainWnd.md) ��
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew)
���܂�.
`lParam` �ɂ�, �}�N���̈����Ƃ��ēn���ꂽ�Ăяo�����_�C�A���O�� window handle ���ڂ��Ă��܂�.

���� Window Message ���󂯂�[`CMainWnd`](CMainWnd.md)��[`OnPopupNotify`](CMainWnd.md#onpopupnotify)��,
`lParam` �ɍڂ���ꂽ window handle ���g����,
�_�C�A���O�̕\���ʒu���A�C�R���̖T�Ɋ񂹂鏈����ǉ����܂�.

����, ���̃}�N���́u�����ꂩ�̃_�C�A���O��
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
���ł��邱�Ɓv��
[Main Window](CMainWnd.md) �ɒm�点���ڂ������Ă��܂�.
�Ȃ̂�, �e�_�C�A���O�� `DoModal` ���甲���Ă�����
`lParam` ����ɂ��Ă�����x���̃}�N�����Ăԃ��[���ɂ��Ă��܂�.


## `GetCommonFont`

[`CMainWnd`](CMainWnd.md) ����u���ʂ̓����t�H���g�v���󂯎��}�N��.

���̂��������֐��ł͂Ȃ�, [`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) �ɒ�`���ꂽ�}�N���ł�.
( �Ȃ̂Ŏ��ۂɂ� class �Ƃ����֌W�ł���. )

���[�U�[��`�� Window Message �ł��� `WM_GET_COMMONFONT` ��
[`CMainWnd`](CMainWnd.md) ��
[send](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
���܂�.

���� Window Message ���󂯂�[`CMainWnd`](CMainWnd.md)��[`OnGetCommonFont`](CMainWnd.md#ongetcommonfont)��,
���炩���ߍ���Ă��������ʂ̓����t�H���g��Ԃ��܂�.


## `ShowHelp`

Help ��\�����Ă��炤�}�N��.

���̂��������֐��ł͂Ȃ�, [`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) �ɒ�`���ꂽ�}�N���ł�.
( �Ȃ̂Ŏ��ۂɂ� class �Ƃ����֌W�ł���. )

���[�U�[��`�� Window Message �ł��� `WWM_SHOW_HELP` ��
[`CMainWnd`](CMainWnd.md) ��
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew)
���܂�.

���� Window Message ���󂯂�[`CMainWnd`](CMainWnd.md)��[`OnShowHelp`](CMainWnd.md#onshowhelp)��,
�ʒm���ꂽ window handle ���瑗�M�� window �̃^�C�g�� ( caption ) �𓾂�,
�����Y���� URL �� web �y�[�W���Q�Ƃ��܂�.


## `DeleteProfileValue`

�s�v�ɂȂ��� registry �l���폜���邽�߂� public �֐�.
<p>

�@�\�Ƃ��Ă͔ėp�ł���,
�{�A�v���ł͌��ʓI�� [`CMainWnd`](CMainWnd.md) ���炵���g���Ă��܂���.
�������� [`CMainWnd`](CMainWnd.md) �̒��Ɏ�������Ηǂ������Ȃ��̂ł���,
�u�g`Profile`�h �Ȋ֐��� `CWinApp` �Ɂv�Ƃ����u�l�����v�ɕt�������Ă�����ɔz�u����Ă��܂�.
���̊֐������̎����̐擪�ɔz�u����Ă���̂�,
�u`public` �͂��̂� `protected` �Ȃ��̂���Ɂv�Ƃ����u�l�����v�ɂ����̂ł�.
<p>

�폜�ɐ��������� `TRUE` ��, �^����ꂽ���O�� registry �l�����݂��Ȃ������� `FALSE` ��Ԃ��܂�.


## `GetVersionInfo`

�^����ꂽ��ʂ̏���^����ꂽ `.exe` �t�@�C���̃o�[�W�������̒���������o���ĕ�����ŕԂ��܂�.

�ȉ��̎�ʂ��w��\�ł�.

| ��ʎw�� | �����o������� | �� |
| --- | --- | --- |
| `%C` | `CompanyName` | `In-house Tool` |
| `%D` | `FileDescription` | `Mail Checker` |
| `%V` | `FileVersion` | `1.0.0.1` |
| `%L` | `LegalCopyright` | `Copyright (C) 2025 In-house Tool` |
| `%P` | `ProductName` | `ChkMails` |

�Ⴆ�Ή��L���̂悤�ȕ�����Ŏw�肷���, ���L�E�̂悤�ȕ����񂪕Ԃ�܂�.

| ���� | �߂�l |
| --- | --- |
| `%P\n%D\nVersion %V\n` | `ChkMails`<br>`Mail Checker`<br>`Version 1.0.0.1` |

����, `.exe` �t�@�C�����w�肷��������ȗ������,
�f�t�H���g�Ŗ{�A�v���� `.exe` �t�@�C����ΏۂƂ��܂�.
<br>
<sub>
�Ƃ����,
��L�ł���Ă���u�{�A�v���� `.exe` �ւ̃p�X�v�̋��ߕ����ӊO�Ɋ�����������������邩������܂���.
�u���A���������̂�
[`GetModuleFileName`](https://learn.microsoft.com/ja-jp/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew)
�ł����񂶂�Ȃ���?�v��.
�ł�, ����ŏ\���Ȃ�ł�. [���ۂɊm���߂�������������Ⴂ�܂�](https://hirokio.jp/visualcpp/getmodulefilename/)��.
</sub>

## `InitInstance`

��ʓI��
[`CWinApp::InitInstance`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#initinstance) �� override �ł�.

[�u�R�����ăz���g�ɕK�v�Ȃ́H�v](https://stackoverflow.com/questions/726841/is-it-necessary-to-use-initcommoncontrolsex-and-initcommoncontrols)
�ƈꕔ�ŋ^���Ă���
�u[`InitCommonControlsEx`](https://learn.microsoft.com/ja-jp/windows/win32/api/commctrl/nf-commctrl-initcommoncontrolsex) ���Ăяo���v
�Ƃ������񑩂��ʂ����Ă���
( [`COMCTL32.DLL` �̍���̌Ăяo���̂��߂ɂ���Ă����ׂ�](https://devblogs.microsoft.com/oldnewthing/20050718-16/?p=34913)�������Ȃ̂� )
, registry �����`���Ă��܂�.

���ꂩ��,
<sub>
( project ��V�K�쐬����ۂ� Windows sockets �Ƀ`�F�b�N�����Y����
���Ƃ���̒ǉ����ʓ|�Ȃ��ƂŒm���Ă��鏉�����̂ЂƂ� )
</sub>
`AfxSocketInit()` �̌Ăяo�����s���Ă��܂�.
<sub>
( �����ЂƂ̒ǉ���Ƃ�
[`framework.h`](../ChkMails/ChkMails/framework.h)
�Ɂ@`#include <afxsock.h>` �Ə��������邱�� )
</sub>

�ȏオ���肪���ȏ������e�ł���, ��������悪���j�[�N�ł�.

�܂�, ���j�[�N�� window class ���ƃ^�C�g���𐶐���,
���� window class ���ƃ^�C�g�������� window ��T����, �������炻�� window ��
[`WM_CLOSE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-close) ��
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew) 
���ďI�������܂�.

���肪���ȁu���d�N���h�~�v�菇�ł���,
���̎���g���ۂ͂��߂� window class �������j�[�N�Ȃ��̂ɂ��Ă����Ȃ���,
[���֌W�� window �̃n���O�Ɋ������܂�郊�X�N������](https://www.codeproject.com/Articles/538/Avoiding-Multiple-Instances-of-an-Application)
���Ƃł��m���Ă��܂�.
( �����ɂ́u�����N���v���̋������l�����ׂ��ł���, �{�A�v��������ȕ��ɋN������l�͋��Ȃ��̂�,
window class ��o�^������ł̏����ōς܂��Ă��܂�. )

��, ��y�� ( ���Ԃ� ) ���ޏ�������,
���炪���� window class ��o�^��,
���� window slass �������� [`CMainWnd`](CMainWnd.md) �� `new` ����,
`CWinApp::m_pMainWnd` �Ƃ��ĕێ����܂�.

�Ō�� [`CMainWnd`](CMainWnd.md) �� `Create()` ���܂�.
`Create()` �̈����ɂ��C���������Ȃ�
( style �̎w��͂Ȃ���, window �̃T�C�Y�� 0 ����, ID �� 0 ���� )
�̂�,
[`CMainWnd`](CMainWnd.md) �������\������C���Ȃ� window ������ł�.

��� `TRUE` ( ���������� ) ��Ԃ��Ă��܂�.


## `PreTranslateMessage`

Window message �� dispatch �����O�ɉ�����ēƎ��̏��������ݍ��ނ��߂ɗp������
[`CWinApp::PreTranslateMessage`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#pretranslatemessage)
�� override �ł�.

�܂�, [`WM_CLOSE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-close) �ɂ͉������,
[`CMainWnd`](CMainWnd.md) ��
[`DestroyWindow`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-destroywindow) ������,
[`CMainWnd`](CMainWnd.md) �� `delete` ���܂�.

���̏��������܂Ȃ���,
�Ȃ��� `m_pMainWnd` ��
`DestroyWindow` ���ꂸ,
�v���Z�X�I������ `m_pMainWnd` �̎������� leak ����Ƃ������ۂ�,
�{�A�v���̂悤�� modeless �ȓ�������� window �ɂ͂����̂ł�.

�u���Ⴀ, ���̂��߂ɂ����? `m_pMainWnd`�v�Ƃ����C�����܂���,
`m_pMainWnd->DestroyWindow();` �Ƃ���,
�Ȃ��� `m_pMainWnd` �� NULL �ɃN���A����Ă��܂��̂�,
�킴�킴 `CWnd* pWnd` �ɃR�s�[���Ă������Ă��܂�.
<br>
<sup>
( ����ق� `m_pMainWnd` ���ӎ����Ă���Ȃ�, �Ȃ������� `m_pMainWnd` �� `DestroyWindow` ���Ȃ�? > MFC )
</sup>

[`WM_CLOSE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-close) �ɑ΂��Ă�
��� `TRUE` ( �����ς� ) ��Ԃ��܂�.

����, [`WM_KEYDOWN`](https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-keydown)
���̃L�[���͊֌W�� Window message �������ꍇ,
���̓t�H�[�J�X�������Ă���̂� Edit Box ��������,
������Ɋۓ������܂�.
�u`Ctrl`+`V` �������� paste ���v�Ƃ��Ȃ�Ƃ���,
Edit Box �ɔ��f���Ă��炢�܂��傤.
�E�`�� window �ň�������Ă��ʓ|�Ȃ��.

������, `Enter` �L�[�֌W�� Window Message ������ Edit Box ����ł͂Ȃ�,
�ʏ�̌o�H�ɏ悹�܂�.
�u`Enter` ���@���ꂽ����͂̊��������ɂ���v�Ȃ�� `PreTranslateMessage` �����肪���ł�����.
<br>
( ���ɂ��̎����ł�
[`CDomainPage::PreTranslateMessage`](CDomainPage.md#pretranslatemessage) �������悤�Ȃ��Ƃ���Ă܂�. )

���̑��� Window Message �ɑ΂��Ă͉������C�͂Ȃ��̂�,
[`CWinApp::PreTranslateMessage`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#pretranslatemessage)
�Ɋۓ����������ʂ�Ԃ��Ă��܂�.


## `DoMessageBox`

�A�v������
[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
���ĂԂ�, ���菄���ČĂяo�����񑩂ɂȂ��Ă���
[`CWinApp::DoMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#domessagebox)
�� override �ł�.

[`CWinApp::DoMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#domessagebox)
�Ƃ�������ȓ��� ( �_�C�A���O���\�[�X�Ȃ��Ƀ_�C�A���O��\�� ) ������
[CMessageBox](CMessageBox.md)
�����������Ă���
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) ���܂�.
���̎����̂�������, �{�A�v���� `AfxMessageBox()` �͈�ԑŐs�ɓ�����������܂�.
���́u��������v�Ƃ�, �u�^�X�N�o�[�̒ʒm�̈���̃A�C�R���ɋ߂��ꏊ�Ƀ_�C�A���O��\������v�Ƃ������̂�,
�f�t�H���g��
[`CWinApp::DoMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#domessagebox)
�̃f�t�H���g����ł���u���j�^�[�̒����Ƀ_�C�A���O��\������v���, �����𓮂������ɍςނ悤�ɂȂ��Ă��܂�.

�߂�l��, ���[�U�[���`���C�X�����{�^���� ID �ł�.

