#include "pch.h"
#include "CKeyMgr.h"

UINT key_value[(int)Keyboard::Keyboard_end] = 
{
    VK_OEM_3,
    VK_OEM_MINUS,
    VK_OEM_PLUS,
    VK_OEM_4,
    VK_OEM_6,
    VK_OEM_5,
    VK_OEM_1,
    VK_OEM_7,
    VK_OEM_COMMA,
    VK_OEM_PERIOD,
    VK_OEM_2,

    'Q',
    'W',
    'E',
    'R',
    'T',
    'Y',
    'U',
    'I',
    'O',
    'P',
    'A',
    'S',
    'D',
    'F',
    'G',
    'H',
    'J',
    'K',
    'L',
    'Z',
    'X',
    'C',
    'V',
    'B',
    'N',
    'M',

    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',

    VK_F1,
    VK_F2,
    VK_F3,
    VK_F4,
    VK_F5,
    VK_F6,
    VK_F7,
    VK_F8,
    VK_F9,
    VK_F10,
    VK_F11,
    VK_F12,

    VK_ESCAPE,
    VK_TAB,
    VK_CAPITAL,
    VK_LSHIFT,
    VK_LCONTROL,
    VK_LMENU,
    VK_BACK,
    VK_RETURN,
    VK_RSHIFT,
    VK_RMENU,
    VK_RCONTROL,

    VK_UP,
    VK_DOWN,
    VK_LEFT,
    VK_RIGHT,

    VK_DECIMAL,
    VK_NUMPAD0,
    VK_NUMPAD1,
    VK_NUMPAD2,
    VK_NUMPAD3,
    VK_NUMPAD4,
    VK_NUMPAD5,
    VK_NUMPAD6,
    VK_NUMPAD7,
    VK_NUMPAD8,
    VK_NUMPAD9,

    // Keyboard_end
};


CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::Init()
{
	// 모든 키 상태를 NONE, 이전에 눌린 적 없음으로 초기화
	for (int i = 0; i < (int)Keyboard::Keyboard_end; ++i)
	{
		m_keyboardInfo.push_back(key_info{ Key_state::NONE, false });
	}
}

void CKeyMgr::Tick()
{
	// 키보드 상태 업데이트
	for (int i = 0; i < (int)Keyboard::Keyboard_end; ++i)
	{
        // 키보드가 이전에 눌렸었음
        if (m_keyboardInfo[i].isPrevPressed)
        {
            // 현재도 눌린 상태임
            if (GetAsyncKeyState(key_value[i]) & 0x8001)
            {
                m_keyboardInfo[i].state = Key_state::PRESSED;
            }
            // 현재는 떼어짐
            else
            {
                m_keyboardInfo[i].state = Key_state::RELEASE;
                m_keyboardInfo[i].isPrevPressed = false;
            }
        }
        // 이전에 눌린 적 없음
        else
        {
            // 현재는 눌린 상태임
            if (GetAsyncKeyState(key_value[i]) & 0x8001)
            {
                m_keyboardInfo[i].state = Key_state::TAP;
                m_keyboardInfo[i].isPrevPressed = true;
            }
            // 계속 안눌려있었음
            else
            {
                m_keyboardInfo[i].state = Key_state::NONE;
            }
        }
	}
}
