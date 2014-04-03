#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




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