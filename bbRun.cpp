
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h> 
#include <conio.h> 
#include <string>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std; 

#define CONSOLE GetStdHandle(STD_OUTPUT_HANDLE) // 콘솔 핸들 가져옴
#define COLOR_RED SetConsoleTextAttribute(CONSOLE, 0x000c); //빨간색
#define COLOR_BLUE SetConsoleTextAttribute(CONSOLE, 0x0001 | 0x0008); //파란색
#define COLOR_GREEN SetConsoleTextAttribute(CONSOLE, 0x00a); //연두색
#define COLOR_WHITE SetConsoleTextAttribute(CONSOLE, 0x000f); //하얀핵
#define COLOR_SKY SetConsoleTextAttribute(CONSOLE, 0x000b); //하늘색
#define COLOR_YELLOW SetConsoleTextAttribute(CONSOLE, 0x000e); //노란색
#define COLOR_GOLD SetConsoleTextAttribute(CONSOLE, 0x0006); //금색
#define COLOR_WINE SetConsoleTextAttribute(CONSOLE, 0x0005); //자주색
#define COLOR_BLOOD SetConsoleTextAttribute(CONSOLE, 0x0004); //짙은빨강
#define COLOR_VIOLET SetConsoleTextAttribute(CONSOLE, 0x0001 | 0x0008 | 0x000c); //보라색

enum COMMON
{
	DISPLAY_DELAY = 20,	

	MAX_INVENTORY_SLOT_COUNT = 10,	

	MAX_PLAYER_COUNT = 100,	
	MAX_OBSTACLE_COUNT = 50,	

	MAX_SHOPITEM_COUNT = 10,	

	MAX_HOLE_COUNT = 50,	
	MAX_STAR_COUNT = 50,	
};


enum SCREEN_TYPE
{
	TYPE_SCREEN_MENU = 0,		 
	TYPE_SCREEN_RANK,			
	TYPE_SCREEN_PLAY,			
	TYPE_SCREEN_LOGIN,			
	TYPE_SCREEN_SHOP,			
	TYPE_SCREEN_INVENTORY,		
	TYPE_HOW_TO_PLAY,			
	TYPE_SCREEN_EXIT,			
};

typedef struct ITEM_INFO
{
	BOOL bUse;					
	char itemFace[MAX_PATH];	
	char itemName[MAX_PATH];	
	int money;					
} ITEM_INFO;

typedef struct PLAYER_INFO
{
	BOOL	bUse;			

	int		x;								
	int		y;
	int		w;				
	int		h;				 

	int		bestScore;		
	int		gameScore;		

	int		money;			

	char	id[MAX_PATH];	
	char	pwd[MAX_PATH]; 

	int		equipIndex;		
	struct ITEM_INFO haveItem[MAX_INVENTORY_SLOT_COUNT]; 
} PLAYER_INFO;




typedef struct HOLE_INFO
{
	BOOL	bUse;			

	int		x;							
	int		y;
	int		w;				
	int		h;				
} HOLE_INFO;

typedef struct STAR_INFO
{
	BOOL	bUse;			

	int		x;				 				
	int		y;
	int		w;				
	int		h;				
} STAR_INFO;

typedef struct OBSTACLE_INFO
{
	BOOL	bUse;		
	int		x;
	int		y;
	int		w;
	int		h;
} OBSTACLE_INFO;


int compareVAL(const void *arg1, const void * arg2) {
	float v1, v2;
	v1 = ((PLAYER_INFO *)arg1)->bestScore;
	v2 = ((PLAYER_INFO *)arg2)->bestScore;

	if (v1 > v2) return -1;
	else if (v1 == v2) return 0;
	else return 1;
}

class GameApp
{
public:
	OBSTACLE_INFO	ObstacleList[MAX_OBSTACLE_COUNT]; 
	PLAYER_INFO playerList[MAX_PLAYER_COUNT]; 
	ITEM_INFO	shopItemList[MAX_SHOPITEM_COUNT]; 
	HOLE_INFO	holeList[MAX_HOLE_COUNT]; 

	STAR_INFO	StarList[MAX_STAR_COUNT]; 
	PLAYER_INFO* MyPlayer;

	BOOL	bGameOver;	
	BOOL	bJump;	
	BOOL	bNeedBGSound;	
	BOOL	bJumpDirection;	

	int playSoundFrame; 
	int currStage; 
	int accelValue; 
	int incrementFrame; 
	int moveSpeed;

public:
	GameApp()
	{
		bGameOver = FALSE;	
		bJump = FALSE;	
		bNeedBGSound = FALSE;	
		bJumpDirection = FALSE;	

		playSoundFrame = 0; 
		currStage = 1; 
		accelValue = 0;
		incrementFrame = 0; 
		moveSpeed = 2; 
	}

public:
	
	void Init()
	{
		
		shopItemList[0].bUse = TRUE;
		strcpy(shopItemList[0].itemFace, "♥");
		strcpy(shopItemList[0].itemName, "타요");
		shopItemList[0].money = 10000;

		shopItemList[1].bUse = TRUE;
		strcpy(shopItemList[1].itemFace, "★");
		strcpy(shopItemList[1].itemName, "차요");
		shopItemList[1].money = 9000;

		shopItemList[2].bUse = TRUE;
		strcpy(shopItemList[2].itemFace, "＆");
		strcpy(shopItemList[2].itemName, "로기");
		shopItemList[2].money = 8000;

		shopItemList[3].bUse = TRUE;
		strcpy(shopItemList[3].itemFace, "§");
		strcpy(shopItemList[3].itemName, "패트");
		shopItemList[3].money = 7000;

		shopItemList[4].bUse = TRUE;
		strcpy(shopItemList[4].itemFace, "◑");
		strcpy(shopItemList[4].itemName, "토토");
		shopItemList[4].money = 6000;

		shopItemList[5].bUse = TRUE;
		strcpy(shopItemList[5].itemFace, "♀");
		strcpy(shopItemList[5].itemName, "루키");
		shopItemList[5].money = 5000;

		shopItemList[6].bUse = TRUE;
		strcpy(shopItemList[6].itemFace, "♡");
		strcpy(shopItemList[6].itemName, "라니");
		shopItemList[6].money = 4000;

		shopItemList[7].bUse = TRUE;
		strcpy(shopItemList[7].itemFace, "∂");
		strcpy(shopItemList[7].itemName, "맥스");
		shopItemList[7].money = 3000;

		shopItemList[8].bUse = TRUE;
		strcpy(shopItemList[8].itemFace, "▲");
		strcpy(shopItemList[8].itemName, "로기");
		shopItemList[8].money = 2000;

		shopItemList[9].bUse = TRUE;
		strcpy(shopItemList[9].itemFace, "○");
		strcpy(shopItemList[9].itemName, "벨리");
		shopItemList[9].money = 1000;



		
		ReadGameDataFile();

		
		PlaySound(TEXT("bgsound.wav"), NULL, SND_ASYNC | SND_LOOP);
	}

	
	BOOL ReadGameDataFile()
	{
		
		FILE* fp = NULL;

		
		fp = fopen("gamedata.txt", "r");
		if (fp == NULL)
		{
			return FALSE;
		}

		
		ZeroMemory(&playerList, sizeof(playerList));

		
		fread(&playerList, sizeof(playerList), 1, fp);


		
		return TRUE;
	}

	
	BOOL WriteGameDataFile()
	{
		
		FILE* fp = NULL;

		
		fp = fopen("gamedata.txt", "w");

		if (fp == NULL)
		{
			cout << "게임 데이터를 쓰기 실패하였습니다!";
			Sleep(1000);
			return FALSE;
		}

		
		fwrite(&playerList, sizeof(playerList), 1, fp);

		return TRUE;
	}

	
	void MainMenu(int* type)
	{
		int i = 0;
		int choice = 0;

		
		system("mode con lines=30 cols=105");
		
		
		COLOR_SKY;

		
		delay_print(13, 4, DISPLAY_DELAY, "         □□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
		delay_print(13, 5, DISPLAY_DELAY, "         □□□■■□□■■□□□■■□□■■□□■■■■□□■■□□\n");
		delay_print(13, 6, DISPLAY_DELAY, "         □□□■■□□■■□□□■■□□■■□□■■■■□□■■□□\n");
		delay_print(13, 7, DISPLAY_DELAY, "         □□□■■■■■■□□□■■■■■■□□□□■■□□■■□□\n");
		delay_print(13, 8, DISPLAY_DELAY, "         □□□■■□□■■□□□■■□□■■□□□□■■□□■■□□\n");
		delay_print(13, 9, DISPLAY_DELAY, "         □□□■■■■■■□□□■■■■■■□□■■■■□□■■□□\n");
		delay_print(13, 10, DISPLAY_DELAY, "         □□□□□□□□□□□□□□□□□□□□■■□□■■■■□□\n");
		delay_print(13, 11, DISPLAY_DELAY, "         □□■■■■■■■■□■■■■■■■■□■■□□□□■■□□\n");
		delay_print(13, 12, DISPLAY_DELAY, "         □□■■■■■■■■□■■■■■■■■□■■■■□□■■□□\n");
		delay_print(13, 13, DISPLAY_DELAY, "         □□□□□■■□□□□□□□■■□□□□■■■■□□■■□□\n");
		delay_print(13, 14, DISPLAY_DELAY, "         □□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
		delay_print(13, 15, DISPLAY_DELAY, "         □□□□■■■■□□□□□■■■■□□□□□■■□□□□□□\n");
		delay_print(13, 16, DISPLAY_DELAY, "         □□□■■□□■■□□□■■□□■■□□□□■■□□□□□□\n");
		delay_print(13, 17, DISPLAY_DELAY, "         □□□■■□□■■□□□■■□□■■□□□□■■■■■■□□\n");
		delay_print(13, 18, DISPLAY_DELAY, "         □□□□■■■■□□□□□■■■■□□□□□■■■■■■□□\n");
		delay_print(13, 19, DISPLAY_DELAY, "         □□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");

		
		COLOR_WHITE;
		delay_print(13, 21, DISPLAY_DELAY, "                               다락방 27기 이수아              \n");

		
		COLOR_VIOLET;
		delay_print(13, 25, DISPLAY_DELAY, " 1. Start   2. How to play   3. Ranking   4. Shopping   5. Inventory   6.Exit\n");


		

		scanf("%d", &choice);

		if (choice == 1)
		{
			
			PlaySound(TEXT("gamebgsound.wav"), NULL, SND_ASYNC | SND_LOOP);
			InitPlayGame();
			*type = TYPE_SCREEN_PLAY; 
		}
		else if (choice == 2)
			*type = TYPE_HOW_TO_PLAY; 
		else if (choice == 3)
			*type = TYPE_SCREEN_RANK; 
		else if (choice == 4)
			*type = TYPE_SCREEN_SHOP;
		else if (choice == 5)
			*type = TYPE_SCREEN_INVENTORY; 
		else if (choice == 6) 
			*type = TYPE_SCREEN_EXIT;

	}

	void Run()
	{
		int screenType = TYPE_SCREEN_LOGIN; 


		while (1)
		{

			switch (screenType)
			{
			case TYPE_SCREEN_PLAY: 
			{
				
				system("cls");
				gamePlay(&screenType);
			}
			break;
			case TYPE_SCREEN_RANK: 
			{
				Ranking(&screenType);
			}
			break;
			case TYPE_SCREEN_LOGIN:
			{
				Login(&screenType);
			}
			break;
			case TYPE_SCREEN_SHOP: 
			{
				Shopping(&screenType);
			}
			break;
			case TYPE_SCREEN_MENU: 
			{
				MainMenu(&screenType);
			}
			break;
			case TYPE_HOW_TO_PLAY:  
			{
				Manual(&screenType);
			}
			break;
			case TYPE_SCREEN_INVENTORY:
			{
				Inventory(&screenType);
			}
			break;
			case TYPE_SCREEN_EXIT: 
			{
				
				WriteGameDataFile();

				exit(0); 
			}
			break;
			}
			Sleep(50);
		}
	}

	
	void Manual(int* type)
	{

		system("cls");
		system("mode con lines=30 cols=105");

		COLOR_YELLOW;
		delay_print(5, 2, 100, "                           ★ How To Play ★                               ");



		COLOR_SKY;
		
		delay_print(5, 6, DISPLAY_DELAY, " 1. 이 게임은 타이밍에 맞춰 점프를 하여 장애물과 구멍을 피하는 게임입니다.");
		delay_print(5, 8, DISPLAY_DELAY, " 2. 스페이스를 누르면 점프가 가능합니다.");
		delay_print(5, 10, DISPLAY_DELAY, " 3. 스테이지 순서대로 클리어 하는 게임입니다.");
		delay_print(5, 12, DISPLAY_DELAY, " 4. 난이도는 속도와 장애물과 구멍의 갯수로 조절됩니다.");
		delay_print(5, 14, DISPLAY_DELAY, " 5. Stage를 클리어시 다음 stage로 넘어가게 됩니다.\n         총3개의 stage가 존재합니다.");
		delay_print(5, 17, DISPLAY_DELAY, " 6. 장애물에 맞거나 구멍에 빠질 시 game over 됩니다.\n\n\n\n\n\n\n\n");

		COLOR_WHITE;
		delay_print(5, 20, DISPLAY_DELAY, "-- 아무 키나 누르시면 메뉴로 돌아갑니다 -- ");
		

		_getch();

		*type = TYPE_SCREEN_MENU;
	}

	void Inventory(int* type)
	{
		int choice = 0;

		system("cls");
		system("mode con lines=30 cols=105"); 

		
		COLOR_YELLOW;
		delay_print(5, 2, 100, "                           ★ Inventory ★                                     ");

		COLOR_GREEN;
		
		printf("\n  ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     \n");
		printf("  │ %s   │     │  %s  │     │  %s  │     │  %s  │     │  %s  │     \n", MyPlayer->haveItem[0].bUse ? MyPlayer->haveItem[0].itemFace : "  ", MyPlayer->haveItem[1].bUse ? MyPlayer->haveItem[1].itemFace : "  ", MyPlayer->haveItem[2].bUse ? MyPlayer->haveItem[2].itemFace : "  ", MyPlayer->haveItem[3].bUse ? MyPlayer->haveItem[3].itemFace : "  ", MyPlayer->haveItem[4].bUse ? MyPlayer->haveItem[4].itemFace : "  ");
		printf("  │      │     │      │     │      │     │      │     │      │     \n");
		printf("  └───┘     └───┘     └───┘     └───┘     └───┘     \n");
		printf("   [0.%s]       [1.%s]       [2.%s]       [3.%s]       [4.%s]      \n", MyPlayer->haveItem[0].bUse ? MyPlayer->haveItem[0].itemName : "none", MyPlayer->haveItem[1].bUse ? MyPlayer->haveItem[1].itemName : "none", MyPlayer->haveItem[2].bUse ? MyPlayer->haveItem[2].itemName : "none", MyPlayer->haveItem[3].bUse ? MyPlayer->haveItem[3].itemName : "none", MyPlayer->haveItem[4].bUse ? MyPlayer->haveItem[4].itemName : "none");
		printf("  ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐     \n");
		printf("  │  %s  │     │  %s  │     │  %s  │     │  %s  │     │  %s  │     \n", MyPlayer->haveItem[5].bUse ? MyPlayer->haveItem[5].itemFace : "  ", MyPlayer->haveItem[6].bUse ? MyPlayer->haveItem[6].itemFace : "  ", MyPlayer->haveItem[7].bUse ? MyPlayer->haveItem[7].itemFace : "  ", MyPlayer->haveItem[8].bUse ? MyPlayer->haveItem[8].itemFace : "  ", MyPlayer->haveItem[9].bUse ? MyPlayer->haveItem[9].itemFace : "  ");
		printf("  │      │     │      │     │      │     │      │     │      │     \n");
		printf("  └───┘     └───┘     └───┘     └───┘     └───┘     \n");
		printf("   [5.%s]       [6.%s]       [7.%s]       [8.%s]       [9.%s]      \n", MyPlayer->haveItem[5].bUse ? MyPlayer->haveItem[5].itemName : "none", MyPlayer->haveItem[6].bUse ? MyPlayer->haveItem[6].itemName : "none", MyPlayer->haveItem[7].bUse ? MyPlayer->haveItem[7].itemName : "none", MyPlayer->haveItem[8].bUse ? MyPlayer->haveItem[8].itemName : "none", MyPlayer->haveItem[9].bUse ? MyPlayer->haveItem[9].itemName : "none");

		COLOR_GOLD;
		printf(" \n   10. Exit                                              \n");

		COLOR_WHITE;
		if (MyPlayer->equipIndex == 0)
		{
			delay_print(5, 20, DISPLAY_DELAY, "현재 장착된 아이템 없음! 아이템 번호를 입력하여 장착해주세요!");
		}
		else
		{
			delay_print(5, 20, DISPLAY_DELAY, "현재 장착된 아이템: %s %s, 교체할 아이템 번호를 입력하면 장착됩니다.", MyPlayer->haveItem[MyPlayer->equipIndex - 1].itemFace, MyPlayer->haveItem[MyPlayer->equipIndex - 1].itemName);
		}

		scanf("%d", &choice);

		if (choice == 10) 
		{
			*type = TYPE_SCREEN_MENU;
			return;
		}

		if (choice >= 0 && choice < 10)
		{
			MyPlayer->equipIndex = choice + 1;
		}
		else
		{
			delay_print(5, 22, DISPLAY_DELAY, "번호를 잘못 입력하셨습니다!");
		}
	}

	void Ranking(int* type)
	{
		int i = 0;

		system("cls");

		qsort(playerList, MAX_PLAYER_COUNT, sizeof(PLAYER_INFO), compareVAL);
 
		COLOR_WINE;
		delay_print(13, 2, DISPLAY_DELAY, " ------------------Ranking--------------------");
		for (i = 0; i < 10; ++i)
		{
			if (playerList[i].bUse)
			{
				COLOR_BLUE;
				delay_print(13, 3 + i, DISPLAY_DELAY, "   %d.%d점 / %s님  ", i + 1, playerList[i].bestScore, playerList[i].id);
			}
		}
		COLOR_WINE;
		delay_print(13, 13, DISPLAY_DELAY, " ---------------------------------------------");

		COLOR_WHITE;
		delay_print(13, 15, DISPLAY_DELAY, "-- 아무키나 누르시면 메뉴화면으로 이동합니다. -- ");

		_getch();

		*type = TYPE_SCREEN_MENU;
	}

	void Login(int* type)
	{
		int i = 0;
		int choice = 0;
		int bAddMember = 0;
		string loginId;  
		string loginPwd;  

		system("mode con lines=30 cols=105");
		
		COLOR_SKY;
		delay_print(13, 4, DISPLAY_DELAY, "         □□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
		delay_print(13, 5, DISPLAY_DELAY, "         □□□■■□□■■□□□■■□□■■□□■■■■□□■■□□\n");
		delay_print(13, 6, DISPLAY_DELAY, "         □□□■■□□■■□□□■■□□■■□□■■■■□□■■□□\n");
		delay_print(13, 7, DISPLAY_DELAY, "         □□□■■■■■■□□□■■■■■■□□□□■■□□■■□□\n");
		delay_print(13, 8, DISPLAY_DELAY, "         □□□■■□□■■□□□■■□□■■□□□□■■□□■■□□\n");
		delay_print(13, 9, DISPLAY_DELAY, "         □□□■■■■■■□□□■■■■■■□□■■■■□□■■□□\n");
		delay_print(13, 10, DISPLAY_DELAY, "         □□□□□□□□□□□□□□□□□□□□■■□□■■■■□□\n");
		delay_print(13, 11, DISPLAY_DELAY, "         □□■■■■■■■■□■■■■■■■■□■■□□□□■■□□\n");
		delay_print(13, 12, DISPLAY_DELAY, "         □□■■■■■■■■□■■■■■■■■□■■■■□□■■□□\n");
		delay_print(13, 13, DISPLAY_DELAY, "         □□□□□■■□□□□□□□■■□□□□■■■■□□■■□□\n");
		delay_print(13, 14, DISPLAY_DELAY, "         □□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
		delay_print(13, 15, DISPLAY_DELAY, "         □□□□■■■■□□□□□■■■■□□□□□■■□□□□□□\n");
		delay_print(13, 16, DISPLAY_DELAY, "         □□□■■□□■■□□□■■□□■■□□□□■■□□□□□□\n");
		delay_print(13, 17, DISPLAY_DELAY, "         □□□■■□□■■□□□■■□□■■□□□□■■■■■■□□\n");
		delay_print(13, 18, DISPLAY_DELAY, "         □□□□■■■■□□□□□■■■■□□□□□■■■■■■□□\n");
		delay_print(13, 19, DISPLAY_DELAY, "         □□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");

		COLOR_YELLOW;
		fflush(stdin); 
		delay_print(13, 21, DISPLAY_DELAY, "                       아이디를 입력하세요!              \n");
		gotoxy(58, 21);
		cin >> loginId; 

		delay_print(13, 23, DISPLAY_DELAY, "                       비밀번호를 입력하세요!              \n");
		gotoxy(60, 23);
		cin >> loginPwd;
		
		if (CheckAuth(loginId.c_str(), loginPwd.c_str()))
		{
			delay_print(13, 24, DISPLAY_DELAY, "                       로그인하였습니다.              \n");
			Sleep(1000);

			*type = TYPE_SCREEN_MENU; 
		}
		else
		{
			delay_print(13, 24, DISPLAY_DELAY, "                       계정정보가 틀렸습니다. 새로등록하시겠습니까? (YES:1, NO:2)            \n");
			scanf("%d", &bAddMember);

			if (bAddMember == 1) 
			{
				fflush(stdin);
				delay_print(13, 25, DISPLAY_DELAY, "                       새로 등록할 아이디를 입력하세요!              \n");
				gotoxy(70, 25);
				cin >> loginId;

				fflush(stdin);
				delay_print(13, 25, DISPLAY_DELAY, "                       새로 등록할 비밀번호를 입력하세요!              \n");
				gotoxy(70, 25);
				cin >> loginPwd;
				
				AddPlayer(loginId.c_str(), loginPwd.c_str());

				delay_print(13, 26, DISPLAY_DELAY, "                       등록되었습니다!              \n");
				Sleep(1000);
			}
			else 
			{
				
			}
		}



	}

	
	void gamePlay(int* type)
	{
		int i = 0;

		if (MyPlayer->equipIndex == 0)
		{
			cout << "\n 장착아이템이 장착되지 않았습니다. 인벤토리에서 아이템을 창착하세요!!\n";
			Sleep(1000);
			*type = TYPE_SCREEN_MENU; 
			return;
		}

		if (_kbhit())
		{
			switch (_getch()) 
			{
			case ' ': 
			{
				
				if (bGameOver == FALSE && bJump == FALSE)
				{
					bJump = TRUE;

					
					PlaySound(TEXT("jump.wav"), NULL, SND_ASYNC);

					bNeedBGSound = TRUE; 
					playSoundFrame = 10;

				}
			}
			break;
			case '\r': 
			{
				if (bGameOver)
				{
					
					if (MyPlayer->gameScore > MyPlayer->bestScore)
						MyPlayer->bestScore = MyPlayer->gameScore;

					
					InitPlayGame();

					
					*type = TYPE_SCREEN_MENU;

					
					PlaySound(TEXT("bgsound.wav"), NULL, SND_ASYNC | SND_LOOP);

				}
			}
			break;
			}
		}

		if (bGameOver == FALSE)
		{
			
			{
				if (bNeedBGSound)
				{
					if (playSoundFrame <= 0)
					{
						
						PlaySound(TEXT("gamebgsound.wav"), NULL, SND_ASYNC | SND_LOOP);

						bNeedBGSound = FALSE;
					}

					if (playSoundFrame)
					{
						playSoundFrame -= 1;
					}
				}

				if (currStage == 1)
				{
					if (incrementFrame > 0 && incrementFrame % 500 == 0)
					{
						 
						AddHole();
					}

					if (incrementFrame > 1500)
					{
						currStage = 2;
						incrementFrame = 0;

						PlaySound(TEXT("changeStage.wav"), NULL, SND_ASYNC);

						bNeedBGSound = TRUE;
						playSoundFrame = 10;
					}

				}
				else if (currStage == 2)
				{

					if (incrementFrame > 0 && incrementFrame % 200 == 0)
					{
						AddHole();
					}

					moveSpeed = 4;

					if (incrementFrame > 3000)
					{
						currStage = 3;
						incrementFrame = 0;

						PlaySound(TEXT("changeStage.wav"), NULL, SND_ASYNC);

						bNeedBGSound = TRUE; 
						playSoundFrame = 10;
					}
				}
				else if (currStage == 3)
				{
					if (incrementFrame > 0 && incrementFrame % 100 == 0)
					{
						AddHole();
					}

					moveSpeed = 6;

					if (incrementFrame > 1000 && incrementFrame % 50 == 0)
					{
						AddObStacle();
					}

				}
			}

			{
				
				for (i = 0; i < MAX_HOLE_COUNT; ++i)
				{
					if (holeList[i].bUse == TRUE)
					{
						holeList[i].x -= moveSpeed; 

													  
						if (holeList[i].x < 0)
						{
							holeList[i].bUse = FALSE; 
							MyPlayer->gameScore += 1;
						}

						if ((holeList[i].x <= MyPlayer->x && (holeList[i].x + holeList[i].w) >= MyPlayer->x) && holeList[i].y == MyPlayer->y + 1)
						{
							MyPlayer->y = 18;

							PlaySound(TEXT("dead.wav"), NULL, SND_ASYNC);

							bGameOver = TRUE;
							return;
						}
					}
				}

				if (incrementFrame > 0 && incrementFrame % 100 == 0)
				{
					AddStar();
				}


				for (i = 0; i < MAX_STAR_COUNT; ++i)
				{
					if (StarList[i].bUse)
					{
						StarList[i].x -= moveSpeed; 

													  
						if (StarList[i].x < 0)
						{
							StarList[i].bUse = FALSE; 
						}
					}
				}

				for (i = 0; i < MAX_OBSTACLE_COUNT; ++i)
				{
					if (ObstacleList[i].bUse)
					{
						ObstacleList[i].x -= moveSpeed; 

														  
						if (ObstacleList[i].x < 0)
						{
							ObstacleList[i].bUse = FALSE; 
						}

						
						if (MyPlayer->x == ObstacleList[i].x && MyPlayer->y == ObstacleList[i].y)
						{
							
							PlaySound(TEXT("dead.wav"), NULL, SND_ASYNC);

							
							bGameOver = TRUE;
							return;
						}
					}
				}

				incrementFrame += 10;
			}

			if (bJump)
			{
				if (accelValue == 0)
				{
					bJumpDirection = TRUE;
					accelValue = 1;
				}

				if (bJumpDirection)
				{
					MyPlayer->y += accelValue; 
				}
				else
				{
					MyPlayer->y -= accelValue; 
					accelValue /= 2;
				}

				if (MyPlayer->y >= 17)
				{
					MyPlayer->y = 17;
					accelValue = 7; 
					bJump = FALSE;  
					bJumpDirection = FALSE;
				}
			}

		}

		COLOR_VIOLET;
		gotoxy(0, 18);
		printf("########################################################################################################\n");
		printf("########################################################################################################\n");
		printf("########################################################################################################\n");
		printf("########################################################################################################\n");
		printf("########################################################################################################\n");
		printf("########################################################################################################\n");
		printf("########################################################################################################\n");
		printf("########################################################################################################\n");
		printf("########################################################################################################\n");
		printf("########################################################################################################\n");
		printf("########################################################################################################\n");



		for (i = 0; i < MAX_STAR_COUNT; ++i)
		{
			if (StarList[i].bUse)
			{
				gotoxy(StarList[i].x, StarList[i].y);
				COLOR_SKY;
				cout << ("★");
			}
		}

		COLOR_GOLD;
		gotoxy(80, 2);
		printf("stage:%d, score:%d", currStage, MyPlayer->gameScore);


		for (i = 0; i < MAX_HOLE_COUNT; ++i)
		{
			if (holeList[i].bUse)
			{
				gotoxy(holeList[i].x, holeList[i].y);
				cout << ("     ");
			}
		}


		COLOR_GREEN;
		gotoxy(MyPlayer->x, MyPlayer->y);
		printf("%s", MyPlayer->haveItem[MyPlayer->equipIndex - 1].itemFace);

		for (i = 0; i < MAX_OBSTACLE_COUNT; ++i)
		{
			if (ObstacleList[i].bUse)
			{
				gotoxy(ObstacleList[i].x, ObstacleList[i].y);
				COLOR_WINE;
				cout << ("▲");
			}
		}

		if (bGameOver)
		{
			COLOR_GREEN;
			gotoxy(20, 10);
			cout << ("◆◆◆ G a m e   O v e r ◆◆◆");

			gotoxy(20, 13);
			cout << ("--- Please Enter Key ---");
		}

	}

		void Shopping(int* type)
		{
			int choice = 0;
			system("cls");

			COLOR_YELLOW;
			delay_print(13, 2, DISPLAY_DELAY, " ┌---------------------Shop---------------------┐");
			delay_print(13, 3, DISPLAY_DELAY, " │                                              │");
			delay_print(13, 4, DISPLAY_DELAY, " │ 차이름            차모양           차가격    │");
			delay_print(13, 5, DISPLAY_DELAY, " │********************************************* │");
			COLOR_RED;
			delay_print(13, 6, DISPLAY_DELAY, " │1.  %s              %s               %d원│", shopItemList[0].itemName, shopItemList[0].itemFace, shopItemList[0].money);
			delay_print(13, 7, DISPLAY_DELAY, " │2.  %s              %s               %d원 │", shopItemList[1].itemName, shopItemList[1].itemFace, shopItemList[1].money);
			delay_print(13, 8, DISPLAY_DELAY, " │3.  %s              %s               %d원 │", shopItemList[2].itemName, shopItemList[2].itemFace, shopItemList[2].money);
			delay_print(13, 9, DISPLAY_DELAY, " │4.  %s              %s               %d원 │", shopItemList[3].itemName, shopItemList[3].itemFace, shopItemList[3].money);
			delay_print(13, 10, DISPLAY_DELAY, " │5.  %s              %s               %d원 │", shopItemList[4].itemName, shopItemList[4].itemFace, shopItemList[4].money);
			delay_print(13, 11, DISPLAY_DELAY, " │6.  %s              %s               %d원 │", shopItemList[5].itemName, shopItemList[5].itemFace, shopItemList[5].money);
			delay_print(13, 12, DISPLAY_DELAY, " │7.  %s              %s               %d원 │", shopItemList[6].itemName, shopItemList[6].itemFace, shopItemList[6].money);
			delay_print(13, 13, DISPLAY_DELAY, " │8.  %s              %s               %d원 │", shopItemList[7].itemName, shopItemList[7].itemFace, shopItemList[7].money);
			delay_print(13, 14, DISPLAY_DELAY, " │9.  %s              %s               %d원 │", shopItemList[8].itemName, shopItemList[8].itemFace, shopItemList[8].money);
			delay_print(13, 15, DISPLAY_DELAY, " │10.  %s              %s               %d원│", shopItemList[9].itemName, shopItemList[9].itemFace, shopItemList[9].money);
			COLOR_YELLOW;
			delay_print(13, 16, DISPLAY_DELAY, " │                                              │");
			COLOR_GOLD;
			delay_print(13, 17, DISPLAY_DELAY, " │  11. Exit                                    │");
			COLOR_YELLOW;
			delay_print(13, 18, DISPLAY_DELAY, " └----------------------------------------------┘");

			COLOR_WHITE;
			delay_print(13, 20, DISPLAY_DELAY, ">> 현재 보유 금액 : %d", MyPlayer->money);
			delay_print(13, 22, DISPLAY_DELAY, "-- 구매하실 상품을 선택하세요 -- ");


			scanf("%d", &choice);

			if (choice < 1 || choice > 11)
				return;

			if (choice == 11) 
			{
				*type = TYPE_SCREEN_MENU;
				return;
			}


			if (MyPlayer->money < shopItemList[choice - 1].money)
			{
				delay_print(13, 23, DISPLAY_DELAY, "금액이 부족합니다.");
				Sleep(1000);
			}

			if (GetHaveItemCount() == 10)
			{
				delay_print(13, 23, DISPLAY_DELAY, "인벤토리가 가득 찼습니다.");
				Sleep(1000);
			}

			delay_print(13, 23, DISPLAY_DELAY, "구입하였습니다");
			Sleep(1000);

			MyPlayer->money -= shopItemList[choice - 1].money;

			BuyItem(&shopItemList[choice - 1]);

			*type = TYPE_SCREEN_MENU; 
			return;
		}


		void delay_print(int x, int y, int delay, char *str, ...)
		{
			char temp_str[512];
			va_list v_list;

			va_start(v_list, str);
			vsprintf(temp_str, str, v_list);
			va_end(v_list);

			gotoxy(x, y); 
			Sleep(delay); 
			printf("%s", temp_str);
		}

		void print(int x, int y, char str[100])
		{
			gotoxy(x, y); 
			printf("%s", str);
		}


		void gotoxy(int x, int y) 
		{
			COORD XY = { x, y };
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), XY);
		}




		
		void AddPlayer(const char* loginId, const char* loginPwd)
		{
			int i;
			for (i = 0; i < MAX_PLAYER_COUNT; i++)
			{
				
				if (playerList[i].bUse == FALSE)
				{
					 
					strcpy(playerList[i].id, loginId);
					strcpy(playerList[i].pwd, loginPwd);

					

					playerList[i].money = 99999; 

														  



					playerList[i].bUse = TRUE;
					return;
				}
			}


		}


		
		BOOL CheckAuth(const char* loginId, const char* loginPwd)
		{
			int i;
			if (loginId == NULL || loginPwd == NULL) return FALSE;

			for (i = 0; i < MAX_PLAYER_COUNT; ++i)
			{
				
				if (playerList[i].bUse)
				{
					
					if (!strcmp(playerList[i].id, loginId))
					{
						
						if (!strcmp(playerList[i].pwd, loginPwd))
						{
							MyPlayer = &playerList[i];

							return TRUE; 
						}
					}
				}
			}


			return FALSE;  
		}


		

		
		void BuyItem(ITEM_INFO* shopItem)
		{
			int i = 0;

			if (shopItem == NULL) return;

			for (i = 0; i < MAX_INVENTORY_SLOT_COUNT; ++i)
			{
				
				if (MyPlayer->haveItem[i].bUse == FALSE)
				{
					strcpy(MyPlayer->haveItem[i].itemFace, shopItem->itemFace);
					strcpy(MyPlayer->haveItem[i].itemName, shopItem->itemName);
					MyPlayer->haveItem[i].money = shopItem->money;

					MyPlayer->haveItem[i].bUse = TRUE;
					return;
				}
			}

		}

		
		int GetHaveItemCount()
		{
			int i = 0;
			int count = 0;
			for (i = 0; i < MAX_INVENTORY_SLOT_COUNT; ++i)
			{
				if (MyPlayer->haveItem[i].bUse)
					count++;
			}

			return count;
		}

		void InitPlayGame()
		{
			int i = 0;

			currStage = 1;
			incrementFrame = 0;
			moveSpeed = 2;
			accelValue = 7;

			bJump = FALSE;
			bJumpDirection = FALSE;
			bGameOver = FALSE;

			MyPlayer->x = 10;
			MyPlayer->y = 17;
			MyPlayer->w = 1;
			MyPlayer->h = 1;

			MyPlayer->gameScore = 0;


			for (i = 0; i < MAX_HOLE_COUNT; ++i)
			{
				if (StarList[i].bUse)StarList[i].bUse = FALSE;
				if (holeList[i].bUse)holeList[i].bUse = FALSE;
				if (ObstacleList[i].bUse)ObstacleList[i].bUse = FALSE;
			}


		}
 
		void AddHole()
		{
			int i = 0;
			for (i = 0; i < MAX_HOLE_COUNT; ++i)
			{
				if (holeList[i].bUse == FALSE)
				{
					holeList[i].x = 100;
					holeList[i].y = 18;
					holeList[i].w = 5;
					holeList[i].bUse = TRUE;
					break;
				}
			}
		}

		void AddStar()
		{
			int i = 0;
			for (i = 0; i < MAX_STAR_COUNT; ++i)
			{
				if (StarList[i].bUse == FALSE)
				{
					StarList[i].x = 100;
					StarList[i].y = rand() % 15;

					StarList[i].bUse = TRUE;
					break;
				}
			}
		}

		void AddObStacle()
		{
			int i = 0;
			for (i = 0; i < MAX_OBSTACLE_COUNT; ++i)
			{
				if (ObstacleList[i].bUse == FALSE)
				{
					ObstacleList[i].x = 100;
					ObstacleList[i].y = 17;
					holeList[i].w = 1;

					ObstacleList[i].bUse = TRUE;
					break;
				}
			}
		}


};




void main()
{
	GameApp app;
	app.Init(); 
	app.Run();  
}

























