# `CDomainPage`

## �\��

�\�[�X�R�[�h���,
[`DomainPage.h`](../ChkMails/ChkMails/DomainPage.h) ��
[`DomainPage.cpp`](../ChkMails/ChkMails/DomainPage.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CDomainPage`](#cdomainpage-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`OnOK`](#onok)
[`PreTranslateMessage`](#pretranslatemessage)

#### �R�}���h�n���h���[

[`OnButtonAdd`](#onbuttonadd)
[`OnButtonDelete`](#onbuttondelete)
[`OnEditDomain`](#oneditdomain)
[`OnChangeList`](#onchangelist)

#### �ŗL�֐�

[`SortDomain`](#SortDomain)
[`CompareDomain`](#comparedomain)


## �T�v

[Domain](../README.md#domain) �̐ݒ���s�����߂́uPage�v�ł�.<br>
[`CProperSheet`](CProperSheet.md) �ɏ��������
[`CFilterSheet`](CFilterSheet.md) ��
���˂Ă��� page �̂����� 1�ł�.

![](../Examples/Filter.Domain.png)

����܂ł̉^�p��, �ǂ̃h���C�����烁�[�����������т�����̂���,
[Main Window](CMainWnd.md) ������s��؂�̕�����ł��炢, ���̕�����ɉ����ăh���C���������X�g�A�b�v���Ă��܂�.

�ǂ̃h���C�����`�F�b�N ( ���� ) ����Ă��邩��,
[Main Window](CMainWnd.md) ������s��؂�̕�����ł�����Ă��܂�.

[`CEdit`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cedit-class)
�ɓ��͂��ꂽ�h���C�����̒ǉ��ɂ��Ή����Ă��܂�.

�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.


## `CDomainPage`

���� class �� constructor �ł�.

�\�[�X�R�[�h�������, ��������Ă��Ȃ��悤�Ɍ����āu�v��́H����B�v�Ƃ��������ł���,
���\�[�X ID ���w�肵�Ċ��N���X�� constructor ���ĂԂƂ����厖�Ȏd�������Ă��܂�.


## `OnInitDialog`

[`CPropertyPage::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
�� override �ł�.

�܂��͕��ʂ�
`CPropertyPage::OnInitDialog` ���Ă񂾌�,
( `CPropertyPage` �� `OnInitDialog` �� override ���Ă��Ȃ��悤�Ȃ̂�,
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
�ɃX���[�p�X���ꂽ��, )
 �ȉ��̓Ǝ����������݂܂�.

1. [`GetCommonFont`](#CChkMailsApp.md#getcommonfont) �ŋ��ʂ̓����t�H���g�𓾂�.
1. Edit Box �ɓ����t�H���g���Z�b�g����.
1. List Control �ɓ����t�H���g���Z�b�g����.
1. List Control ��S�J�����őI��\������悤�ɐݒ�.
1. List Control ���O���b�h�\������悤�ɐݒ�.
1. List Control �� 2�J�����ǉ�.
1. [`SortDomain`](#SortDomain) �Ńh���C���z����\�[�g.
1. �u�h���C���z��v��List Control �ɒǉ�.
1. �u�j���Ώ۔z��v�Ɋ܂܂�Ă����� List Control ��&#x2611;.
1. List Control �̃J����������������.
1. �t�H�[�J�X��ݒ肵�Ă��Ȃ��̂�, `TRUE` ��Ԃ�.


## `OnOK`

[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
�� override �ł�.

��� class �̎����ł���
[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
���Ă΂���, �ȉ��̏��������s���܂�.

* List Control �ɂ���h���C�����u�h���C���z��v�Ɋi�[.
* List Control �� Check Box ��&#x2611;���ꂽ�h���C�����u�j���Ώ۔z��v�Ɋi�[.

������ `public` �Ȕz��� `OnOK` �̌�,
[Main Window](CMainWnd.md) ������ɉ�����Ă����蔤�ɂȂ��Ă��܂�.


## `PreTranslateMessage`

`CProperSheet::PreTranslateMessage` �� override �ł�.

`WM_KEYDOWN` �������Ƃ�, `New domain:` �� Edit Box �ɓ��̓t�H�[�J�X�������ē��̓L�[�� `Enter` ��������,
���̎��ۂ��u`Add` �{�^���������ꂽ�v�ɂ���ւ���
[`OnButtonAdd`](#onbuttonadd) ���Ă΂��悤�Ɏd�����Ă��܂�.

�������� Edit Box �ɉ������ꂽ�̂�,
`Add` �{�^�����������ɃC�L�I�C�� `Enter` �L�[��@�����Ⴄ�������肳�����, ���\����������悤�ł�.
������ page ��� `Enter` �L�[��@���� `OK` �{�^���������ꂽ���ƂɂȂ����Ⴄ�̂�,
���������̓��͂��p�[�ł�.
�Ƃ������Ƃ�, ���̋~�ύ�Ƃ��ċ���ł���S�����ł�.


## `OnButtonAdd`

`Add` �{�^���������ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

`New domain:` �� Edit Box �ɓ��͂��ꂽ�h���C���� List Control �ɒǉ����܂�.

������, ���ł� List Control ���ɑS�������h���C�����G���g���[�ς݂̏ꍇ��,
�ǉ��͂��܂���.
�������̂𕡐��o�^���Ă��Ӗ����Ȃ��̂�.

`Add` ����̃h���C����, List Control ��ł́u�I������Ă���v��ԂɂȂ��Ă��܂�.
�u�ԈႦ���v�Ǝv�����炷������ `Delete` �{�^���������܂��傤.
�� `Add` �����΂���̃h���C�����o�^��������܂�.

Edit Box �ւ̓��͂��I�������_��, ���C�L�I�C�� `Enter` �L�[��@���Ă��܂����Ƃ���,
[`PreTranslateMessage`](#pretranslate) �̐S�����œ��֐����Ă΂�Ă��܂�.


## `OnButtonDelete`

`Delete` �{�^���������ꂽ���ɔ��ł��� `BN_CLICKED` �R�}���h�̃n���h���[�ł�.

�� List Control ��őI������Ă���s�̃G���g���[���폜���܂�.

���́u�I������Ă���v����ɕ\�����邽��,
List Control �̌��l�^�ɂȂ��Ă��郊�\�[�X��ł�,
`Always Show Selection` �� `True` �ɐݒ肳��Ă��܂�.
���̂��A�� List Control �ɓ��̓t�H�[�J�X���������Ă��Ȃ��󋵂ł�,
����Ȃ�Ɂu�I������Ă���v�s���A�s�[������܂�.


## `OnEditDomain`

Edit Box �ŕ����񂪕ύX���ꂽ���ɔ��ł��� `EN_CHANGE` �R�}���h�̃n���h���[�ł�.

* �����񂪋�Ȃ� `Add` �{�^���� disable.
* �����񂪋�łȂ���� `Add` �{�^���� enable.

�Ƃ��Ă��܂�.


## `OnChangeList`

List Control ��̏�Ԃ��ς���ꂽ���ɔ��ł��� `LVN_ITEMCHANGED` �R�}���h�̃n���h���[�ł�.

�ŏI�I�ɂǂ̍s���I�����ꂽ�̂��m�肽��������
[`CAccountDlg::OnChangeList`](CAccountDlg.md#onchangelist) �ł���Ă��鉡���ȏ����ƈ����,
�}�W���ɑS�ẴR�}���h�ɑΉ����Ă��܂�.
�Ȃ��Ȃ�,
<br>
Check Box ��������Ɣ��ł���

* &#x2611;��Ԃ����&#x2611;��ԂɂȂ����Ƃ��� `LVN_ITEMCHANGED`
* ��&#x2611;��Ԃ���&#x2611;��ԂɂȂ����Ƃ��� `LVN_ITEMCHANGED`

�̗����ɂ��ꂼ��Ή�����������ł�.

����� `LVNI_FOCUSED` �̕ω��� `LVNI_SELECTED` �̕ω����ʁX�ɔ��ł���, �Ƃ����璷���Ղ�Ȃ̂�,
���L�̂悤�ɏ�ԕω��𐮗�������őΉ����Ă��܂�.

| �`�F�b�N | �Ӗ� | �Ή� |
| --- | --- | --- |
| `-1` | �ω��Ȃ� | �������Ȃ� |
| `0` | &#x2611;����Ă������̂���&#x2611;���O�ꂽ | &#x2611; �� OFF ����. |
| `1` | &#x2611;����Ă��Ȃ��������̂�&#x2611;���ꂽ | &#x2611; �� ON ����. |

| �I����� | �Ӗ� | �Ή� |
| --- | --- | --- |
| `-1` | �ω��Ȃ� | �������Ȃ� |
| `0` | �I������Ă������̂���I�����O�ꂽ | `Delete` �{�^���� disable. |
| `1` | �I������Ă��Ȃ��������̂��I�����ꂽ | `Delete` �{�^���� enable. |

�I����Ԃւ̑Ή��͍Ō�̏�Ԃɂ����Ή�����Ηǂ��̂ŏ��X�璷�ƌ����܂���,
�`�F�b�N�ւ̑Ή��͗����̏�ԕω��ɑΉ�����K�v������̂�,
��L�̂悤�ɑΉ����Ă��܂�.


## `SortDomain`

��������\�[�g���� `CStringArray` �Ɋi�[���܂�.

`\n` ��؂�� `CString` �Ɏ��߂�ꂽ�h���C����������\�[�g���� `CStringArray` �Ɋi�[���܂�.

�ݒ�l�� registry �Ɋi�[����ۂ� 1�{�̕����񂾂Ɠs�����ǂ��̂ł���,
List �ɕ��ׂĈ������͔z��̕��������₷���̂�,
�������āu�����񁨔z��v�̏������\���Ă��܂�.

�z��ɕ��ׂ�ۂ�, �\�[�g���Ă����ƌ��₷���z��ɂȂ�̂�,
[`CompareDomain`](#comparedomain) �Ŕ�r����, ���̑召�֌W�ŕ��ёւ��Ă��܂�.

## `CompareDomain`

�h���C���������񓯎m���r���܂�.

�^����ꂽ��̃h���C�����̂���, �O�҂���Ɉʒu������̂������� `+1` ��, �O�҂��O�Ɉʒu������̂������� `-1` ��Ԃ��܂�.
�S�����������񂾂����痝�_�� `0` ��Ԃ��̂ł���,
����ȓ��͂� [`OnButtonAdd`](#onbuttonadd) �������Ă��܂���.

������, �P���ɃA���t�@�x�b�g���ɕ�������r����̂ł͂Ȃ�,
[�h���C����](https://www.nic.ad.jp/ja/dom/system.html)���\������v�f���ƂɌ�� ( �E ) �̕������r���܂�.
�܂�, �܂� 2�̃h���C�����́u�g�b�v���x���h���C���v���r��,
����őO�オ���܂�΂��̌��ʂ�Ԃ�, �g�b�v���x���h���C���������Ȃ��2���x���h���C�����r���c�c,
�ƌ��_���o��܂ō��̕��֎��_���ڂ��Ȃ����r���J��Ԃ��܂�.

���̌���, ���L�̂悤�ɕ��ׂ��邱�ƂɂȂ�܂�.

![](../Examples/Filter.Domain5.png)

* ���� `.jp` �Ȃ� `.co.jp` �̕��� `.ne.jp` ���O
* ���� `provider.ne.jp` �Ȃ�, `cat` �̕��� `dog` ���O

�ƂȂ��Ă��܂�.
<br>
( �܂�, `@` �̍����܂œ������[���ł��Ȃ��Ă�������ł���, ���łȂ̂�. )

����, ��̐}�ɂ���ʂ�,
[�g�b�v���x���h���C��](https://ja.wikipedia.org/wiki/�g�b�v���x���h���C��)�ȊO�̃h���C����
( �A�h���X ) �����͂���Ă���ꍇ��,
�h���C���̈Ⴂ������₷���悤�ɉE�l�߂� List ���\������܂�.
���̕ӂ̐S������ [`OnInitDialog`](#oninitdialog) �ł���Ă��܂�.

����V�K�h���C���ǉ�����, ���͂����܂܂̏����œ��͂����܂܂̕����� List �̖����ɉ������Ă����܂�.
����ǉ������̂��������I�ɔ���₷���悤��, �Ƃ̐S�����ł�.
<br>
( �Ȃ̂�, �V�K�ǉ����� `OK` �������� sheet ����Ă���ĂъJ�������,
�\������Ă��鏇�����قȂ�܂�. )

���̊֐���,
�{�A�v���́u�O��\�[�g�p��r�֐��v(
`CDomainPage::CompareDomain`,
[`CZonePage::CompareZone`](CZonePage.md#comparezone),
[`CWhitePage::CompareSender`](CWhitePage.md#comparesender) )
�̂���, �B��R�[���o�b�N�֐��ł͂Ȃ������ƂȂ��Ă���܂�.