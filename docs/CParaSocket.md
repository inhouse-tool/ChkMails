# `CParaSocket`

## �\��

�\�[�X�R�[�h���,
[`ParaSocket.h`](../ChkMails/ChkMails/ParaSocket.h) ��
[`ParaSocket.cpp`](../ChkMails/ChkMails/ParaSocket.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor & Destructor

[`CParaSocket`](#cparasocket-1)
[`~CParaSocket`](#cparasocket-2)

#### Public

[`Connect`](#connect)
[`Close`](#clse)
[`Send`](#send)
[`Receive`](#receive)
[`IOCtl`](#ioctl)
[`SetNotify`](#setnotify)
[`GetLastState`](#getlaststate)

#### Override

[`OnAccept`](#onaccept)
[`OnClose`](#onclose)
[`OnConnect`](#onconnect)
[`OnReceive`](#onreceive)
[`OnSend`](#onsend)

#### �ŗL�֐�

[`NotifyState`](#notifystate)

#### TLS �֘A

[`OnConnectTLS1`](#onconnecttls1)
[`OnConnectTLS2`](#onconnecttls2)
[`OnReceiveTLS`](#onreceivetls)

[`ReceiveTLS`](#receivetls)
[`SendTLS`](#sendtls)
[`CloseTLS`](#closetls)
[`FinishTLS`](#finishtls)

[`EnqueueData`](#enqueuedata)
[`DequeueData`](#dequeuedata)


## �T�v

�񓯊��\�P�b�g class
[`CAsyncSocket`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class)
����h���������ʐM�p�̃\�P�b�g class �ł�.

�u�񓯊��v�Ȃ̂�
�u[event driven](https://ja.wikipedia.org/wiki/�C�x���g�쓮�^�v���O���~���O)�v�ɓ����܂�.
���������������^�������, ���̂��������ɉ����Ă��傱���Ɠ���,
���̂��傱���Ƃ̓������I�������, ���̂�������������܂ŉ������Ȃ��ő҂��Ă���,
�Ƃ��������ł�.
�h������
[`CAsyncSocket`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class)
�����������g����������O��� class �Ȃ̂�, ���̗��V�𓥏P���Ă��܂�.

�� class ��
[TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) ���T�|�[�g���Ă���̂�,
TLS ���g�p����
[POP3S](https://ja.wikipedia.org/wiki/Post_Office_Protocol#�Í���)
�ɂ��g���܂�. ( TLS �̂Ȃ� [POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol) �ɂ��g���܂�. )

���� class �� TLS ���g�p�����ۂ�, �A�v���w���܂ފK�w�\���͉��L�̂悤�ɕ\���܂�.

| Application |
| :-: |
| `CParaSocket` |
| [TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) ||
| [`CAsyncSocket`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class) |
| *Network* |
| *Server* |

�܂�, �A�v���w����� TLS �I����₱���͌����܂���.
��������������₱���͂��� class �̒��Ŗʓ|�����Ă��܂�.
�A�v���w�Ƃ��Ă� TLS ���ӎ������� ( �Í����ȂǍl������ ) �T�[�o�[�ƒʐM���܂�.
POP3 �ł� POP3S �ł���邱�Ƃ͓����ł�.

�ł�, �ǂ������ TLS �����ނ��ۂ������߂Ă���̂��Ƃ�����,
�|�[�g�ԍ��ł�.
���ԓI�� TLS �����ނ��񂾂Ɨ�������Ă���T�[�r�X�̃|�[�g�ԍ��ł̐ڑ������܂ꂽ�ꍇ,
���� class �ł͏���� TLS �����~���ɋ��݂܂�.
�ΏۂƂȂ�|�[�g�͉��\�̒ʂ�ł�.

| �|�[�g�ԍ� | �T�[�r�X |
| --- | --- |
| 443 | [HTTPS](https://ja.wikipedia.org/wiki/HTTPS) |
| 465 | [SMPTS](https://ja.wikipedia.org/wiki/SMTPS) |
| 993 | [IMAPS](https://ja.wikipedia.org/wiki/Internet_Message_Access_Protocol)
| 995 | [POP3S](https://ja.wikipedia.org/wiki/Post_Office_Protocol#�Í���)

���ɂ� TLS �ȃT�[�r�X���Ȃ����Ƃ͂Ȃ��̂ł���, �g���\�肪�Ȃ��̂őΉ����T�{��܂���.
����, �u�|�[�g�ԍ������I�Ɍ��܂�̂��� TLS �����݂����v�Ƃ��������v�̂��߂�,

* �|�[�g�ԍ��ɕ�����n���Ɛ����Ƃ��ď�������� TLS ������

�Ƃ������׍H�������Ă��܂�.

�����̏�������O��镁�ʂ̃|�[�g�ԍ��ł�, ���ʂɐڑ���, ���ʂɒʐM���܂�.
�܂�, �A�v���w����n���ꂽ�f�[�^�����̂܂܃l�b�g���[�N�ɗ���,
�l�b�g���[�N���痈���f�[�^�����̂܂܃A�v���w�ɓn��܂�.


## `CParaSocket`

���� class �� constructor �ł�.

��� class
[`CAsyncSocket`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class)
�� constructor ���ĂԂƂ����厖�Ȏd������������,
�����o�[�ϐ������������Ă��܂�.
<br>
<sub>
�������Ă����Ȃ���[C26495](https://learn.microsoft.com/ja-jp/cpp/code-quality/c26495)�����邳���̂�.
</sub>

## `~CParaSocket`

���� class �� destructor �ł�.

�ғ����Ɋm�ۂ����������[���J������,
��� class
[`CAsyncSocket`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class)
�� destructor �Ń\�P�b�g��j�����܂�.


## `Connect`

[`CAsyncSocket::Connect`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#connect)
�� override �ł�.

�������e�͈ȉ��̒ʂ�ł�.

1. �����Ƃ��ēn���ꂽ�ڑ���ƃ|�[�g�ԍ����L�����Ă���.
1. TLS �Ώۃ|�[�g�ԍ���������, �uTLS�i�K�v�� `1` �ɐi�߂�.
1. �|�[�g�ԍ���������������, �����ɔ��]�̏�, �uTLS�i�K�v�� `1` �ɐi�߂�.
1. ��L 2�҂ɊY�����Ȃ��ꍇ��, �uTLS�i�K�v�� `0` �ɗ��߂�.
1. ����, �uTLS�i�K�v�� `1` ��������, �u�Í��o�b�t�@�v���m�ۂ��āu��v�ɏ�����.
1. [`CAsyncSocket::Connect`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#connect)
�Ń\�P�b�g��ڑ�����.

�Ō��
[`CAsyncSocket::Connect`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#connect)
�̌Ăяo���̖߂�l��Ԃ��܂�.


## `Close`

[`CAsyncSocket::Close`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#close)
�� override �ł�.

�uTLS�i�K�v�� `0` �łȂ����,

* [`CloseTLS`](#closetls) �� TLS �Ƃ��ĕ���.
* �uTLS�i�K�v�� `0` �ɏ�����.
* �u�Í��o�b�t�@�v���J����, �Í����֘A�̕ϐ����N���A.

�Ƃ�����, ������ɂ���
[`CAsyncSocket::Close`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#close)
�Ń\�P�b�g����ďI���ł�.


## `Send`

[`CAsyncSocket::Send`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#send)
�� override �ł�.

�uTLS�i�K�v�� `1` �ȏゾ������, [`SendTLS`](#sendtls) ���Ă�ł��̖߂�l��Ԃ��܂�.
<br>
�uTLS�i�K�v�� `0` ��������,
�\�P�b�g����̑��M�̂��߂�
[`CAsyncSocket::Send`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#send)
���Ă�ł��̖߂�l��Ԃ��܂�.


## `Receive`

[`CAsyncSocket::Receive`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#receive)
�� override �ł�.

�uTLS�i�K�v�� `1` �ȏゾ������, [`ReceiveTLS`](#receivetls) ���Ă�ł��̖߂�l��Ԃ��܂�.
<br>
�uTLS�i�K�v�� `0` ��������,
�\�P�b�g����̎�M�̂��߂�
[`CAsyncSocket::Receive`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#receive)
���Ă�ł��̖߂�l��Ԃ��܂�.


## `IOCtl`

[`CAsyncSocket::IOCtl`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#ioctl)
�� override �ł�.

�uTLS�i�K�v�� `1` �ȏ�ŃR�}���h�� `FIONREAD` �̏ꍇ��, �u�����o�b�t�@�v�ɂ���o�C�g����`����, `TRUE` ��Ԃ��܂�.
<br>
��L�ȊO�̏ꍇ��,
�\�P�b�g�̐���̂��߂�
[`CAsyncSocket::IOCtl`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#ioctl)
���Ă�ł��̖߂�l��Ԃ��܂�.


## `SetNotify`

Socket �̏�ԕω���ʒm���郁�b�Z�[�W��ݒ肵�܂�.

���ꂼ��̈����̈Ӗ��͈ȉ��̒ʂ�ł�:

| ���� | �Ӗ� | �f�t�H���g |
| --- | --- | --- |
| `pWnd` | �ʒm��� `CWnd*` | `AfxGetMainWnd` �œ����� main window |
| `nIdClient` | �N���C�A���g�w��̔C�ӂ̒l | `0` |
| `nIdMessage` | �ʒm���� window message | `WM_SOCKET_NOTIFY` |

���̊֐��Őݒ肳�ꂽ�ʒm������,
[`NotifyState`](#notifystate) �̓A�v���w�ɏ�ԕω���ʒm���܂�.

`AfxGetMainWnd` �œ����� main window ������ class �̎g�p�҂ł���_�C�A���O�x�[�X�̃A�v���̏ꍇ,
���̊֐��̌Ăяo���͏ȗ��\�ł���,
main window ��
[`CFrameWnd`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cframewnd-class)
�̔h�� class �ƂȂ� *Document & View Architecture* �̏ꍇ��,
���� class �̎g�p�҂� [*View*](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cview-class) �Ȃ�,
���̊֐���ʂ��� *View* ���ʒm��ł��邱�Ƃ�ݒ肵�Ă����K�v������܂�.

�T�[�o�[�\�P�b�g���^�p����ꍇ��,
[`Listen`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#listen)
���Ă���\�P�b�g����̒ʒm��,
[`Accept`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#accept)
�����\�P�b�g����̒ʒm���������邽�߂�,
`nIdClient` �ɕʁX�̒l���w�肵�Ă����Ƃ����g����������܂�.

## `GetLastState`

Socket �̌��݂̏�Ԃ�Ԃ��܂�.

��蓾���Ԓl�͈ȉ��̒ʂ�ł�:

| ��Ԓl | �Ӗ� |
| --- | --- |
| `SOCK_STATE_IDLE` | �u�Ȃ����ĂȂ��E�؂ꂽ�v |
| `SOCK_STATE_CONNECTED` | �u�Ȃ������v |
| `SOCK_STATE_RECEIVED` | �u�Ȃ񂩎�M�����v |
| `SOCK_STATE_SENT` | �u���M�ł����v |
| `SOCK_STATE_ACCEPTED` | �u���ꂩ�����v |
| `SOCK_STATE_FAILED` | �u�Ȃ񂩎��s�����v |


## `OnAccept`

[`CAsyncSocket::OnAccept`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#onaccept)
�� override �ł�.

[`NotifyState`](#notifystate) ���Ă�ŃA�v����
`SOCK_STATE_ACCEPTED` ( �G���[���������ꍇ�� `SOCK_STATE_FAILED` ) �ƒʒm���܂�.


## `OnClose`

[`CAsyncSocket::OnClose`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#onclose)
�� override �ł�.

[`NotifyState`](#notifystate) ���Ă�ŃA�v����
`SOCK_STATE_IDLE` ( �G���[���������ꍇ�� `SOCK_STATE_FAILED` ) �ƒʒm���܂�.


## `OnConnect`

[`CAsyncSocket::OnConnect`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#onconnect)
�� override �ł�.

* �G���[���������ꍇ��, [`NotifyState`](#notifystate) ���Ă�ŃA�v���� `SOCK_STATE_FAILED` �ƒʒm���܂�.
* �uTLS�i�K�v�� `1` �̏ꍇ��, �A�v���ɂ͒ʒm����, [`OnConnectTLS1`](#onconnecttls1) ���Ă�� TLS ���J�n���܂�.
* ���̑��̏ꍇ��, [`NotifyState`](#notifystate) ���Ă�ŃA�v���� `SOCK_STATE_CONNECTED` �ƒʒm���܂�.


## `OnReceive`

[`CAsyncSocket::OnReceive`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#onreceive)
�� override �ł�.

���̏������e��, �󋵂ɂ���ĉ��L�̂悤�ɕ��򂵂Ă��܂�.

| �� | �Ή� |
| --- | --- |
| �G���[���� | `SOCK_STATE_FAILED` �ƃA�v���� [`NotifyState`](#notifystate). |
| �uTLS�i�K�v�� `2` | [`OnConnectTLS2`](#onconnecttls2) ���Ă�. | 
| �uTLS�i�K�v�� `3` �ȏ� | [`OnReceiveTLS`](#onreceivetls) ���Ă�. | 
| ���̑� | `SOCK_STATE_RECEIVED` �ƃA�v���� [`NotifyState`](#notifystate). |


## `OnSend`

[`CAsyncSocket::OnSend`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#onsend)
�� override �ł�.

[`NotifyState`](#notifystate) ���Ă�ŃA�v����
`SOCK_STATE_SENT` ( �G���[���������ꍇ�� `SOCK_STATE_FAILED` ) �ƒʒm���܂�.


## `NotifyState`

Socket �̏�ԕω����A�v���w�ɒʒm���܂�.

[`SetNotify`](#setnotify) �Őݒ肳�ꂽ�p�����[�^�[��,

* `WPARAM` �ɂ̓N���C�A���g�̎w��l�Ə�Ԃ������R�[�h��D�荞�񂾒l
* `LPARAM` �ɂ͂��� class �̃C���X�^���X�ւ̃|�C���^�[

�� [post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew)
���܂�.

���̒ʒm���b�Z�[�W���󂯂��A�v���w���\�P�b�g�𑀍삵�ɂ���,
�Ƃ����̂��u�񓯊��v�̗��V�ł�.


## `OnConnectTLS1`

[TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) �ڑ����J�n���܂�. (����1)

[`OnConnect`](#onconnect) ����Ă΂�, TLS �ڑ��́u�͂��܂�̈���v�𓥂ݏo���܂�.

�ʐM���, TCP/IP ��̐ڑ����������Ă���, ������uClient Hello�v�𑗐M����܂ł��s���܂�.

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="../pics/TLS1d.png">
  <source media="(prefers-color-scheme: light)" srcset="../pics/TLS1l.png">
  <img src="../pics/TLS1l.png">
</picture>
<p>

�����, �T�[�o�[����́uServer Hello�v�ƁuCertificate�v��uServer Hello Done�v���Ԃ��Ă���͂��ł���,
�{ class �́u�񓯊��v��M���ɂ��Ă���̂�,
�����͌�������M�����Ƃ��� [`OnReceive`](#onreceive) �����������Ɏn�߂邱�Ƃɂ���,
���̊֐��͂������񔲂��܂�.

���̊֐���������܂łɍs���������e�͈ȉ��̒ʂ�ł�:

1. [`AcquireCredentialsHandle`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/acquirecredentialshandle--schannel)
�� TLS �̒ʐM���s�������𐮂���.
1. [`InitializeSecurityContext`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel)
��, Client Hello �̃��b�Z�[�W���쐬����.
1. [`CAsyncSocket::Send`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#send)
�� Client Hello �̃��b�Z�[�W�𑗐M����.
1. [`FreeContextBuffer`](https://learn.microsoft.com/ja-jp/windows/win32/api/sspi/nf-sspi-freecontextbuffer)
���Ă��, ���M�o�b�t�@�\���J��.
1. �uTLS�i�K�v�� `2` �ɐi�߂�.


## `OnConnectTLS2`

[TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) �ڑ����J�n���܂�. (����2)

[`OnReceive`](#onreceive) ����Ă΂�, TLS �ڑ��́u�c��̎d���v��Еt���܂�.

�ʐM���, ������uServer Hello�v,�uCertificate�v,�uServer Hello Done�v����M����,
�uClient Key Exchange�v,�uChange Cipher Spec�v,�uEncrypted Handshake Message�v��ԐM��,
���̂܂��ԐM�́uChange Cipher Spec�v,�uEncrypted Handshake Message�v����M����܂ł��s���܂�.

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="../pics/TLS2d.png">
  <source media="(prefers-color-scheme: light)" srcset="../pics/TLS2l.png">
  <img src="../pics/TLS1l.png">
</picture>
<p>

����肷��f�[�^���傫���܂���������̂�, 1��ł͌��_���o��, TLS �� API ����u�Â��v�Ƃ��Ԃ��Ă���ꍇ������܂���,
�{ class �́u�񓯊��v��M���ɂ��Ă���̂�,
�����͂Â��̌�������M�����Ƃ��� [`OnReceive`](#onreceive) �����������Ɏn�߂邱�Ƃɂ���,
���̊֐��͂������񔲂��܂�.

���̊֐��̒S���t�F�[�Y���I���܂łɍs���������e�͈ȉ��̒ʂ�ł�:

1. [`CAsyncSocket::Receive`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#receive)
�ŃT�[�o�[����̃��b�Z�[�W����M����.
1. �u�Z�L�����e�B�[�g�[�N���̂����v�Ƃ�������Ŏ�M���b�Z�[�W��
[`InitializeSecurityContext`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel).
1. `SECBUFFER_EXTRA` ����������u�ꏏ�Ɏ�M�����f�[�^�v�Ƃ������ƂŁu�Í��o�b�t�@�v�ɒǉ�.
1. �u�ł����v���Ԃ��Ă�����uTLS�i�K�v�� `3` �ɐi�߂�,
�u�ڑ������v�� [`NotifyState`](#notifystate).
1. �u�����Ɓv���Ԃ��Ă�����, ���̎�M��҂��߂��̏�͏I���ɂ���.
1. �u�Â��v���Ԃ��Ă�����, �ԐM���ׂ��f�[�^���Y�����Ă����炻���
[`CAsyncSocket::Send`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#send)���Ă���
[`FreeContextBuffer`](https://learn.microsoft.com/ja-jp/windows/win32/api/sspi/nf-sspi-freecontextbuffer)
���Ă��, ���M�o�b�t�@�\���J��.

�Ƃ������Ƃ�,
�A�v���w�ɖق����܂ܐ��ʉ��ł��Ȃ�̂������o��, ���_���ł��Ƃ���ł͂��߂�
�u�ڑ������v�ƃA�v���w�ɂ��m�点����̂ł�.

�Ƃ���ŏ�L�̏������I������, ����Ɏ�M�f�[�^���c���Ă���ꍇ������܂���
( �T�[�o�[����ŏ��̈��A�𑗂��Ă��� [POP3](https://ja.wikipedia.org/wiki/Post_Office_Protocol)
�̏ꍇ�͑����Ȃ�܂��� ),
���̏����͈ȉ��̒ʂ�ł�:

1. �u�f�[�^�v�Ƃ�������ŗ��܂��Ă���u�Í��o�b�t�@�v��
[`DecryptMessage`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/decryptmessage--schannel).
1. �����ł�����, ���̃��b�Z�[�W������̓ǂݏo���ɔ����� [`EnqueueData`](#enqueuedata).
1. ��������Ȃ������c�肪��������, ���̕��́u�Í��o�b�t�@�v�ɖ߂��Ă���.
1. �u�Ȃ񂩎�M�����v�� [`NotifyState`](#notifystate).

�u�񓯊��v�ȃA�v���w�Ƃ��Ă�, ���́u�Ȃ񂩎�M�����v�Ƃ����ʒm���󂯂�,
�� class �� [`Receive`](#receive) ���Ăяo���蔤�ɂȂ��Ă��܂�.


## `OnReceiveTLS`

[TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) �ł̎�M���s���܂�.

[`OnReceive`](#onreceive) ����Ă΂�, TLS �ɂ��Í������ꂽ��M���b�Z�[�W�𕜍����܂�.

���̏����͈ȉ��̒ʂ�ł�:

1. �u�Í��o�b�t�@�v�������ς���������, ���������ɋA��.
1. [`CAsyncSocket::Receive`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#receive)
�ňÍ������ꂽ���b�Z�[�W����M.
1. ������M�ł��Ȃ�������A��.
1. �u�Í��o�b�t�@�v�Ɏ�M�f�[�^���c���Ă����, �ȉ����J��Ԃ�:
1. �u�f�[�^�v�Ƃ�������ŗ��܂��Ă���u�Í��o�b�t�@�v��
[`DecryptMessage`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/decryptmessage--schannel).
1. �u�ł����v���Ԃ��Ă�����, ����� [`EnqueueData`](#enqueuedata) ����,<br>
`SECBUFFER_EXTRA` ����������u�ꏏ�Ɋ������񂾃f�[�^�v�ƌ��Ȃ��āu�Í��o�b�t�@�v�ɒǉ���,<br>
����������ĂȂ��f�[�^���c���Ă����� 5. ����J��Ԃ�����, �����łȂ����<br>
�u�Ȃ񂩎�M�����v�� [`NotifyState`](#notifystate) ���ďI��.
1. �u�����v���Ԃ��Ă�����, ���܂Ŏ�M�����f�[�^���Ȃ��������Ƃɂ���.
1. �u�����Ɓv���Ԃ��Ă�����, ���̎�M��҂��߂��̏�͏I���ɂ���.
1. ����ȊO���Ԃ��Ă�����, [`FinishTLS`](#finishtls) �� TLS ��Еt���ďI��.

�܂�, �Í������ꂽ���b�Z�[�W�̎�M���ς܂�����,
���̕������������Ă͂��߂āu�Ȃ񂩎�M�����v�ƃA�v���w�ɂ��m�点����̂ł�.

�u�񓯊��v�ȃA�v���w�Ƃ��Ă�, ���́u�Ȃ񂩎�M�����v�Ƃ����ʒm���󂯂�,
�� class �� [`Receive`](#receive) ���Ăяo���蔤�ɂȂ��Ă��܂�.


## `ReceiveTLS`

[TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) �Ŏ�M�����f�[�^���������܂�.

[`Receive`](#receive) ����Ă΂�, TLS ���瓾�ĕ������ꂽ��M���b�Z�[�W���������܂�.

�����œn���ꂽ��M�o�C�g���𖞂����܂�, �ȉ����J��Ԃ��܂�:

1. �u�����o�b�t�@�v�ɉ��������Ă���ꍇ��, �����Ă��镪���� [`DequeueData`](#dequeuedata).
1. �u�����o�b�t�@�v���󂾂����ꍇ��, ������M����܂�
[`WaitForSingleObject`](https://learn.microsoft.com/ja-jp/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject)

�����ĕ��������o�C�g�� ( �� �����œn���ꂽ��M�o�C�g�� ) ��Ԃ��܂�.
<br>
�����œn���ꂽ�|�C���^�[�̒��g��, �����������b�Z�[�W�Ŗ�������Ă��܂�.


## `SendTLS`

[TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) �Ńf�[�^�𑗐M���܂�.

[`Send`](#send) ����Ă΂�, TLS ����ĈÍ������ꂽ���b�Z�[�W�𑗐M���܂�.

�����œn���ꂽ���M�o�C�g���ɒB����܂�, �ȉ����J��Ԃ��܂�:

1. �����œn���ꂽ���M�o�C�g���ƈ�x�ɑ��M�ł���ő�o�C�g���̏��������𓖍��̑��M�o�C�g���Ƃ���.
1. ���M�o�b�t�@��݂���.
1. �u�w�b�_�v,�u�f�[�^�v,�u�g���C���[�v�Ƃ�������ő��M�o�b�t�@��
[`EncryptMessage`](https://learn.microsoft.com/ja-jp/windows/win32/api/sspi/nf-sspi-encryptmessage).
1. �u�ł����v���Ԃ��Ă�����, �����
[`CAsyncSocket::Send`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#send).
1. ���M�o�b�t�@���J������.
1. �u�ł����v�ȊO���Ԃ��Ă�����, �u�Ȃ񂩎��s�����v�� [`NotifyState`](#notifystate) ���ďI��.

�����đ��M�����o�C�g�� ( �� �����œn���ꂽ���M�o�C�g�� ) ��Ԃ��܂�.
<br>
���M���郁�b�Z�[�W��, �S�ĈÍ�������Ă��܂�.


## `CloseTLS`

[TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) �ڑ����I�����܂�.

[`Close`](#close) ����Ă΂�, TLS �ڑ����I�������܂�.

1. �u�Z�L�����e�B�[�g�[�N���̂����v�Ƃ�������ŃV���b�g�_�E�����b�Z�[�W��
[`InitializeSecurityContext`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel).
1. �u�ł����v���Ԃ��Ă�����, �����
[`CAsyncSocket::Send`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#send).
1. 50[ms] �قǑ҂�.
1. [`FreeContextBuffer`](https://learn.microsoft.com/ja-jp/windows/win32/api/sspi/nf-sspi-freecontextbuffer)
���Ă��, ���M�o�b�t�@�\���J��.
1. [`FinishTLS`](#finishtls) ���Ă��, ���܂ŕێ����Ă��� TLS �p�̎������J��.

�����ŋ^����������̂� 3. �́u�҂v�ł��傤��.
���_���猾���ƕʂɑ҂��Ȃ��Ă����̎x�������܂���.
����, �����ł�����Ƒ҂�
[Wireshark](https://ja.wikipedia.org/wiki/Wireshark)
�Ƃ��ŃL���v�`���[�������O�����ꂢ�ɏI��邩��, �Ƃ��������̗��R�œ���Ă��܂�.

�����ɑ҂����Ԃ����܂Ȃ���,
�������� [`Close`](#close) �ɖ߂�
[`CAsyncSocket::Close`](https://learn.microsoft.com/en-us/cpp/mfc/reference/casyncsocket-class#close)
�� TCP �ڑ���ؒf���邱�ƂɂȂ�܂�.
�����炩�瑗�M�����V���b�g�_�E�����b�Z�[�W�uEncrypted Alert: Close Notify�v�ɑ΂��� TCP ACK ��҂����ɂł�.
����Ȏ���Ȑؒf�̌���, �L���v�`���[�������O���������ƂɂȂ�̂ł�.
( Wireshark �ł� �uTCP Dup ACK�v�Ǝw�E����܂�. ACK ���͂��Ȃ������Ɣ��f�����T�[�o�[���đ����邩��ł�. )

�u�񓯊��v�Łu[event driven](https://ja.wikipedia.org/wiki/�C�x���g�쓮�^�v���O���~���O)�v��W�Ԃ��Ă���ȏ�,
�u�҂����Ԃ����ށv�Ȃ�čs�ׂ͂������Ȃ��̂��Ƃ����C�����܂���,
�����A�v���K�w�ł́uTCP �K�w�v�� ACK ��҂��ׂ͂Ȃ�<sup>*</sup>�̂�,
�u�҂����Ԃ����ށv�Ŏ��ł��Ă��鎟��ł�.
<br>
<sub>
*
�����ɂ́u�Ȃ��͂Ȃ��v�̂ł���,
�u��? ����Ȃ��Ƃ̂��߂ɂ����܂ł���?�v�Ƃ�����i�ɑi���邱�ƂɂȂ�̂�,
�펯�I�Ȏ�i�ŊԂɍ��킹�Ă��܂�.
</sub>


## `FinishTLS`

[TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) ���I�����܂�.

TLS ���I�����悤�Ƃ��邠����������Ă΂�, ����܂ŉ^�p���Ă��� TLS �p�̎������J�����܂�.

��̓I�ɂ�, [`OnConnectTLS1`](#onconnecttls1) ��
[`AcquireCredentialsHandle`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/acquirecredentialshandle--schannel)
�Ŏ擾�����u[���i���](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/obtaining-schannel-credentials)�v�ւ� handle ��,
[`InitializeSecurityContext`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel)
�ō\�z�����u�R���e�L�X�g�v�ւ� handle ���J�����܂�.

��L handle ���J������ǖʂƂ͂��Ȃ킿�ʐM���I������ǖʂȂ̂�,
�I�����邱�ƂɂȂ������@�������̃X�e�[�^�X�œn���Ă��炢,
����I���łȂ������ꍇ�͂��̎|�� [`NotifyState`](#notifystate) ���邱�Ƃ� ( ���ł� ) ����Ă��܂�.


## `EnqueueData`

�u�����o�b�t�@�v�̖����Ƀf�[�^��ǉ����܂�.

�u�����o�b�t�@�v�� [FIFO](https://ja.wikipedia.org/wiki/FIFO) ��
[�L���[](https://ja.wikipedia.org/wiki/�L���[_(�R���s���[�^))�ɂȂ��Ă���̂�,
���̖����ɕ����ς݂̃f�[�^��ǉ����܂�.

[TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) ����Ɏ�M�����f�[�^��,
�A�v���w�ɐ�ɓn��悤�� ( ��M�������Ԓʂ�ɃA�v���w�ɓn��悤�� )
FIFO �̃L���[�`���ƂȂ��Ă��܂�.

�ǉ�������, �L���[�ɕێ�����Ă���f�[�^�͑����܂�.

## `DequeueData`

�u�����o�b�t�@�v�̐擪���炩��f�[�^�����o���܂�.

�u�����o�b�t�@�v�� [FIFO](https://ja.wikipedia.org/wiki/FIFO) ��
[�L���[](https://ja.wikipedia.org/wiki/�L���[_(�R���s���[�^))�ɂȂ��Ă���̂�,
���̐擪���畜���ς݂̃f�[�^�����o���܂�.

[TLS](https://ja.wikipedia.org/wiki/Transport_Layer_Security) ����Ɏ�M�����f�[�^��,
�A�v���w�ɐ�ɓn��悤�� ( ��M�������Ԓʂ�ɃA�v���w�ɓn��悤�� )
FIFO �̃L���[�`���ƂȂ��Ă��܂�.

���o������, �L���[�ɕێ�����Ă���f�[�^�͌���, �c���Ă���f�[�^�͐擪�Ɍ����Ĉړ����܂�.

