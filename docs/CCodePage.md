# `CCodePage`

## �\��

�\�[�X�R�[�h���,
[`CodePage.h`](../ChkMails/ChkMails/CodePage.h) ��
[`CodePage.cpp`](../ChkMails/ChkMails/CodePage.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CCodePage`](#ccodepage-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`OnOK`](#onok)


## �T�v

[Coding](../README.md#coding) �̐ݒ���s�����߂́uPage�v�ł�.<br>
[`CProperSheet`](CProperSheet.md) �ɏ��������
[`CFilterSheet`](CFilterSheet.md) ��
���˂Ă��� page �̂����� 1�ł�.

![](../Examples/Filter.Coding.png)

�`�F�b�N�{�b�N�X�� ON/OFF ���,
bitmap �l�Ƃ��� [Main Window](CMainWnd.md) �Ƃ���肵�Ă��܂�.

�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.


## `CCodePage`

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

1. �u&#x2611;���ꂽ���� bitmap�v�� bit �������Ă��� Check Box ��&#x2611;�ɂ���.
1. ���ɓ��̓t�H�[�J�X���ǂ����ɐݒ肵�Ă��Ȃ��̂�, `TRUE` ��Ԃ�.

���ɊȒP�ɂ���Ă��܂���,

* Check Box �� ID �̏����� bitmap �� bit �ʒu�ƈ�v���Ă���

�Ƃ������Ɏd�������Ă��邩��ł�.


## `OnOK`

[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
�� override �ł�.

��� class �̎����ł���
[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
���Ă΂���,
�u�ǂ̍��ڂ�&#x2611;����Ă��邩�� bitmap�v
���u�ݒ�l�v�Ƃ��Ċi�[���邾���ł�.

���� `public` �ȁu�ݒ�l�v�� `OnOK` �̌�,
[Main Window](CMainWnd.md) ������ɉ�����Ă����蔤�ɂȂ��Ă��܂�.

