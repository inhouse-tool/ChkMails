# `CMainWnd`

## �\��

�\�[�X�R�[�h���,
[`MainWnd.h`](../ChkMails/ChkMails/MainWnd.h) ��
[`MainWnd.cpp`](../ChkMails/ChkMails/MainWnd.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor & Destructor

[`CMainWnd`](#cmainwnd-1)
[`~CMainWnd`](#cmainwnd-2)

#### Override

[`DestroyWindow`](#destroywindow)

#### �W�� Window Message �n���h���[

[`OnCreate`](#oncreate)
[`OnTimer`](#ontimer)
[`OnGetFont`](#ongetfont)
[`OnSocketNotify`](#onsocketnotify)

#### ���[�U�[��` Window Message �n���h���[

[`OnUserTrayNotify`](#onusertraynotify)
[`OnPopupNotify`](#onpopupnotify)
[`OnGetCommonFont`](#ongetcommonfont)
[`OnGetSender`](#ongetsender)
[`OnShowHelp`](#onshowhelp)

#### �R�}���h�n���h���[

[`OnMenuWeb`](#onmenuweb)
[`OnMenuLicense`](#onmenulicense)
[`OnMenuAbout`](#onmenuabout)
[`OnMenuAccounts`](#onmenuaccounts)
[`OnMenuFilter`](#onmenufilter)
[`OnMenuLogs`](#onmenulogs)
[`OnMenuSetup`](#onmenusetup)
[`OnMenuExit`](#onmenuexit)

#### �풓�A�C�R���֘A

[`AddNI`](#addni)
[`DelNI`](#delni)
[`ModNI`](#modni)

#### ���j���[�֘A

[`SetMenu`](#setmenu)

#### �����֘A

[`Introduce`](#introduce)

#### �ݒ�l�֘A

[`LoadAccounts`](#loadaccounts)
[`SaveAccounts`](#saveaccounts)
[`LoadSettings`](#loadsettings)
[`SaveSettings`](#savesettings)
[`LoadFilters`](#loadfilters)
[`SaveFilters`](#savefilters)
[`LoadTLDs`](#loadtlds)
[`SaveTLDs`](#savetlds)

#### �Í��֘A

[`MakeBlob`](#makeblob)
[`EnDecrypt`](#endecrypt)

#### �d����Ԋ֘A

[`OnPower`](#onpower)

#### ���[���`�F�b�N�֘A

[`PollNow`](#pollnow)
[`PollMails`](#pollnow)
[`ParseMail`](#parsemail)<br>
[`GetAttr`](#getattr)
[`GetAuth`](#getauth)
[`GetFrom`](#getfrom)
[`GetSender`](#getsender)
[`GetType`](#gettype)
[`GetEncode`](#getencode)
[`GetTime`](#gettime)
[`GetCodePage`](#getcodepage)
[`CheckMID`](#checkmid)<br>
[`CheckAlias`](#checkalias)
[`CheckBlackList`](#checkblacklist)
[`CheckWhiteList`](#checkwhitelist)
[`CheckLink`](#checklink)
[`CheckUnicode`](#checkunicode)
[`FilterError`](#filtererror)

#### ���O�֘A

[`MakeLog`](#makelog)
[`SaveLog`](#savelog)
[`IsDuplicated`](#isduplicated)
[`AddSuffix`](#addsuffix)
[`TrimFiles`](#trimfiles)
[`MoveFiles`](#movefiles)

#### �T�}���[�֘A

[`MakeSummary`](#makesummary)
[`ShowSummary`](#showsummary)
[`ShareSummary`](#sharesummary)

#### ������ϊ��֘A

[`StringFromHeader`](#stringfromheader)
[`StringFromBody`](#stringfrombody)
[`DecodeBase64`](#decodebase64)
[`DecodeQuoted`](#decodequoted)
[`StringFromCodePage`](#stringfromcodepage)
[`EscapeFromJIS`](#escapefromjis)
[`StringToUTF8`](#stringtoutf8)
[`LFtoCRLF`](#letocrlf)
[`HexToASCII`](#hextoascii)
[`HexToUnicode`](#hextounicode)

#### �ʐM�֘A

[`ConnectPOP`](#connectpop)
[`RespondPOP`](#respondpop)
[`ClosePOP`](#closepop)

#### �f�o�b�O�֘A

[`FeedDebug`](#feeddebug)
[`ReadDebug`](#readdebug)


## �T�v

�̐S�Ȃ��Ƃ�, �S�Ă��� class �̒��ōs���Ă��܂�.
���̃v���W�F�N�g�̒��ɂ͑��ɂ��F�X class ������܂���,
���� class �ȊO�͂قƂ�ǐݒ�̂��߂̃_�C�A���O�Ƃ��ʒm�̂��߂̃_�C�A���O�Ƃ�,
���� class �ƃ��[�U�[�̑Θb�̂��߂ɐ݂���ꂽ���̂ł�.

���� class �̒��ōs���Ă����ȓ����,
�ȉ��̒ʂ�ł�.

1. �ݒ�l����荞��.
1. �^�X�N�o�[�̒ʒm�̈�ɃA�C�R�����d����.
1. �^�C�}�[���d�|����.

��, �^�C�}�[���������邲�Ƃ�,

1. �ݒ肳��Ă������[���T�[�o�[��[POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol)�Őڑ�����.
1. �ݒ肳��Ă������[�U�[���ƃp�X���[�h�� POP3 �ɃT�C���C������.
1. ���[�����͂��Ă��邩�m�F��, �͂��Ă���΂��̒��g��ǂݎ����, �ݒ�ɏ]���ăX�p�����ǂ������肷��.
1. �X�p���Ɣ��肵�����[���͔j������悤 POP3 �Ń��[���T�[�o�[�ɗv������.
1. �X�p���ł͂Ȃ����[�����͂��Ă�����, �A�C�R���ł��m�点����.
1. ���[���̃C���[�W�� [`.eml`](https://www.google.com/search?q=.eml+�t�@�C��) �t�@�C���ŕۑ�����.
1. ��L�C���[�W���f�R�[�h���� `.txt` �t�@�C�����ۑ�����.

�Ƃ��������Ƃ��s���Ă��܂�.

�^�X�N�o�[�ɏ풓�����Ă���A�C�R�����������̓����:

* ���N���b�N���ꂽ�璼���Ƀ��[�����m�F����.
* �E�N���b�N���ꂽ��|�b�v�A�b�v���j���[���o��.

�ƂȂ��Ă���,
�|�b�v�A�b�v���j���[�ł����ꂩ�̍��ڂ��I�����ꂽ�ꍇ��,
���̍��ڂ̓�����s���܂�.

���̑��ׂ̍�������ł���,

* �ݒ�l�̂������[���A�J�E���g�Ɋւ������, �Í������ă��W�X�g���[�Ɋi�[
* ���[���̃T�}���[��
[�t�@�C���}�b�s���O](https://learn.microsoft.com/ja-jp/windows/win32/memory/file-mapping)
�ő��v���Z�X�ɋ��L����@�\����� ( �f�t�H���g�� OFF )
* �󂯎��ς݂̃��[���� `.eml` ����킹�� [Filter](../README.md#filter) �̐ݒ���u���K�v����@�\�t��

�Ƃ������Ƃ��������܂�.

�Ƃ���ł��� class �� `CWnd` ����h������ window �Ȃ̂ł���,
�����\�����܂���. �\������C���Ȃ��̂ł�.
����Ȃ̂ɂȂ��uwindow�v�Ƃ����`�Ԃ��Ƃ��Ă���̂��Ƃ�����,
�����P�ɁuWindow Message ���󂯂ē�����������v�Ƃ��������ł�.
���� Windows&reg; �A�v���P�[�V�����́u�񓯊��v�Ƃ�����
�u[event driven](https://ja.wikipedia.org/wiki/�C�x���g�쓮�^�v���O���~���O)�v�Ȃ��̂ł���,
�� class �����̗�ɘR�ꂸ, �u*Window Message* driven�v�̓��쌴���œ����Ă��܂�.

�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.


## `CMainWnd`

���� class �� constructor �ł�.

�ʂ� constructor �łȂ��Ă�,
�������̌_�@�͂���1�ʂ�[�uwindow �Ƃ��� create ���ꂽ�Ƃ��v](#oncreate)������̂ł���,
���̍ۂɏ���������̂́uwindow �Ƃ��� create ����Ă���łȂ��Ə������ł��Ȃ����́v�Ƃ�,
�����őS�Ẵ����o�[�ϐ������������Ă��܂�.
<sub>
�������Ă����Ȃ���[C26495](https://learn.microsoft.com/ja-jp/cpp/code-quality/c26495)�����邳���̂�.
</sub>

�ʂ̕ϐ��������Ƃ͕ʂ�, �ȉ��̃��[�U�[�ݒ�l�ǂݍ��݂̊֐����Ăяo���Ă��܂�.

| �֐� | �������Ώ� |
| --- | --- |
| [`LoadSettings`](#loadsettings) | [Setup](../README.md#setup) �Őݒ�ł���p�����[�^�[�̓ǂݍ��� |
| [`LoadFilters`](#loadfilters) | [Filter](../README.md#filter) �Őݒ�ł���p�����[�^�[�̓ǂݍ��� |
| [`LoadTLDs`](#loadtlds) | �ߋ��Ɏ�M�������т̂��� [TLD ( Top Level Domain )](https://ja.wikipedia.org/wiki/�g�b�v���x���h���C��) ���̓ǂݍ��� |
| [`MakeBlob`](#makeblob) | �����p [BLOB](https://ja.wikipedia.org/wiki/�o�C�i���E���[�W�E�I�u�W�F�N�g) �̓ǂݍ��� |


## `~CMainWnd`

���� class �� destructor �ł�.

�ʂ� destructor �łȂ��Ă�,
�Еt����_�@�͂���1�ʂ�[�uwindow �Ƃ��� destroy ���ꂽ�Ƃ��v](#destroywindow)������̂ł���,
�uconstructor �ō�������̂� destructor �ŕЕt����v�Ƃ����Ώ̐��̂��߂�,
�Œ���̌�Еt�����s���Ă��܂�.


## `DestroyWindow`

`CWnd::DestroyWindow` �� override �ł�.

[`DelNI`](#delni) ���Ăяo����, �^�X�N�o�[�̒ʒm�̈�ɐ��荞�܂����A�C�R����Еt��,
[`CNotifyWnd`](CNotifyWnd.md) ���Еt���܂�.

�v�́ucreate ���ɍ�������̂� destroy ���ɕЕt����v�Ƃ����Ώ̐��̂��߂�,
�����ɋ��ݍ���ł��܂�.
������, ���̌�� `CWnd::DestroyWindow` �Ɉς˂܂�.
�Ȃ̂Ŗ߂�l�� `CWnd::DestroyWindow` �̔��f�Ɉς˂Ă��܂�.


## `OnCreate`

Window Message [`WM_CREATE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-create)
�ɑ΂���n���h���[�ł�.

�܂���� class �� `OnCreate` ���ς܂�����,
[CProperSheet](CProperSheet.md) �ɗ^���錩�₷���t�H���g
( [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
���������Ă���ǂ݂Â炢�t�H���g�ɑ����� ) ������Ă���,
���̑��̃_�C�A���O�ɂ����ʂœK�p���铙���̃t�H���g������Ă����܂�.

���ꂩ��,
sleep ��Ԃ���̕��A��ߑ����邽�߂̃n���h���[ [`OnPower`](#onpower) ��o�^���܂�.

�Ō�Ɂu�����J�n�v�̂��߂̃^�C�}�[ `TID_INIT` ��҂����ԃ[���Ŏd�|���ďI���ł�.

�u�҂����ԃ[����������^�C�}�[�Ȃ�Ďd�|�����ɂ��̏�ł���B�v�Ǝv������������낤���Ƒ����܂���,
����� [`WM_TIMER` �̗D��x�̒Ⴓ](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-timer#remarks)
�𗘗p�����u�d�؂蒼���v�ł�.
���́u�d�؂蒼���v�ɂ��, ���ۂɁu�����J�n�v�����{�����̂�,
����܂ł̏������łȂ񂾂��񂾔�ь����Ă��� Window Message ���S�ăn�P�Ă���Ƃ������ƂɂȂ�܂�.


## `OnTimer`

Window Message [`WM_TIMER`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-timer)
�ɑ΂���n���h���[�ł�.

���L�̃^�C�}�[�l����舵���Ă��܂�.

| �^�C�}�[�l | �� | �������e |
| --- | --- | --- |
| `TID_INIT` | �u�����J�n�v | [`Introduce`](#introduce) �ɂă��[���A�J�E���g��ǂݍ���. ���ݒ�Ȃ炲���A. |
| `TID_READY` | �u�������v | [`AddNI`](#addni) �ɂă^�X�N�o�[�̒ʒm�̈�ɃA�C�R���o�^. �o�^�����܂ŌJ��Ԃ�. |
| `TID_START` | �u�ʏ�c�ƊJ�n�v | [`PollNow`](#pollnow) �ɂčŏ��̃`�F�b�N�ƈȍ~�̂��߂� `TID_POLL` ���Z�b�g. |
| `TID_POLL` | �u�ʏ�c�ƒ��v | [`PollMails`](#pollmails) �ɂă��[���̃`�F�b�N. |


## `OnGetFont`

Window Message [`WM_GETFONT`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-getfont)
�ɑ΂���n���h���[�ł�.

���炩���� [`OnCreate`](#oncreate) �ō���Ă������u���₷���t�H���g�v�� handle ��Ԃ������ł�.
<br>
�L�ӂȒl��Ԃ��Ă���̂�, `WM_GETFONT` ��
[`PostMessage`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew) �ł͂Ȃ�
[`SendMessage`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
�ő����Ă��������̂��O��ł�.

������ `WM_GETFONT` �𓊂��Ă���̂�,
[`CProperSheet`](CProperSheet.md) ���炢�Ȃ��̂ł�.
���˂Ă��� page �̂��ꂼ���, ���̊֐����Ԃ��Ă���t�H���g��K�p���Ă��܂�.

���̃_�C�A���O�͕��ʂ̃_�C�A���O�Ȃ̂�, ���ꂼ�ꎩ�O�Ō��₷���t�H���g��ݒ肵�Ă��܂�.


## `OnSocketNotify`

Window Message `WM_SOCKET_NOTIFY`
�ɑ΂���n���h���[�ł�.

`WM_SOCKET_NOTIFY` �͌��� MFC �����Ŏg�p���邽�߂̒�`�Ȃ̂ł���
( �̂ɐ������Ă��� web page �����݂��Ȃ� ),
���ʂɃr���h����� `#include` �Ɋ܂܂�Ă��� `afxpriv.h` �ɓ����Ă����`�Ȃ̂�,
�A�v���P�[�V�����v���O���}�[�� ( ����� ) �g�����`�l�ł�.

�����ł� [`CParaSocket`](CParaSocket.md) ����̒ʒm�Ɏg���Ă��܂�.

�n���������̓��e��

| ���� | ���e |
| --- | --- |
| `wParam` | ��� 16bits ���\�P�b�g�̏�Ԃ������R�[�h, ���� 16bits �����[�U�[��`�̎��ʒl. |
| `lParam` | ���M���\�P�b�g�ւ̃|�C���^. |

�ƂȂ��Ă���, �u��Ԃ������R�[�h�v��
[`ParaSocket.h`](../ChkMails/ChkMails/ParaSocket.h)
�ɒ�`����Ă��� `SOCK_STATE_` ����n�܂� macro ���Œ�`���ꂽ�l�ł�.
�����ł�

| ��� | �Ӗ� | �� class �̑Ή� |
| --- | --- | --- |
| `SOCK_STATE_FAILED` | �u�Ȃ񂩎��s�����v | [`ClosePOP`](#closepop) ���Ă�ŒʐM��ؒf. |
| `SOCK_STATE_RECEIVED` | �u�Ȃ񂩎�M�����v | [`RespondPOP`](#respondpop) ���Ă�Ŏ�M�������̂��󂯎��. |

�� 2��ނ݈̂����Ă��܂�.

`SOCK_STATE_RECEIVED` ���ʒm���ꂽ�ꍇ��,
��M�����o�C�g�������ǂݎ���� `CStringA` �Ɏd���ĂĂ���,
[`RespondPOP`](#respondpop) �ɓn���܂�.

���ɈӖ��̂���߂�l��Ԃ��Ă����ł��Ȃ��̂�,
`WM_SOCKET_NOTIFY` ��
[send](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
�ł͂Ȃ�
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew)
�ő����Ă��������Ă��������傤�Ԃł�.


## `OnUserTrayNotify`

[`CChkMailsApp`](CChkMailsApp.md) �Œ�`���� Window Message `WM_USER_TRAY_NOTIFY`
�ɑ΂���n���h���[�ł�.

`WM_USER_TRAY_NOTIFY` ��, [`AddNI`](#addni) �ɂČĂѕԂ��� message �Ƃ��ēo�^���Ă���̂�,
�A�C�R�������삳���Ɠ����Ԃ���Ă��� Window Message �ł�.

���L�̑������舵���Ă��܂�.

| ���� | �Ӗ� | �� class �̑Ή� |
| --- | --- | --- |
| `WM_LBUTTONUP` | �N���b�N | [`PollNow`](#pollnow) �ɂčŏ��̃`�F�b�N�ƈȍ~�̂��߂� `TID_POLL` ���Z�b�g. |
| `WM_RBUTTONUP` | �E�N���b�N | �_�C�A���O�\�����Ȃ炻����ɒ��ӊ��N.<br>�łȂ���� [SetMenu](#setmenu) �ɂă��j���[�\��. |

��L�́u�_�C�A���O�\�����Ȃ�v�Ƃ��������,
[`OnPopupNotify`](#onpopupnotify) ���󂯎�����ŐV�� `lParam` �Ɋ�Â��Ă��܂�.
<br>
<sub>
����, ���Ȃ��Ɂu�o�Ă����_�C�A���O�𑀍삷��v�Ƃ��Ă���������΂���Ȃ��Ƃ��Ȃ��Ă��ςނ�ł���,
�o�Ă����_�C�A���O�ȊO�Ɂu���C�v�����, �u����������Ȃ��B���������B�v�ƒ��ӂ����킯�ł�.
</sub>

���ɈӖ��̂���߂�l��Ԃ��Ă����ł��Ȃ��̂�,
`WM_USER_TRAY_NOTIFY` ��
[send](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
�ł͂Ȃ�
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew)
�ő����Ă��������Ă��������傤�Ԃł�.


## `OnPopupNotify`

[`CChkMailsApp`](CChkMailsApp.md) �Œ�`���� Window Message `WM_POPUP_NOTIFY`
�ɑ΂���n���h���[�ł�.

[`NotifyPopup`](CChkMailsApp.md#notifypopup) �ɂ�� `lParam` �ɑ��M�� window �� handle ���ڂ��Ă���̂�,
����Ɋ�Â��đ��M�� window �̃T�C�Y������,
���̃T�C�Y�Ɋ�Â��đ��M�� window ���v���C�}�����j�^�[�̃^�X�N�o�[�̒ʒm�̈�Ɋ��Y���悤�Ȉʒu�ɂȂ�悤�v�Z��,
���M�� window �̕\���ʒu���ړ������܂�.

Windows&reg; 11 �ł̓^�X�N�o�[�̓��j�^�[�̒�ӂɌŒ�ɂȂ�܂�����,
Windows&reg; 10 �ł͏㉺���E�ɓ�������̂�,
�^�X�N�o�[���ǂ��ɂ���̂������ɂ߂���ŕ\���ʒu�����肵�Ă��܂�.

�Ȃ�, ���� `WM_POPUP_NOTIFY` �������ۂ�
[`CNotifyWnd`](CNotifyWnd.md) ���\������Ă����ꍇ��, ���̕\���͏����Ă����܂�.
���ꂩ��n�܂鑗�M�� window �̑���̎ז��ɂȂ�̂�.

�܂�, `WM_POPUP_NOTIFY` ��
�e�_�C�A���O�� `DoModal` ���甲���Ă��������ۂ� `lParam` ��n���Ă��郋�[���ɂȂ��Ă���̂�,
�u�����ꂩ�̃_�C�A���O��
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
���ł��邱�Ɓv��m�点���ڂ����˂Ă��܂�.
 [`OnUserTrayNotify`](#onusertraynotify) �ł�,
�u`OnPopupNotify` �Ŏ󂯎�����ŐV�� `lParam` ������ۂłȂ���΁v
���̃_�C�A���O���N������悤�ȃ��j���[�̎��s���֎~���Ă��܂�.
<br>
<sub>
����, ���Ȃ��Ɂu�o�Ă����_�C�A���O�𑀍삷��v�Ƃ��Ă���������΂���Ȃ��Ƃ��Ȃ��Ă��ςނ�ł���.
</sub>

���ɈӖ��̂���߂�l��Ԃ��Ă����ł��Ȃ��̂�,
`WM_POPUP_NOTIFY` ��
[send](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
�ł͂Ȃ�
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew)
�ő����Ă��������Ă��������傤�Ԃł�.


## `OnGetCommonFont`

[`CChkMailsApp`](CChkMailsApp.md) �Œ�`���� Window Message `WM_GET_COMMONFONT`
�ɑ΂���n���h���[�ł�.

���炩���� [`OnCreate`](#oncreate) �ō���Ă������u�����̃t�H���g�v�� handle ��Ԃ������ł�.
<br>
�L�ӂȒl��Ԃ��Ă���̂�, `WM_GETFONT` ��
[`PostMessage`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew) �ł͂Ȃ�
[`SendMessage`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
�ő����Ă��������̂��O��ł�.

������ ( [`GetCommonFont`](CChkMailsApp.md#getcommonfont) �o�R�� ) `WM_GET_COMMONFONT` �𓊂��Ă���̂�,
�u�����̃t�H���g�v��K�v�Ƃ���_�C�A���O�݂̂Ȃ����, �Y���ґ����ł�.


## `OnGetSender`

[`CChkMailsApp`](CChkMailsApp.md) �Œ�`���� Window Message `WM_GET_SENDER`
�ɑ΂���n���h���[�ł�.

����̒ʐM�œ������[����

* ���M�҂́u���`�v( `From:` �ɏ����Ă��������M�Җ� )
* ���ۂ̑��M�Җ� ( `Authentication-Results:` �Ȃǂɋ������Ă������M�Җ� )

�� 2�̏��̃y�A��, 1�̕�����Ƀ}�[�W�������̂�Ԃ��܂�.
<br>
�L�ӂȒl��Ԃ��Ă���̂�, `WM_GET_SENDER` ��
[`PostMessage`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew) �ł͂Ȃ�
[`SendMessage`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
�ő����Ă��������̂��O��ł�.

[`CWhitePage`](CWhitePage.md) �� [`TakePair`](CWhitePage.md#takepair) ������ł��� Window Message �ł�.
�� page �ł�������񂪗~�����̂ł���, ���O�Ŋ���o����肷�łɓ������������Ă��邱����� class ��,
���̉��������˗����Ă���킯�ł�.

������, [Filter](../README.md#filter) �Ɉ��������������[���ɑ΂��Ă�,
����ۂ̕������Ԃ��Ă��܂�.
������󂯎���� [`TakePair`](CWhitePage.md#takepair) ��,
�u�ށA���̑��M�҂̓��o�����B�v�Ǝ@����킯�ł�.


## `OnShowHelp`

[`CChkMailsApp`](CChkMailsApp.md) �Œ�`���� Window Message `WM_SHOW_HELP`
�ɑ΂���n���h���[�ł�.

`Help` �{�^������������_�C�A���O(�ƃV�[�g)����, `Help` �{�^���������ꂽ���ɑ����Ă��܂�.
`lParam` �ɑ��M�� window �� handle ���n����Ă���̂�,
����Ɋ�Â��đ��M�� window �̃^�C�g�� ( caption ) ������,
���̕�������������ɂ���Ȃǂ̉��H���s��,
���H��������������� github ��̃g�b�v�� [README.md](../README.md) �Ɂu�߂̖��O�v�Ƃ��ĕt�^�������̂��u���E�U�[�ŊJ��,
�܂�u�Y���̍��ڂ̐�����\��������v�Ƃ���������s���܂�.

���ɈӖ��̂���߂�l��Ԃ��Ă����ł��Ȃ��̂�,
`WM_SHOW_HELP` ��
[send](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
�ł͂Ȃ�
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew)
�ő����Ă��������Ă��������傤�Ԃł�.


## `OnMenuWeb`

[`SetMenu`](#setmenu) ���񎦂������j���[�� [`View info on website`](../README.md#view-info-on-website)
���I�΂ꂽ���ɔ��ł��� `ID_HELP_INDEX` �ɑ΂���n���h���[�ł�.

[`ShellExecute`](https://learn.microsoft.com/ja-jp/windows/win32/api/shellapi/nf-shellapi-shellexecutew)
��[���A�v���̃g�b�v�y�[�W](../README.md#chkmails) ���J�������ł�.


## `OnMenuLicense`

[`SetMenu`](#setmenu) ���񎦂������j���[�� [`License announcement`](../README.md#license-announcement)
���I�΂ꂽ���ɔ��ł��� `ID_HELP_USING` �ɑ΂���n���h���[�ł�.

�P�� [MIT License](https://ja.wikipedia.org/wiki/MIT_License)
��
[License terms](https://en.wikipedia.org/wiki/MIT_License#License_terms)
��
[`AfxMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
�ŕ\�����邾���ł�.


## `OnMenuAbout`

[`SetMenu`](#setmenu) ���񎦂������j���[�� [`About ChkMails`](../README.md#about-chkmails)
���I�΂ꂽ���ɔ��ł��� `ID_APP_ABOUT` �ɑ΂���n���h���[�ł�.

[`CChkMailsApp::GetVersionInfo`](CChkMailsApp.md#getversioninfo)
�ň��������Ă����o�[�W��������
[`AfxMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
�ŕ\�����邾���ł�.


## `OnMenuAccounts`

[`SetMenu`](#setmenu) ���񎦂������j���[�� [`Accounts`](../README.md#accounts)
���I�΂ꂽ���ɔ��ł��� `ID_MENU_ACCOUNTS` �ɑ΂���n���h���[�ł�.

�����菇�͈ȉ��ł�.

1. [`CAccountDlg`](AccountDlg.md) �����̊֐����Ń��[�J���� ( �����ϐ��Ƃ��� ) �錾 ( construct ����� ).
1. ���̃N���X�ŕێ����Ă���A�J�E���g�z�� `m_aAccount` �̃R�s�[�����.
1. [`CAccountDlg`](CAccountDlg.md) �� [`SetAccounts`](CAccountDlg.md#setaccounts) ��ʂ��ăR�s�[(�ւ̎Q��)��^����.
1. [`CAccountDlg`](CAccountDlg.md) �� [`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) ���Ă�ł��̓�����҂�.
1. ������ `IDOK` ( `OK` �{�^���������ꂽ ) �łȂ���ΏI��.
1. �R�s�[�ƃI���W�i���� `m_aAccount` ���r.
1. ����Ă�����R�s�[����荞��, [`SaveAccounts`](#saveaccounts) �ŃZ�[�u�������, [`PollNow`](pollnow) �Ń`�F�b�N�J�n.
1. �A�J�E���g���S�ď�����Ă�����, �����\�����邽�� [`ModNI`](#modni) �ŃA�C�R���ύX.
1. ���̊֐��̏I���ɔ���, [`CAccountDlg`](CAccountDlg.md) ������ ( destruct ����� ).


## `OnMenuFilter`

[`SetMenu`](#setmenu) ���񎦂������j���[�� [`Filter`](../README.md#filter)
���I�΂ꂽ���ɔ��ł��� `ID_MENU_FILTER` �ɑ΂���n���h���[�ł�.

�����菇�͈ȉ��ł�.

1. [`CFilterSheet`](CFilterSheet.md) �����̊֐����Ń��[�J���� ( �����ϐ��Ƃ��� ) �錾 ( construct ����� ).
1. ���̃N���X�ŕێ����Ă���ݒ�l�� [`CFilterSheet`](FilterSheet.md) �z���̊e page �Ɏd����.
1. [`CFilterSheet`](CFilterSheet.md) ��
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) ���Ă�ł��̓�����҂�.
1. ������ `IDOK` ( `OK` �{�^���������ꂽ ) �łȂ���ΏI��.
1. �e page ����ݒ�l�����.
1. ��������l�ƃI���W�i���̐ݒ�l���قȂ���̂���荞��.
1. [`SaveFilters`](#savefilters) �ɂ�, �ύX���ꂽ�ݒ�l���Z�[�u.
1. ���̊֐��̏I���ɔ���, [`CFilterSheet`](CFilterSheet.md) ������ ( destruct ����� ).


## `OnMenuLogs`

[`SetMenu`](#setmenu) ���񎦂������j���[�� [`Logs`](../README.md#logs)
���I�΂ꂽ���ɔ��ł��� `ID_MENU_LOGS` �ɑ΂���n���h���[�ł�.

���O�t�H���_ `m_strLogPath` ��������
[`ShellExecute`](https://learn.microsoft.com/ja-jp/windows/win32/api/shellapi/nf-shellapi-shellexecutew)
�Ɋۓ�������, Explorer �Ń��O�t�H���_���J���Ă��炢�܂�.


## `OnMenuSetup`

[`SetMenu`](#setmenu) ���񎦂������j���[�� [`Setup`](../README.md#setup)
���I�΂ꂽ���ɔ��ł��� `ID_MENU_SETUP` �ɑ΂���n���h���[�ł�.

�����菇�͈ȉ��ł�.

1. [`CSetupDlg`](CSetupDlg.md) �����̊֐����Ń��[�J���� ( �����ϐ��Ƃ��� ) �錾 ( construct ����� ).
1. ���̃N���X�ŕێ����Ă���ݒ�l��[`CSetupDlg`](CSetupDlg.md) �Ɏd����.
1. [`CSetupDlg`](CSetupDlg.md) �� [`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) ���Ă�ł��̓�����҂�.
1. ������ `IDOK` ( `OK` �{�^���������ꂽ ) �łȂ���ΏI��.
1. [`CSetupDlg`](CSetupDlg.md) ����ݒ�l�����.
1. ��������l�ƃI���W�i���̐ݒ�l���قȂ���̂���荞��.
1. [`SaveSettings`](#savesettingss) �ɂ�, �ύX���ꂽ�ݒ�l���Z�[�u.
1. ���̊֐��̏I���ɔ���, [`CSetupDlg`](CSetupDlg.md) ������ ( destruct ����� ).

* [Action](../README.md#action) �̎��ԊԊu���ύX���ꂽ�ꍇ��,
�������[������x�`�F�b�N���Ă���ݒ肳�ꂽ���ԊԊu�ɓ���܂�.
* [Log](../README.md#log) �̃t�H���_�[���ύX���ꂽ�ꍇ��,
[`MoveFiles`](#movefiles) �Ń��O�t�@�C���̈����z�����s���܂�.
* [Log](../README.md#log) �̃t�@�C�������ύX���ꂽ�ꍇ��,
[`TrimFiles`](#trimfiles) �Ń��O�t�@�C�����̒������s���܂�.


## `OnMenuExit`

[`SetMenu`](#setmenu) ���񎦂������j���[�� [`Exit`](../README.md#exit)
���I�΂ꂽ���ɔ��ł��� `ID_MENU_EXIT` �ɑ΂���n���h���[�ł�.

�{���ɏI�����邩���m�F����
[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
���o����,
������ `IDOK` ( `OK` �{�^���������ꂽ ) �ł����,
[`WM_CLOSE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-close)
���� window ��
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew)
���܂�.


## `AddNI`

�^�X�N�o�[�̒ʒm�̈�ɖ{�A�v���̃A�C�R����ǉ����܂�.

�uNI�v�́uNotification Icon�v����, �Ƃ��������x�̃l�[�~���O�ł�.

`AddNI` �́uNI�O�Z��v�̒��j�ł�.

[`Shell_NotifyIcon`](https://learn.microsoft.com/ja-jp/windows/win32/api/shellapi/nf-shellapi-shell_notifyiconw)
�ɗ���ŃA�C�R����ǉ����܂���,
���̌Ăяo���͗��_�㎸�s���邱�Ƃ����邻���Ȃ̂�,
�����ǉ��ł����� `true` ��, �_���������� `false` ��Ԃ����Ƃɂ��Ă��܂�.
������ `false` ���Ԃ����ꍇ��,
�����Ԃ�u���ă��g���C���邱�Ƃɂ��Ă��܂�.
���́u�Ԃ�u���ă��g���C�v�̂��߂�
[`OnTimer`](#ontimer) ����Ăяo����Ă��܂�.


## `DelNI`

�^�X�N�o�[�̒ʒm�̈�ɂ���{�A�v���̃A�C�R�����폜���܂�.

`DelNI` �́uNI�O�Z��v�̎��j�ł�.

�A�C�R�����s�v�ɂȂ�̂�, �{�A�v�����I������Ƃ��ł�.
�Ȃ̂�, [DestroyWindow](#destroywindow) ����Ăяo����Ă��܂�.


## `ModNI`

�^�X�N�o�[�̒ʒm�̈�ɂ���{�A�v���̃A�C�R����ύX���܂�.

`ModNI` �́uNI�O�Z��v�̖����q�ł�.

�Z�B�ƈ���ĖZ����, ������������F��ȋǖʂŌĂ΂�Ă��܂�.

| �Ăяo���� | �� | �Ή� |
| --- | --- | --- |
| [`OnMenuAccounts`](#onmenuaccounts)	| �A�J�E���g���S�č폜���ꂽ | �A�C�R��![](../pics/Icon.White.png)�ƃ��b�Z�[�W�ŃA�J�E���g�������A�s�[��. |
| [`ConnectPOP`](#connectpop)		| �ڑ����J�n���� | �A�C�R��![](../pics/Icon.Yellow.png)�ƃ��b�Z�[�W�ŊJ�n��������. |
| [`ClosePOP`](#closepop)		| �ڑ������s���� | �A�C�R��![](../pics/Icon.Red.png)�ƃ��b�Z�[�W�Ŏ��s��i����. |
| [`RespondPOP`](#respondpop)		| �A�J�E���g�̃`�F�b�N���I����� | �A�C�R��![](../pics/Icon.Grey.png)![](../pics/Icon.Green.png)�ƃ��b�Z�[�W�Ŏ����񍐂���. |
| [`FeedDebug`](#feeddebug)		| �f�o�b�O�p�t�@�C����ǂݐ؂��� | �A�C�R��![](../pics/Icon.Grey.png)![](../pics/Icon.Green.png)�ƃ��b�Z�[�W�Ŏ����񍐂���. |

�Ō�́u�����񍐂���v�ł���, ���̕񍐂��˗�����Ăяo���̍ۂɂ́u���̃��b�Z�[�W���o���v�Ƃ̎w��������܂���.
�����ƂȂ��Ă��郁�b�Z�[�W������ۂŌĂяo����܂�.
����͕񍐂��郁�b�Z�[�W�̑I�ʂ��ۓ�������w����,
�󃁃b�Z�[�W���󂯎�����{�֐���,
���ǂ̃��[�����ɏ]���ă��b�Z�[�W�̕���������,
���ǂ̃��[���̗L���ɏ]���ăA�C�R���̎��
( ![](../pics/Icon.Grey.png)��![](../pics/Icon.Green.png) ) ��I�т܂�.

���̊֐��͏󋵕ω����������ۂɕK���ʂ�؂���ɂȂ��Ă���̂�,
���̊֐����� [`ShowSummary`](#showsummary) �� [`ShareSummary`](#sharesummary) ���Ăяo����,
���ł� [Notification](../README.md#notification) ���s���Ă��܂�.


## `SetMenu`

�|�b�v�A�b�v���j���[��\�����܂�.

[`OnUserTrayNotify`](#onusertraynotify) ����A�C�R�����E�N���b�N���ꂽ�Ƃ��ɌĂяo����܂�.

�i���Ƃ��Ă�:

1. ���j���[���� `IDR_MENU_POPUP` �� 0 �Ԗڂ̃T�u���j���[���Q�b�g.<br>( `IDR_MENU_POPUP` �̃��j���[�ł͂Ȃ�, ���� 0 �Ԗڂ̃T�u���j���[ )
1. [`SetForegroundWindow`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-setforegroundwindow)�ŋ����I�� foreground �ɖ��o��.
1. [`GetCursorPos`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-getcursorpos)�Ō��� ( �A�C�R�����E�N���b�N�����Ƃ��� ) �̃}�E�X�̈ʒu���Q�b�g.
1. [`TrackPopupMenu`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-trackpopupmenu)�Ń}�E�X�|�C���^�[�̈ʒu�� 0 �Ԗڂ̃T�u���j���[��\��.
1. ���܂��Ȃ��� `WM_NULL` ��[`PostMessage`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew).

�ƂȂ��Ă��܂�.

���̂��� 5. �̂��܂��Ȃ���, �×� Visual Studio&reg; ���[�U�[�̊Ԃł͓`������Ă������U�ł���,
�ŋ�, ����[�Ō������̕K�v����F�߂�Ƃ���ƂȂ�܂���.
](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-trackpopupmenu#remarks)
<br>
<sup>
����, �u�����v���������񂶂�Ȃ���, �u�C���v���Ă���񂩂�?
</sup>


## `Introduce`

�u�����v���܂�.

�u�����v�Ƃ��Ă�, �܂� [`LoadAccounts`](#loadaccounts)�Őݒ�ς݂̃��[���A�J�E���g��ǂݍ��ނ��Ƃ���n�߂܂�.

�Ăяo������ [`OnTimer`](#ontimer) �Ń^�C�}�[ `TID_INIT` �����������Ƃ��ł�.
( ������u�N�����Ă����v�ł�. )

��, ���łɃA�J�E���g���ݒ�ς݂������ꍇ��,
`true` ��Ԃ��Ă����܂��ł�.

���, �ݒ�ς݃A�J�E���g���[���������ꍇ��,
[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
���ĉ��L�̂悤�Ɏ��ȏЉ�܂�.<br>

![](../pics/Intro.png)<br>
<sup>
[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
�Ȃ̂Ɏ��O�̃A�C�R�����Y�����Ă���̂�, �����������Ƃ��ł���悤�ɏ��׍H���ꂽ
���Ɛ���[`DoMessageBox`](CChkMailsApp.md#domessagebox) ���g���Ă��邩��ł�.
</sup>

�܂�,<br>
�u���q����A���߂Ă���? ���[���A�J�E���g�̐ݒ肪�K�v�Ȃ񂾂��A�����邩��?�v
<br>
�Ɗm�F���Ă���킯�ł���,
���ꂪ�f��ꂽ�ꍇ�� `false` ��Ԃ��đf���Ɉ���������܂�.

������ꂽ�ꍇ��,
�� window �� `ID_MENU_ACCOUNTS` ���΂���,
[`OnMenuAccounts`](#onmenuaccounts) �Ɏ������݂܂�.


## `LoadAccounts`

[Accounts](../README.md#accounts) �ݒ�����[�h���܂�.

�Ăяo�����͑O�o�� [`Introduce`](#introduce) �ł�.

�{�A�v���� 1�l�̃��[�U�[�������̃��[���A�J�E���g�������ƂɑΉ����Ă���̂�,
�ݒ肳��Ă���A�J�E���g���S�Ă�ǂݍ��݂܂�.
�A�J�E���g���͂��������Í������Ă��܂��Ă���̂�,
�ǂݍ��ނƂ��͕������ĕ����񉻂��Ă��܂�.
������
[`EnDecrypt`](#endecrypt) �Ƃ����ǂ��������Ȗ��O�̊֐����S���ł�.


## `SaveAccounts`

[Accounts](../README.md#accounts) �ݒ���Z�[�u���܂�.

�{�A�v���� 1�l�̃��[�U�[�������̃��[���A�J�E���g�������ƂɑΉ����Ă���̂�,
�ݒ肳�ꂽ�A�J�E���g���S�Ă��������݂܂�.
�A�J�E���g���͂��������Í������Ă��܂��Ă���̂�,
�������ނƂ��͕�������Í������Ă��܂�.
�Í�����
[`EnDecrypt`](#endecrypt) �Ƃ����ǂ��������Ȗ��O�̊֐����S���ł�.

[`OnMenuAccounts`](#onmenuaccounts) �ŃA�J�E���g�o�^�ɕύX���������Ƃ��ɌĂяo����Ă��܂�.

�A�J�E���g��������悤�Ȑݒ�����ꂽ�Ƃ���,
���łɎg���Ȃ��Ȃ����L�^�����܂ł��c���Ă����̂��A���Ȃ̂�,
�����������L�^�𖕏����邽�߂�
[`DeleteProfileValue`](CChkMailsApp.md#deleteprofilevalue)
�̂����b�ɂȂ��Ă��܂�.
���֐��� `FALSE` (�u����Ȃ̂˂���v) ��Ԃ��Ă���܂ŌĂяo�����J��Ԃ���,
�s�v�ȋL�^�͑S�Ė������ꂽ���ƂɂȂ�܂�.


## `LoadSettings`

[Setup](../README.md#setup) �̐ݒ�l�����[�h���܂�.

�\�[�X�R�[�h�������܂�܂�, ���ɂǂ����Ă��Ƃ̂Ȃ������ł���,
[���O](../README.md#log)�����܂��Ă����t�H���_�[�̏����l��,
�Œ�l�ł͂Ȃ����[�U�[���Ƃ̃e���|�����[�t�H���_�[�Ƃ��邽��,
[`GetTempPath`](https://learn.microsoft.com/ja-jp/windows/win32/api/fileapi/nf-fileapi-gettemppathw)
�̂����b�ɂȂ��Ă��܂�.

[constructor](#cmainwnd-1) ����Ăяo����Ă��܂�.


## `SaveSettings`

[Setup](../README.md#setup) �̐ݒ�l���Z�[�u���܂�.

�\�[�X�R�[�h�������܂�܂�, ���ɂǂ����Ă��Ƃ̂Ȃ������ł�.

[`OnMenuSetup`](#onmenusetup) �Őݒ�ɕύX���������Ƃ���,
[`GetAuth`](#getauth) �ŐV���ȔF�ؕ����ɑ��������Ƃ��ɌĂяo����Ă��܂�.


## `LoadFilters`

[Filter](../README.md#filter) �ݒ�����[�h���܂�.

�\�[�X�R�[�h�������܂�܂�, ���ɂǂ����Ă��Ƃ̂Ȃ������ł�.

[constructor](#cmainwnd-1) ����Ăяo����Ă��܂�.


## `SaveFilters`

[Filter](../README.md#filter) �ݒ���Z�[�u���܂�.

�\�[�X�R�[�h�������܂�܂�, ���ɂǂ����Ă��Ƃ̂Ȃ������ł�.

[`OnMenuFilter`](#onmenufilter) �Őݒ�ɕύX���������Ƃ��ɌĂяo����Ă��܂�.


## `LoadTLDs`

[TLD ( Top Level Domain)](https://ja.wikipedia.org/wiki/�g�b�v���x���h���C��) ���������[�h���܂�.

�����ł����uTLD �����v�Ƃ�,
����܂łɎ�M�������Ƃ̂��郁�[���̔��M����
���ߍ��܂�Ă��������N�� URL �̃h���C���̃g�b�v ( �E�[ ) ���L�^���Ă��������̂ł�.
�V�����h���C�����烁�[�������邽�тɒǉ�����Ă����܂�.
( ������݂̃h���C���΂���ɂȂ����Ƃ��ɓ��ł��ɂȂ�܂�. )
[Filter](../README.md#filter) �� [Domain](../README.md#domain) ��ݒ肷��ۂ�,
�I������񎦂��邽�߂Ɏg���Ă�����ł�.

[constructor](#cmainwnd-1) ����Ăяo����Ă��܂�.


## `SaveTLDs`

[TLD ( Top Level Domain)](https://ja.wikipedia.org/wiki/�g�b�v���x���h���C��) �������Z�[�u���܂�.

`LoadTLDs` �ƈ���Ă�����������Ă΂�Ă��܂�.

| �Ăяo���� | �Ăяo���_�@ |
| --- | --- |
| [`OnMenuFilter`](#onmenufilter)	| `New Domain` �Ƃ��ĉ����ǉ����ꂽ��<br>�����ꂩ�̃h���C�����폜���ꂽ�� |
| [`GetFrom`](#getfrom)	| �V���ȃh���C�����烁�[���������� |
| [`CheckLink`](#checklink)	| �V���ȃh���C���ւ̃����N���������� |


## `MakeBlob`

�A�J�E���g���̈Í����E�����p�� [BLOB](https://ja.wikipedia.org/wiki/�o�C�i���E���[�W�E�I�u�W�F�N�g) �𐶐����܂�.

���łɐ����ς݂Ȃ牽�����Ȃ��̂�,
�{�i�I�� BLOB �𐶐�����̂�, �{�A�v�����񓱓����� 1�񂾂��ł�.
��������� BLOB �͖���Ⴄ���e�Ȃ̂�,
���ʓI�ɂ悻���܂��Í����������͕̂����ł��Ȃ��悤�ɂȂ��Ă��܂�.

[constructor](#cmainwnd-1) ����Ăяo����Ă��܂�.


## `EnDecrypt`

�A�J�E���g���̈Í����E�������s���܂�.

�Ȃ񂩂ǂ��������̖��O�ł���,
�Í������������r���܂ł͓����ߒ���ʂ�̂�, �������p�ɂ��������ł�.
�Ō�̈��� `bool bEncode` �łǂ�����s���̂����w�肵�܂�.
( `true` �ňÍ��� / `false` �ŕ��� )

�Ăяo������
[`LoadAccounts`](#loadaccounts) (����) ��
[`SaveAccounts`](#saveaccounts) (�Í���) �� 2�����ł�.

�߂�l�̓G���[�R�[�h��, ����I���Ȃ� `0` ��Ԃ��܂�.


## `OnPower`

�V�X�e�������f�܂��͍ĊJ���ꂽ�Ƃ��ɓ����n���h���[�ł�.

[`OnCreate`](#oncreate) ��
[`PowerRegisterSuspendResumeNotification`](https://learn.microsoft.com/ja-jp/windows/win32/api/powerbase/nf-powerbase-powerregistersuspendresumenotification)
���Ă�Ŏd�|�����n���h���[�ł�.
�����ł̓V�X�e���ĊJ���ɂ��݂₩�Ƀ��[�����`�F�b�N���邽�߂ɓ����Ă��܂�.
����, �V�X�e���ĊJ���シ���Ƀ��[����ǂ����Ƃ��Ă����s���邱�Ƃ��l������̂�,
���� ( �f�t�H���g�� `5`[�b] ) �҂��Ă���`�F�b�N���n�߂�悤�ɂ��Ă��܂�.


## `PollNow`

�u���v���[�����`�F�b�N���܂�.

�Ă΂���, ������ 1�񃁁[�����`�F�b�N���܂�.
<br>( �Ƃ͂����Ă��u���[�����`�F�b�N����^�C�}�[�v���������邾���ł���. )

��, 2��ڈȍ~�͐ݒ肳�ꂽ���� ( �f�t�H���g�� `10`[��] ) �ł̃`�F�b�N�𑱂��܂�.
�܂胁�[�����`�F�b�N����V���Ȏ����̎n�܂�ɂȂ�܂�.

�Ăяo�����͈ȉ��݂̂Ȃ���ł�.

| �Ăяo���� | �Ăяo���_�@ |
| --- | --- |
| [`OnTimer`](#ontimer)	| �^�C�}�[ `TID_START` �����������Ƃ� |
| [`OnUserTrayNotify`](#onusertraynotify) | �A�C�R�����N���b�N���ꂽ�Ƃ� |
| [`OnMenuAccounts`](#onmenuaccounts) | �A�J�E���g�o�^�ɕύX���������Ƃ� |
| [`OnMenuSetup`](#onmenusetup) | ���[���`�F�b�N�̎��ԊԊu�ɕύX���������Ƃ� |


## `PollMails`

���[�����`�F�b�N���܂�.

�܂��O�̂��ߑO��̃`�F�b�N�̌�Еt��

* [`ClosePOP`](#closepop) ��[POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol) �ڑ������.
* `m_iUser` �� `0` �Ƃ��āu�ŏ��̃A�J�E���g�v����n�܂�悤�ɂ���.
* `m_nMail` �� `0` �Ƃ��āu���ǃ��[�����v���`�����ɂ���.
* `m_strSummary` ����ɂ���, �u�T�}���[�v���`�����ɂ���.

�����Ă���, ����̃��[���`�F�b�N�̂��߂�

* [`ConnectPOP`](#connectpop) ���Ă�.

�Ƃ����i���ł�.

�u��? �ڑ����邾��?�v�Ǝv������������낤���Ƒ����܂���,
���̐ڑ����_�@�ɂȂ񂾂��񂾂Ń��[���̃`�F�b�N���i��,
���̊Ԃɂ��S�A�J�E���g�̃`�F�b�N���s���邱�ƂɂȂ�܂�.
<br>
<sub>
�������������I�ɒi���𗝉�����Ƃ��낪,
�u�񓯊��v�Ƃ������u[event driven](https://ja.wikipedia.org/wiki/�C�x���g�쓮�^�v���O���~���O)�v�Ƃ������umessage driven�v�Ȃ��̎����̓����Ȃ̂�������܂���.
</sub>

�Ăяo������ [`OnTimer`](#ontimer) �Ń^�C�}�[ `TID_POLL` �����������Ƃ��ł�.
�f�t�H���g�� `10`[��] �����ŌĂ΂�܂�.


## `ParseMail`

���[������͂��܂�.

1�̃��[���̒��g�̕�����������Ƃ��Ď󂯎��,
���̒��g����͂������ʂ� `true` ( �c���ׂ����[�� ) �� `false` ( �j�����ׂ����[�� ) �ŕԂ��܂�.

�Ăяo������ [`RespondPOP`](#respondpop) (�ʏ�c��) ��
[`ReadDebug`](#readdebug) (�f�o�b�O��) ��,
�����̖��ǃ��[�������܂��Ă���󋵂ł�, 1�̃��[���ɕt�� 1�񂸂Ă΂�܂�.

�����́u���g�̕�����v�̓��[���T�[�o�[�����M�����܂܂̕�����
( [`.eml`](https://www.google.com/search?q=.eml+�t�@�C��)���� ) ��,
�K�� 7bit �����R�[�h�L�q ( [UNICODE](https://ja.wikipedia.org/wiki/Unicode) �Ȃǂł͂Ȃ� ASCII ������ )
�Ȃ̂ň����̌^�� `CStringA`�ł�.
<br>
<sup>
`CStringW` ��ǂ����̖���������Ȃ� `CString` �ł͂���܂���.
</sup>

�d���̒i���͉��L�̂悤�ɂȂ��Ă��܂�.

1. [`GetAttr`](#getattr) �Ń��[���w�b�_�[�̍\���v�f�𔲂��o���ă`�F�b�N����.
1. [`MakeLog`](#makelog) �Ń��[���̃��O�t�@�C���ɃZ�[�u���镶����Ƀf�R�[�h����.
1. [`CheckWhiteList`](#checkwhitelist) �� [`Whitelist`](../README.md#whitelist) �Əƍ�����.
1. �j�����R�̂Ȃ����[���Ȃ�, [�T�}���[](../README.md#notification) �ɒǉ�����.
1. �j�����R�̂��郁�[���Ȃ�, �߂�l�Ƃ��� `�j�����ׂ����[��` ��I��.
1. [`�S�Ă�`���[�������O�ɗ��Ƃ��ݒ�](../README.md#log)��, �j���Ɣ��肵�����[���Ȃ�, [`SaveLog`](#savelog) �Ń��O�ɃZ�[�u����.
1. ���茋�ʂ�Ԃ�.

���̊֐������{�A�v���́u����v��,
���݂̂�Ȃ͂��́u����v���d�����ʂ�����悤���͂��Ă���Ă��鑶�݂ƌ����Ă������ł��傤.
<br>
<sup>
( ����, �ǂ̎q���������琬�藧���Ȃ��Ȃ邩�������̂Ȃ��q�����ł���. )
</sup>


## `GetAttr`

[���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)�ɂ���\���v�f�𔲂��o���ă`�F�b�N���܂�.

�Ƃ����Ă��������Ɋۓ������Ă��邾����, ���ۂ̎d��������Ă���͉̂��L�݂̂Ȃ���ł�.

| �ϑ���				| �Ɩ����e |
| ---				| --- |
| [`GetAuth`](#getauth)		| `Authentication-Results:` �ɂ���F�،��ʂ𔲂��o���ă`�F�b�N. |
| [`GetFrom`](#getfrom)		| `From:` �ɂ��鑗�M�� ( �̖��` ) �𔲂��o���ă`�F�b�N. |
| [`GetSender`](#getsender)	| `Authentication-Results:` �Ȃǂɂ��� ( ���ۂ� ) ���M�҂𔲂��o���ă`�F�b�N. |
| [`GetType`](#gettype)		| `Content-Type:` �ɂ��邠�ꂱ��𔲂��o���ă`�F�b�N. |
| [`GetEncode`](#getencode)	| `Content-Transfer-Encoding:` �ɂ���G���R�[�h�����𔲂��o��. |
| [`GetTime`](#gettime)		| `Date:` �ɂ�������𔲂��o���ă`�F�b�N. |
| [`CheckMID`](#checkmid)	| `Message-ID:` �ɂ��� ID ���}�g�����`�F�b�N. |

��L�ϑ��悩��W�߂�ꂽ���� `CAttr` �Ƃ������\���Ɏ��߂�,
�󒍐�� [`ParseMail`](#parsemail) �ɔ[�i���܂�.

������̈ϑ���������o���������ɃX�p���̒������������,
�[�i���ꂽ `CAttr` �Ɂu�j�����R�v�Ƃ��ĕ񍐂��グ�Ă����񑩂ɂȂ��Ă��܂�.


## `GetAuth`

[���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)��
`Authentication-Results:` �ɂ���
[�F��](https://ja.wikipedia.org/wiki/���M�h���C���F��)���ʂ𔲂��o���� `CAttr` �ɔ[�i���܂�.

�ߋ��̔F�ؗ��� ( `m_dwAuthCache` ) �ɂȂ��F�،��ʂ�����ꂽ�ꍇ��,
�F�ؗ����ɒǉ��̏�, [`SaveSettings`](#savesettings) �ɂčX�V���Ă����܂�.
( ���̓���������̂���, ���Ԃ�{�A�v��������ŏ��̃��[���ł����ł��傤����. )

�Ō��, �F�؂� pass ���Ȃ���������
[Authentication](../README.md#authentication) �Őݒ肳�ꂽ���e���𒴂��Ă��Ȃ����`�F�b�N��,
�����������Ă����炻�̎|�� `attr.m_strReason` �ɏ����ĕ񍐂��グ�܂�.

�n��Ƃ��Ă� [`GetAttr`](#getattr) �P����, ������̎󒍂͐��������Ă��܂���.


## `GetFrom`

[���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)��
`From:` �ɂ��鑗�M�� ( �̖��` ) �𔲂��o���� `CAttr` �ɔ[�i���܂�.

�u`From:` �ɂ���̂́g���`�h�Ȃ񂩂���Ȃ��āA�z���g�ɑ��M�҂̃��[���A�h���X����?�v
�Ǝv������������邩������܂���, ���� `From:` ���Ă��

1. �X�p�����[���̏ꍇ�͋U������Ă���ꍇ������.
1. ���[���z�M��s�Ǝ҂����M�������[���̏ꍇ��, �N���C�A���g��Ƃ̃A�h���X�ɂȂ��Ă���.

�Ƃ������Ƃ�, �u�X�p���΍�A�v���v�Ƃ��Ắu���`�v�Ɓu���M�ҁv�͕ʕ��Ƃ��Ĉ����Ă��܂�.
<br>
<sub>
���� 2. �̂������ŃX�p���΍����₱�������ƂɂȂ�܂���.
</sub>

�Ɩ����e�͈ȉ��̒ʂ�ł�.

1. �^����ꂽ������̒����� `From: ` �� `from: ` ��T��.<br>
<sup>
Q: ��? `from: ` �Ȃ�ď����Ă����������ł���? A: �����ł���. �Ƃ������̋Ǝ҂����ł���.
</sup>
1. Alias (�ʖ�) �̑��� `<` �� `>` �ň͂܂ꂽ�A�h���X������΂���� `attr.m_strFrom` �Ƃ��Ĕ[�i.

1. Alias (�ʖ�) ���Ȃ�������, `From: ` �ɓY���Ă���A�h���X�� `attr.m_strFrom` �Ƃ��Ĕ[�i.

1. `attr.m_strFrom` ��
[TLD ( Top Level Domain )](https://ja.wikipedia.org/wiki/�g�b�v���x���h���C��)�������o�̂��̂łȂ�������,
TLD �����ɒǉ�.

1. `attr.m_strFrom` ���u�u���b�N���X�g�v( [Domain](../README.md#domain) ��&#x2611;���ꂽ�A�h���X )
�Ɉ����������Ă��Ȃ��� [`CheckBlackList`](#checkblacklist) �ɂă`�F�b�N.

1. `attr.m_strFrom` �� [�h���C���𑕂������[���A�h���X](../README.md#sender) ���`�F�b�N.

��, �ӊO�Ƃ��낢�����Ă��܂�.

�`�F�b�N�̌���, �X�p���̒������������,
���̔��藝�R�� `attr.m_strReason` �Ƃ��ĕ񍐂��グ�܂�.

�n��Ƃ��Ă� [`GetAttr`](#getattr) �P���ł���,
[`OnGetSender`](#ongetsender) ����̎󒍂����Ȃ��Ă��܂�.


## `GetSender`

[���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)��
`Authentication-Results:` �Ȃǂɂ��� ( ���ۂ� ) ���M�҂𔲂��o���� `CAttr` �ɔ[�i���܂�.

�܂�, `Authentication-Results:` �ɂ���͂��� `smtp.mailfrom=` �Ƃ����������T���܂�.

`smtp.mailfrom=` ���������ꍇ: ( RFC 5451 ���� )

* �Y�����Ă��鑗�M�҂̃A�h���X�𔲂��o��,
�u���`�v( `attr.m_strFrom` ) �̃h���C�� ( `@` �ȍ~ ) �ƌ����v ( �E�����ׂē����� ) ���Ȃ����,
�u���`�v( `attr.m_strFrom` ) �Ƃ͕ʂ́u���ۂ̑��M�ҁv������Ɣ��f����,
`attr.m_strSender` �ɔ[�i.

`smtp.mailfrom=` ���Ȃ������ꍇ: ( RFC 5451 ���O )

* [���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)��
`Received: ` ���� `from ` �ɓY�����Ă���A�h���X�𔲂��o�����Ƃ��J��Ԃ�,
�Ō�̃A�h���X�� `attr.m_strSender` �ɔ[�i.

��, �������� RFC 5451 ���O�̌Â����[���̌`���ɂ��Ή����āu���ۂ̑��M�ҁv�𔲂��o���Ă��܂�.

������̏ꍇ��:

* `attr.m_strSender` ���u�u���b�N���X�g�v( [Domain](../README.md#domain) ��&#x2611;���ꂽ�A�h���X )
�Ɉ����������Ă��Ȃ��� [`CheckBlackList`](#checkblacklist) �ɂă`�F�b�N.

�Ƃ��ă`�F�b�N�̌���, �X�p���̒������������,
���̔��藝�R�� `attr.m_strReason` �Ƃ��ĕ񍐂��グ�܂�.

�n��Ƃ��Ă� [`GetAttr`](#getattr) �P���ł���,
[`OnGetSender`](#ongetsender) ����̎󒍂����Ȃ��Ă��܂�.


## `GetType`

[���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)��
`Content-Type:` �Ɋ�Â���, �ȉ��̏��𔲂��o���� `CAttr` �ɔ[�i���܂�.

* [���f�B�A�^�C�v](https://ja.wikipedia.org/wiki/Multipurpose_Internet_Mail_Extensions#Content-Type)
�� `text/plain` �� `text/html` ���邢�� `multipart` ��
( ����ȊO�͖��� )
* ���f�B�A�^�C�v�ɓY�����Ă��� `charset`
* ���f�B�A�^�C�v�� `multipart` �̏ꍇ�� part �Ԃ������� `boundary`

�[�i���, ���ꂼ��

* `attr.m_iType` �� `attr.m_iSubType`
* `attr.m_iCharset`
* `attr.m_strBoundary`

�Ƃ��Ă��܂�.

�Ȃ�, `charset` �Ɋւ��Ă�, ���� [code page](https://ja.wikipedia.org/wiki/�R�[�h�y�[�W) �𓾂��Ƃ�
[`GetCodePage`](#getcodepage) �ɑ������ŏo���Ă��܂�.

�n��Ƃ��Ă� [`GetAttr`](#getattr) �P����, ������̎󒍂͐��������Ă��܂���.


## `GetEncode`

[���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)��
`Content-Transfer-Encoding:` �ɂ���
[�G���R�[�h����](https://ja.wikipedia.org/wiki/Multipurpose_Internet_Mail_Extensions#Content-Transfer-Encoding)
�𔲂��o���� `CAttr` �ɔ[�i���܂�.

�n��Ƃ��Ă� [`GetAttr`](#getattr) �P����, ������̎󒍂͐��������Ă��܂���.


## `GetTime`

[���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)��
`Date:` �ɂ��鑗�M�����𔲂��o���� `CAttr` �ɔ[�i���܂�.

�[�i�� `attr.m_time` ��
[`CTime`](https://learn.microsoft.com/ja-jp/cpp/atl-mfc-shared/reference/ctime-class) class �ł���,
`Date:` �̋L�q��

`Sun, 15 Dec 2024 13:35:14 +0800`

�Ƃ������`���Ȃ̂�, ���Ȃ��J���ĕϊ����Ă��܂�.

�����, `attr.m_time` �� UTC �Ŋi�[���邱�Ƃɂ����̂� ( �������E������X�p�����[�����͂��̂� ),
UTC �ւ̕ϊ����K�v�ł�.
���̏����Ƃ��Ă�

1. `Date:` �L�q�̍Ō�ɂ���u���n���Ԃ� UTC ����ǂꂾ���}����Ă��邩�̋L�q�𔲂��o��.
1. �����[���v�P�ʂ̐��l�ɕϊ�����.
1. [`CTimeSpan`](https://learn.microsoft.com/ja-jp/cpp/atl-mfc-shared/reference/ctimespan-class) ������
`attr.m_time` �Ɂ}����.

�Ƃ������Ƃ�����Ă��܂�.

���܂� `+HHMM` �� `-HHMM` �`���ł͂Ȃ�, `GMT` �Ƃ� `PST` �Ƃ�,
[�^�C���]�[��](https://ja.wikipedia.org/wiki/������) �̗��̂����Ō����Ă��鍑�ۋ������̂Ȃ���(�n��)������܂���,
������
[`CTimeZones`](CTimeZones.md) �� [`GetBias`](CTimeZones.md#getbias) �֐��Ő��l�����Ă�����Ă��܂�.

�Ō��, `Date:` �̃^�C���]�[���� [Time Zone](../README.md#time-zone) ��&#x2611;����Ă��Ȃ����`�F�b�N��,
�����������Ă����炻�̎|�� `attr.m_strReason` �ɏ����ĕ񍐂��グ�܂�.

�n��Ƃ��Ă� [`GetAttr`](#getattr) �P����, ������̎󒍂͐��������Ă��܂���.


## `GetCodePage`

[���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)��
`charset=` �ɂ��� [character set](https://ja.wikipedia.org/wiki/�����R�[�h#�ގ��̗p��)
�� [code page](https://ja.wikipedia.org/wiki/�R�[�h�y�[�W)���ʎq�ɕϊ������l��Ԃ��܂�.

�Ȃ�[code page ���ʎq](https://learn.microsoft.com/ja-jp/windows/win32/intl/code-page-identifiers)�Ȃ̂��Ƃ�����,
���Ƃ�
[`MultiByteToWideChar `](https://learn.microsoft.com/ja-jp/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar)
�� UNICODE �ϊ�����Ƃ��ɖʓ|���Ȃ�����ł�. ( [`StringFromCodePage`](#stringfromcodepage) �Q�� )

���Ƃ��� `"utf-8"` �Ƃ����������񂩂� code page `65001` �ւ̕ϊ����s���킯�ł���,
�ǂ̕����񂪂ǂ� code page �Ȃ̂��ϊ��\�����̂��ʓ|�Ȃ̂�,

`HKEY_CLASSES_ROOT\MIME\Database\Charset\`

�̔z���ɕ���ł���e charset ���Q�Ƃ���, code page ���ʎq�̒l�ɕϊ����Ă��܂�.

���ɂ͒��� code page �̒l�ɕϊ��ł��Ȃ� charset �́u�ʖ��v���Ă̂�����܂���
( ��: `us-ascii` �� `iso-8859-1` ),
���̏ꍇ�̓I���W�i���̖��O�Ŏ��g���ċN�Ăяo������ code page ���ʎq�̒l�ɕϊ����Ă��܂�.

�n��Ƃ��Ă� [`GetAttr`](#getattr) �P����[`GetType`](#gettype) �̑������ł���,
���[���w�b�_�[�̃f�R�[�h�ɂ������d�����K�v�Ȃ���,
[`StringFromHeader`](#stringfromheader) ����̔��������������Ă��܂�.


## `CheckMID`

[���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)��
`Message-ID:` �ɂ��� Message ID
���`�F�b�N�������ʂ� `CAttr` �ɔ[�i���܂�.

�n��Ƃ��Ă� [`GetAttr`](#getattr) �P����, ������̎󒍂͐��������Ă��܂���.


## `CheckAlias`

�u�ʖ��v( [Sender](../README.md#sender) �́u�ʖ� ( Alias )�v�֌W )
�Ɉ����������Ă��邩�`�F�b�N���܂�.

���̊֐����

* �ʖ� ( Alias ) ���x�������[���A�h���X
* ���ʂȕʖ� ( Alias ) �ŌĂт����Ă��郁�[��

�̗������`�F�b�N���Ă��܂�.

�Ăяo������, [`MakeLog`](#makelog) ��,
[`GetAttr`](#getattr) �P���ł͂���܂���.
[`MakeLog`](#makelog) �P���Ńf�R�[�h���ς񂾃��[���w�b�_�[����
`From:` �� `To:` �𔲂��o���ă`�F�b�N���܂�.

## `CheckBlackList`

�u�u���b�N���X�g�v( [Domain](../README.md#domain) ��&#x2611;���ꂽ�A�h���X )
�Ɉ����������Ă��邩�`�F�b�N���܂�.

�Ăяo������, [`GetFrom`](#getfrom) �� [`GetSender`](#getsender) �� 2������,
���ꂼ��u���`�v�Ɓu���ۂ̑��M�ҁv���u�u���b�N���X�g�v�Ɉ����������Ă��Ȃ������`�F�b�N���Ă��܂�.
( ����[�ݒ�](../README.md#domain)�Ɋ�Â���[`CheckLink`](#checklink)�ł��Ǝ��Ƀ����N��̃`�F�b�N���s���Ă��܂�. )

�Ăяo������[`GetFrom`](#getfrom)��[`GetSender`](#getsender)��,
���[���́u���`�v( `From:` ) �Ɓu���ۂ̑��M�ҁv�̗������������Ă��܂�.


## `CheckWhiteList`

�u�z���C�g���X�g�v( [Whitelist](../README.md#whitelist) �ɓo�^���ꂽ�u���`�v�Ɓu���M�ҁv )
�Ɋ܂܂�Ă��邩�`�F�b�N���܂�.

�܂܂�Ă����ꍇ�� `attr.m_strReason` ����ɂ��܂�. ����ɂ�荡��̏����ł́u�X�p�����[���v�������ꂸ,
�������Ă��炦��悤�ɂȂ�܂�.

�Ăяo������[`ParseMail`](#parsemail)�݂̂ł�.


## `CheckLink`

���ߍ��܂�Ă��郊���N�� URL ���`�F�b�N���܂�.

�`�F�b�N�̓��e��

* URL �L�q�Ɂu���I�ȕ����R�[�h�v���܂܂�Ă��Ȃ���.
* URL �� [Domain](../README.md#domain) ��&#x2611;���ꂽ�h���C���ɑ����Ă��Ȃ���.

�ƂȂ��Ă��܂�.
�`�F�b�N�Ɉ������������ꍇ�� `CAttr` �Ɂu�j�����R�v�Ƃ��Ă��̎|�������ĕ񍐂��グ�܂�.

[`StringFromBody`](#stringfrombody) �݂̂���Ă΂�Ă��܂�.


## `CheckUnicode`

UNICODE ( [UTF-16LE](https://ja.wikipedia.org/wiki/UTF-16) ) ��������`�F�b�N���܂�.

[Coding](../README.md#coding) �ɋ�����
�u�^�킵�����䕶���v��u���I�ȕ����R�[�h�v�̗L�����`�F�b�N���Ă��܂�.
�Y�����镶�����������ꍇ�� `CAttr` �Ɂu�j�����R�v�Ƃ��Ă��̎|�������ĕ񍐂��グ�܂�.

[`StringFromCodePage`](#stringfromcodepage) ����Ă΂�Ă��܂�.


## `FilterError`

�^����ꂽ�u�j�����R�v�����[���̔j���ɑ������邩���肵�܂�.

[Filter](../README.md#filter) �̊e��ݒ�������Ƃ��ė^����ꂽ�u�j�����R�v�̃R�[�h�ŎQ�Ƃ�,
���ۂɃ��[����j�����邩�ǂ����𔻒肵�܂�.

�j�����ׂ��Ƃ̔��肪�������ꍇ�́u�j�����R�v�̃R�[�h�𕶎��񉻂�,
���̕�������u�j�����R�v��ێ����Ă��� `attr.m_strReason` �ɒǋL���܂�.
�u�ǋL���܂��v�ƌ����Ă���悤��, 1�̃��[���������́u�j�����R�v�Ɉ��������邱�Ƃ�����܂�.

���̊֐��͖{ class �̂�����������Ă΂�Ă��܂�.


## `MakeLog`

���O�t�@�C���ɏ����o������������܂�.

1�̃��[���̒��g�̕�����������Ƃ��Ď󂯎��, ���̒��g���f�R�[�h�������ʂ� UNICODE ������ŕԂ��܂�.

�����́u���g�̕�����v�̓��[���T�[�o�[�����M�����܂܂̕�����
( [`.eml`](https://www.google.com/search?q=.eml+�t�@�C��)���� ) ��,
�K�� 7bit �����R�[�h�L�q ( [UNICODE](https://ja.wikipedia.org/wiki/Unicode) �Ȃǂł͂Ȃ� ASCII ������ )
�Ȃ̂ň����̌^�� `CStringA`�ł�.
���, �Ԃ�������̌^�� `CString` �ł�.
<br>
<sup>
`CStringW` �Ƃ��� `CString` �Ƃ��Ă���̂�, ���� project �͕K�� UNICODE �� build ���Ă�������.<br>
( �Ƃ������Ă銄�ɂ� `_T()` �Ȃ�� multibyte �Ƃ̋�������̌ÏL���}�N����A�����Ă܂���. )
</sup>

�d���̓��e�́F

1. �܂��^����ꂽ���[���̑S����,�u���[���w�b�_�[�v�Ɓu�{���v�ɕ�����.
1. [`StringFromHeader`](#stringfromheader) �Łu���[���w�b�_�[�v���f�R�[�h.
1. [`StringFromBody`](#stringfrombody) �Łu�{���v���f�R�[�h.
1. �j�����R `attr.m_strReason` ������ꍇ��, ���̗��R�̕�����Ɩ��ߍ��܂�Ă��������N��`���ɋ���.
1. �������Ċ��������������Ԃ�.

�ƂȂ��Ă��܂���, ���̂��� 3. �́u�{���v�̈����� `multipart` �̃��[���ɑ΂��Ă�, part ���Ƃɍs���܂�.

���̎d���̌�,
[`CheckWhiteList`](#checkwhitelist) �� [`Whitelist`](../README.md#whitelist) �ɓo�^����Ă������[���͋~�ς���܂���,
���̎d���ŕt�����ꂽ�u�j�����R�v�̓��O�Ɏc�����܂܂ɂȂ�܂�.
����́u�`�F�b�N�̕s���Ō������Ă��܂����v�̂��u[`Whitelist`](../README.md#whitelist)�Ō������Ă����������v�̂�,
�������ɂ����Ȃ�̂�����邽��, �̈ӂɎc���Ă�����̂ł�.

[`ParseMail`](#parsemail) ����Ă΂�Ă��܂�.


## `SaveLog`

���O�t�@�C�����Z�[�u���܂�.

�u[`MakeLog`](#makelog) �ŏ������ޕ�������m�肵����, ���Ƃ̓t�@�C���ɏ������ނ����ł���?�v
�Ǝv���邩������܂���, �ӊO�Ɩʓ|�Ȏd�����c���Ă��܂�.

�d���̓��e�́F

1. �Z�[�u��̃t�H���_�[�����݂��Ȃ��ꍇ��, ���̃t�H���_�[�����. ( �{�A�v����������� 1�����ł��傤��. )
1. [���[���̑��M����](#gettime)�Ƀ��[�U�[�̌��n�������������� UTC �Ƃ���.
1. �j�����R�̂��郁�[����������, �t�@�C�����̐擪�� `!` ������ł���.
1. ���ʓI�ɓ��� `.eml` �t�@�C�����������Ƃ��Ă���̂���[���f](#isduplicated)��, �����Ȃ�Z�[�u����̂���߂Ă���.
1. �K�v�ɉ����ăt�@�C�����ɃT�t�B�b�N�X ( ����t�@�C�����ɂ��� `(2)` �Ȃǂ̔ԍ� ) ��[�t�^](#addsuffix)����.
1. [BOM](https://ja.wikipedia.org/wiki/�o�C�g���}�[�N) �̂Ȃ� ASCII �̃e�L�X�g�t�@�C���Ƃ��� `.eml` ����������.
1. �t�@�C���̃^�C���X�^���v��, [���[���̔��M����](#gettime)�ɍ��킹��.
1. �j�����R�̂��郁�[����������, �t�@�C�����̐擪�� `!` ������ł���.
1. ���ʓI�ɓ��� `.txt` �t�@�C�����������Ƃ��Ă���̂���[���f](#isduplicated)��, �����Ȃ�Z�[�u����̂���߂Ă���.
1. �K�v�ɉ����ăt�@�C�����ɃT�t�B�b�N�X ( ����t�@�C�����ɂ��� `(2)` �Ȃǂ̔ԍ� ) ��[�t�^](#addsuffix)����.
1. [BOM](https://ja.wikipedia.org/wiki/�o�C�g���}�[�N) ��t���� UTF-8 �̃t�@�C���Ƃ��� `.txt` ����������.
1. �t�@�C���̃^�C���X�^���v��, [���[���̔��M����](#gettime)�ɍ��킹��.
1. [`TrimFiles`](#trimfiles) �� `*.eml` �� `*.txt` �̃t�@�C�����𒲐�����.

�Ƃ��Ă��܂�.

`.eml` �� `.txt` ��, ��������g���q�̑O�̃t�@�C������ `From:` ���甲���o�������[���A�h���X�ł�.
�Ȃ̂œ������M�����畡���̃��[����������ƃt�@�C�������J�u��킯�ł���,
[`AddSuffix`](#addsuffix) �̏������؂��, �J�u��Ȃ��悤�ɂ��Ă��܂�.

�������� 7. �� 12. �Ńt�@�C���̃^�C���X�^���v�����[���̑��M�����ɍ��킹�Ă������̂�,
Windows&reg; ����Ȃ��ɂ��Ă��܂����Ƃ�[�`���̘A������](#�����g��-build-����-debug-�������ւ̘A������)�ł��`�������ʂ�ł�.

[`ParseMail`](#parsemail) ����Ă΂�Ă��܂�.


## `IsDuplicated`

���łɓ����t�@�C�������邩�ǂ����`�F�b�N���܂�.

��{�I�ɂ́u�����t�@�C�����œ����^�C���X�^���v�̃t�@�C�������邩�ǂ����v���`�F�b�N����̂ł���,
�{�A�v���ł͓������M�����畡����̑��M�������,
2��ڂ� `(2)`, 3��ڂ� `(3)`�c�c��, �u�T�t�B�b�N�X�v���t�@�C�����ɕt���Ă��܂�.
�����Ƃ��ƍ����K�v�Ȃ̂�, �t�H���_�����t�@�C���������Č��ʂ����߂Ă��܂�.

�߂�l�́u���łɓ����t�@�C��������v�ꍇ�� `true`, �u�����t�@�C���͂Ȃ��v�ꍇ�� `false` ��Ԃ��܂�.

[`SaveLog`](#savelog) ����Ă΂�Ă��܂�.


## `AddSuffix`

���łɓ������O�̃t�@�C��������ꍇ��, �t�@�C�����Ɂu�T�t�B�b�N�X�v��t���܂�.

�����ł����u�T�t�B�b�N�X�v�Ƃ�, �t�@�C�����̖���, �g���q�̎�O�ɋ��� `(2)` �� `(3)` �Ȃǂ̔ԍ�������ł�.
�t�H���_������������, �ł��傫�Ȕԍ��Ɂ{1�����u�T�t�B�b�N�X�v�����񂾃t�@�C������Ԃ��܂�.
�������O�̃t�@�C�����Ȃ������ꍇ��, �I���W�i���̂܂܂̃t�@�C������Ԃ��܂�.

[`SaveLog`](#savelog) ����Ă΂�Ă��܂�.


## `TrimFiles`

���O�t�@�C���̐��𐧌����܂�.

[`SaveLog`](#savelog) �ŃZ�[�u�������O�t�@�C���̌���
[Setup](../README.md#setup) �Ŏw�肳��Ă���t�@�C�����ɗ}���܂�.

�����𒴂�������, ���M�����̌Â�������t�@�C���������Ă����܂�.
( ���̂��߂Ƀ��O�t�@�C���̃^�C���X�^���v�����[���́u���M�����v�ɍ��킹�Ă���Ƃ������܂�. )

���̊֐���, [`SaveLog`](#savelog) ����Ă΂�Ă���̂͂������,
[`OnMenuSetup`](#onmenusetup) ������t�@�C���������炳�ꂽ�ꍇ�ɔ����ČĂ΂�Ă��܂�.


## `MoveFiles`

�w�肳�ꂽ�t�H���_�[�̒��g�������z���܂�.

�����Ƃ��ė^����ꂽ���t�H���_�[�ƐV�t�H���_�[�̊Ԃ�,
�t�H���_�[�̉��̃t�@�C���������z���܂�.
�����z�����I������狌�t�H���_�[���폜���悤�Ƃ��܂�.

���O�t�@�C���̃t�H���_�[�p�̊֐��Ȃ̂�,
���ɍċA�I����͂��Ă��܂���.
�t�H���_�[�̒����Ɉڂ��ׂ��t�@�C���͑S�đ����Ă���Ƃ����O��ł�.

[`OnMenuSetup`](#onmenusetup)����,
���O�����܂��Ă����t�H���_�[���ύX���ꂽ�ۂɌĂяo����܂�.


## `MakeSummary`

[�T�}���[](../README.md#notification)�̂��߂̕�����𐶐����܂�.

1�ʂ̃��[���̃��O�o�͗p������ ( �܂�f�R�[�h�ςݕ����� ) �� `CAttr` ��������,
�T�}���[�Ƃ��ĕ\�����邽�߂ɘA�������������Ԃ��܂�.

`Date:` �̎����\����
[`GetTimeZoneInformation`](https://learn.microsoft.com/ja-jp/windows/win32/api/timezoneapi/nf-timezoneapi-gettimezoneinformation)
��p���ă��[�U�[�̌��n���Ԃɕϊ���,
�����[�U�[�� Windows&reg; �ɐݒ肵�����t�̕\���t�H�[�}�b�g�ɉ����悤,
[`GetDateFormatEx`](https://learn.microsoft.com/ja-jp/windows/win32/api/datetimeapi/nf-datetimeapi-getdateformatex)
��p���ē��t������𐶐����Ă��܂�.

���̌���, �ł������镶�����

`"Subject:\nFrom:\n Date:\n\n"`

�ƂȂ�܂�.

�Ăяo������ [`ParseMail`](#parsemail) ��,
���[��1�ʂ��̃T�}���[�������S���[�����A�����Ă��܂�.


## `ShowSummary`

[�T�}���[](../README.md#notification)�������[�ʒm�p window](CNotifyWnd.md)�Ɏd���݂܂�.

�S�Ă̖��ǃ��[���̃T�}���[�����񂪘A���������̂�������,
[�T�}���[�\��](../README.md#notification)���s���܂�.

�\�����鍀�ڂ͂��ꂼ��̃��[���� `Subject:`, `Date:`, `From:` �ł���,
�������܂Ƃ߂ĂȂ�ׂ��R���p�N�g��, ���������₷���\�����邽��,
���ꂼ��̕�����Ƃ��Ă̒��������L�̂悤�ɍl�@���ĕ\���ʒu�����߂܂���.

| ���� | ���� | �\���ʒu |
| --- | --- | --- |
| `Date:` | �Z�� | ��i�̍��� |
| `From:` | ���Ԃ� `Subject:` ���͒Z�� | ��i�̉E�� |
| `Subject:` | �������Ƃ����� | ���i�S�� |

��L�̕\���ʒu�ƂȂ�悤, ���L�̂悤�Ɉ������������בւ��Ă��܂�.

`"Date:1\t:From:1\nSubject:1\n\nDate:2\t:From:2\nSubject:2\n\n"`

�������ĕ��בւ����������[�ʒm�p window](CNotifyWnd.md)��[`SetText`](#settext)�Ɉ����n���܂�.
�����[�T�}���[�\��](../README.md#notification)�� window �������邱�ƂɂȂ�܂�.

�Ăяo������ [`ModNI`](#modni) ��,
���� [`ModNI`](#modni) �͑S�ẴA�J�E���g�̃��[���`�F�b�N�����������Ƃ���
[`RespondPOP`](#respondpop) ����Ă΂�Ă��܂�.


## `ShareSummary`

[�T�}���[](../README.md#notification)���������񋤗L���܂�.

�S�Ă̖��ǃ��[���̃T�}���[�����񂪘A���������̂����L�p
[�t�@�C���}�b�s���O](https://learn.microsoft.com/ja-jp/windows/win32/memory/file-mapping)
�Ɏd���݂܂�.

���炩���ߌ����Ă����܂���,
���̋@�\�̓f�t�H���g�� OFF �ɂȂ��Ă��܂�.
ON �ɂ����i�͓��A�v���ł͒񋟂��Ă��炸,
���[�U�[���珊��� registry �� Registry Editor ���g���� ON �ɏ��������邵������܂���.
�ꕔ�̃}�j�A�b�N�ȃ��[�U�[�̂��߂̋@�\�ƌ����Ă����ł��傤.
�ǂ������}�j�A�b�N�ȃ��[�U�[����̎��v���������ƌ�����:

* ���[���̃`�F�b�N�͘A���^�]���Ă���T�[�o�[�ł���Ă���.
* ���̃T�[�o�[�ɂ͕��i�� console ���Ȃ��ł��Ȃ�.
* ���̑���ɏ����� USB ���j�^�[��t���Ă���.
* USB ���j�^�[�̉�ʂ͏󋵕\���̂��߂̃A�v���Ő�L���Ă���.

�Ƃ��������������[�U�[�ł���.

��, ���́u��ʂ��L���Ă���A�v���v�ł����[���̓����󋵂�\������������,
�{�A�v�����T�}���[�\�����Ă��镶���� process �Ԃœǂ߂�悤�ɂ��Ăق���,
�Ƃ������v�ł�.

ON �ɂ����,
`"ChkMails Summary"` �Ƃ������O��
[�t�@�C���}�b�s���O�I�u�W�F�N�g](https://learn.microsoft.com/ja-jp/windows/win32/memory/creating-a-file-mapping-object)
�����, �T�}���[�����񂪎擾�ł���悤�ɂȂ�܂�.

�Ăяo������[�uNI�O�Z��v](#addni)��, �T�}���[�̕����񂪕ύX�����ۂɌĂяo����Ă��܂�.


## `StringFromHeader`

[���[���w�b�_�[](https://www.dekyo.or.jp/soudan/contents/ihan/header.html)�̕�������f�R�[�h�����������Ԃ��܂�.

���[���w�b�_�[�ɍڂ����Ă��邢�����̕������,
ASCII �R�[�h�ŕ\���ł�����e�𒴂��������\���̂��߂ɃG���R�[�h����Ă���ꍇ������܂�.

���Ƃ���
```
From: =?utf-8?B?5LiJ5LqV5L2P5Y+L6YqA6KGM?= <admin@propertyagent.co.jp>
To: <pochi.the.cat@provider.ne.jp>
Subject: =?utf-8?B?44CQ5LiJ5LqV5L2P5Y+L44CRU01CQ+OCq+ODvOODieOCouOCq+OCpuODs+ODiOOBruWGjQ==?=
	=?utf-8?B?6KqN6Ki844GM5b+F6KaB44Gn44GZXyM3NzI3Nw==?=
Message-ID: <20241202023811304176@moneylionkmp.top>
```
�� Base64 �ŃG���R�[�h���ꂽ UTF-8 �̕������, ������f�R�[�h�����
```
From: �O��Z�F��s <admin@propertyagent.co.jp>
To: <pochi.the.cat@provider.ne.jp>
Subject: �y�O��Z�F�zSMBC�J�[�h�A�J�E���g�̍ĔF�؂��K�v�ł�_#77277
Message-ID: <20241202023811304176@moneylionkmp.top>
```
�ƂȂ�܂�.
<sub>
<br>
����͎��ۂɓ����ɓ͂����X�p�����[���̗�ł���,
`From:` �����S�ɂł����グ�Ȃ��Ƃ��������Ă�����̂ł�.
`propertyagent.co.jp` �Ƃ������݂̊�Ƃ̃A�h���X���x���Ă���Ƃ��낪���f�Șb�ł���,
`�O��Z�F��s` �̃A�h���X�ł͂Ȃ��̂���ڗđR�ȂƂ���͏΂��܂�.
</sub>

���[���w�b�_�[���̗v�f�R�[�h������, ��ʌ`�ŕ\����

`=?<charset>?{B|Q}?<encoded string>?=`

�Ƃ������[���� ASCII �����񉻂���Ă��܂�.<br>
`=?` ���v�f�R�[�h�����̎n�܂������,<br>
`<charset>` ��[character set](https://ja.wikipedia.org/wiki/�����R�[�h#�ގ��̗p��),<br>
`{B|Q}` �� `B` ��
[Base64](https://ja.wikipedia.org/wiki/Base64), `Q` ��
[Quoted-printable](https://ja.wikipedia.org/wiki/Quoted-printable) �ŃG���R�[�h����Ă��邱�Ƃ�����, �ȍ~
`?=` �܂ł��G���R�[�h���ꂽ character set �̕�����ł�.<br>
�Ȃ̂�, ��̗���������:

| �v�f�R�[�h�����̖`�� | �v�f�R�[�h�����̒��g |
| --- | --- |
| `=?UTF-8?B?` | Base64 �ŃG���R�[�h���ꂽ [UTF-8](https://ja.wikipedia.org/wiki/UTF-8) �̕����� |
| `=?utf-8?q?` | Quoted-printable �ŃG���R�[�h���ꂽ UTF-8 �̕����� |
| `=?ISO-2022-JP?B?` | Base64 �ŃG���R�[�h���ꂽ [ISO-2022-JP](https://ja.wikipedia.org/wiki/ISO-2022-JP) �̕����� |

�ȂǂƂ��������ƂɂȂ�܂�.

�ȏ�̑O��𓥂܂��ď������e����������: <sub>�O�u������!</sub>

1. �{���� character set ������ `attr.m_iCharset` ��ޔ�.
1. `"=?"` ��������Ȃ������� 14. ��.
1. `<charset>` �����𔲂��o��,
1. [`GetCodePage`](#getcodepage)�� `<charset>` ����[code page](https://ja.wikipedia.org/wiki/�R�[�h�y�[�W)�l�𓾂�.
1. `attr.m_iCharset` �𓾂�ꂽ code page �ŏ㏑��.
1. ���ߕs���� `<charset>` ��������, [�ǂ߂Ȃ������Z�b�g](../README.md#coding)�Ƃ��ăG���[��.
1. `<charset>` �̎��� `Q` ��������, �G���R�[�h������ Quoted-printable �Ƃ݂Ȃ�.
1. `<charset>` �̎��� `Q` �ł͂Ȃ�������, �G���R�[�h������ Base64 �Ƃ݂Ȃ�.
1. �r���ɋ��܂������s�͖�������, �v�f�R�[�h�����𔲂��o��.
1. �G���R�[�h������ Quoted-printable ��������[`DecodeQuoted`](#decodequoted)�Ńf�R�[�h.
1. �G���R�[�h������ Base64 ��������[`DecodeBase64`](#decodebase64)�Ńf�R�[�h.
1. �f�R�[�h���ꂽ code page �̕������ [`StringFromCodePage`](#stringfromcodepage)��
`CString` ( [UTF-16LE](https://ja.wikipedia.org/wiki/UTF-16) ) �ɕϊ�.
1. �ȏ�� 2. �܂ŌJ��Ԃ�.
1. �ޔ����Ă����� `attr.m_iCharset` �𕜋A.
1. �ł��������������� ( `CString` ) ��Ԃ�.

�Ƃ����i���ɂȂ��Ă��܂�.

[`MakeLog`](#makelog)����Ăяo����Ă��܂�.


## `StringFromBody`

���[���{���̕�������f�R�[�h�����������Ԃ��܂�.

�������e��:

1. �������� `attr.m_iType` ( [���f�B�A�^�C�v](https://ja.wikipedia.org/wiki/Multipurpose_Internet_Mail_Extensions#Content-Type)
)�� `text/plain` �� `text/html` �łȂ���΋�̕������Ԃ��ďI���.<br>
( `multipart` �̏ꍇ�� part ���ƂɌĂ΂��ʒu�t���̊֐��Ȃ̂� )
1. `attr.m_iEncode` �� `���G���R�[�h` �� `Binary` �� `7bit` �� `8bit` �Ȃ�f�R�[�h���Ȃ�.
1. `attr.m_iEncode` �� `Base64`�Ȃ�[`DecodeBase64`](#decodebase64)�Ńf�R�[�h.
1. `attr.m_iEncode` �� `Quoted-printable`�Ȃ�[`DecodeQuoted`](#decodequoted)�Ńf�R�[�h.
1. `attr.m_iEncode` ���f�R�[�h�ςݕ�����͋���ۂ̂܂܂ɂ��Ă���.
1. [`StringFromCodePage`](#stringfromcodepage) �Ńf�R�[�h�ςݕ������
`CString` ( [UTF-16LE](https://ja.wikipedia.org/wiki/UTF-16) ) �ɕϊ�.
1. [`LFtoCRLF`](#lftocrlf) �� LF ���s�� CR/LF ���s�Ɉꊇ�ϊ�.
1. `attr.m_iSubType` �� `HTML` �Ȃ�,<br>
[`HexToUnicode`](#hextounicode) ��16�i���\�L�𕶎���.<br>
[`CheckLink`](#checklink) �Ŗ��ߍ��܂�Ă��郊���N�� URL ���`�F�b�N.
1. �ł��������������� ( `CString` ) ��Ԃ�.

�Ƃ����i���ɂȂ��Ă��܂�.

[`MakeLog`](#makelog)����Ă΂�Ă��܂�.


## `DecodeBase64`

[Base64](https://ja.wikipedia.org/wiki/Base64)�ŃG���R�[�h���ꂽ��������f�R�[�h���܂�.

�߂�l�� `CStringA` �ł���,
������[�}���`�o�C�g������](https://ja.wikipedia.org/wiki/�}���`�o�C�g����)�Ƃ��Ĉ����܂�.
7bit �����Ƃ͌���܂���. MSB �������������R�[�h���܂܂꓾�邱�ƂɂȂ�܂�.

�������e�Ƃ��Ă�,
�u6bit x 4���� ( 6 x 4 = 24 bits ) �� 8bit x 3bytes ( 8 x 3 = 24 bits ) ��\���v�Ƃ���
[Base64](https://ja.wikipedia.org/wiki/Base64)�̗��V�ɑ�����,
�f�R�[�h���Ă��邾���ł�.
�Ȃ񂩂��ɂ傲�ɂ����Ă܂���, ��[������Ƃ������ꂾ���̏������Ƃ������Ƃ������肢��������Ǝv���܂�.

[`StringFromHeader`](#stringfromheader) �� [`StringFromBody`](#stringfrombody) ����Ă΂�Ă��܂�.


## `DecodeQuoted`

[Quoted-printable](https://ja.wikipedia.org/wiki/Quoted-printable) �ŃG���R�[�h���ꂽ��������f�R�[�h���܂�.

�߂�l�� `CStringA` �ł���,
������[�}���`�o�C�g������](https://ja.wikipedia.org/wiki/�}���`�o�C�g����)�Ƃ��Ĉ����܂�.
7bit �����Ƃ͌���܂���. MSB �������������R�[�h���܂܂꓾�邱�ƂɂȂ�܂�.

�������e�Ƃ��Ă�,

* `=` ����������, ���ꂪ�s���łȂ�����㑱��16�i���\�L�𐔒l�ϊ����ăf�R�[�h�ςݕ�����ɒǉ�
* `=` �ȊO�̕�����, ���̂܂܃f�R�[�h�ςݕ�����ɒǉ�

�Ƃ��Ă��邾���ł�.

[`StringFromHeader`](#stringfromheader) �� [`StringFromBody`](#stringfrombody) ����Ă΂�Ă��܂�.


## `StringFromCodePage`

�w��
[code page ���ʎq](https://learn.microsoft.com/ja-jp/windows/win32/intl/code-page-identifiers)
��
[�}���`�o�C�g������](https://ja.wikipedia.org/wiki/�}���`�o�C�g����)����
`CStringW` ( [UTF-16LE](https://ja.wikipedia.org/wiki/UTF-16) ) �ɕϊ������������Ԃ��܂�.

�v��
[`MultiByteToWideChar`](https://learn.microsoft.com/ja-jp/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar)
���g����,
[�}���`�o�C�g������](https://ja.wikipedia.org/wiki/�}���`�o�C�g����)��
`CStringW` ( [UTF-16LE](https://ja.wikipedia.org/wiki/UTF-16) ) �ɕϊ����Ă��邾���ł���,<br>
�O�����Ƃ���:

* code page �� [`ISO_2022_JP`](https://ja.wikipedia.org/wiki/ISO-2022-JP) ��������,
[`EscapeFromJIS`](#escapefromjis)�ɂ�
[�G�X�P�[�v�V�[�P���X](https://ja.wikipedia.org/wiki/�G�X�P�[�v�V�[�P���X)�̑O�������s���Ă���.

�㏈���Ƃ���:

* ���o�������R�[�h���܂܂�Ă��Ȃ���[`CheckUnicode`](#checkunicode)�Ń`�F�b�N.

�����܂��Ă��܂�.

�܂�,
�ϊ��Ɏ��s�����ꍇ�� `CAttr` �Ɂu�j�����R�v�������ĕ񍐂��グ�邱�Ƃɂ��������Ȃ��Ă��܂�.

[`StringFromHeader`](#stringfromheader) �� [`StringFromBody`](#stringfrombody) ����Ă΂�Ă��܂�.


## `EscapeFromJIS`

[�G�X�P�[�v�V�[�P���X](https://ja.wikipedia.org/wiki/�G�X�P�[�v�V�[�P���X)�ŏC�����ꂽ
JIS ��������G�X�P�[�v�V�[�P���X�̂Ȃ� Shift-JIS ������ɕϊ����܂�.

* ���p�J�i�͑S�p�J�i�ɕϊ����Ă��܂�.<br>( ���łɔ��p���_����������΂悩������ł�����, �����܂ł��C���c�c )
* ������ [`_mbcjistojms`](https://learn.microsoft.com/ja-jp/cpp/c-runtime-library/reference/mbcjistojms-mbcjistojms-l-mbcjmstojis-mbcjmstojis-l)
���g���� Shift-JIS ������ɕϊ����Ă��܂�.

`CString` ( [UTF-16LE](https://ja.wikipedia.org/wiki/UTF-16) ) �ɕϊ�����̂ł͂Ȃ�,
��U Shift-JIS ���o�R���Ă��܂�.
[`_mbcjistojms`](https://learn.microsoft.com/ja-jp/cpp/c-runtime-library/reference/mbcjistojms-mbcjistojms-l-mbcjmstojis-mbcjmstojis-l)
�Ƃ��� API �𗘗p���邽��, �Ƃ����̂�����܂���,
���������\�[�X�R�[�h���̃J�i�����e�[�u���� Shift-JIS �ŋL�q����Ă���̂�,
�ǂ����Ă� Shift-JIS �o�R�ɂȂ炴��𓾂܂���.
<br>
<sup>
( �\�[�X�R�[�h�� UTF-16LE �ŏ����΂悩�����̂�? �ł�, ����Ȃ��Ƃ�������t�@�C���T�C�Y���c�c)
</sup>

[`StringFromCodePage`](#stringfromcodepage) �݂̂���Ă΂�Ă��܂�.


## `StringToUTF8`

`CString` ( [UTF-16LE](https://ja.wikipedia.org/wiki/UTF-16) ) ��
[UTF-8](https://ja.wikipedia.org/wiki/UTF-8) ������ɕϊ����܂�.

[`WideCharToMultiByte`](https://learn.microsoft.com/ja-jp/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte)
���g����,
`CString` ( [UTF-16LE](https://ja.wikipedia.org/wiki/UTF-16) ) ��
[�}���`�o�C�g������](https://ja.wikipedia.org/wiki/�}���`�o�C�g����)�ɕϊ����Ă��邾���ł���,
�ϊ���� code page �� [`StringFromCodePage`](#stringfromcodepage) �̂悤�Ɏw��ł���킯�ł��Ȃ�, UTF-8 �Œ�ł�.
�Ȃ��Ȃ� UTF-8 �ɂ����p���Ȃ�����ł�.

[`SaveLog`](#savelog) �݂̂���Ă΂�Ă��܂�.


## `LFtoCRLF`

LF���s��CR/LF���s�ɓ���ւ��܂�.

���[���{�����f�R�[�h���Ă݂��� LF���s������, �Ƃ�������ɑ��������̂�,
���O�̉��s�`�������ꂳ���悤�ɓ���Ă݂܂���.

[`StringFromBody`](#stringfrombody) �݂̂���Ă΂�Ă��܂�.


## `HexToASCII`

`%xx` �`����16�i�\�L�� ASCII �����ɕϊ����܂�.

���[���{�����f�R�[�h���Ă݂���u`%2e%63%6e`�v�̂悤�Ɍ�������Ǝv��������ɑ��������̂�,
���O���ǂ݂₷���Ȃ�悤�ɓ���Ă݂܂���.

[`CheckLink`](#checklink) �݂̂���Ă΂�Ă��܂�.


## `HexToUnicode`

`&#xXXXX` �`����16�i�\�L�� UNICODE ( [UTF-16LE](https://ja.wikipedia.org/wiki/UTF-16) ) �����ɕϊ����܂�.

���[���{�����f�R�[�h���Ă݂���u`&#x002e&#x0063&#x006e`�v�̂悤�Ɍ�������Ǝv��������ɑ��������̂�,
���O���ǂ݂₷���Ȃ�悤�ɓ���Ă݂܂���.

[`StringFromBody`](#stringfrombody) �݂̂���Ă΂�Ă��܂�.


## `ConnectPOP`

[POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol)�ڑ����J�n���܂�.

�u�ڑ����܂��v�ł͂Ȃ��u�ڑ����J�n���܂��v���x�̎d���������܂���.

�ڑ�����������܂Ō��͂��悤�Ƃ����C���Ȃ��̂ł�.
���� Windows&reg; �A�v���P�[�V�����́u�񓯊��v�Ƃ�����
�u[event driven](https://ja.wikipedia.org/wiki/�C�x���g�쓮�^�v���O���~���O)�v�Ƃ�����
�u*Window Message* driven�v�Ȃ��̂ł���,
�� class ���ڑ��ɗp���Ă��� [`CParaSocket`](CParaSocket.md) ��
[`CAsyncSocket`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class)
����h�������񓯊��ȃ\�P�b�g
( �\�P�b�g�ƊE�p��Ō����Ɓu[�m���u���b�L���O](https://ja.wikipedia.org/wiki/�\�P�b�g_(BSD)#�u���b�L���O�ƃm���u���b�L���O)�v) �Ȃ̂�,
�� class �̍\�������񓯊��ł�.
�u�ڑ�����悤���񂾂񂾂���A���̂����ڑ��ł����Ƃ��ł��Ȃ������Ƃ��A�Ȃ񂩌����Ă��邾��B�v
���炢�̍\�����ł�.

��, ���́u�Ȃ񂩌����Ă���v�̂�҂��󂯂Ă���̂�,
[`OnSocketNotify`](#onsocketnotify) �ł�.
[`CParaSocket`](CParaSocket.md) ����オ���Ă����񍐂��L���b�`����,
���̕񍐂ɉ������A�N�V�������N�����܂�.

�Ƃ����킯��, �񓯊��ȃ\�P�b�g��p���Ă���A�v���ł�,
�ڑ�������������͐ڑ������̒ʒm���󂯂Ă���Ȃ񂩎n�߂�̂����ʂł�.
���ʂȂ�ł���, �����ōs���Ă���ʐM��[POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol)�ł�.
[POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol)�̒ʐM�̏ꍇ,
�N���C�A���g���T�[�o�[�ɐڑ������, �N���C�A���g�����������o���O�ɃT�[�o�[�̕����爥�A����z���Ă����ł�.
�Ȃ̂�, ���A�v���̒ʐM�������u�Ȃ������v�Ƃ����񍐂𕷂����Ⴂ�܂���.
�ꑫ��тɁu�Ȃ񂩎�M�����v�̂��m�点��҂��Ă܂�.

�ȏ�, ���������ȒP�ł͂���܂���, �w�i��������̂����A�ɑウ�����Ă��������܂�.

����, �����̒i���ł���:

1. [`FeedDebug`](#feeddebug) ���f�o�b�O�p ( �e�X�g�p ) �̃f�[�^���d���܂�Ă���ƌ����Ă�����,<br>
���̃f�[�^�����������Ƃɂ��ă}�W���ɒʐM����̂Ƃ���߂�.
1. ���[���A�J�E���g�� 1���ݒ肳��Ă��Ȃ��悤�Ȃ�, ����ς�ʐM�Ƃ����Ȃ��ł�߂�.
1. [`ModNI`](#modni) �ŃA�C�R���Ɂu�ʐM���J�n�����v�ƍ���������.
1. [`CParaSocket`](CParaSocket.md) �� 1�݂���.
1. �ʐM�Ǘ��p�̕ϐ�������������.<br>
`m_iPhase`: �ʐM�菇��̒i�����u�܂��������ĂȂ��v��.<br>
`m_iMessage`: ���ʖڂ̃��[�������u�ŏ��́v��.<br>
`m_nMessage`: ���ʂ̃��[�����͂��Ă��邩���u�����͂��Ă��Ȃ��v��.<br>
1. [`CParaSocket`](CParaSocket.md) �����.<br>
���Ȃ��������߂�.
1. `m_iUser`�Ԗڂ�[���[���A�J�E���g](../README.md#accounts)�� [`CParaSocket`](CParaSocket.md) ��ڑ�����.<br>
�ڑ��ł�����OK.<br>
�ڑ��ł��Ȃ��Ă��u�񓯊��Ȃ񂾂��炷���ɂ̓����v�Ȃ�OK.<br>
1. ���ǐڑ��� OK �łȂ�������<br>
�u�����_���������̂��v�� [`CParaSocket`](CParaSocket.md) �ɖ₢���킹��,<br>
�u�����������P�ŕ��܂��v�� [`CParaSocket`](CParaSocket.md) �����.<br>
1. ���ǐڑ��� OK ��������<br>
`m_iPhase`: �ʐM�菇��̒i�����u��1�i�K�v��.<br>

��, ���ꂾ������Ă��̊֐��̎d���͏I����, �Ăяo������<br>
[`PollMails`](#pollmails) ( �ŏ��̃A�J�E���g�ɐڑ�����ꍇ ) ��<br>
[`RespondPOP`](#respondpop) ( 2�Ԗڈȍ~�̃A�J�E���g�ɐڑ�����ꍇ ) �ɋA��܂�.


## `RespondPOP`

[POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol)�̎�M���b�Z�[�W�ɉ������܂�.

�u��M���܂��v�ł͂Ȃ��u��M���b�Z�[�W�ɉ������܂��v�Ƃ����d�����e�ł�.
���ۂ̎�M��[`OnSocketNotify`](#onsocketnotify) �̒��ōς܂���Ă���,
�u����Ȃ񗈂�����?�v�Ɠn���ꂽ��M���b�Z�[�W��ǂ��,
�ǂ����A�N�V�������悤�����߂Ă���Ƃ����̂�, ���̊֐��̗���ł�.

��ʓI��[POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol)�ł̂�����,
���L�̂悤�ɍs���܂�.

```
> +OK provider.ne.jp POP3 server ready.
< USER pochi.the.cat@provider.ne.jp
> +OK please send PASS command
< PASS *************
> +OK pochi.the.cat@provider.ne.jp is welcome here
< STAT
> +OK 3 19864
< RETR 1
> +OK 4444 octets
Return-Path: <����>
.
< RETR 2
> +OK 12210 octets
Return-Path: <����>
.
< DELE 2
> +OK messege 2 deleted
< RETR 3
> +OK 3210 octets
Return-Path: <����>
.
< QUIT
> +OK pochi.the.cat@provider.ne.jp POP3 server signing off.
```

���[�� `>` �� `<` �͑���̕������������߂ɉ��������̂��Ƃ�������������. `>` ����M���b�Z�[�W, `<` �����M���b�Z�[�W�ł�.
�u�񓯊��v(
�Łu[event driven](https://ja.wikipedia.org/wiki/�C�x���g�쓮�^�v���O���~���O)�v�Ƃ������umessage driven�v)
�Ȃ̂�, ���̂�������C�ɍs���킯�ł͂���܂���.
�u�Ȃ񂩎�M������Ȃ񂩑��M����v���Ȃ񂩎�M���邽�тɍא؂�ɍs���Ă��܂�.

��, �ǂ��܂ł���肵�����������Ă�������, `m_iPhase` �ɏ����Ă����܂�.
���� `m_iPhase` �Ƃ����̊֌W��:

| `m_iPhase` | ���� | ���b�Z�[�W |
| :-: | --: | --- |
| `1` | `>` | `+OK provider.ne.jp POP3 server ready.                    ` |
| `1` | `<` | `USER pochi.the.cat@provider.ne.jp                        ` |
| `2` | `>` | `+OK please send PASS command                             ` |
| `2` | `>` | `PASS *************                                       ` |
| `3` | `>` | `+OK pochi.the.cat@provider.ne.jp is welcome here         ` |
| `3` | `<` | `STAT                                                     ` |
| `4` | `>` | `+OK 3 19864                                              ` |
| `5` | `<` | `RETR 1                                                   ` |
| `6` | `>` | `+OK 4444 octets                                          ` |
| `6` | `>` | `Return-Path: <����>                                      ` |
| `6` | `>` | `.                                                        ` |
| `7` | `<` | `RETR 2                                                   ` |
| `6` | `>` | `+OK 12210 octets                                         ` |
| `6` | `>` | `Return-Path: <����>                                      ` |
| `6` | `>` | `.                                                        ` |
| `6` | `<` | `DELE 2                                                   ` |
| `7` | `>` | `+OK messege 2 deleted                                    ` |
| `7` | `<` | `RETR 3                                                   ` |
| `6` | `>` | `+OK 12210 octets                                         ` |
| `6` | `>` | `Return-Path: <����>                                      ` |
| `6` | `>` | `.                                                        ` |
| `7` | `<` | `QUIT                                                     ` |
| `8` | `>` | `+OK pochi.the.cat@provider.ne.jp POP3 server signing off.` |

�Ƃ������ƂɂȂ�܂�.

�O�u���������Ȃ�܂�����, �����菇�͈ȉ��̂悤�ɂȂ��Ă��܂�.

1. �T�[�o�[����̈��A���󂯂�, `USER` �R�}���h�𑗂�.
1. `+OK` �ƌ���ꂽ��, `PASS` �R�}���h�𑗂�.<br>
���������Ȃ������� `QUIT` ����.
1. `+OK` �ƌ���ꂽ��, `STAT` �R�}���h�𑗂�.<br>
���������Ȃ������� `QUIT` ����.
1. `+OK` �ƌ���ꂽ��, �S���ǂނ���ł܂� 1�ʖڂ���n�߂�.
1. `RETR` �R�}���h�Ŏ擾.
1. `���[���̒��g` ���Ԃ��Ă����̂�,<br>
���g���o������Ă����� [`ParseMail`](#paasemail) �Œ��g���`�F�b�N. �j�����ׂ����[���Ȃ� `DELE` ����.<br>
���g���o������Ă��Ȃ�������, ���̎�M��҂�.
1. �S�Ẵ��[����ǂݏI������Ȃ� `QUIT`, �܂��Ȃ玟�̃��[���� `RETR` ����.
1. `���ʂ�̈��A` ���Ԃ��Ă����̂� [`ClosePOP`](#closepop) ��,<br>
���̃A�J�E���g������Ȃ� [`ConnectPOP`](#connectpop) ��, �Ō�̃A�J�E���g�Ȃ猋�ʂ� [`ModNI`](#modni) ����.
1. `+OK` �ƌ����Ȃ������ꍇ��, [`ClosePOP`](#closepop) ����.

���̒i���� `switch ( m_iPhase )` �Ɋ�Â��� `case` �œ������Ă��܂���,
�ꕔ�� `case` �ł� `break` �����Ɏ��� `case` �ɗ��Ƃ��Ă���ꍇ������̂ł��������Ȃ��悤.<br>
( ���ǃ��[�����������ꍇ�� `4` �� `5` ��, 1�ʕ��ǂݐ؂������Ɣj�����Ȃ������ꍇ�� `6` �� `7` )

### Debug build �ł��̂����𒭂߂Ă�����ւ̘A������

`#define DBGOUTPUT` �ƂƂ����, ���̂����̗l�q�����߂���悤�ɂȂ��Ă��܂���,
�u�Ō�� `.` �܂Ŏ�M�����C�z���Ȃ��̂ɂǂ����Ď��̃��[���� `RETR` �����?�v
�ƍl�����ނ͎̂��Ԃ̃��_�ł�.
������čŌ�� `.` �܂� `OutputDebugStringA` ���؂�ĂȂ������ł�.
[`OutputDebugStringA` �ɂ͏o�͂ł��镶�����Ɍ��E������](http://www.unixwiz.net/techtips/outputdebugstring.html)��ł�.


## `ClosePOP`

[POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol)�̐ڑ����I�����܂�.

���� `nError` �͉��L�̂悤�ɂȂ��Ă��܂�.

| `nError` | �Ӗ� |
| --: | --- |
| `0` | ���Ȃ���������. |  |
| `-1` | [POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol) �ŋ��ۂ�ꂽ. |
| ���̑� | [`CParaSocket`](CParaSocket.md) ���G���[��Ԃ��Ă���. |

������肪�������ꍇ��, [`ModNI`](#modni) �ɂĖ�肪���������Ƃ����m�点���܂�.
���Ȃ����������ꍇ��, �P�� [`CParaSocket`](CParaSocket.md) ��Еt���܂�.


## `FeedDebug`

�f�o�b�O�p�f�[�^���������܂�.

`ChkMails.exe` �̒u���Ă���t�H���_�[�� `Mails` �Ƃ����T�u�t�H���_�[��������,
���̃T�u�t�H���_�[�� `.eml` �t�@�C���� 1�ȏ�����Ă����,
�e `.eml` �̒��g��[POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol)�ʐM�œ������̂悤��,
�{�N���X�ɋ������܂�.

�������e�Ƃ��Ă�:

1. `Mails` �T�u�t�H���_�[���� `*.eml` ����������.
1. �������ʂ�������ɕ��ׂ�.
1. 1�Â� [`ReadDebug`](#readdebug) �œǂݍ���.
1. �ǂݍ��� `.eml` ���������� [`ModNI`](#modni) �Ō��ʂ����m�点����.
1. 1�ł��ǂ񂾎��т�����Ȃ� `true` ��, �����łȂ���� `false` ��Ԃ�.

�Ƃ��Ă��܂�.

�u�f�o�b�O�v�Ə̂��Ă��܂���,
���ۂ̉^�p�Ńn�l��ꂽ�������͒ʉ߂����Ă��܂������[���� `*.eml` ���g����,
[Filter](../README.md#filter) �̏������ċᖡ����Ȃ�,
�f�o�b�O�ړI�ȊO�̎g�p�̕������W���[�ł��傤.


## `ReadDebug`

�f�o�b�O�p�f�[�^��ǂݍ��݂܂�.

�P�Ɉ����Ƃ��ė^����ꂽ�t�@�C�� ( `.eml` ) ��ǂ�ł��邾���ł���,
�ǂݏI���������, �ǂݎ����������� [`ParseMail`](#parsemail) �Ɉ����n����,
�X�p�����[�����ۂ��̔��蓮��������܂�.
