# `CProperSheet`

## �\��

�\�[�X�R�[�h���,
[`ProperSheet.h`](../ChkMails/ChkMails/ProperSheet.h) ��
[`ProperSheet.cpp`](../ChkMails/ChkMails/ProperSheet.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CProperSheet`](#cpropersheet-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`BuildPropPageArray`](#buildproppagearray)

#### �W�� Window Message �n���h���[

[`OnGetMinMaxInfo`](#ongetminmaxinfo)
[`OnSize`](#onsize)

#### �ŗL�֐�

[`OnCallback`](#oncallback)

#### �ŗL class

[`CDialogTemple`](#cdialogtemple)


## �T�v

[`CFilterSheet`](CFilterSheet.md) �̉��~���ɂȂ��Ă����� class �ł�.
���̂܂���ꂪ���c
[`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�Ȃ�ł���, ���c�̃f�t�H���g�̓��������낢��_���Ȃ̂�,
���̃_�����������y�����邽�߂ɊԂɋ���ł��܂�.

�ǂ��y�������̂��Ƃ�����:

* [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�ł�, ���̂������������Ⴂ�������o���Ȃ��̂�, �����������₷�����ɒu�������Ă���.
* [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�ł�, �V�[�g�̃T�C�Y�͕ς����Ȃ� ( resizable ����Ȃ� ) �̂�, �ς�����悤�ɂ��Ă���.

��2�_�ł�.

�����̌y���ɂ��, ���c���uProper�v( �K�؂� ) �uSheet�v( �V�[�g ) �ɂȂ������̂Ǝ������Ă���܂�.<br>
�ȏ�, �u�Ȃ�����ȃp�`�����������l�[�~���O�ɂ����̂��H�v�ł���.<br>
<sup>( �t�Ɍ��c [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class) ��,
�ǂ����Ă���ȃ_���_���Ȏd�l�̂܂ܐ��ɏo����Ă���̂������ɋꂵ�݂܂�. )</sup>


## `CProperSheet`

���� class �� constructor �ł�.

��� class
[`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�� constructor ���ĂԂƂ����厖�Ȏd������������,
�����o�[�ϐ������������Ă��܂�.
<br>
<sub>
�������Ă����Ȃ���[C26495](https://learn.microsoft.com/ja-jp/cpp/code-quality/c26495)�����邳���̂�.
</sub>

## `OnInitDialog`

[`CPropertySheet::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#oninitdialog)
�� override �ł�.

�������e�͈ȉ��̒ʂ�ł�.

1. ��� class �� [`CPropertySheet::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#oninitdialog) ���Ă�.
1. �V�[�g�̍ŏ��̃T�C�Y�������Ă���.
1. �V�[�g��� [`CTabCtrl`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/ctabctrl-class) �� rectangle �𓾂�,
�V�[�g�Ƃ̊Ԃ̃}�[�W�� ( �E�E�� ) ���Z�o���Ă���.
1. �V�[�g��� [`CPropertyPage`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class) ��
rectangle �𓾂�, `CTabCtrl` �Ƃ̊Ԃ̃}�[�W�� ( �E�E�� ) ���Z�o���Ă���.
1. �V�[�g��� button �� window handle ��S�ċL�^���Ă���.
1. �u�������ς݁v�Ƃ̃t���O�𗧂ĂĂ���.
1. [`CProperSheet::OnInitDialog`](CProperSheet.md#oninitdialog) �̖߂�l��Ԃ�.

�Ƃ��Ă��܂�.

[`CPropertySheet::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#oninitdialog) �̖߂�l�Ɋւ�炸,
��L 2.�`6. �̏�����i�߂Ă��܂��Ă���Ƃ��낪,
�ς��ƌ��u��� class �̖߂�l�𖳎����Đi�߂Ă��܂��Ă����̂�?�v�Ƃ�����ۂ�^���܂���,
���̖߂�l��

* `TRUE`: ���̓t�H�[�J�X���ǂ��ɐݒ肷�邩�́u���܂����v��.
* `FALSE`: ���̓t�H�[�J�X�͂������Őݒ肵������, ����ɕς��Ȃ��ł�.

�Ƃ������x�̂��̂Ȃ̂�,
`OnInitDialog` �����s�����ɂ�������炸�˂������Ă���킯�ł͂���܂���.

�}�[�W���� Button �̋L�^��, [�T�C�Y�ύX](#onsize)�̂��߂̕z�΂ł�.

## `DoModal`

`CPropertySheet::DoModal` �� override �ł�.

[`CPropertySheet::DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#domodal)
���ĂԑO��
[`OnCallback`](#oncallback) �ւ̃R�[���o�b�N���������Ă����܂�.

`DoModal` �ɓ����Ă��܂�����͎�o�����ł��Ȃ��Ȃ�܂���,
���̃R�[���o�b�N���������Ă�������������,

* [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�ł�, ���̂������������Ⴂ�������o���Ȃ�.
* [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�ł�, �V�[�g�̃T�C�Y�͕ς����Ȃ� ( resizable ����Ȃ� ).

�� 2�_�����P����悤, ������邱�Ƃ��ł���悤�ɂȂ�܂�.


## `BuildPropPageArray`

`CPropertySheet::BuildPropPageArray` �� override �ł�.

`CPropertySheet::BuildPropPageArray` �̑��݂�
[Microsoft �̃y�[�W](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�ɂ͊J������Ă��܂���. <sub>( �Ȃ�? )</sub>,

������,
`C:\Program Files\Microsoft Visual Studio\2022\`_product_`\VC\Tools\MSVC\`_version_`\atlmfc\include\`
������� ( ���Ԃ� ) ����
`afxdlgs.h` �������,
�m���� [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�̃����o�[�̉��z�֐��Ƃ��Ď��݂��Ă��邱�Ƃ��m�F�ł���Ǝv���܂�.
�����葁������񂾂�����
[github ��̃R�R](https://github.com/mirror/winscp/blob/master/libs/mfc/source/dlgprop.cpp)
�ł�����.

���������󋵂����
[`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
�̃_�����������L���Ă��Ă��܂���,
�߂����ɐ�֐i�݂܂��傤.

��, ���̊�� class �� `CPropertySheet::BuildPropPageArray` ���Ă��, �܂��͋`�����ʂ����܂�.
���ꂩ��:

1. [Main Window](CMainWnd.md) �� `GetFont` ����,�u���₷���t�H���g�v�����������Ă���.
1. �����������t�H���g�� [`LOGFONT`](https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-logfontw) �𓾂�.
1. `m_psh.ppsp` ���N�_�ɑS page ��, �ȉ��̏������s��.<br>
���� page �� [`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate) �𓾂�.<br>
`DLGTEMPLATE` �ɑ΂��� [`CDialogTemple`](#cdialogtemple) �� `Attach`.<br>
`CDialogTemple` �Łu���₷���t�H���g�v�� `SetFont`.<br>
����Ń~�b�V�����R���v���[�g�Ȃ̂�, `CDialogTemple` �� `Detach`.

�Ƃ�������������ł���܂�.

�����葁��,
�u[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
���̃������[�𒼐ڂ��������ăt�H���g����������������v�Ƃ����������܂���,
�I���W�i����蒷���t�H���g���ɂ��邱�Ƃ͂ł��܂���.
����͐��U�@(?)��

* `CDialogTemplate` ( ��[�h���i](#cdialogtemple) ) �����Ă����� `SetFont` ����

�Ƃ�����i���̗p���܂���.

���̏����ɂ��,
�V�[�g�̒��̊e page �̃t�H���g�́u���₷���t�H���g�v�ɒu�������܂�.<br>
�V�[�g���̂̃t�H���g ( tab �� `OK` �{�^���ɕ\���t�H���g ) ��,
[`OnCallback`](#oncallback) �Œu�������Ă��܂�.


## `OnGetMinMaxInfo`

Window Message [`WM_GETMINMAXINFO`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-getminmaxinfo)
�ɑ΂���n���h���[�ł�.

[`OnInitDialog`](oninitdialog) �ŉ����Ă������V�[�g�̍ŏ��̃T�C�Y��,
�u�ŏ��T�C�Y�v�Ƃ��ĕԂ��܂�.
����ɂ��, �V�[�g�̒[�� drag ���Ă����̃T�C�Y��菬�����͂ł��Ȃ��Ȃ�܂�.
���������Ă��Ӗ��͂Ȃ��̂œ���Ă����܂���.
�傫�����镪�ɂ͈Ӗ������� ( �ꍇ������ ) �̂ŕs����ł�.


## `OnSize`

Window Message [`WM_SIZE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-size)
�ɑ΂���n���h���[�ł�.

�u�������ς݁v�܂� [`OnInitDialog`](#oninitdialog) �ς݂̏ꍇ�͈ȉ��̏������s���܂�.

1. �V�[�g�� rectangle �𓾂܂�.
1. �V�[�g��� [`CTabCtrl`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/ctabctrl-class) �� rectangle �𓾂܂�.
1. `CTabCtrl` �̃T�C�Y�����킹�܂�.
1. �V�[�g�� button �̈ʒu�����킹�܂�.
1. �V�[�g�̂��ܑI�΂�Ă��� page �̃T�C�Y�����킹�܂�.

������, ������ɂ��挻�݂̃V�[�g�� rectangle �������Ă����܂�.

���� rectangle �� 4. �̉ߒ���,
�����̃}�[�W������ʒu���Z�o���鍶���� button ��,
�E���̃}�[�W������ʒu���Z�o����E���� button ��
��؂蕪���邽�߂�, ����� `OnSize` �ŗp�����܂�.

�V�[�g�̒[������͂񂾂܂�,
�}�E�X�p�b�h���S�V�S�V����C�L�I�C�� resize ���J��Ԃ���,
�Ƃ�����`�悪�`�����悤�Ɍ����܂���,
�f�� resizable �ȃ_�C�A���O�ł������󋵂������̂�,
���̕ӂ� [DirectX&reg;](https://ja.wikipedia.org/wiki/Microsoft_DirectX) �Ƃ����g���Ă��Ȃ�
�f�� Windows �̑f�̕`��̌��E�Ȃ̂ł��傤.
���������p�ɉ���x��͂���܂���.


## `OnCallback`

�V�[�g�œ������������ۂɌĂ΂��R�[���o�b�N�֐��ł�.

[���̃R�[���o�b�N�֐�](https://learn.microsoft.com/ja-jp/windows/win32/api/prsht/nc-prsht-pfnpropsheetcallback)��,
3��ނ̋ǖʂŌĂ΂�邱�ƂɂȂ��Ă��܂���,
�����ł̓V�[�g���쐬����悤�Ƃ��Ă��邱�Ƃ����� `PSCB_PRECREATE` �ɉ�����Ă��܂�.
���̏������e�͈ȉ��̒ʂ�:

1. [`AfxGetMainWnd`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/application-information-and-management#afxgetmainwnd) �� [Main Window](CMainWnd.md) �ւ̃|�C���^�[�𓾂�.
1. [Main Window](CMainWnd.md) �� `GetFont` ����,�u���₷���t�H���g�v�����������Ă���.
1. �����������t�H���g�� [`LOGFONT`](https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-logfontw) �𓾂�.
1. `lParam` �� [`DLGTEMPLATE*`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate) �Ƃ���, �ȉ��̏������s��.<br>
`DLGTEMPLATE` �ɑ΂��� [`CDialogTemple`](#cdialogtemple) �� `Attach`.<br>
[`CDialogTemple`](#cdialogtemple) �Łu���₷���t�H���g�v�� `SetFont`.<br>
����Ń~�b�V�����R���v���[�g�Ȃ̂�, `CDialogTemple` �� `Detach`.
1. ���ł� `DLGTEMPLATE` �̃X�^�C���� �uresize �\�� `WS_THICKFRAME`�v�ɂ��Ă���.

�Ƃ�������������ł���܂�.

���̏����ɂ��,
�V�[�g���̂̃t�H���g ( tab �� `OK` �{�^���ɕ\���t�H���g ) ��
�u���₷���t�H���g�v�ɒu�������܂�.<br>
�V�[�g�̒��� page �̃t�H���g��
[`BuildPropPageArray`](#buildproppagearray) �Œu�������Ă��܂�.

����,�u���₷���t�H���g�v������ class �Ɏ�������
[Main Window](CMainWnd.md) ���������ɂ������R�� 1�ɂ��̃R�[���o�b�N�֐�������܂�.
�R�[���o�b�N�֐��Ȃ̂ŃN���X�̃����o�[�ɃA�N�Z�X�ł��܂���.
������ [Main Window](CMainWnd.md) �̓R�[���o�b�N�֐�����ł�
[`AfxGetMainWnd`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/application-information-and-management#afxgetmainwnd)
�o�R�ŃA�N�Z�X�ł���̂�,
�����������u���₷���t�H���g�v�̋������� [Main Window](CMainWnd.md) �ɓ��ꂵ�悤! �ƂȂ����킯�ł�.


## `CDialogTemple`

[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
�ɉ�����邽�߂̓Ǝ� class �ł�.

[`BuildPropPageArray`](#buildproppagearray) ��
[`OnCallback`](#oncallback) �Ŏg�p���Ă��܂�.
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate) ��
`Attach` ������ `SetFont` �������� `Detach` ����Ƃ��� hit and away �ȍU��(?) �̂��߂����Ɏg���Ă��܂�.
�������� `Attach` ����ɕK�v�� `GetTemplateSize` �� <sub>( �ǂ������킯�� )</sub> `protected` �����o�[�Ȃ̂�,
`CDialogTemplate` �̔h�� class �Ƃ��Đ錾���邵���Ȃ��Ƃ����w�i���������,
�� class �����ł̂ݎg���Ǝ� class �Ƃ��č\���Ă���܂�.

���[, �h�����ł���
`CDialogTemplate` ���������Ă��� Microsoft �̃y�[�W������܂���˂�. <sub>( �Ȃ�? )</sub>

���̑��݂�
`C:\Program Files\Microsoft Visual Studio\2022\`_product_`\VC\Tools\MSVC\`_version_`\atlmfc\include\`
������� ( ���Ԃ� ) ����
 `afxpriv.h` �Ŋm�F�ł���͂��ł�.
�����葁������񂾂�����
[github ��̃R�R](https://github.com/mirror/winscp/blob/master/libs/mfc/source/dlgtempl.cpp)
�ł�����.

�� class �����ł̂ݎg���Ǝ� class �Ƃ��Đ錾���Ă���̂�,
Visual Studio&reg; �� `Class View` �ɂ̓f�t�H���g�ŏo�Ă��܂���.
`CProperSheet` �� tree ���J����, ���̎P���Ƃ��� `CDialogTemple` �͎p�������܂�.
<br>
<br>
<sup>
( ���{�l�Ȃ�u�V�[�_�C�A���O�e���v���v�ƌĂт����Ƃ��납������܂���,
�����Ƃ��Ă̐S�Â���́u�V�[�_�C�A���O[�e���v��](https://ja.wikipedia.org/wiki/�e���v��_(�ዾ))�v�ł���.
�_�C�A���O�Ɂu�����|����v�I�ȈӖ���. )
</sup>

