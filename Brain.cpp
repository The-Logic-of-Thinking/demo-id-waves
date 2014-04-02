#include "stdafx.h"

#include "Brain.h"



//
// Конструктор коры
//
Brain::Core::Core() {
	// Подготовка генератора случайных чисел от 0 до 1
	this->rnd = gcnew System::Random();
	
	this->initCore();
	
	this->SetSens();
	
	this->PatCreate();
	
	// Инициация нейронов коры
	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			this->Cortex[ix, iy].Type = 0;
			// Комментарии (Егармин К.Н.): 0 - входной аксон
			// Изначально все аксоны входные
			this->Cortex[ix, iy].ID = gcnew System::Collections::ObjectModel::Collection<ID>();
		}
	}

	// Создание шаблона распрстранения активности
	// Сумма всех точек дает единицу. Это позволяет устанавливать пороги для полей в процентах задействованных нейронов
	// Комментарии (Егармин К.Н.):
	// Здесь речь о белых размытых пятнах на картинке слева внизу, а точнее:
	// описывается область, на которую влияет нейрон при срабатывании
	float dx = 0;
	float dy = 0;
	for (int ix = 0; ix < this->NLocalAM; ix++) {
		for (int iy = 0; iy < this->NLocalAM; iy++) {
			dx = (float)(ix - this->RLAM);
			dy = (float)(iy - this->RLAM);
			this->LocalAM[ix, iy] = (float)( (1 / (2 * System::Math::PI * this->Sigma2LAM)) * System::Math::Exp( -(System::Math::Pow(dx, 2) + System::Math::Pow(dy, 2)) / (2 * this->Sigma2LAM) ) );
		}
	}
}


void Brain::Core::initCore() {
	//
	// Инициализация полей:
	//
	
	// Кол-во типов нейронов (int)
	N_Types = 2;
	// Таблица чувствительности синаптических и внесинаптических рецепторов к медиаторам, генерируемым аксонами нейронов разного типа
	SensTable = gcnew array<unsigned short, 3>(N_Types, N_Types, 2);
	// Комментарии (Егармин К.Н.):
	// (N_Types, N_Types, 2);
	// 2 - означает, что всего два вида рецепторов: синаптические и внесинаптические
	// Это число не изменится, даже если типов нейронов станет больше

	// Размеры коры (int)
	X_C = 150;
	Y_C = 150;

	// Основная картинка: кора
	pic = gcnew System::Drawing::Bitmap(X_C, Y_C);
	Gpic = System::Drawing::Graphics::FromImage(this->pic);
		
	// Общее количество паттернов (int)
	NP = 5;
	// Комментарии (Егармин К.Н.):
	// Речь идет о паттернах вызванной активности, которые обозначаются группами красных точек

	// Набор паттернов вызванной активности // Егармин К.Н.: т.е. входных аксонов
	PatSet = gcnew array<Pattern>(NP);

	// Радиус паттерна вызванной активности (int)
	R_Pat = 6;

	// Кол-во нейронов в патерне вызванной активности (int)
	NNPat = 15;

	// Кора
	Cortex = gcnew array<Neuron, 2>(X_C, Y_C);

	// Предел количества нейронов в релаксирующем состоянии для каждого из типов. Выше этого предела подавляется спонтанная активность
	LImin = gcnew array<double>(N_Types);
	// Задание пороговых уровней плотности релаксирующих нейронов для подавления спонтанной активности
	LImin[0] = 0.01;
	LImin[1] = 0.01;

	// Расстояние слежения за активностью для ID (влево и вправо по горизонтали и вертикали) (int)
	// Егармин К.Н.: для метаботропных рецепторов
	R_ID = 15;
	// Расстояние слежения за активностью для весов W (влево и вправо по горизонтали и вертикали) (int)
	// Егармин К.Н.: для синаптических рецепторов
	R_W = R_ID;
	// Радиус распространения активности. Связан с расчетом активности на синапсах (трогать аккуратно) (int)
	RLAM = R_W;
	// шаблон распределения активности (int)
	NLocalAM = 2 * RLAM + 1;
	LocalAM = gcnew array<float, 2>(NLocalAM, NLocalAM);
	// Параметр распространения активности. Дисперсия
	Sigma2LAM = (float)System::Math::Pow(5., 2);

	// Текущий такт (int)
	CT = 0;

	// Период полурапада распределенной активности (тактов) (int)
	TAhalf = 4;
	// Падение активности за один такт
	KA = (float)( 1 / System::Math::Pow( 2.0, 1.0 / TAhalf ) );
	
	// Поле накопленной вызванной активности коры
	Field_A_Accum = gcnew array<float, 2>(X_C, Y_C);
	// Порог окружающей вызванной активности для возможности обучения
	L_ActL1 = 0.15f;
	
	// Поле мгновенной активности коры
	Field_A = gcnew array<float, 2>(X_C, Y_C);
	// Порог активности на синапсах для возможности обучения
	// Егармин К.Н.: грубо говоря, нужно чтобы 2% синапсов активировались для возможности обучения
	L_ActLS = (float)( 0.02 * System::Math::Pow( (float)(2 * R_W + 1), 2 ) );

	// Поле вызванной активности коры
	Field_A_Evoced = gcnew array<float, 2>(X_C, Y_C);
	// Поле волновой активности коры
	Field_A_Wave = gcnew array<float, 2>(X_C, Y_C);
	// Поле плотности релаксирующих нейронов каждого из типов
	Field_A_Relax = gcnew array<float, 3>(X_C, Y_C, N_Types);

	// Продолжительность пакета идентификационной активности (int)
	T_ID = 10;
	// Продолжительность тишины (int)
	T_Passive = T_ID * 4;
	// Число тактов накопления активности при синаптическом обучении (int)
	T_SinapseLearn = T_Passive;
	// Набор полей последних состояний нейронов
	CortexA = gcnew array<unsigned short, 3>(X_C, Y_C, T_SinapseLearn);
	// Число тактов накопления активности при внесинаптическом обучении (int)
	T_ExtraSinapseLearn = 4;

	// Минимальная активность для начала обсчета
	LAmin = 0.008;
	// Предел активности для генерации спонтанных спайков
	LAmax = 0.025;

	// Порог активации по идентификатору
	L_act = 0.7;
	
	// количество волн, необходимых для консолидации (int)
	NCons = 5;

	// Егармин К.Н.: коллекция выбранных для моделирования паттернов вызванной активности
	CheckedPatterns = gcnew System::Collections::ObjectModel::Collection<int>();

	// Вероятность истинно спонтанного спайка
	Psp = 0.02;
	
	// Текущий слой (int)
	CurLayer = 0;

	// Суммарная ритмическая активность (int)
	NSer = 300;
	CurPosA = 0;
	AWave = gcnew array<double>(NSer);

	// Координаты угла исходящего туннеля (int)
	TX1 = X_C / 4;
	TY1 = Y_C / 4;
	// Координаты угла входящего туннеля (int)
	TX2 = X_C / 2;
	TY2 = Y_C / 2;
	// Размеры туннеля (int)
	TD = 30;

	// Параметры для формирования картины серии состояний
	FPic = false; // Егармин К.Н.: флаг того, надо или не надо строить набор картинок активности по времени
	// (int)
	NPic = 16; // Егармин К.Н.: количество картинок
	WPic = 4; // Егармин К.Н.: количество картинок по горизонтали
	Bigpic = gcnew System::Drawing::Bitmap(X_C * WPic, Y_C * 4);

	// Текущая последовательность слоев
	LayersSequence = gcnew array<int>(T_SinapseLearn);

	// Флаг включения обучения нейронов-детекторов
	FLearn = true;

	// Веса синапсов нейронов
	// Егармин К.Н.: видимо, каждый нейрон связан со всеми соседями (на расстоянии <= R_W) синапсами, а это их веса
	CortexW = gcnew array<float, 4>(X_C, Y_C, R_W * 2 + 1, R_W * 2 + 1);

	// Порог активации нейрона
	L_Spike = 0.6f;

	// Продолжительность вызванной активности
	T_EnvokedA = (int)(T_Passive * 1.5);

	// Картина состояния обучения
	pic1 = gcnew System::Drawing::Bitmap(X_C, Y_C);
	
	// Коэффициенты пространственной самоорганизации
	// Порог умирания от одиночества
	L_GameLive_Low = 0.15;
	// Порог умирания от перенасыщения
	L_GameLive_high = 0.25;

	// Картина вызванной активности
	picEvokedA = gcnew System::Drawing::Bitmap(X_C, Y_C);
	// Картина накопленной активности
	picActAccum = gcnew System::Drawing::Bitmap(X_C, Y_C);
	// Картина поля активности (размытая)
	picFieldA = gcnew System::Drawing::Bitmap(X_C, Y_C);
	// Картина релаксации
	picRelaxA = gcnew System::Drawing::Bitmap(X_C, Y_C);
	// Картина поля волны (размытая)
	picWaveA = gcnew System::Drawing::Bitmap(X_C, Y_C);
	
	// Картина поля активности, усредненного по времении (не используется)
	// picAAvg = gcnew System::Drawing::Bitmap(X_C, Y_C);

	//-----------------------------------------------------------------------------------------------
	// Егармин К.Н.: эти поля не используются:
	/*
	// Площадь поля усреднения активности
	SReceptF = (int)System::Math::Pow(RLAM * 2 + 1, 2);
	// Количество активных паттернов в режиме волны (int)
	NPC = 1;
	// Синапсы (int)
	N_Neurotransmitters = 2;
	// Время, отведенное на консолидацию (int)
	T_L1 = 50;
	*/
}


// Задание таблицы чувтвительности рецепторов
void Brain::Core::SetSens() {
	// Волновые нейроны (тип 0):
	// Медиатор аксона - A
	// внесинаптические рецепторы - A
	// синаптические рецепторы - B

	// Нейроны детекторы (тип 1):
	// Медиатор аксона - A, B
	// внесинаптические рецепторы - ---
	// синаптические рецепторы - A

	// Формат таблицы
	// тип передающего нейрона, тип принимающего, чувствительность внесинаптическая (0) - синаптическая (1)
	this->SensTable[0, 0, 0] = 1;
	this->SensTable[0, 0, 1] = 0;

	this->SensTable[0, 1, 0] = 0;
	this->SensTable[0, 1, 1] = 1;

	this->SensTable[1, 0, 0] = 1;
	this->SensTable[1, 0, 1] = 1;

	this->SensTable[1, 1, 0] = 0;
	// this->SensTable[1, 1, 1] = 0; // Комментарии (Егармин К.Н.): этот вариант не соответствует описанию выше
	this->SensTable[1, 1, 1] = 1; // Комментарии (Егармин К.Н.): а этот вариант соответствует

	// Комментарии (Егармин К.Н.):
	//
	// Формат таблицы
	// тип передающего нейрона, тип принимающего, вид рецептора: внесинаптический (0) или синаптический (1)
	//
	// Пример для случая, когда рецептор чувствует:
	// this->SensTable[тип передающего нейрона, тип принимающего, вид рецептора] = 1;
	//
	// Пример для случая, когда рецептор не чувствует:
	// this->SensTable[тип передающего нейрона, тип принимающего, вид рецептора] = 0;
} // Задание таблицы чувтвительности рецепторов. SetSens()


// Создание случайных паттернов вызванной активности
// Егармин К.Н.: однако они здесь еще не активируются, а просто помечаются как необучаемые входные аксоны
void Brain::Core::PatCreate() {
	//
	// Комментарии (Егармин К.Н.):
	//
	// Полярные координаты очередного нейрона в паттерне вызванной активности
	double Ang = 0;
	double R = 0;
	//
	// Комментарии (Егармин К.Н.):
	//
	// Положение очередного нейрона в паттерне вызванной активности относительно центра паттерна
	// в декартовой системе координат
	int dx = 0;
	int dy = 0;

	for (int i = 0; i < this->X_C; i++) {
		for (int j = 0; j < this->Y_C; j++) {
			this->pic->SetPixel(i, j, System::Drawing::Color::Black);
		}
	}

	for (int i = 0; i < this->NP; i++) {
		//
		// Комментарии (Егармин К.Н.):
		//
		//this->PatSet[i].ix = (this->X_C - 3 * this->R_Pat) * rnd.NextDouble() + this->R_Pat;
		//this->PatSet[i].iy = (this->Y_C - 3 * this->R_Pat) * rnd.NextDouble() + this->R_Pat;
		this->PatSet[i].ix = (int)((this->X_C - 6 * this->R_Pat) * this->rnd->NextDouble() + 3 * this->R_Pat);
		this->PatSet[i].iy = (int)((this->Y_C - 6 * this->R_Pat) * this->rnd->NextDouble() + 3 * this->R_Pat);
		
		this->PatSet[i].P = gcnew array<unsigned short, 2>(this->R_Pat * 2 + 1, this->R_Pat * 2 + 1);

		for (int j = 0; j < this->NNPat; j++) {
			Ang = 2 * System::Math::PI * this->rnd->NextDouble();
			R = this->R_Pat * this->rnd->NextDouble();

			// dx = R * System::Math::Sin(Ang);
			// dy = R * System::Math::Cos(Ang);
			//
			// Комментарии (Егармин К.Н.):
			//
			// Мне показалось, что так более корректно с точки зрения математики,
			// хотя это и не имеет значения (sin и cos поменял местами):
			dx = (int)( R * System::Math::Cos(Ang) );
			dy = (int)( R * System::Math::Sin(Ang) );

			this->PatSet[i].P[this->R_Pat + dx, this->R_Pat + dy] = 1;

			// Присвоение статуса входных волокон
			this->Cortex[this->PatSet[i].ix + dx, this->PatSet[i].iy + dy].StatusL = 5;
			//
			// Комментарии (Егармин К.Н.):
			//
			// 5 - не подлежит обучению (входной аксон)
		}

		// Егармин К.Н.: все NP паттернов вызванной активности отметить белыми пикселями
		/*
		for (int ix = 0; ix <= 2 * this->R_Pat; ix++) {
			for (int iy = 0; iy <= 2 * this->R_Pat; iy++) {
				if (this->PatSet[i].P[ix, iy] == 1) {
					this->pic->SetPixel(this->PatSet[i].ix - this->R_Pat + ix, this->PatSet[i].iy - this->R_Pat + iy, System::Drawing::Color::White);
				}
			}
		}
		*/
		// Егармин К.Н.: чтобы не вносить путаницы при разметке одного и того же разными цветами,
		// лучше после создания случайных паттернов вызванной активности просто вызвать
		// PatternsActivation(), в котором вызывается Draw_Status(). Здесь рисовать не будем
	}
} // Создание случайных паттернов вызванной активности. PatCreate()


// Расчет полей состояния коры
void Brain::Core::CalcFields() {
	int x = 0;
	int y = 0;
	int T = 0;
	float s = 0;

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			// Поле накопленной вызванной активности коры
			this->Field_A_Accum[ix, iy] = this->Field_A_Accum[ix, iy] * this->KA;
			// Поле мгновенной активности коры
			this->Field_A[ix, iy] = 0;
			// Поле волновой активности коры
			this->Field_A_Wave[ix, iy] = 0;
			// Поле вызванной активности коры
			this->Field_A_Evoced[ix, iy] = 0;

			for (int i = 0; i < this->N_Types; i++) {
				// Поле плотности релаксирующих нейронов каждого из типов
				this->Field_A_Relax[ix, iy, i] = 0;
			}
		}
	}

	for (int ix = this->RLAM; ix < this->X_C - this->RLAM; ix++) {
		for (int iy = this->RLAM; iy < this->Y_C - this->RLAM; iy++) {
			switch (this->Cortex[ix, iy].Status) {
				// 0 - спокоен // Комментарии (Егармин К.Н.): черный
				// 7 - состояние проверки на возможное участие в волновом идентификаторе

				case 1: // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
				case 6: // 6 - входной аксон
					for (int ix1 = 0; ix1 < this->NLocalAM; ix1++) {
						for (int iy1 = 0; iy1 < this->NLocalAM; iy1++) {
							x = ix + ix1 - this->RLAM;
							y = iy + iy1 - this->RLAM;

							s = this->LocalAM[ix1, iy1];
							
							// Поле накопленной вызванной активности коры
							this->Field_A_Accum[x, y] += s;
							// Поле мгновенной активности коры
							this->Field_A[x, y] += s;
							// Поле вызванной активности коры
							this->Field_A_Evoced[x, y] += s;
						}
					}
					break;
				
				case 2: // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
				case 3: // 3 - первый такт волновой активности
				case 5: // 5 - спонтанная активность // Комментарии (Егармин К.Н.): зеленый
					for (int ix1 = 0; ix1 < this->NLocalAM; ix1++) {
						for (int iy1 = 0; iy1 < this->NLocalAM; iy1++) {
							x = ix + ix1 - this->RLAM;
							y = iy + iy1 - this->RLAM;

							s = this->LocalAM[ix1, iy1];

							// Поле мгновенной активности коры
							this->Field_A[x, y] += s;
							// Поле волновой активности коры
							this->Field_A_Wave[x, y] += s;
						}
					}
					break;

				case 4: // 4 - релаксация после волнового спайка // Комментарии (Егармин К.Н.): синий
					T = this->Cortex[ix, iy].Type;
					// Комментарии (Егармин К.Н.):
					// T = 0 - входной аксон
					// T = 1 - нейрон первого уровня (волновой)
					// T = 2 - нейрон второго уровня
					for (int ix1 = 0; ix1 < this->NLocalAM; ix1++) {
						for (int iy1 = 0; iy1 < this->NLocalAM; iy1++) {
							x = ix + ix1 - this->RLAM;
							y = iy + iy1 - this->RLAM;

							s = this->LocalAM[ix1, iy1];

							// Поле плотности релаксирующих нейронов каждого из типов
							this->Field_A_Relax[x, y, T] += s;
							//
							// Комментарии (Егармин К.Н.):
							//
							// Получается, что релаксирующий нейрон влияет только на своих собратьев по типу?
						}
					}
					break;
			}
		}
	}
} // Расчет полей состояния коры. CalcFields()


// Обработка нейрона в состоянии покоя
void Brain::Core::S0(int %ix, int %iy) {
	int sum;
	int NN;
	//int NP;
	int P;
	
	// Выходим, если уровень активности вокруг нейрона мал и не считаем все возможные слои
	if (this->Field_A[ix, iy] < this->LAmin) {
		return;
	}

	// Если уровень окружающей активности высок, то проверяем на необходимость волнового возбуждения
	if (this->Cortex[ix, iy].ID->Count > 0) {
		for each (ID S in this->Cortex[ix, iy].ID) {
			sum = 0;
			NN = 0;
			//NP = 0;

			for (int ix1 = ix - this->R_ID; ix1 <= ix + this->R_ID; ix1++) {
				for (int iy1 = iy - this->R_ID; iy1 <= iy + this->R_ID; iy1++) {
					P = S.P[ix1 - (ix - this->R_ID), iy1 - (iy - this->R_ID)];
					// Учитывается соответсвие типов медиаторов и рецепторов
					/*
					sum += this->Cortex[ix1, iy1].AAccumExtraSinapse * 
						   P * 
						   this->SensTable[this->Cortex[ix1 - (ix - this->R_ID), iy1 - (iy - this->R_ID)].Type, this->Cortex[ix, iy].Type, 0];
				 	*/
					// Егармин К.Н.: мне кажется, что здесь должно быть так:
					sum += (int)( this->Cortex[ix1, iy1].AAccumExtraSinapse * 
								  P * 
								  this->SensTable[this->Cortex[ix1, iy1].Type, this->Cortex[ix, iy].Type, 0] );
								  // Егармин К.Н.: смотрим только внесинаптический рецептор (0)
					NN += P;
				}
			}

			if ( (sum / (double)NN) > this->L_act ) {
				// Если наступает волновое возбуждение разбираемся с консолидацией МРК
				if (S.ConsStatus == 0) { // 0 - не консолидирован
					S.NAct += 1;
					if (S.NAct > this->NCons) { // NCons - количество волн, необходимых для консолидации
						S.ConsStatus = 1; // 1 - консолидирован
					}
				}

				this->Cortex[ix, iy].Status = 3; // 3 - первый такт волновой активности
				
				// T - Временное событие
				// CT - Текущий такт
				// T_ID - Продолжительность пакета идентификационной активности
				this->Cortex[ix, iy].T = this->CT + this->T_ID;

				// для ускорения можно выйти из цикла, но это может нарушить консолидацию остальных слоев 
				
				//
				// Егармин К.Н.:
				// Не очень понимаю для чего это, но как-будто идет проверка того, что, если
				// выбран только один паттерн вызванной активности, то у нас нет картин идентификации от
				// других паттернов.
				// Зачем это проверять не знаю, это вроде бы итак очевидно, но может это для случая,
				// если мы во время моделирования стали менять галочки выбора паттернов вызванной активности
				//
				if (this->CheckedPatterns->Count == 1 && this->CheckedPatterns[0] != S.NP) {
					this->Cortex[ix, iy].ERR = true;
				}
			}
		} // for each
	}
} // Обработка нейрона в состоянии покоя. S0(int %ix, int %iy)


// Создание метаботропного рецептивного кластера (МК)
void Brain::Core::S1(int %ix, int %iy) {
	// Создаем образ окружающего паттерна (метаботропный рецептивный кластер)
	ID S = ID();

	S.ConsStatus = 0; // 0 - не консолидирован
	S.NAct = 1;
	S.TCreat = this->CT;
	S.P = gcnew array<unsigned short, 2>(this->R_ID * 2 + 1, this->R_ID * 2 + 1); // Массив, описывающий окружающую картину (паттерн)

	for (int ix1 = ix - this->R_ID; ix1 <= ix + this->R_ID; ix1++) {
		for (int iy1 = iy - this->R_ID; iy1 <= iy + this->R_ID; iy1++) {
			// учитывем чувствительность к медиатору
			/*
			S.P[ix1 - (ix - this->R_ID), iy1 - (iy - this->R_ID)] = this->Cortex[ix1, iy1].AAccumExtraSinapse *  // Накопленная за время обучения внесинаптических рецепторов активность
																	this->SensTable[this->Cortex[ix1 - (ix - this->R_ID), iy1 - (iy - this->R_ID)].Type, this->Cortex[ix, iy].Type, 0]; // Егармин К.Н.: смотрим только внесинаптический рецептор (0)
			*/
			// Егармин К.Н.: мне кажется, что здесь должно быть так:
			S.P[ix1 - (ix - this->R_ID), iy1 - (iy - this->R_ID)] = (unsigned short)( this->Cortex[ix1, iy1].AAccumExtraSinapse * // Накопленная за время обучения внесинаптических рецепторов активность
																					  this->SensTable[this->Cortex[ix1, iy1].Type, this->Cortex[ix, iy].Type, 0] ); // Егармин К.Н.: смотрим только внесинаптический рецептор (0)
		}
	}

	// Заполняем к какому паттерну вызванной активности относится этот МК (для контроля)
	if (this->CheckedPatterns->Count == 1) {
		S.NP = CheckedPatterns[0];
	}
	else {
		S.NP = 0;
	}

	this->Cortex[ix, iy].ID->Add(S);
} // Создание метаботропного рецептивного кластера (МК). S1(int %ix, int %iy)


// График суммарного ритма
void Brain::Core::DrawChart() {
	int YSize = 40;
	int Y = 0;
	double Amax = 0.05;
	pic2 = gcnew System::Drawing::Bitmap(this->NSer, YSize);

	int i = 0;
	for (int k = this->CurPosA + 1; k < this->NSer; k++) {
		Y = (int)( this->AWave[k] * (YSize - 1) / Amax );
		if (Y >= YSize - 1) {
			Y = YSize - 1;
		}
		pic2->SetPixel(i, YSize - Y - 1, System::Drawing::Color::Black);
		i++;
	}

	for (int k = 0; k <= this->CurPosA; k++) {
		Y = (int)( this->AWave[k] * (YSize - 1) / Amax );
		if (Y >= YSize - 1) {
			Y = YSize - 1;
		}
		pic2->SetPixel(i, YSize - Y - 1, System::Drawing::Color::Black);
		i++;
	}
} // График суммарного ритма. DrawChart()


// Картина общего состояния коры (статус нейронов)
void Brain::Core::Draw_Status() {
	System::Drawing::Pen ^Pen;
	int i1;
	int i2;
	
	for (int i = 0; i < this->X_C; i++) {
		for (int j = 0; j < this->Y_C; j++) {
			switch (this->Cortex[i, j].Status) {
				case 0: // Покой // 0 - спокоен // Комментарии (Егармин К.Н.): черный
					this->pic->SetPixel(i, j, System::Drawing::Color::Black);
					break;
				case 1: // Вызванная активность // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
					this->pic->SetPixel(i, j, System::Drawing::Color::Orange);
					break;
				case 2: // Спайки волны идентификатора // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
					this->pic->SetPixel(i, j, System::Drawing::Color::Gray);
					if (this->Cortex[i, j].ERR) {
						this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					}
					break;
				case 3: // Фронт волны идентификатора // 3 - первый такт волновой активности
					this->pic->SetPixel(i, j, System::Drawing::Color::White);
					if (this->Cortex[i, j].ERR) {
						this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					}
					break;
				case 4: // Нейроны в заторможенном состоянии // 4 - релаксация после волнового спайка // Комментарии (Егармин К.Н.): синий
					this->pic->SetPixel(i, j, System::Drawing::Color::DarkBlue);
					break;
				case 5: // Истинно спонтанный спайк // 5 - спонтанная активность // Комментарии (Егармин К.Н.): зеленый
					this->pic->SetPixel(i, j, System::Drawing::Color::Green);
					break;
				case 6: // Проекционная активность // 6 - входной аксон
					this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					break;
			}
		}
	}

	// Если есть волновой туннель, то рисование его границ
	// if ((this->Text == "Form1" && cortex2 != nullptr) || this->Text == "2")
	// Егармин К.Н.: думаю лучше так:
	if ( (this->cortex2 != nullptr) != (this->cortex0 != nullptr) ) {
		Pen = gcnew System::Drawing::Pen(System::Drawing::Color::White, 1);
		
		if ( this->cortex2 != nullptr ) {
			i1 = this->TX1;
			i2 = this->TY1;
		}
		else
		{
			i1 = this->TX2;
			i2 = this->TY2;
		}
		
		this->Gpic->DrawLine(Pen, i1, i2, i1 + TD, i2);
		this->Gpic->DrawLine(Pen, i1, i2 + TD, i1 + TD, i2 + TD);
		this->Gpic->DrawLine(Pen, i1, i2, i1, i2 + TD);
		this->Gpic->DrawLine(Pen, i1 + TD, i2, i1 + TD, i2 + TD);
	}
} // Картина общего состояния коры (статус нейронов). Draw_Status()


// Формирование серии картинок
void Brain::Core::addpic() {
	int s = this->CT - this->PicStart - 1; // Егармин К.Н.: номер картинки в серии, начина с нуля и кончая (NPic - 1)
	int Y = (int)System::Math::Floor( s / (double)this->WPic ); // Егармин К.Н.: номер строки, начиная с 0
	int X = s - Y * this->WPic; // Егармин К.Н.: номер столбца, начиная с 0
	X = X * X_C; // Егармин К.Н.: координаты нижнего левого угла очередной картинки в серии
	Y = Y * Y_C; //               на полотне большой картинки

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			this->Bigpic->SetPixel(X + ix, Y + iy, this->pic->GetPixel(ix, iy));
		}
	}

	if (s == this->NPic - 1) { // Егармин К.Н.: последняя ли картинака в серии (наборе)
		this->FPic = false;  // Егармин К.Н.: отрубаем формирование серии картинок
	}
} // Формирование серии картинок. addpic()


// Создать один такт волны
void Brain::Core::Wave() {
	double AW;
	int k;
	int sum;

	// Расчет полей состояния коры
	this->CalcFields();

	/*
	// Комментарии (Егармин К.Н.): этот блок перенесен в отдельную функцию в объекте (модуле) Формы
	// Отображение выбранного поля активности
	switch (ComboBox1->SelectedIndex) {
		case 0:
			Draw_ActAccum(); // акт. накопленная (размытая)
			break;
		case 1:
			Draw_FieldA(); // акт. моментальная (размытая)
			break;
		case 2:
			Draw_EvokedA(); // акт. вызванная
			break;
		case 3:
			Draw_WaveA(); // акт. волновая
			break;
		case 4:
			Draw_RelaxA(); // уровень релаксации
			break;
	}
	*/

	// Определяем переходы состояний нейронов
	for (int ix = this->R_ID; ix < this->X_C - this->R_ID; ix++) {
		for (int iy = this->R_ID; iy < this->Y_C - this->R_ID; iy++) {
			switch (this->Cortex[ix, iy].Status) {
				case 0: // покой // 0 - спокоен // Комментарии (Егармин К.Н.): черный
					// определяем узнавание идентификатора, но не корректируем CortexA, а только меняем статус
					this->S0(ix, iy);
					
					// Если возбуждения нет
					if (this->Cortex[ix, iy].Status == 0) {
						// уровень окружающей активности высок, но еще не достиг верхнего порога, 
						if (this->Field_A[ix, iy] > this->LAmin && // Поле мгновенной активности коры
							this->Field_A_Wave[ix, iy] < this->LAmax && // Поле волновой активности коры
							this->Field_A_Relax[ix, iy, this->Cortex[ix, iy].Type] < this->LImin[this->Cortex[ix, iy].Type]) { // Поле плотности релаксирующих нейронов каждого из типов
							// а блокированных нейронов вокруг мало, то
							if (this->rnd->NextDouble() < this->Psp) {
								// создаем кандидата на добавление в идентификатор
								this->Cortex[ix, iy].Status = 7; // 7 - состояние проверки на возможное участие в волновом идентификаторе
								this->Cortex[ix, iy].T = this->CT + this->T_ExtraSinapseLearn;
							}
						}
					}
					break;
				case 7: // 7 - состояние проверки на возможное участие в волновом идентификаторе
					// определяем узнавание идентификатора, но не корректируем CortexA, а только меняем статус
					this->S0(ix, iy);
					// Егармин К.Н.: если статус не изменился
					if (this->Cortex[ix, iy].Status == 7) {
						if (this->Field_A_Wave[ix, iy] > this->LAmax) {// Поле волновой активности коры
							// Сбрасываем состояние кандидата, если активность превышает порог
							this->Cortex[ix, iy].Status = 0;
						}
						else if (this->CT > this->Cortex[ix, iy].T) {
							// вызываем спонтанную активность и проводим ID обучение 
							this->Cortex[ix, iy].Status = 5; // 5 - спонтанная активность // Комментарии (Егармин К.Н.): зеленый
							this->Cortex[ix, iy].T = this->CT;
							// Создание метаботропного рецептивного кластера
							this->S1(ix, iy);
						}
					}
					break;
				case 1: // вызванная активность // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
					// Сбрасываем вызванную активность по окончании пакета
					if (this->CT > this->Cortex[ix, iy].T) { // Егармин К.Н.: если нейрон был возбужден, то теперь может перейти в состояние покоя
						this->Cortex[ix, iy].Status = 0;
					}
					break;
				case 2: // пачка волновой активности // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
					// если пачка закончилась, то переводим в состояние нечувствительности
					if (this->CT > this->Cortex[ix, iy].T) {
						// задаем время релаксации							 // тем больше, чем сильнее рядом вызванная активность
						this->Cortex[ix, iy].T = this->CT + this->T_Passive; // * (1 + Field_A_Accum(ix, iy) / 0.02)
						this->Cortex[ix, iy].Status = 4; // 4 - релаксация после волнового спайка // Комментарии (Егармин К.Н.): синий
						// Егармин К.Н.: про ошибки я вообще не понимаю
						this->Cortex[ix, iy].ERR = false;
					}
					break;
				case 3: // первый такт волновой активности // 3 - первый такт волновой активности
					this->Cortex[ix, iy].Status = 2; // пачка волновой активности // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
					break;
				case 4: // состояние нечувствительности // 4 - релаксация после волнового спайка // Комментарии (Егармин К.Н.): синий
					if (this->CT > this->Cortex[ix, iy].T) { // Егармин К.Н.: если релаксация прошла, то
						this->Cortex[ix, iy].Status = 0;     // переходим в состояние покоя
					}
					break;
				case 5: // спонтанный спайк // 5 - спонтанная активность // Комментарии (Егармин К.Н.): зеленый
					this->Cortex[ix, iy].Status = 2; // пачка волновой активности // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
					break;
				//case 6: break; // Проекционная (входная) активность // 6 - входной аксон
			}
		}
	}

	// Создаем картину активности нового временного слоя, соответсвующую статусам
	AW = 0; // тип double
	// Сдвигаем текщий временной слой
	this->CurLayer += 1;
	if (this->CurLayer == this->T_SinapseLearn) {
		this->CurLayer = 0;
	}
	
	for (int ix = this->R_ID; ix < this->X_C - this->R_ID; ix++) {
		for (int iy = this->R_ID; iy < this->Y_C - this->R_ID; iy++) {
			switch (this->Cortex[ix, iy].Status) {
				case 1: // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
				case 2: // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
				case 3: // 3 - первый такт волновой активности
				case 5: // 5 - спонтанная активность // Комментарии (Егармин К.Н.): зеленый
				case 6: // 6 - входной аксон
					this->CortexA[ix, iy, this->CurLayer] = 1; // Набор полей последних состояний нейронов.
					AW += 1;
					break;
				default: // 0 - спокоен // Комментарии (Егармин К.Н.): черный
						 // 4 - релаксация после волнового спайка // Комментарии (Егармин К.Н.): синий
						 // 7 - состояние проверки на возможное участие в волновом идентификаторе
					this->CortexA[ix, iy, this->CurLayer] = 0; // Набор полей последних состояний нейронов.
					break;
			}
		}
	}

	// График ритма
	this->CurPosA += 1;
	if (this->CurPosA == this->NSer) {
		this->CurPosA = 0;
	}
	// Суммарная ритмическая активность
	this->AWave[this->CurPosA] = AW / ((this->X_C - 2 * this->R_ID) * (this->Y_C - 2 * this->R_ID));
	
	// График суммарного ритма
	this->DrawChart();
	// Проверяем на консолидацию или удаление идентификационных следов
	// Егармин К.Н.: перерисовать основную картинку
	this->Draw_Status();

	this->CT += 1;

	if (this->FPic) {
		this->addpic();
	}

	// Установка последовательности слоев (0 - текущий, 1 - предыдущий и т.д.)
	k = this->CurLayer; // тип int
	for (int i = 0; i < this->T_SinapseLearn; i++) {
		this->LayersSequence[i] = k;
		k -= 1;
		if (k < 0) {
			k = T_SinapseLearn - 1;
		}
	}

	// Расчет накопленной по времени активности
	for (int ix = this->R_ID; ix < this->X_C - this->R_ID; ix++) {
		for (int iy = this->R_ID; iy < this->Y_C - this->R_ID; iy++) {
			sum = 0; // тип int
			
			// Егармин К.Н.: похоже, что AAccumExtraSinapse и AAccumSinapse - просто флаги того, что
			// последние T_ExtraSinapseLearn и T_SinapseLearn (T_SinapseLearn > T_ExtraSinapseLearn)
			// тактов, соответственно, нейрон находился хоть в каком-то поле активности.
			//
			// Егармин К.Н.: у Редозубова почему-то эти флаги выставлены вещественными числами,
			// а не Byte, как он обычно делает для флагов. Почему?
			
			/*
			for (int i = 0; i < this->T_ExtraSinapseLearn; i++)
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			if (sum > 0)
				this->Cortex[ix, iy].AAccumExtraSinapse = 1;
			else
				this->Cortex[ix, iy].AAccumExtraSinapse = 0;
			
			for (int i = this->T_ExtraSinapseLearn; i < this->T_SinapseLearn; i++) {
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			if (sum > 0)
				this->Cortex[ix, iy].AAccumSinapse = 1;
			else
				this->Cortex[ix, iy].AAccumSinapse = 0;
			*/

			// Егармин К.Н.: код выше несколько не эффективен, лучше записать так:
			this->Cortex[ix, iy].AAccumExtraSinapse = 0;
			this->Cortex[ix, iy].AAccumSinapse = 0;
			
			for (int i = 0; i < this->T_ExtraSinapseLearn; i++) {
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			}
			if (sum > 0) {
				this->Cortex[ix, iy].AAccumExtraSinapse = 1;
				this->Cortex[ix, iy].AAccumSinapse = 1;
				continue;
			}									
			
			for (int i = this->T_ExtraSinapseLearn; i < this->T_SinapseLearn; i++) {
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			}
			if (sum > 0) {
				this->Cortex[ix, iy].AAccumSinapse = 1;
			}
		}
	}
	
	// Обучение
	if (this->FLearn) {
		// Расчет вызванной активности
		this->Calc_Evoked_Activity();

		//this->MakePicFieldA();

		// Обучение синаптических весов. Задание фасилитации
		this->LearnW1();
		
		// Отмена фасилитации, исходя из условий выживания
		// this->LearnW2();
	}

	// Проекция по волновому туннелю
	this->TMove();
} // Создать один такт волны. Wave()


// Расчет вызванной активности
void Brain::Core::Calc_Evoked_Activity() {
	float x1;
	float x2;
	float sum;
	float n1;
	float n2;

	for (int ix = this->R_W; ix < this->X_C - this->R_W; ix++) {
		for (int iy = this->R_W; iy < this->Y_C - this->R_W; iy++) {
			this->Cortex[ix, iy].EvokedA = 0; // Вызванный потенциал активности нейрона в мощностном измерении
			if (this->Cortex[ix, iy].StatusL > 0) { // Текущий статус обучения нейрона
				// 0 - свободен; 1 - фасилитация; 2 - обучен; 5 - не подлежит обучению (входной аксон)
				//
				// Егармин К.Н.: он что, учит даже нейроны со статусом обучения 5, которые не подлежат обучению?
				//
				sum = 0;
				n1 = 0;
				n2 = 0;

				for (int ix1 = 0; ix1 <= 2 * this->R_W; ix1++) {
					for (int iy1 = 0; iy1 <= 2 * this->R_W; iy1++) {
						x1 = this->Cortex[ix + ix1 - this->R_W, iy + iy1 - this->R_W].AAccumSinapse;
						x2 = this->CortexW[ix, iy, ix1, iy1];

						sum += x1 * x2;

						n1 += x1;
						n2 += x2;
					}
				}

				if (sum > 0) {
					this->Cortex[ix, iy].EvokedA = (float)( sum / System::Math::Sqrt(n1 * n2) );
				}

				/*
				if (this->Cortex[ix, iy].EvokedA > this->L_Spike) {
					if (this->Cortex[ix, iy].Status != 1) // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
						this->Cortex[ix, iy].T = this->CT + this->T_EnvokedA;
					this->Cortex[ix, iy].Status = 1;
				}
				*/
				// Егармин К.Н.: чуть эффективнее будет так:
				if (this->Cortex[ix, iy].EvokedA > this->L_Spike && this->Cortex[ix, iy].Status != 1) {
					this->Cortex[ix, iy].T = this->CT + this->T_EnvokedA;
					this->Cortex[ix, iy].Status = 1; // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
				}
			}
		}
	}
} // Расчет вызванной активности. Calc_Evoked_Activity()


// Обучение синаптических весов. Задание фасилитации
void Brain::Core::LearnW1() {
	for (int ix = this->R_W; ix < this->X_C - this->R_W; ix++) {
		for (int iy = this->R_W; iy < this->Y_C - this->R_W; iy++) {
			if (this->Cortex[ix, iy].Status == 2 && // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
				this->Field_A_Accum[ix, iy] > this->L_ActL1 && // Нужно, чтобы поле накопленной (окружающей) вызванной активности коры превысило порог L_ActL1
				this->Field_A[ix, iy] > this->L_ActLS && // Нужно, чтобы поле мгновенной активности коры превысило порог L_ActLS
				this->Cortex[ix, iy].StatusL == 0) { // 0 - свободен
					
					this->Cortex[ix, iy].StatusL = 1; // 1 - фасилитация
					this->Cortex[ix, iy].TL = this->CT; // TL - временное событие обучения

					// Фиксация кандидата в прототип
					for (int ix1 = 0; ix1 <= 2 * this->R_W; ix1++) {
						for (int iy1 = 0; iy1 <= 2 * this->R_W; iy1++) {
							this->CortexW[ix, iy, ix1, iy1] = Cortex[ix + ix1 - this->R_W, iy + iy1 - this->R_W].AAccumSinapse; //* SensTable(Cortex(ix + ix1 - R_W, iy + iy1 - R_W).Type, Cortex(ix, iy).Type, 1)
						}
					}
			}
		}
	}
} // Обучение синаптических весов. Задание фасилитации. LearnW1()


// Картина состояния обучения
void Brain::Core::Draw_Learn() {
	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			switch (this->Cortex[ix, iy].StatusL) {
				case 0: // 0 - свободен
					this->pic1->SetPixel(ix, iy, System::Drawing::Color::Black);
					break;
				case 1: // 1 - фасилитация
					if (this->Cortex[ix, iy].Status == 1) { // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
						// Field_A_Accum - поле накопленной (окружающей) вызванной активности коры
						if (this->Field_A_Accum[ix, iy] < this->L_GameLive_Low) {
							this->pic1->SetPixel(ix, iy, System::Drawing::Color::Blue);
						}
						else if (this->Field_A_Accum[ix, iy] >= this->L_GameLive_Low && 
								 this->Field_A_Accum[ix, iy] < this->L_GameLive_high) {
							this->pic1->SetPixel(ix, iy, System::Drawing::Color::Green);
						}
						else if (this->Field_A_Accum[ix, iy] >= this->L_GameLive_high) {
							this->pic1->SetPixel(ix, iy, System::Drawing::Color::Red);
						}
					}
					else {
						this->pic1->SetPixel(ix, iy, System::Drawing::Color::Yellow);
					}
					break;
				case 2: // 2 - обучен
					if (this->Cortex[ix, iy].Status == 1) { // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
						this->pic1->SetPixel(ix, iy, System::Drawing::Color::White);
					}
					else {
						this->pic1->SetPixel(ix, iy, System::Drawing::Color::DarkBlue);
					}
					break;
				case 5: // 5 - не подлежит обучению (входной аксон)
					this->pic1->SetPixel(ix, iy, System::Drawing::Color::DarkMagenta); // Егармин К.Н.: фиолетовый - этим цветом отмечены начальные паттерны вызванной активности
					break;
			}
		}
	}
} // Картина состояния обучения. Draw_Learn()


// Проекция по волновому туннелю
void Brain::Core::TMove() {
	if (this->cortex2 == nullptr) { // Ничего не делать, если туннель не создан
		return;
	}

	for (int ix = 0; ix < this->TD; ix++) { // TD - размеры туннеля
		for (int iy = 0; iy < this->TD; iy++) {
			// Координаты угла исходящего туннеля TX1, TY1
			// Координаты угла входящего туннеля TX2, TY2
			this->cortex2->Cortex[ix + this->TX2, iy + this->TY2].Status = this->Cortex[ix + this->TX1, iy + this->TY1].Status;
			this->cortex2->CortexA[ix + this->TX2, iy + this->TY2, this->cortex2->CurLayer] = this->CortexA[ix + this->TX1, iy + this->TY1, this->CurLayer];
			this->cortex2->Cortex[ix + this->TX2, iy + this->TY2].T = this->Cortex[ix + this->TX1, iy + this->TY1].T;
		}
	}
	
	this->cortex2->Wave();
} // Проекция по волновому туннелю. TMove()


// Картина вызванной активности
void Brain::Core::Draw_EvokedA() {
	int Y; // Цвет в R=G=B
			
	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Evoced[ix, iy] / 0.08 * 255 ); // Field_A_Evoced - поле вызванной активности коры
			if (Y > 255) {
				Y = 255;
			}
			this->picEvokedA->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // Картина вызванной активности. Draw_EvokedA()


// Картина накопленной активности
void Brain::Core::Draw_ActAccum() {
	int Y; // Цвет в R=G=B
	
	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Accum[ix, iy] * 500 ); // Field_A_Accum - поле накопленной вызванной активности коры
			if (Y > 255) {
				Y = 255;
			}
			this->picActAccum->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // Картина накопленной активности. Draw_ActAccum()


// Картина поля активности (размытая)
void Brain::Core::Draw_FieldA() {
	int Y; // Цвет в R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A[ix, iy] / 0.08 * 255 ); // Field_A - поле мгновенной активности коры
			if (Y > 255) {
				Y = 255;
			}
			this->picFieldA->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // Картина поля активности (размытая). Draw_FieldA()


// Картина релаксации
void Brain::Core::Draw_RelaxA(int ntype) {
	int Y; // Цвет в R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Relax[ix, iy, ntype] / 0.04 * 255 ); // Field_A_Relax - поле плотности релаксирующих нейронов каждого из типов
			if (Y > 255) {
				Y = 255;
			}
			this->picRelaxA->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // Картина релаксации. Draw_RelaxA(int ntype)


// Картина поля волны (размытая)
void Brain::Core::Draw_WaveA() {
	int Y; // Цвет в R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Wave[ix, iy] / 0.04 * 255 ); // Field_A_Wave - поле волновой активности коры
			if (Y > 255) {
				Y = 255;
			}
			this->picWaveA->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // Картина поля волны (размытая). Draw_WaveA()


// Активация входных паттернов (при изменении перечня паттернов вызванной активности)
// Егармин К.Н.: метод оставляет только паттерны вызванной активности с соответствующими статусами нейронов
void Brain::Core::PatternsActivation() {
	for (int i = 0; i < this->X_C; i++) {
		for (int j = 0; j < this->Y_C; j++) {
			this->Cortex[i, j].Status = 0; // 0 - спокоен // Комментарии (Егармин К.Н.): черный
			this->CortexA[i, j, this->CurLayer] = 0; // Набор полей последних состояний нейронов
		}
	}
	
	this->ActivePatternsRefresh(); // Обновление состава активных входных аксонов
} // Активация входных паттернов (при наведении на перечень паттернов вызванной активности). PatternsActivation()


// Обновление состава активных входных аксонов
void Brain::Core::ActivePatternsRefresh() {
	for (int item = 0; item < this->NP; item++) {	
		for (int ix = 0; ix <= 2 * this->R_Pat; ix++) {
			for (int iy = 0; iy <= 2 * this->R_Pat; iy++) {
				if (this->PatSet[item].P[ix, iy] == 1) {
					// Егармин К.Н.: Если данный паттерн выбран как входной аксон, но до этого он не был им, то:
					if (this->CheckedPatterns->Contains(item) && this->Cortex[this->PatSet[item].ix - this->R_Pat + ix, this->PatSet[item].iy - this->R_Pat + iy].Status != 6) {
						this->Cortex[this->PatSet[item].ix - this->R_Pat + ix, this->PatSet[item].iy - this->R_Pat + iy].Status = 6; // делаем его входным аксоном (красный) (6)
					}
					// Егармин К.Н.: Если данный паттерн не выбран как входной аксон, но до этого он был им, то:
					else if (!this->CheckedPatterns->Contains(item) && this->Cortex[this->PatSet[item].ix - this->R_Pat + ix, this->PatSet[item].iy - this->R_Pat + iy].Status == 6) {
						if (this->CT != 0) {
							this->Cortex[this->PatSet[item].ix - this->R_Pat + ix, this->PatSet[item].iy - this->R_Pat + iy].Status = 4; // переводим в состояние релаксации (синий) (4)
						}
						else { // Егармин К.Н.: для случая, когда только начали моделирование:
							this->Cortex[this->PatSet[item].ix - this->R_Pat + ix, this->PatSet[item].iy - this->R_Pat + iy].Status = 0; // переводим в состояние покоя (черный) (0)
						}
					}
					// Егармин К.Н.: В остальных случаях ничего не меняется
				}
			}
		}
	}
				
	// Егармин К.Н.: перерисовать основную картинку
	this->Draw_Status();
	// Егармин К.Н.: нарисовать номера входных аксонов
	this->Draw_Numbers();
} // Обновление состава активных входных аксонов. ActivePatternsRefresh()


//-----------------------------------------------------------------------------------------------
// Картина поля активности, усредненного по времении (этот метод не используется)
/*
void Brain::Core::MakePicAAvg() {
	int Y; // Цвет в R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = this->Cortex[ix, iy].AAccumSinapse * 255; //AAccumSinapse - накопленная за время обучения синапсов активность
			this->picAAvg->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // Картина поля активности, усредненного по времении (этот метод не используется). MakePicAAvg()
*/


// Рисование номеров входных аксонов
void Brain::Core::Draw_Numbers() {
	System::Drawing::SolidBrush ^brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Yellow);
	System::Drawing::Font ^font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);

	for (int i = 0; i < this->NP; i++) {
		this->Gpic->DrawString(i.ToString(), font, brush, (float)this->PatSet[i].ix, (float)this->PatSet[i].iy);
	}
} // Рисование номеров входных аксонов. Draw_Numbers()