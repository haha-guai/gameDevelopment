#include <iostream>
#include <graphics.h>

char board_data[3][3] = {
	{'-', '-', '-'}, {'-', '-', '-'}, {'-', '-', '-'}
};
char current_pieces = 'O';


bool checkWin(char c) {
	for (int i = 0; i < 3; i++) {
		if (board_data[i][0] == c && board_data[i][1] == c && board_data[i][2] == c) return true;
		if (board_data[0][i] == c && board_data[1][i] == c && board_data[2][i] == c) return true;
	}
	if (board_data[0][0] == c && board_data[1][1] == c && board_data[2][2] == c) return true;
	if (board_data[0][2] == c && board_data[1][1] == c && board_data[2][0] == c) return true;
	return false;
}

bool checkDraw() {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (board_data[i][j] == '-') return false;
	return true;
}

void drawBoard() {
	setlinecolor(WHITE);
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);
	line(600, 0, 600, 600);
	line(0, 200, 600, 200);
	line(0, 400, 600, 400);
	line(0, 600, 600, 600);
}

void drawPieces() {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			switch (board_data[i][j]) {
			case 'O':
				setlinecolor(WHITE);
				circle(j * 200 + 100, i * 200 + 100, 80);
				break;
			case 'X':
				setlinecolor(YELLOW);
				line(j * 200 + 20, i * 200 + 20, j * 200 + 180, i * 200 + 180);
				line(j * 200 + 20, i * 200 + 180, j * 200 + 180, i * 200 + 20); break;
			case '-':
				break;
			}
}

void drawTipText() {
	static TCHAR str[64];
	_stprintf_s(str, _T("当前棋手：%c"), current_pieces);
	outtextxy(0, 610, str);
}


int main() {
	bool running = true;
	initgraph(600, 700);
	SetWindowText(GetHWnd(), _T("井字棋"));
	setbkcolor(RGB(23, 23, 43));
	setlinestyle(PS_SOLID, 5);

	BeginBatchDraw();

	while (running) {
		DWORD start_time = GetTickCount();
		ExMessage msg;
		while (peekmessage(&msg)) {
			if (msg.message == WM_LBUTTONDOWN) {
				int x = msg.x / 200;
				int y = msg.y / 200;
				if (board_data[y][x] == '-') {
					board_data[y][x] = current_pieces;
					if (current_pieces == 'O')
						current_pieces = 'X';
					else
						current_pieces = 'O';
				}
			}

			cleardevice();
			drawBoard();
			drawPieces();
			drawTipText();
			FlushBatchDraw();

			if (checkWin('X')) {
				MessageBox(GetHWnd(), _T("玩家X获胜"), _T("游戏结束"), MB_OK);
				running = false;
				break;
			}
			else if (checkWin('O')) {
				MessageBox(GetHWnd(), _T("玩家O获胜"), _T("游戏结束"), MB_OK);
				running = false;
				break;
			}
			else if (checkDraw()) {
				MessageBox(GetHWnd(), _T("平局了"), _T("游戏结束"), MB_OK);
				running = false;
				break;
			}

			DWORD end_time = GetTickCount();
			DWORD delay_time = end_time - start_time;
			if (delay_time < 1000 / 60)
				Sleep(1000 / 60 - delay_time);

		}

	}
	EndBatchDraw();
	return 0;
}