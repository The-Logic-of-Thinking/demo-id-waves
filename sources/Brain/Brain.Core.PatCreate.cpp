#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"





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