#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




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