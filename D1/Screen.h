#pragma once

#include <Windows.h>
#include <iostream>

using namespace std;

class Screen
{
public : 
	// 더블 버퍼링에 사용할 스크린 버퍼
	HANDLE ScreenBuffer[2];
	// 현재 쓰기중인 버퍼의 인덱스
	int CurrentScreenBufferIndex;
	// 버퍼의 크기를 가져오기위한 info
	CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
	int horSize;
	int verSize;

public:
	// 화면 클래스 생성자
	Screen() :
		ScreenBuffer{ nullptr, nullptr },
		CurrentScreenBufferIndex(0),
		BufferInfo{},
		horSize(0),
		verSize(0)
	{}

	virtual ~Screen();

	/// <summary>
	/// 초기화 함수, 버퍼를 생성하고, info를 가져옴
	/// </summary>
	/// <returns>성공 여부를 반환한다</returns>
	bool Init();

	/// <summary>
	/// 버퍼에 스트링을 출력하는 함수
	/// </summary>
	/// <param name="str">출력할 스트링</param>
	/// <param name="position">출력할 좌표</param>
	void PrintString(const wstring& str, const COORD& position);
	/// <summary>
	/// 버퍼에 문자를 출력하는 함수
	/// </summary>
	/// <param name="ch">출력할 문자</param>
	/// <param name="position">출력할 위치</param>
	void PrintChar(const wchar_t ch, const COORD& position);
	void PrintHor(const wchar_t ch, const COORD& position, int length);
	void PrintVer(const wchar_t ch, const COORD& position, int length);

	/// <summary>
	/// 버퍼를 교환하는 함수
	/// </summary>
	/// <returns>성공 여부를 반환한다.</returns>
	bool ChangeScreenBuffer();

	/// <summary>
	/// 화면을 비우는 함수.
	/// 버퍼를 교체할 때 호출된다.
	/// </summary>
	void ClearScreen();
};
