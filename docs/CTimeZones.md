# `CTimeZone`

## �\��

�\�[�X�R�[�h���,
[`TimeZones.h`](../ChkMails/ChkMails/TimeZones.h) ��
[`TimeZones.cpp`](../ChkMails/ChkMails/TimeZones.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�ō\������Ă��܂�.

| �^ | �ϐ��� | ���e | ��� |
| --- | --- | --- | --- |
| `CString` | `m_strKey`	| regstry key			| [`Time Zones\` �z���� key ��](https://learn.microsoft.com/en-us/windows/win32/api/timezoneapi/ns-timezoneapi-time_zone_information#remarks) |
| `CString` | `m_strOffset`	| `UTC+0900` �Ȃǂ̎��ԍ��\��	| registry �� `Display` |
| `CString` | `m_strName`	| �W������			| registry �� `Std` |
| `CString` | `m_strCity`	| `���A�D�y�A����` �Ȃǂ̑�\�s�s��	| registry �� `Display` |
| `CString` | `m_strAbbr`	| `JST` �Ȃǂ̗���			| �Ǝ��ɕt�^ |
| `LONG`    | `m_lBias`	| UTC ����̎��ԍ�[��]			| registry �� `TZI` |
| `LONG`    | `m_lBiasSTD`	| UTC ����̎��ԍ�[��] (�W����)		| registry �� `TZI` |
| `LONG`    | `m_lBiasDST`	| UTC ����̎��ԍ�[��] (�Ď���)		| registry �� `TZI` |
| `SYSTEMTIME` | `m_tSTD`	| �W�����̊J�n����		| registry �� `TZI` |
| `SYSTEMTIME` | `m_tDST`	| �Ď��Ԃ̊J�n����		| registry �� `TZI` |
| `void`    | [`CTimeZone`](#ctimezone-1) | contructor | [C26495](https://learn.microsoft.com/en-us/cpp/code-quality/c26495) ���� |
| `bool`    | [`IsDST`](#isdst) | �^����ꂽ�������Ď��Ԃ����� | �Ǝ����� |

* ��{�I�Ƀf�[�^���i�[���邽�߂� class �Ȃ�ł���,
��O�Ƃ���, [`IsDST( CTime tNow )`](#isdst) �Ƃ����u�^�����������Ď��Ԃ��ۂ��v��Ԃ��֐����t���Ă��܂�.
�������֐��Ƃ��Ă� [constractor](#ctimezone-1) �ɂ͑債���Ӗ��͂���܂���.


## �T�v

1��[�^�C���]�[��](https://ja.wikipedia.org/wiki/������)��\��������𕕓����� class �ł�.

���̃^�C���]�[������, Windows �� registry<p>

[`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Time Zones\`](https://learn.microsoft.com/en-us/windows/win32/api/timezoneapi/ns-timezoneapi-time_zone_information#remarks)

�̉��Ɏ��߂��Ă���f�[�^�x�[�X����擾�������̂ł�.

���� class �̐�����, ��q�� [`CTimeZones`](#ctimezones) �ōs���Ă��܂�.


## `CTimeZone`

���� class �� constructor �ł�.

�����o�[�ϐ������������Ă��܂�.
�������Ȃ���
[C26495](https://learn.microsoft.com/en-us/cpp/code-quality/c26495)
���w�E����ăE�U������, �Ƃ������R�ŕt���Ă���ɂ����܂���.

���̒i�K�ł͈Ӗ��̂Ȃ������l�Ń����o�[�ϐ������������邾����,
�Ӗ��̂���l�Ń����o�[�ϐ������߂���̂�,
���ۂ�[�^�C���]�[��](https://ja.wikipedia.org/wiki/������)��񂪓ǂݍ��܂��
[`CTimeZones::ReadTimeZones`](#readtimezones) �ł̘b�ƂȂ�܂�.


## `IsDST`

�^����ꂽ������[�Ď���](https://ja.wikipedia.org/wiki/�Ď���)���ۂ���Ԃ��܂�.

[`CTime`](https://learn.microsoft.com/ja-jp/cpp/atl-mfc-shared/reference/ctime-class)
�`���ŗ^����ꂽ������,
( `m_tDST` �Ɋi�[����Ă��� ) �Ď��Ԃ̊J�n��������
( `m_tSTD` �Ɋi�[����Ă��� ) �W�����̊J�n�����܂ł̊Ԃɓ����Ă��邩�ۂ����v�Z���܂�.

���̗����́u�J�n�����v�ł���,
���҂� `SYSTEMTIME` �̒��ɉ��L�̂悤�Ɋi�[����Ă��܂�.<sup>*</sup>

| �����o�[ | �Ӗ� |
| --- | --- |
| `wYear` | ���Ӗ��Ȃ̂ŏ�� `0` |
| `wMonth` | �Ď��ԁ^�W�����Ԃ��n�܂�͉̂����� |
| `wDay` | �Ď��ԁ^�W�����Ԃ��n�܂�̂͑扽�j����<br>( `5`�̏ꍇ�͍ŏI�j�� ) |
| `wDayOfWeek` | �Ď��ԁ^�W�����Ԃ��n�܂�̗j�� |
| `wHour` | �Ď��ԁ^�W�����Ԃ��n�܂�͉̂����� |
| `wMinute` | �Ď��ԁ^�W�����Ԃ��n�܂�͉̂����� |


���̏��Ɋ�Â��č��N�̊J�n�������v�Z��, �^����ꂽ���������҂̊Ԃɓ����Ă��邩�����߂܂�.
<br>
<br>
<sub>
*
������� `SYSTEMTIME` �̎g�����Ƃ��Ă͎ד��ł�.
</sub><p>


# `CTimeZones`

## �\��

�\�[�X�R�[�h���,
[`ZonePage.h`](../ChkMails/ChkMails/ZonePage.h) ��
[`ZonePage.cpp`](../ChkMails/ChkMails/ZonePage.cpp)
�Ŏ�������Ă��܂�.

���� class �͈ȉ��̃����o�[�֐��ō\������Ă��܂�.

#### Constructor

[`CTimeZones`](#czonepages-1)

#### Public

[`operator[]`](#operator)
[`GetCount`](#getcount)
[`GetBias`](#getbias)

#### �ŗL�֐�

[`ReadTimeZones`](#readtimezones)
[`MakeAbbr`](#makeabbr)


## �T�v

�S�Ă� [`CTimeZone`](#ctimezone) �𑩂˂Ď�舵�����߂� class �ł�.

�v���O���~���O��̃l�[�~���O�̃e�[�}�ł�,
�u�������O�̒P���`�ƕ����`�������Ďg���ȁB�v�Ɖ��߂��邱�Ƃ�����܂���,
���̏ꍇ�͂��傤������܂���.
 [`CTimeZone`](#ctimezone) �𑩂˂��̂� `CTimeZones` �Ȃ̂ł�.

Constructor ��, Windows&reg; ���������Ă���f�[�^�x�[�X�ɓ����Ă���S�Ẵ^�C���]�[������ǂݏo���Ă��܂�.
�Ȃ̂�, �����o�[�ϐ��Ƃ��� `CTimeZones` �����L���Ă��邾����, ���ł��e�n�̃^�C���]�[�������Q�Ƃ��邱�Ƃ��ł��܂�.

���ݒn�̎����Ɋ�Â��Ċe�n�̎��������߂鉞�p��ɂ��Ă�,
[CZonePage](CZonePage.md) �� [`DrawLocalTime()`](CZonePage.md#drawlocaltime) �Ȃǂ����Q�Ƃ�������.

## `CTimeZones`

���� class �� constructor �ł�.

[`ReadTimeZones`](#readtimezones) ���Ăяo����,
[`CTimeZone`](#ctimezone) �z����\�����܂�.


## `operator[]`

[`CTimeZone`](#ctimezone) �z����Q�Ƃ��邽�߂� `[]` �� operator �ł�.


## `GetCount`

[`CTimeZone`](#ctimezone) �z��̗v�f����Ԃ��܂�.


## `GetBias`

[�^�C���]�[��](https://ja.wikipedia.org/wiki/������)�̗��̂���o�C�A�X�l<sup>*</sup>�𓾂܂�.

����, ���[���w�b�_�[�̑��M������

`Date: Wed, 11 Dec 2024 00:17:26 +0000 (UTC)`

�̂悤�Ƀo�C�A�X�l<sup>*</sup>�ƃ^�C���]�[�����̂𕹋L������̂ł���, ���܂�

`Date: Mon, 23 Dec 2024 09:55:02 GMT`

�Ƃ����^�C���]�[�����̂����ōς܂��Ă��鉡���ȏꍇ������܂�.
<br>
<sup>
(�u�gGMT�h����A���邾��? ���R�B�v�Ƃ����������G�������łȂ񂩃��ł���.
���������gGMT�h����Ȃ��āgUTC�h�Ə����Ă��炦�܂��񂩂�? )
</sup>

���������ꍇ��, ���̗��̂��琔�l�Ƃ��Ẵo�C�A�X�l<sup>* </sup>�𓾂邽�߂̊֐��ł�.
����,
[`CTimeZone`](#ctimezone) �z��𑍃i�����ĊY������ [`CTimeZone`](#ctimezone) ��T�蓖��,
���̃o�C�A�X�l<sup>*</sup>���Q�l�Ɍv�Z�����l��Ԃ��Ă��܂�.

<sub>*
UTC �Ƃ̎��ԍ���\���uMicrosoft�^�C���]�[���ƊE�p��v. [ISO](https://ja.wikipedia.org/wiki/ISO_8601) ��
[RFC](https://datatracker.ietf.org/doc/html/rfc3339) �ƊE�p��ł́uoffset�v.</sub>
<br>
<sup>
�@�Ⴆ�Β����̏ꍇ�� `+0800` �œ��ꂳ��Ă��܂�. ( ����Ȃɓ����ɒ����̂�. )
</sup>


## `ReadTimeZones`

[`CTimeZone`](#ctimezone) �z��̒��g���l�߂܂�.

[`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Time Zones\`](https://learn.microsoft.com/en-us/windows/win32/api/timezoneapi/ns-timezoneapi-time_zone_information#remarks)
�ɓ����Ă���S�Ẵ^�C���]�[������ǂݏo���Ă��܂�.

�\�[�X�R�[�h���������Ă�,
`m_tSTD` �� `m_tDST` ��ǂݎ���Ă���C�z���������Ȃ��̂ł���,
����� `m_lBias` ����n�܂�\�����܂�܂�o�C�i���[�Ƃ��ēǂݎ���Ă��邩��ł�.
���������蔲���ȓǂݎ������ł���悤,
[`CTimeZone`](#ctimezone) �� `m_lBias` �ȉ��̏��\����,
registry �ɓ����Ă���܂�܂̍\�����}�l���Ă��܂�.

## `MakeAbbr`

�^����ꂽ [`CTimeZone`](#ctimezone) ��[�^�C���]�[��](https://ja.wikipedia.org/wiki/������)�̗��̂����܂�.

Windows&reg; �̒��ɂ����Ƀ^�C���]�[�����̂̃f�[�^�x�[�X�Ȃ�Ď�������Ă��Ȃ��悤�Ȃ̂�,
�d���Ȃ����肵���\���Q�Ƃ��ē���Ă��܂�.

�A���t�@�x�b�g�ł������� 3,4 �����̕\�L�Ȃ̂œ��R�Ԃ���킯�ł���
( ���ɁuCST�v�Ƃ� �uChina Standard Time�v�Ȃ̂��u�����W�����v�Ȃ̂��c�c ),
���������ꍇ�ɂ�
�u�o�C�A�X�l�\�L���͂�����ė��̂����Ō����Ă������ȍ��ƒn��v
�����тƓƒf�Ɋ�Â��Č��I���Ă��܂�.