#include "main.h"
#include "input.h"


BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];


void Input::Init()
{

	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

}

void Input::Uninit()
{


}

void Input::Update()
{

	memcpy( m_OldKeyState, m_KeyState, 256 );

	GetKeyboardState( m_KeyState );

}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool Input::IsMouseButtonPressed(int button)
{
    // 仮実装: Windows APIのGetAsyncKeyStateを使う例
    // 0: 左ボタン, 1: 右ボタン
    if (button == 0) return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    if (button == 1) return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
    return false;
}
