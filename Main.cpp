
#include "DxLib.h"

int fonthandle;
const int NUMMAX = 9; //���̍ő�l�ݒ�
int a, b;
int c = 0;
int result = 0;
int timer = 0;
int numquestion = 1;
int numcorrect = 0;
int mode = 1; //�Q�[���J�ڂP�����Ăяo���Q���\�����͑҂��R���͒��S�Z�~���ʂT���ʕ���

const int keylist[21] = { KEY_INPUT_0 , KEY_INPUT_1, KEY_INPUT_2, KEY_INPUT_3, KEY_INPUT_4,
		KEY_INPUT_5, KEY_INPUT_6, KEY_INPUT_7, KEY_INPUT_8, KEY_INPUT_9,
		KEY_INPUT_NUMPAD0,KEY_INPUT_NUMPAD1,KEY_INPUT_NUMPAD2,KEY_INPUT_NUMPAD3,KEY_INPUT_NUMPAD4,
		KEY_INPUT_NUMPAD5,KEY_INPUT_NUMPAD6,KEY_INPUT_NUMPAD7,KEY_INPUT_NUMPAD8,KEY_INPUT_NUMPAD9,
		KEY_INPUT_BACK};//���ׂ�L�[�̃��X�g
int keystate[21] = { 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1 };
int anykeystate = -1;

int CheckKey() //�߂�l -1:���͂Ȃ� -2:�������� 0-9:���� 10:backspace
{
	int ret = -1;

	char buf[256];
	GetHitKeyStateAll(buf);

	for (int i = 0; i < 21; i++)
	{
		if (keystate[i] == 0 && buf[keylist[i]] == 1)//�L�[�_�E���Ŕ��f
		{
			if (ret == -1)
			{
				if (0 <= i && i <= 9)
				{
					ret = i;
				}
				else
				{
					ret = i - 10;
				}
			}
			else
			{
				ret = -2;
			}
		}
		keystate[i] = buf[keylist[i]];
	}

	return ret;
}

int CheckAnswer()//-1:�s���� 0:�ۗ� 1:����
{
	if (c==a+b)
	{
		return 1;
	}
	else
	{
		if (c == (a + b) / 10 && c != 0)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
}

int Update()
{
	if (mode == 1)
	{
		a = GetRand(NUMMAX);
		b = GetRand(NUMMAX);
		mode = 2;
	}
	else if (mode == 2 || mode == 3)
	{
		int key = CheckKey();

		if (0 <= key && key <= 9)
		{
			if (c <= 0)
			{
				c = key;
				mode = 3;
			}
			else
			{
				c = c * 10 + key;
			}
			result =CheckAnswer();
			if (result != 0)
			{
				mode = 4;
			}
		}
		else if (key == 10) //backspace
		{
			if (c < 10)
			{
				c = 0;
				mode = 2;
			}
			else
			{
				c = c / 10;
			}
		}	
	}
	else if (mode == 4)
	{
		timer++;
		if (timer > 60)
		{
			timer = 0;
			c = 0;
			if (result == 1)
			{
				numcorrect++;
			}


			if (numquestion < 10)
			{
				numquestion++;
				mode = 1;
			}
			else
			{
				mode = 5;
			}
		}
	}
	else if (mode == 5)
	{
		int _anykeystate = CheckHitKeyAll(DX_CHECKINPUT_KEY);
		if(anykeystate==0 && _anykeystate !=0)
		{
			return -1;
		}
		anykeystate = _anykeystate;
	}
	return 0;
}


void Draw()
{
	if (mode == 2)
	{
		DrawFormatStringToHandle(50, 180, GetColor(255, 255, 255), fonthandle, "%d + %d = ", a, b);
	}
	else if (mode == 3)
	{
		DrawFormatStringToHandle(50, 180, GetColor(255, 255, 255), fonthandle, "%d + %d = ", a, b);
		DrawFormatStringToHandle(480, 180, GetColor(255, 255, 255), fonthandle, "%d", c);
	}
	else if (mode == 4)
	{
		DrawFormatStringToHandle(50, 180, GetColor(255, 255, 255), fonthandle, "%d + %d = ", a, b);
		DrawFormatStringToHandle(480, 180, GetColor(255, 255, 255), fonthandle, "%d", c);
		if (result > 0)
		{
			DrawCircleAA(320.0f, 240.0f, 150.0f, 64, GetColor(255,0,0), FALSE, 10.0F);
		}
		else
		{
			DrawLineAA(120.0f, 40.0f, 520.0f, 440.0f, GetColor(0,0,255), 10.0F);
			DrawLineAA(520.0f, 40.0f, 120.0f, 440.0f, GetColor(0, 0, 255), 10.0F);
		}

	}
	else if (mode == 5)
	{
		if(numcorrect<10)
		{
			DrawFormatStringToHandle(40, 180, GetColor(255, 255, 255), fonthandle, "���� %d��", numcorrect * 10);
		}
		else
		{
			DrawFormatStringToHandle(15, 20, GetColor(255, 0, 0), fonthandle, "���� %d��", numcorrect * 10);
			DrawStringToHandle(60, 180, "���Ȃ�����", GetColor(255, 255, 255), fonthandle);
			DrawStringToHandle(60, 340, "�V�˂ˁI�I", GetColor(255, 255, 255), fonthandle);
		}

	}

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(640, 480, 32);

	SetMainWindowText("�Z������");
	ChangeWindowMode(true); //�E�C���h�E���[�h

	if (DxLib_Init() == -1)		
	{
		return -1;	
	}

	SetDrawScreen(DX_SCREEN_BACK); //�`���𗠉�ʂɂ���


	if (AddFontResourceEx("Font/yomogifont.otf", FR_PRIVATE, NULL) > 0)//�t�H���g�ǂݍ���
	{
		fonthandle = CreateFontToHandle("������t�H���g", 100, -1, DX_FONTTYPE_ANTIALIASING_8X8);
	}
	else
	{
		MessageBox(NULL, "�t�H���g�Ǎ��G���[", "", MB_OK);

		DxLib_End();
		return 0;
	}


	for(;;) //�Q�[�����[�v
	{
		if (ProcessMessage() != 0)
		{
			break;
		}
		ClearDrawScreen(); //��ʂ�����

		if (Update() == -1)
		{
			break;
		}

		Draw();

		ScreenFlip(); //����ʂƕ\��ʂ����ւ�
	}


	DeleteFontToHandle(fonthandle);

	if (!RemoveFontResourceEx("Font/yomogifont.otf", FR_PRIVATE, NULL))//�t�H���g�폜
	{
		MessageBox(NULL, "�t�H���g�폜�G���[", "", MB_OK);
	}

	DxLib_End();				
	return 0;				 
}





