# `CAuthPage`

## �\��

�\�[�X�R�[�h���,
[`AuthPage.h`](../ChkMails/ChkMails/AuthPage.h) ��
[`AuthPage.cpp`](../ChkMails/ChkMails/AuthPage.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CAuthPage`](#cauthpage-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`OnOK`](#onok)


## �T�v

[Authentication](../README.md#authentication) �̐ݒ���s�����߂́uPage�v�ł�.<br>
[`CProperSheet`](CProperSheet.md) �ɏ��������
[`CFilterSheet`](CFilterSheet.md) ��
���˂Ă��� page �̂����� 1�ł�.

![](../Examples/Filter.Authentication.png)

����܂ł̉^�p��, ���[���T�[�o�[����ǂ� `Authentication-Results:` ���������т�����̂���,
[Main Window](CMainWnd.md) ���� bitmap �l�ł��炢, ���� bitmap �l�ɉ����Ċe authentication �̗L����\�����Ă��܂�.

�ݒ�l�Ƃ��Ắu�����ȏ�� authentication �� pass ���Ȃ������烁�[����j�����邩�v
�Ƃ�������Â炢�l�� 1���邾���ł�.
[`CComboBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/ccombobox-class)
�ŕ\���ƑI�����s���Ă��܂�.

�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.

## `CAuthPage`

���� class �� constructor �ł�.

���\�[�X ID ���w�肵�Ċ��N���X�� constructor ���ĂԂƂ����厖�Ȏd�����ʂ�������,
�����o�[�ϐ������������Ă��܂�.
<sub>
�������Ă����Ȃ���[C26495](https://learn.microsoft.com/ja-jp/cpp/code-quality/c26495)�����邳���̂�.
</sub>


## `OnInitDialog`

[`CPropertyPage::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
�� override �ł�.

�܂��͕��ʂ�
`CPropertyPage::OnInitDialog` ���Ă񂾌�,
( `CPropertyPage` �� `OnInitDialog` �� override ���Ă��Ȃ��悤�Ȃ̂�,
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
�ɃX���[�p�X���ꂽ��, )
 �ȉ��̓Ǝ����������݂܂�.

1. Combo Box �́u�ݒ�l�Ԗځv��I��.
1. �uAuthentication ���� bitmap�v�� bit �������Ă��Ȃ����ڂ̕\���� disable �ɂ���.
1. ���ɓ��̓t�H�[�J�X���ǂ����ɐݒ肵�Ă��Ȃ��̂�, `TRUE` ��Ԃ�.

���ɊȒP�ɂ���Ă��܂���,

* `0` ���܂߂āuCombo Box �̉��Ԗڂ̑I�������v��, �ݒ�l�ƈ�v���Ă���
* ���ږ��� `static` �� ID �̏����� bitmap �� bit �ʒu�ƈ�v���Ă���

�Ƃ������Ɏd�������Ă��邩��ł�.


## `OnOK`

[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
�� override �ł�.

��� class �̎����ł���
[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
���Ă΂���,
�uCombo Box �̉��Ԗڂ̑I�������I�΂�Ă��邩�v
���u�ݒ�l�v�Ƃ��Ċi�[���邾���ł�.

���� `public` �ȁu�ݒ�l�v�� `OnOK` �̌�,
[Main Window](CMainWnd.md) ������ɉ�����Ă����蔤�ɂȂ��Ă��܂�.

�uAuthentication ���� bitmap�v�͏ォ����炤�����œ� class �ł͕ύX���Ȃ�����ʍs�̒l�Ȃ̂�,
���ɉ������܂���.
[`OnInitDialog`](#oninitdialog) �ō��ڂ� enable / disable �Ɏg���Ă��邾���ł�.