# `CNotifyWnd`

## �\��

�\�[�X�R�[�h���,
[`NotifyWnd.h`](../ChkMails/ChkMails/NotifyWnd.h) ��
[`NotifyWnd.cpp`](../ChkMails/ChkMails/NotifyWnd.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CNotifyWnd`](#cnotifywnd-1)

#### Public

[`SetText`](#settext)

#### �W�� Window Message �n���h���[

[`OnCreate`](#oncreate)
[`OnEraseBkgnd`](#onerasebkgnd)
[`OnLButtonDown`](#onlbuttondown)
[`OnPaint`](#onpaint)
[`OnTimer`](#ontimer)
[`OnDisplayChange`](#ondisplaychange)

#### �ŗL�֐�

[`PlaceWindow`](#placewindow)
[`ToastWindow`](#toastwindow)
[`Draw`](#draw)


## �T�v

[�T�}���[](../README.md#notification) �̕\���ɗp�����Ă���ʒm�p�E�B���h�E�� class �ł�.

![](../Examples/Summary.png)
<p>

Windows �ɂ͂�����
�u[�g�[�X�g](https://learn.microsoft.com/ja-jp/windows/apps/design/shell/tiles-and-notifications/toast-notifications-overview)<sup>*</sup>�v
�ƌĂ΂�Ă���ʒm�p�̎d�g�݂�����̂�,
������g�����ƍŏ��͍l�����̂ł���,
���܂�Ɏg���Â炢<sup>**</sup>�̂Œf�O���܂���.

���̑���Ɏ��O�ō�����̂����� class �ł�.
�u�g�[�X�g�v�ł̔��ȓ_�𓥂܂�, �Ȃ�ׂ������ȃE�B���h�E���Ȃ�ׂ��ז��ɂȂ�Ȃ��悤�ɏo�����Ƃ�S�|���Ă��܂�.

���� class �� contructor �� window �̐����܂ōs���܂�.
�Ȃ̂�, �����o�[�ϐ��Ƃ��Ă��� class ���������ł� window �\���̏������ł��Ă����ԂɂȂ�܂�.
������, ������Ԃł� window �͈�������ł��܂�.

Window ��������̂�,
���� class ���񋟂��Ă��� [`SetText( CString strText )`](#settext) �Ƃ����֐��ɕ������^�����Ƃ��ł�.
�^����ꂽ�����񂪎��܂�T�C�Y�� window ���\������܂�.

������͕����s�����Ă��\���܂���. ���̏ꍇ�͍s�Ԃ� `'\n'` �ŋ�؂�܂�.
2�̉��s `"\n\n"` �ŋ�؂��, �\�����ڂ̋�؂�ƌ��Ȃ�, �O��̍s�̊Ԃɉ����������܂�.
1�s�̒��Ƀ^�u���� `'\t'` �������, �^�u�����̑O�̃e�L�X�g�����񂹂�, �^�u�����̌�̃e�L�X�g���E�񂹂ɕ\�����܂�.

�\�����ꂽ window �̓}�E�X�ŃN���b�N�����, �T�N�b�ƈ������݂܂�.

����, �v���C�}���[���j�^�[������ւ��悤�Ȏg���������Ă���l
( ��c����A���Ă�����m�[�gPC�����Ȃ̃��j�^�[�ɂȂ��ł�������v���C�}���[�ɐݒ肵�Ă���悤�Ȑl�Ƃ� )
�����̎蓖�ĂƂ���,
window �\�����Ƀ��j�^�[�̕ω�����������,
�V�����v���C�}���[���j�^�[��ɕ\�����ړ�����d�|���������Ă��܂�.

�Ȃ�, �\�������D�݂ɍ��킹��ۂɃ\�[�X�R�[�h�������炸�ɍςނ悤,
�D�݂̕����ꂻ���Ȑݒ�l�� registry ����ǂނ悤�ɂ��Ă��܂�.
������, ���� registry �l��ύX���邽�߂� GUI �͖{�A�v���ł͗p�ӂ��Ă���܂���.
�ǁ[���Ă��ύX�������l��, �e������� Registry Editor �ł������Ă�������.

<sub>* MS �́u�g�[�X�g�v�Ƃ����l�[�~���O��������Ă���̂�,
[�u�A�v���ʒm�v�Ƃ��������ɋ�J�������Ȗ��O�ɑւ��悤�Ƃ��Ă���炵���ł�.](https://learn.microsoft.com/ja-jp/windows/apps/design/shell/tiles-and-notifications/adaptive-interactive-toasts)
<br>
** �Ȃ�Ƃ�����, �d�|�����傰��������Ƃ�����, ������ C# �����ᒆ�ɂȂ��Ƃ������c�c. <sub>


## `CNotifyWnd`

���� class �� constructor �ł�.

�܂�, �����o�[�ϐ������������Ă��܂�.
�������Ȃ���
[C26495](https://learn.microsoft.com/en-us/cpp/code-quality/c26495)
���w�E����ăE�U������, �Ƃ������R�ł���Ă���̂ł���,
�J�X�^�}�C�Y�\�Ȑݒ�l�Ɋւ��Ă͂����� registry ����ǂݍ���ł��܂�.

���� registry �L�[��

`HKEY_CURRENT_USER\Software\ChkMails\NotifyWnd\`<p>

�ɂ��� ( �{�A�v�����f�t�H���g�ł��g���̏ꍇ��, ��L�̃L�[���̂����݂��܂���. ),
�f�t�H���g�ł͂��ꂼ��̐ݒ�l�����\�̂悤�ɂȂ��Ă��܂�.

| Name | Type | Data |
| --- | --- | --- |
| `FontName`	| `REG_SZ`	| `"Arial"`	|
| `FontSize`	| `REG_DWORD`	| `14`[pix]	|
| `Toast`	| `REG_DWORD`	| `1` (`true`)	|
| `Popup`	| `REG_DWORD`	| `400`[ms]	|
| `crText`	| `REG_DWORD`	| `0x00ffffff` (`RGB( 255, 255, 255 )`)	|
| `crBack`	| `REG_DWORD`	| `0x00000000` (`RGB( 0, 0, 0 )`)	|
| `crLine`	| `REG_DWORD`	| `0x007f7f7f` (`RGB( 127, 127, 127 )`)	|
| `Alpha`	| `REG_DWORD`	| `168` (�Ȃ�Ƃ��w�i���������Č�������x�̓��ߓx)	|

���̂��� `Toast` �́u�g�[�X�g����(����ɂ�������)�����邩�ۂ��v�̃X�C�b�`�ł�.<br>
`0` �Ȃ炻��ȓ���͂���, window �����ʂɕ\������܂�.<br>
`1` �Ȃ� window ���Ă��オ�����g�[�X�g�̂��Ƃ�, �������Ƀ|���Ƃ���オ���Ă��܂�.

`Popup` �̓g�[�X�g���オ�肫��܂łɂ����鎞�� [ms] �ł�.
���̎��Ԃ𒷂�����ƃg�[�X�g�͂������Ƃ���オ���Ă���悤�ɂȂ�,
���܂�u�g�[�X�g���v���Ȃ��Ȃ��Ă��܂�.
�Ȃ�, �\�������s�����Ă��オ�肫�鎞�Ԃ͓����ݒ�l�Ȃ̂�,
�\���s���������������� window �̂���オ�鑬�x�͍����Ȃ�܂�.

��, �����o�[�ϐ��̏��������I����,
������ window �� create ���܂�.
�s�V�̗ǂ� window ��, constructor �����ł� create �܂ł���Ƃ��������͂���,
�O���� create �����`�����Ƃ�̂����ʂ��Ǝv���܂���,
�� class �ł͉������Ă��܂�.
�\�t�g�E�F�A���i�Ƃ��Ă̗��p�̎�y����D�悵�܂���.
<br>
<sub>
( ���_��, ���s���邱�Ƃ����蓾�� create ��, �u���s�����v�Ɩ߂�l�ŕԂ����Ƃ��ł��Ȃ� constructor �ł��̂�,
���X���s�V��������ł����ǂ�. )
</sub>

## `SetText`

�\�����镶�����ݒ肵�܂�.

���̏����菇�͈ȉ��̒ʂ�ł�:

1. �^����ꂽ������̑O��̋󔒂�
[`Trim`](https://learn.microsoft.com/ja-jp/cpp/atl-mfc-shared/reference/cstringt-class?#trim)
����.
1. �����񂪋󂾂�����, window ���������߂�.
1. Device context �� get ���ăt�H���g��I��.
1. ������� `\n` �܂ł�؂�o��. ���s���Ȃ������當������Ō�܂ł���荞��.
1. ������̃T�C�Y�𓾂�.
1. ������ `\t` �Ŏd�؂��Ă�����, �d�؂�̕��Ƃ��ă}�[�W���������ɉ�����.
1. ����������܂ł̍ő�l��������, �ő�l�����ւ���.
1. �T�C�Y�̏c���� window �̍����ɉ�����.
1. �^����ꂽ������̖��[�ɒB����܂� 4. ������J��Ԃ�.
1. Device conntext ���J������.
1. Window �̃T�C�Y�Ƀ}�[�W����������.
1. [`PlaceWindow`](#placewindow) ���Ă�� window �̈ʒu���m�肷��.
1. �u�g�[�X�g����v������ݒ�Ȃ炻�̓����������, ���Ȃ��ݒ�Ȃ炻�̂܂� window ��\��.

�u�g�[�X�g����v������ꍇ��, �����ł͂܂� window ��\�����Ȃ� ( �B�����܂� ) �Ƃ����̂��~�\��,
[`PlaceWindow`](#placewindow) �� window �̈ʒu����܂��Ă��B�����܂�,
���ۂɕ\�������̂́u�g�[�X�g����v�ɓ����Ă���ł�.


## `OnCreate`

Window Message [`WM_CREATE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-create)
�ɑ΂���n���h���[�ł�.

�܂���� class �� `OnCreate` ���ς܂�����,
���L�̂悤�� window �̊g���X�^�C����ݒ肵�܂�.

| �g���X�^�C��		| �ړI |
| --- | --- |
| `WS_EX_LAYERED`	| �w�i�������Č����锼������ window ��. |
| `WS_EX_TOPMOST`	| ���� window �ɃJ�u���ĉB�ꂽ�肵�Ȃ��悤�� |
| `WS_EX_TOOLWINDOW`	| �^�X�N�o�[�ɂ��� window �̃^�C�g�������ז����Ȃ��悤�� |

������
[`SetCursor`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-setcursor)
��, ���� window ��̃}�E�X�J�[�\���̌`�������ӂꂽ���ɐݒ肵����,
[`SetLayeredWindowAttributes`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-setlayeredwindowattributes)
��, ���� window �̓����x ( �s�����x ) ��ݒ肵�܂�.

�Ō��, ���� window �ŕ�����\�����s�����߂̃t�H���g������Ă����Ċ����ł�.

�������� `lpCreateStruct` ��������Ă���̂�����,
�g���X�^�C���̐ݒ�� `lpCreateStruct->dwExStyle` �ɂ���Ă����Ηǂ������Ȃ��̂ł���,
�����؂�f�̂܂� `CWnd::OnCreate` ��ʂ��Ă����ăJ�X�^�}�C�Y�͌ォ��{��,
�Ƃ����̂��Ȃ�Ƃ�����, �l�����݂����Ȃ���ł�.


## `OnEraseBkgnd`

Window Message [`WM_ERASEBKGND`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-erasebkgnd)
�ɑ΂���n���h���[�ł�.

���������� `return` �ł�.

���� `CWnd::OnEraseBkgnd` �̖������́u��ʂ�S�����O�ŕ`�悵�Ă��� window �ƊE�v�ɂ����Ă悭�m���Ă���
�u��ʂ̂������}������@�v�Ȃ̂ł���,
�ƊE�̈���Ƃ��Ċ��K�ɏ]���Ă݂܂���.

��, ���� window �̏ꍇ�قڌ��ʂ͂Ȃ��͂��ł���.


## `OnLButtonDown`

Window Message [`WM_LBUTTONDOWN`](https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-lbuttondown)
�ɑ΂���n���h���[�ł�.

�������� window ���������߂܂�.
�ǂ������ꂽ�̂��ɂ�����炸�ɂł�.


## `OnPaint`

Window Message [`WM_PAINT`](https://learn.microsoft.com/ja-jp/windows/win32/gdi/wm-paint)
�ɑ΂���n���h���[�ł�.

�X�V���ꂽ rectangle ��w�i�F�œh��Ԃ�����,
[`Draw`](#draw) �ɕ`����ς˂܂�.


## `OnTimer`

Window Message [`WM_TIMER`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-timer)
�ɑ΂���n���h���[�ł�.

���L�̃^�C�}�[�l����舵���Ă��܂�.

| �^�C�}�[�l | �� | �������e |
| --- | --- | --- |
| `TID_TOAST` | �u�g�[�X�g���쒆�v | [`ToastWindow`](#toastwindow) �Ńg�[�X�g����. �g�[�X�g�����Ȃ�^�C�}�[����. |
| `TID_MONITOR` | �u���j�^�[�ؑ֌�v | [`PlaceWindow`](#placewindow) �� window ���ړ�. 1�񂫂�Ń^�C�}�[����.|


## `OnDisplayChange`

Window Message [`WM_DISPLAYCHANGE`](https://learn.microsoft.com/ja-jp/windows/win32/gdi/wm-displaychange)
�ɑ΂���n���h���[�ł�.

Window ����������ł��Ȃ����,
��U window ���������߂����,
�^�C�}�[ `TID_MONITOR` �� `5`�b��ɂ������Ă��̏�͏I���܂�.
�Ȃ�� `5`�b���҂�����̂��Ƃ�����,
���j�^�[�ؑ֒���̂������������߂����ɂ�, ���ꂮ�炢�̂�Ƃ肪�~�����悤�Ɍ���������ł�.<sup>��</sup>
<br>
<sub>
�� �l�̊ώ@�ł��B
</sub>


## `PlaceWindow`

Window ��������ׂ��ʒu�ɔz�u���܂�.

���́u������ׂ��ʒu�v�Ƃ����̂�,
�^�X�N�o�[�̒ʒm�̈�<sup>* </sup>�Ɋ��Y���悤�Ȉʒu�ł�.
���̒ʒm�̈�<sup>* </sup>�̓v���C�}���[���j�^�[�ɂ���̂�,
�܂�
[`MonitorFromPoint`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-monitorfrompoint)
��, �v���C�}���[���j�^�[�̃n���h��<sup>**</sup>�𓾂܂�.

��, �v���C�}���[���j�^�[�̃n���h����
[`GetMonitorInfo`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-getmonitorinfow)
�����,

* ���j�^�[�̕\���̈�
* ���j�^�[�̍�Ɨ̈�

�������܂�.
���̗��̈�̍�����, �^�X�N�o�[�Ő�߂��Ă���̈�Ƃ������ƂɂȂ�܂�.

���Ƃ�, ���̍����Ɋ�Â��ă^�X�N�o�[�����j�^�[�̎l�ӂ̂�����ɂ���̂�������,
���̂��肩�ɉ����� window �̈ʒu�����肵�܂�.

�Ō��
[`SetWindowPos`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-setwindowpos)
��, window �����肵���ʒu�Ɉړ����Ċ����ł�.

<sub>* [���́u�ʒm�̈�v���u�^�X�N�g���C�v�ƌĂԐl�͐��オ�Â������ł�.](https://www.yrl.com/column/wazaari_pc/taskbar.html)<br>
�@�������u�^�X�N�g���C�v���u�ʒm�̈�v�Ƃ���, �u�g�[�X�g�v���u�A�v���ʒm�v�Ƃ���, �ŋ߂� Microsoft&reg; �̌��t�I�т͌������Â炭�Ȃ����ł�.
<br>
** ���W ( 0, 0 ) ���܂ރ��j�^�[���`���C�X����΂��ꂪ�v���C�}���[���j�^�[�ł�.
[Windows&reg; �ɏڂ����������񂪂����f�����Ă��܂���](https://devblogs.microsoft.com/oldnewthing/20070809-00/?p=25643). <sub>


## `ToastWindow`

Window ���u�g�[�X�g�v���܂�.

�ǂ��܂Łu�g�[�X�g�v�������Ɋ�Â�, window �̈ʒu���ړ����܂�.

* �u�g�[�X�g�v�̎n�܂�̏ꍇ:<br>
[`PlaceWindow`](#placewindow)����́u�ŏI�ʒu�v�ɂ���͂��Ȃ̂ł��̈ʒu���L����,
�^�X�N�o�[�ɂ��ꂢ�ɉB���ʒu��I��ł���
[`ShowWindow`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-showwindow)
���܂�.
* �u�g�[�X�g�v�̏I���̏ꍇ:<br>
�n�܂�̂Ƃ��ɋL�����Ă������u�ŏI�ʒu�v��I�т܂�.
* �u�g�[�X�g�v�̓r���̏ꍇ:<br>
�ǂ��܂Łu�g�[�X�g�v�������ɉ������ʒu��I�т܂�.

��, �I�񂾈ʒu��
[`SetWindowPos`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-setwindowpos)
���܂�.

��������Ȃ̂�, �^�X�N�o�[�����j�^�[�̏�ɂ���ꍇ��,
���̏ꍇ�� window ���ォ�牺�ɍ~��Ă��铮���ɂȂ�܂�.
�܂����̏ꍇ����, ���������тɉ�ʂ̍ĕ`�悪�K�v�ɂȂ�܂�.
�^�X�N�o�[�����������ݒ�ɂ��Ă���l, ����܂苏�Ȃ���ł����ǂ�.
���������͑Ή����Ă����܂����.


## `Draw`

Window ��`�悵�܂�.

�܂�, window �̕��ɉ����āu�ŏ��� 1�s�v�� rectangle �����߂Ă����܂�.
���ɕ`�惂�[�h���u�w�i�Ȃ��v�ɂ��ĕ����̐F��ݒ肵�܂�.

���Ƃ�, �^�����Ă����e�L�X�g�̖��[�܂�, �ȉ��̏������J��Ԃ��܂�.

1. 1�s���̃e�L�X�g��؂�o��.
1. �󔒍s�Ȃ�, window �̍��[����E�[�܂Ő�������.
1. �󔒍s�łȂ����,<br>
`\t` �ŋ�؂�ꂽ�s�Ȃ�, `\t` �̑O(��)������, `\t` �̌�(�E)���E�񂹂ł��ꂼ��e�L�X�g�`��.<br>
`\t` �ŋ�؂��Ă��Ȃ��s�Ȃ�, �s�S�̂��e�L�X�g�`��.
1. rectangle �� 1�s�����Ɉړ�.

��������Ȃ̂�, �^�X�N�o�[�����j�^�[�̏�ɂ���ꍇ��,
�`�揈���̌��_��������ɂ��炵��, �e�L�X�g�̒�̕��� window �̒�̕��ɕ`�����悤�ɂ��Ă��܂�.
�^�X�N�o�[�����������ݒ�ɂ��Ă���l, ����܂苏�Ȃ���ł����ǂ�.
���������͑Ή����Ă����܂����.