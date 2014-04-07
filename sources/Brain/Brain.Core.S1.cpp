#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




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