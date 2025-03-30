# ����

[ChkMails](../README.md#chkmails) ��,
Visual Studio 2022&reg; �� [MFC](https://en.wikipedia.org/wiki/Microsoft_Foundation_Class_Library) ���x�[�X��,
�v���O���~���O���� C++ �ŏ����ꂽ�A�v���ł�.
<p>

C++ �Ȃ̂�, ������ class ���Ƃɂ܂Ƃ߂��Ă��܂�.
�{�A�v���͈ȉ��� class �ō\������Ă��܂�.

| class | ���� |
| --- | --- |
| [`CChkMailsApp`](CChkMailsApp.md) | �������S�� |
| [`CMainWnd`](CMainWnd.md) | �{�A�v���̎�� |
| [`CAccountDlg`](CAccountDlg.md) | [Accounts](../README.md#accounts) �ݒ�_�C�A���O |
| [`CFilterSheet`](CFilterSheet.md) | [Filter](../README.md#filter) �𑩂˂�V�[�g |
| [`CProperSheet`](CProperSheet.md) | [`CFilterSheet`](CFilterSheet.md) ���x����u�K�؂ȁv�V�[�g |
| [`CAuthPage`](CAuthPage.md) | [Filter](../README.md#filter) �� [Authentication](../README.md#authentication) �S�� |
| [`CCodePage`](CCodePage.md) | [Filter](../README.md#filter) �� [Coding](../README.md#coding) �S�� |
| [`CDomainPage`](CDomainPage.md) | [Filter](../README.md#filter) �� [Domain](../README.md#domain) �S�� |
| [`CNamePage`](CNamePage.md) | [Filter](../README.md#filter) �� [Name](../README.md#name) �S�� |
| [`CSenderPage`](CSenderPage.md) | [Filter](../README.md#filter) �� [Sender](../README.md#sender) �S�� |
| [`CZonePage`](CZonePage.md) | [Filter](../README.md#filter) �� [TimeZone](../README.md#time-zone) �S�� |
| [`CWhitePage`](CWhitePage.md) | [Filter](../README.md#filter) �� [Whitelist](../README.md#whitelist) �S�� |
| [`CSetupDlg`](CSetupDlg.md) | [Setup](../README.md#setup) �ݒ�_�C�A���O |
| [`CMessageBox`](CMessageBox.md) | [`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)�̑�� |
| [`CNotifyWnd`](CNotifyWnd.md) | ���ǃ��[���̂��m�点�W |
| [`CParaSocket`](CParaSocket.md) | �ʐM�S�� socket |
| [`CTimeZones`](CTimeZones.md) | �^�C���]�[�����񋟌� |

Visual Studio 2022&reg; �ł� C++ �Ȃ̂�,
�����̐����ɂ� MFC �ƊE�p�ꂪ���X�o�Ă��܂���,
��������Ă��Ȃ��P�ꓙ�ɂ��Ă�, web �����Ȃǂŕ���Ă����������,
��������ׂ������Ƃ�����Ȃ��Ă����̂ŏ�����܂�.<br>
<sup>�܂������̓s��������邩, �������c�c</sup>


## �����g�� build ���� debug �������ւ̘A������


### 1. ���ʂȌx��

�T�[�o�[�ƒʐM���邽�тɉ��L�̂悤�Ȍx������ output pane �ɏo�܂���,
<sub>( 2025/1������ Windows&reg; 11 24H2 �ɂĊm�F )</sub>

> `8007277C No such service is known. The service cannot be found in the specified name space.`

����� [Windows&reg; ���̕s���](https://github.com/dotnet/runtime/issues/91677)�ɂ����̂ł�.
�{�i�̕i���ɂ͖�肠��܂���. ���S���Ă����p���������܂�.


### 2. �����ς�����t�@�C���̃^�C���X�^���v

���O ( `.eml` �� `.txt` ) �𒭂߂Ă����,
����[�^�C���X�^���v ( �ŏI�X�V���� ) ���p�^�p�^�ƕς���Ă����Ƃ����s�v�c�Ȍ��ۂ�ڌ����邱�Ƃ�����܂�.](https://superuser.com/questions/1852879/how-to-stop-windows-from-changing-date-modified-when-copying-files-from-network)
<sub>( 2025/1������ Windows&reg; 11 24H2 �ɂĊm�F )</sub>

����� Windows&reg; �̂��킴��,
[KB5039212](https://support.microsoft.com/ja-jp/topic/june-11-2024-kb5039212-os-builds-22621-3737-and-22631-3737-d7f574c0-2b13-48ca-a9fc-a63093b1a2c2)
�ł�炩�����͗l�ł�.

�t�@�C���쐬�����͕ς�邱�Ƃ��Ȃ��̂�, ��������Q�l�ɂ��Ă�������. ������������O�Ɠ��������������Ă��܂�.
<br>
<sup>
�t�@�C���̍X�V������ς���ꂽ�獷���o�b�N�A�b�v�����������Ȃ邾�낤���I
OS �Ƃ��Ă̊�{���������ĂȂ����H Windows&reg;.
</sup>


### 3. ����� assertion failure

Debug build �ɂ�����,
�_�C�A���O��V�[�g���\������Ă����Ԃ�,
���� Close �{�^�� ( ��̐Ԃ��A�� ) �̓��O���}�E�X�ŃE���E��������
tooltip ���o������������߂�������C�悭�J��Ԃ��Ă���Ɓc�c,
( ���}�Q�� )
<br><img src="../pics/Tooltip.png">
<sup>( ��ʂ̏�[�M���M���ɍ\���� tooltip ���o��̂�҂�, �o���� tooltip �̕��ɓ������̂��R�c�ł�. )</sup><br>
�����Ȃ艺�L�̂悤�� assertion �Ɏ��s���邱�Ƃ�����܂�.
<sub>( 2025/1������ Windows&reg; 11 24H2 �ɂĊm�F )</sub>

<img src="../pics/Assertion.png">

���������Ȃ̂� `Retry` �������Ď����������Ă݂��,

* [`WM_NCMOUSEMOVE`](https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-ncmousemove) ��
[`WM_MOUSEMOVE`](https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-mousemove) ���󂯂� `CWnd` ��,
����� Window Message �̈��� window �ɑ΂���
[�g���R���� `CWnd` ����, �܂� window �Ƃ��Đ����Ă���?�h](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-iswindow)
�� `ASSERT` ���Ă݂���, ���̂Ƃ����ł� window �͎���ł���

�Ƃ����󋵂Ɉ������������悤�ł�. ( ���}�Q�� )

![](../pics/AssertionFailure.png)

���̍Č��菇�ł��̏󋵂Ɉ��������肻���� `CWnd` �� tooltip �������܂���. �Ȃ̂ŔƐl�� tooltip �ł�.

�c�c����, �Ɛl�Ă΂��͔G��߂ł���. tooltip �͎����̎d�������Ă��������ł�.
�^�Ɛl��, ���܂��܎����ƕ\���̈悪�J�u���Ă�������Ƃ���������,
tooltip ���Ăɔ��ł��� Window Message ��
`CWnd::FromHandle(pMsg->hwnd)` ���ĕs���ɉ䂪���̈�����,
���ꂾ���ł͖O�����炸 `ASSERT_VALID` ����ߒ��� `IsWindow(m_hWnd)` �܂ł���
 [MFC](https://en.wikipedia.org/wiki/Microsoft_Foundation_Class_Library) ��
`CWnd` �ł�.<br>

��������
[`IsWindow`�̉��](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-iswindow#remarks)��,

> A thread should not use **IsWindow** for a window that it did not create because the window could be destroyed after this function was called.

�y�Ӗ�z<br>
> �悻�� thread ������� window �� `IsWindow` �͎g��Ȃ��łˁB���̊֐����Ă΂ꂽ�ア�j������邩������Ȃ�����B

�Ə�����Ă��܂�.
<sub>
( �������A���ł���,
[�a��](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-iswindow#remarks)�͂����ƃA���������̂�, �����ĈӖ󂵂Ă݂܂���. )
</sub>

���� tooltip �� assertion �Ɏ��s���� thread �Ƃ͖��֌W�� thread �Ő�������Ă��� window �ł���,
�}�E�X�����Ă����ƈ������ސ���������̂�,
�܂��ɂ��̉�����x�����Ă���ʂ�̏󋵂ł�.

����Ȃ̂� tooltip �Ƃ����u�悻�� window�v�ɑ΂��� `IsWindow` �������� `ASSERT` �ł�����,
�߂��d�˂Ă��܂�.

���̌��ۂ� Visual Studio&reg; �Łu�V�K�쐬�v���������� project �ł��Č��ł���̂� <sub>( Version 17.11.6 ���� )</sub>,
�����̂�����͂������̖ڂł��m���߂��������̂��ꋻ����.

`ASSERT` �Ȃ񂩂��Ȃ� Release build �ł͓��R����Ȍ��ۂ͏o�܂���,
���X����������Ȃ̂�, ������J��Ԃ��Ă����R���̖�������܂���.
���S���� debug �����i�߂�������.
��L��ʂ������Ă� `Ignore` ����΂����ł��傤.

���܂�ɂ��傢���傢�����̂ŃC�����Ƃ����� ( `OK` �{�^������Ȃ��ĐԂ��A���� dialog �����h�̕� ) ��,
[`CChkMailsApp::PreTranslateMessage`](CChkMailsApp.md#pretranslatemessage) �̖`����
```
#ifdef _DEBUG
if ( !IsWindow( pMsg->hwnd ) )
    return TURE;
#endif
```
�Ƃł�����ł�����, ����Ȏ����Ɋ������܂ꂸ�ɍς݂܂�.
<br>
<br>
<sup>
���̌��ۂ͍Č���������肷��̂ɋ�J���܂���.
�v���O�����̓���Ƃ͖��֌W�ȃ^�C�~���O�ŏo�邵,
web ��̒N������Ȃ��ƌ����ĂȂ�������.
</sup>


### 4. ���[�N�m�F

���A�v���͏풓�v���Z�X�Ȃ̂�, ���������[�N��n���h�����[�N�Ȃǂ��Ă��Ȃ����Ƃ̊m�F���K�{�ł�.
���A�v���́u10[min] �� 1�񂿂傱���Ɠ����v�Ƃ������̐�����,
[Performance Monitor](https://en.wikipedia.org/wiki/Performance_Monitor)���𗘗p�������[�N�m�F�ɂ�,
���Ȃ�̎��� ( ���Ȃ��Ƃ� 12[h] �ȏ� ) ���K�v�ƂȂ�܂�.

���ɓ��A�v���́u���傱���Ɠ����v�Ƃ��Ɂu���傱���Ɓv�������[���m�ۂ���̂�,
���̃������[�m�ۂ�
�u[LFH: Low Fragmentation Heap](https://learn.microsoft.com/ja-jp/windows/win32/memory/low-fragmentation-heap)�v
�Ƃ��� heap ��ōs���܂�.
���� heap �Ɋւ��� Microsoft&reg;����́u�f�Љ������Ȃ��v�ƃ����b�g����������Ă��܂���,

* [����������̃^�C�~���O���v���O�����̓����ƃY����̂Ń��[�N���Ă��邩�ۂ��̌��ɂ߂����](https://togarasi.wordpress.com/2009/01/14/��f�Љ��q�[�v�ilow-fragmentation-heap�j/)

�Ƃ����f�����b�g������܂�.
<br>
<sub>
( �ň�, �Ȃ��Ȃ����ۂ̊J�����s��ꂸ, �}�W�Ń��[�N���Ă���悤�ɂ��������Ȃ��ꍇ���炠��܂�. )
</sub>

�Z�p�I�ɂ�, �uLFH ����������O�� ( �̂Ȃ���� ) Look-aside List �������w�肷��v
�Ƃ������������邻���ł���,
[Look-aside List �����ɂ͍U���ɑ΂���Ǝ㐫������](https://atmarkit.itmedia.co.jp/ait/articles/1408/28/news010_2.html)�̂�,
���p�I�ɂ� LFH �Ɂu�Â񂶂�v�����Ȃ������ł�.

LFH �̉e�����󂯂āu���[�N���Ă���悤�Ɍ�����v�̂�,
Performance Monitor �̑��荀�ڂ̂���

* Page File Bytes
* Private Bytes

�� 2�ł��傤. ( ���҂͂��ꂢ�ɏd�Ȃ�قǓ����O���t��`���܂�. )

* Virtual Bytes
* Working Set

�� LFH �̉e�����������Ȃ��悤�Ȃ̂�,
����� 2�̐��ڂ𒭂߂�

�u�����A���[�N�͂��ĂȂ��������ȁc�c(���Ԃ�)�B�v

�Ƒ�G�c�ɔ��f���Ă����̂�, �܂�, �u�������v�Ƃ������̂ł��傤��.

<div style="text-align: right;">
In-house Tool / �ƒ� �O
</div>
