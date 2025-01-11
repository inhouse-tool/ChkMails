# `CZonePage`

## �\��

�\�[�X�R�[�h���,
[`ZonePage.h`](../ChkMails/ChkMails/ZonePage.h) ��
[`ZonePage.cpp`](../ChkMails/ChkMails/ZonePage.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CZonePage`](#czonepage-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`OnOK`](#onok)

#### �W�� Window Message �n���h���[

[`OnTimer`](#ontimer)

#### �R�}���h�n���h���[

[`OnChangeList`](#onchangelist)
[`OnClickHeader`](#onclickheader)

#### �ŗL�֐�

[`OnSelectItem`](#onselectitem)
[`DecodeDST`](#decodedst)
[`DrawLocalTime`](#drawlocaltime)
[`CompareZone`](#comparezone)


## �T�v

[Time Zone](../README.md#time-zone) �̐ݒ���s�����߂́uPage�v�ł�.<br>
[`CProperSheet`](CProperSheet.md) �ɏ��������
[`CFilterSheet`](CFilterSheet.md) ��
���˂Ă��� page �̂����� 1�ł�.

![](../pics/Filter.TimeZone.png)

[�^�C���]�[��](https://ja.wikipedia.org/wiki/������)�̗񋓂⎞�ԍ����v�Z����̂��ʓ|�ȏ����Ȃ̂�,
�����𕕂����߂� [`CTimeZones`](CTimeZones.md)
�Ƃ�����p class �𗘗p���Ă��܂�.
���� [`CTimeZones`](CTimeZones.md) �ł� Windows&reg; ���������Ă���f�[�^�x�[�X����,
�ǂ̂悤�ȃ^�C���]�[��������̂����擾���Ă���̂�,
�� class �ł� Windows&reg; ���݂Ƀ^�C���]�[����񋓂��Ă��܂�.

�I�������^�C���]�[���̌��n������\������Ƃ������ɕK�v���Ȃ��@�\��,
�킴�킴�g�ݍ��܂�Ă��܂�.

����������^�C���]�[���������ł��������₷�����邽��,
���� page ��
[CList Control](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class)
�ɂ�, ���X�g�w�b�_�[������, ���ꂽ�w�b�_�[�ɏ]���ă\�[�g����@�\���t�����Ă��܂�.

�ȉ�, �{ class ���Ɏ������ꂽ�֐����ꂼ��̐����ł�.


## `CZonePage`

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

1. List Control ��S�J�����őI��\������悤�ɐݒ�.
1. List Control ���O���b�h�\������悤�ɐݒ�.
1. List Control �� Check Box ��\������悤�ɐݒ�.
1. List Control �� 5�J�����ǉ�.
1. [`CTimeZones`](CTimeZones.md) �̔z��̑S�Ă̗v�f�ɑ΂��Ĉȉ������s:<br>
���Y�^�C���]�[���̃o�C�A�X�l����o�C�A�X<sup>*</sup>�\�������� ( `"+0900"` �Ȃ� ) �𐶐�.<br>
�J���� 1 �Ƀo�C�A�X�\����������Z�b�g.<br>
�J���� 2 �ɓ��Y�^�C���]�[���� [DST](https://ja.wikipedia.org/wiki/�Ď���) ���{�n��Ȃ獡�� DST ���ۂ�<sup>**</sup>���Z�b�g.<br>
�J���� 3 �ɓ��Y�^�C���]�[���̗��̂��Z�b�g.<br>
�J���� 4 �ɓ��Y�^�C���]�[���̑�\�s�s�����Z�b�g.<br>
�J���� 0 �� Check Box �� OFF �ɂ��Ă���.<br>
���̍s�����s�ڂ����f�[�^�Ƃ��ăZ�b�g. ( �\�[�g�p )
1. List Control �̃J����������������.
1. �u�j���Ώ۔z��v�Ɋ܂܂�Ă����� Check Box ��&#x2611;.
1. &#x2611;�����s��������悤�� List Control ���X�N���[��.
1. ���n�����\���X�V�p�^�C�}�[����������.
1. ���n�����\���̏���X�V����������.
1. �t�H�[�J�X��ݒ肵�Ă��Ȃ��̂�, `TRUE` ��Ԃ�.

<sup>*
UTC �Ƃ̎��ԍ���\���uMicrosoft�^�C���]�[���ƊE�p��v. [ISO](https://ja.wikipedia.org/wiki/ISO_8601) ��
[RFC](https://datatracker.ietf.org/doc/html/rfc3339) �ƊE�p��ł́uoffset�v.
<br>
�Ⴆ�Β����̏ꍇ�� `+0800` �œ��ꂳ��Ă��܂�. ( ����Ȃɍ��y�������ɒ����̂�. )
<br>
** 
�{�A�v���̃��[�U�[�C���^�[�t�F�C�X�͍��ۓI�ɒ��� ( [Neutral] ) ��S�|���Ă���̂�,
�\��������������ł���ׂ��ł�.
�����ȍ��ە����ł̓A�����J�p��ł͂Ȃ��C�M���X�p����g���̂����ʂȂ̂�,
�uDST�v�ȂǂƂ����A�����J�p��ۏo���̌��t�͎g�������Ȃ������̂ł���,
�u�Ď��ԁv�̓C�M���X�p��ł́uSummer Timer�v�Ȃ̂ŗ����ƁuST�v,
������̑΋`��́u�W�����ԁv�́uStandard Time�v�Ȃ̂ŗ����ƁuST�v.
��a�̑I���ł��̕\���̓A�����J�p��ۏo���̌��t���̗p��������ł�.
</sup>

## `OnOK`

[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
�� override �ł�.

��� class �̎����ł���
[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
���Ă΂���, �ȉ��̏��������s���܂�.

* List Control �� Check Box ��&#x2611;���ꂽ�^�C���]�[�����u�j���Ώ۔z��v�Ɋi�[.

���� `public` �Ȕz��� `OnOK` �̌�,
[Main Window](CMainWnd.md) ������ɉ�����Ă����蔤�ɂȂ��Ă��܂�.


## `OnTimer`

Window Message [`WM_TIMER`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-timer)
�ɑ΂���n���h���[�ł�.

���L�̃^�C�}�[�l����舵���Ă��܂�.

| �^�C�}�[�l | �� | �������e |
| --- | --- | --- |
| `TID_SECOND` | �u���n�����\���X�V�v | ���P�ʂ����܂����� [`DrawLocalTime`](#drawlocaltime) �Ăяo��. |


## `OnChangeList`

List Control ��̏�Ԃ��ς���ꂽ���ɔ��ł��� `LVN_ITEMCHANGED` �R�}���h�̃n���h���[�ł�.

�ŏI�I�ɂǂ̍s���I�����ꂽ�̂��m�肽��������
[`CAccountDlg::OnChangeList`](CAccountDlg.md#onchangelist) �ł���Ă��鉡���ȏ����ƈ����,
�����}�W���ɃR�}���h�ɑΉ����Ă��܂�.
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

������, ��L�̏�ԕω��� `1` �������ꍇ��,

* &#x2611;���ꂽ�^�C���]�[���Ɠ������ԍ��̒n��S�Ă���������

�Ƃ����d�������Ă��܂�.

�ȏ�, �`�F�b�N��Ԃ̕ω��Ɋւ��Ă̎d����Еt�����,
�I����Ԃւ̑Ή��� [`OnSelectItem`](#onselectitem) �Ɋۓ������Ă��܂�.


## `OnClickHeader`

List Control ��̃w�b�_�[�����ꂽ���ɔ��ł��� `HDN_ITEMCLICK` �R�}���h�̃n���h���[�ł�.

���L�̂悤�ɏ�����i�߂Ă��܂�.

1. �w�b�_�[��̂ǂ̃J���������ꂽ�̂��𓾂�.
1. ���̃J�����̌��݂́u�t�H�[�}�b�g�v�𓾂�.
1. ���݂́u�t�H�[�}�b�g�v���u�����\�[�g�v�Ȃ�u�~���\�[�g�v�ɕύX.
1. ���݂́u�t�H�[�}�b�g�v���u�~���\�[�g�v�Ȃ�u�\�[�g�Ȃ��v�ɕύX.
1. ���݂́u�t�H�[�}�b�g�v���u�\�[�g�Ȃ��v�Ȃ�u�����\�[�g�v�ɕύX.
1. �ύX�����u�t�H�[�}�b�g�v���w�b�_�[�ɃZ�b�g.
1. ���ꂽ�J�����ȊO�́u�t�H�[�}�b�g�v���u�\�[�g�Ȃ��v�ɕύX���ăZ�b�g.
1. �S�Ă̍s���u�\�[�g�Ȃ��v�ŕ��ג���.
1. �S�Ă̍s��ύX�����u�t�H�[�}�b�g�v�ŕ��ג���.

��L 8. �� 9. �́u���ג����v�ł���,
�Ȃ���U�u�\�[�g�Ȃ��v�ŕ��ג����Ă���w��̃\�[�g���������̂��Ƃ�����,
��U�f�t�H���g�̏�Ԃ� List ����ג������߂ł�.
�{�Ԃ̃\�[�g��, ���̃f�t�H���g�̏�Ԃɑ΂��čs����O��ɂȂ��Ă��܂�.
<br>
<sub>
( [`CWhitePage::SortSender`](CWhitePage.md#sortsender) �ł͂��́u�\�[�g�� 2�x�|���v�͍s�Ȃ킸�� 1�x�ōς܂��Ă��܂���,
���̑���Ɂu`�s�̃f�[�^` �C�R�[�� `�s�̔ԍ� ( �Ƃ������C���f�b�N�X )`�v�ƂȂ�悤, ���I��
[`CListCtrl::SetItemData`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class#setitemdata)
���Ă��܂�.
�����𓝈ꂷ��Ηǂ������Ȃ���ł���,
�\�[�X�R�[�h�ė��p�̂��߂Ɏc���Ă����o���G�[�V�������g������������ł�. )
</sub>

���̃\�[�g��Ƃ̈ꎟ�ϑ����
[`CListCtrl::SortItems`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class#sortitems)
�ł���,
[`CListCtrl::SortItems`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class#sortitems)
�ɂ̓R�[���o�b�N�֐��Ƃ��Ắu��r�֐��v���w�肷��K�v������܂�.
�� class �ł� [`CompareZone`](#comparezone) ���w�肵�Ă��܂���,
���̊֐��ɂ͈ȉ��̕����̈�����^����K�v������܂�.

* List Control �ւ̃|�C���^�[
* �ǂ̃J�������L�[�Ƀ\�[�g���邩
* �\�[�g�́u�t�H�[�}�b�g�v

�ł���, [`CompareZone`](#comparezone) ��
class �̃����o�[�ϐ��ɂ̓A�N�Z�X�ł��Ȃ��R�[���o�b�N�֐���,
�n��������� 1����ł�.
�����ł͂��̈������u�\���̂ւ̃|�C���^�[�v�Ƃ���,
���̍\���̂ɕK�v�ȏ����l�ߍ���ł��܂�.
<br>
<sub>
����Ⴀ, List Control �� ID �͌��ߑł��ɂ���, �c�� 2�� 32bits ���� 1�� `DWORD_PTR` �ɋl�ߍ���,
�Ƃ�����΍ςޘb�Ȃ�ł���,
�����͂����Ɓu��ʉ��v�Ȃ����Œʂ��Ă݂���������ł�.
</sub>


## `OnSelectItem`

List Control ��̑I�����ς���ꂽ���ƂɑΉ����܂�.

[`OnChangeList`](#omchangelist) ��������d���̌㔼��Ƃ���,
[`OnChangeList`](#omchangelist) �����c�����u�I�����ꂽ�s���ς�����v���Ƃɑ΂���d����Еt���܂�.
�܂�,

1. �I�����ꂽ�^�C���]�[���̊T�v�����i�ɕ\������
1. �I�����ꂽ�^�C���]�[���̌��ݎ�����\������悤�ɐݒ肷��

�Ƃ����d���ł�.

���̂��� 1. �́u�^�C���]�[���̊T�v�v�̕\���ł���,
���� DST �̏󋵂�\�����镶����̐����� [`DecodeDST`](#decodedst) �Ɉς˂Ă��܂�.
�܂�, 2. �́u�^�C���]�[���̌��ݎ����v�ł���,
���̕\���� [`DrawLocalTime`](#drawlocaltime) �Ɉς˂Ă��܂�.


## `DecodeDST`

�^����ꂽ `SYSTEMTIME` �Ɋi�[����Ă���u�Ď��ԁ^�W�����Ԃ�������n�܂�̂��v�𕶎���ŕԂ��܂�.

`SYSTEMTIME` �͕��ʂȂ����̓�����\�����߂Ɏg������\���ł���,
�����œn����� `SYSTEMTIME` �͈ꖡ�����, ���L�̂悤�ȏ�����������Ă��܂�.

| �����o�[ | �Ӗ� |
| --- | --- |
| `wYear` | ���Ӗ��Ȃ̂ŏ�� `0` |
| `wMonth` | �Ď��ԁ^�W�����Ԃ��n�܂�͉̂����� |
| `wDay` | �Ď��ԁ^�W�����Ԃ��n�܂�̂͑扽�j����<br>( `5`�̏ꍇ�͍ŏI�j�� ) |
| `wDayOfWeek` | �Ď��ԁ^�W�����Ԃ��n�܂�̗j�� |
| `wHour` | �Ď��ԁ^�W�����Ԃ��n�܂�͉̂����� |
| `wMinute` | �Ď��ԁ^�W�����Ԃ��n�܂�͉̂����� |

�Ȃ̂�, ���̊֐��ł͂��̏��ɉ����������񉻂��s��, ���̕������Ԃ��܂�.

�Ȃ�, �Ď��Ԃ����{���Ă��Ȃ��^�C���]�[��
( `wDay` �� `0` �ŕ\������� )
�ɑ΂��Ă�, ��̕������Ԃ��܂�.


## `DrawLocalTime`

�^����ꂽ�C���f�b�N�X�Ŏ����ꂽ�^�C���]�[���̌��n������\�����܂�.

�����ŗ^������u�C���f�b�N�X�v�Ƃ�,
�� class ���ێ����Ă��� [`CTimeZones`](CTimeZones.md#ctimezones)
�������̃^�C���]�[����\�� [`CTimeZone`](CTimeZones.md#ctimezone) ���Q�Ƃ��邽�߂̃C���f�b�N�X�ł�.
���� [`CTimeZone`](CTimeZones.md#ctimezone) ���猻�n���Ԃ̃o�C�A�X�𓾂�,

* �{�A�v�����ғ����Ă���n��̃o�C�A�X��ł������� UTC �𓾂�,
* �C���f�b�N�X�Ŏ����ꂽ�^�C���]�[���̃o�C�A�X�� UTC �ɉ������Č��n�����𓾂�,
* �C���f�b�N�X�Ŏ����ꂽ�^�C���]�[�����Ď��ԂȂ炻�̎��ԍ������n�����ɉ�������,
* ����ꂽ���n�����𕶎��񉻂��ĕ\������

�Ƃ����i���𓥂�ł��܂�.

## `CompareZone`

List Control �ɂ��鍀�ړ��m���r���܂�.

�^����ꂽ��̍��ڂ̂���, �O�҂���Ɉʒu������̂������� `+1` ��, �O�҂��O�Ɉʒu������̂������� `-1` ��Ԃ��܂�.
�S�������ʒu�������� `0` ��Ԃ��܂�.

[`OnClickHeader`](#onclickheader) ����
[`CListCtrl::SortItems`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class#sortitems)
���o�R���ĊԐړI�ɌĂяo����Ă��܂�.

�ǂ̃J�����̍��ړ��m���r����̂���,
�����Ƃ��ė^����ꂽ�\���̂�������o���܂���,
�J�����ɂ���ĉ��L�̂悤�Ȕ�r���s���܂�.

| �J���� | ��r���e |
| --- | --- |
| `0`: Check Box | &#x2611;����Ă�����̂Ƃ���Ă��Ȃ����̂Ȃ�, ����Ă�����̂��O |
| `1`: `Time` | �o�C�A�X�̏����� ( ���t���ς��̂��x�� ) �����O |
| ���̑� | ������ |

������, �w�b�_�[�� 2�����ă\�[�g���t���ɂȂ��Ă���ꍇ��,
�Ԃ��O��֌W�𔽓]���܂�.

�w�b�_�[�� 3�����ă\�[�g���Ȃ��w��̏ꍇ��,
�I���W�i���̏����ł̑O��֌W��Ԃ��܂�.
���̃I���W�i���̏����Ƃ� ( �Ď��Ԃł͂Ȃ� ) �W�����Ԃł̃o�C�A�X�̑傫�� ( ���t���ς��̂����� ) ���ł�.

Class �̃����o�[�̂悤�ȃt�������Ă��܂���, �R�[���o�b�N�֐��Ȃ̂� class �����o�[�ɂ̓A�N�Z�X�ł��܂���.
�Ȃ̂ŕK�v�ȏ��͑S�Ĉ����Ŏ󂯎���Ă��܂�.

���̊֐���,
�{�A�v���́u�O��\�[�g�p��r�֐��v(
[`CDomainPage::CompareDomain`](CDomainPage.md#comparedomain),
`CZonePage::CompareZone`,
[`CWhitePage::CompareSender`](CWhitePage.md#comparesender) )
�� 1�ł�.