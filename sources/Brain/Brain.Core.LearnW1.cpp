#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




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