#include <bangtal.h>
#include <iostream>
#include <ctime>

using namespace bangtal;
using namespace std;

int main()
{
	srand((unsigned int)time(NULL));
	time_t start_time;
	auto scene0 = Scene::create("Fort", "Images/Fort.png");
	auto scene = Scene::create("Background", "Images/Background.png");
	auto startButton = Object::create("Images/start.png", scene0, 590, 30);
	startButton->setScale(0.1f);

	startButton->setOnMouseCallback([&](ObjectPtr object, int, int, MouseAction)->bool {
		startButton->hide();
		scene->enter();
		return true;
		});

	//보드 생성
	ObjectPtr board[9];
	ObjectPtr init_board[9];
	int blank = 8;
	for (int i = 0; i < 9; i++)
	{
		string filename = "Images/" + to_string(i + 1) + ".png";
		board[i] = Object::create(filename, scene, 340 + (i % 3) * 200, 422 - (i / 3) * 200);
		init_board[i] = board[i];

		//클릭했을 때 퍼즐 조각 위치 변경
		board[i]->setOnMouseCallback([&](ObjectPtr object, int, int, MouseAction)->bool{
			int j=0;
			for (j = 0; j < 9; j++)
			{
				if (board[j] == object)
					break;
			}

			if ((j%3>0 && j-1==blank) || (j%3<2 && j+1==blank) || (j>2 && j-3 == blank) || (j<7 && j+3 == blank))
			{
				//이미지 위치 서로 변경
				board[j]->locate(scene, 340 + (blank % 3) * 200, 422 - (blank / 3) * 200);
				board[blank]->locate(scene, 340 + (j % 3) * 200, 422 - (j / 3) * 200);

				//array내에서 숫자 값 서로 변경
				board[j] = board[blank];
				board[blank] = object;
				blank = j;

				//완료됬는지 array 서로 비교
				int k = 0;
				for (k = 0; k < 9; k++)
				{
					if (board[k] != init_board[k])
						break;
				}
				if (k == 9)
				{
					auto end_time = time(NULL);
					cout << difftime(end_time, start_time) << endl;

					string record = to_string(difftime(end_time, start_time)) + "초 걸렸습니다!";
					showMessage(record.c_str());
				}
				
			}
			

			return true;
		});
	}

	//마지막칸 하이드 처리
	board[blank]->hide();

	//애니메이션
	int count = 0;
	auto timer = Timer::create(0.1f);
	timer->setOnTimerCallback([&](TimerPtr t)->bool
		{
			int j = 0;
			do {
				switch (rand() % 4)
					{
						case 0:
							j = blank - 1;
							break;
						case 1:
							j = blank + 1;
							break;
						case 2:
							j = blank - 3;
							break;
						case 3:
							j = blank + 3;
							break;
					}
			} while (j < 0 || j > 8 || !((j % 3 > 0 && j - 1 == blank) ||
				(j % 3 < 2 && j + 1 == blank) ||
				(j > 2 && j - 3 == blank) ||
				(j < 7 && j + 3 == blank)));
			{
				board[j]->locate(scene, 340 + (blank % 3) * 200, 422 - (blank / 3) * 200);
				board[blank]->locate(scene, 340 + (j % 3) * 200, 422 - (j / 3) * 200);

				auto object = board[j];
				board[j] = board[blank];
				board[blank] = object;
				blank = j;
			}

			count++;
			if (count < 5)
			{
				t->set(0.1f);
				t->start();
			}
			
			return true;
		});
	timer->start();

	start_time = time(NULL);
	startGame(scene0);

}
