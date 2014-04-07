#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




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